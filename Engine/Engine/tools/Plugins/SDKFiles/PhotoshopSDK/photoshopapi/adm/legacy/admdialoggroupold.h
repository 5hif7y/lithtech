/***********************************************************************/
/*                                                                     */
/* ADMDialogGroupOld.h                                                 */
/* Old versions of ADM Dialog Group Suite                              */
/*                                                                     */
/* ADOBE SYSTEMS INCORPORATED                                          */
/* Copyright 1996-2001 Adobe Systems Incorporated                      */
/* All Rights Reserved                                                 */
/*                                                                     */
/* NOTICE:  Adobe permits you to use, modify, and distribute this file */
/* in accordance with the terms of the Adobe license agreement         */
/* accompanying it. If you have received this file from a source other */
/* than Adobe, then your use, modification, or distribution of it      */
/* requires the prior written permission of Adobe.                     */
/*                                                                     */
/***********************************************************************/

#ifndef __ADMDialogGroupOld__
#define __ADMDialogGroupOld__

#ifndef __ADMTypes__
#include "admTypes.h"
#endif


// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


// =============================================================================
//		* ADM Dialog Group Suite, version 1
// =============================================================================

#define kADMDialogGroupSuiteVersion1 1
#define kADMDialogGroupSuiteVersion 1
#define kADMDialogGroupVersion kADMDialogGroupSuiteVersion1

// -----------------------------------------------------------------------------

typedef struct ADMDialogGroupSuite1
{

	ASErr ASAPI (*GetDialogName)(ADMDialogRef inDialog, const char** outName);

	ASErr ASAPI (*GetDialogGroupInfo)(ADMDialogRef inDialog, const char** outGroupName,
				ASInt32* outPositionCode);

	ASErr ASAPI (*SetDialogGroupInfo)(ADMDialogRef inDialog, const char* inGroupName,
				ASInt32 inPositionCode);

	// The queries below take a docking position code obtained from above functions or prefs

		// You should always create a palette that is not stand alone
	ASBoolean ASAPI (*IsStandAlonePalette)(ASInt32 inPositionCode);

		// Your "show palette" menu would be "Show..." unless palette dock code returns true for
		// IsDockVisible() and IsFrontTab(). 
	ASBoolean ASAPI (*IsDockVisible)(ASInt32 inPositionCode);
	ASBoolean ASAPI (*IsFrontTab)(ASInt32 inPositionCode);

		// You probably won't ever use this, but it here for completeness.
	ASBoolean ASAPI (*IsCollapsed)(ASInt32 inPositionCode);

	ASErr ASAPI (*SetTabGroup)(ADMDialogRef inDialog, const char* inTabGroupName,
				ASBoolean inBringToFront);
 
	ASErr ASAPI (*CountDialogs)(ASInt32* outCount);
	ASErr ASAPI (*GetNthDialog)(ASInt32 inIndex, ADMDialogRef* outDialog);
	ASErr ASAPI (*GetNamedDialog)(const char* inName, ADMDialogRef* outDialog);

	void ASAPI (*ToggleAllFloatingDialogs)();

} ADMDialogGroupSuite1;

typedef ADMDialogGroupSuite1 ADMDialogGroupSuite;	// for historical reasons


// =============================================================================
//		* ADM Dialog Group Suite, version 2
// =============================================================================

#define kADMDialogGroupSuiteVersion2 2

// -----------------------------------------------------------------------------

typedef struct ADMDialogGroupSuite2
{

	// *** This suite is FROZEN.
	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.

	// dialog info

	ASErr ASAPI (*GetDialogName)(ADMDialogRef inDialog, const char** outName);

	ASErr ASAPI (*GetDialogGroupInfo)(ADMDialogRef inDialog, const char** outGroupName,
				ASInt32* outPositionCode);

	ASErr ASAPI (*SetDialogGroupInfo)(ADMDialogRef inDialog, const char* inGroupName,
				ASInt32 inPositionCode);

	// docking group info

		// The position codes used below are obtained from above functions or prefs.

		// You should always create a palette that is not stand alone
	ASBoolean ASAPI (*IsStandAlonePalette)(ASInt32 inPositionCode);

		// Your "show palette" menu would be "Show..." unless palette dock code
		// returns true for IsDockVisible() and IsFrontTab().
	ASBoolean ASAPI (*IsDockVisible)(ASInt32 inPositionCode);
	ASBoolean ASAPI (*IsFrontTab)(ASInt32 inPositionCode);

		// You probably won't ever use this, but it here for completeness.
	ASBoolean ASAPI (*IsCollapsed)(ASInt32 inPositionCode);

	// place a dialog in a tab group

	ASErr ASAPI (*SetTabGroup)(ADMDialogRef inDialog, const char* inTabGroupName,
				ASBoolean inBringToFront);
 
 	// dialog list accessors
 
	ASErr ASAPI (*CountDialogs)(ASInt32* outCount);
	ASErr ASAPI (*GetNthDialog)(ASInt32 inIndex, ADMDialogRef* outDialog);
	ASErr ASAPI (*GetNamedDialog)(const char* inName, ADMDialogRef* outDialog);

	// show/hide floating palettes

	void ASAPI (*ToggleAllFloatingDialogs)();
	void ASAPI (*ShowAllFloatingDialogs)(ASBoolean inShow);

}
ADMDialogGroupSuite2;

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

#endif
