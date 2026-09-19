/***********************************************************************/
/*                                                                     */
/* ADMDialogOld.h                                                      */
/* Old versions of ADM Dialog Suite                                    */
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

#ifndef __ADMDialogOld__
#define __ADMDialogOld__

#ifndef __ADMDialog__
#include "admDialog.h"
#endif


// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


// =============================================================================
//		* ADM Dialog Suite, version 2
// =============================================================================

#define kADMDialogSuiteVersion2 2

// -----------------------------------------------------------------------------

typedef struct ADMDialogSuite2
{

	// dialog creation

	ADMDialogRef ASAPI (*Create)(SPPluginRef inPluginRef, const char* inName,
				ASInt32 inDialogID, ADMDialogStyle inDialogStyle,
				ADMDialogInitProc inInitProc, ADMUserData inUserData);

	// modal dialogs

	ASInt32 ASAPI (*Modal)(SPPluginRef inPluginRef, ASInt32 inDialogID,
				ADMDialogStyle inDialogStyle, ADMDialogInitProc inInitProc,
				ADMUserData inUserData);

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

	void ASAPI (*SetNotifierData)(ADMDialogRef inDialog, ADMUserData l);
	ADMUserData ASAPI (*GetNotifierData)(ADMDialogRef inDialog);

	// dialog destroy proc

	void ASAPI (*SetDestroyProc)(ADMDialogRef inDialog, ADMDialogDestroyProc inDestroyProc);
	ADMDialogDestroyProc ASAPI (*GetDestroyProc)(ADMDialogRef inDialog);
	
	// dialog timer

	ADMTimerRef ASAPI (*CreateTimer)(ADMDialogRef inDialog, ASUInt32 inMilliseconds,
				ADMActionMask inAbortMask, ADMDialogTimerProc inTimerProc,
				ADMDialogTimerAbortProc inAbortProc);
	
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
	void ASAPI (*GetText)(ADMDialogRef inDialog, char* outText, ASInt32 inMaxLength);
	ASInt32 ASAPI (*GetTextLength)(ADMDialogRef inDialog);

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
				ADMItemInitProc inInitProc, ADMUserData inUserData);

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
	
}
ADMDialogSuite2;


// =============================================================================
//		* ADM Dialog Suite, version 3
// =============================================================================

#define kADMDialogSuiteVersion3 3
#define kADMDialogSuiteVersion kADMDialogSuiteVersion3

// -----------------------------------------------------------------------------

typedef struct ADMDialogSuite3
{

	// dialog creation

	ADMDialogRef ASAPI (*Create)(SPPluginRef inPluginRef, const char* inName,
				ASInt32 inDialogID, ADMDialogStyle inDialogStyle,
				ADMDialogInitProc inInitProc, ADMUserData inUserData);

	// modal dialogs

	ASInt32 ASAPI (*Modal)(SPPluginRef inPluginRef, const char* inName,
				ASInt32 inDialogID, ADMDialogStyle inDialogStyle,
				ADMDialogInitProc inInitProc, ADMUserData inUserData);

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
				ADMDialogTimerAbortProc inAbortProc);
	
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
	void ASAPI (*GetText)(ADMDialogRef inDialog, char* outText, ASInt32 inMaxLength);
	ASInt32 ASAPI (*GetTextLength)(ADMDialogRef inDialog);
	
	// dialog size constriants
	
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
				ADMItemInitProc inInitProc, ADMUserData inUserData);

	void ASAPI (*DestroyItem)(ADMDialogRef inDialog, ADMItemRef inItem);
	
	ADMItemRef ASAPI (*GetNextItem)(ADMDialogRef inDialog, ADMItemRef inItem);
	ADMItemRef ASAPI (*GetPreviousItem)(ADMDialogRef inDialog, ADMItemRef inItem);

	// default/cancel items

	void ASAPI (*SetDefaultItemID)(ADMDialogRef inDialog, ASInt32 inItemID);
	ASInt32 ASAPI (*GetDefaultItemID)(ADMDialogRef inDialog);

	void ASAPI (*SetCancelItemID)(ADMDialogRef inDialog, ASInt32 inItemID);
	ASInt32 ASAPI (*GetCancelItemID)(ADMDialogRef inDialog);
	
	ASBoolean  ASAPI (*EndModal)(ADMDialogRef inDialog, ASInt32 inModalResultID,
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
	
}
ADMDialogSuite3;

typedef ADMDialogSuite3 ADMDialogSuite; 


// =============================================================================
//		* ADM Dialog Suite, version 4
// =============================================================================

#define kADMDialogSuiteVersion4 4

// -----------------------------------------------------------------------------

typedef struct ADMDialogSuite4
{

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
	void ASAPI (*GetText)(ADMDialogRef inDialog, char* outText, ASInt32 inMaxLength);
	ASInt32 ASAPI (*GetTextLength)(ADMDialogRef inDialog);
	
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
	
	ASBoolean  ASAPI (*EndModal)(ADMDialogRef inDialog, ASInt32 inModalResultID,
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

	ADMItemRef ASAPI (*CreateGroupInDialog)(ADMDialogRef inDialog, SPPluginRef inPluginRef,
				const char* inName, ASInt32 inDialogID, ADMItemInitProc inInitProc,
				ADMUserData inUserData, ASInt32 inOptions);

	void ASAPI (*SetWindowRef)(ADMDialogRef inDialog, ASWindowRef inWindowRef);
	
	ASAPI void (*SetDialogName)(ADMDialogRef inDialog, const char* inName);

}
ADMDialogSuite4;


// =============================================================================
//		* ADM Dialog Suite, version 5
// =============================================================================

#define kADMDialogSuiteVersion5 5

// -----------------------------------------------------------------------------

typedef struct ADMDialogSuite5
{

	// *** This suite is FROZEN.
	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.

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
	void ASAPI (*GetText)(ADMDialogRef inDialog, char* outText, ASInt32 inMaxLength);
	ASInt32 ASAPI (*GetTextLength)(ADMDialogRef inDialog);

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

}
ADMDialogSuite5;


// =============================================================================
//		* ADM Dialog Suite, version 6
// =============================================================================

#define kADMDialogSuiteVersion6 6

// -----------------------------------------------------------------------------

typedef struct ADMDialogSuite6
{

	// *** This suite is FROZEN.
	// *** It shipped with Illustrator 9.0.

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
	void ASAPI (*GetText)(ADMDialogRef inDialog, char* outText, ASInt32 inMaxLength);
	ASInt32 ASAPI (*GetTextLength)(ADMDialogRef inDialog);

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

}
ADMDialogSuite6;

// =============================================================================
//		* ADM Dialog Suite, version 7
// =============================================================================

#define kADMDialogSuiteVersion7 7

// -----------------------------------------------------------------------------
typedef struct ADMDialogSuite7
{

	// *** This suite is FROZEN for changes.
	// *** It shipped with Acrobat 5.0, After Effects 5.0

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

}
ADMDialogSuite7;



// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

#endif  // __ADMDialogOld__
