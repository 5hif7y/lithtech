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
//		PIAbout.h
//
//	Copyright 1992-2000, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Distribution:
//		PUBLIC
//
//	Description:
//		This file describes version 4.0 of Photoshop's plug-in module interface
//		for the about box selector.
//
//	Use:
//		Use in all types of Photoshop plug-ins.
//
//	Version history:
//		1.0.0	10/9/1997	Ace		Initial compilation.
//		
//-------------------------------------------------------------------------------
// $Id: //photoshop/8.0/photoshop/sharedinterfaces/PIAbout.h#1 $ 
// $DateTime: 2003/07/08 15:07:21 $ 
// $Change: 75352 $ 
// $Author: byer $ 

#ifndef __PIAbout__
#define __PIAbout__

#if PRAGMA_ONCE
#pragma once
#endif

#include "SPBasic.h"
#include "SPMData.h"

/******************************************************************************/
/* Pragma to byte align structures; only for Borland C */

#if defined(__BORLANDC__)
#pragma option -a-
#endif

/******************************************************************************/
#define plugInSelectorAbout 	 0

typedef struct AboutRecord 
	{

	void *		platformData;		/* Platform specific information. */
	
	SPBasicSuite *sSPBasic;			/* SuitePea basic suite */
	void		*plugInRef;			/* plugin reference used by SuitePea*/

	char		reserved [244]; 	/* Set to zero */

	}
AboutRecord, *AboutRecordPtr;

/******************************************************************************/
/* turn off the byte align pragma back to its original state; only for Borland C */

#if defined(__BORLANDC__)
#pragma option -a.
#endif

/******************************************************************************/

#endif // __PIAbout_h__ 
