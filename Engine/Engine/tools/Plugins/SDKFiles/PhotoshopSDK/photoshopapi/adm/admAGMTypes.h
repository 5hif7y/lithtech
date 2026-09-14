/***********************************************************************/
/*                                                                     */
/* ADMAGMTypes.h                                                       */
/* Abstract references to AGM types for ADM interfaces                 */
/*                                                                     */
/* ADOBE SYSTEMS INCORPORATED                                          */
/* Copyright 1999-2001 Adobe Systems Incorporated                      */
/* All Rights Reserved                                                 */
/*                                                                     */
/* NOTICE:  Adobe permits you to use, modify, and distribute this file */
/* in accordance with the terms of the Adobe license agreement         */
/* accompanying it. If you have received this file from a source other */
/* than Adobe, then your use, modification, or distribution of it      */
/* requires the prior written permission of Adobe.                     */
/*                                                                     */
/* Started by Eric Scouten, 06/24/1999                                 */
/*                                                                     */
/***********************************************************************/

#ifndef _ADMAGMTYPES_H_
#define _ADMAGMTYPES_H_

#include "asTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _t_ADMAGMColrSpace ADMAGMColorSpace;
typedef int ADMAGMColorSpaceFamily;
typedef struct _t_ADMAGMRasterPort ADMAGMRasterPort;
typedef struct _t_ADMAGMPort ADMAGMPort;
typedef ADMAGMPort* ADMAGMPortPtr;
typedef struct _t_ADMAGMColorTab ADMAGMColorTab;
typedef struct _t_ADMAGMImageRecord ADMAGMImageRecord;

typedef enum _ADMLineCap {
	kButtCap = 0,				/* Butt line caps */
	kRoundCap,					/* Round line caps */
	kProjectingCap				/* Projecting line caps */
} ADMLineCap;

struct _t_ADMAGMColorTab	{
	ASInt32	numColors;
	void*	theColors;
};

typedef struct _t_ADMAGMInt16Rect {
	ASInt16	xMin;
	ASInt16	yMin;
	ASInt16	xMax;
	ASInt16	yMax;
} ADMAGMInt16Rect;

struct _t_ADMAGMImageRecord	{
	ADMAGMInt16Rect	bounds;
	void*			baseAddr;
	ASInt32			byteWidth;
	ASInt16			colorSpace;
	ASInt16			bitsPerPixel;
	float*		decodeArray;
	ADMAGMColorTab	colorTab;
};

#ifdef __cplusplus
}
#endif

#endif // _ADMAGMTYPES_H_
