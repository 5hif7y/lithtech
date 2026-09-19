/***********************************************************************/
/*                                                                     */
/* ADMHierarchyList.h                                                  */
/* ADM Hierarchy List Suite                                            */
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

#ifndef __ADMHierarchyList__
#define __ADMHierarchyList__

#ifndef __ADMTypes__
#include "admTypes.h"
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
//	Hierarchy list flags

#define kMultiSelectAcrossHierarchyLevels 0x00000001
#define kHierarchyListEndedWithFrame 0x00000002
#define kHierarchyListTopLevelSpecialBackground 0x00000004
#define kHierarchyListLeafOnly 0x00000008		// entries cannot have sublists


// -----------------------------------------------------------------------------
//	User defined procedures

typedef ASErr ASAPI (*ADMListEntryInitProc)(ADMListEntryRef inEntry);
typedef void ASAPI (*ADMListEntryDrawProc)(ADMListEntryRef inEntry, ADMDrawerRef inDrawer);
typedef ASBoolean ASAPI (*ADMListEntryTrackProc)(ADMListEntryRef inEntry, ADMTrackerRef inTracker);
typedef void ASAPI (*ADMListEntryNotifyProc)(ADMListEntryRef inEntry, ADMNotifierRef inNotifier);
typedef void ASAPI (*ADMListEntryDestroyProc)(ADMListEntryRef inEntry);


// =============================================================================
//		* ADM Hierarchy List Suite
// =============================================================================

#define kADMHierarchyListSuite "ADM Hierarchy List Suite"
#define kADMHierarchyListSuiteVersion4 4

// -----------------------------------------------------------------------------

typedef struct ADMHierarchyListSuite4
{

	// *** This suite is OPEN for changes.
	// *** It will possibly ship with Illustrator 10

	// menu items

	void ASAPI (*SetMenuID)(ADMHierarchyListRef inList, SPPluginRef inMenuResPlugin,
				ASInt32 inMenuResID, const char* inMenuResName);

	ASInt32 ASAPI (*GetMenuID)(ADMHierarchyListRef inList);
	
	// item dereference
	
	ADMItemRef ASAPI (*GetItem)(ADMHierarchyListRef inList);

	// user storage
	
	void ASAPI (*SetUserData)(ADMHierarchyListRef inList, ADMUserData inData);
	ADMUserData ASAPI (*GetUserData)(ADMHierarchyListRef inList);

	// item initialization proc

	void ASAPI (*SetInitProc)(ADMHierarchyListRef inList, ADMListEntryInitProc inInitProc);

	void ASAPI (*SetInitProcRecursive)(ADMHierarchyListRef inList,
				ADMListEntryInitProc inInitProc);

	ADMListEntryInitProc ASAPI (*GetInitProc)(ADMHierarchyListRef inList);

	// item draw proc
	
	void ASAPI (*SetDrawProc)(ADMHierarchyListRef inList, ADMListEntryDrawProc inDrawProc);

	void ASAPI (*SetDrawProcRecursive)(ADMHierarchyListRef inList,
				ADMListEntryDrawProc inDrawProc);

	ADMListEntryDrawProc ASAPI (*GetDrawProc)(ADMHierarchyListRef inList);
	
	// item tracking proc
	
	void ASAPI (*SetTrackProc)(ADMHierarchyListRef inList, ADMListEntryTrackProc inTrackProc);

	void ASAPI (*SetTrackProcRecursive)(ADMHierarchyListRef inList,
				ADMListEntryTrackProc inTrackProc);

	ADMListEntryTrackProc ASAPI (*GetTrackProc)(ADMHierarchyListRef inList);

	// item action mask
	
	void ASAPI (*SetMask)(ADMHierarchyListRef inEntry, ADMActionMask inMask);
	void ASAPI (*SetMaskRecursive)(ADMHierarchyListRef inEntry, ADMActionMask inMask);
	ADMActionMask ASAPI (*GetMask)(ADMHierarchyListRef inEntry);

	// item notify proc
	
	void ASAPI (*SetNotifyProc)(ADMHierarchyListRef inList,
				ADMListEntryNotifyProc inNotifyProc);

	void ASAPI (*SetNotifyProcRecursive)(ADMHierarchyListRef inList,
				ADMListEntryNotifyProc inNotifyProc);

	ADMListEntryNotifyProc ASAPI (*GetNotifyProc)(ADMHierarchyListRef inList);

	// user data for item notifier proc
	
	void ASAPI (*SetNotifierData)(ADMHierarchyListRef inEntry, ADMUserData inData);
	ADMUserData ASAPI (*GetNotifierData)(ADMHierarchyListRef inEntry);

	// item destroy proc

	void ASAPI (*SetDestroyProc)(ADMHierarchyListRef inList,
				ADMListEntryDestroyProc inDestroyProc);

	void ASAPI (*SetDestroyProcRecursive)(ADMHierarchyListRef inList,
				ADMListEntryDestroyProc inDestroyProc);

	ADMListEntryDestroyProc ASAPI (*GetDestroyProc)(ADMHierarchyListRef inList);

	// item entry bounds
	
	void ASAPI (*SetEntryWidth)(ADMHierarchyListRef inList, ASInt32 inWidth);
	void ASAPI (*SetEntryWidthRecursive)(ADMHierarchyListRef inList, ASInt32 inWidth);
	ASInt32 ASAPI (*GetEntryWidth)(ADMHierarchyListRef inList);
	ASInt32 ASAPI (*GetNonLeafEntryWidth)(ADMHierarchyListRef inList);
	
	void ASAPI (*SetEntryHeight)(ADMHierarchyListRef inList, ASInt32 inHeight);
	void ASAPI (*SetEntryHeightRecursive)(ADMHierarchyListRef inList, ASInt32 inHeight);
	ASInt32 ASAPI (*GetEntryHeight)(ADMHierarchyListRef inList);
	
	void ASAPI (*SetEntryTextRect)(ADMHierarchyListRef inList, const ASRect* inRect);
	void ASAPI (*SetEntryTextRectRecursive)(ADMHierarchyListRef inList, const ASRect* inRect);
	void ASAPI (*GetEntryTextRect)(ADMHierarchyListRef inList, ASRect* outRect);

	void ASAPI (*SetNonLeafEntryTextRect)(ADMHierarchyListRef inList, const ASRect* inRect);
	void ASAPI (*SetNonLeafEntryTextRectRecursive)(ADMHierarchyListRef inList, const ASRect* inRect);
	void ASAPI (*GetNonLeafEntryTextRect)(ADMHierarchyListRef inList, ASRect* outRect);
	
	// item list manipulation
	
	ADMListEntryRef ASAPI (*InsertEntry)(ADMHierarchyListRef inList, ASInt32 inIndex);
	void ASAPI (*RemoveEntry)(ADMHierarchyListRef inList, ASInt32 inIndex);
	
	ADMListEntryRef ASAPI (*GetEntry)(ADMHierarchyListRef inList, ASInt32 inEntryID);
	ADMListEntryRef ASAPI (*IndexEntry)(ADMHierarchyListRef inList, ASInt32 inIndex);
	ADMListEntryRef ASAPI (*FindEntry)(ADMHierarchyListRef inList, const char* inText);
	
	ADMListEntryRef ASAPI (*PickEntry)(ADMHierarchyListRef inList, const ASPoint* inPoint);
	ADMListEntryRef ASAPI (*PickLeafEntry)(ADMHierarchyListRef inList, const ASPoint* inPoint);
	
	ADMListEntryRef ASAPI (*GetActiveEntry)(ADMHierarchyListRef inList);
	ADMListEntryRef ASAPI (*GetActiveLeafEntry)(ADMHierarchyListRef inList);
	
	// selection list manipulation
	
	ADMListEntryRef ASAPI (*IndexSelectedEntry)(ADMHierarchyListRef inList,
				ASInt32 inSelectionIndex);

	ADMListEntryRef ASAPI (*IndexAllSelectedEntriesInHierarchy)(ADMHierarchyListRef inList,
				ASInt32 inSelectionIndex);

	ADMListEntryRef ASAPI (*IndexUnNestedSelectedEntriesInHierarchy)(ADMHierarchyListRef inList,
				ASInt32 inSelectionIndex);
	
	ASInt32 ASAPI (*NumberOfEntries)(ADMHierarchyListRef inList);
	
	ASInt32 ASAPI (*NumberOfSelectedEntries)(ADMHierarchyListRef inList);
	ASInt32 ASAPI (*NumberOfAllSelectedEntriesInHierarchy)(ADMHierarchyListRef inList);
	ASInt32 ASAPI (*NumberOfUnNestedSelectedEntriesInHierarchy)(ADMHierarchyListRef inList);
	
	// item hierarchy
	
	ADMListEntryRef ASAPI (*GetParentEntry)(ADMHierarchyListRef inList);

	// coordinate system conversion

	void ASAPI (*GetLocalRect)(ADMHierarchyListRef inList, ASRect* outRect);

	void ASAPI (*LocalToScreenPoint)(ADMHierarchyListRef inList, ASPoint* ioPoint);
	void ASAPI (*ScreenToLocalPoint)(ADMHierarchyListRef inList, ASPoint* ioPoint);

	void ASAPI (*LocalToGlobalPoint)(ADMHierarchyListRef inList, ASPoint* ioPoint);
	void ASAPI (*GlobalToLocalPoint)(ADMHierarchyListRef inList, ASPoint* ioPoint);

	void ASAPI (*LocalToGlobalRect)(ADMHierarchyListRef inList, ASRect* ioRect);
	void ASAPI (*GlobalToLocalRect)(ADMHierarchyListRef inList, ASRect* ioRect);

	// item margin accessors

	void ASAPI (*SetIndentationWidth)(ADMHierarchyListRef inList, ASInt32 inWidth);
	void ASAPI (*SetIndentationWidthRecursive)(ADMHierarchyListRef inList, ASInt32 inWidth);
	ASInt32 ASAPI (*GetIndentationWidth)(ADMHierarchyListRef inList);

	void ASAPI (*SetLocalLeftMargin)(ADMHierarchyListRef inList, ASInt32 inWidth);
	ASInt32 ASAPI (*GetLocalLeftMargin)(ADMHierarchyListRef inList);
	ASInt32 ASAPI (*GetGlobalLeftMargin)(ADMHierarchyListRef inList);

	void ASAPI (*SetDivided)(ADMHierarchyListRef inList, ASBoolean inDivided);
	void ASAPI (*SetDividedRecursive)(ADMHierarchyListRef inList, ASBoolean inDivided);
	ASBoolean ASAPI (*GetDivided)(ADMHierarchyListRef inList);

	void ASAPI (*SetFlags)(ADMHierarchyListRef inList, ASInt32 inFlags);
	void ASAPI (*SetFlagsRecursive)(ADMHierarchyListRef inList, ASInt32 inFlags);
	ASInt32 ASAPI (*GetFlags)(ADMHierarchyListRef inList);

	// item invalidation

	void ASAPI (*Invalidate)(ADMHierarchyListRef inList);

	// leaf item accessors

	ADMListEntryRef ASAPI (*IndexLeafEntry)(ADMHierarchyListRef inList, ASInt32 inLeafItem);
	ASInt32 ASAPI (*GetLeafIndex)(ADMHierarchyListRef inList, ADMListEntryRef inEntry);
	ASInt32 ASAPI (*NumberOfLeafEntries)(ADMHierarchyListRef inList);

	// item sequence manipulation
	
	void ASAPI (*SwapEntries)(ADMHierarchyListRef inList, ASInt32 inFromIndex,
				ASInt32 inToIndex);

	ADMListEntryRef ASAPI (*InsertGivenEntry)(ADMHierarchyListRef inList,
				ADMListEntryRef inEntry, ASInt32 inIndex);

	ADMListEntryRef ASAPI (*UnlinkEntry)(ADMHierarchyListRef inList, ASInt32 inIndex);

	// item selection

	void ASAPI (*DeselectAll)(ADMHierarchyListRef inList);
	
	// customizing appearance
	
	void ASAPI (*SetBackgroundColor)(ADMHierarchyListRef inList,
				ADMColor inColor);

	// expanded item accessors

	ADMListEntryRef ASAPI (*IndexExpandedEntry)(ADMHierarchyListRef inList, ASInt32 inExpandedItem);
	ASInt32 ASAPI (*GetExpandedIndex)(ADMHierarchyListRef inList, ADMListEntryRef inEntry);
	ASInt32 ASAPI (*NumberOfExpandedEntries)(ADMHierarchyListRef inList);

	// restrict item invalidation
	
	void ASAPI (*StartMultipleItemInvalidate)(ADMHierarchyListRef inList);
	void ASAPI (*StopMultipleItemInvalidate)(ADMHierarchyListRef inList);

} ADMHierarchyListSuite4;


// -----------------------------------------------------------------------------
//	Old versions of suites

#ifndef __ADMHierarchyListOld__
#ifdef MAC_ENV
#include "ADMHierarchyListOld.h"
#else
#include ".\Legacy\ADMHierarchyListOld.h"
#endif
#endif

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------

#endif	// __ADMHierarchyList__
