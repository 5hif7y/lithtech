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
//		Idle.r
//
//	Description:
//		This file contains the resource text descriptions for the
//		Import module Idle, a very simple example of a module
//		that just idles and sends update events.
//
//	Use:
//		This import module is good as an example of a module
//		that forces the background application to redraw
//		its windows.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Definitions -- Required by include files.
//-------------------------------------------------------------------------------

// The About box and resources are created in DialogUtilities.r.
// You can easily override them, if you like.

#define plugInName			"Idle"
#define plugInCopyrightYear	"1996"
#define plugInDescription \
	"An example selection module to idle for a set amount of time in Adobe Photoshop¨."

//-------------------------------------------------------------------------------
//	Definitions -- Required by other resources in this rez file.
//-------------------------------------------------------------------------------

// Dictionary (aete) resources:

#define vendorName			"AdobeSDK"
#define plugInAETEComment 	"idle example selection plug-in"

#define plugInSuiteID		'sdKD'
#define plugInClassID		plugInSuiteID
#define plugInEventID		'idlE'

//-------------------------------------------------------------------------------
//	Set up included files for Macintosh and Windows.
//-------------------------------------------------------------------------------

#include "PIDefines.h"

#ifdef __PIMac__
	#include "Types.r"
	#include "SysTypes.r"
	#include "PIGeneral.r"
	#include "PIUtilities.r"
	#include "DialogUtilities.r"
#elif defined(__PIWin__)
	#include "PIGeneral.h"
	#include "PIUtilities.r"
	#include "WinDialogUtils.r"
#endif

#include "PIActions.h"

#include "IdleTerminology.h"	// Terminology for this plug-in.

//-------------------------------------------------------------------------------
//	PiPL resource
//-------------------------------------------------------------------------------

resource 'PiPL' (ResourceID, plugInName " PiPL", purgeable)
{
	{
		Kind { Selection },
		Name { plugInName "..." },
		Category { vendorName },
		Version { (latestSelectionVersion << 16) | latestSelectionSubVersion },

		#ifdef __PIMac__
	        CodeCarbonPowerPC { 0, 0, "" },
		#elif defined(__PIWin__)
			CodeWin32X86 { "PluginMain" },
		#endif

		HasTerminology { plugInClassID, plugInEventID, ResourceID, vendorName " " plugInName },
		/* class ID, event ID, aete ID, uniqueString */
		
		EnableInfo { "true" },
		
	}
};

//-------------------------------------------------------------------------------
//	Dictionary (scripting) resource
//-------------------------------------------------------------------------------

resource 'aete' (ResourceID, plugInName " dictionary", purgeable)
{
	1, 0, english, roman,					/* aete version and language specifiers */
	{
		vendorName,							/* vendor suite name */
		"Adobe example plug-ins",			/* optional description */
		plugInSuiteID,						/* suite ID */
		1,									/* suite code, must be 1 */
		1,									/* suite level, must be 1 */
		{									/* structure for filters */
			vendorName " " plugInName,		/* unique selection name */
			plugInAETEComment,				/* optional description */
			plugInClassID,					/* class ID, must be unique or Suite ID */
			plugInEventID,					/* event ID, must be unique */

			NO_REPLY,						/* never a reply */
			IMAGE_DIRECT_PARAMETER,			/* direct parameter, used by Photoshop */
			{								// filter or selection class here:
				"idle seconds",								// name
				keyIdleAmount,								// key ID
				typeFloat,									// type ID
				"idle amount",								// optional description
				flagsSingleParameter,						// flags

				"progress indicator",						// name
				keyUseProgress,								// key ID
				typeBoolean,								// type ID
				"whether to show progress indicator",		// optional description
				flagsSingleParameter,						// flags				

				"send update events",						// name
				keySendUpdates,								// key ID
				typeBoolean,								// type ID
				"whether to send update events",			// optional description
				flagsSingleParameter,						// flags			
			}
		},
		{},	/* non-filter plug-in class here */
		{}, /* comparison ops (not supported) */
		{}	/* any enumerations */
	}
};

//-------------------------------------------------------------------------------
//	Dialog resource
//-------------------------------------------------------------------------------

resource 'DLOG' (ResourceID+1, plugInName " UI", purgeable)
{
	{102, 148, 190, 452},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	ResourceID+1,
	plugInName,
	centerParentWindowScreen
};

resource 'dlgx' (ResourceID+1) {
	versionZero {
		kDialogFlagsHandleMovableModal + kDialogFlagsUseThemeControls + kDialogFlagsUseThemeBackground
	}
};

resource 'DITL' (ResourceID+1, plugInName " UI", purgeable)
{
	{	/* array DITLarray: 7 elements */
		/* [1] */
		{6, 228, 26, 282},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{38, 228, 58, 282},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{8, 0, 28, 68},
		StaticText {
			disabled,
			"Amount:"
		},
		/* [4] */
		{8, 72, 24, 136},
		EditText {
			enabled,
			""
		},
		/* [5] */
		{40, 68, 56, 222},
		CheckBox {
			enabled,
			"Show Progress"
		},
		/* [6] */
		{8, 144, 28, 200},
		StaticText {
			disabled,
			"seconds"
		},
		/* [7] */
		{60, 68, 76, 222},
		CheckBox {
			enabled,
			"Send Update events"
		}
	}
};

//-------------------------------------------------------------------------------

// end Idle.r
