VERSION 5.00
Begin VB.Form PropertiesForm 
   Caption         =   "Get Properties From Photoshop"
   ClientHeight    =   3195
   ClientLeft      =   5370
   ClientTop       =   4500
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   1  'CenterOwner
   Begin VB.ComboBox moreKeyComboBox 
      Enabled         =   0   'False
      Height          =   315
      Left            =   960
      TabIndex        =   8
      Text            =   "Select a Key"
      Top             =   1440
      Visible         =   0   'False
      Width           =   3375
   End
   Begin VB.ComboBox keyComboBox 
      Height          =   315
      Left            =   960
      TabIndex        =   7
      Text            =   "Select a Key"
      Top             =   840
      Width           =   3375
   End
   Begin VB.TextBox valueTextBox 
      Height          =   285
      Left            =   960
      TabIndex        =   6
      Top             =   2640
      Width           =   3375
   End
   Begin VB.TextBox typeTextBox 
      Height          =   285
      Left            =   960
      TabIndex        =   5
      Top             =   2160
      Width           =   3375
   End
   Begin VB.ComboBox classComboBox 
      Height          =   315
      Left            =   960
      TabIndex        =   1
      Text            =   "Select a Class"
      Top             =   240
      Width           =   3375
   End
   Begin VB.Label moreKeyLabel 
      Caption         =   "Key"
      Enabled         =   0   'False
      Height          =   375
      Left            =   240
      TabIndex        =   9
      Top             =   1440
      Visible         =   0   'False
      Width           =   495
   End
   Begin VB.Label Label4 
      Caption         =   "Value"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   2640
      Width           =   615
   End
   Begin VB.Label Label3 
      Caption         =   "Type"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   2160
      Width           =   615
   End
   Begin VB.Label Label2 
      Caption         =   "Key"
      Height          =   375
      Left            =   240
      TabIndex        =   2
      Top             =   840
      Width           =   495
   End
   Begin VB.Label Label1 
      Caption         =   "Class"
      Height          =   375
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   495
   End
End
Attribute VB_Name = "PropertiesForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


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
'-------------------------------------------------------------------
' Routine gets called if a key is actually a descriptor that
' needs more information pulled out of it
Sub UpdateMoreKeysInClass(objDesc As IActionDescriptor)
    Dim reference As IActionReference
    Dim keyCount As Long
    Dim counter As Long
    Dim currentKey As Long
    Dim currentAsString As String
    
    On Error Resume Next
    
    moreKeyComboBox.Clear
    
    objDesc.GetCount keyCount
    If keyCount > 0 Then
        For counter = 0 To keyCount - 1
            objDesc.GetKey counter, currentKey
            currentAsString = LongToStr(currentKey, "phKey")
            moreKeyComboBox.AddItem currentAsString
        Next counter
    End If

    moreKeyComboBox.Text = "Select A Key"

End Sub



' Update the keyComboBox with the class that was just selected
Private Sub classComboBox_Click()
    Dim classAsLong As Long
    
    On Error Resume Next
    
    moreKeyComboBox.Enabled = False
    moreKeyComboBox.Visible = False
    moreKeyLabel.Enabled = False
    moreKeyLabel.Visible = False
    
    classAsLong = StrToLong(classComboBox.Text)
    UpdateKeysInClass classAsLong

End Sub

' Add the main classes for Photoshop
Private Sub Form_Load()
    On Error Resume Next
    
    classComboBox.AddItem "phClassApplication"
    classComboBox.AddItem "phClassHistoryState"
    classComboBox.AddItem "phClassDocument"
    classComboBox.AddItem "phClassChannel"
    classComboBox.AddItem "phClassPath"
    classComboBox.AddItem "phClassLayer"
End Sub

Private Sub Form_UnLoad(Cancel As Integer)
    On Error Resume Next
    
    VBAutoPhoto.FormRunning = False
End Sub

' After the user selects a class, update the next combo box
' with the keys from this class
Sub UpdateKeysInClass(inputClass As Long)
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    Dim keyCount As Long
    Dim counter As Long
    Dim currentKey As Long
    Dim currentAsString As String
    
    On Error Resume Next
    
    Set reference = PSApp.MakeReference
    
    reference.PutEnumerated inputClass, phTypeOrdinal, phEnumTarget
    gControl.GetActionProperty reference, result
    
    keyComboBox.Clear
    
    If Not result Is Nothing Then
        result.GetCount keyCount
        If keyCount > 0 Then
            For counter = 0 To keyCount - 1
                result.GetKey counter, currentKey
                currentAsString = LongToStr(currentKey, "phKey")
                keyComboBox.AddItem currentAsString
            Next counter
        End If
    End If
    
    keyComboBox.Text = "Select A Key"

End Sub
' Update the text boxes with the current class and key information
Sub GetTypeAndValue(inputDesc As IActionDescriptor, key As Long, typeStr As String, valueStr As String, outputDesc As IActionDescriptor)
    Dim valueLong As Long
    Dim valueBool As Long
    Dim valueDouble As Double
    Dim unitKey As Long
    Dim unitType As Long
    Dim unitID As Long
    Dim classID As Long
    
    On Error Resume Next
    
    If key = 0 Then
        inputDesc.GetKey 0, unitKey
    Else
        unitKey = key
    End If
    
    inputDesc.GetType unitKey, unitType
    typeStr = LongToStr((unitType), "phType")
    Select Case (unitType)
        Case phTypeInteger
            inputDesc.GetInteger unitKey, valueLong
            valueStr = CStr(valueLong)
        Case phTypeFloat
            inputDesc.GetDouble unitKey, valueDouble
            valueStr = CStr(valueDouble)
        Case phTypeUnitFloat
            inputDesc.GetUnitDouble unitKey, unitID, valueDouble
            valueStr = LongToStr(unitID, "phUnit") + " " + Str(valueDouble)
        Case phTypeChar, phTypePath
            inputDesc.GetString unitKey, valueStr
        Case phTypeBoolean
            inputDesc.GetBoolean unitKey, valueBool
            valueStr = CStr(valueBool)
        Case phTypeEnumerated
            inputDesc.GetEnumerated unitKey, unitType, valueLong
            valueStr = LongToStr(unitType, "phType") + " " + LongToStr(valueLong, "phEnum")
        Case phTypePath, phTypeAlias
            inputDesc.GetAlias unitKey, valueStr
        Case phTypeObject, phTypeGlobalObject
            valueStr = "Object Type"
            If Not outputDesc Is Nothing Then
                outputDesc.Clear
            End If
            inputDesc.GetObject unitKey, classID, outputDesc
            UpdateMoreKeysInClass outputDesc
            moreKeyComboBox.Visible = True
            moreKeyComboBox.Enabled = True
            moreKeyLabel.Visible = True
            moreKeyLabel.Enabled = True
        Case Else
            valueStr = "Unsupported Type"
    End Select
End Sub


' After the user clicks a class and then clicks a key
' update the text boxes with the information we found
Private Sub keyComboBox_Click()
    Dim reference As IActionReference
    Dim result As IActionDescriptor
    Dim keyAsLong As Long
    Dim classAsLong As Long
    Dim tempStr As String
    Dim temppStr As String
    
    On Error Resume Next
    
    ' create the reference object
    Set reference = PSApp.MakeReference
    
    moreKeyComboBox.Enabled = False
    moreKeyComboBox.Visible = False
    moreKeyLabel.Enabled = False
    moreKeyLabel.Visible = False
    
    keyAsLong = StrToLong(keyComboBox.Text)
    
    classAsLong = StrToLong(classComboBox.Text)
    
    reference.PutProperty phClassProperty, keyAsLong
    reference.PutEnumerated classAsLong, phTypeOrdinal, phEnumTarget
    
    gControl.GetActionProperty reference, result
    
    If Not result Is Nothing Then
        GetTypeAndValue result, 0, tempStr, temppStr, gMoreKeyDesc
        valueTextBox.Text = temppStr
        typeTextBox.Text = tempStr
    End If
End Sub

' update the information if there is an object in the key selected
Private Sub moreKeyComboBox_Click()
    Dim keyAsLong As Long
    Dim tempStr As String
    Dim temppStr As String
    Dim tempDesc As IActionDescriptor
    
    On Error Resume Next
    
    keyAsLong = StrToLong(moreKeyComboBox.Text)
    
    If Not gMoreKeyDesc Is Nothing And Not keyAsLong = -1 Then
        GetTypeAndValue gMoreKeyDesc, keyAsLong, tempStr, temppStr, tempDesc
        If Not tempDesc Is Nothing Then
            Set gMoreKeyDesc = tempDesc
        End If
        valueTextBox.Text = temppStr
        typeTextBox.Text = tempStr
    End If
End Sub


