// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2001 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
//-------------------------------------------------------------------------------
//
//	File:
//		PolygonSelect.r
//
//	Copyright 1999-1999, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Description:
//		Resource information for radius, number of sides, and angle of rotation
//		Sample Automation plug-in.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Definitions -- Required by other resources in this rez file.
//-------------------------------------------------------------------------------

// Dictionary (aete) resources:

#define vendorName			"AdobeSDK"
#define plugInAETEComment 	"PolygonSelect example actions plug-in"

#define plugInSuiteID		'sdKG'
#define plugInClassID		plugInSuiteID
#define plugInEventID		'plyS'

//-------------------------------------------------------------------------------
//	Set up included files for Macintosh and Windows.
//-------------------------------------------------------------------------------

#include "PIDefines.h"

#include "Types.r"
#include "SysTypes.r"
#include "PIGeneral.r"
#include "PIActions.h"

#include "PolygonSelectTerminology.h"


//-------------------------------------------------------------------------------
//	Dialog resource
//-------------------------------------------------------------------------------

resource 'DLOG' (kDialogID, plugInName " UI", purgeable) {
	{190, 203, 333, 483},
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
	{	/* array DITLarray: 12 elements */
		/* [1] */
		{7, 210, 27, 278},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{37, 210, 57, 278},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{7, 7, 22, 60},
		StaticText {
			enabled,
			" Radius:"
		},
		/* [4] */
		{15, 3, 74, 193},
		UserItem {
			enabled
		},
		/* [5] */
		{25, 9, 38, 80},
		StaticText {
			disabled,
			"Amount:"
		},
		/* [6] */
		{24, 128, 40, 173},
		EditText {
			enabled,
			""
		},
		/* [7] */
		{50, 9, 65, 118},
		StaticText {
			disabled,
			"Number of Sides:"
		},
		/* [8] */
		{51, 128, 67, 173},
		EditText {
			enabled,
			""
		},
		/* [9] */
		{77, 7, 92, 73},
		StaticText {
			enabled,
			" Rotation:"
		},
		/* [10] */
		{85, 3, 119, 193},
		UserItem {
			enabled
		},
		/* [11] */
		{95, 10, 110, 70},
		StaticText {
			disabled,
			"Angle:"
		},
		/* [12] */
		{96, 128, 112, 173},
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

//-------------------------------------------------------------------------------

// end PolygonSelect.r
