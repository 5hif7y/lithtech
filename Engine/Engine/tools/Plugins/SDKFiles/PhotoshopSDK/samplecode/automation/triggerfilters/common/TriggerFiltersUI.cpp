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
//		TriggerFiltersUI.cpp
//
//	Description:
//		This file contains the source code and routines for the
//		Actions module TriggerFilters, an example of a module
//		that uses ADM and the Actions suite to execute events
//		from the Filter menu.
//
//	Use:
//		This is a good example of a basic shell to pop a cross-
//		platform UI using ADM and trigger basic events.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "TriggerFilters.h"

//-------------------------------------------------------------------------------
//	Definitions -- Dialog items
//-------------------------------------------------------------------------------
const unsigned char kConvertStrLen = 33; // Minimum string length for conversions. 
// Rarely need more than this for precision, but you can
// specify it directly with a number from 0...255:
const unsigned char kPIUNoPrecision = 0;
const unsigned char kPIUSinglePrecision = 1;
const unsigned char kPIUDoublePrecision = 2;
const unsigned char kPIUQuadPrecision = 4;
#define kPSErrorUnknownStringID	'!sid'

enum
	{
	kDNoUI = -1,		// Error condition.
	kDOk_button = 1,	// Must always be one.
	kDCancel_button,	// Must always be two.
	kDDoit_button,
	kDUndo_button,
	kDFilterTitle_staticText,
	kDFilter_userBox,
	kDGroup_popup,
	kDFilter_popup,
	kDDialog_popup,
	kDDialog_staticText,
	kDOther_editText,
	kDOther_staticText,
	kDResultsTitle_staticText,
	kDResults_userBox,
	kDResults_staticText,
	kDLogToFile_checkBox,
	kDLogToFile_staticText
	};
	
//-------------------------------------------------------------------------------
//	Prototypes.
//-------------------------------------------------------------------------------
static ASErr ASAPI DoUIInit(ADMDialogRef dialog);
static void ASAPI DoDialogCleanup(ADMDialogRef dialog);
static SPErr ParseFilenameToDisplay(const SPPlatformFileSpecification fileSpec,char displayString[kMaxStr255Len]);
static SPErr LongToCString(const long value,char outString[kConvertStrLen]);

static SPErr InitDialogList(ADMDialogRef dialog);
static SPErr InitGroupList(ADMDialogRef item);
static SPErr InitFilterList
	(
	/* IN */		ADMDialogRef dialog, 
	/* IN */		const eventList_t* eventList,
	/* IN */		const eventList_t* lastEventList,
	/* IN/OUT */	ADMItemRef item
	);
static SPErr InitCancelButton(ADMDialogRef dialog);
static SPErr InitLogToFile(ADMDialogRef dialog);

static ASBoolean ASAPI DoReset(ADMItemRef item, ADMTrackerRef tracker);

static void ASAPI DoCancel(ADMItemRef item, ADMNotifierRef notifier);
static void ASAPI DoDialogDoit(ADMItemRef item, ADMNotifierRef notifer);
static void ASAPI DoDialogUndo(ADMItemRef item, ADMNotifierRef notifier);
static void ASAPI DoFilterList(ADMItemRef item, ADMNotifierRef notifier);
static void ASAPI DoGroupList(ADMItemRef item, ADMNotifierRef notifier);
static void ASAPI DoDialogList(ADMItemRef item, ADMNotifierRef notifier);
static void ASAPI DoLogToFile(ADMItemRef item, ADMNotifierRef notifier);
static void ASAPI DoOtherEditText(ADMItemRef item, ADMNotifierRef notifier);

static void HideShowFilterList(void);
static void SaveParameters(void);
static void RestoreParameters(void);
static void SetTextToCancel(ADMItemRef item);
static void SetTextToReset(ADMItemRef item);

#ifdef __PIMac__
	// Macintosh requires updates to the dialog while an event is
	// playing, so we create our own routine to handle this stuff.
	void MacPlayNotifier(const EventRecord * event, void * data);
#endif

//-------------------------------------------------------------------------------
//	File globals.
//-------------------------------------------------------------------------------
bool gCancelButtonIsReset = false;	// Cancel | Reset button.

groupList_t*	saveLastGroupList = NULL;
eventList_t*	saveLastEventList = NULL;
bool saveLogToFile = false;
SPPlatformFileSpecification saveFile;
showDialog_t	saveShowDialog = kShowDialogOnce;

ADMItemRef	gFilter_userBox = NULL;

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
			"TriggerFilters",
			kDialogID, // uiID,
			kADMModalDialogStyle,
			DoUIInit,
			NULL, /* No user data */
			0 //inOptions = 0
			);
		}
	
	if (item != kDOk_button)
		{
		// If we did something, we should undo it here (DoUndo()).
		error = 'STOP'; //'STOP' =  kUserCancel;
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
	ASErr error = kSPNoError;
	ADMItemRef item;
	
	if (gFilter_userBox == NULL)
		gFilter_userBox = sADMDialog->GetItem(dialog, kDFilter_userBox);

	// Set up list and display default item:
	InitGroupList(dialog);
	InitDialogList(dialog);
	InitCancelButton(dialog);
	InitLogToFile(dialog);

	// Set up "Do it!" button to notify us when its been clicked:
	item = sADMDialog->GetItem(dialog, kDDoit_button);
	sADMItem->SetNotifyProc(item, DoDialogDoit);

	// Set up "Undo" button to notify us when its been clicked:
	item = sADMDialog->GetItem(dialog, kDUndo_button);
	sADMItem->SetNotifyProc(item, DoDialogUndo);
		
	// Set up group menu to notify us when changed:
	item = sADMDialog->GetItem(dialog, kDGroup_popup);
	sADMItem->SetNotifyProc(item, DoGroupList);
	
	// Set up group menu to notify us when changed:
	item = sADMDialog->GetItem(dialog, kDDialog_popup);
	sADMItem->SetNotifyProc(item, DoDialogList);
	
	// Set up Other edit text:
	item = sADMDialog->GetItem(dialog, kDOther_editText);
	sADMItem->SetNotifyProc(item, DoOtherEditText);
	// And initialize its font and style:
	sADMItem->SetFont(item, kADMPaletteFont); // smaller
	// And set its maximum size:
	sADMItem->SetMaxTextLength(item, kMaxStr255Len);
	
	// Set up result font:
	item = sADMDialog->GetItem(dialog, kDResults_staticText);
	sADMItem->SetFont(item, kADMPaletteFont); // smaller
	
	// Set up justification and style for static text items:
	item = sADMDialog->GetItem(dialog, kDDialog_staticText);
	sADMItem->SetJustify(item, kADMRightJustify);
	
	item = sADMDialog->GetItem(dialog, kDOther_staticText);
	sADMItem->SetJustify(item, kADMRightJustify);
	
	sADMDialog->SetDestroyProc(dialog, DoDialogCleanup);

	return error;
}

//-------------------------------------------------------------------------------
//
//	DoDialogCleanup
//
//	Remove any items we've added to this dialog just in case we're run later
//	without unloading our resource fork.
//
//-------------------------------------------------------------------------------

static void ASAPI DoDialogCleanup(ADMDialogRef dialog)
	{
	const ADMItemRef filterPopUp = sADMDialog->GetItem(dialog, kDFilter_popup);
	ADMItemRef item = NULL;
	
	for 
		(
		groupList_t* groupList = gGroupList; 
		groupList != NULL; 
		groupList = groupList->next
		)
		{
		item = groupList->eventListDialogItem;
		if (item != NULL)
			{
			if (item != filterPopUp)
				{
				sADMItem->Destroy(item);
				}
			}
		}

	if (gFilter_userBox != NULL)
		{
		sADMItem->Destroy(gFilter_userBox);
		gFilter_userBox = NULL;
		}
	}
	
//-------------------------------------------------------------------------------
//
//	InitGroupList
//
//	Create group pop-up menu.  There will be a filter pop-up menu created for
//	each group pop-up menu, using kDFilter_popup as the first and prototype
//	for any additional ones that need to be created.
//
//-------------------------------------------------------------------------------
static SPErr InitGroupList(ADMDialogRef dialog)
	{
	SPErr error = kSPNoError;
	
	ADMItemRef item = sADMDialog->GetItem(dialog, kDGroup_popup);
	ADMListRef list = sADMItem->GetList(item);
	
	// First clear our the list, just in case.
	ASInt32 numberOfEntries = sADMList->NumberOfEntries(list);
	
	for (ASInt32 loop = numberOfEntries; loop > 0; loop--)
		sADMList->RemoveEntry(list, loop-1);

	// How build a new list:
	ADMEntryRef entry;
	
	bool usedFilterPopUp = false;
	
	const ADMItemRef filterPopUp = sADMDialog->GetItem(dialog, kDFilter_popup);
	
	groupList_t*	groupList = gGroupList;
	while (groupList != NULL)
		{
		if (groupList->eventListDialogItem == NULL)
			{
			if (!usedFilterPopUp)
				{			
				// Can only use this one once:
				usedFilterPopUp = true;
				groupList->eventListDialogItem = filterPopUp;
				}
			else
				{
				// Have to create a new filter pop-up for this group category:
				ASRect filterPopUpRect;
				sADMItem->GetBoundsRect(filterPopUp, &filterPopUpRect);
				
				groupList->eventListDialogItem =
					sADMItem->Create
						(
						dialog,
						groupList->index,		// ID
						kADMPopupListType,
						&filterPopUpRect,
						NULL,					// Init Proc
						NULL					// User data
						);
				
				// Sigh.  Windows tab order makes it so that all frame boxes
				// must be last in the tab order so that events get to the
				// elements in the frame (and not sent to the frame).  When
				// we create this new item, it gets created at the end of
				// the tab order.  There is no provision in ADM to renumber
				// the Windows tab order.  So what we'll do is delete the
				// frame, add our item, then add the frame back.  This
				// guarantees its the very last item.
				#ifdef __PIWin__
					ASRect frameRect;
					int id = 0;

					if (gFilter_userBox != NULL)
						{
						sADMItem->GetBoundsRect(gFilter_userBox, &frameRect);
						
						id = sADMItem->GetID(gFilter_userBox);
						
						// Hide before we destroy:
						sADMItem->Show(gFilter_userBox, false);
						sADMItem->Destroy(gFilter_userBox);
						}

					gFilter_userBox = sADMItem->Create
						(
						dialog,
						id,
						kADMFrameType,
						&frameRect,
						NULL,
						NULL
						);
						
					if (gFilter_userBox != NULL)
						{
						sADMItem->SetItemStyle(gFilter_userBox, kADMEtchedFrameStyle);
						}

				#endif


				}
				
			} // eventListDialogItem
			
		entry = sADMList->InsertEntry(list, groupList->index);
					
		if (entry != NULL)
			{
			sADMEntry->SetID(entry, groupList->index);
			
			if (sSPBasic->IsEqual(groupList->name, (char*)kSeparator))
				sADMEntry->MakeSeparator(entry, true);
			else
				{
				sADMEntry->SetText(entry, groupList->name);

				// InitFilterList for every valid groupList.
				error = InitFilterList
					(
					dialog, 
					groupList->eventList,
					groupList->lastEventList, 
					groupList->eventListDialogItem
					);
				}
			
			groupList = groupList->next;	
			}
		else
			{
			groupList = NULL;
			}
		
		
		}
				
	// sADMList->SetMenuID( list, uiID );

	// Highlight last filter:
	
	// Could use GetEntry() to get by ID, but the index is the
	// same as the ID, so lets use the faster way:
	entry = sADMList->IndexEntry(list, gLastGroupList->index);
	if (entry != NULL)
		sADMEntry->Select(entry, true);
			
	HideShowFilterList();
	
	return error;
	}			

//-------------------------------------------------------------------------------
//
//	HideShowEventList
//
//	Hides other event lists and shows the current one.
//
//-------------------------------------------------------------------------------
static void HideShowFilterList(void)
	{
	ADMItemRef item = NULL;
	
	groupList_t*	groupList = gGroupList;
	while (groupList != NULL)
		{
		item = groupList->eventListDialogItem;
		if (item != NULL)
			{
			if (groupList == gLastGroupList)
				{
				sADMItem->Show(item, true);
				}
			else
				{
				sADMItem->Show(item, false);
				}
			}
		groupList = groupList->next;	
		}
	}

//-------------------------------------------------------------------------------
//
//	InitFilterList
//
//	Create filter pop-up menu.
//
//-------------------------------------------------------------------------------
static SPErr InitFilterList
	(
	/* IN */		ADMDialogRef dialog, 
	/* IN */		const eventList_t* eventList,
	/* IN */		const eventList_t* lastEventList,
	/* IN/OUT */	ADMItemRef item
	)
	{
	SPErr error = kSPNoError;
	
	if (item != NULL)
		{
		ADMListRef list = sADMItem->GetList(item);
		
		// First clear our the list, just in case.
		long numberOfEntries = sADMList->NumberOfEntries(list);
		
		// Set up Popup menu to notify us when changed:
		sADMItem->SetNotifyProc(item, DoFilterList);
		
		// Select first item (to avoid lots of flicker as we update):
		ADMEntryRef entry = sADMList->InsertEntry(list, 0);
		if (entry != NULL)
			{ // Adding a blank entry to view while we're doing this:
			sADMEntry->SetText(entry, "");
			sADMEntry->Select(entry, true);
			}
		
		// How build a new list:
		unsigned short index = 1;
		unsigned short selectedIndex = 0;
		
		while (eventList != NULL)
			{
			if (index > numberOfEntries)
				{
				// Off end.  Add one.
				entry = sADMList->InsertEntry(list, index);
				}
			else
				{
				// Get pointer to this entry.
				entry = sADMList->IndexEntry(list, index);
				}

			if (entry != NULL)
				{
				sADMEntry->SetID(entry, eventList->index);
				
				if (sSPBasic->IsEqual(eventList->id, (char*)kSeparator))
					{
					sADMEntry->SetText(entry, "");
					sADMEntry->MakeSeparator(entry, true);
					}
				else
					{
					sADMEntry->SetText(entry, eventList->name);
					sADMEntry->Enable(entry, true);
					// Just in case if in its previous life it
					// was a separator:
					sADMEntry->MakeSeparator(entry, false);
					}
				
				if (eventList == lastEventList)
					selectedIndex = index-1; // 0-based.
						
				++index;
				eventList = eventList->next;	
				}
			else
				{
				eventList = NULL;
				}
			}

		// Now remove anything sitting at the end of the list:
		for (long loop = numberOfEntries; loop >= index; loop--)
			sADMList->RemoveEntry(list, loop);

		sADMList->RemoveEntry(list, 0); // Remove blank first entry.
				
		// sADMList->SetMenuID( list, uiID );

		// Highlight last filter:
		
		// Could use GetEntry() to get by ID, but the index is the
		// same as the ID, so lets use the faster way:
		entry = sADMList->IndexEntry(list, selectedIndex);
		if (entry != NULL)
			sADMEntry->Select(entry, true);
		
		item = sADMDialog->GetItem(dialog, kDOther_editText);
		if (sSPBasic->IsEqual(gLastGroupList->lastEventList->id, otherKey))
			{
			sADMItem->Enable(item, true);
			sADMItem->SetText(item, gOtherEvent);
			}
		else
			{
			sADMItem->Enable(item, false);
			sADMItem->SetText(item, "");
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
//	InitDialogList
//
//	Create dialog pop-up menu.
//
//-------------------------------------------------------------------------------
static SPErr InitDialogList(ADMDialogRef dialog)
	{
	SPErr error = kSPNoError;
	
	ADMItemRef item = sADMDialog->GetItem(dialog, kDDialog_popup);
	ADMListRef list = sADMItem->GetList(item);
	ADMEntryRef entry = NULL;
	
	// First clear our the list, just in case.
	ASInt32 numberOfEntries = sADMList->NumberOfEntries(list);
	ASInt32 loop = 0;
	
	for (loop = numberOfEntries; loop > 0; loop--)
		sADMList->RemoveEntry(list, loop-1);
	
	for 
		(
		loop = 0;
		loop < mapKeyToShowDialog_size;
		loop++
		)
		{
		char tempString[256] = "";
		sADMBasic->GetIndexString(gPlugInRef, mapKeyToShowDialog[loop].resourceID, 1, tempString, 256);

		entry = sADMList->InsertEntry(list, loop);
		
		sADMEntry->SetID(entry, loop);
			
		sADMEntry->SetText(entry, tempString);
				
		} // loop
			
	entry = sADMList->GetEntry(list, gShowDialog);
	if (entry != NULL)
		sADMEntry->Select(entry, true);
	
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

			// And set up a tracker for it to be "Reset" when it needs to:
			sADMItem->SetTrackProc(item, DoReset);
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
//	InitLogToFile
//
//	Initializes log to file button and text.
//
//-------------------------------------------------------------------------------
static SPErr InitLogToFile(ADMDialogRef dialog)
	{
	SPErr error = kSPNoError;
	
	if (dialog != NULL)
		{
		// Set up Log to file:
		ADMItemRef item = sADMDialog->GetItem(dialog, kDLogToFile_checkBox);
		
		if (item != NULL)
			{
			sADMItem->SetNotifyProc(item, DoLogToFile);
	
			char string[kMaxStr255Len] = "";
						
		if (ValidFileSpec(gFile))
				{
				ParseFilenameToDisplay(gFile, string);
				sADMItem->SetIntValue(item, 1 ); /* ON */
				}	
			else
				sADMItem->SetIntValue(item, 0 /* OFF */);
				
			// Set up Log to file text:
				item = sADMDialog->GetItem(dialog, kDLogToFile_staticText);
				
				if (item != NULL)
					{
					sADMItem->SetFont(item, kADMPaletteFont); // smaller
					sADMItem->SetText(item, string);
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
		}
	else
		{
		error = kSPBadParameterError;
		}
	
	return error;
	
	}

//-------------------------------------------------------------------------------
//
//	DoDialogList
//
//	When dialog list pop-up is accessed, take users' selection and store it
//	in a global variable.
//
//-------------------------------------------------------------------------------
static void ASAPI DoDialogList
	(
	ADMItemRef		item,
	ADMNotifierRef	notifier
	)
	{
	if (sADMNotify->IsNotifierType(notifier, kADMUserChangedNotifier))
		{ // Correct notifier.  Do this:
		ADMListRef		list = sADMItem->GetList(item);
		ADMEntryRef		entry = sADMList->GetActiveEntry(list);
		
		ASInt32 index = sADMEntry->GetIndex(entry);
		
		if (index >= 0 && index < mapKeyToShowDialog_size)
			{
			showDialog_t showDialog = mapKeyToShowDialog[index].showDialog;

			if (gShowDialog != showDialog)
				{
				// New selection.  Change it.
				gShowDialog = showDialog;
				// Do any updates here (none needed).
				}
			}

		} // notifier

	} // DoDialogList

//-------------------------------------------------------------------------------
//
//	DoGroupList
//
//	When group list pop-up is accessed, take users' selection and store it
//	in a global variable.
//
//-------------------------------------------------------------------------------
static void ASAPI DoGroupList
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
		
		groupList_t* groupList = FindIndexInGroupList((short)index);

		if (gLastGroupList != groupList && groupList != NULL)
			{
			// It is a new selection, do something about that.
			gLastGroupList = groupList;

			// Now update our filter list, since we changed groups:
			HideShowFilterList();
			
			item = sADMDialog->GetItem(dialog, kDOther_editText);
			if 
				(
				sSPBasic->IsEqual
					(
					gLastGroupList->lastEventList->id, 
					otherKey
					)
				)
				{ 
				// Selected other, so lets activate the edit text field:
				sADMItem->Enable(item, true);
				sADMItem->Activate(item, true);
				sADMItem->SelectAll(item);
				}
			else
				sADMItem->Enable(item, false);
				
			}

		} // notifier

	} // DoGroupList

//-------------------------------------------------------------------------------
//
//	DoFilterList
//
//	When filter list pop-up is accessed, take users' selection and store it
//	in a global variable.
//
//-------------------------------------------------------------------------------
static void ASAPI DoFilterList
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
		
		eventList_t* eventList = FindIndexInEventList
			(
			(short)index,
			gLastGroupList->eventList
			);
		
		if (gLastGroupList->lastEventList != eventList && eventList != NULL)
			{
			// Really is different.
			gLastGroupList->lastEventList = eventList;

			item = sADMDialog->GetItem(dialog, kDOther_editText);
			if 
				(
				sSPBasic->IsEqual
					(
					gLastGroupList->lastEventList->id, 
					otherKey
					)
				)
				{ 
				// Selected other, so lets move focus to the
				// edit text field:
				sADMItem->Enable(item, true);
				sADMItem->Activate(item, true);
				sADMItem->SelectAll(item);
				}
			else
				sADMItem->Enable(item, false);
				
				
			} // Index out of bounds.

		} // notifier

} // DoFilterList

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
			
			// ADMDialogRef dialog = sADMItem->GetDialog(item);
			// sADMDialog->EndModal(dialog, kDCancel_button, true /* Cancelling */);			
			}
		}
	else
		sADMItem->DefaultNotify(item, notifier);
			
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
		char* cancelString = "Cancel";
		sADMItem->SetText(item, cancelString);
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
		char* resetString = "Reset";
		sADMItem->SetText(item, resetString);
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
//	DoDialogDoit
//
//	When user clicks Doit button, dispatch filter that is selected in pop-up.
//
//-------------------------------------------------------------------------------
static void ASAPI DoDialogDoit
	(
	ADMItemRef item, 
	ADMNotifierRef notifier
	)
	{
	if (sADMNotify->IsNotifierType(notifier, kADMUserChangedNotifier))
		{ // Correct notifier.  Do this:
	
		if (gLastGroupList != NULL && gLastGroupList->lastEventList != NULL)
			{
			ADMDialogRef dialog = sADMItem->GetDialog(item);
			unsigned long ticks;
		
			SPErr error = PlayFilter(&(gLastGroupList->lastEventList->descriptor), &ticks);
			DisplayResults(dialog, ticks, error);
			}
						
		} // Notify
		
	} // End DoDialogDoit
				
//-------------------------------------------------------------------------------
//
//	DoDialogUndo
//
//	When undo button is pressed, call generic routine to trigger undo event.
//
//-------------------------------------------------------------------------------
static void ASAPI DoDialogUndo
	(
	ADMItemRef item, 
	ADMNotifierRef notifier
	)
	{
	#ifdef __PIMWCWMac__
		#pragma unused(item)
	#endif
	if (sADMNotify->IsNotifierType(notifier, kADMUserChangedNotifier))
		{ // Correct notifier.  Do this:
		DoUndo ();
		}
	} // end DoDialogDoit

//-------------------------------------------------------------------------------
//
//	DoLogToFile
//
//	When the checkbox is toggled, we follow with our global.
//
//-------------------------------------------------------------------------------
static void ASAPI DoLogToFile
	(
	ADMItemRef item, 
	ADMNotifierRef notifier
	)
	{
	if (sADMNotify->IsNotifierType(notifier, kADMUserChangedNotifier))
		{ // Correct notifier.  Do this:
		bool logToFile = (sADMItem->GetIntValue(item) != 0);
		
		if (logToFile != gLogToFile)
			{
			// It is different.
			gLogToFile = logToFile;
		
			ADMDialogRef dialog = sADMItem->GetDialog(item);
			ADMItemRef textItem = sADMDialog->GetItem(dialog, kDLogToFile_staticText);
					
			if (gLogToFile)
				{
				char message[256] = ""; 
				char startingFile[256] = ""; 
				
				sADMBasic->GetIndexString(gPlugInRef, kQueryForFileID, 1, message, 256);
				sADMBasic->GetIndexString(gPlugInRef, kQueryDefaultFilenameID, 1, startingFile, 256);

				SPPlatformFileSpecification startingDir;
				SPPlatformFileSpecification result;
				
				sSPRuntime->GetRuntimePluginsFolder(&startingDir);
				
				ASBoolean success = sADMBasic->StandardPutFileDialog(message,
																	 &startingDir,
																	 startingFile,
																	 &result);
					
				if (success)
					{
					// Rip apart path (win) and name (mac) to display:
					
					char string[kMaxStr255Len] = "";
					
					ParseFilenameToDisplay(result, string);
					
					sADMItem->SetText(textItem, string);
					
					gFile = result;
					}
				else
					{
					// Oops, failed.
					gLogToFile = false;
					sADMItem->SetIntValue(item, 0);
					}
				}
			else
				{
				sADMItem->SetText(textItem, "");
				}
				
			} // logToFile different

		} // notifier
			
	} // end DoLogToFile

//-------------------------------------------------------------------------------
//
//	DoOtherEditText
//
//	Grab the other edit text when it changes.
//
//-------------------------------------------------------------------------------
static void ASAPI DoOtherEditText
	(
	ADMItemRef item, 
	ADMNotifierRef notifier
	)
	{
	if (sADMNotify->IsNotifierType(notifier, kADMUserChangedNotifier))
		{ // Correct notifier.  Do this:
	
		sADMItem->GetText(item, gOtherEvent, kMaxStr255Len);
		
		} // notifier
		
	} // end DoDialogDoit
		

//-------------------------------------------------------------------------------
//
//	DisplayResults
//
//	Displays the timing information.
//
//-------------------------------------------------------------------------------
void DisplayResults
	(
	ADMDialogRef dialog,
	const unsigned long ticks,
	const SPErr inError
	)
	{
	double ticksAsDouble = ticks;
	char resultString[256] = "";
	char tempString[256] = "";
	
	sADMBasic->GetIndexString(gPlugInRef, kDisplayResultsStringID, 1, resultString, 256);

	if (resultString != NULL)
		{
		char convertString1[kConvertStrLen];
		char convertString2[kConvertStrLen];
		char convertString3[kConvertStrLen];
		char errString[256] = "";
		
		if (inError == userCanceledErr)
			sADMBasic->GetIndexString(gPlugInRef, kDisplayResultsCancelStringID, 1, errString, 256);
		else if (inError == kPSErrorUnknownStringID)
			sADMBasic->GetIndexString(gPlugInRef, kDisplayResultsUnknownStringID, 1, errString, 256);
		else if (inError != kSPNoError)
			{
			sADMBasic->GetIndexString(gPlugInRef, kDisplayResultsUnknownErrorID, 1, errString, 256);

			if (errString != NULL)
				{
				
				LongToCString(inError, convertString1);
				
				sADMBasic->ValueToString
					(
					(float)inError,
					convertString1,
					kConvertStrLen,	// Maximum length.
					kADMNoUnits,
					0, 				// Precision.
					false			// Always append units?
					);
				strcpy(tempString, resultString);
				sprintf(errString, tempString, convertString1);
				}
			}
			
			strcpy(tempString, resultString);

		char nameString[kMaxStr255Len] = "";
		char* nameStr = gLastGroupList->lastEventList->name;
		
		if (sSPBasic->IsEqual(gLastGroupList->lastEventList->id, otherKey))
			{ // Use 4 character code instead of "Other":
			
			unsigned short size = strlen(gOtherEvent);
			
			if (size > 0)
				{
				if (size > 4)
					{ // Size is a string.
					nameStr = gOtherEvent;
					}
				else	// Size is a 4-character code.
					{
					nameString[0] = '\'';
					nameString[1] = 0;
					strncpy(&nameString[1],gOtherEvent,size);
					nameString[1+size] = '\'';
					nameString[2+size] = 0;
					
					nameStr = nameString;
					}
				}
				
			} // was not otherKey.
		
			LongToCString(ticks, convertString1);	
		sADMBasic->ValueToString
			(
			(float)ticks,
			convertString1,
			kConvertStrLen,	// Maximum length.
			kADMNoUnits,
			kPIUNoPrecision,
			false			// Always append units?
			);
		
		long mins = (long)(ticksAsDouble / 60.0 / 60.0);
		
		const unsigned char kPIUNoPrecision = 0; 

		sADMBasic->ValueToString
			(
			(float)mins,
			convertString2,
			kConvertStrLen,			// Maximum length.
			kADMNoUnits,
			kPIUNoPrecision,
			false					// Always append units?
			);
			
		ticksAsDouble -= mins*60*60; // Subtract any minutes.
		
		double secs = ticksAsDouble / 60.0;
		

		sADMBasic->ValueToString
			(
			(float)secs,
			convertString3,
			kConvertStrLen,		// Maximum length.
			kADMNoUnits,
			kPIUQuadPrecision, 
			false				// Always append units?
			);
						
		sprintf(resultString, tempString, errString, nameStr, convertString1, convertString2, convertString3);
			
		
		if (dialog != NULL)
			{
			ADMItemRef item = sADMDialog->GetItem(dialog, kDResults_staticText);
			sADMItem->SetText(item, resultString);
			}
			
		if (gLogToFile)
			AppendToFile(resultString);
		
		}

	} // DisplayResults
	
//-------------------------------------------------------------------------------
//
//	Save and Restore File globals.
//
//-------------------------------------------------------------------------------
static void SaveParameters(void)
	{
	saveLastGroupList = gLastGroupList;
	saveLastEventList = gLastGroupList->lastEventList;
	saveShowDialog = gShowDialog;
	saveLogToFile = gLogToFile;
	saveFile = gFile;
	}
	
static void RestoreParameters(void)
	{
	gLastGroupList = saveLastGroupList;
	gLastGroupList->lastEventList = saveLastEventList;
	gShowDialog = saveShowDialog;
	gLogToFile = saveLogToFile;
	gFile = saveFile;
	}

//-------------------------------------------------------------------------------

#if defined(__PIMac__) && defined(UsePlayNotifier)

//-------------------------------------------------------------------------------
//
// ADM automagically takes care of update events if you pop dialogs
// over yours during Play.  If you want to, or are managing these
// events yourself, you should use the WindowsNotify suite and
// instantiate a handler for events during Play.  An example
// might be:
//
//	sPSWindowNotify->AddNotify(window, MacPlayNotifier, NULL)
//
//-------------------------------------------------------------------------------
//
//	MacPlayNotifier
//
//	Process window events during playing of an event.
//
//-------------------------------------------------------------------------------
void MacPlayNotifier(const EventRecord* event, void* data)
	{
	WindowPtr 		window = (WindowPtr)event->message;
	
	// ADM doesn't have a WindowToDialog() routine, so we
	// do this little bit to get the number of currently
	// active ADM windows and use the most current window
	// as our dialog reference:
	
	ADMDialogRef 	dialog = NULL;
	
	long numberOfDialogs = 0;
	
	SPErr error = noErr;
	
	if (sADMDialogGroup.IsValid())
		{
		error = sADMDialogGroup->CountDialogs(&numberOfDialogs);
		
		if (error == noErr && numberOfDialogs > 0)
			{ // Nth dialog is 0-based.  CountDialogs is 1-based:
			sADMDialogGroup->GetNthDialog(numberOfDialogs-1, &dialog);
			}
		}
		
	if (data != NULL)
		{
		// This can be your userdata.  Hang anything off
		// here you need.
		}

	if (event->what == activateEvt)
		{
		// Sorry.  Don't respond to Activate events during this
		// notifier.  If we do, it's usually going to cause our
		// window to redraw in front of another window.  So we'll
		// just throw them on the floor.  But we would do:
		/*
		if (sADMDialog.IsValid())
			{
			sADMDialog->Activate(dialog, true);
			}
		*/
		}		

	else if (event->what == updateEvt)
		{
		if (sADMDialog.IsValid())
			{
			sADMDialog->Update(dialog);
			}
		}
	}
#endif
//-------------------------------------------------------------------------------
//
//	ParseFilenameToDisplay.
//
//	Takes a SPPlatformFileSpec and returns the shortened filename.
//
//-------------------------------------------------------------------------------
static SPErr ParseFilenameToDisplay
	(
	/* IN */	const SPPlatformFileSpecification fileSpec,
	/* OUT */	char displayString[kMaxStr255Len]
	)
	{
	SPErr error = kSPNoError;
	
	#ifdef __PIMac__
	
		strncpy(displayString, (const char*)&fileSpec.name[1], fileSpec.name[0]);
		displayString[fileSpec.name[0]] = 0;
	
	#elif defined(__PIWin__)
	
		_splitpath
			(
			fileSpec.path,
			NULL,			// drive,
			NULL,			// directory,
			displayString,	// filename
			NULL			// extension
			);
	#endif
	
	return error;
	
	} // ParseFilenameToDisplay

//-------------------------------------------------------------------------------
//
//	LongToCString
//	
//	Converts a signed long to a CString.
//
//-------------------------------------------------------------------------------
SPErr LongToCString
	(
	/* IN */	const long value,
	/* OUT */	char outString[kConvertStrLen]
	)
	{
	
	SPErr error = kSPNoError;
	
	if (outString != NULL)
		{
	 	outString[0] = 0;
	 	
		#ifdef __PIMac__
		
			Str255 pascalString = "";
			
			NumToString(value, pascalString);
			
			if (pascalString[0] >= kConvertStrLen)
				pascalString[0] = kConvertStrLen-1;
			
			strncpy(outString, (const char*)&pascalString[1], (unsigned short)pascalString[0]);
			outString[pascalString[0]] = 0;
		
		#elif defined(__PIWin__)
		
			ltoa(value, outString, 10 /* base 10 */);
		
		#endif
		
		}
	else
		{
		error = kSPBadParameterError;
		}
		
	return error;
	}	
//-------------------------------------------------------------------------------
// end TriggerFiltersUI.cpp
