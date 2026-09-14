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
//		PathsToPostScript.h
//
//	Copyright 1990-1992, Thomas Knoll.
//	All Rights Reserved.
//
//	Description:
//		This file contains the resource information
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
//	Definitions -- Required by include files.
//-------------------------------------------------------------------------------

// The About box and resources are created in DialogUtilities.r.
// You can easily override them, if you like.

#define plugInName			"PathsToPostScript"
#define plugInCopyrightYear	"1993"
#define plugInDescription \
	"A plug-in module to export pen paths to Adobe Illustrator¨."

//-------------------------------------------------------------------------------
//	Definitions -- Required by other resources in this rez file.
//-------------------------------------------------------------------------------

// Dictionary (aete) resources:

#define vendorName				"AdobeSDK"
#define plugInAETEComment		"PathsToPostScript export plug-in"

#define plugInSuiteID			'sdKC'
#define plugInClassID			'illE'
#define plugInEventID			typeNull // must be this

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

#include "PathsToPostScriptTerminology.h"	// Terminology for this plug-in.

//-------------------------------------------------------------------------------
//	PiPL resource
//-------------------------------------------------------------------------------

resource 'PiPL' (ResourceID, plugInName " PiPL", purgeable)
{
	{
		Kind { Export },
		Name { "PathsToPostScript..." },
		Version { (latestExportVersion << 16) | latestExportSubVersion },
		
		#ifdef __PIMac__
	        CodeCarbonPowerPC { 0, 0, "" },
		#elif defined(__PIWin__)
			CodeWin32X86 { "PluginMain" },
		#endif

		HasTerminology
		{
			plugInClassID,		// class ID
			plugInEventID,		// event ID
			ResourceID,			// aete ID
			"" 					// uniqueString
		},
		
		SupportedModes
		{
			noBitmap, doesSupportGrayScale,
			doesSupportIndexedColor, doesSupportRGBColor,
			doesSupportCMYKColor, doesSupportHSLColor,
			doesSupportHSBColor, doesSupportMultichannel,
			doesSupportDuotone, doesSupportLABColor
		},
		
		EnableInfo
		{
			"in (PSHOP_ImageMode, GrayScaleMode,"
			"IndexedColorMode, RGBMode,"
			"CMYKMode, HSLMode,"
		    "HSBMode, MultichannelMode,"
		    "DuotoneMode, LabMode)"
		}
	}
};


//-------------------------------------------------------------------------------
//	PiMI resource (Photoshop 2.5 and other older hosts)
//-------------------------------------------------------------------------------

resource 'PiMI' (ResourceID, plugInName " PiMI", purgeable)
{ 
		latestExportVersion,
		latestExportSubVersion,
		0,
		supportsGrayScale +
		supportsRGBColor +
		supportsCMYKColor +
		supportsHSLColor +
		supportsHSBColor +
		supportsMultichannel +
		supportsDuotone +
		supportsLABColor,
		'    ', /* No required host */
		{}
};
	
//-------------------------------------------------------------------------------
//	Dictionary (scripting) resource
//-------------------------------------------------------------------------------

resource 'aete' (ResourceID, plugInName " dictionary", purgeable)
{
	1, 0, english, roman,									/* aete version and language specifiers */
	{
		vendorName,											/* vendor suite name */
		"Adobe example plug-ins",							/* optional description */
		plugInSuiteID,										/* suite ID */
		1,													/* suite code, must be 1 */
		1,													/* suite level, must be 1 */
		{},													/* structure for filters */
		{													/* non-filter plug-in class here */
			vendorName " " plugInName,						/* unique class name */
			plugInClassID,									/* class ID, must be unique or Suite ID */
			plugInAETEComment,								/* optional description */
			{												/* define inheritance */
				"<Inheritance>",							/* must be exactly this */
				keyInherits,								/* must be keyInherits */
				classExport,								/* parent: Format, Import, Export */
				"parent class export",						/* optional description */
				flagsSingleProperty,						/* if properties, list below */
				
				"in",										/* our path */
				keyIn,										/* common key */
				typePlatformFilePath,						/* correct path for platform */
				"file destination",							/* optional description */
				flagsSingleProperty,
				
				vendorName " path",							// name of path
				keyOurPath,									// our own key
				typeText,									// text
				"path",										// you get the idea
				flagsSingleProperty
				/* no more properties */
			},
			{}, /* elements (not supported) */
			/* class descriptions */
		},
		{}, /* comparison ops (not supported) */
		{}	/* any enumerations */
	}
};

//-------------------------------------------------------------------------------
//	Dialog resources
//-------------------------------------------------------------------------------

//resource 'DLOG' (uiID, "UI", purgeable)
//{
//	{0, 0, 223, 344},
//	dBoxProc,
//	invisible,
//	noGoAway,
//	0x0,
//	uiID,
//	plugInName,
//	centerParentWindowScreen
//};

//resource 'dctb' (uiID, "UI", purgeable)
//	{
//	{}
//	};

//resource 'DITL' (uiID, "UI", purgeable)
//	{
//		{
//		{161, 252, 181, 332}, Button { enabled, "Save" },
//		{130, 252, 150, 332}, Button { enabled, "Cancel" },
//		{0, 0, 0, 0},		  HelpItem { disabled, HMScanhdlg { -6043 } },
//		{8, 235, 24, 337},	  UserItem { enabled },
//		{32, 252, 52, 332},   Button { enabled, "Eject" },
//		{60, 252, 80, 332},   Button { enabled, "Desktop" },
//		{29, 12, 127, 230},   UserItem { enabled },
//		{6, 12, 25, 230},	  UserItem { enabled },
//		{119, 250, 120, 334}, Picture { disabled, 11 },
//		{157, 15, 173, 227},  EditText { enabled, "" },
//		{136, 15, 152, 227},  StaticText { disabled, "Export paths to file:" },
//		{88, 252, 108, 332},  UserItem { disabled },
//		{194, 14, 213, 330},   Control { enabled, uiID }
//		}
//	};

resource 'CNTL' (17005, "UI", purgeable)
	{
	{194, 14, 213, 330},	/* Enclosing rectangle from above. */
	popupTitleLeftJust,
	visible,
	49,
	17005,
	popupMenuCDEFProc,
	0,
	"Write:"
	};
	
resource 'MENU' (17005, "UI", preload)
	{
	17005,
	textMenuProc,
	0b1111111111111111111111111110101,
	enabled,
	"Write",
		{
		"Document Bounds",	noIcon,	noKey,	noMark,	plain;
		"-",				noIcon,	noKey,	noMark,	plain;
		"All Paths",		noIcon,	noKey,	noMark,	plain;
		"-",				noIcon,	noKey,	noMark,	plain;
		}
	};

resource 'DITL' (17006, purgeable)
{
	{
	{10, 10, 30, 330},   Control { enabled, 17005 }
	}	
};


//-------------------------------------------------------------------------------
//	Resource text entries
//
//	Entering these as separate resources because then they
//	transfer directly over to windows via CNVTPIPL.
//
//	If I use a Macintosh 'STR#' resource, I could put all these
//	strings into a single resource, but there is no
//	parallel on Windows.  'STR ' resources, which hold
//	one string per ID, exist on Windows and Macintosh.
//-------------------------------------------------------------------------------

resource StringResource (kPrompt, "Prompt", purgeable)
{
	"Export paths to file:"
};

resource StringResource (kCreatorAndType, "CreatorAndType", purgeable)
{
	"ART5" 	// creator
	"TEXT"	// type
};

resource StringResource (kSuffix, "Suffix", purgeable)
{
	".ai"
};

resource StringResource (kPSTitle, "PSTitle", purgeable)
{
	"%%Title: (^0)\n"
};

resource StringResource (kPSBoundingBox, "BoundingBox", purgeable)
{
	"%%BoundingBox: "
};

resource StringResource (kPSHiResBoundingBox, "HiResBoundingBox", purgeable)
{
	"%%HiResBoundingBox: "
};

resource StringResource (kPSCropmarks, "Cropmarks", purgeable)
{
	"%AI3_Cropmarks: "
};

resource StringResource (kPSHeader, "Header", purgeable)
{
	"%!PS-Adobe-2.0\n"
	"%%Creator: Adobe Photoshop¨ " plugInName "\n"
};

resource StringResource (kPSProcSets, "ProcSets", purgeable)
{
	"%%DocumentProcSets: Adobe_Illustrator_1.1 0 0\n"
	"%%ColorUsage: Black&White\n"
};

resource StringResource (kPSBegin, "PSBegin", purgeable)
{
	"%%DocumentPreview: None\n"
	"%%EndComments\n"
	"%%EndProlog\n"
	"%%BeginSetup\n"
	"Adobe_Illustrator_1.1 begin\n"
	"n\n"
	"%%EndSetup\n"
	"0.0 0.0 0.0 1.0 k\n"
	"0 i 0 J 0 j 1 w 4 M []0 d\n"
	"%%Note:\n"
};

resource StringResource (kPSTrailer, "PSTrailer", purgeable)
{
	"%%Trailer\n"
	"%%EOF\n"
};

resource StringResource (kDocBounds, "Document Bounds", purgeable)
{
	"Document Bounds"
};

resource StringResource (kAllPaths, "All Paths", purgeable)
{
	"All Paths"
};
	
//-------------------------------------------------------------------------------
//	Resource text entries -- alerts
//-------------------------------------------------------------------------------

resource StringResource (kAINeed30, purgeable)
{
	"Sorry, path export requires Photoshop¨ 3.0 or later."
};

resource StringResource (kAITooBig, purgeable)
{
	"The document is too big for Illustrator's coordinate space."
};

resource StringResource (kAIRange, purgeable)
{
	"A point lies outside the bounds which are valid for Illustrator."
};

resource StringResource (kAIInvalidPath, purgeable)
{
	"This document contains an unsupported path format."
};

//-------------------------------------------------------------------------------

// end PathsToPostScript.r
