/***********************************************************************/
/*                                                                     */
/* ADMIcon.h                                                           */
/* ADM Icon Suite                                                      */
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
/* Started by Paul Asente, 08/01/1996                                  */
/*                                                                     */
/***********************************************************************/

#ifndef __ADMIcon__
#define __ADMIcon__

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
//	Icon types

typedef enum {

		// Mac types
	kCICN, kPICT, kIconSuite,
	
		// Windows types
	kWinIcon, kBMP,

		// Either type
	kADMImageIcon,
	kUnknown

}
ADMIconType;


// =============================================================================
//		* ADM Icon Suite
// =============================================================================

#define kADMIconSuite				"ADM Icon Suite"
#define kADMIconSuiteVersion2		2

// -----------------------------------------------------------------------------

typedef struct ADMIconSuite2
{

	// *** This suite is FROZEN.
	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.

	// icon creation/destruction

	ADMIconRef ASAPI (*GetFromResource)(SPPluginRef inPluginRef, const char* inName,
				ASInt32 inIconID, ASInt32 inIconIndex);

	ADMIconRef ASAPI (*Create)(ADMIconType inIconType, ASInt32 inWidth,
				ASInt32 inHeight, void* inData);

	void ASAPI (*Destroy)(ADMIconRef inIcon);
	
	// icon data accessors

	ADMIconType ASAPI (*GetType)(ADMIconRef inIcon);
	ASInt32 ASAPI (*GetWidth)(ADMIconRef inIcon);
	ASInt32 ASAPI (*GetHeight)(ADMIconRef inIcon);
	ASAPI void* (*GetData)(ADMIconRef inIcon);

	ASBoolean ASAPI (*IsFromResource)(ADMIconRef inIcon);

	// conversion from ADMImage

	ADMIconRef ASAPI (*CreateFromImage)(ADMImageRef inImage);
		// The ADMIcon takes ownership of the image.


}
ADMIconSuite2;


// -----------------------------------------------------------------------------
//	Old versions of suites

#ifndef __ADMIconOld__
#ifdef MAC_ENV
#include "ADMIconOld.h"
#else
#include ".\Legacy\ADMIconOld.h"
#endif
#endif

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif
 
// -----------------------------------------------------------------------------

#endif	// __ADMIcon__
