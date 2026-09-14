Attribute VB_Name = "Filter"
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
' These are actions that a user could perform via the Photoshop Filter
' menu pull down.

Option Explicit

Function GaussianBlur(Radius As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    Dim hasKey As Boolean
    Dim ErrorString As String

    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' put the information in the descriptor
    descriptor.PutDouble phKeyRadius, Radius
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventGaussianBlur, descriptor, phDialogSilent)

    ' now check for errors
    GaussianBlur = CheckPlayResult(result)
End Function

Function ColoredPencil(Width As Long, Pressure As Long, brightness As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    Dim hasKey As Boolean

    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' put the information in the descriptor
    descriptor.PutInteger phKeyPencilWidth, Width
    descriptor.PutInteger phKeyStrokePressure, Pressure
    descriptor.PutInteger phKeyPaperBrightness, brightness
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventColoredPencil, descriptor, phDialogSilent)

    ' now check for errors
    ColoredPencil = CheckPlayResult(result)
End Function
Function Blur() As Long
    Dim result As IActionDescriptor
    Dim hasKey As Boolean
    
    On Error Resume Next
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventBlur, Nothing, phDialogSilent)

    ' now check for errors
    Blur = CheckPlayNonResult(result)
End Function
Function BlurMore() As Long
    Dim result As IActionDescriptor
    Dim hasKey As Boolean
    
    On Error Resume Next
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventBlurMore, Nothing, phDialogSilent)

    ' now check for errors
    BlurMore = CheckPlayNonResult(result)
End Function
Function AccentedEdges(Width As Long, brightness As Long, Smoothness As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    Dim hasKey As Boolean

    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' put the information in the descriptor
    descriptor.PutInteger phKeyEdgeWidth, Width
    descriptor.PutInteger phKeyEdgeBrightness, brightness
    descriptor.PutInteger phKeySmoothness, Smoothness
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventAccentedEdges, descriptor, phDialogSilent)

    ' now check for errors
    AccentedEdges = CheckPlayResult(result)
End Function
Function DiffuseGlow(Graininess As Long, Glow As Long, Clear As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    Dim hasKey As Boolean

    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' put the information in the descriptor
    descriptor.PutInteger phKeyGraininess, Graininess
    descriptor.PutInteger phKeyGlowAmount, Glow
    descriptor.PutInteger phKeyClearAmount, Clear
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventDiffuseGlow, descriptor, phDialogSilent)

    ' now check for errors
    DiffuseGlow = CheckPlayResult(result)
End Function
Function AddNoise(Amount As Long, Distortion As Long, Monochromatic As Boolean) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    Dim hasKey As Boolean

    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' put the information in the descriptor
    descriptor.PutInteger phKeyAmount, Amount
    descriptor.PutEnumerated phKeyDistortion, phTypeDistribution, Distortion
    descriptor.PutBoolean phKeyMonochromatic, Monochromatic
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventAddNoise, descriptor, phDialogSilent)

    ' now check for errors
    AddNoise = CheckPlayResult(result)
End Function
Function ColorHalftone(Radius As Long, Angle1 As Long, Angle2 As Long, Angle3 As Long, Angle4 As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    Dim hasKey As Boolean

    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' put the information in the descriptor
    descriptor.PutInteger phKeyRadius, Radius
    descriptor.PutInteger phKeyAngle1, Angle1
    descriptor.PutInteger phKeyAngle2, Angle2
    descriptor.PutInteger phKeyAngle3, Angle3
    descriptor.PutInteger phKeyAngle4, Angle4
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventColorHalftone, descriptor, phDialogSilent)

    ' now check for errors
    ColorHalftone = CheckPlayResult(result)
End Function
Function Clouds() As Long
    Dim result As IActionDescriptor
    Dim hasKey As Boolean
    
    On Error Resume Next
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventClouds, Nothing, phDialogSilent)

    ' now check for errors
    Clouds = CheckPlayNonResult(result)
End Function
Function Sharpen() As Long
    Dim result As IActionDescriptor
    Dim hasKey As Boolean
    
    On Error Resume Next
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventSharpen, Nothing, phDialogSilent)

    ' now check for errors
    Sharpen = CheckPlayNonResult(result)
End Function
Function UnsharpMask(Radius As Double, Amount As Long, Threshold As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    Dim hasKey As Boolean

    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' put the information in the descriptor
    descriptor.PutDouble phKeyRadius, Radius
    descriptor.PutInteger phKeyAmount, Amount
    descriptor.PutInteger phKeyThreshold, Threshold
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventUnsharpMask, descriptor, phDialogSilent)

    ' now check for errors
    UnsharpMask = CheckPlayResult(result)
End Function
Function BasRelief(Detail As Long, Smoothness As Long, LightDirection As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    Dim hasKey As Boolean

    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' put the information in the descriptor
    descriptor.PutInteger phKeyDetail, Detail
    descriptor.PutInteger phKeySmoothness, Smoothness
    descriptor.PutEnumerated phKeyLightDirection, phTypeLightDirection, LightDirection
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventBasRelief, descriptor, phDialogSilent)

    ' now check for errors
    BasRelief = CheckPlayResult(result)
End Function
Function Diffuse(Mode As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    Dim hasKey As Boolean

    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' put the information in the descriptor
    descriptor.PutEnumerated phKeyMode, phTypeDiffuseMode, Mode
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventDiffuse, descriptor, phDialogSilent)

    ' now check for errors
    Diffuse = CheckPlayResult(result)
End Function
Function Craquelure(Spacing As Long, Depth As Long, brightness As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    Dim hasKey As Boolean

    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' put the information in the descriptor
    descriptor.PutInteger phKeyCrackSpacing, Spacing
    descriptor.PutInteger phKeyCrackDepth, Depth
    descriptor.PutInteger phKeyCrackBrightness, brightness
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventCraquelure, descriptor, phDialogSilent)

    ' now check for errors
    Craquelure = CheckPlayResult(result)
End Function
Function DeInterlace(Eliminate As Long, Create As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    Dim hasKey As Boolean

    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' put the information in the descriptor
    descriptor.PutEnumerated phKeyInterlaceEliminateType, phTypeInterlaceEliminateType, Eliminate
    descriptor.PutEnumerated phKeyInterlaceCreateType, phTypeInterlaceCreateType, Create
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventDeInterlace, descriptor, phDialogSilent)

    ' now check for errors
    DeInterlace = CheckPlayResult(result)
End Function
Function HighPass(Radius As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    Dim hasKey As Boolean

    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' put the information in the descriptor
    descriptor.PutDouble phKeyRadius, Radius
    
    ' play the event and get back what really happened
    Set result = gControl.Play(phEventHighPass, descriptor, phDialogSilent)

    ' now check for errors
    HighPass = CheckPlayResult(result)
End Function
