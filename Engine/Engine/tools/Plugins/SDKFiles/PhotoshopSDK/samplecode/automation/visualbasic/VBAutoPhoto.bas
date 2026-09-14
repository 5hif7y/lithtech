Attribute VB_Name = "Main"
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

'This routine makes a monstor VB selection using the lasso tool
'and defining points along the way
Function MakeVBSelection() As Long
    Dim Horizontal(10) As Double
    Dim Vertical(10) As Double
    Dim counter As Long
    Dim error As Long
    
    On Error Resume Next
    
    ' make the V with the Polygon Lasso
    counter = 1
    Horizontal(counter) = 71.84375
    Vertical(counter) = 100.28125
    counter = counter + 1
    Horizontal(counter) = 107.75
    Vertical(counter) = 98.78125
    counter = counter + 1
    Horizontal(counter) = 191.59375
    Vertical(counter) = 323.3125
    counter = counter + 1
    Horizontal(counter) = 251.46875
    Vertical(counter) = 100.28125
    counter = counter + 1
    Horizontal(counter) = 275.40625
    Vertical(counter) = 98.78125
    counter = counter + 1
    Horizontal(counter) = 191.59375
    Vertical(counter) = 369.71875
    counter = counter + 1
    Horizontal(counter) = 71.84375
    Vertical(counter) = 100.28125
    
    error = SelectPolygon(phEventSet, Horizontal, Vertical, counter, True)
    
    If error Then GoTo TheEnd

    ' make the outside of the B
    counter = 1
    Horizontal(counter) = 321.8125
    Vertical(counter) = 100.28125
    counter = counter + 1
    Horizontal(counter) = 320.3125
    Vertical(counter) = 366.71875
    counter = counter + 1
    Horizontal(counter) = 420.59375
    Vertical(counter) = 366.71875
    counter = counter + 1
    Horizontal(counter) = 473#
    Vertical(counter) = 312.84375
    counter = counter + 1
    Horizontal(counter) = 495.4375
    Vertical(counter) = 290.375
    counter = counter + 1
    Horizontal(counter) = 493.96875
    Vertical(counter) = 206.5625
    counter = counter + 1
    Horizontal(counter) = 480.46875
    Vertical(counter) = 169.125
    counter = counter + 1
    Horizontal(counter) = 446.0625
    Vertical(counter) = 137.6875
    counter = counter + 1
    Horizontal(counter) = 414.625
    Vertical(counter) = 121.21875
    counter = counter + 1
    Horizontal(counter) = 318.8125
    Vertical(counter) = 101.78125
    
    error = SelectPolygon(phEventAddTo, Horizontal, Vertical, counter, True)
    
    If error Then GoTo TheEnd
    
    counter = 1
    Horizontal(counter) = 345.75
    Vertical(counter) = 133.21875
    counter = counter + 1
    Horizontal(counter) = 345.75
    Vertical(counter) = 133.21875
    counter = counter + 1
    Horizontal(counter) = 348.75
    Vertical(counter) = 224.53125
    counter = counter + 1
    Horizontal(counter) = 458.03125
    Vertical(counter) = 224.53125
    counter = counter + 1
    Horizontal(counter) = 443.0625
    Vertical(counter) = 176.625
    counter = counter + 1
    Horizontal(counter) = 420.59375
    Vertical(counter) = 155.65625
    counter = counter + 1
    Horizontal(counter) = 398.15625
    Vertical(counter) = 143.6875
    counter = counter + 1
    Horizontal(counter) = 369.71875
    Vertical(counter) = 139.1875
    counter = counter + 1
    Horizontal(counter) = 352.5
    Vertical(counter) = 137.6875
    
    error = SelectPolygon(phEventSubtractFrom, Horizontal, Vertical, counter, True)
    
    If error Then GoTo TheEnd
    

    counter = 1
    Horizontal(counter) = 347.25
    Vertical(counter) = 254.46875
    counter = counter + 1
    Horizontal(counter) = 347.25
    Vertical(counter) = 254.46875
    counter = counter + 1
    Horizontal(counter) = 347.25
    Vertical(counter) = 347.25
    counter = counter + 1
    Horizontal(counter) = 411.625
    Vertical(counter) = 339.78125
    counter = counter + 1
    Horizontal(counter) = 444.5625
    Vertical(counter) = 302.34375
    counter = counter + 1
    Horizontal(counter) = 447.5625
    Vertical(counter) = 272.40625
    counter = counter + 1
    Horizontal(counter) = 437.0625
    Vertical(counter) = 257.4375
    counter = counter + 1
    Horizontal(counter) = 393.65625
    Vertical(counter) = 252.96875
    counter = counter + 1
    Horizontal(counter) = 363.71875
    Vertical(counter) = 251.46875
    counter = counter + 1
    Horizontal(counter) = 356.25
    Vertical(counter) = 252.96875
    
    error = SelectPolygon(phEventSubtractFrom, Horizontal, Vertical, counter, True)
    
TheEnd:
    MakeVBSelection = error

End Function
' This routine looks at all the current documents in Photoshop
' and gives a simple yes no for selecting the document
Function PickDocument() As Long
    Dim numDocuments As Long
    Dim docCounter As Long
    Dim docTitle() As String
    Dim docIndex() As Long
    Dim answer As Long
    Dim error As Long
    Dim getInfoVariant As Variant
    Dim currentDocIndex As Long
    
    On Error Resume Next
    
    ' how many documents are there
    getInfoVariant = GetInfoFromPhotoshop(phClassApplication, phKeyNumberOfDocuments)
    If (IsNumeric(getInfoVariant)) Then
        numDocuments = getInfoVariant
    Else
        numDocuments = -1 'make an invalid number of documents
    End If
    ' this should also work but it could return a string so get a variant
    ' and then try to convert
    getInfoVariant = GetInfoFromPhotoshop(phClassDocument, phKeyCount)
    If IsNumeric(getInfoVariant) Then
        If getInfoVariant <> numDocuments Then
            MsgBox ("That's odd. These should be the same!")
        End If
    End If
     
    If numDocuments > 1 Then
        ReDim docTitle(numDocuments)
        ReDim docIndex(numDocuments)
        
        ' get the current document index so we don't try to select something that is already selected
        getInfoVariant = GetInfoFromPhotoshop(phClassDocument, phKeyItemIndex)
        If IsNumeric(getInfoVariant) Then
            currentDocIndex = getInfoVariant
        Else
            MsgBox ("Could not get the current document index! Odd!")
            currentDocIndex = -1
        End If
        
        ' run through all the documents getting the title and index
        For docCounter = 1 To numDocuments
            getInfoVariant = GetInfoFromPhotoshopByIndex(docCounter, phClassDocument, phKeyTitle)
            If Not IsNumeric(getInfoVariant) Then
                docTitle(docCounter) = getInfoVariant
            Else
                docTitle(docCounter) = "Unknown"
            End If
            getInfoVariant = GetInfoFromPhotoshopByIndex(docCounter, phClassDocument, phKeyItemIndex)
            If IsNumeric(getInfoVariant) Then
                docIndex(docCounter) = getInfoVariant
            Else
                docIndex(docCounter) = -1
            End If
            If docIndex(docCounter) <> docCounter Then
                MsgBox "ERROR: Index not equal to index in SelectDocument"
            End If
        Next docCounter

        ' which one do you want me to pick
        For docCounter = 1 To numDocuments
            answer = MsgBox("Select " + docTitle(docCounter), vbYesNo, "Select a Document")
            If answer = vbYes Then
                Exit For
            End If
        Next docCounter
    
        ' pick the document by index
        ' don't bother if all the answers were no
        If docCounter <= numDocuments Then
            If currentDocIndex <> docIndex(docCounter) Then
                error = SelectDocumentByIndex(docIndex(docCounter))
            End If
        End If
    Else
        MsgBox "There are less than two documents open. Please open more documents and try again."
        error = 0
    End If
    PickDocument = error
End Function



' This routine looks at all the current layers on the current
' document in Photoshop and gives a simple yes no for selecting
' a layer
' NOTE: The special handling of background layers. Background layers are counted in the phClassLayer
' but they are accessed by phClassBackgroundLayer. This is quite a hassle. Here is one solution.
Function PickLayer() As Long
    Dim numLayers As Long
    Dim layerCounter As Long
    Dim layerName() As String
    Dim layerIndex() As Long
    Dim answer As Long
    Dim error As Long
    Dim getInfoVariant As Variant
    Dim currentLayerIndex As Long
    Dim backgroundDesc As IActionDescriptor
    Dim documentIndex As Long
    Dim hasBackgroundLayer As Boolean
    
    On Error Resume Next
    
    ' how many layers are there
    getInfoVariant = GetInfoFromPhotoshop(phClassLayer, phKeyCount)
    If IsNumeric(getInfoVariant) Then
        numLayers = getInfoVariant
    Else
        numLayers = -1 'make an invalid number of layers
    End If
    
    If numLayers > 1 Then
        ReDim layerName(numLayers)
        ReDim layerIndex(numLayers)
        
        ' get the current layer index so we don't try to select something that is already selected
        getInfoVariant = GetInfoFromPhotoshop(phClassLayer, phKeyItemIndex)
        If IsNumeric(getInfoVariant) Then
            currentLayerIndex = getInfoVariant
        Else
            MsgBox ("Could not get the current layer index! Odd!")
            currentLayerIndex = -1
        End If
        
        ' see if one of these layers is a background layer
        hasBackgroundLayer = False
        getInfoVariant = GetInfoFromPhotoshop(phClassDocument, phKeyItemIndex)
        If IsNumeric(getInfoVariant) Then
            documentIndex = getInfoVariant
            Set backgroundDesc = GetBackgroundInfo(documentIndex)
            If Not backgroundDesc Is Nothing Then
                backgroundDesc.GetString phKeyName, layerName(numLayers)
                backgroundDesc.GetInteger phKeyItemIndex, layerIndex(numLayers)
                hasBackgroundLayer = True
                numLayers = numLayers - 1
            End If
        Else
            MsgBox ("Could not get the document index! Odd!")
        End If
        
        For layerCounter = 1 To numLayers
            getInfoVariant = GetInfoFromPhotoshopByIndex(layerCounter, phClassLayer, phKeyName)
            If Not IsNumeric(getInfoVariant) Then
                layerName(layerCounter) = getInfoVariant
            Else
                layerName(layerCounter) = "Unknown"
            End If
            getInfoVariant = GetInfoFromPhotoshopByIndex(layerCounter, phClassLayer, phKeyItemIndex)
            If IsNumeric(getInfoVariant) Then
                layerIndex(layerCounter) = getInfoVariant
            Else
                layerIndex(layerCounter) = -1
            End If
          Next layerCounter

        ' which one do you want me to pick
        If hasBackgroundLayer Then
            numLayers = numLayers + 1
        End If
        For layerCounter = 1 To numLayers
            answer = MsgBox("Select " + layerName(layerCounter), vbYesNo, "Select a Document")
            If answer = vbYes Then
                Exit For
            End If
        Next layerCounter
    
        ' pick the document by index
        ' don't bother if all the answers were no
        If layerCounter <= numLayers Then
            If currentLayerIndex <> layerIndex(layerCounter) Then
                If hasBackgroundLayer And layerCounter = numLayers Then
                    error = SelectBackgroundLayer
                Else
                    If hasBackgroundLayer Then
                        error = SelectLayerByIndex(layerIndex(layerCounter) - 1)
                    Else
                        error = SelectLayerByIndex(layerIndex(layerCounter))
                    End If
                End If
            End If
        End If
    Else
        MsgBox ("There are less than two layers available. Please create more layers and try again.")
    End If
    PickLayer = error
End Function
