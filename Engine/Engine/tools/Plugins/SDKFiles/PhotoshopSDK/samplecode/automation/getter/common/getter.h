// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2002 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this 
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//
//	File:
//		Getter.h
//
//	Description:
//		Get all the information you can from the Photoshop actions system. This
//		plug in uses the PIUActionUtils routines to dump this information to a 
//		log file called "Getter.log".
//	
//		NOTE: The "Getter.log" file makes it look like you can set the
//		information. This is not true. I am only using the PIUActionUtils routine
//		so I don't have to rewrite the same code into a different format.
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#ifndef __Getter_H__		// Has this not been defined yet?
#define __Getter_H__		// Only include this once by predefining it

#include "PIDefines.h"
#include "SPHost.h"
#include "GetInfoFromPhotoshop.h"
#include "piactionsplugin.h"
#include "PIUSuites.h"
#include "PIUSelect.h"
#include "pihandlesuite.h"
#include "AutomationPlugin.h"

#if __PIMac__
	#include <Folders.h>
	#include <TextUtils.h>
	#include <stdio.h>
#endif

#endif
// end Getter.h