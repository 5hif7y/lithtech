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
//		TriggerFilters.r
//
//	Description:
//		This file contains the resource information for the
//		Actions module TriggerFilters, an example of a module
//		that uses ADM and the Actions suite to execute events
//		from the Filter menu.
//
//	Use:
//		This is a good example of a basic shell to pop a cross-
//		platform UI using ADM and trigger basic events.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Definitions -- Required by other resources in this rez file.
//-------------------------------------------------------------------------------

// Additional scripting keys:

//-------------------------------------------------------------------------------
//	Set up included files for Macintosh and Windows.
//-------------------------------------------------------------------------------

#include "PIDefines.h"

#ifdef __PIMac__
	#include "Types.r"
	#include "SysTypes.r"
	#include "TriggerFiltersTerminology.h"
	#include "PIGeneral.r"
	#include "PIUtilities.r"
	#include "DialogUtilities.r"
#elif defined(__PIWin__)
	#define Rez
	#include "TriggerFiltersTerminology.h"
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
			vendorName " " plugInName
			},
		
		// If you want this on all the time, remove the EnableInfo property (such
		// as for help menu entries.  To have this on according to document
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
			{								/* structure for filters */
			plugInName,						/* name */
			plugInAETEComment,				/* optional description */
			plugInClassID,					/* class ID, must be unique or Suite ID */
			plugInEventID,					/* event ID, must be unique */

			NO_REPLY,						/* never a reply */
			IMAGE_DIRECT_PARAMETER,			/* direct parameter, used by Photoshop */
				{							// filter or selection class here:
				// name:
				"group",
				// key ID:
				keyGroup,
				// type ID:
				typeText,
				// optional description:
				"",
				// flags:
				flagsSingleParameter,
				
				// name:
				"filter",
				// key ID:
				keyLastFilter,
				// type ID:
				typeText,
				// optional description:
				"",
				// flags:
				flagsSingleParameter,
				
				// name:
				"id",
				keyLastID,
				typeText,
				"",
				flagsSingleParameter,
				
				"display dialog",
				keyShowDialog,
				typeDialogOptions,
				"",
				flagsEnumeratedParameter,
				
				"append to log",
				keyLogToFile,
				typeBoolean,
				"",
				flagsOptionalSingleParameter,
				
				"file",
				keyOurFilename,
				typePlatformFilePath,
				"",
				flagsOptionalSingleParameter
				}
			},
			{},	/* non-filter plug-in class here */
			{}, /* comparison ops (not supported) */
			{
				typeDialogOptions,
				{
					// Name:
					"once",
					// Filter:
					keyDialogOnce,
					// Optional description:
					"",
					
					"always",
					keyDialogAlways,
					"",
					
					"never",
					keyDialogNever,
					""
				}		
			}	/* any enumerations */
		}
	};

//-------------------------------------------------------------------------------

// end TriggerFilters.r
