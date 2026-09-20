/***********************************************************************/
/*                                                                     */
/* ADMDialog.h                                                         */
/* ADM Dialog Suite                                                    */
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
/* Started by Dave Lazarony, 03/10/1996                                */
/*                                                                     */
/***********************************************************************/

#ifndef __ADMDialog__
#define __ADMDialog__

#ifndef __ADMItem__
#include "admItem.h"
#endif

#ifndef __ADMTypes__
#include "admTypes.h"
#endif

#ifndef __ASHelp__
#include "ashelp.h"
#endif

#ifndef __SPInterface__
#include "SPInterf.h"
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
//	Create custom item message.

#define kADMCaller "Adobe Dialog Manager"
#define kADMCreateCustomItem "Create Custom Item"

typedef struct
{
	SPMessageData d;
	ADMDialogRef dialog;
	ASInt32 itemID;
	ADMItemType itemType;
	ASRect boundsRect;
	ADMItemInitProc initProc;
	ADMUserData data;
	ADMItemRef item;
}
ADMCreateCustomItemMessage;


// -----------------------------------------------------------------------------
//	Dialog options (for Create() call).
//	Options above 1L << 28 are reserved for internal use.

#define kADMTabbedDialogShowsCycleOption (1L << 0)
		// When creating tabbed dialog with a cycle button on the tab.

#define kADMPassMouseDownEventInModalDialogOption (1L << 1)
		// To allow modal dialogs pass mouse down events through to
		// the user dialog tracker on the application side.

#define kReservedForUseByCustomResourcesOption (1L << 2)

#define kADMKeypadEnterNotDefault (1L << 3)
		// Keypad 'enter' key does not activate default item.

#define kADMCreateDialogItemsHiddenByDefault (1L << 4)
		// Reduce flicker by creating items hidden.

#define kADMForceRomanFontForDialog (1L << 5)
		// Forces for all items within dialog, except as overridden.

#define kADMTrackEnterBeforeDialogDoesOK	(1L << 6)
		// Track the enter keys (carriage return and keypad enter) before the
		// dialog treats the event as equivalent to pressing the OK button --
		// and prevent that behavior if the tracker returns true. Note that by
		// default, the enter keys cause text item trackers to commit their text
		// and return true, so this option normally prevents the "OK" behavior
		// when enter is pressed within a text item.
		// (This option currently relevant only on Mac platform.)

#define kADMModalDialogHasPaletteSystemControlsOption (1L << 7)
		// 0 by default. If set, ADM Modal dialogs on Windows will have a 
		// close box on the top right hand corner. Also there is a host
		// option that a user can use if all dialogs in the application
		// need that behavior. 
		// dagashe:09/29/00:added for Acrobat 5.0 bug #382265

#define kADMCreatePopupDialogAsFloatingClassOption (1L << 8)
		//If this option is set for a dialog of style kADMPopupControldialogStyle
		//then ADM will create the dialog of kFloatingwindowclass. This option
		//is currently used only on MacOSX.

// -----------------------------------------------------------------------------
//	Dialog styles (for Create() call).
//	Style constants above FFFF are reserved for host use.
 
typedef enum
{
	kADMModalDialogStyle = 0,
	kADMAlertDialogStyle = 1,
	kADMFloatingDialogStyle = 2,
	kADMTabbedFloatingDialogStyle = 3,
	kADMResizingFloatingDialogStyle = 4,
	kADMTabbedResizingFloatingDialogStyle = 5,
	kADMPopupDialogStyle = 6,
	kADMNoCloseFloatingDialogStyle = 7,
	kADMSystemAlertDialogStyle = 8,
	kADMPopupControlDialogStyle = 9,
	kADMResizingModalDialogStyle = 10,
	kADMLeftSidedFloatingDialogStyle = 11,
	kADMLeftSidedNoCloseFloatingDialogStyle = 12,
	kADMNoTitleDockFloatingDialogStyle = 13,
	kADMHostDefinedDialogStyle = 65536,
	kADMDummyDialogStyle = 0xFFFFFFFF
}
ADMDialogStyle;

 
// -----------------------------------------------------------------------------
//	Standard dialog item IDs

typedef enum
{
	kADMUniqueItemID = 0,
	kADMFirstItemID = -1,
	kADMLastItemID = -2,
	kADMDefaultItemID = -3,
	kADMCancelItemID = -4,
	kADMMenuItemID = -5,
	kADMResizeItemID = -6,
	kADMPrivateUniqueItemID = -7,	// dhearst 1/31/01 - changed from -10 to kADMFirstUnusedPrivateItemID.
									// Part of fix for EveParser Bug (ABTS #409214)
	// The constant kADMFirstUnusedPrivateItemID should ALWAYS be one more negative than the last
	// non-Dummy value in this enum. If you add more enum values, be sure to update kADMFirstUnusedPrivateItemID.
	kADMFirstUnusedPrivateItemID = -8,
	kADMDummyItemID = 0x7FFFFFFF
}
ADMStandardDialogItemID;


// -----------------------------------------------------------------------------
//	Dialog error codes
 
#define kDialogResourceNotFoundError 'DLOG'
			// error data == resID

#define kDialogItemListResourceNotFoundError 'DITL'
			// error data == resID

#define kCouldntCreateItemError '!itm'
			// error data == resID


// -----------------------------------------------------------------------------
//	User-provided function prototypes

// ADMItemRef and ADMDialogRef are declared in ADMTypes.h

typedef ASErr ASAPI (*ADMDialogInitProc)(ADMDialogRef inDialog);
typedef void ASAPI (*ADMDialogDrawProc)(ADMDialogRef inDialog, ADMDrawerRef inDrawer);
typedef ASBoolean ASAPI (*ADMDialogTrackProc)(ADMDialogRef inDialog, ADMTrackerRef inTracker);
typedef void ASAPI (*ADMDialogNotifyProc)(ADMDialogRef inDialog, ADMNotifierRef inNotifier);
typedef void ASAPI (*ADMDialogDestroyProc)(ADMDialogRef inDialog);
typedef ASBoolean ASAPI (*ADMDialogTimerProc)(ADMDialogRef inDialog, ADMTimerRef inTimerID);
typedef void ASAPI (*ADMDialogTimerAbortProc)(ADMDialogRef inDialog, ADMTimerRef inTimerID,
			ADMAction inAbortAction);


// =============================================================================
//		* ADM Dialog Suite
// =============================================================================

#define kADMDialogSuite "ADM Dialog Suite"
#define kADMDialogSuiteVersion8 8

// -----------------------------------------------------------------------------

typedef struct ADMDialogSuite8
{

	// *** This suite is OPEN.
	// *** It will ship with Illustrator10.

	// dialog creation

	ADMDialogRef ASAPI (*Create)(SPPluginRef inPluginRef, const char* inName,
				ASInt32 inDialogID, ADMDialogStyle inDialogStyle,
				ADMDialogInitProc inInitProc, ADMUserData inUserData, ASInt32 inOptions);

	// modal dialogs

	ASInt32 ASAPI (*Modal)(SPPluginRef inPluginRef, const char* inName,
				ASInt32 inDialogID, ADMDialogStyle inDialogStyle,
				ADMDialogInitProc inInitProc, ADMUserData inUserData, ASInt32 inOptions);

	ASInt32 ASAPI (*PopupModal)(SPPluginRef inPluginRef, const char* inName,
				ASInt32 inDialogID, ADMDialogInitProc inInitProc, ADMUserData inUserData,
				ASInt32 inOptions);

	ASInt32 ASAPI (*DisplayAsModal)(ADMDialogRef inDialog);

	// dialog destruction

	void ASAPI (*Destroy)(ADMDialogRef inDialog);
	
	// dialog draw proc
	
	void ASAPI (*SetDrawProc)(ADMDialogRef inDialog, ADMDialogDrawProc inDrawProc);
 	ADMDialogDrawProc ASAPI (*GetDrawProc)(ADMDialogRef inDialog);
	void ASAPI (*DefaultDraw)(ADMDialogRef inDialog, ADMDrawerRef inDrawer);

	// dialog track proc
	
	void ASAPI (*SetTrackProc)(ADMDialogRef inDialog, ADMDialogTrackProc inTrackProc);
	ADMDialogTrackProc ASAPI (*GetTrackProc)(ADMDialogRef inDialog);
	ASBoolean ASAPI (*DefaultTrack)(ADMDialogRef inDialog, ADMTrackerRef inTracker);

	void ASAPI (*SetMask)(ADMDialogRef inDialog, ADMActionMask inMask);
	ADMActionMask ASAPI (*GetMask)(ADMDialogRef inDialog);

	// dialog notify proc

	void ASAPI (*SetNotifyProc)(ADMDialogRef inDialog, ADMDialogNotifyProc inNotifyProc);
	ADMDialogNotifyProc ASAPI (*GetNotifyProc)(ADMDialogRef inDialog);
	void ASAPI (*DefaultNotify)(ADMDialogRef inDialog, ADMNotifierRef inNotifier);
	void ASAPI (*SendNotify)(ADMDialogRef inDialog, const char* inNotifierType);

	void ASAPI (*SetNotifierData)(ADMDialogRef inDialog, ADMUserData inUserData);
	ADMUserData ASAPI (*GetNotifierData)(ADMDialogRef inDialog);

	// dialog destroy proc

	void ASAPI (*SetDestroyProc)(ADMDialogRef inDialog, ADMDialogDestroyProc inDestroyProc);
	ADMDialogDestroyProc ASAPI (*GetDestroyProc)(ADMDialogRef inDialog);

	// dialog timer
	
	ADMTimerRef ASAPI (*CreateTimer)(ADMDialogRef inDialog, ASUInt32 inMilliseconds,
				ADMActionMask inAbortMask, ADMDialogTimerProc inTimerProc,
				ADMDialogTimerAbortProc inAbortProc, ASInt32 inOptions);
	
	void ASAPI (*AbortTimer)(ADMDialogRef inDialog, ADMTimerRef inTimerID);

	// dialog user data
	
	void ASAPI (*SetUserData)(ADMDialogRef inDialog, ADMUserData inUserData);
	ADMUserData ASAPI (*GetUserData)(ADMDialogRef inDialog);

	// dialog information accessors

	ASAPI const char* (*GetDialogName)(ADMDialogRef inDialog);
	ASInt32 ASAPI (*GetID)(ADMDialogRef inDialog);
	SPPluginRef ASAPI (*GetPluginRef)(ADMDialogRef inDialog);
	ASWindowRef ASAPI (*GetWindowRef)(ADMDialogRef inDialog);

	void ASAPI (*SetDialogStyle)(ADMDialogRef inDialog, ADMDialogStyle inDialogStyle);
	ADMDialogStyle ASAPI (*GetDialogStyle)(ADMDialogRef inDialog);

	// dialog state accessors
	
	void ASAPI (*Show)(ADMDialogRef inDialog, ASBoolean inShow);
	ASBoolean ASAPI (*IsVisible)(ADMDialogRef inDialog);
	
	void ASAPI (*Enable)(ADMDialogRef inDialog, ASBoolean inEnable);
	ASBoolean ASAPI (*IsEnabled)(ADMDialogRef inDialog);
	
	void ASAPI (*Activate)(ADMDialogRef inDialog, ASBoolean inActivate);
	ASBoolean ASAPI (*IsActive)(ADMDialogRef inDialog);
	
	// dialog bounds accessors
	
	void ASAPI (*SetLocalRect)(ADMDialogRef inDialog, const ASRect* inLocalRect);
	void ASAPI (*GetLocalRect)(ADMDialogRef inDialog, ASRect* outLocalRect);
	
	void ASAPI (*SetBoundsRect)(ADMDialogRef inDialog, const ASRect* inBoundsRect);
	void ASAPI (*GetBoundsRect)(ADMDialogRef inDialog, ASRect* outBoundsRect);
	
	void ASAPI (*Move)(ADMDialogRef inDialog, ASInt32 inHorizPosition, ASInt32 inVertPosition);
	void ASAPI (*Size)(ADMDialogRef inDialog, ASInt32 inWidth, ASInt32 inHeight);

	// coordinate system transformations
	
	void ASAPI (*LocalToScreenPoint)(ADMDialogRef inDialog, ASPoint* ioPoint);
	void ASAPI (*ScreenToLocalPoint)(ADMDialogRef inDialog, ASPoint* ioPoint);
	
	void ASAPI (*LocalToScreenRect)(ADMDialogRef inDialog, ASRect* ioRect);
	void ASAPI (*ScreenToLocalRect)(ADMDialogRef inDialog, ASRect* ioRect);

	// dialog redraw requests
	
	void ASAPI (*Invalidate)(ADMDialogRef inDialog);
	void ASAPI (*InvalidateRect)(ADMDialogRef inDialog, const ASRect* inInvalRect);
	void ASAPI (*Update)(ADMDialogRef inDialog);

	// cursor ID accessors

	ASBoolean ASAPI (*SetCursorID)(ADMDialogRef inDialog, SPPluginRef inPluginRef,
				ASInt32 inCursorID);

	void ASAPI (*GetCursorID)(ADMDialogRef inDialog, SPPluginRef* outPluginRef,
				ASInt32* outCursorID);
	
	// dialog text accessors

	void ASAPI (*SetFont)(ADMDialogRef inDialog, ADMFont inFont);
	ADMFont ASAPI (*GetFont)(ADMDialogRef inDialog);

	void ASAPI (*SetText)(ADMDialogRef inDialog, const char* inText);
	void ASAPI (*SetTextW)(ADMDialogRef inDialog, const ASUnicode* inText);

	void ASAPI (*GetText)(ADMDialogRef inDialog, char* outText, ASInt32 inMaxLength);
	void ASAPI (*GetTextW)(ADMDialogRef inDialog, ASUnicode* outText, ASInt32 inMaxLength);

	ASInt32 ASAPI (*GetTextLength)(ADMDialogRef inDialog);
	ASInt32 ASAPI (*GetTextLengthW)(ADMDialogRef inDialog);

	// dialog size constraints
	
	void ASAPI (*SetMinWidth)(ADMDialogRef inDialog, ASInt32 inWidth);
	ASInt32 ASAPI (*GetMinWidth)(ADMDialogRef inDialog);
	
	void ASAPI (*SetMinHeight)(ADMDialogRef inDialog, ASInt32 inHeight);
	ASInt32 ASAPI (*GetMinHeight)(ADMDialogRef inDialog);
	
	void ASAPI (*SetMaxWidth)(ADMDialogRef inDialog, ASInt32 inWidth);
	ASInt32 ASAPI (*GetMaxWidth)(ADMDialogRef inDialog);
	
	void ASAPI (*SetMaxHeight)(ADMDialogRef inDialog, ASInt32 inHeight);
	ASInt32 ASAPI (*GetMaxHeight)(ADMDialogRef inDialog);
	
	void ASAPI (*SetHorizontalIncrement)(ADMDialogRef inDialog, ASInt32 inIncrement);
	ASInt32 ASAPI (*GetHorizontalIncrement)(ADMDialogRef inDialog);

	void ASAPI (*SetVerticalIncrement)(ADMDialogRef inDialog, ASInt32 inIncrement);
	ASInt32 ASAPI (*GetVerticalIncrement)(ADMDialogRef inDialog);

	// item accessors

	ADMItemRef ASAPI (*GetItem)(ADMDialogRef inDialog, ASInt32 inItemID);
	
	ADMItemRef ASAPI (*CreateItem)(ADMDialogRef inDialog, ASInt32 inItemID,
				ADMItemType inItemType, const ASRect* inBoundsRect,
				ADMItemInitProc inInitProc, ADMUserData inUserData, ASInt32 inOptions);

	void ASAPI (*DestroyItem)(ADMDialogRef inDialog, ADMItemRef inItem);
	
	ADMItemRef ASAPI (*GetNextItem)(ADMDialogRef inDialog, ADMItemRef inItem);
	ADMItemRef ASAPI (*GetPreviousItem)(ADMDialogRef inDialog, ADMItemRef inItem);

	// default/cancel items

	void ASAPI (*SetDefaultItemID)(ADMDialogRef inDialog, ASInt32 inItemID);
	ASInt32 ASAPI (*GetDefaultItemID)(ADMDialogRef inDialog);

	void ASAPI (*SetCancelItemID)(ADMDialogRef inDialog, ASInt32 inItemID);
	ASInt32 ASAPI (*GetCancelItemID)(ADMDialogRef inDialog);

	ASBoolean ASAPI (*EndModal)(ADMDialogRef inDialog, ASInt32 inModalResultID,
				ASBoolean inCancelling);
	
	// custom item types
	
	ASErr ASAPI (*RegisterItemType)(SPPluginRef inPluginRef, ADMItemType inItemType);
	ASErr ASAPI (*UnregisterItemType)(SPPluginRef inPluginRef, ADMItemType inItemType);

	// tooltips

	void ASAPI (*LoadToolTips)(ADMDialogRef inDialog, ASInt32 inStringID);

	// context help

	void ASAPI (*SetHelpID)(ADMDialogRef inDialog, ASHelpID inHelpID);
	ASHelpID ASAPI (*GetHelpID)(ADMDialogRef inDialog);
	void ASAPI (*Help)(ADMDialogRef inDialog);

	// dialog state accessors	

	ASBoolean ASAPI (*IsCollapsed)(ADMDialogRef inDialog);

	ADMItemRef ASAPI (*CreateGroupInDialog)(ADMDialogRef inDialog,
				SPPluginRef inPluginRef, const char* inName, ASInt32 inDialogID,
				ADMItemInitProc inInitProc, ADMUserData inUserData, ASInt32 inOptions);

	void ASAPI (*SetWindowRef)(ADMDialogRef inDialog, ASWindowRef inWindowRef);
	void ASAPI (*SetDialogName)(ADMDialogRef inDialog, const char* inName);

	ASBoolean ASAPI (*IsDialogContextHidden)(ADMDialogRef inDialog);

	void ASAPI (*SetUpdateEnabled)(ADMDialogRef inDialog, ASBoolean inEnableUpdate);
	ASBoolean ASAPI (*IsUpdateEnabled)(ADMDialogRef inDialog);
	
	void ASAPI (*SetForcedOnScreen)(ADMDialogRef ioDialog,
				ASBoolean inForcedOnScreen);
	ASBoolean ASAPI (*IsForcedOnScreen)(ADMDialogRef inDialog);

	ASInt32 ASAPI (*DisplayAsPopupModal)(ADMDialogRef inDialog);
	
	//Use this function to reposition the tab order of the item in the dialog. Note the item should 
	//be already created.
	ASErr ASAPI (*AdjustItemTabOrder)(ADMDialogRef inDialog, ADMItemRef inItem, ASInt32 position);

}
ADMDialogSuite8;


// -----------------------------------------------------------------------------
//	Old versions of suites

#ifndef __ADMDialogOld__
#ifdef MAC_ENV
#include "ADMDialogOld.h"
#else
#include ".\Legacy\ADMDialogOld.h"
#endif
#endif

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------

#endif	// __ADMDialog__
