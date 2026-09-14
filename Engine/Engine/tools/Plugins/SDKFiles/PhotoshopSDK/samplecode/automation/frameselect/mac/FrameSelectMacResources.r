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
//		FrameSelectMacResources.r
//
//	Description:
//		Resource information for inset/outset selection
//		sample Automation plug-in.
//
//-------------------------------------------------------------------------------

#include "PIDefines.h"

#include "Types.r"
#include "SysTypes.r"
#include "PIGeneral.r"
#include "PIActions.h"

#include "FrameSelectTerminology.h"

//-------------------------------------------------------------------------------
//	Dialog resource
//-------------------------------------------------------------------------------

resource 'DLOG' (kDialogID, plugInName " UI", purgeable) {
	{190, 203, 450, 523},
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
	{	/* array DITLarray: 18 elements */
		/* [1] */
		{8, 252, 28, 320},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{40, 252, 60, 320},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{9, 12, 29, 64},
		StaticText {
			enabled,
			" Gutter:"
		},
		/* [4] */
		{17, 0, 96, 240},
		UserItem {
			enabled
		},
		/* [5] */
		{34, 36, 54, 108},
		StaticText {
			disabled,
			"Amount:"
		},
		/* [6] */
		{35, 120, 51, 220},
		EditText {
			enabled,
			""
		},
		/* [7] */
		{65, 36, 85, 108},
		StaticText {
			disabled,
			"Units:"
		},
		/* [8] */
		{63, 117, 83, 223},
		Control {
			enabled,
			16001
		},
		/* [9] */
		{101, 12, 121, 64},
		StaticText {
			enabled,
			" Offset:"
		},
		/* [10] */
		{109, 0, 253, 240},
		UserItem {
			enabled
		},
		/* [11] */
		{129, 36, 149, 108},
		StaticText {
			disabled,
			"Top:"
		},
		/* [12] */
		{130, 120, 146, 220},
		EditText {
			enabled,
			""
		},
		/* [13] */
		{161, 36, 181, 108},
		StaticText {
			disabled,
			"Left:"
		},
		/* [14] */
		{162, 120, 178, 220},
		EditText {
			enabled,
			""
		},
		/* [15] */
		{191, 36, 211, 108},
		StaticText {
			disabled,
			"Bottom:"
		},
		/* [16] */
		{192, 120, 208, 220},
		EditText {
			enabled,
			""
		},
		/* [17] */
		{221, 36, 241, 108},
		StaticText {
			disabled,
			"Right:"
		},
		/* [18] */
		{222, 120, 238, 220},
		EditText {
			enabled,
			""
		}
	}
};

resource 'CNTL' (kDialogID, "Units", purgeable) {
	{16, 4, 36, 110},
	0,
	visible,
	0,
	kDialogID,
	1016,
	0,
	""
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

resource 'STR#' (kUnitsID, purgeable)
{
	{
		"pixels",
		"inches",
		"cm",
		"points",
		"picas",
		"percent"
	}
};

//-------------------------------------------------------------------------------

// end FrameSelectMacResources.r
