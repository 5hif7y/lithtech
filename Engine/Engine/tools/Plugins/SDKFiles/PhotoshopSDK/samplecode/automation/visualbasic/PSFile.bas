Attribute VB_Name = "File"
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
' These are actions that a user could perform via the Photoshop File
' menu pull down.

Option Explicit
Function SaveFileAsPSD(FullPath As String) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' Save a file in the Photoshop format
    descriptor.PutClass phKeyAs, phClassPhotoshop35Format
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)
    
    ' now check for errors
    SaveFileAsPSD = CheckPlayResult(result)
End Function
Function SaveFile() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' Save the current file in whatever format
    Set result = gControl.Play(phEventSave, Nothing, phDialogSilent)
    
    ' now check for errors
    SaveFile = CheckPlayNonResult(result)
End Function
Function SaveFileAsIFF(FullPath As String) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' Save a file in the Amiga IFF format
    descriptor.PutString phKeyAs, "Amiga IFF"
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)

    ' now check for errors
    SaveFileAsIFF = CheckPlayResult(result)
End Function
'Depth = phEnumBitDepth1, phEnumBitDepth4, phEnumBitDepth8, or phEnumBitDepth24
'Platform = phEnumOS2, phEnumWindows, phEnumMacintosh, or phEnumIBMPC
Function SaveFileAsBMP(FullPath As String, Depth As Long, Platform As Long, Compression As Boolean) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    
    ' Save a file in the BMP format
    object.PutEnumerated phKeyPlatform, phTypePlatform, Platform
    object.PutEnumerated phKeyBitDepth, phTypeBitDepth, Depth
    object.PutBoolean phKeyCompression, Compression
    descriptor.PutGlobalObject phKeyAs, phClassBMPFormat, object
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)
    
    ' now check for errors
    SaveFileAsBMP = CheckPlayResult(result)
End Function
'Preview = phEnumTIFF or phEnumMacintosh
'Depth = phEnum1BitPerPixel or phEnum8BitsPerPixel
'Encoding = phEnumASCII, phEnumBinary, phEnumJPEG, or phEnumZip
Function SaveFileAsEPS(FullPath As String, Preview As Long, Depth As Long, Encoding As Long, HalftoneScreen As Boolean, Transfer As Boolean, ColorManagement As Boolean) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    
    ' Safe the file in EPS format
    object.PutEnumerated phKeyPreview, phTypeEPSPreview, Preview
    object.PutEnumerated phKeyDepth, phTypeDepth, Depth
    object.PutEnumerated phKeyEncoding, phTypeEncoding, Encoding
    object.PutBoolean phKeyHalftoneScreen, HalftoneScreen
    object.PutBoolean phKeyTransferFunction, Transfer
    object.PutBoolean phKeyColorManagement, ColorManagement
    descriptor.PutObject phKeyAs, phClassPhotoshopEPSFormat, object
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)

    ' now check for errors
    SaveFileAsEPS = CheckPlayResult(result)
End Function
Function SaveFileAsFPX(FullPath As String, Compress As Boolean) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    
    ' Save the file as Flash Pix format
    object.PutBoolean phKeyFPXCompress, Compress
    descriptor.PutGlobalObject phKeyAs, StringToHash("'FPXF'"), object
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    'doesn't appear to respect the phDialogSilent parameter
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)

    ' now check for errors
    SaveFileAsFPX = CheckPlayResult(result)
End Function
'Quality = 0 to 10
'Scans = 3, 4, or 5
Function SaveFileAsJPG(FullPath As String, Quality As Long, Optimized As Boolean, Scans As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    
    ' Save the file in JPG format
    object.PutInteger phKeyQuality, Quality
    If Optimized Then
        object.PutBoolean phKeyOptimized, Optimized
    End If
    If Scans = 3 Or Scans = 4 Or Scans = 5 Then
        object.PutInteger phKeyScans, Scans
    End If
    descriptor.PutObject phKeyAs, phClassJPEGFormat, object
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)

    ' now check for errors
    SaveFileAsJPG = CheckPlayResult(result)
End Function
Function SaveFileAsPCX(FullPath As String) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' Save the file in PCX format
    descriptor.PutString phKeyAs, "PCX"
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)

    ' now check for errors
    SaveFileAsPCX = CheckPlayResult(result)
End Function
'Encoding = phEnumJPEG or phEnumZip
'Quality = 0 to 10
Function SaveFileAsPDF(FullPath As String, Encoding As Long, Quality As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    
    ' Save the file PDF format
    object.PutEnumerated phKeyEncoding, phTypeEncoding, Encoding
    If (Encoding = phEnumJPEG) Then
        object.PutInteger phKeyQuality, Quality
    End If
    descriptor.PutObject phKeyAs, phClassPhotoshopPDFFormat, object
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)

    ' now check for errors
    SaveFileAsPDF = CheckPlayResult(result)
End Function
'Resolution = phEnum16BitsPerPixel or phEnum32BitsPerPixel
Function SaveFileAsPCT(FullPath As String, Resolution As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    
    ' Save the file in PICT format
    object.PutEnumerated phKeyResolution, phTypeDeepDepth, Resolution
    descriptor.PutObject phKeyAs, phClassPICTFileFormat, object
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)

    ' now check for errors
    SaveFileAsPCT = CheckPlayResult(result)
End Function
Function SaveFileAsPXR(FullPath As String) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' Save the file in Pixar format
    descriptor.PutString phKeyAs, "Pixar"
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)

    ' now check for errors
    SaveFileAsPXR = CheckPlayResult(result)
End Function
'Interlace = phEnumPNGInterlaceNone or phEnumPNGInterlaceAdam7
'Filter = phEnumPNGFilterNone, phEnumPNGFilterSub, phEnumPNGFilterUp, phEnumPNGFilterAverage, phEnumPNGFilterPaeth, or phEnumPNGFilterAdaptive
Function SaveFileAsPNG(FullPath As String, Interlace As Long, Filter As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    
    ' Save the file in PNG format
    object.PutEnumerated phKeyPNGInterlaceType, phTypePNGInterlaceType, Interlace
    object.PutEnumerated phKeyPNGFilter, phTypePNGFilter, Filter
    descriptor.PutGlobalObject phKeyAs, phClassPNGFormat, object
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)

    ' now check for errors
    SaveFileAsPNG = CheckPlayResult(result)
End Function
'FileType = "????"
'FileCreator = "8BIM"
'Header = 0 to 32768
Function SaveFileAsRAW(FullPath As String, FileType As String, FileCreator As String, Header As Long, Interleaved As Boolean) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    
    ' Save the file in RAW format
    object.PutString phKeyFileType, FileType
    object.PutString phKeyFileCreator, FileCreator
    object.PutInteger phKeyHeader, Header
    object.PutBoolean phKeyChannelsInterleaved, Interleaved
    descriptor.PutObject phKeyAs, phClassRawFormat, object
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)

    ' now check for errors
    SaveFileAsRAW = CheckPlayResult(result)
End Function
Function SaveFileAsSCT(FullPath As String) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    ' Save the file in SCITEX format
    descriptor.PutClass phKeyAs, phClassScitexCTFormat
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)

    ' now check for errors
    SaveFileAsSCT = CheckPlayResult(result)
End Function
'Depth = phEnumBitDepth1, phEnumBitDepth4, phEnumBitDepth8, or phEnumBitDepth24
Function SaveFileAsTGA(FullPath As String, Depth As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    
    ' Save the file in Targa format
    object.PutInteger phKeyBitDepth, Depth
    descriptor.PutGlobalObject phKeyAs, phClassTargaFormat, object
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)

    ' now check for errors
    SaveFileAsTGA = CheckPlayResult(result)
End Function
'ByteOrder = phEnumOS2, phEnumWindows, phEnumMacintosh, or phEnumIBMPC
Function SaveFileAsTIF(FullPath As String, ByteOrder As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    
    ' Save the file in TIFF format
    object.PutEnumerated phKeyByteOrder, phTypePlatform, ByteOrder
    descriptor.PutObject phKeyAs, phClassTIFFFormat, object
    descriptor.PutPath phKeyIn, FullPath
    descriptor.PutBoolean phKeyLowerCase, True
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSave, descriptor, phDialogSilent)

    ' now check for errors
    SaveFileAsTIF = CheckPlayResult(result)
End Function
Function MakeNewDocument(Name As String, Mode As Long, Width As Double, Height As Double, Resolution As Double, Fill As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    
    'create the document class in this object
    object.PutUnitDouble phKeyResolution, phUnitDensity, Resolution
    object.PutString phKeyName, Name
    object.PutClass phKeyMode, Mode
    object.PutUnitDouble phKeyWidth, phUnitDistance, Width
    object.PutUnitDouble phKeyHeight, phUnitDistance, Height
    object.PutEnumerated phKeyFill, phTypeFill, Fill

    'object goes in the main descriptor
    descriptor.PutObject phKeyNew, phClassDocument, object

    'play the event and get the result back
    Set result = gControl.Play(phEventMake, descriptor, phDialogSilent)
    
    'now check for errors, why isn't this returning something
    MakeNewDocument = CheckPlayNonResult(result)
End Function
Function OpenDocument(FullPath As String) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    'build the descriptor
    descriptor.PutPath phKeyNull, FullPath
    
    'play the event and get the result back
    Set result = gControl.Play(phEventOpen, descriptor, phDialogSilent)
    
    'now check for errors
    OpenDocument = CheckPlayResult(result)
End Function
Function OpenDocumentAs(FullPath As String, Format As Long) As Long
    Dim descriptor As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    
    'build the descriptor
    descriptor.PutPath phKeyNull, FullPath
    descriptor.PutClass phKeyAs, Format
    
    'play the event and get the result back
    Set result = gControl.Play(phEventOpen, descriptor, phDialogSilent)
    
    'now check for errors
    OpenDocumentAs = CheckPlayResult(result)
End Function
Function CloseDocument() As Long
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    'play the event and get the result back
    Set result = gControl.Play(phEventClose, Nothing, phDialogSilent)
    
    'now check for errors
    CloseDocument = CheckPlayNonResult(result)
End Function
Function PlacePDFDocument(FullPath As String, PageNumber As Long, Horizontal As Double, Vertical As Double) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    
    'build the descriptor
    descriptor.PutPath phKeyNull, FullPath
    descriptor.PutInteger phKeyPageNumber, PageNumber
    descriptor.PutEnumerated phKeyFreeTransformCenterState, phTypeQuadCenterState, phEnumQCSAverage
     
    object.PutUnitDouble phKeyHorizontal, phUnitPixels, Horizontal
    object.PutUnitDouble phKeyVertical, phUnitPixels, Vertical
    
    descriptor.PutObject phKeyOffset, phClassOffset, object
    
    'play the event and get the result back
    Set result = gControl.Play(phEventPlace, descriptor, phDialogSilent)
    
    'now check for errors
    PlacePDFDocument = CheckPlayResult(result)
End Function
Function ExportViaOutbound(FullPath As String) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim result As IActionDescriptor
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    
    'build the descriptor
    object.PutPath phKeyIn, FullPath
    
    descriptor.PutGlobalObject phKeyUsing, StringToHash("'outB'"), object
    
    'play the event and get the result back
    Set result = gControl.Play(phEventExport, descriptor, phDialogSilent)
    
    'now check for errors
    ExportViaOutbound = CheckPlayResult(result)
End Function
Function FileInfo(key As Long, Value As Variant) As Long
    Dim descriptor As IActionDescriptor
    Dim object As IActionDescriptor
    Dim reference As IActionReference
    Dim list As IActionList
    Dim result As IActionDescriptor

    Dim longValue As Long
    Dim boolValue As Boolean
    Dim strValue As String
    
    On Error Resume Next
    
    ' create the descriptor object
    Set descriptor = PSApp.MakeDescriptor
    Set object = PSApp.MakeDescriptor
    Set reference = PSApp.MakeReference
    
    'build the reference
    reference.PutProperty phClassProperty, phKeyFileInfo
    'target the current document
    reference.PutEnumerated phClassDocument, phTypeOrdinal, phEnumTarget

    'reference goes in main descriptor
    descriptor.PutReference phKeyNull, reference
     
    ' there are three basic types for file info
    ' Boolean, Integer, or String
    ' this routine switches on the key and fills the
    ' object descriptor accordingly
    Select Case (key)
        Case phKeyUrgency
            If IsNumeric(Value) Then
                longValue = Value
            End If
            object.PutInteger phKeyUrgency, longValue
        Case phKeyCopyright
            If IsNumeric(Value) Then
                boolValue = Value
            End If
            object.PutBoolean phKeyCopyright, boolValue
        Case phKeySupplementalCategories, phKeyKeywords
            Set list = PSApp.MakeList
            strValue = Value
            list.PutString strValue
            object.PutList key, list
        Case Else
            strValue = Value
            object.PutString key, strValue
    End Select
    
    'object goes in the main descriptor
    descriptor.PutObject phKeyTo, phClassFileInfo, object
    
    'play the event and get the result back
    Set result = gControl.Play(phEventSet, descriptor, phDialogSilent)
    
    'now check for errors
    FileInfo = CheckPlayResult(result)
End Function
