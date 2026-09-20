// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2003 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
/*
 * Name:
 *	PIUIHooksSuite.h
 *
 * Purpose:
 *	This file contains all the public definitions and
 *	structures related to user interface elements.
 *
 *	Use the UIHooks suite to get the main application
 *	window (on Windows), access a handful of Photoshop
 *	cursors, and similar utilities.
 *
 * Distribution:
 *	PUBLIC
 *
 * Version history:
 *		Version 1.0.0	1-Jan-1998	JF		Created for Photoshop 5.0.
 *			Compiled by Josh Freeman.
 *		Version 2.0.0	30-Jun-2003	Ace		Updated for Photoshop CS.
 *			Added SetPluginMenuEntry () and bumped to verison 2 of suite.
 */

#ifndef __PIUIHooksSuite__
#define __PIUIHooksSuite__

#if PRAGMA_ONCE
#pragma once
#endif

//-------------------------------------------------------------------------------
//	Includes.
//-------------------------------------------------------------------------------
#include "pigeneral.h"
#include "piactions.h"
#include "SPTypes.h"

//-------------------------------------------------------------------------------
//	C++ wrapper.
//-------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------
//	UI hooks suite.
//-------------------------------------------------------------------------------
#define kPSUIHooksSuite 			"Photoshop UIHooks Suite for Plug-ins"

//-------------------------------------------------------------------------------
//	Error codes.
//-------------------------------------------------------------------------------
#define kPSCursorError				'!cur'

//-------------------------------------------------------------------------------
//	SetCursor.  Sets cursor to some popular Photoshop cursors.  Mac and Win.
//-------------------------------------------------------------------------------

// Cursor IDs:
#define kPICursorNone				NULL
#define kPICursorArrow				"Photoshop Cursor Arrow"
#define kPICursorWatch				"Photoshop Cursor Watch"
#define kPICursorWatchContinue		"Photoshop Cursor Watch Continue"
#define kPICursorMove				"Photoshop Cursor Move"
#define kPICursorHand				"Photoshop Cursor Hand"
#define kPICursorGrab				"Photoshop Cursor Grab"
#define kPICursorZoomIn				"Photoshop Cursor Zoom In"
#define kPICursorZoomOut			"Photoshop Cursor Zoom Out"
#define kPICursorZoomLimit			"Photoshop Cursor Zoom Limit"
#define kPICursorPencil				"Photoshop Cursor Pencil"
#define kPICursorEyedropper			"Photoshop Cursor Eyedropper"
#define kPICursorEyedropperPlus		"Photoshop Cursor Eyedropper Plus"
#define kPICursorEyedropperMinus	"Photoshop Cursor Eyedropper Minus"
#define kPICursorBucket				"Photoshop Cursor Bucket"
#define kPICursorCrosshair			"Photoshop Cursor Crosshair"
#define kPICursorType				"Photoshop Cursor Type"
#define kPICursorPathArrow			"Photoshop Cursor Path Arrow"

typedef char* PICursor_t;
	
typedef SPAPI SPErr (*HostSetCursorProc) (const PICursor_t cursorID);
	
//-------------------------------------------------------------------------------
//	MainAppWindow.  Returns Windows parent window HWND, NULL on Mac.
//-------------------------------------------------------------------------------
typedef SPAPI long (*MainAppWindowProc) (void);

//-------------------------------------------------------------------------------
//	TickCount.  Gets the tick count as 60 ticks per second.  Mac and Win.
//-------------------------------------------------------------------------------
typedef SPAPI unsigned long (*HostTickCountProc) (void);

//-------------------------------------------------------------------------------
//	GetPluginName.  Returns the name of the plugins specified by pluginRef.
//-------------------------------------------------------------------------------
typedef SPAPI SPErr (*PluginNameProc) (SPPluginRef pluginRef, ASZString *pluginName);

//-------------------------------------------------------------------------------
//	UI hooks suite version 1.
//-------------------------------------------------------------------------------
#define kPSUIHooksSuiteVersion1		1

typedef struct 
	{
	ProcessEventProc 				processEvent;
	DisplayPixelsProc				displayPixels;
	ProgressProc					progressBar;
	TestAbortProc					testAbort;
	MainAppWindowProc				MainAppWindow;
	HostSetCursorProc				SetCursor;
	HostTickCountProc				TickCount;
	PluginNameProc					GetPluginName;

	} PSUIHooksSuite1;

//-------------------------------------------------------------------------------
//	SetPluginMenuEntry.  Sets the menu entry for the Last Filter, Undo, Redo, and
//	History state for only the last filter event.  (Have to set it again on each
//	invocation, if you want it to persist.)  Pass the SPPluginRef to the current
//	plug-in only.
//
//  New in version 2.
//-------------------------------------------------------------------------------
typedef SPAPI SPErr (*SetPluginMenuEntryProc)
	(
	const SPPluginRef self,			// Reference to calling (active) filter plug-in.
	const ASZString menuEntry,		// String to display in Undo, Redo, Last Filter, and History.
	const DescriptorEventID eventID	// Event ID to use for any descriptor returned.
	);

//-------------------------------------------------------------------------------
//	UI hooks suite version 2.
//-------------------------------------------------------------------------------

#define kPSUIHooksSuiteVersion2		2

typedef struct 
	{
	ProcessEventProc 				processEvent;
	DisplayPixelsProc				displayPixels;
	ProgressProc					progressBar;
	TestAbortProc					testAbort;
	MainAppWindowProc				MainAppWindow;
	HostSetCursorProc				SetCursor;
	HostTickCountProc				TickCount;
	PluginNameProc					GetPluginName;
	SetPluginMenuEntryProc			SetPluginMenuEntry;

	} PSUIHooksSuite2;

//-------------------------------------------------------------------------------
//	C++ wrapper.
//-------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

//-------------------------------------------------------------------------------
#endif	// PIUIHooksSuite
