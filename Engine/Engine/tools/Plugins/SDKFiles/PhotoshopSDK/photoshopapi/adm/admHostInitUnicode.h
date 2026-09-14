/***********************************************************************/
/*                                                                     */
/* ADMHostInitUnicode.h                                                */
/* ADM Host Init Unicode Conversion Suite                              */
/*                                                                     */
/* ADOBE SYSTEMS INCORPORATED                                          */
/* Copyright 1999-2001 Adobe Systems Incorporated                      */
/* All Rights Reserved                                                 */
/*                                                                     */
/* NOTICE:  Adobe permits you to use, modify, and distribute this file */
/* in accordance with the terms of the Adobe license agreement         */
/* accompanying it. If you have received this file from a source other */
/* than Adobe, then your use, modification, or distribution of it      */
/* requires the prior written permission of Adobe.                     */
/*                                                                     */
/* Started by Eric Scouten, 11/15/1999                                 */
/* Updated by Dave Hackel, 2/1/2000                                    */
/*                                                                     */
/***********************************************************************/

#ifndef __ADMUnicode__
#define __ADMUnicode__

#ifndef __ASTypes__
#include "asTypes.h"
#endif

#ifndef __ADMTypes__
#include "admTypes.h"
#endif


// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN

// -----------------------------------------------------------------------------

#define kADMScriptRoman			0
#define	kADMScriptJapanese		1
#define	kADMScriptTradChinese	2
#define	kADMScriptKorean		3
#define kADMScriptArabic		4
#define kADMScriptHebrew		5
#define kADMScriptGreek			6
#define	kADMScriptCyrillic		7
#define	kADMScriptSimpChinese	25
#define	kADMScriptEastEurope	29


// =============================================================================
//		* ADM Host Init Unicode Conversion Suite
// =============================================================================
//	The host of ADM can provide this suite for ADM to use at runtime.
//	This suite is only required if the host application (or any of its plugins)
//	wishes to set or retrieve text from ADM's interfaces via Unicode.
//	If the application only communicates with ADM using multibyte encodings,
//	this suite can be omitted.
 
#define kADMHostInitUnicodeSuite "ADM Host Init Unicode Suite"
#define kADMHostInitUnicodeSuiteVersion1 1

// -----------------------------------------------------------------------------

typedef struct ADMHostInitUnicodeSuite1
{

	// *** This suite is FROZEN.
	// *** It shipped with Acrobat 5.0.

	// character conversion

	ASErr ASAPI (*MultibyteToUnicode)(const char* inMBCSText, ASUInt32 inSourceLength,
				ASUnicode* outUnicodeText, ASUInt32* outUnicodeSize, ASUInt32 inADMScriptCode);

	ASErr ASAPI (*UnicodeToMultibyte)(const ASUnicode* inUnicodeText,
				ASUInt32 inSourceLength, char* outMBCSText,
				ASUInt32* outMBCSSize, ASUInt32 inADMScriptCode);

	// script determination
	
	ASErr ASAPI (*GetPreferredScript)(const ASUnicode* inUnicodeText,
				ASUInt32 inDefaultScript, ASUInt32* outMBCSScript);

	ASErr ASAPI (*GetFontForScript)(ASUInt32 inADMScriptCode,
				ADMFont inFontSelector, ADMPlatformFontRef* outFont);

	// allowing the host to sub-class input windows to enable keyboard
	// layout switching

	ASErr ASAPI (*SubclassItemForUnicodeInput)(ASWindowRef inWindow);
	ASErr ASAPI (*GetCurrentUnicodeInputScript)(ASWindowRef inWindow, ASUInt32 *outADMScriptCode);
	ASErr ASAPI (*SetCurrentInputScriptFromUnicode)(ASWindowRef inWindow, const ASUnicode* inUnicodeText);
	ASErr ASAPI (*UnsubclassItemForUnicodeInput)(ASWindowRef inWindow);
}
ADMHostInitUnicodeSuite1;


// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------

#endif	// __ADMUnicode__
