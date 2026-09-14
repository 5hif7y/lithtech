Attribute VB_Name = "Image"
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
' These are actions that a user could perform via the Photoshop Image
' menu pull down.

Option Explicit
'16 or 8
Function ConvertModeDepth(NewDepth As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    'build the descriptor
    descriptor.PutInteger phKeyDepth, NewDepth
    
    'play the event and get back the result
    Set result = gControl.Play(phEventConvertMode, descriptor, phDialogSilent)

    'now check for errors
    ConvertModeDepth = CheckPlayResult(result)
End Function

Function FlattenImage() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    'play the event and get back the result
    Set result = gControl.Play(phEventFlattenImage, Nothing, phDialogSilent)
    
    'now check for errors
    FlattenImage = CheckPlayNonResult(result)
End Function

'phclassRGBColorMode phclassCMYKColorMode phclassLabColorMode phclassMultichannelMode
'phclassBitmapMode phclassgrayscalemode phclassindexedcolormode phclassduotonemode
Function ConvertMode(NewMode As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    
     On Error Resume Next
    
   ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor

    'build the descriptor
    descriptor.PutClass phKeyTo, NewMode
    
    'play the event and get back the result
    Set result = gControl.Play(phEventConvertMode, descriptor, phDialogSilent)
    
    'now check for errors
    ConvertMode = CheckPlayResult(result)
End Function
Function NewColorTable(red() As Double, green() As Double, blue() As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    Dim list As IActionList
    Dim Index As Integer
    
    On Error Resume Next
    
    ' create the descriptor and other objects
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    Set list = PSApp.Maklist
     
    'build the reference
    reference.PutProperty phClassColor, phKeyColorTable
    
    'build the descriptor
    descriptor.PutReference phKeyNull, reference
          
    For Index = 1 To 256
        object.PutFloat phKeyRed, red(Index)
        object.PutFloat phKeyGreen, green(Index)
        object.PutFloat phKeyBlue, blue(Index)
        list.PutObject phClassRGBColor, object
        object.Clear
    Next Index
    
    descriptor.PutList phKeyTo, list
    
    'play the event and get back the result
    Set result = gControl.Play(phEventSet, descriptor, phDialogSilent)
    
    'now check for errors
    NewColorTable = CheckPlayResult(result)
End Function
Function DuplicateDocument(NewName As String) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
     
    'build the reference
    reference.PutEnumerated phClassDocument, phTypeOrdinal, phEnumTarget
    
    'build the descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutString phKeyName, NewName
    
    'play the event and get back the result
    Set result = gControl.Play(phEventDuplicate, descriptor, phDialogSilent)
    
    'now check for errors
    DuplicateDocument = CheckPlayResult(result)
End Function

Function ImageSize(Width As Double, Height As Double, ConstrainProportions As Boolean) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor

    'build the descriptor
    If ConstrainProportions Then
        descriptor.PutUnitDouble phKeyWidth, phUnitDistance, Width
        descriptor.PutBoolean phKeyConstrainProportions, True
    Else
        descriptor.PutUnitDouble phKeyWidth, phUnitDistance, Width
        descriptor.PutUnitDouble phKeyHeight, phUnitDistance, Height
    End If
    
    descriptor.PutEnumerated phKeyInterfaceIconFrameDimmed, phTypeInterpolation, phEnumNearestNeighbor
    
    'play the event and get back the result
    Set result = gControl.Play(phEventImageSize, descriptor, phDialogSilent)
    
    'now check for errors
    ImageSize = CheckPlayResult(result)
End Function

Function CanvasSize(Width As Double, Height As Double, HorizontalLocation As Long, VerticalLocation As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    'build the descriptor
    descriptor.PutUnitDouble phKeyWidth, phUnitPixels, Width
    descriptor.PutUnitDouble phKeyWidth, phUnitPixels, Height
    descriptor.PutEnumerated phKeyHorizontal, phTypeHorizontalLocation, HorizontalLocation
    descriptor.PutEnumerated phKeyVertical, phTypeVerticalLocation, VerticalLocation
    
    'play the event and get back the result
    Set result = gControl.Play(phEventCanvasSize, descriptor, phDialogSilent)
    
    'now check for errors
    CanvasSize = CheckPlayResult(result)
End Function

Function Crop() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    'play the event and get back the result
    Set result = gControl.Play(phEventCrop, Nothing, phDialogSilent)

    'now check for errors
    Crop = CheckPlayNonResult(result)
End Function
Function Rotate(Angle As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
     
    'build the reference
    reference.PutEnumerated phClassDocument, phTypeOrdinal, phEnumTarget
    
    'build the descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutUnitDouble phKeyAngle, phUnitAngle, Angle
    
    'play the event and get back the result
    Set result = gControl.Play(phEventRotate, descriptor, phDialogSilent)
    
    'now check for errors
    Rotate = CheckPlayResult(result)
End Function
' takes either phEnumHorizontal or phEnumVertical
Function Flip(Axis As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    'build the reference
    reference.PutEnumerated phClassLayer, phTypeOrdinal, phEnumTarget
    
    'build the descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutEnumerated phKeyAxis, phTypeOrientation, Axis
    
    'play the event and get back the result
    Set result = gControl.Play(phEventFlip, descriptor, phDialogSilent)
    
    'now check for errors
    Flip = CheckPlayResult(result)
End Function


