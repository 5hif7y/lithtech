// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2002 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.

#define plugInName			"MakeSelectFill"
#define plugInCopyrightYear	"1998"
#define plugInDescription \
	"An Actions Module sample for Adobe Photoshop¨."

#include "PIDefines.h"
#ifdef __PIMac__
	#include "Types.r"
	#include "SysTypes.r"
	#include "PIGeneral.r"
#elif defined(__PIWin__)
	#define Rez
	#include "PIGeneral.h"
	#include "PIUtilities.r"
	#include "WinDialogUtils.r"
#endif

resource 'PiPL' ( 16000, "Auto", purgeable)
	{
		{
		Kind { Actions },//"Actions" for automation plug-ins
		Name { "MakeSelectFill" },//name of your plug-in
		Category { "AdobeSDK" },//type of plug-in, ie. company name
		Version { (latestActionsPlugInVersion << 16) | latestActionsPlugInSubVersion },
		#ifdef __PIMac__
	        CodeCarbonPowerPC { 0, 0, "" },
		#elif defined(__PIWin__)
			CodeWin32X86 { "AutoPluginMain" },//the name of the entrypoint
		#endif
	//	EnableInfo { "true" },//if true, enables plug-in when there's open document
							//if unspecified, plug-in always enabled
		}
	};