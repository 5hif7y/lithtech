// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2001 Adobe Systems Incorporated
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
 *	PIGetPathSuite.h
 *
 * Copyright 1986-2000 Adobe Systems Incorporated.
 * All Rights Reserved.
 *
 * Purpose:
 *	This file contains all the public definitions and
 *	structures related to user interface elements.
 *
 *	Use the GetPath suite to get the path from FSSpec
 *	
 *
 * Distribution:
 *	PUBLIC
 *
 * Version history:
 *		Version 1.0.0	2/9/1999	SAK		Created for Photoshop 5.5.
 *			Created by Sarah Kong.
 */
// $Id: //photoshop/8.0/photoshop/sweetpeasuites/api/PIGetPathSuite.h#1 $ 
// $DateTime: 2003/07/08 15:07:21 $ 
// $Change: 75352 $ 
// $Author: byer $ 

#ifndef __PIGetPathSuite__
#define __PIGetPathSuite__

#if PRAGMA_ONCE
#pragma once
#endif

//-------------------------------------------------------------------------------
//	Includes.
//-------------------------------------------------------------------------------
#include "pigeneral.h"

//-------------------------------------------------------------------------------
//	C++ wrapper.
//-------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------
//	UI GetPath suite.
//-------------------------------------------------------------------------------
#define kPSGetPathSuite 			"Photoshop GetPath Suite for Plug-ins"

//-------------------------------------------------------------------------------
//	
//-------------------------------------------------------------------------------
typedef SPAPI void (*GetPathNameProc) (SPPlatformFileSpecification* fileSpec, char* path, int16 maxLength );

//-------------------------------------------------------------------------------
//	 GetPath suite version 1.
//-------------------------------------------------------------------------------

#define kPSGetPathSuiteVersion1		1

typedef struct 
	{
	GetPathNameProc 		GetPathName;
	} PSGetPathSuite1;


//-------------------------------------------------------------------------------
//	C++ wrapper.
//-------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

//-------------------------------------------------------------------------------
#endif	// PIGetPathSuite
