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
//		FrameSelect.r
//
//	Description:
//		Resource information for inset/outset selection
//		sample Automation plug-in.
//
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Set up included files for Macintosh and Windows.
//-------------------------------------------------------------------------------

#include "PIDefines.h"

#ifdef __PIMac__
	#include "Types.r"
	#include "SysTypes.r"
	#include "FrameSelectTerminology.h"
	#include "PIGeneral.r"
	#include "PIUtilities.r"
	#include "DialogUtilities.r"
#elif defined(__PIWin__)
	#define Rez
	#include "FrameSelectTerminology.h"
	#include "PIGeneral.h"
	#include "PIUtilities.r"
	#include "WinDialogUtils.r"
#endif

#include "PIActions.h"

//-------------------------------------------------------------------------------
//	PiPL resource
//-------------------------------------------------------------------------------

resource 'PiPL' (ResourceID, plugInName " PiPL", purgeable)
	{
		{
		Kind { Actions },
		Name { plugInName "..." },
		Category { vendorName },
		Version { (latestActionsPlugInVersion << 16) | latestActionsPlugInSubVersion },

		#ifdef __PIMac__
	        CodeCarbonPowerPC { 0, 0, "" },
		#elif defined(__PIWin__)
			CodeWin32X86 { "AutoPluginMain" },
		#endif
		
		HasTerminology
			{ 
			plugInClassID, 
			plugInEventID, 
			ResourceID, 
			vendorName " " plugInName	// Unique string.
			},
		
		// If you want this on all the time, remove the EnableInfo property (such
		// as for help menu entries.)  To have this on according to document
		// open, close, and mode guidelines, provide a minimal EnableInfo, such
		// as the one here:
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
			{								/* structure for automation */
			plugInName,						/* name */
			plugInAETEComment,				/* optional description */
			plugInClassID,					/* class ID, must be unique or Suite ID */
			plugInEventID,					/* event ID, must be unique */

			NO_REPLY,						/* never a reply */
			IMAGE_DIRECT_PARAMETER,			/* direct parameter, used by Photoshop */
				{							// filter or selection class here:
				// name:
				"offset",
				// key ID:
				keyOffset,
				// type ID:
				typeClass,
				// optional description:
				"",
				// flags:
				flagsOptionalSingleParameter,				
				
				"gutter",
				keyMyGutter,
				typeFloat,
				"",
				flagsOptionalSingleParameter,
				}
			},
			{},	/* non-filter/automation plug-in class here */
			{}, /* comparison ops (not supported) */
			{ // Enumerations go here:
			}	/* end of any enumerations */
		}
	};


//-------------------------------------------------------------------------------

// end FrameSelect.r
