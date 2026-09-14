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
/*
	File: WinDialogUtils.r


	Rez source file for Windows Dialog Utilities.
*/

/********************************************************************************/
/* Strings for About boxes */

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
	
/********************************************************************************/
/* Strings for Alerts */

/********************************************************************************/
