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
//		PathsToPostScriptTerminology.h
//
//	Copyright 1990-1992, Thomas Knoll.
//	All Rights Reserved.
//
//	Description:
//		This file contains the resource definitions
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

//-------------------------------------------------------------------------------
//	Definitions -- Scripting keys
//-------------------------------------------------------------------------------

#define keyOurPath			'patH'

//-------------------------------------------------------------------------------
//	Definitions -- Resources
//-------------------------------------------------------------------------------

// Prompt and Postscript strings
#define kPrompt				16100
#define kCreatorAndType		kPrompt+1
#define kSuffix				kCreatorAndType+1
#define kPSTitle			kSuffix+1
#define kPSBoundingBox		kPSTitle+1
#define kPSHiResBoundingBox	kPSBoundingBox+1
#define kPSCropmarks		kPSHiResBoundingBox+1
#define kPSHeader			kPSCropmarks+1
#define kPSProcSets			kPSHeader+1
#define kPSBegin			kPSProcSets+1
#define kPSTrailer			kPSBegin+1
#define kDocBounds			kPSTrailer+1
#define kAllPaths			kDocBounds+1

// Conversions
#define kSavedOne 0x01000000L
	
//-------------------------------------------------------------------------------
//	Definitions -- Alerts
//-------------------------------------------------------------------------------

#define kAINeed30			16200
#define kAITooBig			kAINeed30+1
#define kAIRange			kAITooBig+1
#define kAIInvalidPath		kAIRange+1

//-------------------------------------------------------------------------------
// end PathsToPostScriptTerminology.h
