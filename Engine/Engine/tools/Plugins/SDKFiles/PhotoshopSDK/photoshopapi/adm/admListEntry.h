/***********************************************************************/
/*                                                                     */
/* ADMListEntry.h                                                      */
/* ADM List Entry Suite                                                */
/*                                                                     */
/* ADOBE SYSTEMS INCORPORATED                                          */
/* Copyright 1997-2001 Adobe Systems Incorporated                      */
/* All Rights Reserved                                                 */
/*                                                                     */
/* NOTICE:  Adobe permits you to use, modify, and distribute this file */
/* in accordance with the terms of the Adobe license agreement         */
/* accompanying it. If you have received this file from a source other */
/* than Adobe, then your use, modification, or distribution of it      */
/* requires the prior written permission of Adobe.                     */
/*                                                                     */
/* Started by Wei Huang, 10/17/1997                                    */
/*                                                                     */
/***********************************************************************/

#ifndef __ADMListEntry__
#define __ADMListEntry__

#ifndef __ADMTypes__
#include "admTypes.h"
#endif

#ifndef __ADMTracker__
#include "admTracker.h"
#endif

#ifndef __ASHelp__
#include "ashelp.h"
#endif


// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN

// Mac OS headers #define GetItem, but we want to use it as a name.
#ifdef GetItem
#undef GetItem
#endif

// -----------------------------------------------------------------------------

typedef ASBoolean ASAPI (*ADMListEntryTimerProc)(ADMListEntryRef inEntry,
			ADMTimerRef inTimer);

typedef void ASAPI (*ADMListEntryTimerAbortProc)(ADMListEntryRef inEntry,
			ADMTimerRef inTimer, ADMAction abortAction);


// =============================================================================
//		* ADM List Entry Suite
// =============================================================================

#define kADMListEntrySuite "ADM List Entry Suite"
#define kADMListEntrySuiteVersion4 4

// -----------------------------------------------------------------------------

typedef struct ADMListEntrySuite4
{

	// *** This suite is FROZEN.
	// *** It shipped with Acrobat 5.0.

	// entry creation/destruction

	ADMListEntryRef ASAPI (*Create)(ADMHierarchyListRef inList);
	void ASAPI (*Destroy)(ADMListEntryRef inEntry);
	
	// entry sequence
	
	ASInt32 ASAPI (*GetIndex)(ADMListEntryRef inEntry);
	
	// container accessors
	
	ADMItemRef ASAPI (*GetItem)(ADMListEntryRef inEntry);
	ADMHierarchyListRef ASAPI (*GetList)(ADMListEntryRef inEntry);

	// hierarchy accessors
	
	ADMHierarchyListRef ASAPI (*GetChildList)(ADMListEntryRef inEntry);
	ADMHierarchyListRef ASAPI (*CreateChildList)(ADMListEntryRef inEntry);
	void ASAPI (*DeleteChildList)(ADMListEntryRef inEntry);

	void ASAPI (*ExpandHierarchy)(ADMListEntryRef inEntry, ASBoolean inExpand);
	ASBoolean ASAPI (*IsHierarchyExpanded)(ADMListEntryRef inEntry);

	void ASAPI (*HideEntryName)(ADMListEntryRef inEntry, ASBoolean inHideName);
	ASBoolean ASAPI (*IsEntryNameHidden)(ADMListEntryRef inEntry);

	void ASAPI (*EnableChildSelection)(ADMListEntryRef inEntry, ASBoolean inEnable);
	ASBoolean ASAPI (*IsChildSelectable)(ADMListEntryRef inEntry);

	ASInt32 ASAPI (*GetHierarchyDepth)(ADMListEntryRef inEntry);
	ASInt32 ASAPI (*GetVisualHierarchyDepth)(ADMListEntryRef inEntry);

	// entry ID

	void ASAPI (*SetID)(ADMListEntryRef inEntry, ASInt32 inEntryID);
	ASInt32 ASAPI (*GetID)(ADMListEntryRef inEntry);

	// entry user storage
	
	void ASAPI (*SetUserData)(ADMListEntryRef inEntry, ADMUserData inUserData);
	ADMUserData ASAPI (*GetUserData)(ADMListEntryRef inEntry);

	// default behaviors

	void ASAPI (*DefaultDraw)(ADMListEntryRef inEntry, ADMDrawerRef inDrawer);
	ASBoolean ASAPI (*DefaultTrack)(ADMListEntryRef inEntry, ADMTrackerRef inTracker);
	void ASAPI (*DefaultNotify)(ADMListEntryRef inEntry, ADMNotifierRef inNotifier);
	void ASAPI (*SendNotify)(ADMListEntryRef inEntry, const char* inNotifierType);

	// selection
	
	void ASAPI (*Select)(ADMListEntryRef inEntry, ASBoolean inSelect);
	ASBoolean ASAPI (*IsSelected)(ADMListEntryRef inEntry);
	ASBoolean ASAPI (*AreChildrenSelected)(ADMListEntryRef inEntry);

	// display settings
	
	void ASAPI (*Enable)(ADMListEntryRef inEntry, ASBoolean inEnable);
	ASBoolean ASAPI (*IsEnabled)(ADMListEntryRef inEntry);
	
	void ASAPI (*Activate)(ADMListEntryRef inEntry, ASBoolean inActivate);
	ASBoolean ASAPI (*IsActive)(ADMListEntryRef inEntry);
	
	void ASAPI (*Check)(ADMListEntryRef inEntry, ASBoolean inCheck);
	ASBoolean ASAPI (*IsChecked)(ADMListEntryRef inEntry);
	
	void ASAPI (*MakeSeparator)(ADMListEntryRef inEntry, ASBoolean inSeparator);
	ASBoolean ASAPI (*IsSeparator)(ADMListEntryRef inEntry);

	// bounds accessors

	void ASAPI (*GetLocalRect)(ADMListEntryRef inEntry, ASRect* outLocalRect);
	void ASAPI (*GetExpandArrowLocalRect)(ADMListEntryRef inEntry, ASRect* outLocalRect);
	void ASAPI (*GetBoundsRect)(ADMListEntryRef inEntry, ASRect* outBoundsRect);

	// coordinate transformations
	
	void ASAPI (*LocalToScreenPoint)(ADMListEntryRef inEntry, ASPoint* ioPoint);
	void ASAPI (*ScreenToLocalPoint)(ADMListEntryRef inEntry, ASPoint* ioPoint);
	
	void ASAPI (*LocalToScreenRect)(ADMListEntryRef inEntry, ASRect* inRect);
	void ASAPI (*ScreenToLocalRect)(ADMListEntryRef inEntry, ASRect* inRect);

	// redrawing
	
	void ASAPI (*Invalidate)(ADMListEntryRef inEntry);
	void ASAPI (*Update)(ADMListEntryRef inEntry);

	// entry pictures
	
	void ASAPI (*SetPictureID)(ADMListEntryRef inEntry, ASInt32 inPictureResID,
				const char* inPictureResName);

	void ASAPI (*GetPictureID)(ADMListEntryRef inEntry, ASInt32* inPictureResID,
				const char** outPictureResName);
	
	void ASAPI (*SetSelectedPictureID)(ADMListEntryRef inEntry, ASInt32 inPictureResID,
				const char* inPictureResName);

	void ASAPI (*GetSelectedPictureID)(ADMListEntryRef inEntry, ASInt32* outPictureResID,
				const char** outPictureResName);
	
	void ASAPI (*SetDisabledPictureID)(ADMListEntryRef inEntry, ASInt32 inPictureResID,
				const char* inPictureResName);

	void ASAPI (*GetDisabledPictureID)(ADMListEntryRef inEntry, ASInt32* outPictureResID,
				const char** outPictureResName);
	
	// entry text
	
	void ASAPI (*SetText)(ADMListEntryRef inEntry, const char* inText);
	void ASAPI (*SetTextW)(ADMListEntryRef inEntry, const ASUnicode* inText);

	void ASAPI (*GetText)(ADMListEntryRef inEntry, char* outText, ASInt32 inMaxLength);
	void ASAPI (*GetTextW)(ADMListEntryRef inEntry, ASUnicode* outText, ASInt32 inMaxLength);

	ASInt32 ASAPI (*GetTextLength)(ADMListEntryRef inEntry);
	ASInt32 ASAPI (*GetTextLengthW)(ADMListEntryRef inEntry);
	
	// entry timer
	
	ADMTimerRef ASAPI (*CreateTimer)(ADMListEntryRef inEntry, ASUInt32 inMilliseconds,
				ADMActionMask inAbortMask, ADMListEntryTimerProc inTimerProc,
				ADMListEntryTimerAbortProc inAbortProc, ASInt32 inOptions);

	void ASAPI (*AbortTimer)(ADMListEntryRef inEntry, ADMTimerRef inTimer);

	// entry visibility

	void ASAPI (*MakeInBounds)(ADMListEntryRef inEntry);
	ASBoolean ASAPI (*IsInBounds)(ADMListEntryRef inEntry);

	// entry help

	ASHelpID ASAPI (*GetHelpID)(ADMListEntryRef inEntry);
	void ASAPI (*SetHelpID)(ADMListEntryRef inEntry, ASHelpID inHelpID);
	void ASAPI (*Help)(ADMListEntryRef inEntry);

	// entry pictures

	void ASAPI (*SetPicture)(ADMListEntryRef inEntry, ADMIconRef inPicture);
	ADMIconRef ASAPI (*GetPicture)(ADMListEntryRef inEntry);

	void ASAPI (*SetSelectedPicture)(ADMListEntryRef inEntry, ADMIconRef inPicture);
	ADMIconRef ASAPI (*GetSelectedPicture)(ADMListEntryRef inEntry);

	void ASAPI (*SetDisabledPicture)(ADMListEntryRef inEntry, ADMIconRef inPicture);
	ADMIconRef ASAPI (*GetDisabledPicture)(ADMListEntryRef inEntry);

	// for in-place editing: text rect used to display the edit field.

	void ASAPI (*SetEntryTextRect)(ADMListEntryRef inEntry, ASRect* inRect);

	// for controls in lists

	ADMItemRef ASAPI (*GetEntryItem)(ADMListEntryRef inEntry);
	void ASAPI (*SetEntryItem)(ADMListEntryRef inEntry, ADMItemRef inItem);
	
	// customizing appearance
	
	void ASAPI (*SetFont)(ADMListEntryRef inEntry, ADMFont inFont);
	void ASAPI (*SetTextColor)(ADMListEntryRef inEntry, ADMColor inColor);
	void ASAPI (*SetBackgroundColor)(ADMListEntryRef inEntry, ADMColor inColor);
	void ASAPI (*SetDividingLineColor)(ADMListEntryRef inEntry, ADMColor inColor);

}
ADMListEntrySuite4;


// -----------------------------------------------------------------------------
//	Old versions of suites

#ifndef __ADMListEntryOld__
#ifdef MAC_ENV
#include "ADMListEntryOld.h"
#else
#include ".\Legacy\ADMListEntryOld.h"
#endif
#endif

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------

#endif	// __ADMListEntry__
