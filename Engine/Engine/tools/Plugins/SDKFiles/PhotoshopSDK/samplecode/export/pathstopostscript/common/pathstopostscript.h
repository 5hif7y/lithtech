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
//		PathsToPostScript.h
//
//	Copyright 1990-1992, Thomas Knoll.
//	All Rights Reserved.
//
//	Description:
//		This file contains the headers and prototypes
//		for the Export module PathsToPostScript, a module that
//		creates a file with PostScript information
//		detailing a specific path, or all paths.  It
//		is taken from the source of the "Paths to Illustrator"
//		plug-in that ships with the product.
//
//	Use:
//		This module shows how to convert paths accessed
//		from the properties suite into PostScript and save
//		them to a file.  You access it via
//		File>>Export>>PathsToPostScript.
//
//-------------------------------------------------------------------------------

#ifndef __PathsToPostScript_H__		// Has this been defined yet?
#define __PathsToPostScript_H__		// Only include once by predefining it.

#include "piexport.h"						// Export Photoshop header file.
#include "PIUtilities.h"					// SDK Utility library.
#include "piproperties.h"					// Photoshop Properties suite.
#include "PathsToPostScriptTerminology.h"	// Terminology for this plug-in.

//-------------------------------------------------------------------------------
//	Definitions -- Dialog items
//-------------------------------------------------------------------------------

// Pop up menu items

#define iDocBounds			1
#ifdef __PIWin__
	#define iAllPaths		2
	#define iPathStart		3
#else // we can do separators, so use them
	#define iAllPaths		3
	#define iPathStart		5
#endif

//-------------------------------------------------------------------------------
//	Definitions -- Enumerations -- Path types
//-------------------------------------------------------------------------------

enum
{
	kClosedSubpathLength,
	kSmoothClosedKnot,
	kCuspClosedKnot,
	kOpenSubpathLength,
	kSmoothOpenKnot,
	kCuspOpenKnot
};

#define kMinPathKind kClosedSubpathLength
#define kMaxPathKind kCuspOpenKnot

//-------------------------------------------------------------------------------
//	Definitions -- Structures -- Path element
//-------------------------------------------------------------------------------

// NOTE: Make sure alignment is Mac68k.

typedef struct SavedPathElement
{
	int16	kind;
	int32	value [6];
} SavedPathElement;

#define kHardPathLength 26 // absolute size for a path

//-------------------------------------------------------------------------------
//	Globals -- structures
//-------------------------------------------------------------------------------

typedef struct Globals
{ // This is our structure that we use to pass globals between routines:

	short					*result;				// Must always be first in Globals.
	ExportRecord			*exportParamBlock;		// Must always be second in Globals.

	Boolean					queryForParameters;
	Boolean 				sameNames;
	short 					fRefNum;	
	short 					popUpValue;
	
	Fixed 					docHeight;
	Fixed 					docWidth;
	
	// AliasHandle on Mac, Handle on Windows:
	PIPlatformFileHandle	aliasHandle;
	#if Macintosh
	StandardFileReply		reply;
	#endif

} Globals, *GPtr, **GHdl;

//-------------------------------------------------------------------------------
//	Globals -- definitions and macros
//-------------------------------------------------------------------------------

#define gResult 				(*(globals->result))
#define gStuff  				(globals->exportParamBlock)

#define gQueryForParameters		(globals->queryForParameters)

#define gDocHeight 				(globals->docHeight)
#define gDocWidth				(globals->docWidth)

#define gFRefNum 				(globals->fRefNum)
#define gAliasHandle			(globals->aliasHandle)
#define gSameNames				(globals->sameNames)

#define gPopUpValue 			(globals->popUpValue)

#define gReply					(globals->reply)

//-------------------------------------------------------------------------------
//	Prototypes
//-------------------------------------------------------------------------------

Boolean ReadScriptParams (GPtr globals);	// Read any scripting params.
OSErr WriteScriptParams (GPtr globals);		// Write any scripting params.

void DoAbout (AboutRecordPtr about); 	    // Pop about box.
Boolean DoUI (GPtr globals);				// Show the UI.

// Boolean QueryForExportFile (GPtr globals);

void ValidateParameters (GPtr globals);

Boolean PromptForFile (GPtr globals);

Boolean CreateExportFile (GPtr globals);
Boolean WriteExportFile (GPtr globals);
Boolean CloseExportFile (GPtr globals);

void DisplayStopAlert (GPtr globals, short id);
Boolean WriteTitle (GPtr globals);
void SmallFileName(Str255 s, const Str255 file);
void AppendStringAndString (Str255 s, Str255 append);
void AppendStringAndCharacter (Str255 s, unsigned char c);
Boolean GetPathCount (GPtr globals, int32 *count);
int16 GetPathIndex (GPtr globals, OSType pathProperty);
Boolean GetPathName (GPtr globals, int32 index, Str255 s);

Handle PopUp2Handle (GPtr globals, short popup);
short Handle2PopUp (GPtr globals, Handle h);

//-------------------------------------------------------------------------------

#endif // __PathsToPostScript_H__
