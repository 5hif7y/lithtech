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
/*
 * Name:
 *	PIProgressSuite.h
 *
 * Copyright 1986-2000 Adobe Systems Incorporated.
 * All Rights Reserved.
 *
 * Purpose:
 *	Define the Plug-in Progress Suite. 
 *
 * Distribution:
 *	PUBLIC
 *
 * Version history:
 *	1.0.0 4/1/1998	Ace	First Version.
 *		Created by Andy Coven.
 */
// $Id: //photoshop/8.0/photoshop/sharedinterfaces/PIProgressSuite.h#1 $ 
// $DateTime: 2003/07/08 15:07:21 $ 
// $Change: 75352 $ 
// $Author: byer $ 

#ifndef __PIProgressSuite__
#define __PIProgressSuite__

#if PRAGMA_ONCE
#pragma once
#endif

#include "pigeneral.h"

#include "aszstringsuite.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define kPSProgressSuite 			"Photoshop Progress Suite for Plug-ins"


//-------------------------------------------------------------------------
//	Progress suite typedefs 
//-------------------------------------------------------------------------


typedef SPErr (*Progress_DoProgress) (const ASZString zs, SPErr (*proc) (void *), void *refCon);


// Section off a portion of the unused progress bar for execution of a subtask.
// taskLength should be between 0.0 and 1.0. This routine returns the
// errorcode from proc.

typedef SPErr (*Progress_DoTask) (double taskLength, SPErr (*proc) (void *), void *refCon);


// Limit progress to a section of the progress bar based on executing
// segmentLength out of total steps. done is a counter of how much we've
// accomplished so far and will be incremented by segmentLength if the task
// succeeds. (We need done because as with DoTask, what we care about is
// the percentage of the remaining progress bar.)

typedef SPErr (*Progress_DoSegmentTask) (int32 segmentLength,
										 int32 *done,
										 int32 total,
										 SPErr (*proc) (void *),
										 void *refCon);


// Change the current text in the progress bar.

typedef void (*Progress_ChangeProgressText) (const ASZString zs);


// Perform a task using the preview sniffing logic. This aborts if we encounter
// an event meeting the indicated conditions. selector should be one of the
// following:
//		"up"		-- Process until mouse down or key stroke
//		"down"		-- Process until mouse is released
//		"paused"	-- Process until mouse is moved or released
// Passing null will cause the code to choose between "up" and "paused"
// dependent on the current mouse state. All other strings result in an
// immediate error.

typedef SPErr (*Progress_DoPreviewTask) (const char *selector,
										SPErr (*proc) (void *),
					 					void *refCon);


// Run a task with the watch cursor up.

typedef SPErr (*Progress_DoWatchTask) (SPErr (*proc) (void *), void *refCon);


// Run a task without the watch cursor. This doesn't actually take the watch
// cursor down, but it will keep the watch from spinning.

typedef SPErr (*Progress_DoSuspendedWatchTask) (SPErr (*proc) (void *), void *refCon);

typedef void (*Progress_ContinueWatchCursor) (void);


//-------------------------------------------------------------------------
//	Progress suite version 1
//-------------------------------------------------------------------------

#define kPSProgressSuiteVersion1		1

typedef struct 
{
	Progress_DoProgress					DoProgress;
	Progress_DoTask						DoTask;
	Progress_DoSegmentTask				DoSegmentTask;
	Progress_ChangeProgressText			ChangeProgressText;
	Progress_DoPreviewTask				DoPreviewTask;
	Progress_DoWatchTask				DoWatchTask;
	Progress_DoSuspendedWatchTask		DoSuspendedWatchTask;
	Progress_ContinueWatchCursor		ContinueWatchCursor;

//	The following are taken from from the parameter block and are defined in PIGeneral.h:
	TestAbortProc						TestAbort;
	ProgressProc						UpdateProgress;
		
} PSProgressSuite1;

/******************************************************************************/

#ifdef __cplusplus
}
#endif


#endif	// PIProgressSuite
