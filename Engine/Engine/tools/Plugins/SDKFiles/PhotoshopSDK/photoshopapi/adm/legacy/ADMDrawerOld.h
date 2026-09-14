/***********************************************************************/
/*                                                                     */
/* ADMDrawerOld.h                                                      */
/* Old versions of ADM Drawer Suite                                    */
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

#ifndef __ADMDrawerOld__
#define __ADMDrawerOld__

#ifndef __ADMTypes__
#include "admTypes.h"
#endif

#ifndef _ADMAGMTYPES_H_
#include "admAGMTypes.h"
#endif


// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


// =============================================================================
//		* ADM Drawer Suite, version 2
// =============================================================================

#define kADMDrawerSuiteVersion2 2

// -----------------------------------------------------------------------------

typedef struct ADMDrawerSuite2
{

	// port accessor

	ASPortRef ASAPI (*GetPortRef)(ADMDrawerRef inDrawer);

	// clear entire area of drawer
	
	void ASAPI (*Clear)(ADMDrawerRef inDrawer);

	// bounds accessor
	
	void ASAPI (*GetBoundsRect)(ADMDrawerRef inDrawer, ASRect* outBoundsRect);

	// clipping
	
	void ASAPI (*GetClipRect)(ADMDrawerRef inDrawer, ASRect* outClipRect);
	void ASAPI (*SetClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);
	void ASAPI (*IntersectClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);
	void ASAPI (*UnionClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);
	void ASAPI (*SubtractClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);

	void ASAPI (*SetClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);

	void ASAPI (*IntersectClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);

	void ASAPI (*UnionClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);

	void ASAPI (*SubtractClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);
	
	// port origin
	
	void ASAPI (*GetOrigin)(ADMDrawerRef inDrawer, ASPoint* outOrigin);
	void ASAPI (*SetOrigin)(ADMDrawerRef inDrawer, const ASPoint* inOrigin);
	
	// drawing state accessors
	
	void ASAPI (*GetRGBColor)(ADMDrawerRef inDrawer, ASRGBColor* outColor);
	void ASAPI (*SetRGBColor)(ADMDrawerRef inDrawer, const ASRGBColor* inColor);
	
	ADMColor ASAPI (*GetADMColor)(ADMDrawerRef inDrawer);
	void ASAPI (*SetADMColor)(ADMDrawerRef inDrawer, ADMColor inColor);
	
	ADMDrawMode ASAPI (*GetDrawMode)(ADMDrawerRef inDrawer);
	void ASAPI (*SetDrawMode)(ADMDrawerRef inDrawer, ADMDrawMode inDrawMode);
	
	ADMFont ASAPI (*GetFont)(ADMDrawerRef inDrawer);
	void ASAPI (*SetFont)(ADMDrawerRef inDrawer, ADMFont inFont);

	// simple shape drawers
	
	void ASAPI (*DrawLine)(ADMDrawerRef inDrawer, const ASPoint* inStartPoint,
				const ASPoint* inEndPoint);
	
	void ASAPI (*DrawPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints, ASInt32 inNumPoints);
	void ASAPI (*FillPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints, ASInt32 inNumPoints);
	
	void ASAPI (*DrawRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*FillRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*ClearRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawSunkenRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawRaisedRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*InvertRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	
	void ASAPI (*DrawOval)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*FillOval)(ADMDrawerRef inDrawer, const ASRect* inRect);

	// AGM access
	
	ADMAGMPortPtr ASAPI (*GetAGMPort)(ADMDrawerRef inDrawer);

	void ASAPI (*DrawAGMImage)(ADMDrawerRef inDrawer,
				const struct _t_ADMAGMImageRecord* inImage,
				const ASFixedMatrix* inMatrix, ASInt32 inFlags);

	// resource drawing
	
	void ASAPI (*DrawResPicture)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,
				ASInt32 inRsrcID, const ASPoint* inTopLeftPoint);

	void ASAPI (*DrawResPictureCentered)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,
				ASInt32 inRsrcID, const ASRect* inRect);
	
	void ASAPI (*DrawRecoloredResPicture)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,
				ASInt32 inRsrcID, const ASPoint* inTopLeftPoint, ADMRecolorStyle inStyle);

	void ASAPI (*DrawRecoloredResPictureCentered)(ADMDrawerRef inDrawer,
				SPPluginRef inPluginRef, ASInt32 inRsrcID, const ASRect* inRect,
				ADMRecolorStyle inStyle);

	// icon drawing

	void ASAPI (*DrawIcon)(ADMDrawerRef inDrawer, ADMIconRef inIcon, const ASPoint* inTopLeftPoint);
	void ASAPI (*DrawIconCentered)(ADMDrawerRef inDrawer, ADMIconRef inIcon, const ASRect* inRect);

	void ASAPI (*DrawRecoloredIcon)(ADMDrawerRef inDrawer, ADMIconRef inIcon,
				const ASPoint* inTopLeftPoint, ADMRecolorStyle inStyle);

	void ASAPI (*DrawRecoloredIconCentered)(ADMDrawerRef inDrawer, ADMIconRef inIcon,
				const ASRect* inRect, ADMRecolorStyle inStyle);

	void ASAPI (*GetResPictureBounds)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,
				ASInt32 inRsrcID, ASRect* outBoundsRect);

	// text drawing

	ASInt32 ASAPI (*GetTextWidth)(ADMDrawerRef inDrawer, const char* inText);
	void ASAPI (*GetFontInfo)(ADMDrawerRef inDrawer, ADMFontInfo* outFontInfo);
	
	void ASAPI (*DrawText)(ADMDrawerRef inDrawer, const char* inText, const ASPoint* inPoint);
	void ASAPI (*DrawTextLeft)(ADMDrawerRef inDrawer, const char* inText, const ASRect* inRect);
	void ASAPI (*DrawTextCentered)(ADMDrawerRef inDrawer, const char* inText, const ASRect* inRect);
	void ASAPI (*DrawTextRight)(ADMDrawerRef inDrawer, const char* inText, const ASRect* inRect);

	// standard arrows
	
	void ASAPI (*DrawUpArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawDownArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawLeftArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawRightArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);

	// drawer creation/destruction

	ADMDrawerRef ASAPI (*Create)(ASPortRef inPortRef, const ASRect* inBoundsRect, ADMFont inFont);	
	void ASAPI (*Destroy)(ADMDrawerRef inDrawer);

	// image drawing

	void ASAPI (*DrawADMImage)(ADMDrawerRef inDrawer, ADMImageRef inImage,
				const ASPoint* inTopLeftPoint);

	void ASAPI (*DrawADMImageCentered)(ADMDrawerRef inDrawer, ADMImageRef inImage,
				const ASRect* inRect);
	
}
ADMDrawerSuite2;


// =============================================================================
//		* ADM Drawer Suite, version 3
// =============================================================================

#define kADMDrawerSuiteVersion3 3

// -----------------------------------------------------------------------------

typedef struct ADMDrawerSuite3
{

	// *** This suite is FROZEN.
	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.

	// port accessor

	ASPortRef ASAPI (*GetPortRef)(ADMDrawerRef inDrawer);

	// clear entire area of drawer
	
	void ASAPI (*Clear)(ADMDrawerRef inDrawer);

	// bounds accessor
	
	void ASAPI (*GetBoundsRect)(ADMDrawerRef inDrawer, ASRect* outBoundsRect);

	// clipping
	
	void ASAPI (*GetClipRect)(ADMDrawerRef inDrawer, ASRect* outClipRect);
	void ASAPI (*SetClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);
	void ASAPI (*IntersectClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);
	void ASAPI (*UnionClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);
	void ASAPI (*SubtractClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);

	void ASAPI (*SetClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);

	void ASAPI (*IntersectClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);

	void ASAPI (*UnionClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);

	void ASAPI (*SubtractClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);
	
	// port origin
	
	void ASAPI (*GetOrigin)(ADMDrawerRef inDrawer, ASPoint* outOrigin);
	void ASAPI (*SetOrigin)(ADMDrawerRef inDrawer, const ASPoint* inOrigin);
	
	// drawing state accessors
	
	void ASAPI (*GetRGBColor)(ADMDrawerRef inDrawer, ASRGBColor* outColor);
	void ASAPI (*SetRGBColor)(ADMDrawerRef inDrawer, const ASRGBColor* inColor);
	
	ADMColor ASAPI (*GetADMColor)(ADMDrawerRef inDrawer);
	void ASAPI (*SetADMColor)(ADMDrawerRef inDrawer, ADMColor inColor);
	
	ADMDrawMode ASAPI (*GetDrawMode)(ADMDrawerRef inDrawer);
	void ASAPI (*SetDrawMode)(ADMDrawerRef inDrawer, ADMDrawMode inDrawMode);
	
	ADMFont ASAPI (*GetFont)(ADMDrawerRef inDrawer);
	void ASAPI (*SetFont)(ADMDrawerRef inDrawer, ADMFont inFont);

	// simple shape drawers
	
	void ASAPI (*DrawLine)(ADMDrawerRef inDrawer, const ASPoint* inStartPoint,
				const ASPoint* inEndPoint);
	
	void ASAPI (*DrawPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);

	void ASAPI (*FillPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);
	
	void ASAPI (*DrawRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*FillRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*ClearRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawSunkenRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawRaisedRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*InvertRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	
	void ASAPI (*DrawOval)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*FillOval)(ADMDrawerRef inDrawer, const ASRect* inRect);

	// AGM access
	
	ADMAGMPortPtr ASAPI (*GetAGMPort)(ADMDrawerRef inDrawer);

	void ASAPI (*DrawAGMImage)(ADMDrawerRef inDrawer,
				const struct _t_ADMAGMImageRecord* inImage,
				const ASFixedMatrix* inMatrix, ASInt32 inFlags);

	// resource drawing
	
	void ASAPI (*DrawResPicture)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,
				ASInt32 inRsrcID, const ASPoint* inTopLeftPoint);

	void ASAPI (*DrawResPictureCentered)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,
				ASInt32 inRsrcID, const ASRect* inRect);
	
	void ASAPI (*DrawRecoloredResPicture)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,
				ASInt32 inRsrcID, const ASPoint* inTopLeftPoint, ADMRecolorStyle inStyle);

	void ASAPI (*DrawRecoloredResPictureCentered)(ADMDrawerRef inDrawer,
				SPPluginRef inPluginRef, ASInt32 inRsrcID, const ASRect* inRect,
				ADMRecolorStyle inStyle);

	// icon drawing

	void ASAPI (*DrawIcon)(ADMDrawerRef inDrawer, ADMIconRef inIcon, const ASPoint* inTopLeftPoint);
	void ASAPI (*DrawIconCentered)(ADMDrawerRef inDrawer, ADMIconRef inIcon, const ASRect* inRect);

	void ASAPI (*DrawRecoloredIcon)(ADMDrawerRef inDrawer, ADMIconRef inIcon,
				const ASPoint* inTopLeftPoint, ADMRecolorStyle inStyle);

	void ASAPI (*DrawRecoloredIconCentered)(ADMDrawerRef inDrawer, ADMIconRef inIcon,
				const ASRect* inRect, ADMRecolorStyle inStyle);

	void ASAPI (*GetResPictureBounds)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,
				ASInt32 inRsrcID, ASRect* outBoundsRect);

	// text drawing

	ASInt32 ASAPI (*GetTextWidth)(ADMDrawerRef inDrawer, const char* inText);
	void ASAPI (*GetFontInfo)(ADMDrawerRef inDrawer, ADMFontInfo* outFontInfo);
	
	void ASAPI (*DrawText)(ADMDrawerRef inDrawer, const char* inText, const ASPoint* inPoint);
	void ASAPI (*DrawTextLeft)(ADMDrawerRef inDrawer, const char* inText, const ASRect* inRect);
	void ASAPI (*DrawTextCentered)(ADMDrawerRef inDrawer, const char* inText, const ASRect* inRect);
	void ASAPI (*DrawTextRight)(ADMDrawerRef inDrawer, const char* inText, const ASRect* inRect);
	
	// standard arrows

	void ASAPI (*DrawUpArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawDownArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawLeftArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawRightArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);

	// drawer creation/destruction

	ADMDrawerRef ASAPI (*Create)(ASPortRef inPortRef, const ASRect* inBoundsRect, ADMFont inFont);	
	void ASAPI (*Destroy)(ADMDrawerRef inDrawer);

	// image drawing

	void ASAPI (*DrawADMImage)(ADMDrawerRef inDrawer, ADMImageRef inImage,
				const ASPoint* inTopLeftPoint);

	void ASAPI (*DrawADMImageCentered)(ADMDrawerRef inDrawer, ADMImageRef inImage,
				const ASRect* inRect);	

	// platform port accessors

	ASPortRef ASAPI (*GetADMWindowPort)(ASWindowRef inWindowRef);
	void ASAPI (*ReleaseADMWindowPort)(ASWindowRef inWindowRef, ASPortRef inPort);

	// font information

	void ASAPI (*GetThisFontInfo)(ADMFont inFont, ADMFontInfo* outFontInfo);

	// text drawing

	void ASAPI (*DrawTextInABox)(ADMDrawerRef inDrawer, const ASRect* inRect, const char* inText);

	// update area accessor

	void ASAPI (*GetUpdateRect)(ADMDrawerRef inDrawer, ASRect* outRect);

}
ADMDrawerSuite3;

// -----------------------------------------------------------------------------

typedef ADMDrawerSuite3 ADMDrawerSuite;
#define kADMDrawerSuiteVersion kADMDrawerSuiteVersion3  // for historical reasons

// =============================================================================
//		* ADM Drawer Suite, version 4
// =============================================================================


#define kADMDrawerSuite "ADM Drawer Suite"
#define kADMDrawerSuiteVersion4 4

// -----------------------------------------------------------------------------

typedef struct ADMDrawerSuite4
{

	// *** This suite is FROZEN.
	// *** It shipped with Acrobat 5.0.

	// port accessor

	ASPortRef ASAPI (*GetPortRef)(ADMDrawerRef inDrawer);

	// clear entire area of drawer
	
	void ASAPI (*Clear)(ADMDrawerRef inDrawer);

	// bounds accessor
	
	void ASAPI (*GetBoundsRect)(ADMDrawerRef inDrawer, ASRect* outBoundsRect);

	// clipping
	
	void ASAPI (*GetClipRect)(ADMDrawerRef inDrawer, ASRect* outClipRect);
	void ASAPI (*SetClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);
	void ASAPI (*IntersectClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);
	void ASAPI (*UnionClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);
	void ASAPI (*SubtractClipRect)(ADMDrawerRef inDrawer, const ASRect* inClipRect);

	void ASAPI (*SetClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);

	void ASAPI (*IntersectClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);

	void ASAPI (*UnionClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);

	void ASAPI (*SubtractClipPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);
	
	// port origin
	
	void ASAPI (*GetOrigin)(ADMDrawerRef inDrawer, ASPoint* outOrigin);
	void ASAPI (*SetOrigin)(ADMDrawerRef inDrawer, const ASPoint* inOrigin);
	
	// drawing state accessors
	
	void ASAPI (*GetRGBColor)(ADMDrawerRef inDrawer, ASRGBColor* outColor);
	void ASAPI (*SetRGBColor)(ADMDrawerRef inDrawer, const ASRGBColor* inColor);
	
	ADMColor ASAPI (*GetADMColor)(ADMDrawerRef inDrawer);
	void ASAPI (*SetADMColor)(ADMDrawerRef inDrawer, ADMColor inColor);
	
	ADMDrawMode ASAPI (*GetDrawMode)(ADMDrawerRef inDrawer);
	void ASAPI (*SetDrawMode)(ADMDrawerRef inDrawer, ADMDrawMode inDrawMode);
	
	ADMFont ASAPI (*GetFont)(ADMDrawerRef inDrawer);
	void ASAPI (*SetFont)(ADMDrawerRef inDrawer, ADMFont inFont);

	// simple shape drawers
	
	void ASAPI (*DrawLine)(ADMDrawerRef inDrawer, const ASPoint* inStartPoint,
				const ASPoint* inEndPoint);
	
	void ASAPI (*DrawPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);

	void ASAPI (*FillPolygon)(ADMDrawerRef inDrawer, const ASPoint* inPoints,
				ASInt32 inNumPoints);
	
	void ASAPI (*DrawRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*FillRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*ClearRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawSunkenRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawRaisedRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*InvertRect)(ADMDrawerRef inDrawer, const ASRect* inRect);
	
	void ASAPI (*DrawOval)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*FillOval)(ADMDrawerRef inDrawer, const ASRect* inRect);

	// AGM access
	
	ADMAGMPortPtr ASAPI (*GetAGMPort)(ADMDrawerRef inDrawer);

	void ASAPI (*DrawAGMImage)(ADMDrawerRef inDrawer,
				const struct _t_ADMAGMImageRecord* inImage,
				const ASFixedMatrix* inMatrix, ASInt32 inFlags);

	// resource drawing
	
	void ASAPI (*DrawResPicture)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,
				ASInt32 inRsrcID, const ASPoint* inTopLeftPoint);

	void ASAPI (*DrawResPictureCentered)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,
				ASInt32 inRsrcID, const ASRect* inRect);
	
	void ASAPI (*DrawRecoloredResPicture)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,
				ASInt32 inRsrcID, const ASPoint* inTopLeftPoint, ADMRecolorStyle inStyle);

	void ASAPI (*DrawRecoloredResPictureCentered)(ADMDrawerRef inDrawer,
				SPPluginRef inPluginRef, ASInt32 inRsrcID, const ASRect* inRect,
				ADMRecolorStyle inStyle);

	// icon drawing

	void ASAPI (*DrawIcon)(ADMDrawerRef inDrawer, ADMIconRef inIcon, const ASPoint* inTopLeftPoint);
	void ASAPI (*DrawIconCentered)(ADMDrawerRef inDrawer, ADMIconRef inIcon, const ASRect* inRect);

	void ASAPI (*DrawRecoloredIcon)(ADMDrawerRef inDrawer, ADMIconRef inIcon,
				const ASPoint* inTopLeftPoint, ADMRecolorStyle inStyle);

	void ASAPI (*DrawRecoloredIconCentered)(ADMDrawerRef inDrawer, ADMIconRef inIcon,
				const ASRect* inRect, ADMRecolorStyle inStyle);

	void ASAPI (*GetResPictureBounds)(ADMDrawerRef inDrawer, SPPluginRef inPluginRef,
				ASInt32 inRsrcID, ASRect* outBoundsRect);

	// text drawing

	ASInt32 ASAPI (*GetTextWidth)(ADMDrawerRef inDrawer, const char* inText);
	ASInt32 ASAPI (*GetTextWidthW)(ADMDrawerRef inDrawer, const ASUnicode* inText);

	void ASAPI (*GetFontInfo)(ADMDrawerRef inDrawer, ADMFontInfo* outFontInfo);
	
	void ASAPI (*DrawText)(ADMDrawerRef inDrawer, const char* inText, const ASPoint* inPoint);
	void ASAPI (*DrawTextW)(ADMDrawerRef inDrawer, const ASUnicode* inText, const ASPoint* inPoint);

	void ASAPI (*DrawTextLeft)(ADMDrawerRef inDrawer, const char* inText, const ASRect* inRect);
	void ASAPI (*DrawTextLeftW)(ADMDrawerRef inDrawer, const ASUnicode* inText, const ASRect* inRect);

	void ASAPI (*DrawTextCentered)(ADMDrawerRef inDrawer, const char* inText, const ASRect* inRect);
	void ASAPI (*DrawTextCenteredW)(ADMDrawerRef inDrawer, const ASUnicode* inText, const ASRect* inRect);

	void ASAPI (*DrawTextRight)(ADMDrawerRef inDrawer, const char* inText, const ASRect* inRect);
	void ASAPI (*DrawTextRightW)(ADMDrawerRef inDrawer, const ASUnicode* inText, const ASRect* inRect);
	
	// standard arrows

	void ASAPI (*DrawUpArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawDownArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawLeftArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);
	void ASAPI (*DrawRightArrow)(ADMDrawerRef inDrawer, const ASRect* inRect);

	// drawer creation/destruction

	ADMDrawerRef ASAPI (*Create)(ASPortRef inPortRef, const ASRect* inBoundsRect, ADMFont inFont);	
	void ASAPI (*Destroy)(ADMDrawerRef inDrawer);

	// image drawing

	void ASAPI (*DrawADMImage)(ADMDrawerRef inDrawer, ADMImageRef inImage,
				const ASPoint* inTopLeftPoint);

	void ASAPI (*DrawADMImageCentered)(ADMDrawerRef inDrawer, ADMImageRef inImage,
				const ASRect* inRect);	

	// platform port accessors

	ASPortRef ASAPI (*GetADMWindowPort)(ASWindowRef inWindowRef);
	void ASAPI (*ReleaseADMWindowPort)(ASWindowRef inWindowRef, ASPortRef inPort);

	// font information

	void ASAPI (*GetThisFontInfo)(ADMFont inFont, ADMFontInfo* outFontInfo);

	// text drawing

	void ASAPI (*DrawTextInABox)(ADMDrawerRef inDrawer, const ASRect* inRect,
				const char* inText);
	void ASAPI (*DrawTextInABoxW)(ADMDrawerRef inDrawer, const ASRect* inRect,
				const ASUnicode* inText);

	// update area accessor

	void ASAPI (*GetUpdateRect)(ADMDrawerRef inDrawer, ASRect* outRect);

	// text measurement
	
	ASInt16 ASAPI (*GetTextRectHeight)(ADMDrawerRef inDrawer, ASInt16 inWidth,
				const char* inText);
	ASInt16 ASAPI (*GetTextRectHeightW)(ADMDrawerRef inDrawer, ASInt16 inWidth,
				const ASUnicode* inText);

}
ADMDrawerSuite4;

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

#endif
