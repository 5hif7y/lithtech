VERSION 5.00
Begin VB.Form VBAutoPhoto 
   Caption         =   "VB Automating Photoshop"
   ClientHeight    =   2070
   ClientLeft      =   4230
   ClientTop       =   3360
   ClientWidth     =   6135
   LinkTopic       =   "Form1"
   ScaleHeight     =   2070
   ScaleWidth      =   6135
   StartUpPosition =   2  'CenterScreen
   Begin VB.CheckBox foregroundChkBx 
      Caption         =   "Foreground"
      Enabled         =   0   'False
      Height          =   255
      Left            =   4200
      TabIndex        =   9
      Top             =   1560
      Value           =   1  'Checked
      Visible         =   0   'False
      Width           =   1575
   End
   Begin VB.TextBox greenValueTxt 
      Enabled         =   0   'False
      Height          =   285
      Left            =   2640
      TabIndex        =   7
      Text            =   "127"
      Top             =   1560
      Visible         =   0   'False
      Width           =   495
   End
   Begin VB.TextBox blueValueTxt 
      Enabled         =   0   'False
      Height          =   285
      Left            =   3360
      TabIndex        =   8
      Text            =   "127"
      Top             =   1560
      Visible         =   0   'False
      Width           =   495
   End
   Begin VB.TextBox redValueTxt 
      Enabled         =   0   'False
      Height          =   285
      Left            =   1920
      TabIndex        =   6
      Text            =   "127"
      Top             =   1560
      Visible         =   0   'False
      Width           =   495
   End
   Begin VB.CommandButton setColorBtn 
      Caption         =   "SetColor"
      Enabled         =   0   'False
      Height          =   375
      Left            =   360
      TabIndex        =   5
      Top             =   1320
      Visible         =   0   'False
      Width           =   1095
   End
   Begin VB.CommandButton startPSbtn 
      Caption         =   "Start Photoshop"
      Height          =   375
      Left            =   360
      TabIndex        =   1
      Top             =   120
      Width           =   5535
   End
   Begin VB.ComboBox eventComboBox 
      Enabled         =   0   'False
      Height          =   315
      Left            =   3600
      TabIndex        =   4
      Text            =   "Run this Event"
      Top             =   720
      Visible         =   0   'False
      Width           =   2175
   End
   Begin VB.CommandButton hideShowBtn 
      Caption         =   "Hide/Show"
      Enabled         =   0   'False
      Height          =   375
      Left            =   360
      TabIndex        =   2
      Top             =   720
      Visible         =   0   'False
      Width           =   1095
   End
   Begin VB.CommandButton getInfoBtn 
      Caption         =   "Get Info"
      Enabled         =   0   'False
      Height          =   375
      Left            =   2040
      TabIndex        =   3
      Top             =   720
      Visible         =   0   'False
      Width           =   1095
   End
   Begin VB.Label blueLabel 
      Caption         =   "Blue:"
      Enabled         =   0   'False
      Height          =   255
      Left            =   3360
      TabIndex        =   11
      Top             =   1320
      Visible         =   0   'False
      Width           =   495
   End
   Begin VB.Label greenLabel 
      Caption         =   "Green:"
      Enabled         =   0   'False
      Height          =   255
      Left            =   2640
      TabIndex        =   10
      Top             =   1320
      Visible         =   0   'False
      Width           =   495
   End
   Begin VB.Label redLabel 
      Caption         =   "Red:"
      Enabled         =   0   'False
      Height          =   255
      Left            =   1920
      TabIndex        =   0
      Top             =   1320
      Visible         =   0   'False
      Width           =   495
   End
End
Attribute VB_Name = "VBAutoPhoto"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' Keeps track of the Properties form to see if it is already up
Public FormRunning As Boolean

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

' Enable/Disable all the form objects depending on wether
' Photoshop is running
Private Sub FormItems(Enabled As Boolean)
    
    On Error Resume Next
    
    eventComboBox.Enabled = Enabled
    eventComboBox.Visible = Enabled
    
    getInfoBtn.Enabled = Enabled
    getInfoBtn.Visible = Enabled
    
    hideShowBtn.Enabled = Enabled
    hideShowBtn.Visible = Enabled
    
    setColorBtn.Visible = Enabled
    setColorBtn.Enabled = Enabled
    
    redLabel.Visible = Enabled
    redLabel.Enabled = Enabled
    
    greenLabel.Visible = Enabled
    greenLabel.Enabled = Enabled
    
    blueLabel.Visible = Enabled
    blueLabel.Enabled = Enabled
    
    redValueTxt.Visible = Enabled
    redValueTxt.Enabled = Enabled
    
    greenValueTxt.Visible = Enabled
    greenValueTxt.Enabled = Enabled
    
    blueValueTxt.Visible = Enabled
    blueValueTxt.Enabled = Enabled
    
    foregroundChkBx.Visible = Enabled
    foregroundChkBx.Enabled = Enabled
End Sub
Private Static Sub GetCurrentColorValues()

End Sub


' User clicked on a "Run this Event" item and is dispatched to here.
Private Static Sub eventComboBox_Click()
    Dim error As Long
    Dim counter As Long
    
    On Error Resume Next
    
    If PSAttached Then
       Select Case (eventComboBox.Text)
           Case "Make a New Document"
               counter = counter + 1
               error = MakeNewDocument("VB Auto Doc " + Str(counter), phClassRGBColorMode, 720, 720, 72, phEnumTransparent)
           Case "Make a New Layer"
               counter = counter + 1
               error = MakeNewLayer("VB Auto Layer " + Str(counter), 95#, phEnumNormal, False)
           Case "Make a Selection"
               error = MakeVBSelection
           Case "Fill Selection"
               error = Fill(phEnumForegroundColor, 100#, phEnumNormal)
           Case "Gaussian Blur"
               GaussianBlur (12)
           Case "Select This Document"
               error = PickDocument
           Case "Select This Layer"
               error = PickLayer
           Case "Make Type Layer"
               counter = counter + 1
               error = MakeTypeLayer(30#, 90#, "VB " + Str(counter), "Helvetica", phEnumCenter, 144#, True, True)
           Case Else
               MsgBox "Something bad just happened"
       End Select
       
       If error Then
           MsgBox "Error in " + eventComboBox.Text
       End If
    End If
End Sub


' Load the main form, initialize the big constant array,
' add the items to the event combo box
Private Sub Form_Load()
    
    On Error Resume Next
    
    Call InitPSConstArray
    eventComboBox.AddItem "Make a New Document"
    eventComboBox.AddItem "Make a New Layer"
    eventComboBox.AddItem "Make a Selection"
    eventComboBox.AddItem "Fill Selection"
    eventComboBox.AddItem "Gaussian Blur"
    eventComboBox.AddItem "Select This Document"
    eventComboBox.AddItem "Select This Layer"
    eventComboBox.AddItem "Make Type Layer"
End Sub

' Detach from Photoshop but don't quit it
Private Sub Form_UnLoad(Cancel As Integer)
    
    On Error Resume Next
    
    If PSAttached Then
        Set PSApp = Nothing
        PSAttached = False
    End If
End Sub

' Show the Get Properties dialog box/form,
' don't display it more that once
Private Static Sub getInfoBtn_Click()
    Dim Fm As PropertiesForm

    On Error Resume Next
    
    If PSAttached Then
        If FormRunning = False Then
            Set Fm = New PropertiesForm
            Fm.Show
            FormRunning = True
        Else
            Fm.Show
        End If
    End If
End Sub


' Hide and show PHotoshop
Private Sub hideShowBtn_Click()
    
    On Error Resume Next
    
    If PSAttached Then
        If PSApp.Visible Then
            PSApp.Visible = False
            hideShowBtn.Caption = "Show"
        Else
            PSApp.Visible = True
            hideShowBtn.Caption = "Hide"
        End If
    End If
End Sub




Private Sub setColorBtn_Click()
    Dim red As Double
    Dim green As Double
    Dim blue As Double
    Dim foreground As Boolean
    Dim answer As Long
    
    On Error Resume Next
    
    answer = GetColor(True, phClassRGBColor, red, green, blue, blue)
    
    red = redValueTxt.Text
    green = greenValueTxt.Text
    blue = blueValueTxt.Text
    foreground = foregroundChkBx.Value
    
    answer = SetColorRGB(foreground, red, green, blue)
End Sub

'Start and stop Photoshop
Private Sub startPSbtn_Click()
    
    ' On Error Resume Next
    
    If StrComp(startPSbtn.Caption, "Start Photoshop", vbTextCompare) = 0 Then
        startPSbtn.Caption = "...Starting....Please Wait..."
        Set PSApp = CreateObject("Photoshop.Application.8.1")
        Set gControl = PSApp.MakeControlObject 'make only one of these
        startPSbtn.Caption = "Stop Photoshop"
        PSAttached = True
        FormItems (True)
        If PSApp.Visible Then
            hideShowBtn.Caption = "Hide"
        Else
            hideShowBtn.Caption = "Show"
        End If
   Else
        Set gControl = Nothing
        PSApp.Quit
        PSAttached = False
        FormItems (False)
        startPSbtn.Caption = "Start Photoshop"
        hideShowBtn.Caption = "Show"
    End If
End Sub



