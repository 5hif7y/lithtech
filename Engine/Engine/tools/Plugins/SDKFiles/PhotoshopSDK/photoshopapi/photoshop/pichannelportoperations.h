// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2002 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this 
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
//-------------------------------------------------------------------
/*
 * Name:
 *	PIChannelPortOperations.h
 *
 * Copyright 1986-2000 Adobe Systems Incorporated.
 * All Rights Reserved.
 *
 * Purpose:
 *	Documents supported imaging operations on channel ports.
 *
 * Distribution:
 *	PUBLIC
 */
// $Id: //photoshop/8.0/photoshop/sharedinterfaces/PIChannelPortOperations.h#1 $ 
// $DateTime: 2003/07/08 15:07:21 $ 
// $Change: 75352 $ 
// $Author: byer $ 

#ifndef __PIChannelPortOperations__
#define __PIChannelPortOperations__

#if PRAGMA_ONCE
#pragma once
#endif

#include "pigeneral.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

/* Gaussian blur from the source to the destination. */

#define PSChannelPortGaussianBlurFilter "Photoshop Gaussian Blur Filter"

struct PSGaussianBlurParameters
	{
	Fixed radius;	// Radius in pixels using 16.16 fixed point values. 0 to 250 supported.
	int32 padding;	// 0-255 for constant padding. -1 for edge replication.
	};

/******************************************************************************/

/* Min, Max, and Median filters. */

#define PSChannelPortMinimumFilter "Photoshop Minimum Filter"
#define PSChannelPortMaximumFilter "Photoshop Maximum Filter"
#define PSChannelPortMedianFilter  "Photoshop Median Filter"

struct PSMinMaxMedianParameters
	{
	int32 radius;	// Radius in pixels. 1-16 supported for median. 1-10 for min and max.
	int32 padding;	// 0-255 for constant padding. -1 for edge replication.
	};

/******************************************************************************/

/* Copy source to destination. If rect is specified it will define the bounds
to process. It will be restricted to the actual writable bounds. It ignores
the mask parameter. */

#define PSChannelPortCopyPixels "Photoshop Copy Pixels"

/******************************************************************************/

/* Set pixels to a given value. Only implemented for depth 8. */

#define PSChannelPortSetPixels "Photoshop Set Pixels"

struct PSChannelPortSetPixelsParameters
	{
	int32 value;	// 0-255
	};

/******************************************************************************/


#ifdef __cplusplus
}
#endif


#endif	// PIChannelPortOperations
