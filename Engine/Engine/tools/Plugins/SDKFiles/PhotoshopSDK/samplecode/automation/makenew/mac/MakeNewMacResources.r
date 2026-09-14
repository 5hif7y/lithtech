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
//		MakeNew.r
//
//	Description:
//		Resource information for make new document
//		sample Automation plug-in.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Set up included files for Macintosh and Windows.
//-------------------------------------------------------------------------------

#include "PIDefines.h"

#include "Types.r"
#include "SysTypes.r"
#include "PIGeneral.r"
#include "PIActions.h"

#include "MakeNewTerminology.h"


//-------------------------------------------------------------------------------
//	Dialog resource
//-------------------------------------------------------------------------------

resource 'DLOG' (kDialogID, plugInName " UI", purgeable) {
	{87, 27, 307, 319},
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
	{	/* array DITLarray: 14 elements */
		/* [1] */
		{8, 216, 28, 284},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{40, 216, 60, 284},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{12, 8, 32, 80},
		StaticText {
			disabled,
			"Width:"
		},
		/* [4] */
		{13, 92, 29, 192},
		EditText {
			enabled,
			""
		},
		/* [5] */
		{44, 8, 64, 80},
		StaticText {
			disabled,
			"Height:"
		},
		/* [6] */
		{45, 92, 61, 192},
		EditText {
			enabled,
			""
		},
		/* [7] */
		{74, 8, 94, 88},
		StaticText {
			disabled,
			"Resolution:"
		},
		/* [8] */
		{75, 92, 91, 192},
		EditText {
			enabled,
			""
		},
		/* [9] */
		{108, 4, 140, 96},
		StaticText {
			disabled,
			"Mode:"
		},
		/* [10] */
		{108, 104, 124, 264},
		RadioButton {
			enabled,
			"RGB"
		},
		/* [11] */
		{124, 104, 140, 264},
		RadioButton {
			enabled,
			"CMYK"
		},
		/* [12] */
		{156, 4, 188, 80},
		StaticText {
			disabled,
			"Fill:"
		},
		/* [13] */
		{156, 104, 172, 264},
		RadioButton {
			enabled,
			"White"
		},
		/* [14] */
		{172, 104, 188, 264},
		RadioButton {
			enabled,
			"Transparent"
		}
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

//-------------------------------------------------------------------------------

// end MakeNew.r
