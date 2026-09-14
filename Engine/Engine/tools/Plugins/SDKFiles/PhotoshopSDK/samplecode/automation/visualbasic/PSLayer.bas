Attribute VB_Name = "Layer"
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
' These are actions that a user could perform via the Photoshop Layer
' menu pull down.

Option Explicit

Function MakeTypeLayer(HorizontalPosition As Double, VerticalPosition As Double, Text As String, FontName As String, Alignment As Long, Size As Double, AntiAlias As Boolean, AutoKern As Boolean) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    Dim reference As IActionReference
    
    Dim textDescriptor As IActionDescriptor
    Dim clickPointDescriptor As IActionDescriptor
    Dim textShapeDescriptor As IActionDescriptor
    Dim textStyleDescriptor As IActionDescriptor
    Dim fontDescriptor As IActionDescriptor
    Dim fontColorDescriptor As IActionDescriptor
    Dim paraDescriptor As IActionDescriptor
    Dim paraStyleDescriptor As IActionDescriptor
    Dim warpDescriptor As IActionDescriptor
    
    Dim textShapeList As IActionList
    Dim textStyleList As IActionList
    Dim paraStyleList As IActionList
    
    
    Dim DocWidth As Double
    Dim DocHeight As Double
    Dim RuntimeKeyID As Long
    Dim RuntimeTypeID As Long
    Dim RuntimeEnumID As Long
    Dim RuntimeObjID As Long
    Dim RuntimeListID As Long
    Dim RuntimeClassID As Long
                
    On Error Resume Next
    
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    Set textDescriptor = PSApp.MakeDescriptor
    Set clickPointDescriptor = PSApp.MakeDescriptor
    Set textShapeDescriptor = PSApp.MakeDescriptor
    Set textStyleDescriptor = PSApp.MakeDescriptor
    Set fontDescriptor = PSApp.MakeDescriptor
    Set fontColorDescriptor = PSApp.MakeDescriptor
    Set paraDescriptor = PSApp.MakeDescriptor
    Set paraStyleDescriptor = PSApp.MakeDescriptor
    Set warpDescriptor = PSApp.MakeDescriptor
    
    Set textShapeList = PSApp.MakeList
    Set textStyleList = PSApp.MakeList
    Set paraStyleList = PSApp.MakeList
    

    reference.PutClass phClassTextLayer
    descriptor.PutReference phKeyNull, reference
     
    textDescriptor.PutString phKeyText, Text
          
    clickPointDescriptor.PutUnitDouble phKeyHorizontal, phUnitPercent, 12.8968
    clickPointDescriptor.PutUnitDouble phKeyVertical, phUnitPercent, 49.7222
    textDescriptor.PutObject phKeyTextClickPoint, phClassPoint, clickPointDescriptor
          
    gControl.StringIDToTypeID "textGridding", RuntimeKeyID
    gControl.StringIDToTypeID "textGridding", RuntimeTypeID
          
    textDescriptor.PutEnumerated RuntimeKeyID, RuntimeTypeID, phEnumNone
    textDescriptor.PutEnumerated phKeyOrientation, phTypeOrientation, phEnumHorizontal
    textDescriptor.PutEnumerated phKeyAntiAlias, phTypeAntiAlias, phEnumAntiAliasCrisp
          
    textShapeDescriptor.PutEnumerated 1413830740, phTypeChar, 1349415968
                    
    gControl.StringIDToTypeID "textShape", RuntimeClassID
             
    textShapeList.PutObject RuntimeClassID, textShapeDescriptor
               
    gControl.StringIDToTypeID "textShape", RuntimeKeyID
    textDescriptor.PutList RuntimeKeyID, textShapeList
          
    textStyleDescriptor.PutInteger phKeyFrom, 0
    textStyleDescriptor.PutInteger phKeyTo, Len(Text) + 1
                    
    gControl.StringIDToTypeID "syntheticBold", RuntimeKeyID
    fontDescriptor.PutBoolean RuntimeKeyID, False
                         
    gControl.StringIDToTypeID "syntheticItalic", RuntimeKeyID
    fontDescriptor.PutBoolean RuntimeKeyID, False
                         
    gControl.StringIDToTypeID "fontPostScriptNameStr", RuntimeKeyID
    fontDescriptor.PutString RuntimeKeyID, "Helvetica"
                         
    fontDescriptor.PutString phKeyFontName, "Helvetica"
    fontDescriptor.PutString phKeyFontStyleName, "Regular"
    fontDescriptor.PutInteger phKeyFontScript, 0
    fontDescriptor.PutInteger phKeyFontTechnology, 0
    fontDescriptor.PutDouble phKeySizeKey, 144
    
    fontColorDescriptor.PutDouble phKeyRed, 0
    fontColorDescriptor.PutDouble phKeyGreen, 0
    fontColorDescriptor.PutDouble phKeyBlue, 0
    fontDescriptor.PutObject phKeyColor, phClassRGBColor, fontColorDescriptor
    
    gControl.StringIDToTypeID "autoLeading", RuntimeKeyID
    fontDescriptor.PutBoolean RuntimeKeyID, True
    fontDescriptor.PutInteger phKeyHorizontalScale, 100
    fontDescriptor.PutInteger phKeyVerticalScale, 100
    fontDescriptor.PutInteger phKeyTracking, 0
    fontDescriptor.PutInteger phKeyKerning, 0
    fontDescriptor.PutBoolean phKeyAutoKern, True
    fontDescriptor.PutDouble phKeyBaselineShift, 0
    gControl.StringIDToTypeID "fontCaps", RuntimeKeyID
    gControl.StringIDToTypeID "fontCaps", RuntimeTypeID
    fontDescriptor.PutEnumerated RuntimeKeyID, RuntimeTypeID, phEnumNormal
    gControl.StringIDToTypeID "baseline", RuntimeKeyID
    gControl.StringIDToTypeID "baseline", RuntimeTypeID
    fontDescriptor.PutEnumerated RuntimeKeyID, RuntimeTypeID, phEnumNormal
    fontDescriptor.PutBoolean phKeyUnderline, False
    gControl.StringIDToTypeID "verticalUnderlinePosition", RuntimeKeyID
    gControl.StringIDToTypeID "verticalUnderlineRight", RuntimeEnumID
    gControl.StringIDToTypeID "verticalUnderlinePosition", RuntimeTypeID
    fontDescriptor.PutEnumerated RuntimeKeyID, RuntimeTypeID, RuntimeEnumID
    gControl.StringIDToTypeID "strikethrough", RuntimeKeyID
    fontDescriptor.PutBoolean RuntimeKeyID, False
    gControl.StringIDToTypeID "ligature", RuntimeKeyID
    fontDescriptor.PutBoolean RuntimeKeyID, True
    gControl.StringIDToTypeID "oldStyle", RuntimeKeyID
    fontDescriptor.PutBoolean RuntimeKeyID, False
    gControl.StringIDToTypeID "proportionalNumbers", RuntimeKeyID
    fontDescriptor.PutBoolean RuntimeKeyID, False
    fontDescriptor.PutBoolean phKeyRotate, True
    gControl.StringIDToTypeID "baselineDirection", RuntimeKeyID
    gControl.StringIDToTypeID "withStream", RuntimeEnumID
    gControl.StringIDToTypeID "baselineDirection", RuntimeTypeID
    fontDescriptor.PutEnumerated RuntimeKeyID, RuntimeTypeID, RuntimeEnumID
    gControl.StringIDToTypeID "mojiZume", RuntimeKeyID
    fontDescriptor.PutDouble RuntimeKeyID, 0
    gControl.StringIDToTypeID "gridAlignment", RuntimeKeyID
    gControl.StringIDToTypeID "roman", RuntimeEnumID
    gControl.StringIDToTypeID "gridAlignment", RuntimeTypeID
    fontDescriptor.PutEnumerated RuntimeKeyID, RuntimeTypeID, RuntimeEnumID
    gControl.StringIDToTypeID "wariChuCount", RuntimeKeyID
    fontDescriptor.PutInteger RuntimeKeyID, 1
    gControl.StringIDToTypeID "wariChuScale", RuntimeKeyID
    fontDescriptor.PutDouble RuntimeKeyID, 1
    gControl.StringIDToTypeID "wariChuWidow", RuntimeKeyID
    fontDescriptor.PutInteger RuntimeKeyID, 25
    gControl.StringIDToTypeID "wariChuOrphan", RuntimeKeyID
    fontDescriptor.PutInteger RuntimeKeyID, 25
    gControl.StringIDToTypeID "noBreak", RuntimeKeyID
    fontDescriptor.PutBoolean RuntimeKeyID, False
    
    textStyleDescriptor.PutObject phKeyTextStyle, phClassTextStyle, fontDescriptor
    
    textStyleList.PutObject phClassTextStyleRange, textStyleDescriptor
    
    textDescriptor.PutList phKeyTextStyleRange, textStyleList
    
    paraDescriptor.PutInteger phKeyFrom, 0
    paraDescriptor.PutInteger phKeyTo, 6
    paraDescriptor.PutEnumerated phKeyAlignment, phTypeAlignment, phEnumLeft
    gControl.StringIDToTypeID "firstLineIndent", RuntimeKeyID
    
    paraStyleDescriptor.PutDouble RuntimeKeyID, 0
    gControl.StringIDToTypeID "startIndent", RuntimeKeyID
    paraStyleDescriptor.PutDouble RuntimeKeyID, 0
    gControl.StringIDToTypeID "endIndent", RuntimeKeyID
    paraDescriptor.PutDouble RuntimeKeyID, 0
    gControl.StringIDToTypeID "spaceBefore", RuntimeKeyID
    paraDescriptor.PutDouble RuntimeKeyID, 0
    gControl.StringIDToTypeID "spaceAfter", RuntimeKeyID
    paraDescriptor.PutDouble RuntimeKeyID, 0
    gControl.StringIDToTypeID "hyphenate", RuntimeKeyID
    paraDescriptor.PutBoolean RuntimeKeyID, True
    gControl.StringIDToTypeID "hyphenateWordSize", RuntimeKeyID
    paraDescriptor.PutInteger RuntimeKeyID, 8
    gControl.StringIDToTypeID "hyphenatePreLength", RuntimeKeyID
    paraDescriptor.PutInteger RuntimeKeyID, 3
    gControl.StringIDToTypeID "hyphenatePostLength", RuntimeKeyID
    paraDescriptor.PutInteger RuntimeKeyID, 3
    gControl.StringIDToTypeID "hyphenateLimit", RuntimeKeyID
    paraDescriptor.PutInteger RuntimeKeyID, 2
    gControl.StringIDToTypeID "hyphenationZone", RuntimeKeyID
    paraDescriptor.PutDouble RuntimeKeyID, 36
    gControl.StringIDToTypeID "hyphenateCapitalized", RuntimeKeyID
    paraDescriptor.PutBoolean RuntimeKeyID, True
    gControl.StringIDToTypeID "justificationWordMinimum", RuntimeKeyID
    paraDescriptor.PutDouble RuntimeKeyID, 0.8
    gControl.StringIDToTypeID "justificationWordDesired", RuntimeKeyID
    paraDescriptor.PutDouble RuntimeKeyID, 1
    gControl.StringIDToTypeID "justificationWordMaximum", RuntimeKeyID
    paraDescriptor.PutDouble RuntimeKeyID, 1.33
    gControl.StringIDToTypeID "justificationLetterMinimum", RuntimeKeyID
    paraDescriptor.PutDouble RuntimeKeyID, 0
    gControl.StringIDToTypeID "justificationLetterDesired", RuntimeKeyID
    paraDescriptor.PutDouble RuntimeKeyID, 0
    gControl.StringIDToTypeID "justificationLetterMaximum", RuntimeKeyID
    paraDescriptor.PutDouble RuntimeKeyID, 0
    gControl.StringIDToTypeID "justificationGlyphMinimum", RuntimeKeyID
    paraDescriptor.PutDouble RuntimeKeyID, 1
    gControl.StringIDToTypeID "justificationGlyphDesired", RuntimeKeyID
    paraDescriptor.PutDouble RuntimeKeyID, 1
    gControl.StringIDToTypeID "justificationGlyphMaximum", RuntimeKeyID
    paraDescriptor.PutDouble RuntimeKeyID, 1
    gControl.StringIDToTypeID "hangingRoman", RuntimeKeyID
    paraDescriptor.PutBoolean RuntimeKeyID, False
    gControl.StringIDToTypeID "burasagari", RuntimeKeyID
    paraDescriptor.PutBoolean RuntimeKeyID, False
    gControl.StringIDToTypeID "preferredKinsokuOrder", RuntimeKeyID
    gControl.StringIDToTypeID "pushIn", RuntimeEnumID
    gControl.StringIDToTypeID "preferredKinsokuOrder", RuntimeTypeID
    paraDescriptor.PutEnumerated RuntimeKeyID, RuntimeTypeID, RuntimeEnumID
    gControl.StringIDToTypeID "leadingType", RuntimeKeyID
    gControl.StringIDToTypeID "leadingBelow", RuntimeEnumID
    gControl.StringIDToTypeID "leadingType", RuntimeTypeID
    paraDescriptor.PutEnumerated RuntimeKeyID, RuntimeTypeID, RuntimeEnumID
    gControl.StringIDToTypeID "paragraphStyle", RuntimeObjID
    gControl.StringIDToTypeID "paragraphStyle", RuntimeKeyID
    paraDescriptor.PutObject RuntimeKeyID, RuntimeObjID, paraStyleDescriptor
    gControl.StringIDToTypeID "paragraphStyleRange", RuntimeClassID
    paraStyleList.PutObject RuntimeClassID, paraDescriptor
    gControl.StringIDToTypeID "paragraphStyleRange", RuntimeKeyID
    
    textDescriptor.PutList RuntimeKeyID, paraStyleList
    descriptor.PutObject phKeyUsing, phClassTextLayer, textDescriptor
    
    gControl.StringIDToTypeID "warpStyle", RuntimeKeyID
    gControl.StringIDToTypeID "warpNone", RuntimeEnumID
    gControl.StringIDToTypeID "warpStyle", RuntimeTypeID
    warpDescriptor.PutEnumerated RuntimeKeyID, RuntimeTypeID, RuntimeEnumID
    gControl.StringIDToTypeID "warpValue", RuntimeKeyID
    warpDescriptor.PutDouble RuntimeKeyID, 0
    gControl.StringIDToTypeID "warpPerspective", RuntimeKeyID
    warpDescriptor.PutDouble RuntimeKeyID, 0
    gControl.StringIDToTypeID "warpPerspectiveOther", RuntimeKeyID
    warpDescriptor.PutDouble RuntimeKeyID, 0
    gControl.StringIDToTypeID "warpRotate", RuntimeKeyID
    warpDescriptor.PutEnumerated RuntimeKeyID, phTypeOrientation, phEnumHorizontal
    gControl.StringIDToTypeID "warp", RuntimeObjID
    gControl.StringIDToTypeID "warp", RuntimeKeyID
    descriptor.PutObject RuntimeKeyID, RuntimeObjID, warpDescriptor
    
    Set result = gControl.Play(phEventMake, descriptor, phDialogSilent)
    
    MakeTypeLayer = CheckPlayResult(result)
End Function
Function DuplicateLayerToNewDocument(DocumentName As String, layerName As String) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim usingReference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference  object
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    Set usingReference = PSApp.MakeReference
    
    'build the refernce
    reference.PutClass phClassDocument
    
    'build the descriptor
    descriptor.PutReference phKeyNull, reference
    
    descriptor.PutString phKeyName, DocumentName
     
    usingReference.PutEnumerated phClassLayer, phTypeOrdinal, phEnumTarget
    descriptor.PutReference phKeyUsing, usingReference
    
    descriptor.PutString phKeyLayerName, layerName
    
    'play the event and get back the result
    Set result = gControl.Play(phEventMake, descriptor, phDialogSilent)
    
    'now check for errors
    DuplicateLayerToNewDocument = CheckPlayResult(result)
End Function
Function DuplicateLayer(layerName As String) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    'build the refernce
    reference.PutEnumerated phClassLayer, phTypeOrdinal, phEnumTarget

    'build the descriptor
    descriptor.PutReference phKeyNull, reference
    descriptor.PutString phKeyName, layerName
    
    'play the event and get back the result
    Set result = gControl.Play(phEventDuplicate, descriptor, phDialogSilent)
    
    'now check for errors
    DuplicateLayer = CheckPlayResult(result)
End Function
Function MakeNewLayer(layerName As String, Opacity As Double, BlendMode As Long, Group As Boolean) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    'build the refernce
    reference.PutClass phClassLayer

    'build the descriptor
    descriptor.PutReference phKeyNull, reference
    object.PutString phKeyName, layerName
    object.PutUnitDouble phKeyOpacity, phUnitPercent, Opacity
    object.PutEnumerated phKeyMode, phTypeBlendMode, BlendMode
    object.PutBoolean phKeyGroup, Group
    
    descriptor.PutObject phKeyUsing, phClassLayer, object
    
    'play the event and get back the result
    Set result = gControl.Play(phEventMake, descriptor, phDialogSilent)
    
    'now check for errors
    MakeNewLayer = CheckPlayResult(result)
End Function
Function DeleteLayer() As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    'build the refernce
    reference.PutEnumerated phClassLayer, phTypeOrdinal, phEnumTarget
    
    'build the descriptor
    descriptor.PutReference phKeyNull, reference
    
    'play the event and get back the result
    Set result = gControl.Play(phEventDelete, descriptor, phDialogSilent)
    
    'now check for errors
    DeleteLayer = CheckPlayResult(result)
End Function
Function Group() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    'play the event and get back the result
    Set result = gControl.Play(phEventGroup, Nothing, phDialogSilent)
    
    'now check for errors
    Group = CheckPlayResult(result)
End Function

Function Ungroup() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    'play the event and get back the result
    Set result = gControl.Play(phEventUngroup, Nothing, phDialogSilent)
    
    'now check for errors
    Ungroup = CheckPlayResult(result)
End Function



Function LayerUserMaskEnabled(Enabled As Boolean) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    'build the refernce
    reference.PutEnumerated phClassLayer, phTypeOrdinal, phEnumTarget
    
    'build the descriptor
    descriptor.PutReference phKeyNull, reference
     
    object.PutBoolean phKeyUserMaskEnabled, Enabled
    descriptor.PutObject phKeyTo, phClassLayer, object
    
    'play the event and get back the result
    Set result = gControl.Play(phEventSet, descriptor, phDialogSilent)

    'now check for errors
    LayerUserMaskEnabled = CheckPlayResult(result)
End Function
'phEnumFront, phEnumBack, phEnumPrevious, phEnumNext
Function MoveLayer(NewPosition As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim reference As IActionReference
    Dim toReference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    Set toReference = PSApp.MakeReference
    
    'build the refernce
    reference.PutEnumerated phClassLayer, phTypeOrdinal, phEnumTarget
    
    'build the descriptor
    descriptor.PutReference phKeyNull, reference
    
    'build the to refernce
    toReference.PutEnumerated phClassLayer, phTypeOrdinal, NewPosition
    
    descriptor.PutReference phKeyTo, toReference
    
    'play the event and get back the result
    Set result = gControl.Play(phEventMove, descriptor, phDialogSilent)

    'now check for errors
    MoveLayer = CheckPlayResult(result)
End Function
Function MergeLayers() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    'play the event and get back the result
    Set result = gControl.Play(phEventMergeLayers, Nothing, phDialogSilent)
    
    'now check for errors
    MergeLayers = CheckPlayResult(result)
End Function

Function CopyEffects() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    'play the event and get back the result
    Set result = gControl.Play(phEventCopyEffects, Nothing, phDialogSilent)
    
    'now check for errors
    CopyEffects = CheckPlayResult(result)
End Function

Function PasteEffects() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    'play the event and get back the result
    Set result = gControl.Play(phEventPasteEffects, Nothing, phDialogSilent)
    
    'now check for errors
    PasteEffects = CheckPlayNonResult(result)
End Function

Function DisableLayerFX() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    'play the event and get back the result
    Set result = gControl.Play(phEventDisableLayerFX, Nothing, phDialogSilent)
    
    'now check for errors
    DisableLayerFX = CheckPlayNonResult(result)
End Function

Function GlobalLightingAngle(Angle As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor and reference objectss
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    'build the refernce
    reference.PutProperty phClassProperty, phKeyGlobalAngle
    reference.PutEnumerated phClassDocument, phTypeOrdinal, phEnumTarget
    
    'build the descriptor
    descriptor.PutReference phKeyNull, reference
    object.PutUnitDouble phKeyGlobalLightingAngle, phUnitAngle, Angle
    descriptor.PutObject phKeyTo, phClassGlobalAngle, object

    'play the event and get back the result
    Set result = gControl.Play(phEventSet, descriptor, phDialogSilent)
    
    'now check for errors
    GlobalLightingAngle = CheckPlayResult(result)
End Function

