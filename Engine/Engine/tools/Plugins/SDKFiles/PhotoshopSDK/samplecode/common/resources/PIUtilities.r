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
//		PIUtilities.r
//
//
//	Description:
//		This rez file contains standard resources
//		to simplify the use of suites and also some
//		helpful common plug-in resources. 
//
//	Use:
//		PIUtilities is intended to group common resources
//		and functions into a simple library that provides
//		most of the basic plug-in functionality, including
//		error reporting and About boxes.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Defines -- Version information for SDK releases.
//-------------------------------------------------------------------------------

#ifndef VersionString
	#define VersionString 	"CS"
#endif

#ifndef ReleaseString
	#define ReleaseString	"1 (9/2003)"
#endif

#ifndef CurrentYear
	#define CurrentYear		"2003"
#endif

//-------------------------------------------------------------------------------
//	Defines -- Generic resources, such as about boxes and resource IDs.
//	Redefine these if you need to.
//-------------------------------------------------------------------------------

#ifndef StringResource
	#define StringResource	'STR '
#endif

#ifndef ResourceID
	#define ResourceID		16000
#endif

#ifndef AboutID
	#define AboutID			ResourceID
#endif

#ifndef uiID
	#define uiID			ResourceID+1
#endif

#ifndef ADMAboutID
	#define ADMAboutID		uiID+1
#endif

#ifndef AlertID
	#define AlertID			16990
#endif

#ifndef kBadNumberID
	#define kBadNumberID	AlertID
#endif

#ifndef kBadDoubleID
	#define kBadDoubleID	kBadNumberID+1
#endif

#ifndef kNeedVers
	#define kNeedVers		kBadDoubleID+1
#endif

#ifndef kWrongHost
	#define kWrongHost		kNeedVers+1
#endif

#ifndef kSuiteMissing
	#define kSuiteMissing	kWrongHost+1
#endif

//-------------------------------------------------------------------------------
//	Alert strings.
//-------------------------------------------------------------------------------

resource StringResource (kBadNumberID, "Bad number", purgeable)
{
	"An integer is required between ^1 and ^2."
};

resource StringResource (kBadDoubleID, "Bad double", purgeable)
{
	"A decimal number is required between ^1 and ^2."
};

resource StringResource (kNeedVers, "Need vers", purgeable)
{
	"This plug-in requires Adobe Photoshop¨ ^1 or later functionality."
};

resource StringResource (kWrongHost, "Wrong host", purgeable)
{
	"This plug-in is incompatible with this version of the host program."
};

resource StringResource (kSuiteMissing, "Suite missing", purgeable)
{
	/* The host cannot complete this command because... */
	"the required suite \"^0\" is unavailable" /* period is added by host */
};

//-------------------------------------------------------------------------------
//
//	'FltD' resource.  Dynamic resource for Adobe Premiere.
//
//	Descriptor for allowing filters to animate over time. A
//	structure of this type can be added to a 'VFlt', an 'AFlt',
//	or a Photoshop filter to describe the data structure of its
//	data blob. Specify pdOpaque for any non-scalar data in the
//	record, or data that you don't want Premiere to interpolate
//	for you. Make the FltD describe all the bytes in the data
//	blob. Use ID 1000D
//
//-------------------------------------------------------------------------------

type 'FltD' {
	array {
		integer					// Specifies the type of the data
			pdOpaque = 0,			// Opaque - don't interpolate this
			pdChar = 1,				// Interpolate as signed byte
			pdShort = 2,			// Interpolate as signed short
			pdLong = 3,				// Interpolate as signed long
			pdUnsignedChar = 4,		// Interpolate as unsigned byte
			pdUnsignedShort = 5,	// Interpolate as unsigned short
			pdUnsignedLong = 6,		// Interpolate as unsigned long
			pdExtended = 7,			// Interpolate as an extended
			pdDouble = 8,			// Interpolate as a double
			pdFloat = 9;			// Interpolate as a float
		integer;				// Count of bytes to skip with pdOpaque, 0 otherwise.
	};
};

//-------------------------------------------------------------------------------

// end PIUtilities.r
