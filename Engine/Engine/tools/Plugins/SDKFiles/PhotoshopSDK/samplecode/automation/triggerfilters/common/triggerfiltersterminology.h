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
//		TriggerFiltersTerminology.h
//
//	Description:
//		This file contains the resource definitions
//		for the Automation module TriggerFilters, a module
//		showing the use of the Actions playback suite and ADM.
//
//	Use:
//		This module pops a UI from the File>>Automation>>
//		TriggerFilters... selection, and then executes each
//		chosen filter while leaving its UI up.
//
//-------------------------------------------------------------------------------

#ifndef __TriggerFiltersTerminology_h__
#define __TriggerFiltersTerminology_h__

//-------------------------------------------------------------------------------
//	Definitions -- Required by include files.
//-------------------------------------------------------------------------------

// The About box and resources are created in DialogUtilities.r.
// You can easily override them, if you like.

#define vendorName			"AdobeSDK"
#define plugInName			"Trigger Filters"
#define plugInCopyrightYear	"1998"
#define plugInDescription \
	"An example Actions Module to trigger filters in Adobe Photoshop¨."

#define plugInAETEComment 	"triggerfilters example actions plug-in"

#define plugInSuiteID		'sdKE'
#define plugInClassID		plugInSuiteID
#define plugInEventID		'triG'



//-------------------------------------------------------------------------------
//	Definitions -- Scripting keys
//-------------------------------------------------------------------------------

// keyGroup	is already defined in PIActions.h.  We can use that.
#define keyLastFilter		'sKLF'
#define keyLastID			'sKiD'
#define keyShowDialog		'sKsD'
#define typeDialogOptions	'tsKo'
	#define keyDialogOnce	'ksD1'
	#define keyDialogAlways	'ksDA'
	#define keyDialogNever	'ksDN'

#define keyLogToFile		'sKlF'
#define keyOurFilename		'sKoF'

#define otherName			"Other..."
#define otherKey			"othR"

#define menuLineName		"-"
#define menuLineKey			"-"

//-------------------------------------------------------------------------------
//	Definitions -- Resources
//-------------------------------------------------------------------------------
// Have to do it this way, and not with an enum, because the Windows
// precompiler does not do the right thing when precompiling the .r file:
#define	ResourceID						16000
#define kDialogID						ResourceID+1
#define kDisplayResultsStringID			ResourceID+10
#define kDisplayResultsCancelStringID	kDisplayResultsStringID+1
#define kDisplayResultsUnknownStringID	kDisplayResultsCancelStringID+1
#define kDisplayResultsUnknownErrorID	kDisplayResultsUnknownStringID+1
#define kQueryForFileID					kDisplayResultsUnknownErrorID+1
#define kQueryDefaultFilenameID			kQueryForFileID+1
#define kResetStringID					kQueryDefaultFilenameID+1
#define kCancelStringID					kResetStringID+1
#define kShowDialogStringOnceID			kCancelStringID+1
#define kShowDialogStringAlwaysID		kShowDialogStringOnceID+1
#define kShowDialogStringNeverID		kShowDialogStringAlwaysID+1
#define kGroupMenuID					kShowDialogStringNeverID+1
#define kFilterSet1ID					16100
#define kFilterSet2ID					16200
#define kFilterSet3ID					16300

//-------------------------------------------------------------------------------

#endif // __TriggerFiltersTerminology_h__
