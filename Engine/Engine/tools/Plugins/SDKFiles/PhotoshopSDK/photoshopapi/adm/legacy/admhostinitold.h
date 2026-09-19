/***********************************************************************/
/*                                                                     */
/* ADMHostInitOld.h                                                    */
/* Old versions of ADM Host Initialization Suite                       */
/*                                                                     */
/* ADOBE SYSTEMS INCORPORATED                                          */
/* Copyright 1997-2001 Adobe Systems Incorporated                      */
/* All Rights Reserved                                                 */
/*                                                                     */
/* NOTICE:  Adobe permits you to use, modify, and distribute this file */
/* in accordance with the terms of the Adobe license agreement         */
/* accompanying it. If you have received this file from a source other */
/* than Adobe, then your use, modification, or distribution of it      */
/* requires the prior written permission of Adobe.                     */
/*                                                                     */
/***********************************************************************/

#ifndef __ADMHostInitOld__
#define __ADMHostInitOld__

// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


// =============================================================================
//		* ADM Host Initialization Suite, version 1
// =============================================================================

#define kADMHostInitSuiteVersion1 1
#define kADMHostInitSuiteVersion kADMHostInitSuiteVersion1

// -----------------------------------------------------------------------------

typedef struct ADMHostInitSuite1
{

	// *** This suite is FROZEN.
	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.

	void ASAPI (*GetADMHostAppContextProcs)(ADMHostAppContextProcs* outAppContextProcs);
	void ASAPI (*GetADMHostAppName)(char* outHostName, ASInt16 inMaxLength);
	void ASAPI (*GetADMResourceAccessProcs)(ADMResourceAccessProcs* outResProcs);
	void ASAPI (*GetADMHostOptions)(ASInt32* outOptions);
	void ASAPI (*GetHostModifyPaletteLayoutBoundsProc)(ModifyPaletteLayoutBoundsProc* outProc);
	
	#ifdef MAC_ENV
		ADMWindowActivateProc ASAPI (*GetWindowActivateProc)();
		void ASAPI (*GetModalEventProc)(ADMModalEventProc* outModalEventProc);
	#endif

	#ifdef WIN_ENV
		ASWindowRef ASAPI (*GetAppWindow)();
		void ASAPI (*GetUIResourceDirectory)(SPPlatformFileSpecification* outResDir);
					// default is 'System'
	#endif

	ASBoolean ASAPI (*GetIndexString)(ASInt16 inStringID, ASInt16 inStringIndex,
		char *outString, ASInt16 inMaxLength);

}
ADMHostInitSuite1;

typedef ADMHostInitSuite1 ADMHostInitSuite;

// =============================================================================
//		* ADM Host Initialization Suite, version 2
// =============================================================================

#define kADMHostInitSuiteVersion2 2

// -----------------------------------------------------------------------------

typedef struct ADMHostInitSuite2
{
	// *** This suite is FROZEN.
	// *** It was used during development of Acrobat 5.0.

	void ASAPI (*GetADMHostAppContextProcs)(ADMHostAppContextProcs* outAppContextProcs);
	void ASAPI (*GetADMHostAppName)(char* outHostName, ASInt16 inMaxLength);
	void ASAPI (*GetADMResourceAccessProcs)(ADMResourceAccessProcs* outResProcs);
	void ASAPI (*GetADMHostOptions)(ASInt32* outOptions);
	void ASAPI (*GetHostModifyPaletteLayoutBoundsProc)(ModifyPaletteLayoutBoundsProc* outProc);
	
	#ifdef MAC_ENV
		ADMWindowActivateProc ASAPI (*GetWindowActivateProc)();
		void ASAPI (*GetModalEventProc)(ADMModalEventProc* outModalEventProc);
	#endif

	#ifdef WIN_ENV
		ASWindowRef ASAPI (*GetAppWindow)();
		void ASAPI (*GetUIResourceDirectory)(SPPlatformFileSpecification* outResDir);
					// default is 'System'
	#endif

	ASBoolean ASAPI (*GetIndexString)(ASInt16 inStringID, ASInt16 inStringIndex,
			char *outString, ASInt16 outMaxLength);

}
ADMHostInitSuite2;

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------

#endif	// __ADMHostInitOld__
