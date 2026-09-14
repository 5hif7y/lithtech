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
//		DialogUtilities.r
//
//
//	Description:
//		This rez file contains the resources and defines
//		related to Macintosh dialog manipulation. 
//
//	Use:
//		DialogUtilities is intended to make Macintosh dialog
//		manipulation a little simpler by providing a standard
//		suite of functions.  These generic resources are a good
//		starting point for basic plug-in functionality.
//		Feel free to tweak them for your own work.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Definitions -- Plug-in supplies.
//	Create generic definitions if not supplied by plug-in.
//-------------------------------------------------------------------------------

#ifndef plugInName
	#define plugInName	"Generic Plug-in"
#endif

#ifndef plugInCopyrightYear
	#define plugInCopyrightYear		"2003"
#endif

#ifndef plugInDescription
	#define plugInDescription \
		"An example plug-in module for Adobe Photoshop¨."
#endif

//-------------------------------------------------------------------------------
//	Definitions -- SDK release information.  Used to version SDK builds.
//-------------------------------------------------------------------------------

#ifndef VersionString
	#define VersionString 	"CS"
#endif

#ifndef ReleaseString
	#define ReleaseString	"1 (9/2003)"
#endif

#ifndef CurrentYear
	#define CurrentYear		"2003"
#endif

//-------------------------------------------------------------------------------
//	Defines -- Generic resources, such as about boxes and resource IDs.
//	Redefine these if you need to.
//-------------------------------------------------------------------------------

#ifndef StringResource
	#define StringResource	'STR '
#endif

#ifndef ResourceID
	#define ResourceID		16000
#endif

#ifndef AboutID
	#define AboutID			ResourceID
#endif

#ifndef uiID
	#define uiID			ResourceID+1
#endif

#ifndef ADMAboutID
	#define ADMAboutID		uiID+1
#endif

#ifndef AlertID
	#define AlertID			16990
#endif

#ifndef kBadNumberID
	#define kBadNumberID	AlertID
#endif

#ifndef kBadDoubleID
	#define kBadDoubleID	kBadNumberID+1
#endif

#ifndef kNeedVers
	#define kNeedVers		kBadDoubleID+1
#endif

#ifndef kWrongHost
	#define kWrongHost		kNeedVers+1
#endif

//-------------------------------------------------------------------------------
//	About dialog box.
//-------------------------------------------------------------------------------

resource 'DLOG' (AboutID, plugInName " Generic About Box", purgeable)
{
	{20, 0, 214, 390},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	ResourceID,
	"About " plugInName "...",
	alertPositionMainScreen	// Universal 3.0 headers.
};

resource 'dlgx' (AboutID) {
	versionZero {
		kDialogFlagsHandleMovableModal + kDialogFlagsUseThemeControls + kDialogFlagsUseThemeBackground
	}
};

resource 'DITL' (AboutID, plugInName " Generic About Box", purgeable)
{
	{
		{-80, 0, -60, 60}, 		Button { enabled, "Hidden" },
		{5, 5, 155, 380}, 		UserItem { enabled },
		{5, 5, 155, 380},		StaticText { disabled, "^0\n^1\n^2\n" },
		{160, 10, 180, 190}, 	Button { enabled, "www.adobe.com" },
		{160, 200, 180, 380},	Button { enabled, "partners.adobe.com" } 
	}
};

//-------------------------------------------------------------------------------
//	About string resource.  This is stored as text, instead of explicitly in
//	the About box definition, so that it can be used cross-platform.  Resources
//	of type 'STR ' (StringResource) are converted automatically by CNVTPIPL.
//-------------------------------------------------------------------------------

resource StringResource (AboutID, plugInName " About Text", purgeable)
{
	plugInName "\n\n"
	"Version " VersionString " "
	"Release " ReleaseString "\n"
	"Copyright " plugInCopyrightYear "-" CurrentYear ", Adobe Systems Incorporated.\n"
	"All Rights Reserved.\n\n"
	plugInDescription
};

// The ADM provided about box doesn't understand literals, but does understand
// slash characters.  So we'll provide it:
resource StringResource (ADMAboutID, plugInName "About Text", purgeable)
	{
	plugInName "\\n\\n"
	"Version " VersionString " "
	"Release " ReleaseString "\\n"
	"Copyright " plugInCopyrightYear "-" CurrentYear " "
	"Adobe Systems Incorporated. "
	"All Rights Reserved.\\n\\n"
	plugInDescription "\\n\\n\\n\\n" // Must end with \\n \\n due to resizing error.
	};
	
//-------------------------------------------------------------------------------
//	Version 'vers' resource.
//-------------------------------------------------------------------------------

resource 'vers' (1, plugInName " Version", purgeable)
{
	8, 0x00, final, 0, verUs,
	VersionString,
	VersionString " (c) " plugInCopyrightYear "-" CurrentYear ", Adobe Systems Incorporated. All Rights Reserved."
};

//-------------------------------------------------------------------------------
//	Alert dialog box.
//-------------------------------------------------------------------------------

resource 'DITL' (AlertID, plugInName " Generic Alert", purgeable)
{
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{87, 230, 107, 298},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{12, 60, 76, 300},
		StaticText {
			disabled,
			"^0"
		}
	}
};

resource 'ALRT' (AlertID, plugInName " Generic Alert", purgeable)
{
	{0, 0, 120, 310},
	AlertID,
	{	/* array: 4 elements */
		/* [1] */
		OK, visible, sound1,
		/* [2] */
		OK, visible, sound1,
		/* [3] */
		OK, visible, sound1,
		/* [4] */
		OK, visible, sound1
	},
	alertPositionParentWindowScreen	// Universal 3.0 headers.
};

//-------------------------------------------------------------------------------

// end DialogUtilities.r

