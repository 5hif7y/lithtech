// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2002 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
/*
 * Name:
 *	PIHandleSuite.h
 *
 * Copyright 1986-2000 Adobe Systems Incorporated.
 * All Rights Reserved.
 *
 * Purpose:
 *	Photoshop Handle Suite for Plug-ins.
 *
 * Distribution:
 *	PUBLIC
 *
 * Version history:
 *	1.0.0 4/1/1998	JF	First Version.
 *		Created by Josh Freeman.
 */
// $Id: //photoshop/8.0/photoshop/sharedinterfaces/PIHandleSuite.h#1 $ 
// $DateTime: 2003/07/08 15:07:21 $ 
// $Change: 75352 $ 
// $Author: byer $ 

#ifndef __PIHandleSuite__
#define __PIHandleSuite__

#if PRAGMA_ONCE
#pragma once
#endif

#include "pigeneral.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define kPSHandleSuite 			"Photoshop Handle Suite for Plug-ins"

//-------------------------------------------------------------------------------
//	Handle suite typedefs
//-------------------------------------------------------------------------------

typedef MACPASCAL void (*SetPIHandleLockProc) (Handle h, Boolean lock, Ptr *address, Boolean *oldLock);

//-------------------------------------------------------------------------------
//	Handle suite version 1
//-------------------------------------------------------------------------------

#define kPSHandleSuiteVersion1		1
#define kPSHandleSuiteVersion2		2

typedef struct
{
	NewPIHandleProc New;
	DisposePIHandleProc Dispose;
	SetPIHandleLockProc SetLock;
	GetPIHandleSizeProc GetSize;
	SetPIHandleSizeProc SetSize;
	RecoverSpaceProc RecoverSpace;
	
} PSHandleSuite1;

typedef struct
{
	NewPIHandleProc New;
	DisposePIHandleProc Dispose;
	DisposeRegularHandlePIHandleProc DisposeRegularHandle;
	SetPIHandleLockProc SetLock;
	GetPIHandleSizeProc GetSize;
	SetPIHandleSizeProc SetSize;
	RecoverSpaceProc RecoverSpace;
	
} PSHandleSuite2;

/******************************************************************************/

#ifdef __cplusplus
}
#endif


#endif	// PIHandleSuite
