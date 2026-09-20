/***********************************************************************/
/*                                                                     */
/* ADMImage.h                                                          */
/* ADM Image Suite                                                     */
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
/* Started by Dave Lazarony, 11/20/1996                                */
/*                                                                     */
/***********************************************************************/

#ifndef __ADMImage__
#define __ADMImage__

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

#define kADMImageHasAlphaChannelOption (1L << 0)


// =============================================================================
//		* ADM Image Suite
// =============================================================================

#define kADMImageSuite "ADM Image Suite"
#define kADMImageSuiteVersion2 2

// -----------------------------------------------------------------------------

typedef struct ADMImageSuite2
{

	// *** This suite is FROZEN.
	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.

	// image creation/destruction

	ADMImageRef ASAPI (*Create)(ASInt32 inWidth, ASInt32 inHeight, ASInt32 inOptions);
	void ASAPI (*Destroy)(ADMImageRef inImage);

	// image data accessors

	ASInt32 ASAPI (*GetWidth)(ADMImageRef inImage);
	ASInt32 ASAPI (*GetHeight)(ADMImageRef inImage);
	ASInt32 ASAPI (*GetByteWidth)(ADMImageRef inImage);
	ASInt32 ASAPI (*GetBitsPerPixel)(ADMImageRef inImage);
	
	// raw pixel data access
	
	ASBytePtr ASAPI (*BeginBaseAddressAccess)(ADMImageRef inImage);
	void ASAPI (*EndBaseAddressAccess)(ADMImageRef inImage);

	// ADMDrawer access
	
	ADMDrawerRef ASAPI (*BeginADMDrawer)(ADMImageRef inImage);
	void ASAPI (*EndADMDrawer)(ADMImageRef inImage);

	// AGMImage access
		// only available if host app exports AGM suites
	
	void ASAPI (*BeginAGMImageAccess)(ADMImageRef inImage, struct _t_ADMAGMImageRecord* outImageRecord);
	void ASAPI (*EndAGMImageAccess)(ADMImageRef inImage);

	// single pixel access
	
	ASErr ASAPI (*GetPixel)(ADMImageRef inImage, const ASPoint* inPoint, ASRGBColor* outColor);
	ASErr ASAPI (*SetPixel)(ADMImageRef inImage, const ASPoint* inPoint, const ASRGBColor* inColor);

	// more image creation options
	
	ADMImageRef ASAPI (*CreateBitmap)(ASInt32 inWidth, ASInt32 inHeight, ASInt32 inOptions);
	ADMImageRef ASAPI (*CreateOffscreen)(ASInt32 inWidth, ASInt32 inHeight, ASInt32 inOptions);

}
ADMImageSuite2;


// -----------------------------------------------------------------------------
//	Old versions of suites

#ifndef __ADMImageOld__
#ifdef MAC_ENV
#include "ADMImageOld.h"
#else
#include ".\Legacy\ADMImageOld.h"
#endif
#endif

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------

#endif	// __ADMImage__
