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
//		PIPicker.h	
//
//	Copyright 1991-2000, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Distribution:
//		PUBLIC
//
//	Description:
//		This file describes version 1.0 of Photoshop's color picker plug-in module
//		interface.
//
//	Use:
//		Use in all color picker plug-ins.
//		
//
//	Version history:
//		1.0.0	10/9/1997	Ace		Initial compilation.
//		
//-------------------------------------------------------------------------------
// $Id: //photoshop/8.0/photoshop/sharedinterfaces/PIPicker.h#1 $ 
// $DateTime: 2003/07/08 15:07:21 $ 
// $Change: 75352 $ 
// $Author: byer $ 

#ifndef __PIPicker__
#define __PIPicker__

#if PRAGMA_ONCE
#pragma once
#endif

#include "piactions.h"
#include "pigeneral.h"
#include "SPBasic.h"

/********************************************************************************/

/* Operation selectors */

#define pickerSelectorAbout 	 0
#define pickerSelectorPick		 1

/********************************************************************************/

/* Error return values. The plug-in module may also return standard Macintosh
   operating system error codes, or report its own errors, in which case it
   can return any positive integer. */
 
#define pickerBadParameters -30800	/* "of a problem with the plug-in module interface" */

/********************************************************************************/

typedef struct PickParms
	{
	int16 sourceSpace;				// The colorspace the original color is in
	int16 resultSpace;				// The colorspace of the returned result
									// Can be plugIncolorServicesChosenSpace
	unsigned16 colorComponents[4];	// Will contain the original color when the
									// plugin is called.  The plugin will put
									// the returned color here as well.
	Str255 *pickerPrompt;
	}
PickParms;

/********************************************************************************/

typedef struct PIPickerParams
	{
	
	/* We start with the fields that are common to pretty much all of the plug-ins. */
	
	int32		serialNumber;		/* DEPRECATED - Formerly the host serial number.
									   The host will now report
									   zero for the serialNumber. Plug-ins should use 
									   the PropertySuite and specify 'propSerialString2'
									   to get the serial string. */

	TestAbortProc	abortProc;		/* The plug-in module may call this no-argument
									   BOOLEAN function (using Pascal calling
									   conventions) several times a second during long
									   operations to allow the user to abort the operation.
									   If it returns TRUE, the operation should be aborted
									   (and a positive error code returned). */

	ProgressProc	progressProc;	/* The plug-in module may call this two-argument
									   procedure periodically to update a progress
									   indicator.  The first parameter is the number
									   of operations completed; the second is the total
									   number of operations. */

	OSType		hostSig;			/* Creator code for host application */
	HostProc	hostProc;			/* Host specific callback procedure */

	BufferProcs *bufferProcs;		/* The host buffer procedures. */
	
	ResourceProcs *resourceProcs;	/* The host plug-in resource procedures. */
	
	ProcessEventProc processEvent;	/* Pass event to the application. */
	
	DisplayPixelsProc displayPixels;/* Display dithered pixels. */

	HandleProcs	*handleProcs;		/* Platform independent handle manipulation. */

	ColorServicesProc colorServices; /* Routine to access color services. */
	
	ImageServicesProcs *imageServicesProcs;
									/* Suite of image processing callbacks. */
			
	PropertyProcs	*propertyProcs;	/*	Routines to query and set document and
										view properties. The plug-in needs to
										dispose of the handle returned for
										complex properties (the plug-in also
										maintains ownership of handles for
										set properties.
									*/
									
	ChannelPortProcs *channelPortProcs;
									/* Suite for passing pixels through channel ports. */
			
	PIDescriptorParameters	*descriptorParameters;	/* For recording and playback */
	Str255					*errorString;			/* For silent and errReportString */
	
	PlugInMonitor	monitor;		/* Information on current monitor */

	void 		*platformData;		/* Platform specific information. */

	char reserved [4];				/* 4 bytes of reserved space. */

	/* We follow this with the fields specific to this type of plug-in. */

	PickParms pickParms;
	
	SPBasicSuite 	*sSPBasic;		/* SuitePea basic suite */
	void			*plugInRef;		/* plugin reference used by SuitePea*/
	
	/* And, of course, space for future expansion. */
	
	char reservedBlock [252];
			
	}
PIPickerParams, *PickerRecordPtr;

/********************************************************************************/

#endif // __PIPicker_h__
