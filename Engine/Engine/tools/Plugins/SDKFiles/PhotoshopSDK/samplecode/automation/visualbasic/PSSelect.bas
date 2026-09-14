Attribute VB_Name = "Select"
'// ADOBE SYSTEMS INCORPORATED",
'// Copyright  1993 - 2002 Adobe Systems Incorporated",
'// All Rights Reserved",
'//",
'// NOTICE:  Adobe permits you to use, modify, and distribute this ",
'// file in accordance with the terms of the Adobe license agreement",
'// accompanying it.  If you have received this file from a source", 
'// other than Adobe, then your use, modification, or distribution",
'// of it requires the prior written permission of Adobe.");
'-------------------------------------------------------------------
' These are actions that a user could perform via the Photoshop Select
' menu pull down.

' Added generic selection routines to select objects: Documents, Layers, Paths, Channels
Option Explicit
Function SelectEllipse(ellipseTop As Double, ellipseLeft As Double, ellipseBottom As Double, ellipseRight As Double, AntiAlias As Boolean) As Long
    Dim descriptor As IActionDescriptor
    Dim ellipseDesc As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    Set reference = PSApp.MakeReference
    Set descriptor = PSApp.MakeDescriptor
    Set ellipseDesc = PSApp.MakeDescriptor
    
    'build the reference
    reference.PutProperty phClassChannel, phKeySelection
    
    descriptor.PutReference phKeyNull, reference
    
    ellipseDesc.PutUnitDouble phKeyTop, phUnitPixels, ellipseTop
    ellipseDesc.PutUnitDouble phKeyLeft, phUnitPixels, ellipseLeft
    ellipseDesc.PutUnitDouble phKeyBottom, phUnitPixels, ellipseBottom
    ellipseDesc.PutUnitDouble phKeyRight, phUnitPixels, ellipseRight
    
    descriptor.PutObject phKeyTo, phClassEllipse, ellipseDesc
    descriptor.PutBoolean phKeyAntiAlias, AntiAlias
    
    'play the event and get back the result
    Set result = gControl.Play(phEventSet, descriptor, phDialogSilent)
    
    'now check for errors
    SelectEllipse = CheckPlayResult(result)
End Function

Function SelectPolygon(EventValue As Long, Horizontal() As Double, Vertical() As Double, PointCount As Long, AntiAlias As Boolean) As Long
    Dim descriptor As IActionDescriptor
    Dim pointDesc As IActionDescriptor
    Dim listDesc As IActionDescriptor
    Dim list As IActionList
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    Set reference = PSApp.MakeReference
    Set descriptor = PSApp.MakeDescriptor
    Set pointDesc = PSApp.MakeDescriptor
    Set listDesc = PSApp.MakeDescriptor
    Set list = PSApp.MakeList
    Dim counter As Long
    
    'build the reference
    reference.PutProperty phClassChannel, phKeySelection
    
    descriptor.PutReference phKeyNull, reference
    
    ' add the list of click points
    For counter = 1 To PointCount
        pointDesc.PutUnitDouble phKeyHorizontal, phUnitPixels, Horizontal(counter)
        pointDesc.PutUnitDouble phKeyVertical, phUnitPixels, Vertical(counter)
        list.PutObject phClassPoint, pointDesc
    Next counter
    
    listDesc.PutList phKeyPoints, list
    descriptor.PutObject phKeyTo, phClassPolygon, listDesc
    descriptor.PutBoolean phKeyAntiAlias, AntiAlias
    
    'play the event and get back the result
    Set result = gControl.Play(EventValue, descriptor, phDialogSilent)
    
    'now check for errors
    SelectPolygon = CheckPlayResult(result)
End Function


Function SelectRectangle(Top As Double, Left As Double, Bottom As Double, Right As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    'build the reference
    reference.PutProperty phClassChannel, phKeySelection
    
    'reference goes in the main descriptor
    descriptor.PutReference phKeyNull, reference
     
    ' build the rectangle object
    object.PutUnitDouble phKeyTop, phUnitPixels, Top
    object.PutUnitDouble phKeyLeft, phUnitPixels, Left
    object.PutUnitDouble phKeyBottom, phUnitPixels, Bottom
    object.PutUnitDouble phKeyRight, phUnitPixels, Right
    
    'rectangle object goes in the main descriptor
    descriptor.PutObject phKeyTo, phClassRectangle, object

    'play the event and get back the result
    Set result = gControl.Play(phEventSet, descriptor, phDialogSilent)
    
    'now check for errors
    SelectRectangle = CheckPlayResult(result)
End Function

Function SelectAll() As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
     
    On Error Resume Next
    
    ' Make the descriptor, reference and objects
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    ' build the reference
    reference.PutProperty phClassChannel, phKeySelection

    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutEnumerated phKeyTo, phTypeOrdinal, phEnumAll
    
    'play this event and get back the result
    Set result = gControl.Play(phEventSet, descriptor, phDialogSilent)

    ' now check for errors
    SelectAll = CheckPlayResult(result)
End Function
Function ColorRange(Fuzziness As Long, MinL As Double, MinA As Double, MinB As Double, MaxL As Double, MaxA As Double, MaxB As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim result As IActionDescriptor
     
    On Error Resume Next
    
    ' Make the descriptor, object and objects
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    
    ' build the descriptor
    ' give the minimum and maximum colors in LAB values
    object.PutDouble phKeyLuminance, MinL
    object.PutDouble phKeyA, MinA
    object.PutDouble phKeyB, MinB
     
    'put this object in the main descriptor
    descriptor.PutObject phKeyMinimum, phClassLabColor, object
    
    'clear out the descriptor
    object.Clear
     
    ' give the minimum and maximum colors in LAB values
    object.PutDouble phKeyLuminance, MaxL
    object.PutDouble phKeyA, MaxA
    object.PutDouble phKeyB, MaxB

    'put this object in the main descriptor
    descriptor.PutObject phKeyMaximum, phClassLabColor, object
    descriptor.PutInteger phKeyFuzziness, Fuzziness
    
    'play this event and get back the result
    Set result = gControl.Play(phEventColorRange, descriptor, phDialogSilent)

    ' now check for errors
    ' color range event doesn't return anything, ODD
    ColorRange = CheckPlayNonResult(result)
End Function
Function SelectInverse() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventInverse, Nothing, phDialogSilent)

    ' now check for errors
    SelectInverse = CheckPlayNonResult(result)
End Function
Function Feather(Radius As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
     
    On Error Resume Next
    
    ' Make the descriptor, objectDesc and objects
    Set descriptor = PSApp.MakeDescriptor
    
    ' build the descriptor
    descriptor.PutUnitDouble phKeyRadius, phUnitPixels, Radius
    
    ' play the event and get the result
    Set result = gControl.Play(phEventFeather, descriptor, phDialogSilent)

    ' now check for errors
    Feather = CheckPlayResult(result)
End Function
Function Border(Width As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
     
    On Error Resume Next
    
    ' Make the descriptor, objectDesc and objects
    Set descriptor = PSApp.MakeDescriptor
    
    ' build the descriptor
    descriptor.PutUnitDouble phKeyWidth, phUnitPixels, Width
    
    ' play the event and get the result
    Set result = gControl.Play(phEventBorder, descriptor, phDialogSilent)

    ' now check for errors
    Border = CheckPlayResult(result)
End Function
Function Smooth(Radius As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
     
    On Error Resume Next
    
    ' Make the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' build the descriptor
    descriptor.PutUnitDouble phKeyRadius, phUnitPixels, Radius
    
    ' play the event and get the result
    Set result = gControl.Play(phEventSmooth, descriptor, phDialogSilent)

    ' now check for errors
    Smooth = CheckPlayResult(result)
End Function
Function Expand(Pixels As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
     
    On Error Resume Next
    
    ' Make the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' build the descriptor
    descriptor.PutUnitDouble phKeyBy, phUnitPixels, Pixels
    
    ' play the event and get the result
    Set result = gControl.Play(phEventExpand, descriptor, phDialogSilent)

    ' now check for errors
    Expand = CheckPlayResult(result)
End Function
Function Contract(Pixels As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
     
    On Error Resume Next
    
    ' Make the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' build the descriptor
    descriptor.PutUnitDouble phKeyBy, phUnitPixels, Pixels
    
    ' play the event and get the result
    Set result = gControl.Play(phEventContract, descriptor, phDialogSilent)

    ' now check for errors
    Contract = CheckPlayResult(result)
End Function
Function Grow(Tolerance As Long, AntiAlias As Boolean) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
     
    On Error Resume Next
    
    ' Make the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    ' build the reference
    reference.PutProperty phClassChannel, phKeySelection

    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutInteger phKeyTolerance, Tolerance
    descriptor.PutBoolean phKeyAntiAlias, AntiAlias
    
    ' play the event and get the result
    Set result = gControl.Play(phEventGrow, descriptor, phDialogSilent)

    ' now check for errors
    Grow = CheckPlayResult(result)
End Function
Function Similar(Tolerance As Long, AntiAlias As Boolean) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
     
    On Error Resume Next
    
    ' Make the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    ' build the reference
    reference.PutProperty phClassChannel, phKeySelection

    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutInteger phKeyTolerance, Tolerance
    descriptor.PutBoolean phKeyAntiAlias, AntiAlias
    
    ' play the event and get the result
    Set result = gControl.Play(phEventSimilar, descriptor, phDialogSilent)

    ' now check for errors
    Similar = CheckPlayResult(result)
End Function
Function SelectTransparentLayer(Invert As Boolean) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim toReference As IActionReference
    Dim result As IActionDescriptor
     
    On Error Resume Next
    
    ' Make the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set toReference = PSApp.MakeReference
    Set reference = PSApp.MakeReference
    
    ' build the from reference
    reference.PutProperty phClassChannel, phKeySelection
    
    ' build the to reference
     toReference.PutEnumerated phClassChannel, phTypeChannel, phEnumTransparency

    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutReference phKeyTo, toReference
    descriptor.PutBoolean phKeyInvert, Invert
    
    ' play the event and get the result
    Set result = gControl.Play(phEventSet, descriptor, phDialogSilent)

    ' now check for errors
    SelectTransparentLayer = CheckPlayResult(result)
End Function
Function SelectBackgroundLayer() As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
     
    'build the reference
    reference.PutProperty phClassLayer, phKeyBackground
    
    'build the descriptor
    descriptor.PutReference phKeyNull, reference
        
    ' play the event and get the result
    Set result = gControl.Play(phEventSelect, descriptor, phDialogSilent)
        
    'now check for errors
    SelectBackgroundLayer = CheckPlayResult(result)
End Function
    
Function SelectLayerByName(layerName As String) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
     
    'build the reference
    reference.PutName phClassLayer, layerName

    'build the descriptor
    descriptor.PutReference phKeyNull, reference
        
    ' play the event and get the result
    Set result = gControl.Play(phEventSelect, descriptor, phDialogSilent)
        
    'now check for errors
    SelectLayerByName = CheckPlayResult(result)
End Function

Function SelectLayerByIndex(layerIndex As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
     
    'build the reference
    reference.PutIndex phClassLayer, layerIndex

    'build the descriptor
    descriptor.PutReference phKeyNull, reference
        
    ' play the event and get the result
    Set result = gControl.Play(phEventSelect, descriptor, phDialogSilent)
        
    'now check for errors
    SelectLayerByIndex = CheckPlayResult(result)
End Function

'Takes the composite, phenumCMYK, or one of the individual colors
'phenumBlack, phenumCyan, phenumYellow, phenumMagenta
'phenumRGB, phenumRed, phenumGreen, phenumBlue
' etc...
Function SelectChannelByColor(Color As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
     
    'build the reference
    reference.PutEnumerated phClassChannel, phTypeChannel, Color
    
    'build the descriptor
    descriptor.PutReference phKeyNull, reference
        
    ' play the event and get the result
    Set result = gControl.Play(phEventSelect, descriptor, phDialogSilent)
        
    'now check for errors
    SelectChannelByColor = CheckPlayResult(result)
End Function
    
Function SelectChannelByName(ChannelName As String) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
     
    'build the reference
    reference.PutName phClassChannel, ChannelName

    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
        
    ' play the event and get the result
    Set result = gControl.Play(phEventSelect, descriptor, phDialogSilent)
        
    'now check for errors
    SelectChannelByName = CheckPlayResult(result)
End Function

Function SelectDocumentByIndex(Index As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
     
    'build the reference
    reference.PutIndex phClassDocument, Index

    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
        
    ' play the event and get the result
    Set result = gControl.Play(phEventSelect, descriptor, phDialogSilent)
        
    'now check for errors
    SelectDocumentByIndex = CheckPlayResult(result)
End Function
    
Function SelectDocumentByName(DocumentName As String) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
     
    'build the reference
    reference.PutName phClassDocument, DocumentName

    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
        
    ' play the event and get the result
    Set result = gControl.Play(phEventSelect, descriptor, phDialogSilent)
        
    'now check for errors
    SelectDocumentByName = CheckPlayResult(result)
End Function
    
Function SelectDocumentByOffset(Offset As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
     
    'build the reference
    reference.PutOffset phClassDocument, Offset

    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
        
    ' play the event and get the result
    Set result = gControl.Play(phEventSelect, descriptor, phDialogSilent)
        
    'now check for errors
    SelectDocumentByOffset = CheckPlayResult(result)
End Function
    
Function SelectWorkPath() As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
     
    'build the reference
    reference.PutProperty phClassPath, phKeyWorkPath

    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
    
    ' play the event and get the result
    Set result = gControl.Play(phEventSelect, descriptor, phDialogSilent)
        
    'now check for errors
    SelectWorkPath = CheckPlayResult(result)
End Function
    
Function SelectPathByName(PathName As String) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
     
    'build the reference
    reference.PutName phClassPath, PathName
    
    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
        
    ' play the event and get the result
    Set result = gControl.Play(phEventSelect, descriptor, phDialogSilent)
        
    'now check for errors
    SelectPathByName = CheckPlayResult(result)
End Function
    

Function DeSelectPath() As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
     
    'build the reference
    reference.PutClass phClassPath
    
    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
    
    ' play the event and get the result
    Set result = gControl.Play(phEventDeselect, descriptor, phDialogSilent)

    'now check for errors
    DeSelectPath = CheckPlayResult(result)
End Function

Function DuplicateSelectionAsChannel(Name As String) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
     
    On Error Resume Next
    
    ' Make the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    ' build the reference
    reference.PutProperty phClassChannel, phKeySelection

    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutString phKeyName, Name
    
    ' play the event and get the result
    Set result = gControl.Play(phEventDuplicate, descriptor, phDialogSilent)
    
    ' now check for errors
    DuplicateSelectionAsChannel = CheckPlayResult(result)
End Function
'Must have a path selected
Function DuplicatePath(Name As String) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
     
    On Error Resume Next
    
    ' Make the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    ' build the reference
    reference.PutEnumerated phClassPath, phTypeOrdinal, phEnumTarget

    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutString phKeyName, Name
    
    ' play the event and get the result
    Set result = gControl.Play(phEventDuplicate, descriptor, phDialogSilent)
    
    ' now check for errors
    DuplicatePath = CheckPlayResult(result)
End Function

Function SelectNothing() As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
     
    On Error Resume Next
    
    ' Make the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    ' build the reference
    reference.PutProperty phClassChannel, phKeySelection

    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutEnumerated phKeyTo, phTypeOrdinal, phEnumNone
    
    ' play the event and get the result
    Set result = gControl.Play(phEventSet, descriptor, phDialogSilent)

    ' now check for errors
    SelectNothing = CheckPlayResult(result)
End Function
Function SelectPrevious() As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
     
    On Error Resume Next
    
    ' Make the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    ' build the reference
    reference.PutProperty phClassChannel, phKeySelection

    ' build the descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutEnumerated phKeyTo, phTypeOrdinal, phEnumPrevious
    
    ' play the event and get the result
    Set result = gControl.Play(phEventSet, descriptor, phDialogSilent)

    ' now check for errors
    SelectPrevious = CheckPlayResult(result)
End Function

