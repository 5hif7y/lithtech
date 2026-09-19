/***********************************************************************/
/*                                                                     */
/* ADMHostWin.h                                                        */
/* ADM Host Window Suite                                               */
/*                                                                     */
/* ADOBE SYSTEMS INCORPORATED                                          */
/* Copyright 1998-2001 Adobe Systems Incorporated                      */
/* All Rights Reserved                                                 */
/*                                                                     */
/* NOTICE:  Adobe permits you to use, modify, and distribute this file */
/* in accordance with the terms of the Adobe license agreement         */
/* accompanying it. If you have received this file from a source other */
/* than Adobe, then your use, modification, or distribution of it      */
/* requires the prior written permission of Adobe.                     */
/*                                                                     */
/* Started by Joe Ault and Rick Doty, 05/27/1998                       */
/*                                                                     */
/***********************************************************************/

#ifndef __ADMHostWin__
#define __ADMHostWin__


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
//		* ADM Host Window Suite
// =============================================================================
//	The host of ADM provides this suite for ADM as the window handler for
//	palettes. ADM will draw into a panel provided by the host.

#define kADMHostWindowSuite "ADM Host Window Suite"
#define kADMHostWindowSuiteVersion1 1
#define kADMHostWindowSuiteVersion kADMHostWindowSuiteVersion1

// -----------------------------------------------------------------------------

typedef struct ADMHostWindowSuite1
{

	// *** This suite is FROZEN.
	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.

	void ASAPI (*GetWindowRef)(SPPluginRef inPluginRef, ASWindowRef* inWindow,
				ADMDialogStyle inStyle);

	void ASAPI (*ShowWindow)(SPPluginRef inPluginRef, ASWindowRef* inWindow,
				ASBoolean inShowState);

	// Also need others depending on feedback. Hide, create, destroy, activate?
	// Separate from SetProcs?
	
}
ADMHostWindowSuite1;

typedef ADMHostWindowSuite1 ADMHostWindowSuite;

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------

#endif	// __ADMHostWin__
