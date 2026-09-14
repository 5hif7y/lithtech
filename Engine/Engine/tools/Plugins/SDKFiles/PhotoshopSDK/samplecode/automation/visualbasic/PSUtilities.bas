Attribute VB_Name = "Utilities"
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
Type PSConstArray
    LongVal As Long
    StrStr As String
End Type

' these didn't make it to the 5.0.2 type library
' but they did for the 6.0 type library
'Public Const phKeySelection = 1718838636
'Public Const phKeyExactPoints = 1165517648
'Public Const phKeyNumberOfCacheLevels = 1313039208
'Public Const phKeyUseCacheForHistograms = 1433617251
'Public Const phKeyIsDirty = 1232290930
'Public Const phKeyItemIndex = 1232366921
'Public Const phKeyPreviewWinThumbnail = 1349678679
'Public Const phKeyPreviewsQuery = 1349678673
'Public Const phKeyExtensionsQuery = 1165522001
'Public Const phKeySaveComposite = 1400259437
'Public Const phKeyFileSavePrefs = 1181504336
'Public Const phKeyCount = 1131312160

Public Const MAX_KEYS = 2003
Public Const SMALLESTHASHVALUE = 538976288

Public PSCA(MAX_KEYS) As PSConstArray
Public PSApp As Object
Public PSAttached As Boolean
Public gMoreKeyDesc As IActionDescriptor
Public gControl As IActionControl
' check the result for errors
' we are expecting a good result
Function CheckPlayResult(result As IActionDescriptor) As Long
    Dim hasKey As Long
    
    On Error Resume Next
    
    If result Is Nothing Then
        CheckPlayResult = -1
    Else
        result.hasKey phKeyMessage, hasKey
        If hasKey Then
            CheckPlayResult = -1
        Else
            CheckPlayResult = 0
        End If
    End If
End Function
' check the result for errors
' we are not expecting anything here
Function CheckPlayNonResult(result As IActionDescriptor) As Long
    Dim hasKey As Long
    
    On Error Resume Next
    
    If result Is Nothing Then
        CheckPlayNonResult = 0
    Else
        result.hasKey phKeyMessage, hasKey
        If hasKey Then
            CheckPlayNonResult = -1
        Else
            CheckPlayNonResult = 0
        End If
    End If
End Function




' Given a class and property the GetActionProperty will
' return a descriptor with the information in it. This
' routine pulls open that descriptor and returns the
' data for you. This uses VB's Variant technology to
' return any type for you.
Function GetInfoFromPhotoshop(Class As Long, property As Long) As Variant
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    Dim hasKey As Long
    Dim keyType As Long
    Dim unitID As Long
    ' All the return types available
    Dim longValue As Long
    Dim doubleValue As Double
    Dim stringValue As String
    Dim boolValue As Long
    Dim descValue As IActionDescriptor
    Dim enumValue As Long
    Dim listValue As IActionList
    Dim refValue As IActionReference
    
    On Error Resume Next
    
    If property > 0 Then
        Set reference = PSApp.MakeReference
    
        reference.PutProperty phClassProperty, property
        reference.PutEnumerated Class, phTypeOrdinal, phEnumTarget
        
        gControl.GetActionProperty reference, result
    
        result.hasKey property, hasKey
        
        If hasKey Then
            result.GetType property, keyType
            Select Case (keyType)
                Case phTypeInteger
                    result.GetInteger property, longValue
                    GetInfoFromPhotoshop = longValue
    
                Case phTypeFloat
                    result.GetDouble property, doubleValue
                    GetInfoFromPhotoshop = doubleValue
    
                Case phTypeUnitFloat
                    result.GetUnitDouble property, unitID, doubleValue
                    GetInfoFromPhotoshop = doubleValue
    
                Case phTypeChar
                     result.GetString property, stringValue
                     GetInfoFromPhotoshop = stringValue
                
                Case phTypePath
                     result.GetPath property, stringValue
                     GetInfoFromPhotoshop = stringValue
    
                Case phTypeBoolean
                    result.GetBoolean property, boolValue
                     GetInfoFromPhotoshop = boolValue
    
                Case phTypeObject
                    result.GetObject property, unitID, descValue
                    Set GetInfoFromPhotoshop = descValue
    
                Case phTypeGlobalObject
                    result.GetGlobalObject property, unitID, descValue
                    Set GetInfoFromPhotoshop = descValue
                    
                Case phTypeEnumerated
                    result.GetEnumerated property, unitID, enumValue
                    GetInfoFromPhotoshop = enumValue
    
                Case phTypePath, phTypeAlias
                    result.GetPath property, stringValue
                    GetInfoFromPhotoshop = stringValue
    
                Case phTypeValueList
                    result.GetList property, listValue
                    Set GetInfoFromPhotoshop = listValue
    
                Case phTypeObjectSpecifier
                    result.GetReference property, refValue
                    Set GetInfoFromPhotoshop = refValue
    
                Case phTypeType, phTypeGlobalClass
                    result.GetClass property, unitID
                    GetInfoFromPhotoshop = unitID
                    
                Case Else
                    GetInfoFromPhotoshop = "Unsupported type: GetInfoFromPhotoshop"
            End Select
        Else
            GetInfoFromPhotoshop = "ERROR: Does not have this key."
        End If 'hasKey
    Else
        GetInfoFromPhotoshop = "ERROR: Property must be a positive value."
    End If 'property > 0
End Function
' This routine is like GetInfoFromPhotoshop but allows you
' to index into an Object: Document, Layer, Channel, Path
' and get the information without having to select it first
' Look at PickDocument and PickLayer for uses.
Function GetInfoFromPhotoshopByIndex(Index As Long, Class As Long, property As Long) As Variant
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    Dim hasKey As Long
    Dim keyType As Long
    Dim unitID As Long
    ' All the return types available
    Dim longValue As Long
    Dim doubleValue As Double
    Dim stringValue As String
    Dim boolValue As Long
    Dim descValue As IActionDescriptor
    Dim enumValue As Long
    Dim listValue As IActionList
    Dim refValue As IActionReference
    
    On Error Resume Next
    
    If property > 0 Then
        Set reference = PSApp.MakeReference
    
        reference.PutProperty phClassProperty, property
        reference.PutIndex Class, Index
        
        gControl.GetActionProperty reference, result
    
        result.hasKey property, hasKey
        
        If hasKey Then
            result.GetType property, keyType
            Select Case (keyType)
                Case phTypeInteger
                    result.GetInteger property, longValue
                    GetInfoFromPhotoshopByIndex = longValue
    
                Case phTypeFloat
                    result.GetDouble property, doubleValue
                    GetInfoFromPhotoshopByIndex = doubleValue
    
                Case phTypeUnitFloat
                    result.GetUnitDouble property, unitID, doubleValue
                    GetInfoFromPhotoshopByIndex = doubleValue
    
                Case phTypeChar
                     result.GetString property, stringValue
                     GetInfoFromPhotoshopByIndex = stringValue
                
                Case phTypePath
                     result.GetPath property, stringValue
                     GetInfoFromPhotoshopByIndex = stringValue
    
                Case phTypeBoolean
                    result.GetBoolean property, boolValue
                     GetInfoFromPhotoshopByIndex = boolValue
    
                Case phTypeObject
                    result.GetObject property, unitID, descValue
                    GetInfoFromPhotoshopByIndex = descValue
    
                Case phTypeGlobalObject
                    result.GetGlobalObject property, unitID, descValue
                    Set GetInfoFromPhotoshopByIndex = descValue
                    
                Case phTypeEnumerated
                    result.GetEnumerated property, unitID, enumValue
                    GetInfoFromPhotoshopByIndex = enumValue
    
                Case phTypePath, phTypeAlias
                    result.GetPath property, stringValue
                    GetInfoFromPhotoshopByIndex = stringValue
    
                Case phTypeValueList
                    result.GetList property, listValue
                    Set GetInfoFromPhotoshopByIndex = listValue
    
                Case phTypeObjectSpecifier
                    result.GetReference property, refValue
                    Set GetInfoFromPhotoshopByIndex = refValue
    
                Case phTypeType, phTypeGlobalClass
                    result.GetClass property, unitID
                    GetInfoFromPhotoshopByIndex = unitID
                    
                Case Else
                    GetInfoFromPhotoshopByIndex = "Unsupported type: GetInfoFromPhotoshop"
            End Select
        Else
            GetInfoFromPhotoshopByIndex = "ERROR: Does not have this key."
        End If 'hasKey
    Else
        GetInfoFromPhotoshopByIndex = "ERROR: Property must be a positive value."
    End If 'property > 0
End Function
' Gets the background layer info
Function GetBackgroundInfo(docIndex As Long) As IActionDescriptor
    Dim reference As IActionReference
    
    On Error Resume Next
    
    Set reference = PSApp.MakeReference
    reference.PutProperty phClassBackgroundLayer, phKeyBackground
    reference.PutIndex phClassDocument, docIndex
    
    gControl.GetActionProperty reference, GetBackgroundInfo
End Function
' Pulls an error string out of your descriptor, useful for debugging
Function GetErrorStringFromDescriptor(desc As IActionDescriptor) As String
    Dim hasString As Long
    
    On Error Resume Next ' i need a way to tell that the desc is nothing
    
    GetErrorStringFromDescriptor = ""
    'If Not (desc = Nothing) Then
        desc.hasKey phKeyMessage, hasString
        If hasString Then
            desc.GetString phKeyMessage, GetErrorStringFromDescriptor
        End If
    'End If
End Function
' Converts a long value 'Hash' into its string equivalent
' "Hash"
Function HashToString(hash As Long) As String
    Dim temp As Long
    Dim LessThis As Long
    Dim counter As Integer
    
    On Error Resume Next
    
    HashToString = "'"
    For counter = 1 To 4
        temp = 256 ^ (4 - counter)
        LessThis = hash / temp
        hash = hash - temp * LessThis
        HashToString = HashToString + Chr(LessThis)
    Next counter
    HashToString = HashToString + "'"
End Function

' Opposite of HashToString
Function StringToHash(inString As String) As Long
    Dim temp As Long
    Dim counter As Integer
    Dim tickmark As Integer
    
    On Error Resume Next
    
    If Len(inString) > 6 Then
        StringToHash = -1
    End If
    
    tickmark = InStr(1, inString, "'", vbTextCompare)
    
    If tickmark = 0 Then
        StringToHash = -1
    End If
    
    tickmark = InStr(1 + tickmark, inString, "'", vbTextCompare)
    
    If tickmark = 6 Then
        'remove the ' marks from the string and then convert
        inString = Mid(inString, 2, 4)
    Else
        StringToHash = -1
    End If
    
    If Not StringToHash = -1 Then
        If StrComp(inString, "", vbTextCompare) <> 0 Then
            For counter = 1 To 4
                temp = Asc(Mid$(inString, counter, 1)) * (256 ^ (4 - counter))
                StringToHash = StringToHash + temp
            Next counter
        End If
    End If
End Function

' Initialize the big array for easy lookup for "ph" VB constant
' values and there actual long values
Sub InitPSConstArray()
    
    On Error Resume Next
    
    Call InitPSConstStrStr
    Call InitPSConstLongVal
End Sub



'Takes a visual basic constant string "phKeyMode"
'and converts to the long value
' 1) Look through the array above for a match
' 2) See if it's a "'Hash'" and convert
' 3) Use the runtime converter
Function StrToLong(inputStr As String) As Long
    Dim counter As Long
    
    On Error Resume Next
    
    StrToLong = -1
    
    For counter = 1 To MAX_KEYS
        If StrComp(PSCA(counter).StrStr, inputStr) = 0 Then
            StrToLong = PSCA(counter).LongVal
            Exit For
        End If
    Next counter
    
    If StrToLong = -1 Then
        StrToLong = StringToHash(inputStr)
    End If
    
    If StrToLong = -1 Then
        gControl.StringIDToTypeID inputStr, StrToLong
    End If

End Function
'Takes a visual basic long and converts to the Photoshop
' Type library constants "phxxxxxx"
' 1) If it's small then it's a runtime ID
' 2) Look through the array above for a match
' 3) Convert the number to a "'Hash'"
Function LongToStr(inputLong As Long, keyType As String) As String
    Dim counter As Long
    Dim found As Boolean
    
    On Error Resume Next
    
    found = False
    LongToStr = ""
    
    ' if the input long is a small number then it's a runtime ID
    ' use the gControl to get the string
    If inputLong < SMALLESTHASHVALUE Then
        gControl.TypeIDToStringID inputLong, keyType
        LongToStr = keyType
        found = True
    End If
    
    If Not found Then
        For counter = 1 To MAX_KEYS
            If PSCA(counter).LongVal = inputLong Then
                If StrComp(Left(PSCA(counter).StrStr, 5), Left(keyType, 5)) = 0 Then
                LongToStr = PSCA(counter).StrStr
                Exit For
                End If
            End If
        Next counter
    End If
    
    If StrComp(LongToStr, "") = 0 And inputLong > SMALLESTHASHVALUE Then
        LongToStr = HashToString((inputLong))
    End If

End Function


