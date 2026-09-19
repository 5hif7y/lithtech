// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2002 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
//-------------------------------------------------------------------------------
//
//	File:
//		PolygonSelect.h
//
//	Copyright 1999-1999, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Description:
//		Creates a polygon-shaped selection.
//
//-------------------------------------------------------------------------------

#ifndef __PolygonSelect_H__		// Has this not been defined yet?
#define __PolygonSelect_H__		// Only include this once by predefining it

#include "PolygonSelectTerminology.h"		// Terminology for this plug-in.
#include "piactionsplugin.h"			// Actions plug-in Photoshop header file
#include "piproperties.h"				// Properties and ruler units.
#include <math.h>

#include "piactions.h"
#include "PIUI.h"

#include "SPTypes.h"
#include "SPAccess.h"
#include "SPInterf.h"

//-------------------------------------------------------------------------------
//	Definitions -- Constants
//-------------------------------------------------------------------------------
const double kUnitDistanceBasePerInch = 72.0;	// Used by ConvertUnit routines.

//-------------------------------------------------------------------------------
//	Globals -- structures
//-------------------------------------------------------------------------------
extern long 	gRadius;
extern long 	gNumberSides;
extern double gRotationAngle;


extern SPBasicSuite							*sSPBasic;
extern SPPluginRef							gPlugInRef;

//A point that takes floating point numbers
typedef struct PIUDPoint_t
	{
	double	h;
	double	v;
	} PIUDPoint_t;
extern PIUDPoint_t	gDocumentBounds;


//-------------------------------------------------------------------------------
//	Global prototypes
//-------------------------------------------------------------------------------

SPErr DoUI ();								// Show the UI.

SPErr ReadScriptParams (PIActionParameters*);					// Read any scripting params.
SPErr WriteScriptParams (PIActionParameters*);					// Write any scripting params.


//-------------------------------------------------------------------------------

#endif // __PolygonSelect_H__
