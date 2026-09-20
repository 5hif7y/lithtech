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
//		Idle.h
//
//
//	Description:
//		This file contains the header prototypes and macros for the
//		Import module Idle, a very simple example of a module
//		that just idles and sends update events.
//
//	Use:
//		This import module is good as an example of a module
//		that forces the background application to redraw
//		its windows.
//
//-------------------------------------------------------------------------------

#ifndef __Idle_H__			// Has this not been defined yet?
#define __Idle_H__			// Only include this once by predefining it

#include "piselection.h"		// Selection Photoshop header file.
#include "PIUtilities.h"		// SDK Utility library.
#include "IdleTerminology.h"	// Terminology for this plug-in.

//-------------------------------------------------------------------------------
//	Definitions -- Constants
//-------------------------------------------------------------------------------

#define kDAmountMin		0.0
#define kDAmountMax		600.0
#define kDPrecision		4

//-------------------------------------------------------------------------------
//	Globals -- structures
//-------------------------------------------------------------------------------

typedef struct Globals
{ // This is our structure that we use to pass globals between routines:

	short				*result;				// Must always be first in Globals.
	PISelectionParams	*selectionParamBlock;	// Must always be second in Globals.

	short				queryForParameters;
	double				idleAmount;
	Boolean				useProgress;
	Boolean 			sendUpdates;

} Globals, *GPtr, **GHdl;				// *GPtr = global pointer; **GHdl = global handle

//-------------------------------------------------------------------------------
//	Globals -- definitions and macros
//-------------------------------------------------------------------------------

#define gResult				(*(globals->result))
#define gStuff  			(globals->selectionParamBlock)
#define gQueryForParameters (globals->queryForParameters)

#define gIdleAmount			(globals->idleAmount)
#define gUseProgress		(globals->useProgress)
#define gSendUpdates		(globals->sendUpdates)

//-------------------------------------------------------------------------------
//	Prototypes
//-------------------------------------------------------------------------------

void InitGlobals (Ptr globalPtr);		  	// Initialize globals.
void DoAbout (AboutRecordPtr about); 	    // Pop about box.
void DoExecute (GPtr globals);				// Main routine.

void ValidateParameters (GPtr globals);		// Validates (inits) parameters.
Boolean DoUI (GPtr globals);				// Show the UI.

Boolean ReadScriptParams (GPtr globals);	// Read any scripting params.
OSErr WriteScriptParams (GPtr globals);		// Write any scripting params.

void CopyExistingSelection (GPtr globals); 	// Copies selection so things stay the same

void DoIdle (GPtr globals);					// Main idle routine.
uint32 IdleGetTicks (void);					// Get current ticks.
double IdleGetTicksRate (void);				// Ticks per second.
double IdleGetProgressRate (void);			// Ticks per second for progress.
void IdleSendUpdate (GPtr globals);			// Force update application windows.

//-------------------------------------------------------------------------------

#endif // __Idle_H__
