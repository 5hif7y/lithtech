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
//		PIExport.h
//
//	Copyright 1990-1991, Thomas Knoll.
//	Copyright 1991-2000, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Distribution:
//		PUBLIC
//
//	Description:
//		This file describes version 4.0 of Photoshop's Export module interface.		
//
//	Use:
//		Include in all Export plug-ins.
//		
//
//	Version history:
//		Version 1.0.0	1/1/90	TK		Initial compilation.
//		Version 2.0.0	10/1/97	Ace		Updated for Photoshop 4.0.
//		
//-------------------------------------------------------------------------------
// $Id: //photoshop/8.0/photoshop/sharedinterfaces/PIExport.h#1 $ 
// $DateTime: 2003/07/08 15:07:21 $ 
// $Change: 75352 $ 
// $Author: byer $ 

#ifndef __PIExport__
#define __PIExport__

#if PRAGMA_ONCE
#pragma once
#endif

#include "piactions.h"
#include "pigeneral.h"
#include "SPBasic.h"

/* Operation selectors */

#define exportSelectorAbout    0
#define exportSelectorStart    1
#define exportSelectorContinue 2
#define exportSelectorFinish   3
#define exportSelectorPrepare  4

/* Error return values. The plug-in module may also return standard Macintosh
   operating system error codes, or report its own errors, in which case it
   can return any positive integer. */

#define exportBadParameters -30200	/* "a problem with the export module interface" */
#define exportBadMode		-30201	/* "the export module does not support <mode> images" */

/******************************************************************************/

#define PIExpFlagsProperty		0x65787066 /* 'expf' <Set> Export flags */

/******************************************************************************/

/* Export flag indices. */

#define PIExpSupportsTransparency 0

/******************************************************************************/
/* Pragma to byte align structures; only for Borland C */

#if defined(__BORLANDC__)
#pragma option -a-
#endif

/******************************************************************************/

#if PRAGMA_STRUCT_ALIGN
#pragma options align=mac68k
#endif

typedef struct ExportRecord
	{

	int32			serialNumber;	/* DEPRECATED - Formerly the host serial number.
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

	int32			maxData;		/* Maximum number of bytes that should be
									   requested at once (the plug-in should reduce
									   its requests by the size any large buffers
									   it allocates). The plug-in may reduce this
									   value in the exportSelectorPrepare routine. */

	int16			imageMode;		/* Image mode */
	Point			imageSize;		/* Size of image -- see also imageSize32 */
	int16			depth;			/* Bits per sample, currently will be 1 or 8 */
	int16			planes; 		/* Samples per pixel */

	Fixed			imageHRes;		/* Pixels per inch */
	Fixed			imageVRes;		/* Pixels per inch */

	LookUpTable		redLUT; 		/* Red LUT, only used for Indexed Color images */
	LookUpTable		greenLUT;		/* Green LUT, only used for Indexed Color images */
	LookUpTable		blueLUT;		/* Blue LUT, only used for Indexed Color images */

	Rect			theRect;		/* Rectangle requested, set to empty rect when done
										see also theRect32 */
	
	int16			loPlane;		/* First plane requested */
	int16			hiPlane;		/* Last plane requested */

	void *			data;			/* A pointer to the requested image data */
	int32			rowBytes;		/* Spacing between rows */

	Str255 			filename;		/* Document file name */
	int16			vRefNum;		/* Volume reference number, or zero if none */
	Boolean 		dirty;			/* Changes since last saved flag. The plug-in may clear
									   this field to prevent prompting the user when
									   closing the document. */

	Rect			selectBBox; 	/* Bounding box of current selection, or an empty
									   rect if there is no current selection. 
									   see also selectBBox32 */

	OSType			hostSig;		/* Creator code for host application */
	HostProc		hostProc;		/* Host specific callback procedure */

	Handle			duotoneInfo;	/* Handle to duotone information. */

	int16			thePlane;		/* Currently selected channel,
									   or -1 if a composite color channel,
									   or -2 if all channels. */

	PlugInMonitor	monitor;		/* Information on current monitor */

	void *			platformData;	/* Platform specific hook. */

	BufferProcs *	bufferProcs;	/* Host buffer maintenance procedures. */
	
	ResourceProcs *	resourceProcs;	/* Host plug-in resource procedures. */
	
	ProcessEventProc processEvent;	/* Pass events to the application. */
	
	DisplayPixelsProc displayPixels;/* Display dithered pixels. */

	HandleProcs		*handleProcs;	/* Platform independent handle manipulation. */

	/* New in 3.0. */

	ColorServicesProc colorServices; /* Routine to access color services. */
	
	GetPropertyProc	getPropertyObsolete; /* Use the suite if available */
									   
	AdvanceStateProc advanceState;	/* Allowed from Start and Continue. */
	
	int16			layerPlanes;
	int16			transparencyMask;
	int16			layerMasks;				/* Zero. */
	int16			invertedLayerMasks;		/* Zero. */
	int16			nonLayerPlanes;
									   
	/* Note that the layer mask fields are necessarily zero in 3.0.4 since
	   the export module receives the merged data and the merged data doesn't
	   really have a layer mask associated with it. */
									   
	/* New in 3.0.4. */
	
	ImageServicesProcs *imageServicesProcs;
										/* Suite of image processing callbacks. */
	
	int16				tileWidth;		/* The width of the tiles. Zero if not set. */
	int16				tileHeight;		/* The height of the tiles. Zero if not set. */
	
	Point				tileOrigin;		/* The origin point for the tiles. */

	PropertyProcs	*propertyProcs;	/*	Routines to query and set document and
										view properties. The plug-in needs to
										dispose of the handle returned for
										complex properties (the plug-in also
										maintains ownership of handles for
										set properties.
									*/
	
	/* New in 4.0 */
	
	PIDescriptorParameters	*descriptorParameters;	/* For recording and playback */
	Str255					*errorString;			/* For silent and errReportString */

	ChannelPortProcs *channelPortProcs;
									/* Suite for passing pixels through channel ports. */
			
	ReadImageDocumentDesc *documentInfo;	/* The document info for the document being filtered. */
	
	/* New in 5.0 */
	
	SPBasicSuite	*sSPBasic; 		/* SuitePea basic suite */
	void			*plugInRef;		/* plugin reference used by SuitePea*/
	
	int32			transparentIndex;	/* if IndexedColor, and between 0 and 255, this is the index of the transparent color (for GIF) */
	
	Handle			iCCprofileData;		/* Handle containing the ICC profile for the image. (NULL if none)
											Photoshop allocates the handle using Photoshop's handle suite
											The handle is unlocked while calling the plug-in
											The handle will be valid from Start to Finish
											Photoshop will free the handle after Finish
										*/
	int32			iCCprofileSize;		/* size of profile. */
	
	int32			canUseICCProfiles;	/* non-zero if the host can accept/export ICC profiles 
											if this is zero, you'd better not set or dereference iCCprofileData :-)
										*/

	/* New in 5.5 */
	
	int32			lutCount;			/* Number of entries in the indexed color table.
										   This will include the transparent index if any. */
	
	/* New in 8.0 */
	
	int32		HostSupports32BitCoordinates;	/* set by host if the host supports 32 bit plugin API */
	
	int32		PluginUsing32BitCoordinates;	/* set to nonzero by the plugin if it is
													using the 32 bit fields */
	
	VPoint		imageSize32;					/* Size of image in 32 bit coordinates 
													replaces imageSize */
	
	VRect		theRect32;						/* Rectangle being returned in 32 bit coordinates 
													replaces theRect */
	
	VRect		selectBBox32;					/* Rectangle being returned in 32 bit coordinates 
													replaces theRect */

	/* Reserved Space for Expansion -- decrement when you add stuff */
	
	char			reserved [106]; /* Set to zero */

	}
ExportRecord, *ExportRecordPtr;

#if PRAGMA_STRUCT_ALIGN
#pragma options align=reset
#endif

/******************************************************************************/
/* turn off the byte align pragma back to its original state; only for Borland C */

#if defined(__BORLANDC__)
#pragma option -a.
#endif

/******************************************************************************/

#endif // __PIExport_h__
