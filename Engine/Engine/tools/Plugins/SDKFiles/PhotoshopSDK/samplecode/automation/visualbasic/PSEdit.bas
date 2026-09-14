Attribute VB_Name = "Edit"
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
' These are actions that a user could perform via the Photoshop Edit
' menu pull down.

Option Explicit

Function Cut() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    'play the event and get the result
    Set result = gControl.Play(phEventCut, Nothing, phDialogSilent)
    
    'now check for errors
    Cut = CheckPlayNonResult(result)
End Function

Function Paste() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    'play the event and get the result
    Set result = gControl.Play(phEventPaste, Nothing, phDialogSilent)
    
    'now check for errors
    Paste = CheckPlayNonResult(result)
End Function


Function Copy() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    'play the event and get the result
    Set result = gControl.Play(phEventCopy, Nothing, phDialogSilent)
    
    'now check for errors
    Copy = CheckPlayNonResult(result)
End Function

Function CopyMerged() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    'play the event and get the result
    Set result = gControl.Play(phEventCopyMerged, Nothing, phDialogSilent)
    
    'now check for errors
    CopyMerged = CheckPlayNonResult(result)
End Function

Function PasteInto() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    'play the event and get the result
    Set result = gControl.Play(phEventPasteInto, Nothing, phDialogSilent)
    
    'now check for errors
    PasteInto = CheckPlayNonResult(result)
End Function

Function Fill(Using As Long, Opacity As Double, Mode As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    'build the descriptor
    descriptor.PutEnumerated phKeyUsing, phTypeFillContents, Using
    descriptor.PutUnitDouble phKeyOpacity, phUnitPercent, Opacity
    descriptor.PutEnumerated phKeyMode, phTypeBlendMode, Mode
    
    'play the event and get the result
    Set result = gControl.Play(phEventFill, descriptor, phDialogSilent)
    
    'now check for errors
    Fill = CheckPlayResult(result)
End Function

Function Stroke(Width As Long, Location As Long, Opacity As Double, Mode As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    'build the descriptor
    descriptor.PutInteger phKeyWidth, Width
    descriptor.PutEnumerated phKeyLocation, phTypeStrokeLocation, Location
    descriptor.PutUnitDouble phKeyOpacity, phUnitPercent, Opacity
    descriptor.PutEnumerated phKeyMode, phTypeBlendMode, Mode

    'play the event and get the result
    Set result = gControl.Play(phEventStroke, descriptor, phDialogSilent)
    
    'now check for errors
    Stroke = CheckPlayResult(result)
End Function

Function Transform(PositionHorizontal As Double, PositionVertical As Double, OffsetHorizontal As Double, OffsetVertical As Double, SkewHorizontal As Double, SkewVertical As Double, UsingHorizontal As Double, UsingVertical As Double, Relative As Boolean, ConstrainProportions As Boolean, Angle As Double, Height As Double, Width As Double, FreeTransformCenterState As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
     On Error Resume Next
    
   ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    'build the reference
    reference.PutEnumerated phClassLayer, phTypeOrdinal, phEnumTarget
    
    'build the descriptor
    descriptor.PutReference phKeyNull, reference
    
    ' if this key is 0 then we are doing a numeric transform
    If FreeTransformCenterState = 0 Then
        descriptor.PutBoolean phKeyRelative, Relative
        descriptor.PutBoolean phKeyConstrainProportions, ConstrainProportions
    Else
        descriptor.PutEnumerated phKeyFreeTransformCenterState, phTypeQuadCenterState, FreeTransformCenterState
    End If
    
    If Not PositionHorizontal Or Not PositionVertical Then
        object.PutUnitDouble phKeyHorizontal, phUnitPixels, PositionHorizontal
        object.PutUnitDouble phKeyVertical, phUnitPixels, PositionVertical
        descriptor.PutObject phKeyPosition, phClassPoint, object
        object.Clear
    End If
    
    If Not OffsetHorizontal Or Not OffsetVertical Then
        object.PutUnitDouble phKeyHorizontal, phUnitPixels, OffsetHorizontal
        object.PutUnitDouble phKeyVertical, phUnitPixels, OffsetVertical
        descriptor.PutObject phKeyOffset, phClassOffset, object
        object.Clear
    End If
    
    If Not SkewHorizontal Or Not SkewVertical Then
        object.PutUnitDouble phKeyHorizontal, phUnitAngle, SkewHorizontal
        object.PutUnitDouble phKeyVertical, phUnitAngle, SkewVertical
        descriptor.PutObject phKeySkew, phClassPoint, object
        object.Clear
    End If
    
    If Not UsingHorizontal Or Not UsingVertical Then
        object.PutUnitDouble phKeyHorizontal, phUnitPercent, UsingHorizontal
        object.PutUnitDouble phKeyVertical, phUnitPercent, UsingVertical
        descriptor.PutObject phKeyUsing, phClassPoint, object
        object.Clear
    End If
    
    If Not Angle Then
        descriptor.PutUnitDouble phKeyAngle, phUnitAngle, Angle
    End If
        
    If ConstrainProportions Then
        descriptor.PutBoolean phKeyConstrainProportions, ConstrainProportions
        descriptor.PutUnitDouble phKeyHeight, phUnitPercent, Height
    Else
        descriptor.PutUnitDouble phKeyHeight, phUnitPercent, Height
        descriptor.PutUnitDouble phKeyWidth, phUnitPercent, Width
    End If

    'play the event and get the result
    Set result = gControl.Play(phEventTransform, descriptor, phDialogSilent)
    
    'now check for errors
    Transform = CheckPlayResult(result)
End Function




Function DefinePattern() As Long
    Dim result As IActionDescriptor
    
     On Error Resume Next
    
   'play the event and get the result
    Set result = gControl.Play(phEventDefinePattern, Nothing, phDialogSilent)
    
    'now check for errors
    DefinePattern = CheckPlayNonResult(result)
End Function

' This function can take
' phEnumUndo, phEnumClipboard, phEnumPattern, phEnumHistory, phEnumAll
Function Purge(Item As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    
     On Error Resume Next
    
   ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    'build the descriptor
    descriptor.PutEnumerated phKeyNull, phTypePurgeItem, Item
    
    'play the event and get the result
    Set result = gControl.Play(phEventPurge, descriptor, phDialogSilent)
    
    'now check for errors
    'Purge undo, clipboard,histories,  returns nothing, ODD
    'purge pattern, all is ok
    If Item = phEnumAll Or Item = phEnumClipboard Then
        Purge = CheckPlayResult(result)
    Else
        Purge = CheckPlayNonResult(result)
    End If
End Function



