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

#include "PIDefines.h"

#include "Types.r"
#include "SysTypes.r"
#include "PIGeneral.r"
#include "PIActions.h"

#include "TriggerFiltersTerminology.h"

//-------------------------------------------------------------------------------
//	Dialog resource
//-------------------------------------------------------------------------------

resource 'DLOG' (kDialogID, plugInName " UI", purgeable) {
	{118, 488, 296, 940},
	movableDBoxProc,
	visible,
	noGoAway,
	0x0,
	kDialogID,
	plugInName,
	centerParentWindowScreen
};

resource 'dlgx' (kDialogID) {
	versionZero {
		kDialogFlagsHandleMovableModal + kDialogFlagsUseThemeControls + kDialogFlagsUseThemeBackground
	}
};

resource 'DITL' (kDialogID, plugInName " UI", purgeable) {
	{	/* array DITLarray: 15 elements */
		/* [1] */
		{8, 386, 28, 450},
		Button {
			enabled,
			"Done"
		},
		/* [2] */
		{40, 386, 60, 450},
		Button {
			enabled,
			"Cancel"
		},

		/* [2.5] */
		{8, 317, 28, 379},
		// {56, 317, 78, 377},
		Button {
			enabled,
			"Do it!"
		},

		/* [3] */
		{40, 317, 60, 379},
		// {82, 317, 104, 377},
		Button {
			enabled,
			"Undo"
		},
		/* [4] */
		{0, 12, 14, 62},
		StaticText {
			disabled,
			" Filter:"
		},
		/* [5] */
		{8, 0, 88, 312},
		UserItem {
			enabled
		},
		/* [6] Group */
		{15, 4, 36, 110},
		Control {
			enabled,
			kDialogID+1
		},
		/* [7] Filter */
		{15, 115, 36, 305},
		Control {
			enabled,
			kDialogID
		},
		/* [8] Dialog */
		{39, 115, 60, 238},
		Control {
			enabled,
			kDialogID+2
		},
		
		/* [8.1] */
		{42, 4, 61, 110},
		StaticText {
			disabled,
			"Show dialog:"
		},
		
		/* [9] Other */
		{66, 118, 81, 235},
		EditText {
			enabled,
			""
		},
		
		/* [10] */
		{66, 4, 80, 110},
		StaticText {
			disabled,
			"Other:"
		},
		
		/* [11] */
		{96, 12, 110, 75},
		StaticText {
			disabled,
			" Results:"
		},
		/* [12] */
		{104, 0, 176, 451},
		UserItem {
			enabled
		},
		/* [13] */
		{112, 4, 143, 449},
		StaticText {
			disabled,
			""
		},
		/* [14] */
		{144, 4, 164, 124},
		CheckBox {
			enabled,
			"Append to log:"
		},
		/* [15] */
		{146, 126, 174, 449},
		StaticText {
			disabled,
			""
		}
	}
};

resource 'CNTL' (kDialogID, "FilterList", purgeable) {
	{16, 4, 44, 194},
	0,
	visible,
	0,
	kDialogID,
	1016,
	0,
	""
};

resource 'CNTL' (kDialogID+1, "Group", purgeable) {
	{16, 4, 36, 114},
	0,
	visible,
	0,
	kDialogID+1,
	1016,
	0,
	""
};

resource 'CNTL' (kDialogID+2, "Dialog", purgeable) {
	{41, 4, 62, 127},
	0,
	visible,
	0,
	kDialogID+2,
	1016,
	0,
	""
};


#ifdef WantToDoMenuResource
	// If you want to instantiate a menu resource and
	// use ADM that way, then create a menu resource on
	// both Mac and Win, then assign the menu resource
	// ID at runtime using sADMList->SetMenuID(list, resourceID).
	resource 'MENU' (kDialogID, plugInName " Popup") {
		kDialogID,
		textMenuProc,
		allEnabled,
		enabled,
		"Filter",
			{
			// Additional menu items here.
			menuLineName, noIcon, noKey, noMark, plain,
			otherName, noIcon, noKey, noMark, plain
			}
	};
#endif // WantToDoMenuResource

resource 'STR#' (kDisplayResultsStringID, purgeable)
	{
		{
		"%s%s: %s ticks = %s mins, %s secs."
		}
	};
	
resource 'STR#' (kDisplayResultsCancelStringID, purgeable)
	{
		{
		"User cancel, "
		}
	};
	
resource 'STR#' (kDisplayResultsUnknownStringID, purgeable)
	{
		{
		"Can't find "
		}
	};

resource 'STR#' (kDisplayResultsUnknownErrorID, purgeable)
	{
		{
		"%s error, "
		}
	};

resource 'STR#' (kQueryForFileID, purgeable)
	{
		{
		"Append to log..."
		}
	};
	
resource 'STR#' (kQueryDefaultFilenameID, purgeable)
	{
		{
		"TriggerFilters-log.txt"
		}
	};
	
resource 'STR#' (kResetStringID, purgeable)
	{
		{
		"Reset"
		}
	};
	
resource 'STR#' (kCancelStringID, purgeable)
	{
		{
		"Cancel"
		}
	};
	
resource 'STR#' (kShowDialogStringOnceID, purgeable)
	{
		{
		"Once"
		}
	};

resource 'STR#' (kShowDialogStringAlwaysID, purgeable)
	{
		{
		"Always"
		}
	};
	
resource 'STR#' (kShowDialogStringNeverID, purgeable)
	{
		{
		"Never"
		}
	};

resource 'STR#' (kGroupMenuID, purgeable)
	{
		{
		"Internal|16100|"
		"Standard|16200|"
		"Effect|16300|"
		}
	};

resource 'STR#' (kFilterSet1ID, purgeable)
	{
		{
		"Blur|Blr |"
		"Blur More|BlrM|"
		"Gaussian Blur|GsnB|"
		"Motion Blur|MtnB|"
		"-|-|"
		"+"
		}
	};

resource 'STR#' (kFilterSet1ID+1, purgeable)
	{
		{
		"Add Noise|AdNs|"
		"Despeckle|Dspc|"
		"Dust & Scratches|DstS|"
		"Median|Mdn |"
		"-|-|"
		"+"
		}
	};
	
resource 'STR#' (kFilterSet1ID+2, purgeable)
	{
		{
		"Facet|Fct |"
		"Fragment|Frgm|"
		"Mosaic|Msc |"
		"-|-|"
		"+"
		}
	};
	
resource 'STR#' (kFilterSet1ID+3, purgeable)
	{
		{
		"Sharpen|Shrp|"
		"Sharpen Edges|ShrE|"
		"Sharpen More|ShrM|"
		"Unsharp Mask|UnsM|"
		"-|-|"
		"+"
		}
	};

resource 'STR#' (kFilterSet1ID+4, purgeable)
	{
		{
		"Diffuse|Dfs |"
		"Emboss|Embs|"
		"Find Edges|FndE|"
		"Trace Contour|TrcC|"
		"-|-|"
		"+"
		}
	};
	
resource 'STR#' (kFilterSet1ID+5, purgeable)
	{
		{
		"Custom|Cstm|"
		"High Pass|HghP|"
		"Maximum|Mxm |"
		"Minimum|Mnm |"
		"Offset|Ofst|"
		"-|-|"
		"+"
		}
	};
	
resource 'STR#' (kFilterSet1ID+6, purgeable)
	{
		{
		"Other...|othR|"
		}
	};
	
resource 'STR#' (kFilterSet2ID, purgeable)
	{
		{
		"Radial Blur|RdlB|"
		"Smart Blur|SmrB|"
		"-|-|"
		"+"
		}
	};
	
resource 'STR#' (kFilterSet2ID+1, purgeable)
	{
		{
		"Color Halftone|ClrH|"
		"Crystallize|Crst|"
		"Mezzotint|Mztn|"
		"-|-|"
		"+"
		}
	};
	
resource 'STR#' (kFilterSet2ID+2, purgeable)
	{
		{
		"3D Transform|TdT |"
		"Clouds|Clds|"
		"Difference Clouds|DfrC|"
		"Lens Flare|LnsF|"
		"-|-|"
		"+"
		}
	};
	
resource 'STR#' (kFilterSet2ID+3, purgeable)
	{
		{
		"Extrude|Extr|"
		"Solarize|Slrz|"
		"Tiles|Tls |"
		"Wind|Wnd |"
		"-|-|"
		"+"
		}
	};

resource 'STR#' (kFilterSet2ID+4, purgeable)
	{
		{
		"De-Interlace|Dntr|"
		"NTSC Colors|NTSC|"
		"-|-|"
		"+"
		}
	};
	
resource 'STR#' (kFilterSet2ID+5, purgeable)
	{
		{
		"Other...|othR|"
		}
	};

resource 'STR#' (kFilterSet3ID, purgeable)
	{
		{
		"Colored Pencil|ClrP|"
		"Cutout|Ct  |"
		"Dry Brush|DryB|"
		"Film Grain|FlmG|"
		"Fresco|Frsc|"
		"Neon Glow|NGlw|"
		"Paint Daubs|PntD|"
		"Palette Knife|PltK|"
		"Plastic Wrap|PlsW|"
		"Poster Edges|PstE|"
		"Rough Pastels|RghP|"
		"Smudge Stick|SmdS|"
		"Sponge|Spng|"
		"Underpainting|Undr|"
		"Watercolor|Wtrc|"
		"-|-|"
		"+"
		}
	};

resource 'STR#' (kFilterSet3ID+1, purgeable)
	{
		{
		"Accented Edges|AccE|"
		"Angled Strokes|AngS|"
		"Crosshatch|Crsh|"
		"Dark Strokes|DrkS|"
		"Ink Outlines|InkO|"
		"Splatter|Spt |"
		"Sprayed Strokes|SprS|"
		"Sumi-e|Smie|"
		"-|-|"
		"+"
		}
	};

resource 'STR#' (kFilterSet3ID+2, purgeable)
	{
		{
		"Diffuse Glow|DfsG|"
		"Glass|Gls |"
		"Ocean Ripple|OcnR|"
		"-|-|"
		"+"
		}
	};

resource 'STR#' (kFilterSet3ID+3, purgeable)
	{
		{
		"Craquelure|Crql|"
		"Grain|Grn |"
		"Mosaic|MscT|"
		"Patchwork|Ptch|"
		"Stained Glass|StnG|"
		"Texturizer|Txtz|"
		"-|-|"
		"+"
		}
	};

resource 'STR#' (kFilterSet3ID+4, purgeable)
	{
		{
		"Other...|othR|"
		}
	};

	
//-------------------------------------------------------------------------------

// end TriggerFilters.r
