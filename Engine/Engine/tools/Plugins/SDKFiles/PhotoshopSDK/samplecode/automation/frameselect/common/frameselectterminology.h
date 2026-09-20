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
//		FrameSelectTerminology.h
//
//	Description:
//		Resource and code definitions for New Document plug-in. 
//
//-------------------------------------------------------------------------------

#ifndef __FrameSelectTerminology_h__
#define __FrameSelectTerminology_h__


//-------------------------------------------------------------------------------
//	Definitions -- Scripting keys
//-------------------------------------------------------------------------------

// All the keys in this plug-in, keyOffset, keyTop, keyLeft, keyRight, and keyBottom are
// defined by Photoshop, except for one:
#define keyMyGutter	'gutR'

#define vendorName			"AdobeSDK"
#define plugInName			"Frame Select"
#define plugInCopyrightYear	"1998"
#define plugInDescription \
	"An example Actions Module to alter selection in Adobe Photoshop¨."

#define plugInAETEComment 	"FrameSelect example actions plug-in"

#define plugInSuiteID		'sdKG'
#define plugInClassID		plugInSuiteID
#define plugInEventID		'frmS'

//-------------------------------------------------------------------------------
//	Definitions -- Resources
//-------------------------------------------------------------------------------
// Have to do it this way, and not with an enum, because the Windows
// precompiler does not do the right thing when precompiling the .r file:
#define ResourceID				16000
#define kDialogID				ResourceID+1
#define kResetStringID			ResourceID+10
#define kCancelStringID			kResetStringID+1
#define kUnitsID				kCancelStringID+1

//-------------------------------------------------------------------------------

#endif // __FrameSelectTerminology_h__
