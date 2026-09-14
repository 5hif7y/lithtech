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
//		MakeNew.h
//
//	Description:
//		Create a new document.
//
//-------------------------------------------------------------------------------

#ifndef __MakeNew_H__		// Has this not been defined yet?
#define __MakeNew_H__		// Only include this once by predefining it

#include "MakeNewTerminology.h"			// Terminology for this plug-in.
#include "piactionsplugin.h"			// Actions plug-in Photoshop header file

#include "piactions.h"
#include "PIUSuites.h"
#include "PIUI.h"

//-------------------------------------------------------------------------------
//	Globals -- structures
//-------------------------------------------------------------------------------
extern int32 gWidth;
extern int32 gHeight;
extern int32 gResolution;
extern DescriptorEnumID gFill;
extern DescriptorClassID gMode;

extern SPBasicSuite* sSPBasic;
extern SPPluginRef gPlugInRef;

//-------------------------------------------------------------------------------
//	Prototypes
//-------------------------------------------------------------------------------
SPErr Execute(PIActionParameters* actionParams);// Main routine.
SPErr DoUI();// Show the UI.
SPErr ReadScriptParams(PIActionParameters* actionParams);// Read any scripting params.
SPErr WriteScriptParams(PIActionParameters* actionParams);// Write any scripting params.

//-------------------------------------------------------------------------------

#endif // __MakeNew_H__
