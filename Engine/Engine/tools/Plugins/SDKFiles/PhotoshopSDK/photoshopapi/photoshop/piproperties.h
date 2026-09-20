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
//		PIProperties.h
//
//	(c) Copyright 1994-2000, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Distribution:
//		PUBLIC
//
//	Description:
//		This file contains the public definitions and structures
//		for the properties callback suite.
//
//	Use:
//		If you are working with any properties via the properties
//		or action get mechanisms, this documents what is available
//		and its basic format.
//
//	Version history:
//		Version 1.0.0	1/1/1994	MH		Created.
//			Created for Photoshop 3.0.
//
//		Version 1.5.0	1/1/1996	MP		Photoshop 4.0.
//			Added Photoshop 4.0 properties.
//
//		Version 2.0.0	4/1/1997	Ace		Photoshop 5.0.
//			Updated for new headers for Photoshop 5.0.
//
//-------------------------------------------------------------------------------
// $Id: //photoshop/8.0/photoshop/sharedinterfaces/PIProperties.h#2 $ 
// $DateTime: 2003/07/10 10:49:58 $ 
// $Change: 75607 $ 
// $Author: byer $ 

#ifndef __PIProperties_h__ // Already defined?
#define __PIProperties_h__

#if PRAGMA_ONCE
#pragma once
#endif

//-------------------------------------------------------------------------------

// Get the number of channels.  Returns the number of channels in the simple
// property field:   
#define propNumberOfChannels  'nuch'

// Returns a handle containing the characters for the name of the channel. (Complex):
#define propChannelName		  'nmch'

// Returns a handle containing the unicode characters for the name of the channel. (Complex):
#define propUnicodeChannelName 'unch'

// Returns the lock bits for the target layer (Simple):
#define propTargetLayerLock	  'tllk'

#define propTargetLayerLockNone					0x0
#define	propTargetLayerLockTransparency			0x1 << 0
#define	propTargetLayerLockComposite			0x1 << 1
#define	propTargetLayerLockPosition				0x1 << 2

// Returns the image mode using the plugInMode constants.  (Simple):
#define propImageMode		  'mode'

// Returns the number of channels including the work path. (Simple):
#define propNumberOfPaths	  'nupa'

// Returns the name of the indexed path (zero-based). (Complex):
#define propPathName          'nmpa'

// Returns the unicode name of the indexed path. (Complex):
#define propUnicodePathName	  'unmp'

// Returns the contents of the indexed path (zero-based). (Complex):
#define propPathContents      'path'

// Returns the contents of the path as AI data (zero-based). Only
// valid for getting properties. (Complex):
#define propPathContentsAI	  'paAI'

// The index of the work path.  -1 if no work path. (Simple):
#define propWorkPathIndex     'wkpa'

// The index of the clipping path.  -1 if none. (Simple):
#define propClippingPathIndex 'clpa'

// The index of the target path.  -1 if none. (Simple):
#define propTargetPathIndex   'tgpa'

// The file meta information is an IPTC-NAA record (Complex, Modifiable):
#define propCaption			  'capt'

// The file meta information is an IPTC-NAA record (Complex, Modifiable):
#define propXMP			  	  'xmpd'

// The big nudge distance. Horizontal and vertical components. These are
// represented as a 16.16 values. (Simple, Modifiable):
#define propBigNudgeH		  'bndH'
#define propBigNudgeV		  'bndV'

// The current interpolation method: 1 = point sample, 2 = bilinear, 3 = bicubic
// (Simple):
#define propInterpolationMethod 'intp'

// The current ruler units. (Simple):
#define propRulerUnits		    'rulr'

// Ruler units will be one from this enum:
typedef enum
	{
	kRulerPixels,
	kRulerInches,
	kRulerCm,
	kRulerMillimeters,			// Type units in Japan (Q/Ha saved for 7)
	kRulerPoints,
	kRulerPicas,
	kRulerPercent
	} RulerUnits;

// The current ruler origin.  Horizontal and vertical components.  These are
// represented as 16.16 values. (Simple, Modifiable):
#define propRulerOriginH		'rorH'
#define propRulerOriginV		'rorV'

// The current major grid rules, in inches, unless propRulerUnits is pixels, and
// then pixels.  Represented as 16.16 value. (Simple, Modifiable):
#define propGridMajor			'grmj'

// The current number of grid subdivisions per major rule (Simple, Modifiable):
#define propGridMinor			'grmn'

// The serial number string shown to the user. (Complex):
// NOTE: DEPRECATED - Use propSerialString2 below 
#define propSerialString		'sstr'

// The serial number string shown to the user. (Complex):
#define propSerialString2		'sstR'

// The hardware gamma table. (PC Only) (Complex):
#define propHardwareGammaTable	'hgam'

// The interface Color scheme:
#define propInterfaceColor		'iclr'

// The watch suspension level. When non-zero, you can make callbacks to the host
// without fear that the watch will start spinning. It is reset to zero at the
// beginning of each call from the host to the plug-in. (Simple, modifiable):
#define propWatchSuspension		'wtch'

// Whether the current image is considered copywritten (Simple, Modifiable).
// These both access the same property. The first can only be used to
// set the flag.
#define propCopyright			'cpyr'
#define propCopyright2			'cpyR'

// Indicate whether a digital signature or watermark is present (Simple, Modifiable).
// The (c) copyright symbol will appear if this is set, OR if the user has checked
// the copyright property in the File Info dialog.  Do NOT turn the copyright
// flag off, ever.  Use propWatermark to indicate if you've found your digital
// signature or not:
#define propWatermark			'watr'

// The URL for the current image (Complex, Modifiable):
#define propURL					'URL '

// The title of the current document (Complex):
#define propTitle				'titl'

// The width of the current document in pixels (Simple):
#define propDocumentWidth		'docW'

// The height of the current document in pixels (Simple):
#define propDocumentHeight		'docH'

// The slices for the document (Complex, modifiable)
#define propSlices				'slcs'

// The currently selected slice ID (Simple, modifiable)
#define propSelectedSliceID		'slid'

// The currently selected slice index (Simple)
#define propSelectedSliceIndex	'slin'

// Whether the slice numers are show (Simple)
#define propShowSliceNumbers	'slsh'

// The color of the slice lines (Complex)
#define propSliceLineColor		'sllc'

// Whether tool tips are being displayed (Simple)
// 0 - off
// 1 - on
#define propToolTips			'tltp'

// What kind of paint cursors are being used (Simple)
// 1 - standard
// 2 - precise
// 3 - brush size
#define propPaintCursorKind		'PCrK'


// EXIF camera and device data
#define propEXIFData			'EXIF'

// current version of photoshop Major.Minor.Fix
#define propVersion				'vers'

// Is an action play in progress? actions palette or automation plug in(Simple)
#define propPlayInProgress		'plip'

// Get the unicode name of the document
#define propUnicodeName			'unnm'

// Get the unicode name of the document
#define propUniStr255Name		'u25n'

// Get the unicode name of the document without any extension
#define propUnicodeNameWithoutExtension 'neun'

// Global "Don't show again" counter.  This is incremented
// every time these dialogs are reset.  Each dialog that supports this feature
// should maintain its own unsigned32 counter in the preferences file.  It
// should be initialized to zero.  Before showing the dialog, the local counter
// should be compared to the global counter.  If equal, don't show the dialog.
// When the user checks the "Don't show again" checkbox, the local counter
// should be set to the master counter. 
// Read only, unsigned32.
#define propDontShowAgainCount	'DSAC'

// Returns the number of layers in the document. (Simple):
#define propNumberOfLayers		'nuly'

// Returns the index of the target layer. (Simple):
#define propTargetLayerIndex	'tgly'

// Returns the layer name given the index. (Complex):
#define propLayerName			'lynm'

// Returns the layer name as unicode given the index. (Complex):
#define propUnicodeLayerName	'lynu'



//-------------------------------------------------------------------------------

#endif // __PIProperties_h__
