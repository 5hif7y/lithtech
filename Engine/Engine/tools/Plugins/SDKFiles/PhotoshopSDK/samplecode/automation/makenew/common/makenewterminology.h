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
//		MakeNewTerminology.h
//
//	Description:
//		Resource and code definitions for New Document plug-in. 
//
//-------------------------------------------------------------------------------

#ifndef __MakeNewTerminology_h__
#define __MakeNewTerminology_h__

//-------------------------------------------------------------------------------
//	Definitions -- Required by include files.
//-------------------------------------------------------------------------------

// The About box and resources are created in DialogUtilities.r.
// You can easily override them, if you like.

#define plugInName			"MakeNew"
#define plugInCopyrightYear	"1997"
#define plugInDescription \
	"An example Actions Module to make a new document in Adobe Photoshop¨."

//-------------------------------------------------------------------------------
//	Definitions -- Scripting keys
//-------------------------------------------------------------------------------

#define keyMyWidth			keyWidth
#define keyMyHeight			keyHeight
#define keyMyResolution		keyResolution
#define keyMyFill			keyFill
#define typeMyFill			typeFill
#define keyMyMode			keyMode // RGB, CMYK, etc.
#define typeClassMyMode		typeClassMode

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

#endif // __MakeNewTerminology_h__
