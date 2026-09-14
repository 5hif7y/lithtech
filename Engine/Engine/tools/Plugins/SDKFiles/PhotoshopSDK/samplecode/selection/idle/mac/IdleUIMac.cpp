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
//		IdleUIMac.cpp
//
//	Copyright 1996-1997, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Description:
//		This file contains the dialog and platform-specific functions
//		for the Import module Idle, a very simple example of a module
//		that just idles and sends update events.
//
//	Use:
//		This import module is good as an example of a module
//		that forces the background application to redraw
//		its windows.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "PIDefines.h"
#include "Idle.h"

#include <Events.h> // specific for Mac, need to create a redraw event

//-------------------------------------------------------------------------------
//	Definitions -- Dialog items
//-------------------------------------------------------------------------------

static const uint8 kDAmount = 4;
static const uint8 kDShowProgress = 5;
static const uint8 kDSendUpdates = 7;

//-------------------------------------------------------------------------------
//	Prototypes.
//-------------------------------------------------------------------------------

// Validate all numeric dialog text items:
static short Validate (DialogPtr dp, 
			  		   short *item, 
					   double *dx, 
					   Boolean useAlert);
			
// Set all fields / buttons in UI to current global values:	
static void UpdateUI (GPtr globals, DialogPtr dp);

//-------------------------------------------------------------------------------
//
//	DoAbout
//
//	Displays the About box.
//
//	Inputs:
//		AboutRecordPtr about	Is actually a pointer to some platform
//								specific data for Windows only.
//		AboutID					#define with ID of String resource to use for text
//
//	Outputs:
//		Displays About box (Dialog).  Press Escape, Enter, or Return to
//		dismiss, or click the mouse.
//
//-------------------------------------------------------------------------------

void DoAbout (AboutRecordPtr /*about*/)
{
	ShowAbout (AboutID);
} // end DoAbout

//----------------------------------------------------------------------------
//
//	IdleGetTicks
//
//	Return the ticks or some semblence thereof.
//
//	I'm doing this as a function and not a macro because I have a feeling
//	the Windows version is going to need a bit of coersion.
//
//	Inputs:
//		none
//
//	Outputs:
//		uint32		Current ticks count since machine was turned on.
//
//----------------------------------------------------------------------------

uint32 IdleGetTicks()
{
	return (uint32)TickCount();
}

//----------------------------------------------------------------------------
//
//	IdleGetTicksRate
//
//	Return the amount of ticks per second.
//
//	I'm doing this as a function and not a macro because I have a feeling
//	the Windows version is going to need a bit of coersion.
//
//	Inputs:
//		none
//
//	Outputs:
//		double		Number of ticks per second (60 on Mac, 1000 on Windows).
//
//----------------------------------------------------------------------------

double IdleGetTicksRate()
{
	return 60.0; // 60 fields per second
}

//----------------------------------------------------------------------------
//
//	IdleGetProgressRate
//
//	Return the amount of ticks per progress bar indicator.
//
//	The progress indicator is a bit touchy with extremely large numbers,
//	so, depending on the ticks values, divide by some magic number to get
//	the ticks rate into some useable progress value.
//
//	I'm doing this as a function and not a macro because I have a feeling
//	the Windows version is going to need a bit of coersion.
//
//	Inputs:
//		none
//
//	Outputs:
//		double		Number of ticks per second divided by good number
//					to use for progress (60 / 2.0 on Mac,
//					1000 / 4.0 on Windows).
//
//----------------------------------------------------------------------------

double IdleGetProgressRate()
{
	return IdleGetTicksRate() / 2.0; // smaller so progress indicator can handle it
}

//----------------------------------------------------------------------------
//
//	IdleSendUpdate
//
//	Send an update/redraw event to the application to redraw all its
//	windows.
//
//	Inputs:
//		GPtr globals	Pointer to globals structure.
//
//	Outputs:
//		Posts an update event and grabs that event from the event queue
//		and passes it to the app.  On Macintosh, this is done via
//		the processEvent callback.  On Windows, this is done via
//		the PostMessage OS call.
//
//----------------------------------------------------------------------------

void IdleSendUpdate (GPtr globals)
{
	if (gStuff->processEvent != NULL)
	{
		EventRecord er;
		WindowPtr window = 0;
		static Boolean doneEval = false;
		GrafPtr savePort = 0;
		
		GetPort( &savePort );
		window = FrontWindow ();
		
		if (!doneEval)
		{
			doneEval = true;
			//InvalRect(&(window->portRect));
			PostEvent(updateEvt, (long)window); // just post one event
		}
		
		GetNextEvent(updateMask, &er);

		(*(gStuff->processEvent))(&er);
		
	} // gStuff->processEvent

} // end IdleSendUpdate

//-------------------------------------------------------------------------------
//
//	UpdateUI
//
//	Updates all the fields of the Parameters dialog to the default values.
//
//	Inputs:
//		GPtr globals			Pointer to global structure.
//
//		DialogPtr dp			Pointer to current dialog.
//
//		Boolean gUseProgress	State to set check box.
//
//		Boolean gSendUpdates	State to set check box.
//
//		Double gIdleAmount		Number for idle seconds text entry.
//
//	Outputs:
//		Updates parameters dialog fields with values.  Sets check boxes
//		and radio groups to correct on/off states.
//
//-------------------------------------------------------------------------------

static void UpdateUI (GPtr globals, DialogPtr dp)
{
	if (dp != NULL)
	{	
		SetCheckBoxState (dp, kDShowProgress, gUseProgress);
		SetCheckBoxState (dp, kDSendUpdates, gSendUpdates);

		StuffDouble (dp, kDAmount, gIdleAmount, kDPrecision);
	}
	
} // end UpdateUI

//-------------------------------------------------------------------------------
//
//	Validate
//
//	Validates numeric fields of the Parameters dialog.
//
//	Inputs:
//		DialogPtr dp			Pointer to dialog.
//
//		short *item				Current item selected.
//
//		Boolean useAlert		TRUE = Pop alert if value out of range.
//								FALSE = Silently coerce value.
//
//	Outputs:
//		short *item				If alert popped, set to 0.  Otherwise,
//								unchanged.
//
//		double *dx				Value or coerced value.
//
//		returns short err		Enum of either noErr, errOutOfRange, or
//								errNotANumber.
//
//-------------------------------------------------------------------------------

static short Validate (DialogPtr dp, 
					   short *item, 
					   double *dx, 
					   Boolean useAlert)
{
	short 	numberErr = noErr;
	long 	x = 0; // returned long
	short 	loop = 1;
	
	const double propDoubles[] =
	{
		kDAmount, kDAmountMin, kDAmountMax
	};

	const short	numPropDoubles = 
		(propDoubles[0] > 0) ? ((sizeof(propDoubles) / sizeof(double)) / 3) : 0;

	const long	propNumbers[] =
	{
		0, 0, 0
	};

	const short	numPropNumbers = 
		(propNumbers[0] > 0) ? ((sizeof(propNumbers) / sizeof(long)) / 3) : 0;
	
	const short foundIt = -1; // flag that we found it

	
	// first loop through doubles, looking for match
	while (loop <= numPropDoubles && loop != foundIt)
	{
		if (propDoubles[loop*3-3] == *item)
		{
			numberErr = FetchDouble (dp,
									 *item,
									 propDoubles[loop*3-2], // max
									 propDoubles[loop*3-1], // min
									 dx); // returned double
			if (numberErr != noErr && useAlert)
			{ // out of bounds problem.  Report and change focus back.
				AlertDouble (dp,
							 *item,
							 propDoubles[loop*3-2], // max
							 propDoubles[loop*3-1], // min
							 dx, // value
							 AlertID, // global alert dialog
							 numberErr); // error we found
				*item = 0;
			}
			loop = foundIt; // found it!
		} else loop++; // next
	}

	if (loop != foundIt) loop = 1; // reset
	// now loop through longs, looking for match
	while (loop <= numPropNumbers && loop != foundIt)
	{
		if (propNumbers[loop*3-3] == *item)
		{
			numberErr = FetchNumber (dp,
									 *item,
									 propNumbers[loop*3-2], // max
									 propNumbers[loop*3-1], // min
									 &x); // returned long
			if (numberErr != noErr && useAlert)
			{ // out of bounds problem.  Report and change focus back.
				AlertNumber (dp,
							 *item,
							 propNumbers[loop*3-2], // max
							 propNumbers[loop*3-1], // min
							 &x, // value
							 AlertID, // global alert dialog
							 numberErr); // error we found
				*item = 0;
			}
			*dx = (double)x; // coerce long as returned double
			loop = foundIt; // found it!
		} else loop++; // next
	}
	return numberErr;

} // end Validate


//-------------------------------------------------------------------------------
//
//	DoUI
//
//	Displays the Parameters dialog
//
//	Inputs:
//		GPtr globals		Pointer to global structure.
//
//	Outputs:
//		Displays parameters dialog.
//
//		returns TRUE		If displaying dialog succeeded, user picked an
//							option, and clicked OK.
//		returns FALSE		If displaying dialog failed or user clicked CANCEL
//
//-------------------------------------------------------------------------------

Boolean DoUI (GPtr globals)
{
	long		x = 1;
	double		dx = 0;
	short		numberErr = noErr;
	short		item, lastItem = 0, lastTitem = kDAmount;
	DialogPtr	dp;
	DialogTHndl	dt;
	short		gotErr = 0;
	
	const Boolean		uUseProgress = gUseProgress;
	const Boolean		uSendUpdates = gSendUpdates;
	const double		uIdleAmount = gIdleAmount;

	dt = (DialogTHndl) GetResource ('DLOG', uiID);
	HNoPurge ((Handle) dt);
	
	CenterDialog (dt);

	dp = GetNewDialog (uiID, nil, (WindowPtr) -1);

	(void) SetDialogDefaultItem (dp, ok);
	(void) SetDialogCancelItem (dp, cancel);
	(void) SetDialogTracksCursor (dp, TRUE);
		
	UpdateUI (globals, dp);
	
	SelectTextItem (dp, lastTitem);
	
	ShowWindow (GetDialogWindow(dp));

	do
	{
		
		MoveableModalDialog (dp, gStuff->processEvent, nil, &item);
	
		if (lastItem != item &&
			item != cancel)
		{ // if its something that doesn't blast away everything else,
		  // we need to go back and validate the last field, then.
			x = lastItem;
			switch (lastItem)
			{ // We just left this field, so check for valid bounds.	
				case kDAmount:
					if ((numberErr = Validate(dp, &lastItem, &dx, true)) != noErr)
						item = x; // force this number to be stored in next switch
					break;

			} // end switch
		} // end if
		
		switch (item)
		{ // now check what item we just clicked/used
			case kDShowProgress:
				gUseProgress = ToggleCheckBoxState(dp, item);
				SelectTextItem(dp, lastTitem);
				break;
			case kDSendUpdates:
				gSendUpdates = ToggleCheckBoxState(dp, item);
				SelectTextItem(dp, lastTitem);
				break;
			case kDAmount:
				numberErr = Validate(dp, &item, &dx, false);
				gIdleAmount = dx;
				lastTitem = item;
				break;
		} // end switch
		lastItem = item;
		
	} while (item != ok && item != cancel);

	DisposeDialog (dp);
	HPurge ((Handle) dt);

	if (item == cancel)
	{ // return to orig params
		gIdleAmount = uIdleAmount;
		gUseProgress = uUseProgress;
		gSendUpdates = uSendUpdates;
		gResult = userCanceledErr;
	}

	return (item == ok);

} // end DoUI

//-------------------------------------------------------------------------------

// end IdleUIMac.cpp