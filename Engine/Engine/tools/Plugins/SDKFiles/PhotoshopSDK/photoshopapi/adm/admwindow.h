/***********************************************************************/
/*                                                                     */
/* ADMWindow.h                                                         */
/* ADM Window Suite                                                    */
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

#ifndef __ADMWindow__
#define __ADMWindow__

#ifndef __ADMTypes__
#include "admTypes.h"
#endif


// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


// -----------------------------------------------------------------------------
//	Window style codes

typedef enum
{

	// NOTE: These first four correspond to ADMDialogStyles
	// and are all variants of type kADMWindow.

	kADMFloatingWindowStyle = 2,
	kADMTabbedFloatingWindowStyle = 3,
	kADMResizingFloatingWindowStyle = 4,
	kADMTabbedResizingFloatingWindowStyle = 5,
	
	kADMDocumentWindowStyle = 100,
				// for searches only, to find all document window types

	kADMGrowNoZoomDocumentWindowStyle = 100,
				// for new window calls, use a specific type

	kADMNoGrowNoZoomDocumentWindowStyle = 104,
	kADMGrowZoomDocumentWindowStyle = 108,
	kADMNoGrowZoomDocumentWindowStyle = 112,
	
	kADMPopupWindowStyle = 300,
	kADMPopupFloatingWindowStyle = 301,
	
	kADMDummyWindowStyle = 0xFFFFFFFF

}
ADMWindowStyle;


// =============================================================================
//		* ADM Window Suite
// =============================================================================
//	NOTE: This suite is currently implemented on MacOS only.

#define kADMWindowSuite "ADM Window Suite"
#define kADMWindowSuiteVersion1 1
#define kADMWindowSuiteVersion kADMWindowSuiteVersion1

// -----------------------------------------------------------------------------

typedef struct ADMWindowSuite1
{

	// *** This suite is FROZEN.
	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.

	// window creation/destruction

	ASWindowRef ASAPI (*NewLayerWindow)(ADMWindowStyle inWindowKind,
				unsigned char* inStorage, const ASRect* inBoundsRect,
				const char* inTitle, ASBoolean inVisible,
				ASBoolean inGoAwayFlag, ASInt32 inRefCon);

	ASWindowRef ASAPI (*GetNewLayerWindow)(SPPluginRef inPluginRef,
				ADMWindowStyle inWindowKind, ASInt16 inWindowID,
				unsigned char* inStorage);

	void ASAPI (*DisposeLayerWindow)(ASWindowRef inWindow);

	// window visibility and layering

	void ASAPI (*HideLayerWindow)(ASWindowRef inWindow);
	void ASAPI (*ShowAndSelectLayerWindow)(ASWindowRef inWindow);
	void ASAPI (*SelectLayerWindow)(ASWindowRef inWindow);
	void ASAPI (*ShowLayerWindow)(ASWindowRef inWindow);
	
	// window dragging and snapping
	
	void ASAPI (*DragLayerWindow)(ASWindowRef inWindow, ASPoint inStartPoint,
				const ASRect* inDraggingBounds);

	// window order accessors

	ASWindowRef ASAPI (*GetFrontWindow)(ADMWindowStyle inWindowKind);
	void ASAPI (*UpdateWindowOrder)();

}
ADMWindowSuite1;

typedef ADMWindowSuite1 ADMWindowSuite;

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------

#endif	// __ADMWindow__
