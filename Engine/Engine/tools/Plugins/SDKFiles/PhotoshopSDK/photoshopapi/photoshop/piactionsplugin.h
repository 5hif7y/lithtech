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
//
//	File:
//		PIActionsPlugIn.h	
//
//	Copyright 1991-2000, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Distribution:
//		PUBLIC
//
//	Description:
//		This file describes version 1.0 of Photoshop's actions plug-in module
//		interface.
//
//	Use:
//		Include in Photoshop Action plug-ins.
//		
//
//	Version history:
//		Version 1.0.0	1/15/98		CM		Initial compilation.
//		
//		
//
//-------------------------------------------------------------------------------
// $Id: //photoshop/8.0/photoshop/sharedinterfaces/PIActionsPlugIn.h#1 $ 
// $DateTime: 2003/07/08 15:07:21 $ 
// $Change: 75352 $ 
// $Author: byer $ 

#ifndef __PIActionsPlugIn__
#define __PIActionsPlugIn__

#if PRAGMA_ONCE
#pragma once
#endif

#include "piactions.h"
#include "pigeneral.h"
#include "SPBasic.h"
#include "SPMData.h"

/********************************************************************************/


typedef struct PSActionsPlugInMessage
	{
	SPMessageData			d;
	PIActionParameters		*actionParameters;	/* For recording and playback */
	} PSActionsPlugInMessage;

#endif // __PIActionsPlugIn_h__
