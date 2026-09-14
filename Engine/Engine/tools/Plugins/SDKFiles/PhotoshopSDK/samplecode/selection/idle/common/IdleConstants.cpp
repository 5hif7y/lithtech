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
//-------------------------------------------------------------------------------
//
//	File:
//		IdleConstants.c
//
//	Description:
//		This file contains the constants for the
//		Import module Idle, a very simple example of a module
//		that just idles and sends update events.
//
//	Use:
//		This import module is good as an example of a module
//		that forces the background application to redraw
//		its windows.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "PIDefines.h"
#include "Idle.h"

//-------------------------------------------------------------------------------
//	Definitions -- Constants
//-------------------------------------------------------------------------------

double kDAmountMin = 0.0;
double kDAmountMax = 100000.0;
const uint8 kDPrecision = 4;

//-------------------------------------------------------------------------------
//	Definitions -- Scripting keys
//-------------------------------------------------------------------------------

const DescriptorKeyID keyUseProgress = 'proG';
const DescriptorKeyID keyIdleAmount = 'idlA';
const DescriptorKeyID keySendUpdates = 'sndU';


//-------------------------------------------------------------------------------
//	Globals -- Define global variables for plug-in scope.
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------

// end IdleConstants.cpp
