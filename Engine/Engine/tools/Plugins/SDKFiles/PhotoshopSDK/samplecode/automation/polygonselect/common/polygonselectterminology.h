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
//		PolygonSelectTerminology.h
//
//	Copyright 1999-1999, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Description:
//		Resource and code definitions for Polygon Selection plug-in. 
//
//-------------------------------------------------------------------------------

#ifndef __PolygonSelectTerminology_h__
#define __PolygonSelectTerminology_h__


//-------------------------------------------------------------------------------
//	Definitions -- Required by include files.
//-------------------------------------------------------------------------------

// The About box and resources are created in DialogUtilities.r.
// You can easily override them, if you like.

#define plugInName			"PolygonSelect"
#define plugInCopyrightYear	"1999"
#define plugInDescription \
	"An example Actions Module to alter selection in Adobe Photoshop¨."

//-------------------------------------------------------------------------------
//	Definitions -- Scripting keys
//-------------------------------------------------------------------------------

// All the keys in this plug-in, keyOffset, keyTop, keyLeft, keyRight, and keyBottom are
// defined by Photoshop, except for one:
#define keyNumberSides	'Nbsd'

//-------------------------------------------------------------------------------
//	Definitions -- Resources
//-------------------------------------------------------------------------------
// Have to do it this way, and not with an enum, because the Windows
// precompiler does not do the right thing when precompiling the .r file:
#define ResourceID				16000
#define kDialogID				ResourceID+1
#define kResetStringID			ResourceID+10
#define kCancelStringID			kResetStringID+1

//-------------------------------------------------------------------------------

#endif // __PolygonSelectTerminology_h__
