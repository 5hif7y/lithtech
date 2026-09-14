/***********************************************************************/
/*                                                                     */
/* ADMIconOld.h                                                        */
/* Old versions of ADM Icon Suite                                      */
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

#ifndef __ADMIconOld__
#define __ADMIconOld__

#ifndef __ADMTypes__
#include "admTypes.h"
#endif

#ifndef __ADMIcon__
#include "admIcon.h"
#endif


// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


// =============================================================================
//		* ADM Icon Suite, version 1
// =============================================================================

#define kADMIconSuiteVersion1 1
#define kADMIconSuiteVersion kADMIconSuiteVersion1

// -----------------------------------------------------------------------------

typedef struct ADMIconSuite1
{

	// icon creation/destruction

	ADMIconRef ASAPI (*GetFromResource)(SPPluginRef inPluginRef, ASInt32 inIconID,
				ASInt32 inIconIndex);

	ADMIconRef ASAPI (*Create)(ADMIconType inIconType, ASInt32 inWidth,
				ASInt32 inHeight, void* inData);

	void ASAPI (*Destroy)(ADMIconRef inIcon);

	// icon data accessors

	ADMIconType ASAPI (*GetType)(ADMIconRef inIcon);
	ASInt32 ASAPI (*GetWidth)(ADMIconRef inIcon);
	ASInt32 ASAPI (*GetHeight)(ADMIconRef inIcon);
	ASAPI void* (*GetData)(ADMIconRef inIcon);

	ASBoolean ASAPI (*IsFromResource)(ADMIconRef inIcon);

}
ADMIconSuite1;

typedef ADMIconSuite1 ADMIconSuite;

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

#endif  // __ADMIconOld__
