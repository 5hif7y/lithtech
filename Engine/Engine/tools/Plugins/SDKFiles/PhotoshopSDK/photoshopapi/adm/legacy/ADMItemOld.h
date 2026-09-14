/***********************************************************************/
/*                                                                     */
/* ADMItemOld.h                                                        */
/* Old versions of ADM Item Suite                                      */
/*                                                                     */
/* ADOBE SYSTEMS INCORPORATED                                          */
/* Copyright 1996-2002 Adobe Systems Incorporated                      */
/* All Rights Reserved                                                 */
/*                                                                     */
/* NOTICE:  Adobe permits you to use, modify, and distribute this file */
/* in accordance with the terms of the Adobe license agreement         */
/* accompanying it. If you have received this file from a source other */
/* than Adobe, then your use, modification, or distribution of it      */
/* requires the prior written permission of Adobe.                     */
/*                                                                     */
/***********************************************************************/

#ifndef __ADMItemOld__
#define __ADMItemOld__

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
//		* ADM Item Suite, version 3
// =============================================================================

#define kADMItemSuiteVersion3 3

// -----------------------------------------------------------------------------

typedef struct ADMItemSuite3
{

	// item creation/destruction

	ADMItemRef ASAPI (*Create)(ADMDialogRef inDialog, ASInt32 inItemID, ADMItemType inItemType,
				const ASRect* inBoundsRect, ADMItemInitProc inInitProc, ADMUserData inUserData);

	void ASAPI (*Destroy)(ADMItemRef inItem);
	
	// item draw proc
	
	void ASAPI (*SetDrawProc)(ADMItemRef inItem, ADMItemDrawProc inDrawProc);
	ADMItemDrawProc ASAPI (*GetDrawProc)(ADMItemRef inItem);
	void ASAPI (*DefaultDraw)(ADMItemRef inItem, ADMDrawerRef inDrawer);

	// item track proc
	
	void ASAPI (*SetTrackProc)(ADMItemRef inItem, ADMItemTrackProc inTrackProc);
	ADMItemTrackProc ASAPI (*GetTrackProc)(ADMItemRef inItem);
	ASBoolean ASAPI (*DefaultTrack)(ADMItemRef inItem, ADMTrackerRef inTracker);

	void ASAPI (*SetMask)(ADMItemRef inItem, ADMActionMask inActionMask);
	ADMActionMask ASAPI (*GetMask)(ADMItemRef inItem);

	// item notify proc

	void ASAPI (*SetNotifyProc)(ADMItemRef inItem, ADMItemNotifyProc inNotifyProc);
	ADMItemNotifyProc ASAPI (*GetNotifyProc)(ADMItemRef inItem);
	void ASAPI (*DefaultNotify)(ADMItemRef inItem, ADMNotifierRef inNotifier);
	void ASAPI (*SendNotify)(ADMItemRef inItem, const char* inNotifierType);

	void ASAPI (*SetNotifierData)(ADMItemRef inItem, ADMUserData inNotifierData);
	ADMUserData ASAPI (*GetNotifierData)(ADMItemRef inItem);

	// item destroy proc

	void ASAPI (*SetDestroyProc)(ADMItemRef inItem, ADMItemDestroyProc inDestroyProc);
	ADMItemDestroyProc ASAPI (*GetDestroyProc)(ADMItemRef inItem);
	
	// item timer
	
	ADMTimerRef ASAPI (*CreateTimer)(ADMItemRef inItem, ASUInt32 inMilliseconds,
				ADMActionMask inAbortMask, ADMItemTimerProc inTimerProc,
				ADMItemTimerAbortProc inTimerAbortProc);

	void ASAPI (*AbortTimer)(ADMItemRef inItem, ADMTimerRef inTimer);

	// item user data

	void ASAPI (*SetUserData)(ADMItemRef inItem, ADMUserData inUserData);
	ADMUserData ASAPI (*GetUserData)(ADMItemRef inItem);

	// item information accessors

	ASInt32 ASAPI (*GetID)(ADMItemRef inItem);
	ADMDialogRef ASAPI (*GetDialog)(ADMItemRef inItem);
	ASWindowRef ASAPI (*GetWindowRef)(ADMItemRef inItem);
	ADMListRef ASAPI (*GetList)(ADMItemRef inItem);
	ADMItemRef ASAPI (*GetChildItem)(ADMItemRef inItem, ASInt32 inChildID);	

	void ASAPI (*SetPluginRef)(ADMItemRef inItem, SPPluginRef inPluginRef);
	SPPluginRef ASAPI (*GetPluginRef)(ADMItemRef inItem);

	void ASAPI (*SetItemType)(ADMItemRef inItem, ADMItemType inItemType);
	ADMItemType ASAPI (*GetItemType)(ADMItemRef inItem);
	
	void ASAPI (*SetItemStyle)(ADMItemRef inItem, ADMItemStyle inItemStyle);
	ADMItemStyle ASAPI (*GetItemStyle)(ADMItemRef inItem);
	
	// item state accessors
	
	void ASAPI (*Show)(ADMItemRef inItem, ASBoolean inShow);
	ASBoolean ASAPI (*IsVisible)(ADMItemRef inItem);
	
	void ASAPI (*Enable)(ADMItemRef inItem, ASBoolean inEnable);
	ASBoolean ASAPI (*IsEnabled)(ADMItemRef inItem);
	
	void ASAPI (*Activate)(ADMItemRef inItem, ASBoolean inActivate);
	ASBoolean ASAPI (*IsActive)(ADMItemRef inItem);
	
	void ASAPI (*Known)(ADMItemRef inItem, ASBoolean inKnown);
	ASBoolean ASAPI (*IsKnown)(ADMItemRef inItem);
	
	// item bounds accessors

	void ASAPI (*SetLocalRect)(ADMItemRef inItem, const ASRect* inLocalRect);
	void ASAPI (*GetLocalRect)(ADMItemRef inItem, ASRect* outLocalRect);
	
	void ASAPI (*SetBoundsRect)(ADMItemRef inItem, const ASRect* inBoundsRect);
	void ASAPI (*GetBoundsRect)(ADMItemRef inItem, ASRect* outBoundsRect);
	
	void ASAPI (*GetBestSize)(ADMItemRef inItem, ASPoint* outBestSize);

	void ASAPI (*Move)(ADMItemRef inItem, ASInt32 inHorizPosition, ASInt32 inVertPosition);
	void ASAPI (*Size)(ADMItemRef inItem, ASInt32 inWidth, ASInt32 inHeight);

	// coordinate system transformations	

	void ASAPI (*LocalToScreenPoint)(ADMItemRef inItem, ASPoint* ioPoint);
	void ASAPI (*ScreenToLocalPoint)(ADMItemRef inItem, ASPoint* ioPoint);
	
	void ASAPI (*LocalToScreenRect)(ADMItemRef inItem, ASRect* ioRect);
	void ASAPI (*ScreenToLocalRect)(ADMItemRef inItem, ASRect* ioRect);
	
	// item display
	
	void ASAPI (*Invalidate)(ADMItemRef inItem);
	void ASAPI (*Update)(ADMItemRef inItem);
	
	// cursor ID accessors
	
	ASBoolean ASAPI (*SetCursorID)(ADMItemRef inItem, SPPluginRef inPluginRef, ASInt32 inCursorID);
	void ASAPI (*GetCursorID)(ADMItemRef inItem, SPPluginRef* outPluginRef, ASInt32* outCursorID);

	// picture ID accessors

	void ASAPI (*SetPictureID)(ADMItemRef inItem, ASInt32 inPictureResID);
	ASInt32 ASAPI (*GetPictureID)(ADMItemRef inItem);
	
	void ASAPI (*SetSelectedPictureID)(ADMItemRef inItem, ASInt32 inPictureResID);
	ASInt32 ASAPI (*GetSelectedPictureID)(ADMItemRef inItem);
	
	void ASAPI (*SetDisabledPictureID)(ADMItemRef inItem, ASInt32 inPictureResID);
	ASInt32 ASAPI (*GetDisabledPictureID)(ADMItemRef inItem);

	// tooltips

	void ASAPI (*SetTipString)(ADMItemRef inItem, const char* inTipString);
	void ASAPI (*GetTipString)(ADMItemRef inItem, char* outTipString, ASInt32 inMaxLength);
	
	ASInt32 ASAPI (*GetTipStringLength)(ADMItemRef inItem);
	
	void ASAPI (*EnableTip)(ADMItemRef inItem, ASBoolean inEnable);
	ASBoolean ASAPI (*IsTipEnabled)(ADMItemRef inItem);
	
	void ASAPI (*ShowToolTip)(ADMItemRef inItem, const ASPoint* inWhere);
	void ASAPI (*HideToolTip)(ADMItemRef inItem);
	
	// item text accessors

	void ASAPI (*SetFont)(ADMItemRef inItem, ADMFont inFontID);
	ADMFont ASAPI (*GetFont)(ADMItemRef inItem);

	void ASAPI (*SetText)(ADMItemRef inItem, const char* inText);
	void ASAPI (*GetText)(ADMItemRef inItem, char* outText, ASInt32 inMaxLength);
	ASInt32 ASAPI (*GetTextLength)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxTextLength)(ADMItemRef inItem, ASInt32 inLength);
	ASInt32 ASAPI (*GetMaxTextLength)(ADMItemRef inItem);
	
	void ASAPI (*SelectAll)(ADMItemRef inItem);
	void ASAPI (*SetSelectionRange)(ADMItemRef inItem, ASInt32 inSelStart, ASInt32 inSelEnd);
	void ASAPI (*GetSelectionRange)(ADMItemRef inItem, ASInt32* outSelStart, ASInt32* outSelEnd);

	void ASAPI (*SetJustify)(ADMItemRef inItem, ADMJustify inJustify);
	ADMJustify ASAPI (*GetJustify)(ADMItemRef inItem);
	
	void ASAPI (*SetUnits)(ADMItemRef inItem, ADMUnits inUnits);
	ADMUnits ASAPI (*GetUnits)(ADMItemRef inItem);
	
	// item value accessors

	void ASAPI (*SetPrecision)(ADMItemRef inItem, ASInt32 inNumberOfDecimalPlaces);
	ASInt32 ASAPI (*GetPrecision)(ADMItemRef inItem);
	
	void ASAPI (*SetBooleanValue)(ADMItemRef inItem, ASBoolean inValue);
	ASBoolean ASAPI (*GetBooleanValue)(ADMItemRef inItem);
	
	void ASAPI (*SetIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetIntValue)(ADMItemRef inItem);
		
	void ASAPI (*SetFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMinIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetMinIntValue)(ADMItemRef inItem);
		
	void ASAPI (*SetMinFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetMinFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetMaxIntValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMinFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetMinFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetMaxFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetMaxFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetSmallIncrement)(ADMItemRef inItem, float inIncrement);	
	float ASAPI (*GetSmallIncrement)(ADMItemRef inItem);
	
	void ASAPI (*SetLargeIncrement)(ADMItemRef inItem, float inIncrement);
	float ASAPI (*GetLargeIncrement)(ADMItemRef inItem);
	
	void ASAPI (*SetAllowUnits)(ADMItemRef inItem, ASBoolean inAllowUnits);
	ASBoolean ASAPI (*GetAllowUnits)(ADMItemRef inItem);

	void ASAPI (*SetAllowMath)(ADMItemRef inItem, ASBoolean inAllowMath);
	ASBoolean ASAPI (*GetAllowMath)(ADMItemRef inItem);
	
	void ASAPI (*SetFloatToTextProc)(ADMItemRef inItem, ADMItemFloatToTextProc inProc);
	ADMItemFloatToTextProc ASAPI (*GetFloatToTextProc)(ADMItemRef inItem);
	ASBoolean ASAPI (*DefaultFloatToText)(ADMItemRef inItem, float inValue, char* outText,
				ASInt32 inMaxLength);

	void ASAPI (*SetTextToFloatProc)(ADMItemRef inItem, ADMItemTextToFloatProc inProc);
	ADMItemTextToFloatProc ASAPI (*GetTextToFloatProc)(ADMItemRef inItem);
	ASBoolean ASAPI (*DefaultTextToFloat)(ADMItemRef inItem, const char* inText, float* outValue);

	void ASAPI (*ShowUnits)(ADMItemRef inItem, ASBoolean inShowUnits);
	ASBoolean ASAPI (*GetShowUnits)(ADMItemRef inItem);

	ASBoolean ASAPI (*WasPercentageChange)(ADMItemRef inItem);

	// item help ID
	
	void ASAPI (*SetHelpID)(ADMItemRef inItem, ASHelpID inHelpID);
	ASHelpID ASAPI (*GetHelpID)(ADMItemRef inItem);
	void ASAPI (*Help)(ADMItemRef inDialog);

}
ADMItemSuite3;


// =============================================================================
//		* ADM Item Suite, version 4
// =============================================================================

#define kADMItemSuiteVersion4 4
#define kADMItemSuiteVersion kADMItemSuiteVersion4

// Begin AI9 #203277 -------------------------------------------------------

// FIX_ME (jreid 12/1/99) -- The following typedef is NOT part of the frozen
// ADMItemSuite4; it is being shoehorned in temporarily and WILL BE REMOVED.

typedef void ASAPI (*ADMPreDropDownProc)(ADMItemRef ioItem);

// End AI9 #203277 ---------------------------------------------------------

// -----------------------------------------------------------------------------

typedef struct ADMItemSuite4
{

	// item creation/destruction

	ADMItemRef ASAPI (*Create)(ADMDialogRef inDialog, ASInt32 inItemID, ADMItemType inItemType,
				const ASRect* inBoundsRect, ADMItemInitProc inInitProc, ADMUserData inUserData);

	void ASAPI (*Destroy)(ADMItemRef inItem);
	
	// item draw proc
	
	void ASAPI (*SetDrawProc)(ADMItemRef inItem, ADMItemDrawProc inDrawProc);
	ADMItemDrawProc ASAPI (*GetDrawProc)(ADMItemRef inItem);
	void ASAPI (*DefaultDraw)(ADMItemRef inItem, ADMDrawerRef inDrawer);

	// item track proc
	
	void ASAPI (*SetTrackProc)(ADMItemRef inItem, ADMItemTrackProc inTrackProc);
	ADMItemTrackProc ASAPI (*GetTrackProc)(ADMItemRef inItem);
	ASBoolean ASAPI (*DefaultTrack)(ADMItemRef inItem, ADMTrackerRef inTracker);

	void ASAPI (*SetMask)(ADMItemRef inItem, ADMActionMask inActionMask);
	ADMActionMask ASAPI (*GetMask)(ADMItemRef inItem);

	// item notify proc

	void ASAPI (*SetNotifyProc)(ADMItemRef inItem, ADMItemNotifyProc inNotifyProc);
	ADMItemNotifyProc ASAPI (*GetNotifyProc)(ADMItemRef inItem);
	void ASAPI (*DefaultNotify)(ADMItemRef inItem, ADMNotifierRef inNotifier);
	void ASAPI (*SendNotify)(ADMItemRef inItem, const char* inNotifierType);

	void ASAPI (*SetNotifierData)(ADMItemRef inItem, ADMUserData inNotifierData);
	ADMUserData ASAPI (*GetNotifierData)(ADMItemRef inItem);

	// item destroy proc

	void ASAPI (*SetDestroyProc)(ADMItemRef inItem, ADMItemDestroyProc inDestroyProc);
	ADMItemDestroyProc ASAPI (*GetDestroyProc)(ADMItemRef inItem);
	
	// item timer
	
	ADMTimerRef ASAPI (*CreateTimer)(ADMItemRef inItem, ASUInt32 inMilliseconds,
				ADMActionMask inAbortMask, ADMItemTimerProc inTimerProc,
				ADMItemTimerAbortProc inTimerAbortProc);

	void ASAPI (*AbortTimer)(ADMItemRef inItem, ADMTimerRef inTimer);

	// item user data

	void ASAPI (*SetUserData)(ADMItemRef inItem, ADMUserData inUserData);
	ADMUserData ASAPI (*GetUserData)(ADMItemRef inItem);

	// item information accessors

	ASInt32 ASAPI (*GetID)(ADMItemRef inItem);
	ADMDialogRef ASAPI (*GetDialog)(ADMItemRef inItem);
	ASWindowRef ASAPI (*GetWindowRef)(ADMItemRef inItem);
	ADMListRef ASAPI (*GetList)(ADMItemRef inItem);
	ADMItemRef ASAPI (*GetChildItem)(ADMItemRef inItem, ASInt32 inChildID);	

	void ASAPI (*SetPluginRef)(ADMItemRef inItem, SPPluginRef inPluginRef);
	SPPluginRef ASAPI (*GetPluginRef)(ADMItemRef inItem);

	void ASAPI (*SetItemType)(ADMItemRef inItem, ADMItemType inItemType);
	ADMItemType ASAPI (*GetItemType)(ADMItemRef inItem);
	
	void ASAPI (*SetItemStyle)(ADMItemRef inItem, ADMItemStyle inItemStyle);
	ADMItemStyle ASAPI (*GetItemStyle)(ADMItemRef inItem);
	
	// item state accessors
	
	void ASAPI (*Show)(ADMItemRef inItem, ASBoolean inShow);
	ASBoolean ASAPI (*IsVisible)(ADMItemRef inItem);
	
	void ASAPI (*Enable)(ADMItemRef inItem, ASBoolean inEnable);
	ASBoolean ASAPI (*IsEnabled)(ADMItemRef inItem);
	
	void ASAPI (*Activate)(ADMItemRef inItem, ASBoolean inActivate);
	ASBoolean ASAPI (*IsActive)(ADMItemRef inItem);
	
	void ASAPI (*Known)(ADMItemRef inItem, ASBoolean inKnown);
	ASBoolean ASAPI (*IsKnown)(ADMItemRef inItem);
	
	// item bounds accessors

	void ASAPI (*SetLocalRect)(ADMItemRef inItem, const ASRect* inLocalRect);
	void ASAPI (*GetLocalRect)(ADMItemRef inItem, ASRect* outLocalRect);
	
	void ASAPI (*SetBoundsRect)(ADMItemRef inItem, const ASRect* inBoundsRect);
	void ASAPI (*GetBoundsRect)(ADMItemRef inItem, ASRect* outBoundsRect);
	
	void ASAPI (*GetBestSize)(ADMItemRef inItem, ASPoint* outBestSize);

	void ASAPI (*Move)(ADMItemRef inItem, ASInt32 inHorizPosition, ASInt32 inVertPosition);
	void ASAPI (*Size)(ADMItemRef inItem, ASInt32 inWidth, ASInt32 inHeight);

	// coordinate system transformations	

	void ASAPI (*LocalToScreenPoint)(ADMItemRef inItem, ASPoint* ioPoint);
	void ASAPI (*ScreenToLocalPoint)(ADMItemRef inItem, ASPoint* ioPoint);
	
	void ASAPI (*LocalToScreenRect)(ADMItemRef inItem, ASRect* ioRect);
	void ASAPI (*ScreenToLocalRect)(ADMItemRef inItem, ASRect* ioRect);
	
	// item display
	
	void ASAPI (*Invalidate)(ADMItemRef inItem);
	void ASAPI (*InvalidateRect)(ADMItemRef inDialog, const ASRect* inInvalRect);
	void ASAPI (*Update)(ADMItemRef inItem);
	
	// cursor ID accessors
	
	ASBoolean ASAPI (*SetCursorID)(ADMItemRef inItem, SPPluginRef inPluginRef, ASInt32 inCursorID);
	void ASAPI (*GetCursorID)(ADMItemRef inItem, SPPluginRef* outPluginRef, ASInt32* outCursorID);

	// picture ID accessors

	void ASAPI (*SetPictureID)(ADMItemRef inItem, ASInt32 inPictureResID);
	ASInt32 ASAPI (*GetPictureID)(ADMItemRef inItem);
	
	void ASAPI (*SetSelectedPictureID)(ADMItemRef inItem, ASInt32 inPictureResID);
	ASInt32 ASAPI (*GetSelectedPictureID)(ADMItemRef inItem);
	
	void ASAPI (*SetDisabledPictureID)(ADMItemRef inItem, ASInt32 inPictureResID);
	ASInt32 ASAPI (*GetDisabledPictureID)(ADMItemRef inItem);

	// tooltips

	void ASAPI (*SetTipString)(ADMItemRef inItem, const char* inTipString);
	void ASAPI (*GetTipString)(ADMItemRef inItem, char* outTipString, ASInt32 inMaxLength);
	
	ASInt32 ASAPI (*GetTipStringLength)(ADMItemRef inItem);
	
	void ASAPI (*EnableTip)(ADMItemRef inItem, ASBoolean inEnable);
	ASBoolean ASAPI (*IsTipEnabled)(ADMItemRef inItem);
	
	void ASAPI (*ShowToolTip)(ADMItemRef inItem, const ASPoint* inWhere);
	void ASAPI (*HideToolTip)(ADMItemRef inItem);
	
	// item text accessors

	void ASAPI (*SetFont)(ADMItemRef inItem, ADMFont inFontID);
	ADMFont ASAPI (*GetFont)(ADMItemRef inItem);

	void ASAPI (*SetText)(ADMItemRef inItem, const char* inText);
	void ASAPI (*GetText)(ADMItemRef inItem, char* outText, ASInt32 inMaxLength);
	ASInt32 ASAPI (*GetTextLength)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxTextLength)(ADMItemRef inItem, ASInt32 inLength);
	ASInt32 ASAPI (*GetMaxTextLength)(ADMItemRef inItem);
	
	void ASAPI (*SelectAll)(ADMItemRef inItem);
	void ASAPI (*SetSelectionRange)(ADMItemRef inItem, ASInt32 inSelStart, ASInt32 inSelEnd);
	void ASAPI (*GetSelectionRange)(ADMItemRef inItem, ASInt32* outSelStart, ASInt32* outSelEnd);

	void ASAPI (*SetJustify)(ADMItemRef inItem, ADMJustify inJustify);
	ADMJustify ASAPI (*GetJustify)(ADMItemRef inItem);
	
	void ASAPI (*SetUnits)(ADMItemRef inItem, ADMUnits inUnits);
	ADMUnits ASAPI (*GetUnits)(ADMItemRef inItem);
	
	// item value accessors

	void ASAPI (*SetPrecision)(ADMItemRef inItem, ASInt32 inNumberOfDecimalPlaces);
	ASInt32 ASAPI (*GetPrecision)(ADMItemRef inItem);
	
	void ASAPI (*SetBooleanValue)(ADMItemRef inItem, ASBoolean inValue);
	ASBoolean ASAPI (*GetBooleanValue)(ADMItemRef inItem);
	
	void ASAPI (*SetIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetIntValue)(ADMItemRef inItem);
		
	void ASAPI (*SetFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMinIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetMinIntValue)(ADMItemRef inItem);
		
	void ASAPI (*SetMinFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetMinFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetMaxIntValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMinFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetMinFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetMaxFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetMaxFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetSmallIncrement)(ADMItemRef inItem, float inIncrement);	
	float ASAPI (*GetSmallIncrement)(ADMItemRef inItem);
	
	void ASAPI (*SetLargeIncrement)(ADMItemRef inItem, float inIncrement);
	float ASAPI (*GetLargeIncrement)(ADMItemRef inItem);
	
	void ASAPI (*SetAllowUnits)(ADMItemRef inItem, ASBoolean inAllowUnits);
	ASBoolean ASAPI (*GetAllowUnits)(ADMItemRef inItem);

	void ASAPI (*SetAllowMath)(ADMItemRef inItem, ASBoolean inAllowMath);
	ASBoolean ASAPI (*GetAllowMath)(ADMItemRef inItem);
	
	void ASAPI (*SetFloatToTextProc)(ADMItemRef inItem, ADMItemFloatToTextProc inProc);
	ADMItemFloatToTextProc ASAPI (*GetFloatToTextProc)(ADMItemRef inItem);

	ASBoolean ASAPI (*DefaultFloatToText)(ADMItemRef inItem, float inValue, char* outText,
				ASInt32 inMaxLength);

	void ASAPI (*SetTextToFloatProc)(ADMItemRef inItem, ADMItemTextToFloatProc inProc);
	ADMItemTextToFloatProc ASAPI (*GetTextToFloatProc)(ADMItemRef inItem);
	ASBoolean ASAPI (*DefaultTextToFloat)(ADMItemRef inItem, const char* inText, float* outValue);

	void ASAPI (*ShowUnits)(ADMItemRef inItem, ASBoolean inShowUnits);
	ASBoolean ASAPI (*GetShowUnits)(ADMItemRef inItem);

	ASBoolean ASAPI (*WasPercentageChange)(ADMItemRef inItem);

	// item help ID
	
	void ASAPI (*SetHelpID)(ADMItemRef inItem, ASHelpID inHelpID);
	ASHelpID ASAPI (*GetHelpID)(ADMItemRef inItem);
	void ASAPI (*Help)(ADMItemRef inDialog);

	// hierarchy list accessor
	
	ADMHierarchyListRef ASAPI (*GetHierarchyList)(ADMItemRef inItem);

// Begin AI9 #203277 -------------------------------------------------------

	// Do not use the following function! It was NOT part of the frozen
	// ADMItemSuite4; it is being shoehorned in temporarily and WILL BE REMOVED.

	void ASAPI (*SetPreDropDownProc)(ADMItemRef ioItem,
			ADMPreDropDownProc inPreDropDownProc, ADMUserData inUserData);

// End AI9 #203277 ---------------------------------------------------------

}
ADMItemSuite4;

typedef ADMItemSuite4 ADMItemSuite;


// =============================================================================
//		* ADM Item Suite, version 5
// =============================================================================

#define kADMItemSuiteVersion5 5

// -----------------------------------------------------------------------------

typedef struct ADMItemSuite5
{

	// *** This suite is FROZEN.
	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.

	// item creation/destruction

	ADMItemRef ASAPI (*Create)(ADMDialogRef inDialog, ASInt32 inItemID,
				ADMItemType inItemType, const ASRect* inBoundsRect,
				ADMItemInitProc inInitProc, ADMUserData inUserData,
				ASInt32 inOptions);

	void ASAPI (*Destroy)(ADMItemRef inItem);
	
	// item draw proc
	
	void ASAPI (*SetDrawProc)(ADMItemRef inItem, ADMItemDrawProc inDrawProc);
	ADMItemDrawProc ASAPI (*GetDrawProc)(ADMItemRef inItem);
	void ASAPI (*DefaultDraw)(ADMItemRef inItem, ADMDrawerRef inDrawer);

	// item track proc
	
	void ASAPI (*SetTrackProc)(ADMItemRef inItem, ADMItemTrackProc inTrackProc);
	ADMItemTrackProc ASAPI (*GetTrackProc)(ADMItemRef inItem);
	ASBoolean ASAPI (*DefaultTrack)(ADMItemRef inItem, ADMTrackerRef inTracker);

	void ASAPI (*SetMask)(ADMItemRef inItem, ADMActionMask inActionMask);
	ADMActionMask ASAPI (*GetMask)(ADMItemRef inItem);

	// item notify proc

	void ASAPI (*SetNotifyProc)(ADMItemRef inItem, ADMItemNotifyProc inNotifyProc);
	ADMItemNotifyProc ASAPI (*GetNotifyProc)(ADMItemRef inItem);
	void ASAPI (*DefaultNotify)(ADMItemRef inItem, ADMNotifierRef inNotifier);
	void ASAPI (*SendNotify)(ADMItemRef inItem, const char* inNotifierType);

	void ASAPI (*SetNotifierData)(ADMItemRef inItem, ADMUserData inNotifierData);
	ADMUserData ASAPI (*GetNotifierData)(ADMItemRef inItem);

	// item destroy proc

	void ASAPI (*SetDestroyProc)(ADMItemRef inItem, ADMItemDestroyProc inDestroyProc);
	ADMItemDestroyProc ASAPI (*GetDestroyProc)(ADMItemRef inItem);
	
	// item timer
	
	ADMTimerRef ASAPI (*CreateTimer)(ADMItemRef inItem, ASUInt32 inMilliseconds,
				ADMActionMask inAbortMask, ADMItemTimerProc inTimerProc,
				ADMItemTimerAbortProc inTimerAbortProc, ASInt32 inOptions);

	void ASAPI (*AbortTimer)(ADMItemRef inItem, ADMTimerRef inTimer);

	// item user data

	void ASAPI (*SetUserData)(ADMItemRef inItem, ADMUserData inUserData);
	ADMUserData ASAPI (*GetUserData)(ADMItemRef inItem);

	// item information accessors

	ASInt32 ASAPI (*GetID)(ADMItemRef inItem);
	ADMDialogRef ASAPI (*GetDialog)(ADMItemRef inItem);
	ASWindowRef ASAPI (*GetWindowRef)(ADMItemRef inItem);
	ADMListRef ASAPI (*GetList)(ADMItemRef inItem);
	ADMItemRef ASAPI (*GetChildItem)(ADMItemRef inItem, ASInt32 inChildID);	

	void ASAPI (*SetPluginRef)(ADMItemRef inItem, SPPluginRef inPluginRef);
	SPPluginRef ASAPI (*GetPluginRef)(ADMItemRef inItem);

	void ASAPI (*SetItemType)(ADMItemRef inItem, ADMItemType inItemType);
	ADMItemType ASAPI (*GetItemType)(ADMItemRef inItem);
	
	void ASAPI (*SetItemStyle)(ADMItemRef inItem, ADMItemStyle inItemStyle);
	ADMItemStyle ASAPI (*GetItemStyle)(ADMItemRef inItem);
	
	// item state accessors
	
	void ASAPI (*Show)(ADMItemRef inItem, ASBoolean inShow);
	ASBoolean ASAPI (*IsVisible)(ADMItemRef inItem);
	
	void ASAPI (*Enable)(ADMItemRef inItem, ASBoolean inEnable);
	ASBoolean ASAPI (*IsEnabled)(ADMItemRef inItem);
	
	void ASAPI (*Activate)(ADMItemRef inItem, ASBoolean inActivate);
	ASBoolean ASAPI (*IsActive)(ADMItemRef inItem);
	
	void ASAPI (*Known)(ADMItemRef inItem, ASBoolean inKnown);
	ASBoolean ASAPI (*IsKnown)(ADMItemRef inItem);
	
	// item bounds accessors

	void ASAPI (*SetLocalRect)(ADMItemRef inItem, const ASRect* inLocalRect);
	void ASAPI (*GetLocalRect)(ADMItemRef inItem, ASRect* outLocalRect);
	
	void ASAPI (*SetBoundsRect)(ADMItemRef inItem, const ASRect* inBoundsRect);
	void ASAPI (*GetBoundsRect)(ADMItemRef inItem, ASRect* outBoundsRect);
	
	void ASAPI (*GetBestSize)(ADMItemRef inItem, ASPoint* outBestSize);

	void ASAPI (*Move)(ADMItemRef inItem, ASInt32 inHorizPosition, ASInt32 inVertPosition);
	void ASAPI (*Size)(ADMItemRef inItem, ASInt32 inWidth, ASInt32 inHeight);

	// coordinate system transformations	

	void ASAPI (*LocalToScreenPoint)(ADMItemRef inItem, ASPoint* ioPoint);
	void ASAPI (*ScreenToLocalPoint)(ADMItemRef inItem, ASPoint* ioPoint);
	
	void ASAPI (*LocalToScreenRect)(ADMItemRef inItem, ASRect* ioRect);
	void ASAPI (*ScreenToLocalRect)(ADMItemRef inItem, ASRect* ioRect);
	
	// item display
	
	void ASAPI (*Invalidate)(ADMItemRef inItem);
	void ASAPI (*InvalidateRect)(ADMItemRef inDialog, const ASRect* inInvalRect);
	void ASAPI (*Update)(ADMItemRef inItem);
	
	// cursor ID accessors
	
	ASBoolean ASAPI (*SetCursorID)(ADMItemRef inItem, SPPluginRef inPluginRef, ASInt32 inCursorID,
				const char* inCursorName);

	void ASAPI (*GetCursorID)(ADMItemRef inItem, SPPluginRef* outPluginRef, ASInt32* outCursorID,
				const char** outCursorName);

	// picture ID accessors

	void ASAPI (*SetPictureID)(ADMItemRef inItem, ASInt32 inPictureResID,
				const char* inPictureResName);

	void ASAPI (*GetPictureID)(ADMItemRef inItem, ASInt32* outPictureResID,
				const char** outPictureResName);
	
	void ASAPI (*SetSelectedPictureID)(ADMItemRef inItem, ASInt32 inPictureResID,
				const char* inPictureResName);

	void ASAPI (*GetSelectedPictureID)(ADMItemRef inItem, ASInt32* outPictureResID,
				const char** outPictureResName);
	
	void ASAPI (*SetDisabledPictureID)(ADMItemRef inItem, ASInt32 inPictureResID,
				const char* inPictureResName);

	void ASAPI (*GetDisabledPictureID)(ADMItemRef inItem, ASInt32* outPictureResID,
				const char** outPictureResName);

	// tooltips

	void ASAPI (*SetTipString)(ADMItemRef inItem, const char* inTipString);
	void ASAPI (*GetTipString)(ADMItemRef inItem, char* outTipString, ASInt32 inMaxLength);
	
	ASInt32 ASAPI (*GetTipStringLength)(ADMItemRef inItem);
	
	void ASAPI (*EnableTip)(ADMItemRef inItem, ASBoolean inEnable);
	ASBoolean ASAPI (*IsTipEnabled)(ADMItemRef inItem);
	
	void ASAPI (*ShowToolTip)(ADMItemRef inItem, const ASPoint* inWhere);
	void ASAPI (*HideToolTip)(ADMItemRef inItem);
	
	// item text accessors

	void ASAPI (*SetFont)(ADMItemRef inItem, ADMFont inFontID);
	ADMFont ASAPI (*GetFont)(ADMItemRef inItem);

	void ASAPI (*SetText)(ADMItemRef inItem, const char* inText);
	void ASAPI (*GetText)(ADMItemRef inItem, char* outText, ASInt32 inMaxLength);
	ASInt32 ASAPI (*GetTextLength)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxTextLength)(ADMItemRef inItem, ASInt32 inLength);
	ASInt32 ASAPI (*GetMaxTextLength)(ADMItemRef inItem);
	
	void ASAPI (*SelectAll)(ADMItemRef inItem);
	void ASAPI (*SetSelectionRange)(ADMItemRef inItem, ASInt32 inSelStart, ASInt32 inSelEnd);
	void ASAPI (*GetSelectionRange)(ADMItemRef inItem, ASInt32* outSelStart, ASInt32* outSelEnd);

	void ASAPI (*SetJustify)(ADMItemRef inItem, ADMJustify inJustify);
	ADMJustify ASAPI (*GetJustify)(ADMItemRef inItem);
	
	void ASAPI (*SetUnits)(ADMItemRef inItem, ADMUnits inUnits);
	ADMUnits ASAPI (*GetUnits)(ADMItemRef inItem);
	
	// item value accessors

	void ASAPI (*SetPrecision)(ADMItemRef inItem, ASInt32 inNumberOfDecimalPlaces);
	ASInt32 ASAPI (*GetPrecision)(ADMItemRef inItem);
	
	void ASAPI (*SetBooleanValue)(ADMItemRef inItem, ASBoolean inValue);
	ASBoolean ASAPI (*GetBooleanValue)(ADMItemRef inItem);
	
	void ASAPI (*SetIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetIntValue)(ADMItemRef inItem);
		
	void ASAPI (*SetFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMinIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetMinIntValue)(ADMItemRef inItem);
		
	void ASAPI (*SetMinFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetMinFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetMaxIntValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMinFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetMinFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetMaxFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetMaxFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetSmallIncrement)(ADMItemRef inItem, float inIncrement);	
	float ASAPI (*GetSmallIncrement)(ADMItemRef inItem);
	
	void ASAPI (*SetLargeIncrement)(ADMItemRef inItem, float inIncrement);
	float ASAPI (*GetLargeIncrement)(ADMItemRef inItem);
	
	void ASAPI (*SetAllowUnits)(ADMItemRef inItem, ASBoolean inAllowUnits);
	ASBoolean ASAPI (*GetAllowUnits)(ADMItemRef inItem);
	void ASAPI (*SetAllowMath)(ADMItemRef inItem, ASBoolean inAllowMath);
	ASBoolean ASAPI (*GetAllowMath)(ADMItemRef inItem);
	
	void ASAPI (*SetFloatToTextProc)(ADMItemRef inItem, ADMItemFloatToTextProc inProc);
	ADMItemFloatToTextProc ASAPI (*GetFloatToTextProc)(ADMItemRef inItem);
	
	ASBoolean ASAPI (*DefaultFloatToText)(ADMItemRef inItem, float inValue, char* outText,
		ASInt32 inMaxLength);

	void ASAPI (*SetTextToFloatProc)(ADMItemRef inItem, ADMItemTextToFloatProc inProc);
	ADMItemTextToFloatProc ASAPI (*GetTextToFloatProc)(ADMItemRef inItem);
	ASBoolean ASAPI (*DefaultTextToFloat)(ADMItemRef inItem, const char* inText, float* outValue);

	void ASAPI (*ShowUnits)(ADMItemRef inItem, ASBoolean inShowUnits);
	ASBoolean ASAPI (*GetShowUnits)(ADMItemRef inItem);

	ASBoolean ASAPI (*WasPercentageChange)(ADMItemRef inItem);

	// item help ID
	
	void ASAPI (*SetHelpID)(ADMItemRef inItem, ASHelpID inHelpID);
	ASHelpID ASAPI (*GetHelpID)(ADMItemRef inItem);
	void ASAPI (*Help)(ADMItemRef inDialog);

	// hierarchy list accessor
	
	ADMHierarchyListRef ASAPI (*GetHierarchyList)(ADMItemRef inItem);
	
	// item group accessors
	
 	void ASAPI (*AddItem)(ADMItemRef inGroup, ADMItemRef inItem);
 	void ASAPI (*RemoveItem)(ADMItemRef inGroup, ADMItemRef inItem);

	// item pictures (from icons)
	
	void ASAPI (*SetPicture)(ADMItemRef inItem, ADMIconRef inPicture);
	ADMIconRef ASAPI (*GetPicture)(ADMItemRef inItem);

	void ASAPI (*SetSelectedPicture)(ADMItemRef inItem, ADMIconRef inPicture);
	ADMIconRef ASAPI (*GetSelectedPicture)(ADMItemRef inItem);

	void ASAPI (*SetDisabledPicture)(ADMItemRef inItem, ADMIconRef inPicture);
	ADMIconRef ASAPI (*GetDisabledPicture)(ADMItemRef inItem);
	
	// popup control accessors
	
	void ASAPI (*SetPopupDialog)(ADMItemRef inItem, ASInt32 inPopupItemID, ADMDialogRef inDialog);
	ADMDialogRef ASAPI (*GetPopupDialog)(ADMItemRef inItem);

}
ADMItemSuite5;

// -----------------------------------------------------------------------------
// =============================================================================
//		* ADM Item Suite, version 6
// =============================================================================

#define kADMItemSuiteVersion6 6

typedef struct ADMItemSuite6
{

	// *** This suite is FROZEN.
	// *** It shipped with Acrobat 5.0.

	// item creation/destruction

	ADMItemRef ASAPI (*Create)(ADMDialogRef inDialog, ASInt32 inItemID,
				ADMItemType inItemType, const ASRect* inBoundsRect,
				ADMItemInitProc inInitProc, ADMUserData inUserData,
				ASInt32 inOptions);

	void ASAPI (*Destroy)(ADMItemRef inItem);
	
	// item draw proc
	
	void ASAPI (*SetDrawProc)(ADMItemRef inItem, ADMItemDrawProc inDrawProc);
	ADMItemDrawProc ASAPI (*GetDrawProc)(ADMItemRef inItem);
	void ASAPI (*DefaultDraw)(ADMItemRef inItem, ADMDrawerRef inDrawer);

	// item track proc
	
	void ASAPI (*SetTrackProc)(ADMItemRef inItem, ADMItemTrackProc inTrackProc);
	ADMItemTrackProc ASAPI (*GetTrackProc)(ADMItemRef inItem);
	ASBoolean ASAPI (*DefaultTrack)(ADMItemRef inItem, ADMTrackerRef inTracker);

	void ASAPI (*SetMask)(ADMItemRef inItem, ADMActionMask inActionMask);
	ADMActionMask ASAPI (*GetMask)(ADMItemRef inItem);

	// item notify proc

	void ASAPI (*SetNotifyProc)(ADMItemRef inItem, ADMItemNotifyProc inNotifyProc);
	ADMItemNotifyProc ASAPI (*GetNotifyProc)(ADMItemRef inItem);
	void ASAPI (*DefaultNotify)(ADMItemRef inItem, ADMNotifierRef inNotifier);
	void ASAPI (*SendNotify)(ADMItemRef inItem, const char* inNotifierType);

	void ASAPI (*SetNotifierData)(ADMItemRef inItem, ADMUserData inNotifierData);
	ADMUserData ASAPI (*GetNotifierData)(ADMItemRef inItem);

	// item destroy proc

	void ASAPI (*SetDestroyProc)(ADMItemRef inItem, ADMItemDestroyProc inDestroyProc);
	ADMItemDestroyProc ASAPI (*GetDestroyProc)(ADMItemRef inItem);
	
	// item timer
	
	ADMTimerRef ASAPI (*CreateTimer)(ADMItemRef inItem, ASUInt32 inMilliseconds,
				ADMActionMask inAbortMask, ADMItemTimerProc inTimerProc,
				ADMItemTimerAbortProc inTimerAbortProc, ASInt32 inOptions);

	void ASAPI (*AbortTimer)(ADMItemRef inItem, ADMTimerRef inTimer);

	// item user data

	void ASAPI (*SetUserData)(ADMItemRef inItem, ADMUserData inUserData);
	ADMUserData ASAPI (*GetUserData)(ADMItemRef inItem);

	// item information accessors

	ASInt32 ASAPI (*GetID)(ADMItemRef inItem);
	ADMDialogRef ASAPI (*GetDialog)(ADMItemRef inItem);
	ASWindowRef ASAPI (*GetWindowRef)(ADMItemRef inItem);
	ADMListRef ASAPI (*GetList)(ADMItemRef inItem);
	ADMItemRef ASAPI (*GetChildItem)(ADMItemRef inItem, ASInt32 inChildID);	

	void ASAPI (*SetPluginRef)(ADMItemRef inItem, SPPluginRef inPluginRef);
	SPPluginRef ASAPI (*GetPluginRef)(ADMItemRef inItem);

	void ASAPI (*SetItemType)(ADMItemRef inItem, ADMItemType inItemType);
	ADMItemType ASAPI (*GetItemType)(ADMItemRef inItem);
	
	void ASAPI (*SetItemStyle)(ADMItemRef inItem, ADMItemStyle inItemStyle);
	ADMItemStyle ASAPI (*GetItemStyle)(ADMItemRef inItem);
	
	// item state accessors
	
	void ASAPI (*Show)(ADMItemRef inItem, ASBoolean inShow);
	ASBoolean ASAPI (*IsVisible)(ADMItemRef inItem);
	
	void ASAPI (*Enable)(ADMItemRef inItem, ASBoolean inEnable);
	ASBoolean ASAPI (*IsEnabled)(ADMItemRef inItem);
	
	void ASAPI (*Activate)(ADMItemRef inItem, ASBoolean inActivate);
	ASBoolean ASAPI (*IsActive)(ADMItemRef inItem);
	
	void ASAPI (*Known)(ADMItemRef inItem, ASBoolean inKnown);
	ASBoolean ASAPI (*IsKnown)(ADMItemRef inItem);
	
	// item bounds accessors

	void ASAPI (*SetLocalRect)(ADMItemRef inItem, const ASRect* inLocalRect);
	void ASAPI (*GetLocalRect)(ADMItemRef inItem, ASRect* outLocalRect);
	
	void ASAPI (*SetBoundsRect)(ADMItemRef inItem, const ASRect* inBoundsRect);
	void ASAPI (*GetBoundsRect)(ADMItemRef inItem, ASRect* outBoundsRect);
	
	void ASAPI (*GetBestSize)(ADMItemRef inItem, ASPoint* outBestSize);

	void ASAPI (*Move)(ADMItemRef inItem, ASInt32 inHorizPosition, ASInt32 inVertPosition);
	void ASAPI (*Size)(ADMItemRef inItem, ASInt32 inWidth, ASInt32 inHeight);

	// coordinate system transformations	

	void ASAPI (*LocalToScreenPoint)(ADMItemRef inItem, ASPoint* ioPoint);
	void ASAPI (*ScreenToLocalPoint)(ADMItemRef inItem, ASPoint* ioPoint);
	
	void ASAPI (*LocalToScreenRect)(ADMItemRef inItem, ASRect* ioRect);
	void ASAPI (*ScreenToLocalRect)(ADMItemRef inItem, ASRect* ioRect);
	
	// item display
	
	void ASAPI (*Invalidate)(ADMItemRef inItem);
	void ASAPI (*InvalidateRect)(ADMItemRef inDialog, const ASRect* inInvalRect);
	void ASAPI (*Update)(ADMItemRef inItem);
	
	// cursor ID accessors
	
	ASBoolean ASAPI (*SetCursorID)(ADMItemRef inItem, SPPluginRef inPluginRef, ASInt32 inCursorID,
				const char* inCursorName);

	void ASAPI (*GetCursorID)(ADMItemRef inItem, SPPluginRef* outPluginRef, ASInt32* outCursorID,
				const char** outCursorName);

	// picture ID accessors

	void ASAPI (*SetPictureID)(ADMItemRef inItem, ASInt32 inPictureResID,
				const char* inPictureResName);

	void ASAPI (*GetPictureID)(ADMItemRef inItem, ASInt32* outPictureResID,
				const char** outPictureResName);
	
	void ASAPI (*SetSelectedPictureID)(ADMItemRef inItem, ASInt32 inPictureResID,
				const char* inPictureResName);

	void ASAPI (*GetSelectedPictureID)(ADMItemRef inItem, ASInt32* outPictureResID,
				const char** outPictureResName);
	
	void ASAPI (*SetDisabledPictureID)(ADMItemRef inItem, ASInt32 inPictureResID,
				const char* inPictureResName);

	void ASAPI (*GetDisabledPictureID)(ADMItemRef inItem, ASInt32* outPictureResID,
				const char** outPictureResName);

	// tooltips

	void ASAPI (*SetTipString)(ADMItemRef inItem, const char* inTipString);
	void ASAPI (*SetTipStringW)(ADMItemRef inItem, const ASUnicode* inTipString);

	void ASAPI (*GetTipString)(ADMItemRef inItem, char* outTipString, ASInt32 inMaxLength);
	void ASAPI (*GetTipStringW)(ADMItemRef inItem, ASUnicode* outTipString, ASInt32 inMaxLength);
	
	ASInt32 ASAPI (*GetTipStringLength)(ADMItemRef inItem);
	ASInt32 ASAPI (*GetTipStringLengthW)(ADMItemRef inItem);
	
	void ASAPI (*EnableTip)(ADMItemRef inItem, ASBoolean inEnable);
	ASBoolean ASAPI (*IsTipEnabled)(ADMItemRef inItem);
	
	void ASAPI (*ShowToolTip)(ADMItemRef inItem, const ASPoint* inWhere);
	void ASAPI (*HideToolTip)(ADMItemRef inItem);
	
	// item text accessors

	void ASAPI (*SetFont)(ADMItemRef inItem, ADMFont inFontID);
	ADMFont ASAPI (*GetFont)(ADMItemRef inItem);

	void ASAPI (*SetText)(ADMItemRef inItem, const char* inText);
	void ASAPI (*SetTextW)(ADMItemRef inItem, const ASUnicode* inText);

	void ASAPI (*GetText)(ADMItemRef inItem, char* outText, ASInt32 inMaxLength);
	void ASAPI (*GetTextW)(ADMItemRef inItem, ASUnicode* outText, ASInt32 inMaxLength);

	ASInt32 ASAPI (*GetTextLength)(ADMItemRef inItem);
	ASInt32 ASAPI (*GetTextLengthW)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxTextLength)(ADMItemRef inItem, ASInt32 inLength);
	ASInt32 ASAPI (*GetMaxTextLength)(ADMItemRef inItem);
	
	void ASAPI (*SelectAll)(ADMItemRef inItem);
	void ASAPI (*SetSelectionRange)(ADMItemRef inItem, ASInt32 inSelStart, ASInt32 inSelEnd);
	void ASAPI (*GetSelectionRange)(ADMItemRef inItem, ASInt32* outSelStart, ASInt32* outSelEnd);

	void ASAPI (*SetJustify)(ADMItemRef inItem, ADMJustify inJustify);
	ADMJustify ASAPI (*GetJustify)(ADMItemRef inItem);
	
	void ASAPI (*SetUnits)(ADMItemRef inItem, ADMUnits inUnits);
	ADMUnits ASAPI (*GetUnits)(ADMItemRef inItem);
	
	// item value accessors

	void ASAPI (*SetPrecision)(ADMItemRef inItem, ASInt32 inNumberOfDecimalPlaces);
	ASInt32 ASAPI (*GetPrecision)(ADMItemRef inItem);
	
	void ASAPI (*SetBooleanValue)(ADMItemRef inItem, ASBoolean inValue);
	ASBoolean ASAPI (*GetBooleanValue)(ADMItemRef inItem);
	
	void ASAPI (*SetIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetIntValue)(ADMItemRef inItem);
		
	void ASAPI (*SetFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMinIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetMinIntValue)(ADMItemRef inItem);
		
	void ASAPI (*SetMinFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetMinFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetMaxIntValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMinFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetMinFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetMaxFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetMaxFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetSmallIncrement)(ADMItemRef inItem, float inIncrement);	
	float ASAPI (*GetSmallIncrement)(ADMItemRef inItem);
	
	void ASAPI (*SetLargeIncrement)(ADMItemRef inItem, float inIncrement);
	float ASAPI (*GetLargeIncrement)(ADMItemRef inItem);
	
	void ASAPI (*SetAllowUnits)(ADMItemRef inItem, ASBoolean inAllowUnits);
	ASBoolean ASAPI (*GetAllowUnits)(ADMItemRef inItem);

	void ASAPI (*SetAllowMath)(ADMItemRef inItem, ASBoolean inAllowMath);
	ASBoolean ASAPI (*GetAllowMath)(ADMItemRef inItem);
	
	void ASAPI (*SetFloatToTextProc)(ADMItemRef inItem, ADMItemFloatToTextProc inProc);
	ADMItemFloatToTextProc ASAPI (*GetFloatToTextProc)(ADMItemRef inItem);
	
	ASBoolean ASAPI (*DefaultFloatToText)(ADMItemRef inItem, float inValue, char* outText,
		ASInt32 inMaxLength);

	ASBoolean ASAPI (*DefaultFloatToTextW)(ADMItemRef inItem, float inValue, ASUnicode* outText,
		ASInt32 inMaxLength);

	void ASAPI (*SetTextToFloatProc)(ADMItemRef inItem, ADMItemTextToFloatProc inProc);
	ADMItemTextToFloatProc ASAPI (*GetTextToFloatProc)(ADMItemRef inItem);

	ASBoolean ASAPI (*DefaultTextToFloat)(ADMItemRef inItem, const char* inText, float* outValue);
	ASBoolean ASAPI (*DefaultTextToFloatW)(ADMItemRef inItem, const ASUnicode* inText, float* outValue);

	void ASAPI (*ShowUnits)(ADMItemRef inItem, ASBoolean inShowUnits);
	ASBoolean ASAPI (*GetShowUnits)(ADMItemRef inItem);

	ASBoolean ASAPI (*WasPercentageChange)(ADMItemRef inItem);

	// item help ID
	
	void ASAPI (*SetHelpID)(ADMItemRef inItem, ASHelpID inHelpID);
	ASHelpID ASAPI (*GetHelpID)(ADMItemRef inItem);
	void ASAPI (*Help)(ADMItemRef inDialog);

	// hierarchy list accessor
	
	ADMHierarchyListRef ASAPI (*GetHierarchyList)(ADMItemRef inItem);
	
	// item group accessors
	
 	void ASAPI (*AddItem)(ADMItemRef inGroup, ADMItemRef inItem);
 	void ASAPI (*RemoveItem)(ADMItemRef inGroup, ADMItemRef inItem);

	// item pictures (from icons)
	
	void ASAPI (*SetPicture)(ADMItemRef inItem, ADMIconRef inPicture);
	ADMIconRef ASAPI (*GetPicture)(ADMItemRef inItem);

	void ASAPI (*SetSelectedPicture)(ADMItemRef inItem, ADMIconRef inPicture);
	ADMIconRef ASAPI (*GetSelectedPicture)(ADMItemRef inItem);

	void ASAPI (*SetDisabledPicture)(ADMItemRef inItem, ADMIconRef inPicture);
	ADMIconRef ASAPI (*GetDisabledPicture)(ADMItemRef inItem);
	
	// popup control accessors
	
	void ASAPI (*SetPopupDialog)(ADMItemRef inItem, ASInt32 inPopupItemID, ADMDialogRef inDialog);
	ADMDialogRef ASAPI (*GetPopupDialog)(ADMItemRef inItem);


	// Set/Get the WantsFocus property - whether or not the item wants to receive focus
	void ASAPI (*SetWantsFocus)(ADMItemRef inItem, ASBoolean inWantsFocus);
	ASBoolean ASAPI (*GetWantsFocus)(ADMItemRef inItem);

	// Basic color accessors for all applicable ADM items
	void ASAPI (*SetForeColor)(ADMItemRef inItem, ADMColor inColor);
	ADMColor ASAPI (*GetForeColor)(ADMItemRef inItem);
	void ASAPI (*SetBackColor)(ADMItemRef inItem, ADMColor inColor);
	ADMColor ASAPI (*GetBackColor)(ADMItemRef inItem);
}
ADMItemSuite6;

// -----------------------------------------------------------------------------

// =============================================================================
//		* ADM Item Suite
// =============================================================================

#define kADMItemSuite "ADM Item Suite"
#define kADMItemSuiteVersion7 7

// -----------------------------------------------------------------------------

typedef struct ADMItemSuite7
{

	// *** This suite is FROZEN.
	// *** It shipped with Illustrator 10

	// item creation/destruction

	ADMItemRef ASAPI (*Create)(ADMDialogRef inDialog, ASInt32 inItemID,
				ADMItemType inItemType, const ASRect* inBoundsRect,
				ADMItemInitProc inInitProc, ADMUserData inUserData,
				ASInt32 inOptions);

	void ASAPI (*Destroy)(ADMItemRef inItem);
	
	// item draw proc
	
	void ASAPI (*SetDrawProc)(ADMItemRef inItem, ADMItemDrawProc inDrawProc);
	ADMItemDrawProc ASAPI (*GetDrawProc)(ADMItemRef inItem);
	void ASAPI (*DefaultDraw)(ADMItemRef inItem, ADMDrawerRef inDrawer);

	// item track proc
	
	void ASAPI (*SetTrackProc)(ADMItemRef inItem, ADMItemTrackProc inTrackProc);
	ADMItemTrackProc ASAPI (*GetTrackProc)(ADMItemRef inItem);
	ASBoolean ASAPI (*DefaultTrack)(ADMItemRef inItem, ADMTrackerRef inTracker);

	void ASAPI (*SetMask)(ADMItemRef inItem, ADMActionMask inActionMask);
	ADMActionMask ASAPI (*GetMask)(ADMItemRef inItem);

	// item notify proc

	void ASAPI (*SetNotifyProc)(ADMItemRef inItem, ADMItemNotifyProc inNotifyProc);
	ADMItemNotifyProc ASAPI (*GetNotifyProc)(ADMItemRef inItem);
	void ASAPI (*DefaultNotify)(ADMItemRef inItem, ADMNotifierRef inNotifier);
	void ASAPI (*SendNotify)(ADMItemRef inItem, const char* inNotifierType);

	void ASAPI (*SetNotifierData)(ADMItemRef inItem, ADMUserData inNotifierData);
	ADMUserData ASAPI (*GetNotifierData)(ADMItemRef inItem);

	// item destroy proc

	void ASAPI (*SetDestroyProc)(ADMItemRef inItem, ADMItemDestroyProc inDestroyProc);
	ADMItemDestroyProc ASAPI (*GetDestroyProc)(ADMItemRef inItem);
	
	// item timer
	
	ADMTimerRef ASAPI (*CreateTimer)(ADMItemRef inItem, ASUInt32 inMilliseconds,
				ADMActionMask inAbortMask, ADMItemTimerProc inTimerProc,
				ADMItemTimerAbortProc inTimerAbortProc, ASInt32 inOptions);

	void ASAPI (*AbortTimer)(ADMItemRef inItem, ADMTimerRef inTimer);

	// item user data

	void ASAPI (*SetUserData)(ADMItemRef inItem, ADMUserData inUserData);
	ADMUserData ASAPI (*GetUserData)(ADMItemRef inItem);

	// item information accessors

	ASInt32 ASAPI (*GetID)(ADMItemRef inItem);
	ADMDialogRef ASAPI (*GetDialog)(ADMItemRef inItem);
	ASWindowRef ASAPI (*GetWindowRef)(ADMItemRef inItem);
	ADMListRef ASAPI (*GetList)(ADMItemRef inItem);
	ADMItemRef ASAPI (*GetChildItem)(ADMItemRef inItem, ASInt32 inChildID);	

	void ASAPI (*SetPluginRef)(ADMItemRef inItem, SPPluginRef inPluginRef);
	SPPluginRef ASAPI (*GetPluginRef)(ADMItemRef inItem);

	void ASAPI (*SetItemType)(ADMItemRef inItem, ADMItemType inItemType);
	ADMItemType ASAPI (*GetItemType)(ADMItemRef inItem);
	
	void ASAPI (*SetItemStyle)(ADMItemRef inItem, ADMItemStyle inItemStyle);
	ADMItemStyle ASAPI (*GetItemStyle)(ADMItemRef inItem);
	
	// item state accessors
	
	void ASAPI (*Show)(ADMItemRef inItem, ASBoolean inShow);
	ASBoolean ASAPI (*IsVisible)(ADMItemRef inItem);
	
	void ASAPI (*Enable)(ADMItemRef inItem, ASBoolean inEnable);
	ASBoolean ASAPI (*IsEnabled)(ADMItemRef inItem);
	
	void ASAPI (*Activate)(ADMItemRef inItem, ASBoolean inActivate);
	ASBoolean ASAPI (*IsActive)(ADMItemRef inItem);
	
	void ASAPI (*Known)(ADMItemRef inItem, ASBoolean inKnown);
	ASBoolean ASAPI (*IsKnown)(ADMItemRef inItem);
	
	// item bounds accessors

	void ASAPI (*SetLocalRect)(ADMItemRef inItem, const ASRect* inLocalRect);
	void ASAPI (*GetLocalRect)(ADMItemRef inItem, ASRect* outLocalRect);
	
	void ASAPI (*SetBoundsRect)(ADMItemRef inItem, const ASRect* inBoundsRect);
	void ASAPI (*GetBoundsRect)(ADMItemRef inItem, ASRect* outBoundsRect);
	
	void ASAPI (*GetBestSize)(ADMItemRef inItem, ASPoint* outBestSize);

	void ASAPI (*Move)(ADMItemRef inItem, ASInt32 inHorizPosition, ASInt32 inVertPosition);
	void ASAPI (*Size)(ADMItemRef inItem, ASInt32 inWidth, ASInt32 inHeight);

	// coordinate system transformations	

	void ASAPI (*LocalToScreenPoint)(ADMItemRef inItem, ASPoint* ioPoint);
	void ASAPI (*ScreenToLocalPoint)(ADMItemRef inItem, ASPoint* ioPoint);
	
	void ASAPI (*LocalToScreenRect)(ADMItemRef inItem, ASRect* ioRect);
	void ASAPI (*ScreenToLocalRect)(ADMItemRef inItem, ASRect* ioRect);
	
	// item display
	
	void ASAPI (*Invalidate)(ADMItemRef inItem);
	void ASAPI (*InvalidateRect)(ADMItemRef inDialog, const ASRect* inInvalRect);
	void ASAPI (*Update)(ADMItemRef inItem);
	
	// cursor ID accessors
	
	ASBoolean ASAPI (*SetCursorID)(ADMItemRef inItem, SPPluginRef inPluginRef, ASInt32 inCursorID,
				const char* inCursorName);

	void ASAPI (*GetCursorID)(ADMItemRef inItem, SPPluginRef* outPluginRef, ASInt32* outCursorID,
				const char** outCursorName);

	// picture ID accessors

	void ASAPI (*SetPictureID)(ADMItemRef inItem, ASInt32 inPictureResID,
				const char* inPictureResName);

	void ASAPI (*GetPictureID)(ADMItemRef inItem, ASInt32* outPictureResID,
				const char** outPictureResName);
	
	void ASAPI (*SetSelectedPictureID)(ADMItemRef inItem, ASInt32 inPictureResID,
				const char* inPictureResName);

	void ASAPI (*GetSelectedPictureID)(ADMItemRef inItem, ASInt32* outPictureResID,
				const char** outPictureResName);
	
	void ASAPI (*SetDisabledPictureID)(ADMItemRef inItem, ASInt32 inPictureResID,
				const char* inPictureResName);

	void ASAPI (*GetDisabledPictureID)(ADMItemRef inItem, ASInt32* outPictureResID,
				const char** outPictureResName);

	// tooltips

	void ASAPI (*SetTipString)(ADMItemRef inItem, const char* inTipString);
	void ASAPI (*SetTipStringW)(ADMItemRef inItem, const ASUnicode* inTipString);

	void ASAPI (*GetTipString)(ADMItemRef inItem, char* outTipString, ASInt32 inMaxLength);
	void ASAPI (*GetTipStringW)(ADMItemRef inItem, ASUnicode* outTipString, ASInt32 inMaxLength);
	
	ASInt32 ASAPI (*GetTipStringLength)(ADMItemRef inItem);
	ASInt32 ASAPI (*GetTipStringLengthW)(ADMItemRef inItem);
	
	void ASAPI (*EnableTip)(ADMItemRef inItem, ASBoolean inEnable);
	ASBoolean ASAPI (*IsTipEnabled)(ADMItemRef inItem);
	
	void ASAPI (*ShowToolTip)(ADMItemRef inItem, const ASPoint* inWhere);
	void ASAPI (*HideToolTip)(ADMItemRef inItem);
	
	// item text accessors

	void ASAPI (*SetFont)(ADMItemRef inItem, ADMFont inFontID);
	ADMFont ASAPI (*GetFont)(ADMItemRef inItem);

	void ASAPI (*SetText)(ADMItemRef inItem, const char* inText);
	void ASAPI (*SetTextW)(ADMItemRef inItem, const ASUnicode* inText);

	void ASAPI (*GetText)(ADMItemRef inItem, char* outText, ASInt32 inMaxLength);
	void ASAPI (*GetTextW)(ADMItemRef inItem, ASUnicode* outText, ASInt32 inMaxLength);

	ASInt32 ASAPI (*GetTextLength)(ADMItemRef inItem);
	ASInt32 ASAPI (*GetTextLengthW)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxTextLength)(ADMItemRef inItem, ASInt32 inLength);
	ASInt32 ASAPI (*GetMaxTextLength)(ADMItemRef inItem);
	
	void ASAPI (*SelectAll)(ADMItemRef inItem);
	void ASAPI (*SetSelectionRange)(ADMItemRef inItem, ASInt32 inSelStart, ASInt32 inSelEnd);
	void ASAPI (*GetSelectionRange)(ADMItemRef inItem, ASInt32* outSelStart, ASInt32* outSelEnd);

	void ASAPI (*SetJustify)(ADMItemRef inItem, ADMJustify inJustify);
	ADMJustify ASAPI (*GetJustify)(ADMItemRef inItem);
	
	void ASAPI (*SetUnits)(ADMItemRef inItem, ADMUnits inUnits);
	ADMUnits ASAPI (*GetUnits)(ADMItemRef inItem);
	
	// item value accessors

	void ASAPI (*SetPrecision)(ADMItemRef inItem, ASInt32 inNumberOfDecimalPlaces);
	ASInt32 ASAPI (*GetPrecision)(ADMItemRef inItem);
	
	void ASAPI (*SetBooleanValue)(ADMItemRef inItem, ASBoolean inValue);
	ASBoolean ASAPI (*GetBooleanValue)(ADMItemRef inItem);
	
	void ASAPI (*SetIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetIntValue)(ADMItemRef inItem);
		
	void ASAPI (*SetFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMinIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetMinIntValue)(ADMItemRef inItem);
		
	void ASAPI (*SetMinFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetMinFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxIntValue)(ADMItemRef inItem, ASInt32 inValue);
	ASInt32 ASAPI (*GetMaxIntValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMinFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetMinFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxFixedValue)(ADMItemRef inItem, ASFixed inValue);	
	ASFixed ASAPI (*GetMaxFixedValue)(ADMItemRef inItem);
	
	void ASAPI (*SetMaxFloatValue)(ADMItemRef inItem, float inValue);	
	float ASAPI (*GetMaxFloatValue)(ADMItemRef inItem);
	
	void ASAPI (*SetSmallIncrement)(ADMItemRef inItem, float inIncrement);	
	float ASAPI (*GetSmallIncrement)(ADMItemRef inItem);
	
	void ASAPI (*SetLargeIncrement)(ADMItemRef inItem, float inIncrement);
	float ASAPI (*GetLargeIncrement)(ADMItemRef inItem);
	
	void ASAPI (*SetAllowUnits)(ADMItemRef inItem, ASBoolean inAllowUnits);
	ASBoolean ASAPI (*GetAllowUnits)(ADMItemRef inItem);

	void ASAPI (*SetAllowMath)(ADMItemRef inItem, ASBoolean inAllowMath);
	ASBoolean ASAPI (*GetAllowMath)(ADMItemRef inItem);
	
	void ASAPI (*SetFloatToTextProc)(ADMItemRef inItem, ADMItemFloatToTextProc inProc);
	ADMItemFloatToTextProc ASAPI (*GetFloatToTextProc)(ADMItemRef inItem);
	
	ASBoolean ASAPI (*DefaultFloatToText)(ADMItemRef inItem, float inValue, char* outText,
		ASInt32 inMaxLength);

	ASBoolean ASAPI (*DefaultFloatToTextW)(ADMItemRef inItem, float inValue, ASUnicode* outText,
		ASInt32 inMaxLength);

	void ASAPI (*SetTextToFloatProc)(ADMItemRef inItem, ADMItemTextToFloatProc inProc);
	ADMItemTextToFloatProc ASAPI (*GetTextToFloatProc)(ADMItemRef inItem);

	ASBoolean ASAPI (*DefaultTextToFloat)(ADMItemRef inItem, const char* inText, float* outValue);
	ASBoolean ASAPI (*DefaultTextToFloatW)(ADMItemRef inItem, const ASUnicode* inText, float* outValue);

	void ASAPI (*ShowUnits)(ADMItemRef inItem, ASBoolean inShowUnits);
	ASBoolean ASAPI (*GetShowUnits)(ADMItemRef inItem);

	ASBoolean ASAPI (*WasPercentageChange)(ADMItemRef inItem);

	// item help ID
	
	void ASAPI (*SetHelpID)(ADMItemRef inItem, ASHelpID inHelpID);
	ASHelpID ASAPI (*GetHelpID)(ADMItemRef inItem);
	void ASAPI (*Help)(ADMItemRef inDialog);

	// hierarchy list accessor
	
	ADMHierarchyListRef ASAPI (*GetHierarchyList)(ADMItemRef inItem);
	
	// item group accessors
	
 	void ASAPI (*AddItem)(ADMItemRef inGroup, ADMItemRef inItem);
 	void ASAPI (*RemoveItem)(ADMItemRef inGroup, ADMItemRef inItem);

	// item pictures (from icons)
	
	void ASAPI (*SetPicture)(ADMItemRef inItem, ADMIconRef inPicture);
	ADMIconRef ASAPI (*GetPicture)(ADMItemRef inItem);

	void ASAPI (*SetSelectedPicture)(ADMItemRef inItem, ADMIconRef inPicture);
	ADMIconRef ASAPI (*GetSelectedPicture)(ADMItemRef inItem);

	void ASAPI (*SetDisabledPicture)(ADMItemRef inItem, ADMIconRef inPicture);
	ADMIconRef ASAPI (*GetDisabledPicture)(ADMItemRef inItem);
	
	// popup control accessors
	
	void ASAPI (*SetPopupDialog)(ADMItemRef inItem, ASInt32 inPopupItemID, ADMDialogRef inDialog);
	ADMDialogRef ASAPI (*GetPopupDialog)(ADMItemRef inItem);


	// Set/Get the WantsFocus property - whether or not the item wants to receive focus
	void ASAPI (*SetWantsFocus)(ADMItemRef inItem, ASBoolean inWantsFocus);
	ASBoolean ASAPI (*GetWantsFocus)(ADMItemRef inItem);

	// Basic color accessors for all applicable ADM items
	void ASAPI (*SetForeColor)(ADMItemRef inItem, ADMColor inColor);
	ADMColor ASAPI (*GetForeColor)(ADMItemRef inItem);
	void ASAPI (*SetBackColor)(ADMItemRef inItem, ADMColor inColor);
	ADMColor ASAPI (*GetBackColor)(ADMItemRef inItem);

	
	//Accessor functions for Rollover state.

	void ASAPI (*SetRolloverPictureID)(ADMItemRef inItem, ASInt32 inPictureResID,
				const char* inPictureResName);

	void ASAPI (*GetRolloverPictureID)(ADMItemRef inItem, ASInt32* outPictureResID,
				const char** outPictureResName);

	void ASAPI (*SetRolloverPicture)(ADMItemRef inItem, ADMIconRef inPicture);
	ADMIconRef ASAPI (*GetRolloverPicture)(ADMItemRef inItem);

	// Set/Get the Has Rollover property - whether or not the item wants to have rollover
	void ASAPI (*SetHasRollOverProperty)(ADMItemRef inItem, ASBoolean inRollover);
	ASBoolean ASAPI (*GetHasRollOverProperty)(ADMItemRef inItem);

	// Set/Get whether or not the item is currently rolled over or not
	void ASAPI (*SetInRollOverState)(ADMItemRef inItem, ASBoolean inRolloverState);
	ASBoolean ASAPI (*IsInRollOverState)(ADMItemRef inItem);

}
ADMItemSuite7;
// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

#endif
