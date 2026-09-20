/***********************************************************************/
/*                                                                     */
/* ADMImageOld.h                                                       */
/* Old versions of ADM Image suite                                     */
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

#ifndef __ADMImageOld__
#define __ADMImageOld__

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
//		* ADM Image Suite, version 1
// =============================================================================

#define kADMImageSuiteVersion1 1
#define kADMImageSuiteVersion kADMImageSuiteVersion1	// not current, just frozen

// -----------------------------------------------------------------------------

typedef struct ADMImageSuite1
{
	ADMImageRef ASAPI (*Create)(ASInt32 inWidth, ASInt32 inHeight);
	void ASAPI (*Destroy)(ADMImageRef inImage);

	ASInt32 ASAPI (*GetWidth)(ADMImageRef inImage);
	ASInt32 ASAPI (*GetHeight)(ADMImageRef inImage);
	ASInt32 ASAPI (*GetByteWidth)(ADMImageRef inImage);
	ASInt32 ASAPI (*GetBitsPerPixel)(ADMImageRef inImage);
	
	ASBytePtr ASAPI (*BeginBaseAddressAccess)(ADMImageRef inImage);
	void ASAPI (*EndBaseAddressAccess)(ADMImageRef inImage);
	
	ADMDrawerRef ASAPI (*BeginADMDrawer)(ADMImageRef inImage);
	void ASAPI (*EndADMDrawer)(ADMImageRef inImage);
	
	void ASAPI (*BeginAGMImageAccess)(ADMImageRef inImage, struct _t_ADMAGMImageRecord* inImageRecord);
	void ASAPI (*EndAGMImageAccess)(ADMImageRef inImage);
	
	ASErr ASAPI (*GetPixel)(ADMImageRef inImage, const ASPoint* inPoint, ASRGBColor* outColor);
	ASErr ASAPI (*SetPixel)(ADMImageRef inImage, const ASPoint* inPoint, const ASRGBColor* inColor);
	
	ADMImageRef ASAPI (*CreateBitmap)(ASInt32 inWidth, ASInt32 inHeight);
	ADMImageRef ASAPI (*CreateOffscreen)(ASInt32 inWidth, ASInt32 inHeight);

}
ADMImageSuite1;

typedef ADMImageSuite1 ADMImageSuite;	// for historical reasons


// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

#endif
