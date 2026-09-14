// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2002 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this 
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
//-------------------------------------------------------------------------------
#define plugInName			"Invert"
#define	plugInCopyrightYear "2000"
#define plugInDescription \
	"A bare bones invert routine with tiling for Adobe Photoshop¨."

#include "PIDefines.h"

#ifdef __PIMac__
	#include "Types.r"
	#include "SysTypes.r"
	#include "PIGeneral.r"
	#include "PIUtilities.r"
	#include "DialogUtilities.r"
#elif defined(__PIWin__)
	#define Rez
	#include "PIGeneral.h"
	#include "PIUtilities.r"
	#include "WinDialogUtils.r"
#endif

resource 'PiPL' ( 16000, "Invert", purgeable )
{
	{
		Kind { Filter },
		Name { plugInName },
		Category { "AdobeSDK" },
		Version { (latestFilterVersion << 16) | latestFilterSubVersion },
		#ifdef __PIWin__
			CodeWin32X86 { "PluginMain" },
		#else
	        CodeCarbonPowerPC { 0, 0, "" },
		#endif

		SupportedModes
		{
			noBitmap, doesSupportGrayScale,
			noIndexedColor, doesSupportRGBColor,
			doesSupportCMYKColor, doesSupportHSLColor,
			doesSupportHSBColor, doesSupportMultichannel,
			doesSupportDuotone, doesSupportLABColor
		},
			
		EnableInfo
		{
			"in (PSHOP_ImageMode, GrayScaleMode, RGBMode,"
			"CMYKMode, HSLMode, HSBMode, MultichannelMode,"
			"DuotoneMode, LabMode,"
			"Gray16Mode, RGB48Mode, CMYK64Mode, Lab48Mode)"
		},

		PlugInMaxSize { 2000000, 2000000 },

		FilterCaseInfo {
			{	/* array: 7 elements */
				/* Flat data, no selection */
				inStraightData,
				outStraightData,
				doNotWriteOutsideSelection,
				doesNotFilterLayerMasks,
				doesNotWorkWithBlankData,
				copySourceToDestination,
				/* Flat data with selection */
				inStraightData,
				outStraightData,
				doNotWriteOutsideSelection,
				doesNotFilterLayerMasks,
				doesNotWorkWithBlankData,
				copySourceToDestination,
				/* Floating selection */
				inStraightData,
				outStraightData,
				doNotWriteOutsideSelection,
				doesNotFilterLayerMasks,
				doesNotWorkWithBlankData,
				copySourceToDestination,
				/* Editable transparency, no selection */
				inStraightData,
				outStraightData,
				doNotWriteOutsideSelection,
				doesNotFilterLayerMasks,
				doesNotWorkWithBlankData,
				copySourceToDestination,
				/* Editable transparency, with selection */
				inStraightData,
				outStraightData,
				doNotWriteOutsideSelection,
				doesNotFilterLayerMasks,
				doesNotWorkWithBlankData,
				copySourceToDestination,
				/* Preserved transparency, no selection */
				inStraightData,
				outStraightData,
				doNotWriteOutsideSelection,
				doesNotFilterLayerMasks,
				doesNotWorkWithBlankData,
				copySourceToDestination,
				/* Preserved transparency, with selection */
				inStraightData,
				outStraightData,
				doNotWriteOutsideSelection,
				doesNotFilterLayerMasks,
				doesNotWorkWithBlankData,
				copySourceToDestination
			}
		}
	}
};


