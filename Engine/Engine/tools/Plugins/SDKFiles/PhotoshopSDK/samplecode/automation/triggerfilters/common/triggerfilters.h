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
//		TriggerFilters.h
//
//	Description:
//		This file contains the header prototypes and macros for the
//		Actions module TriggerFilters, an example of a module
//		that uses ADM and the Actions suite to execute events
//		from the Filter menu.
//
//	Use:
//		This is a good example of a basic shell to pop a cross-
//		platform UI using ADM and trigger basic events.
//
//-------------------------------------------------------------------------------

#ifndef __TriggerFilters_H__		// Has this not been defined yet?
#define __TriggerFilters_H__		// Only include this once by predefining it

#include "PIDefines.h"
#include "PIUActionUtils.h"
#include "piactionsplugin.h"			// Actions plug-in Photoshop header file
#include "TriggerFiltersTerminology.h"	// Terminology for this plug-in.

#include "piactions.h"
#include "admBasic.h"
#include "admDialog.h"
#include "admItem.h"
#include "admNotifier.h"
#include "admList.h"
#include "admTracker.h"
#include "admDrawer.h"
#include "admEntry.h"
#include "SPRuntme.h"
#include "piuihookssuite.h"
#include "pihandlesuite.h"
#if Macintosh
	#include <cstdio>
	#include <string.h>
#endif
#include "PIUSuites.h"
//-------------------------------------------------------------------------------
//	Globals -- structures
//-------------------------------------------------------------------------------


typedef enum showDialog_t
	{
	kShowDialogOnce = 0,
	kShowDialogAlways,
	kShowDialogNever
	} showDialog_t;
	
typedef struct mapKeyToShowDialog_t
	{
	DescriptorKeyID	key;
	showDialog_t	showDialog;
	short			resourceID;
	} mapKeyToShowDialog_t;

typedef struct eventList_t
	{
	unsigned short		index;
	char*				name;
	char*				id;
	PIActionDescriptor	descriptor;
	eventList_t*		next;
	} eventList_t;
	
typedef struct groupList_t
	{
	unsigned short		index;
	char*				name;
	short				id;
	eventList_t*		eventList;
	unsigned short		size;
	eventList_t*		lastEventList;
	ADMItemRef			eventListDialogItem;
	groupList_t*		next;
	} groupList_t;

// takes floating point numbers
typedef struct PIUDRect_t
	{
	double	top;
	double	left;
	double	bottom;
	double	right;
	} PIUDRect_t;

// takes floating point numbers
typedef struct PIUDPoint_t
	{
	double	h;
	double	v;
	} PIUDPoint_t;

extern groupList_t*		gGroupList;
extern groupList_t*		gLastGroupList;
extern unsigned short	gGroupList_size;

extern char 	gOtherEvent[];

extern showDialog_t	gShowDialog;

extern bool gLogToFile;
extern SPPlatformFileSpecification gFile;

extern SPBasicSuite				*sSPBasic;
extern SPPluginRef				gPlugInRef;

extern AutoSuite<ADMBasicSuite3> sADMBasic;
extern AutoSuite<ADMDialogSuite5> sADMDialog;
extern AutoSuite<ADMItemSuite> sADMItem;
extern AutoSuite<ADMNotifierSuite> sADMNotify;
extern AutoSuite<ADMListSuite> sADMList;
extern AutoSuite<ADMEntrySuite> sADMEntry;
extern AutoSuite<ADMTrackerSuite> sADMTrack;
extern AutoSuite<ADMDrawerSuite> sADMDrawer;

//-------------------------------------------------------------------------------
//	Definitions -- Constants
//-------------------------------------------------------------------------------
const char kDelim[] = "|";
const char kNext[] = "+";
const char kSeparator[] = "-";			
	
const mapKeyToShowDialog_t mapKeyToShowDialog[] =
	{
		{ keyDialogOnce, kShowDialogOnce, kShowDialogStringOnceID },
		{ keyDialogAlways, kShowDialogAlways, kShowDialogStringAlwaysID },
		{ keyDialogNever, kShowDialogNever, kShowDialogStringNeverID }
	};
	
const unsigned short mapKeyToShowDialog_size = 
	sizeof(mapKeyToShowDialog) / sizeof(mapKeyToShowDialog_t);

const int32 kMaxStr255Len = 255; // Maximum standard string length. (Pascal, etc.)

//-------------------------------------------------------------------------------
//	Prototypes
//-------------------------------------------------------------------------------

SPErr Execute(PIActionParameters*);							// Main routine.
SPErr DoUI ();								// Show the UI.

SPErr ReadScriptParams (PIActionParameters*);					// Read any scripting params.
SPErr WriteScriptParams (PIActionParameters*);					// Write any scripting params.

void DisplayResults
	(
	ADMDialogRef dialog,
	const unsigned long ticks,
	const SPErr inError
	);
	
SPErr AppendToFile(const char* const string);

SPErr PlayFilter(PIActionDescriptor* descriptor, unsigned long* ticks);
void DoUndo (void);

SPErr MakeGroupList(void);
eventList_t* MakeFilterList(short id, unsigned short* index);
void FreeGroupList(void);
void FreeFilterList (eventList_t** beginEventList);

eventList_t* FindNameInEventList
	(
	/* IN */		const char* const name,
	/* IN */		eventList_t* eventList
	);
	
eventList_t* FindIndexInEventList
	(
	/* IN */		const unsigned short index,
	/* IN */		eventList_t* eventList
	);
	
eventList_t* FindIDInEventList
	(
	/* IN */		const char* const id,
	/* IN */		eventList_t* eventList
	);

groupList_t* FindIDInGroupList
	(
	/* IN */		const short id
	);

groupList_t* FindIndexInGroupList
	(
	/* IN */		const unsigned short index
	);
	
groupList_t* FindNameInGroupList
	(
	/* IN */		const char* const name
	);

bool ValidFileSpec(const SPPlatformFileSpecification fileSpec);
//-------------------------------------------------------------------------------

#endif // __TriggerFilters_H__
