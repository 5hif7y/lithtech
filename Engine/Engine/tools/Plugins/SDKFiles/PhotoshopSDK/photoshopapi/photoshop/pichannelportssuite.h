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
 *	PIChannelPortsSuite.h
 *
 * Copyright 1986-2000 Adobe Systems Incorporated.
 * All Rights Reserved.
 *
 * Purpose:
 *	Photoshop ChannelPorts Suite for Plug-ins
 *
 * Distribution:
 *	PUBLIC
 *
 * Version history:
 *	1.0.0 4/1/1998	JF	First Version.
 *		Created by Josh Freeman.
 */
// $Id: //photoshop/8.0/photoshop/sharedinterfaces/PIChannelPortsSuite.h#1 $ 
// $DateTime: 2003/07/08 15:07:21 $ 
// $Change: 75352 $ 
// $Author: byer $ 

#ifndef __PIChannelPortsSuite__
#define __PIChannelPortsSuite__

#if PRAGMA_ONCE
#pragma once
#endif

#include "pigeneral.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define kPSChannelPortsSuite 			"Photoshop ChannelPorts Suite for Plug-ins"

/******************************************************************************/

#define kPSChannelPortsSuiteVersion2			2	// we're using version 2
#define kPSChannelPortsSuiteVersion3			3	// ...or 3

/******************************************************************************/

typedef struct PSChannelPortsSuite1
	{
	// Determine how many levels we have. Zero if the port is invalid.
	SPAPI SPErr (*CountLevels)
		(
		PIChannelPort port, 
		int32 *count
		);

	// Get the depth at a given level. Zero if the port or level is
	// invalid.
	SPAPI SPErr (*GetDepth)
		(
		PIChannelPort port, 
		int32 level, 
		int32 *depth
		);

	// Get the bounds for the pixel data. Return an empty rectangle if
	// the parameters are invalid.
	SPAPI SPErr (*GetDataBounds) 
		(
		PIChannelPort port,
		int32 level,
		VRect *bounds
		);

	// Get the bounds to which we can write at a given level.
	SPAPI SPErr (*GetWriteLimit) 
		(
		PIChannelPort port,
		int32 level,
		VRect *writeBounds
		);

	// Get the tiling information at a given level.
	SPAPI SPErr (*GetTilingGrid) 
		(
		PIChannelPort port,
		int32 level,
		VPoint *tileOrigin,
		VPoint *tileSize
		);

	// Find the rectangle used as the downsample source for a particular
	// level in the pyramid. level is set to -1 if no support rectangle
	// exists.
	SPAPI SPErr (*GetSupportRect) 
		(
		PIChannelPort port,
		int32 level, const 
		VRect *bounds,
		int32 *supportLevel, 
		VRect *supportBounds
		);

	// Get the dependent rectangle at a particular level.
	SPAPI SPErr (*GetDependentRect) 
		(
		PIChannelPort port,
		int32 sourceLevel, 
		const VRect *sourceBounds,
		int32 dependentLevel, 
		VRect *dependentBounds
		);

	// Can we read from this port?
	SPAPI SPErr (*CanRead) 
		(
		PIChannelPort port, 
		Boolean *canRead
		);

	// Can we write to this port?
	SPAPI SPErr (*CanWrite) 
		(
		PIChannelPort port, 
		Boolean *canWrite
		);

	// Read pixels from a given level of the port. If the result is
	// noErr, then bounds will reflect the pixels actually read. If
	// it reflects an error, then the value of bounds is undefined.
	SPAPI SPErr (*ReadPixelsFromLevel) 
		(
		PIChannelPort port,
		int32 level, 
		VRect *bounds,	/* IN/OUT */
		const PixelMemoryDesc *destination
		);

	// Write to a level in the pyramid.
	SPAPI SPErr (*WritePixelsToBaseLevel) 
		(
		PIChannelPort port,
		VRect *bounds,	/* IN/OUT */
		const PixelMemoryDesc *source
		);

	// Read scaled data from the pyramid. Adjusts readRect to reflect the area
	// actually read.
	SPAPI SPErr (*ReadScaledPixels) 
		(
		PIChannelPort port,
		VRect *readRect,	/* IN/OUT */
		const PSScaling *scaling,
		const PixelMemoryDesc *destination
		);

	// If we just want to find out what level would be used for a given scaling,
	// we can use the following routine. sourceLevel and sourceRect describe
	// where in the pyramid we will be reading from. sourceScalingBounds is the
	// bounds used for scaling from this level to the final result.
	SPAPI SPErr (*FindSourceForScaledRead) 
		(
		PIChannelPort port,
		const VRect *readRect,
		const PSScaling *scaling,
		int32 dstDepth,
		int32 *sourceLevel,
		VRect *sourceRect,
		VRect *sourceScalingBounds
		);

	// Create a pixel array and the port to go with it.
	SPAPI SPErr (*New) 
		(
		PIChannelPort *port,
		const VRect *rect,
		int32 depth,
		Boolean globalScope
		);

	// Dispose of a port and set the port to NULL.
	SPAPI SPErr(* Dispose)
		(
		PIChannelPort *port
		);
	
	/*********************************************************************/

	// The following routines are new in version 3.

	// We wrap a variety of filtering operations into two callbacks. One
	// checks to see whether an operation is supported and the other
	// actually applies the operation from a source port to a destination
	// port. We do this rather than adding new routines for each operation
	// because it keeps us from having to repeatedly revise the suite
	// number. The list of operations is provided in PIChannelPortOperations.h.

	SPAPI SPErr(*SupportsOperation)
		(
		const char *operation,
		Boolean *supported
		);

	SPAPI SPErr(*ApplyOperation)
		(
		const char *operation,
		PIChannelPort sourcePort,
		PIChannelPort destinationPort,
		PIChannelPort maskPort,
		void *parameters,	// The exact structure of which will depend on the operation
		VRect *rect			// A bounding rectangle. Could also be an output.
		);

	// We also support plug-ins adding and removing operations.

	SPAPI SPErr(*AddOperation) (const char *operation,
								SPErr (*proc) (PIChannelPort, PIChannelPort, PIChannelPort,
											   void *, VRect *, void *refCon),
								void *refCon);

	SPAPI SPErr(*RemoveOperation) (const char *operation, void **refCon);

	// We provide support for using the copy-on-write mechanism. The base port must
	// be frozen. writeLimit can be NULL in which case writing will be allowed
	// everywhere.

	SPAPI SPErr (*NewCopyOnWrite) (PIChannelPort *result,
								   PIChannelPort basePort,
								   VRect *writeLimit,
								   Boolean globalScope);

	// Freeze the data associated with a channel port. This should generally only
	// be used in conjunction with ports allocated via NewCopyOnWrite.

	SPAPI SPErr (*Freeze) (PIChannelPort port);

	// We can restore an area within a copy-on-write port to its initial state.
	// Passing null will restore everything.

	SPAPI SPErr (*Restore) (PIChannelPort port, VRect *area);

	} PSChannelPortsSuite1;


#ifdef __cplusplus
}
#endif


#endif	// PIChannelPortsSuite
