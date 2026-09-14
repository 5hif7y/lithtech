Attribute VB_Name = "Color"
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
Option Explicit

Function SetColorRGB(foreground As Boolean, red As Double, green As Double, blue As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim colorDescriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set colorDescriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    'build the reference
    If (foreground) Then
        reference.PutProperty phClassColor, phKeyForegroundColor
    Else
        reference.PutProperty phClassColor, phKeyBackgroundColor
    End If
    
    'build the descriptor
    colorDescriptor.PutDouble phKeyRed, red
    colorDescriptor.PutDouble phKeyGreen, green
    colorDescriptor.PutDouble phKeyBlue, blue
    
    ' stuff into main descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutObject phKeyTo, phClassRGBColor, colorDescriptor
        
    'play the event and get the result
    Set result = gControl.Play(phEventSet, descriptor, phDialogSilent)
    
    'now check for errors
    SetColorRGB = CheckPlayNonResult(result)

End Function
Function SetColorHSB(foreground As Boolean, hue As Double, hueUnits As Long, saturation As Double, brightness As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim colorDescriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set colorDescriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    'build the reference
    If (foreground) Then
        reference.PutProperty phClassColor, phKeyForegroundColor
    Else
        reference.PutProperty phClassColor, phKeyBackgroundColor
    End If
    
    'build the descriptor
    colorDescriptor.PutUnitDouble phKeyHue, hueUnits, hue
    colorDescriptor.PutDouble phKeySaturation, saturation
    colorDescriptor.PutDouble phKeyBrightness, brightness
    
    ' stuff into main descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutObject phKeyTo, phClassHSBColor, colorDescriptor
        
    'play the event and get the result
    Set result = gControl.Play(phEventSet, descriptor, phDialogSilent)
    
    'now check for errors
    SetColorHSB = CheckPlayNonResult(result)

End Function
Function SetColorCMYK(foreground As Boolean, cyan As Double, magenta As Double, yellow As Double, black As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim colorDescriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objects
    Set descriptor = PSApp.MakeDescriptor
    Set colorDescriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    'build the reference
    If (foreground) Then
        reference.PutProperty phClassColor, phKeyForegroundColor
    Else
        reference.PutProperty phClassColor, phKeyBackgroundColor
    End If
    
    'build the descriptor
    colorDescriptor.PutDouble phKeyCyan, cyan
    colorDescriptor.PutDouble phKeyMagenta, magenta
    colorDescriptor.PutDouble phKeyYellow, yellow
    colorDescriptor.PutDouble phKeyBlack, black
    
    ' stuff into main descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutObject phKeyTo, phClassCMYKColor, colorDescriptor
        
    'play the event and get the result
    Set result = gControl.Play(phEventSet, descriptor, phDialogSilent)
    
    'now check for errors
    SetColorCMYK = CheckPlayNonResult(result)

End Function

Function SetColorLAB(foreground As Boolean, luminance As Double, a As Double, b As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim colorDescriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the objects
    Set descriptor = PSApp.MakeDescriptor
    Set colorDescriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    'build the reference
    If (foreground) Then
        reference.PutProperty phClassColor, phKeyForegroundColor
    Else
        reference.PutProperty phClassColor, phKeyBackgroundColor
    End If
    
    'build the descriptor
    colorDescriptor.PutDouble phKeyLuminance, luminance
    colorDescriptor.PutDouble phKeyA, a
    colorDescriptor.PutDouble phKeyB, b
    
    ' stuff into main descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutObject phKeyTo, phClassLabColor, colorDescriptor
        
    'play the event and get the result
    Set result = gControl.Play(phEventSet, descriptor, phDialogSilent)
    
    'now check for errors
    SetColorLAB = CheckPlayNonResult(result)

End Function

Function GetColor(foreground As Boolean, colorClass As Long, color1 As Double, color2 As Double, color3 As Double, color4 As Double) As Long
    Dim result As IActionDescriptor
    Dim whatever As Variant
    Dim hasKey As Long
    
    On Error Resume Next
    
    colorClass = -1
    color1 = -1
    color2 = -1
    color3 = -1
    color4 = -1
    GetColor = 0
    
    On Error Resume Next
    
    If (foreground) Then
        Set result = GetInfoFromPhotoshop(phClassApplication, phKeyForegroundColor)
    Else
        Set result = GetInfoFromPhotoshop(phClassApplication, phKeyBackgroundColor)
    End If
         
    If Not result Is Nothing Then
    
        result.hasKey phKeyRed, hasKey
        
        If hasKey Then
            'get the RGB color out
            colorClass = phClassRGBColor
            result.GetDouble phKeyRed, color1
            result.GetDouble phKeyGreen, color2
            result.GetDouble phKeyBlue, color3
        End If
        
        result.hasKey phKeyCyan, hasKey
        
        If hasKey Then
            'get the CMYK color out
            colorClass = phClassCMYKColor
            result.GetDouble phKeyCyan, color1
            result.GetDouble phKeyMagenta, color2
            result.GetDouble phKeyYellow, color3
            result.GetDouble phKeyBlack, color4
        End If
        
        result.hasKey phKeyLuminance, hasKey
        
        If hasKey Then
            'get the Lab color out
            colorClass = phClassLabColor
            result.GetDouble phKeyLuminance, color1
            result.GetDouble phKeyA, color2
            result.GetDouble phKeyB, color3
        End If
        
        result.hasKey phKeyHue, hasKey
        
        If hasKey Then
            'get the HSB color out
            colorClass = phClassHSBColor
            result.GetDouble phKeyHue, color1
            result.GetDouble phKeySaturation, color2
            result.GetDouble phKeyBrightness, color3
        End If
    
        If colorClass = -1 Then
            GetColor = -1
        End If
    Else
        GetColor = -1
    End If
    
End Function
