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
//		FrameSelect.h
//
//	Description:
//		Create a new document.
//
//-------------------------------------------------------------------------------

#ifndef __FrameSelect_H__		// Has this not been defined yet?
#define __FrameSelect_H__		// Only include this once by predefining it

#include "FrameSelectTerminology.h"		// Terminology for this plug-in.
#include "piactionsplugin.h"			// Actions plug-in Photoshop header file
#include "piproperties.h"				// Properties and ruler units.

#include "piactions.h"
#include "PIUSuites.h"
#include "admBasic.h"
#include "admDialog.h"
#include "admItem.h"
#include "admNotifier.h"
#include "admList.h"
#include "admTracker.h"
#include "admDrawer.h"
#include "admEntry.h"
//-------------------------------------------------------------------------------
//	Definitions -- Constants
//-------------------------------------------------------------------------------
const double kUnitDistanceBasePerInch = 72.0;	// Used by ConvertUnit routines.

//-------------------------------------------------------------------------------
//	Globals -- structures
//-------------------------------------------------------------------------------
// takes floating point numbers
typedef struct PIUDRect_t
	{
	double	top;
	double	left;
	double	bottom;
	double	right;
	} PIUDRect_t;

//takes floating point numbers
typedef struct PIUDPoint_t
	{
	double	h;
	double	v;
	} PIUDPoint_t;

extern SPBasicSuite* sSPBasic;

extern SPPluginRef gPlugInRef;

extern AutoSuite<ADMBasicSuite3> sADMBasic;
extern AutoSuite<ADMDialogSuite5> sADMDialog;
extern AutoSuite<ADMItemSuite> sADMItem;
extern AutoSuite<ADMNotifierSuite> sADMNotify;
extern AutoSuite<ADMListSuite> sADMList;
extern AutoSuite<ADMEntrySuite> sADMEntry;
extern AutoSuite<ADMTrackerSuite> sADMTrack;
extern AutoSuite<ADMDrawerSuite> sADMDrawer;

extern PIUDRect_t  gRect;
extern RulerUnits gRulerUnits;
extern double 	gAmount;

extern RulerUnits 	gDocumentRulerUnits;
extern PIUDPoint_t	gDocumentBounds;
extern double		gDocumentMinBound;
extern PIUDPoint_t	gDocumentOrigin;
extern double		gDocumentResolution;

//-------------------------------------------------------------------------------
//	Global prototypes
//-------------------------------------------------------------------------------

SPErr DoUI ();								// Show the UI.

SPErr ReadScriptParams (PIActionParameters*);					// Read any scripting params.
SPErr WriteScriptParams (PIActionParameters*);					// Write any scripting params.

SPErr SetRectFromGutter
	(
	/* IN */	const double gutter,
	/* OUT */	PIUDRect_t* outRect
	);

// Convert a double rect to given units:
PIUDRect_t ConvertRect
	(
	/* IN */		const PIUDRect_t inRect,			// Input rect.
	/* IN */		const double resolutionPerInch,		// Resolution of document in pixels per inch.
	/* IN */		const PIUDPoint_t documentBounds,	// For percent, a bounds of the document in unitDistance.
	/* IN */		const RulerUnits inUnits,			// Incoming units.
	/* IN */		const RulerUnits outUnits			// Desired outgoing units.
	);
	
// Convert a double rect to or from unit distance, which is a special type of pixels unit:
PIUDRect_t ConvertRectToUnitDistance
	(
	/* IN */		const PIUDRect_t inRect,			// Input rect.
	/* IN */		const double resolutionPerInch,		// Resolution of document in pixels per inch.
	/* IN */		const PIUDPoint_t documentBounds,	// For percent, a bounds of the document in unitDistance.
	/* IN */		const RulerUnits inUnits			// Incoming units.
	);
	
PIUDRect_t ConvertRectFromUnitDistance
	(
	/* IN */		const PIUDRect_t inRect,			// Input rect.
	/* IN */		const double resolutionPerInch,		// Resolution of document in pixels per inch.
	/* IN */		const PIUDPoint_t documentBounds,	// For percent, a bounds of the document in unitDistance.
	/* IN */		const RulerUnits outUnits			// Desired units.
	);

double ConvertFromUnitDistanceToUnits
	(
	const double value, 				// Incoming value.
	const double resolutionPerInch,		// Resolution of document in pixels per inch.
	const double documentBound,			// For percent.
	const RulerUnits outUnits			// Desired units.
	);
	
double ConvertFromUnitsToUnitDistance
	(
	const double value, 				// Incoming value.
	const double resolutionPerInch,		// Resolution of document in pixels per inch.
	const double documentBound,			// For percent.
	const RulerUnits inUnits			// Desired units.
	);

//-------------------------------------------------------------------------------

#endif // __FrameSelect_H__
