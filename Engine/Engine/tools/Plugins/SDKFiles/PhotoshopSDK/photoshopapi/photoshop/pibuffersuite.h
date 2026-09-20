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
 *	PIBufferSuite.h
 *
 * Copyright 1986-2000 Adobe Systems Incorporated.
 * All Rights Reserved.
 *
 * Purpose:
 *	Photoshop Buffer Suite for Plug-ins
 *
 * Distribution:
 *	PUBLIC
 *
 * Version history:
 *	1.0.0 4/1/1998	JF	First Version.
 *		Created by Josh Freeman.
 */
// $Id: //photoshop/8.0/photoshop/sharedinterfaces/PIBufferSuite.h#1 $ 
// $DateTime: 2003/07/08 15:07:21 $ 
// $Change: 75352 $ 
// $Author: byer $ 

#ifndef __PIBufferSuite__
#define __PIBufferSuite__

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define kPSBufferSuite 			"Photoshop Buffer Suite for Plug-ins"

//-------------------------------------------------------------------------
//	Buffer suite typedefs 
//-------------------------------------------------------------------------

typedef SPAPI Ptr (*BufferNewProc) (unsigned32 *pRequestedSize,	/* IN/OUT: Actual size returned here. */
							  unsigned32 minimumSize);	
							  				
    /* Attempts to allocate the number of bytes specified by the
    /  variable pointed to by the requested size parameter and if
    /  this many bytes cannot be allocated, then the largest possible
    /  number (greater than minimumSize) will be allocated and the
    /  number of bytes actually allocated will be returned in the
    /  requestedSize variable.  NULL may be passed as the pointer-
    /  to-requestedSize parameter, in which case the specified minimum
    /  number of bytes will be allocated.  If this minimum number
    /  of bytes cannot be allocated, then the function will fail
    /  and return NULL. */


typedef SPAPI void (*BufferDisposeProc)	(Ptr *ppBuffer);	
										  
    /* Disposes of the buffer and sets the variable that refers to it
    /  to NULL. Does nothing if the buffer pointer is already NULL. */


typedef SPAPI unsigned32 (*BufferGetSizeProc) (Ptr pBuffer);

    /* Returns the size of the buffer OR zero if the buffer pointer
    /  is invalid. */
    

typedef SPAPI unsigned32 (*BufferGetSpaceProc) (void);
	/* Returns the amount of remaining space available - may not be contiguous */

//-------------------------------------------------------------------------
//	Buffer suite version 1 
//-------------------------------------------------------------------------

#define kPSBufferSuiteVersion1		1

typedef struct 
{
    BufferNewProc		New;
    BufferDisposeProc	Dispose;
    BufferGetSizeProc	GetSize;
    BufferGetSpaceProc	GetSpace;

} PSBufferSuite1;


/******************************************************************************/

#ifdef __cplusplus
}
#endif


#endif	// PIBufferSuite
