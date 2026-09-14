// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2002 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this 
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
//-------------------------------------------------------------------
//-------------------------------------------------------------------------------
//
//	File:
//		FrameSelectUI.cpp
//
//	Description:
//		This file contains the source code and routines for the
//		Actions module FrameSelect, an example of a module
//		that uses ADM and the Actions suite to change a selection.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------


#include "PhotoshopSDK.h"
#include "FrameSelect.h"


//-------------------------------------------------------------------------------
//	Definitions -- Dialog items
//-------------------------------------------------------------------------------
enum
	{
	kDNoUI = -1,		// Error.
	kDOk_button = 1,	// Must be one.
	kDCancel_button,	// Must be two.
	kDGutter_staticText,
	kDGutter_frame,
	kDAmount_staticText,
	kDAmount_editText,
	kDUnits_staticText,
	kDUnits_popUp,
	kDOffset_staticText,
	kDOffset_frame,
	kDTop_staticText,
	kDTop_editText,
	kDLeft_staticText,
	kDLeft_editText,
	kDBottom_staticText,
	kDBottom_editText,
	kDRight_staticText,
	kDRight_editText
	};
	
//-------------------------------------------------------------------------------
//	Constants.
//-------------------------------------------------------------------------------

const float kTopMin = -30000.0;
const float kTopMax = 30000.0;

const float kLeftMin = kTopMin;
const float kLeftMax = kTopMax;

const float kBottomMin = kTopMin;
const float kBottomMax = kTopMax;

const float kRightMin = kTopMin;
const float kRightMax = kTopMax;

const unsigned short kPrecision = 4; //4 decimal places
const unsigned short kMaxTextLength = 11; // Room for "-12345.1234".

//-------------------------------------------------------------------------------
//	Prototypes.
//-------------------------------------------------------------------------------
static ASErr ASAPI DoUIInit(ADMDialogRef dialog);

static SPErr InitCancelButton(ADMDialogRef dialog);
static SPErr InitUnitsPopUp(ADMDialogRef dialog);

static void SaveParameters(void);
static void RestoreParameters(void);
static void SetTextToCancel(ADMItemRef item);
static void SetTextToReset(ADMItemRef item);

static void ASAPI DoCancel(ADMItemRef item, ADMNotifierRef notifier);
static ASBoolean ASAPI DoReset(ADMItemRef item, ADMTrackerRef tracker);

static double ConvertEditTextNumber(ADMItemRef item);
static ASBoolean ASAPI TrackAmountEditText(ADMItemRef item, ADMTrackerRef tracker);
static void ASAPI DoAmountEditText(ADMItemRef item, ADMNotifierRef notifier);
static void ASAPI DoTopEditText(ADMItemRef item, ADMNotifierRef notifier);
static void ASAPI DoLeftEditText(ADMItemRef item, ADMNotifierRef notifier);
static void ASAPI DoBottomEditText(ADMItemRef item, ADMNotifierRef notifier);
static void ASAPI DoRightEditText(ADMItemRef item, ADMNotifierRef notifier);
static void ASAPI DoUnitsPopUp(ADMItemRef item, ADMNotifierRef notifier);

static void UpdateRectValues
	(
	/* IN */	const ADMDialogRef dialog
	);

//-------------------------------------------------------------------------------
//	File globals.
//-------------------------------------------------------------------------------
PIUDRect_t saveRect;

bool gCancelButtonIsReset = false;	// Cancel | Reset button.

//-------------------------------------------------------------------------------
//
//	DoUI
//
//	Display the ADM user interface.
//
//-------------------------------------------------------------------------------
SPErr DoUI ()
	{
	SPErr error = noErr;
	
	int item = kDNoUI;	// Error value.
	
	SaveParameters();	// Save our parameters, just in case.
	if (sADMDialog != NULL)
	{
		item = sADMDialog->Modal
			(
			gPlugInRef,
			"FrameSelection",
			kDialogID,
			kADMModalDialogStyle,
			DoUIInit,
			NULL, /* No user data */
			0//InOptions = 0
			);
		}
	
	if (item != kDOk_button)
		{
		error = 'STOP';  //'STOP' = kUserCancel;
		RestoreParameters();
		}

	return error;
	
	}

//-------------------------------------------------------------------------------
//
//	DoUIInit
//
//	Handle initialization of the ADM dialog.
//
//-------------------------------------------------------------------------------
static ASErr ASAPI DoUIInit(ADMDialogRef dialog)
{
	// SPErr = Sweet Pea / PICA error type;
	// OSErr = Photoshop uses the Operating system type;
	// ASErr = SPErr = "Adobe Systems" error type;
	ASErr error = kSPNoError;
	
	ADMItemRef item;
	
	// Set up list and display default item:
	InitCancelButton(dialog);
	
	// Set up pop-up menu:
	InitUnitsPopUp(dialog);
	
	// Set up group menu to notify us when changed:
	item = sADMDialog->GetItem(dialog, kDUnits_popUp);
	sADMItem->SetNotifyProc(item, DoUnitsPopUp);

	// Set up Amount edit text:
	item = sADMDialog->GetItem(dialog, kDAmount_editText);
	sADMItem->SetMaxTextLength(item, kMaxTextLength);
	sADMItem->SetUnits(item, kADMNoUnits);
	sADMItem->SetPrecision(item, kPrecision);
	sADMItem->SetMinFloatValue(item, kTopMin);
	sADMItem->SetMaxFloatValue(item, kTopMax);
	sADMItem->SetNotifyProc(item, DoAmountEditText);

	// Install tracker to change other items as text is
	// edited:
	sADMItem->SetTrackProc(item, TrackAmountEditText);

	// Hilight edit text as first item:
	sADMItem->Activate(item, true);
	sADMItem->SelectAll(item);

	// Set up Top edit text:
	item = sADMDialog->GetItem(dialog, kDTop_editText);
	sADMItem->SetMaxTextLength(item, kMaxTextLength);
	sADMItem->SetUnits(item, kADMNoUnits);
	sADMItem->SetPrecision(item, kPrecision);
	sADMItem->SetMinFloatValue(item, kTopMin);
	sADMItem->SetMaxFloatValue(item, kTopMax);
	sADMItem->SetNotifyProc(item, DoTopEditText);

	// Set up Left edit text:
	item = sADMDialog->GetItem(dialog, kDLeft_editText);
	sADMItem->SetMaxTextLength(item, kMaxTextLength);
	sADMItem->SetUnits(item, kADMNoUnits);
	sADMItem->SetPrecision(item, kPrecision);
	sADMItem->SetMinFloatValue(item, kLeftMin);
	sADMItem->SetMaxFloatValue(item, kLeftMax);
	sADMItem->SetNotifyProc(item, DoLeftEditText);
	
	// Set up Bottom edit text:
	item = sADMDialog->GetItem(dialog, kDBottom_editText);
	sADMItem->SetMaxTextLength(item, kMaxTextLength);
	sADMItem->SetUnits(item, kADMNoUnits);
	sADMItem->SetPrecision(item, kPrecision);
	sADMItem->SetMinFloatValue(item, kBottomMin);
	sADMItem->SetMaxFloatValue(item, kBottomMax);
	sADMItem->SetNotifyProc(item, DoBottomEditText);
	
	// Set up Right edit text:
	item = sADMDialog->GetItem(dialog, kDRight_editText);
	sADMItem->SetMaxTextLength(item, kMaxTextLength);
	sADMItem->SetUnits(item, kADMNoUnits);
	sADMItem->SetPrecision(item, kPrecision);
	sADMItem->SetMinFloatValue(item, kRightMin);
	sADMItem->SetMaxFloatValue(item, kRightMax);
	sADMItem->SetNotifyProc(item, DoRightEditText);

	UpdateRectValues(dialog);
	
	// Set up justification and style for static text items:
	item = sADMDialog->GetItem(dialog, kDAmount_staticText);
	sADMItem->SetJustify(item, kADMRightJustify);
	
	item = sADMDialog->GetItem(dialog, kDUnits_staticText);
	sADMItem->SetJustify(item, kADMRightJustify);

	item = sADMDialog->GetItem(dialog, kDTop_staticText);
	sADMItem->SetJustify(item, kADMRightJustify);

	item = sADMDialog->GetItem(dialog, kDLeft_staticText);
	sADMItem->SetJustify(item, kADMRightJustify);

	item = sADMDialog->GetItem(dialog, kDBottom_staticText);
	sADMItem->SetJustify(item, kADMRightJustify);
	
	item = sADMDialog->GetItem(dialog, kDRight_staticText);
	sADMItem->SetJustify(item, kADMRightJustify);
	
	return error;
}

//-------------------------------------------------------------------------------
//
//	InitCancelButton
//
//	Sets up Cancel button's notifier, etc.
//
//-------------------------------------------------------------------------------
static SPErr InitCancelButton(ADMDialogRef dialog)
	{
	SPErr error = kSPNoError;
	
	if (dialog != NULL)
		{
		// Set up "Cancel" button to notify us when its been clicked:
		ADMItemRef item = sADMDialog->GetItem(dialog, kDCancel_button);
		if (item != NULL)
			{
			sADMItem->SetNotifyProc(item, DoCancel);
		
			// Set up name of Cancel button.  Since we have to have
			// resources around for "Reset" and "Cancel", we might
			// as well check them to load the right value:	
			if (gCancelButtonIsReset)
				SetTextToReset(item);
			else
				SetTextToCancel(item);

			// Set up mask for tracker function:
			ADMActionMask mask = sADMItem->GetMask(item);
			sADMItem->SetMask
				(
				item, 
				mask |
				kADMModKeyDownMask | 
				kADMModKeyUpMask |
				kADMLeaveMask |
				kADMEnterMask |
				kADMButtonUpMask
				);
			
			// Install tracker for it to be "Reset" when it needs to:
			sADMItem->SetTrackProc(item, DoReset);
			}
		else
			{
			error = kSPBadParameterError;
			}
		}
	else
		{
		error = kSPBadParameterError;
		}
						
	return error;
	
	}
	
//-------------------------------------------------------------------------------
//
//	InitUnitsPopUp
//
//	Create group pop-up menu.  There will be a filter pop-up menu created for
//	each group pop-up menu, using kDFilter_popup as the first and prototype
//	for any additional ones that need to be created.
//
//-------------------------------------------------------------------------------
static SPErr InitUnitsPopUp(ADMDialogRef dialog)
	{
	SPErr error = kSPNoError;
	
	ADMItemRef item = sADMDialog->GetItem(dialog, kDUnits_popUp);
	ADMListRef list = sADMItem->GetList(item);
	
	// First clear our the list, just in case.
	ASInt32 numberOfEntries = sADMList->NumberOfEntries(list);
	
	ASInt32 loop = 0;
	
	for (loop = numberOfEntries; loop > 0; loop--)
		sADMList->RemoveEntry(list, loop-1);

	// How build a new list:
	ADMEntryRef entry;
	
	const ADMItemRef popUp = sADMDialog->GetItem(dialog, kDUnits_popUp);

	for (loop = (short)kRulerPixels; loop <= (short)kRulerPercent; loop++)
		{
		entry = sADMList->InsertEntry(list, -1); // At end.
					
		if (entry != NULL)
			{
			char text[256] = "";
			sADMBasic->GetIndexString(gPlugInRef, kUnitsID, loop+1, text, 256);
			sADMEntry->SetText(entry, text);
			}
		}
				
	// sADMList->SetMenuID( list, uiID );

	// Highlight last filter:
	
	// Could use GetEntry() to get by ID, but the index is the
	// same as the ID, so lets use the faster way:
	entry = sADMList->IndexEntry(list, (int)gRulerUnits);
	if (entry != NULL)
		sADMEntry->Select(entry, true);
	
	return error;
	}	
	
//-------------------------------------------------------------------------------
//
//	DoCancel
//
//	Cancel button.
//
//-------------------------------------------------------------------------------
static void ASAPI DoCancel
	(
	ADMItemRef		item,
	ADMNotifierRef	notifier
	)
	{
	if (sADMNotify->IsNotifierType(notifier, kADMUserChangedNotifier))
		{ // Correct notifier.  Do this:
		if (gCancelButtonIsReset)
			{ // Must be reset!
			RestoreParameters(); // Resets.
			ADMDialogRef dialog = sADMItem->GetDialog(item);
			DoUIInit(dialog);
			}
		else
			{
			sADMItem->DefaultNotify(item, notifier);
			}
		}
	else
		{
		sADMItem->DefaultNotify(item, notifier);
		}
	}
	
//-------------------------------------------------------------------------------
//
//	SetTextToCancel
//
//	Set button text to "Cancel".  (For Cancel button.)
//
//-------------------------------------------------------------------------------
static void SetTextToCancel
	(
	/* IN */	ADMItemRef		item	// Item ID for Cancel button.
	)
	{
	if (item != NULL)
		{
		char text[256];
		sADMBasic->GetIndexString(gPlugInRef, kCancelStringID, 1, text, 256);
		sADMItem->SetText(item, text);
		} // item null		
	}

//-------------------------------------------------------------------------------
//
//	SetTextToReset
//
//	Set button text to "Reset".  (For Cancel button.)
//
//-------------------------------------------------------------------------------
static void SetTextToReset
	(
	/* IN */	ADMItemRef		item	// Item ID for Cancel button.
	)
	{
	if (item != NULL)
		{
		char text[256];
		sADMBasic->GetIndexString(gPlugInRef, kResetStringID, 1, text, 256);
		sADMItem->SetText(item, text);
		} // item null
	}
	
//-------------------------------------------------------------------------------
//
//	DoReset
//
//	Cancel button.
//
//-------------------------------------------------------------------------------
static ASBoolean ASAPI DoReset
	(
	ADMItemRef		item,
	ADMTrackerRef	tracker
	)
	{
	ASBoolean doNotify = false;
		
	if (sADMTrack->TestAction(tracker, kADMModKeyDownAction))
		{ // Is the modifier (option/alt) key pressed?
		if (!gCancelButtonIsReset)
			{ // Only if the button is not already "Reset":
			SetTextToReset(item);
			gCancelButtonIsReset = true;
			}
		// Did the user click?  Then let the default track
		// take care of it by dispatching the notifier:
		if (sADMTrack->TestAction(tracker, kADMButtonUpAction))
			doNotify = sADMItem->DefaultTrack(item, tracker);
		}
	else if (sADMTrack->TestAction(tracker, kADMModKeyUpAction) ||
			 sADMTrack->TestAction(tracker, kADMLeaveAction))
		{ // Is the modifier (option/alt) up or the mouse has left?
		if (gCancelButtonIsReset)
			{ // Only if the button is not already "Cancel":
			SetTextToCancel(item);
			gCancelButtonIsReset = false;
			}
		}
	else if (sADMTrack->TestAction(tracker, kADMEnterAction))
		{ // Did the mouse just enter?
		if (sADMTrack->TestModifier(tracker, kADMModKeyDownModifier))
			{ // Is the modifier (option/alt) key pressed?
			if (!gCancelButtonIsReset)
				{ // Only if the button is not already "Reset":
				SetTextToReset(item);
				gCancelButtonIsReset = true;
				}
			
			// Did the user click?
			if (sADMTrack->TestAction(tracker, kADMButtonUpAction))
				doNotify = sADMItem->DefaultTrack(item, tracker);
			}
		else if (gCancelButtonIsReset)
			{ // Only if the button is not already "Cancel":
			SetTextToCancel(item);
			gCancelButtonIsReset = true;
			}
		}
	else
		doNotify = sADMItem->DefaultTrack(item, tracker);

	return doNotify;
	
	}

//-------------------------------------------------------------------------------
//
//	TrackAmountEditText
//
//	Change other edit text fields when gutter amount changes.
//
//-------------------------------------------------------------------------------
static ASBoolean ASAPI TrackAmountEditText
	(
	ADMItemRef		item,
	ADMTrackerRef	tracker
	)
	{
	ASBoolean doNotify = false;
		
	if (sADMTrack->TestAction(tracker, kADMKeyStrokeAction))
		{
		// Unfortunately the value is not converted on every
		// keystroke, but instead after the field has been marked
		// changed.  So we have to get the text string and convert
		// it to its current value ourself.
		
		// First, call the default tracker to take care of the
		// character that was just typed:
		doNotify = sADMItem->DefaultTrack(item, tracker);
		
		// Now convert the text sitting in the field to a
		// number that is useful:
		char text[kMaxTextLength] = "";
		
		sADMItem->GetText(item, text, kMaxTextLength);
		
		float value = 0.0;
		
		bool validValue = (sADMBasic->StringToValue
			(
			text,
			&value,
			kADMNoUnits
			) == TRUE); // Returns as ASBoolean.
		
		if (validValue)
			{
			value = (float)ConvertFromUnitsToUnitDistance
				(
				value,
				gDocumentResolution,
				gDocumentMinBound,
				gRulerUnits
				);
		
			if (value != gAmount)
				{
				gAmount = (double)value;
				SetRectFromGutter(gAmount, &gRect);
				
				ADMDialogRef dialog = sADMItem->GetDialog(item);
				UpdateRectValues(dialog);
				}
			
			} // validValue
		
		sADMItem->SetSelectionRange(item, kMaxTextLength, kMaxTextLength);
						
		}
	else
		doNotify = sADMItem->DefaultTrack(item, tracker);
		
	return doNotify;
	}


//-------------------------------------------------------------------------------
//
//	UpdateRectValues
//
//-------------------------------------------------------------------------------
static void UpdateRectValues
	(
	/* IN */	const ADMDialogRef dialog
	)
	{

	// First convert the user rect from unitDistance to
	// absolute distance:
	PIUDRect_t outRect = ConvertRectFromUnitDistance
		(
		gRect,
		gDocumentResolution,
		gDocumentBounds,
		gRulerUnits
		);

	// Now store the new values in the UI:
	ADMItemRef item = sADMDialog->GetItem(dialog, kDTop_editText);
	sADMItem->SetFloatValue(item, (float)outRect.top);
	
	item = sADMDialog->GetItem(dialog, kDLeft_editText);
	sADMItem->SetFloatValue(item, (float)outRect.left);
	
	item = sADMDialog->GetItem(dialog, kDBottom_editText);
	sADMItem->SetFloatValue(item, (float)outRect.bottom);
	
	item = sADMDialog->GetItem(dialog, kDRight_editText);
	sADMItem->SetFloatValue(item, (float)outRect.right);
	
	// Convert the gutter:
	double value = ConvertFromUnitDistanceToUnits
		(
		gAmount,
		gDocumentResolution,
		gDocumentMinBound,
		gRulerUnits
		);
	
	// Shove the result in the UI:
	item = sADMDialog->GetItem(dialog, kDAmount_editText);
	sADMItem->SetFloatValue(item, (float)value);
	
	}
		
//-------------------------------------------------------------------------------
//
//	ConvertEditTextNumber
//
//	Grab the edit text and return it as a unit distance.
//
//-------------------------------------------------------------------------------
static double ConvertEditTextNumber
	(
	ADMItemRef item
	)
	{
	double value = (double)sADMItem->GetFloatValue(item);
	
	value = ConvertFromUnitsToUnitDistance
		(
		value,
		gDocumentResolution,
		gDocumentMinBound,
		gRulerUnits
		);
	
	return value;
	}

//-------------------------------------------------------------------------------
//
//	DoEditText
//
//	Grab the other edit text when it changes.
//
//-------------------------------------------------------------------------------
static void ASAPI DoAmountEditText
	(
	ADMItemRef item, 
	ADMNotifierRef notifier
	)
	{
	if (sADMNotify->IsNotifierType(notifier, kADMUserChangedNotifier))
		{ // Correct notifier.  Do this:
	
		gAmount = ConvertEditTextNumber(item);		
		
		// Automatically fill out other text fields:
		SetRectFromGutter(gAmount, &gRect);

		ADMDialogRef dialog = sADMItem->GetDialog(item);
		UpdateRectValues(dialog);
		
		} // notifier
		
	} // end DoTopEditText

static void ASAPI DoTopEditText
	(
	ADMItemRef item, 
	ADMNotifierRef notifier
	)
	{
	if (sADMNotify->IsNotifierType(notifier, kADMUserChangedNotifier))
		{ // Correct notifier.  Do this:
	
		gRect.top = ConvertEditTextNumber(item);
		
		} // notifier
		
	} // end DoTopEditText
		
static void ASAPI DoLeftEditText
	(
	ADMItemRef item, 
	ADMNotifierRef notifier
	)
	{
	if (sADMNotify->IsNotifierType(notifier, kADMUserChangedNotifier))
		{ // Correct notifier.  Do this:
	
		gRect.left = ConvertEditTextNumber(item);
		
		} // notifier
		
	} // end DoLeftEditText

static void ASAPI DoBottomEditText
	(
	ADMItemRef item, 
	ADMNotifierRef notifier
	)
	{
	if (sADMNotify->IsNotifierType(notifier, kADMUserChangedNotifier))
		{ // Correct notifier.  Do this:
	
		gRect.bottom = ConvertEditTextNumber(item);
		
		} // notifier
		
	} // end DoBottomEditText

static void ASAPI DoRightEditText
	(
	ADMItemRef item, 
	ADMNotifierRef notifier
	)
	{
	if (sADMNotify->IsNotifierType(notifier, kADMUserChangedNotifier))
		{ // Correct notifier.  Do this:
	
		gRect.right = ConvertEditTextNumber(item);
		
		} // notifier
		
	} // end DoRightEditText

//-------------------------------------------------------------------------------
//
//	DoUnitsPopUp
//
//	Take users' selection and store it in a global variable.
//
//-------------------------------------------------------------------------------
static void ASAPI DoUnitsPopUp
	(
	ADMItemRef		item,
	ADMNotifierRef	notifier
	)
	{
	if (sADMNotify->IsNotifierType(notifier, kADMUserChangedNotifier))
		{ // Correct notifier.  Do this:
		ADMListRef		list = sADMItem->GetList(item);
		ADMEntryRef		entry = sADMList->GetActiveEntry(list);
		ADMDialogRef	dialog = sADMItem->GetDialog(item);
			
		ASInt32 index = sADMEntry->GetIndex(entry);
		
		gRulerUnits = (RulerUnits)index;

		UpdateRectValues(dialog);
		
		item = sADMDialog->GetItem(dialog, kDAmount_editText);
		// Highlight edit text as first item:
		sADMItem->Activate(item, true);
		sADMItem->SelectAll(item);

		} // notifier

	} // DoFilterList

//-------------------------------------------------------------------------------
//
//	Save and Restore File globals.
//
//-------------------------------------------------------------------------------
static void SaveParameters(void)
	{
	saveRect = gRect;	
	}
	
static void RestoreParameters(void)
	{
	gRect = saveRect;
	}

//-------------------------------------------------------------------------------

// end FrameSelectUI.cpp
