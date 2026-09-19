/***********************************************************************/
/*                                                                     */
/* ADMHost.h                                                           */
/* ADM Host Suite                                                      */
/*                                                                     */
/* ADOBE SYSTEMS INCORPORATED                                          */
/* Copyright 1996-2001 Adobe Systems Incorporated                      */
/* All Rights Reserved                                                 */
/*                                                                     */
/* NOTICE:  Adobe permits you to use, modify, and distribute this file */
/* in accordance with the terms of the Adobe license agreement         */
/* accompanying it. If you have received this file from a source other */
/* than Adobe, then your use, modification, or distribution of it      */
/* requires the prior written permission of Adobe.                     */
/*                                                                     */
/* Started by Dave Lazarony, 03/09/1996                                */
/*                                                                     */
/***********************************************************************/

#ifndef __ADMHost__
#define __ADMHost__

#ifndef __ADMTypes__
#include "admTypes.h"
#endif

#ifndef	__ADMVERSION_H__
#include "admVersion.h"
#endif

#ifndef __SPMessageData__
#include "SPMData.h"
#endif

#ifndef __SPAccess__
#include "SPAccess.h"
#endif

#ifndef _ADMAGMTYPES_H_
#include "admAGMTypes.h"
#endif


// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


// -----------------------------------------------------------------------------
//	SweetPea callback interface

#define kADMPluginName "ADM Plugin"

#define kADMHostCaller "ADM Host"
#define kADMHostStartSelector "ADM Started"
#define kADMHostEndSelector "ADM Ending"

typedef struct ADMHostInterfaceMessage
{
	SPMessageData d;
}
ADMHostInterfaceMessage;


// -----------------------------------------------------------------------------
//	GetADMVersion returns the version number ADM is compiled with. This number
//	consists of a hi word which is the major version number, and a low word which
//	is the minor revision number. Beginning with 2.30, the minor version is in
//	the low word, high byte. The low word, low byte is for branch versions.
//	
//	The macro ADM_VERSION can be used to make sure you are running the version
//	you think you are.

#define ADM_VERSION (0x00 | (ADMVMajor << 16) | (ADMVMinor << 8) | (ADMVBuildNumber))


// -----------------------------------------------------------------------------
//	User define procedures callbacks

typedef void ASAPI (*ADMAGMCalibrationProc)(ADMAGMPort* inPort);
typedef void ASAPI (*ModifyPaletteLayoutBoundsProc)(ASRect* ioDimensions);


// -----------------------------------------------------------------------------
//	Application context

typedef struct _t_ADMHostAppContextOpaque* ADMHostAppContextRef;

typedef ASErr ASAPI (*BeginHostAppContextProc)(ADMPluginRef inPlugin,
			ADMHostAppContextRef* inAppContext);
typedef ASErr ASAPI (*EndHostAppContextProc)(ADMHostAppContextRef inAppContext);

typedef struct ADMHostAppContextProcs
{
	BeginHostAppContextProc beginAppContext;
	EndHostAppContextProc endAppContext;
} 
ADMHostAppContextProcs;


// -----------------------------------------------------------------------------
//	Resource context

typedef struct _t_ADMResContextOpaque* ADMResContextRef;

	// This is basically the same as the SPAccess suite call GetAccessInfo().
	// Use it on Windows to supply an instance handle and on Mac if you have
	// highly managed resource chain.
typedef ASErr ASAPI (*GetResourceAccessInfoProc)(ADMPluginRef inPlugin,
			SPPlatformAccessInfo* inInfo);

	// These can be used to order the MacResource chain (e.g. bring a plug-in
	// to the top) and restore a previous order if necessary. Restore doesn't
	// necessarily have to do anything. The resContext you return to the set
	// function will be returned by restore if you need it.
typedef ASErr ASAPI (*SetResourceContextProc)(ADMPluginRef inPlugin, ADMResContextRef* inResContext);
typedef ASErr ASAPI (*RestoreResourceContextProc)(ADMResContextRef inResContext);

typedef struct ADMResourceAccessProcs
{
		// Supply functions getResAccessInfo() *OR* set/resetResContext() 
	GetResourceAccessInfoProc getResAccessInfo;
		
	SetResourceContextProc setResContext;		
	RestoreResourceContextProc resetResContext;
}
ADMResourceAccessProcs;


// -----------------------------------------------------------------------------
//	Overridable defaults for text items

typedef struct ADMDefaultTextInfo
{

	// text

	ASInt32 maxTextLen;
	ADMJustify justify;

	// numerics

	ADMUnits units;
	ASInt32 numberOfDecimalPlaces;
	ASBoolean allowUnits;
	ASBoolean allowMath;

}
ADMDefaultTextInfo;


// -----------------------------------------------------------------------------

#ifndef kMaxHostNameLen
#define kMaxHostNameLen 256
#endif

#ifndef kADMMaxLanguageCode
#define kADMMaxLanguageCode 34
#endif


// =============================================================================
//		* ADM Host Suite, version 5
// =============================================================================

#define kADMHostSuite "ADM Host Suite"
#define kADMHostSuiteVersion5 5

// -----------------------------------------------------------------------------

typedef struct ADMHostSuite5
{

	// This suite version (5) was introduced during development of PS 6.0/IR 3.0
	// Should be frozen when PS 6.0 ships.	// dhearst 6/20/00

	// AGM calibration

	void ASAPI (*SetAGMCalibrationProc)(ADMAGMCalibrationProc inCalibrationProc);
	ADMAGMCalibrationProc ASAPI (*GetAGMCalibrationProc)();
	
	// mouse information accessors
	
	ASBoolean ASAPI (*PointerInUse)();
	
	// palette layout bounds modifiers
	
	void ASAPI (*SetModifyPaletteLayoutBoundsProc)(ModifyPaletteLayoutBoundsProc inModifyBoundsProc);
	ModifyPaletteLayoutBoundsProc ASAPI (*GetModifyPaletteLayoutBoundsProc)();

	// host information accessors

	void ASAPI (*SetADMHostAppContextProcs)(const ADMHostAppContextProcs* inAppContextProcs);
	void ASAPI (*GetADMHostAppContextProcs)(ADMHostAppContextProcs* outAppContextProcs);
	
	void ASAPI (*SetADMHostAppName)(const char* inHostName);
	void ASAPI (*GetADMHostAppName)(char* outHostName, ASInt16 inMaxLength);

	void ASAPI (*SetADMResourceAccessProcs)(const ADMResourceAccessProcs* inResProcs);
	void ASAPI (*GetADMResourceAccessProcs)(ADMResourceAccessProcs* outResProcs);

	// ADM build version

	ASUInt32 ASAPI (*GetADMVersion)();
	
	// text behaviors
	
	void ASAPI (*SetADMDefaultTextInfo)(const ADMDefaultTextInfo* inDefaultTextInfo);
	void ASAPI (*GetADMDefaultTextInfo)(ADMDefaultTextInfo* outDefaultTextInfo);

	void ASAPI (*SetADMLanguageCode)(const char* inLanguageCode);
	void ASAPI (*GetADMLanguageCode)(char* outLanguageCode);
										// max length is kADMMaxLanguageCode

	// modal dialog mode (currently unimplemented)

	void ASAPI (*SetADMModalMode)(ASBoolean inModalMode);
	ASBoolean ASAPI (*GetADMModalMode)();

	// ADM-handled keys

	ASBoolean ASAPI (*IsADMHandledKey)(const char inKeyCode);

	// text commitment
	
	ASBoolean ASAPI (*CommitActiveTextItem)(void);

}
ADMHostSuite5;


// =============================================================================
//		* ADM Host Suite, version 6
// =============================================================================

#define kADMHostSuiteVersion6 6

// -----------------------------------------------------------------------------

typedef struct ADMHostSuite6
{

	// AGM calibration

	void ASAPI (*SetAGMCalibrationProc)(ADMAGMCalibrationProc inCalibrationProc);
	ADMAGMCalibrationProc ASAPI (*GetAGMCalibrationProc)();
	
	// mouse information accessors
	
	ASBoolean ASAPI (*PointerInUse)();
	
	// palette layout bounds modifiers
	
	void ASAPI (*SetModifyPaletteLayoutBoundsProc)(ModifyPaletteLayoutBoundsProc inModifyBoundsProc);
	ModifyPaletteLayoutBoundsProc ASAPI (*GetModifyPaletteLayoutBoundsProc)();

	// host information accessors

	void ASAPI (*SetADMHostAppContextProcs)(const ADMHostAppContextProcs* inAppContextProcs);
	void ASAPI (*GetADMHostAppContextProcs)(ADMHostAppContextProcs* outAppContextProcs);
	
	void ASAPI (*SetADMHostAppName)(const char* inHostName);
	void ASAPI (*GetADMHostAppName)(char* outHostName, ASInt16 inMaxLength);

	void ASAPI (*SetADMHostAppNameW)(const ASUnicode* inHostName);
	void ASAPI (*GetADMHostAppNameW)(ASUnicode* outHostName, ASInt16 inMaxLength);

	void ASAPI (*SetADMResourceAccessProcs)(const ADMResourceAccessProcs* inResProcs);
	void ASAPI (*GetADMResourceAccessProcs)(ADMResourceAccessProcs* outResProcs);

	// ADM build version

	ASUInt32 ASAPI (*GetADMVersion)();
	
	// text behaviors
	
	void ASAPI (*SetADMDefaultTextInfo)(const ADMDefaultTextInfo* inDefaultTextInfo);
	void ASAPI (*GetADMDefaultTextInfo)(ADMDefaultTextInfo* outDefaultTextInfo);

	void ASAPI (*SetADMLanguageCode)(const char* inLanguageCode);
	void ASAPI (*GetADMLanguageCode)(char* outLanguageCode);
										// max length is kADMMaxLanguageCode

	// modal dialog mode (currently unimplemented)

	void ASAPI (*SetADMModalMode)(ASBoolean inModalMode);
	ASBoolean ASAPI (*GetADMModalMode)();

	// ADM-handled keys

	ASBoolean ASAPI (*IsADMHandledKey)(const char inKeyCode);

	// text commitment
	
	ASBoolean ASAPI (*CommitActiveTextItem)(void);

}
ADMHostSuite6;


// =============================================================================
//		* ADM MacOS Host Suite
// =============================================================================

#ifdef MAC_ENV

#define kADMMacHostSuite "ADM Mac Host Suite"
#define kADMMacHostSuiteVersion2 2

// -----------------------------------------------------------------------------

typedef ASErr ASAPI (*ADMModalEventProc)(struct EventRecord* inEvent);
typedef ASErr ASAPI (*ADMWindowActivateProc)(struct EventRecord* inEvent);

// -----------------------------------------------------------------------------

typedef struct ADMMacHostSuite2
{

	// *** This suite is FROZEN.
	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.

	// event hand-off to ADM

	ASBoolean ASAPI (*HandleADMEvent)(struct EventRecord* inEvent);

	// modal dialog event callback
	
	void ASAPI (*SetModalEventProc)(ADMModalEventProc inModalEventProc);
	ADMModalEventProc ASAPI (*GetModalEventProc)();

	// window activation
	
	void ASAPI (*ActivateWindows)(ASBoolean inActivate);

	// activation notification
	
	void ASAPI (*SetWindowActivateProc)(ADMWindowActivateProc inWindowActivateProc);
	ADMWindowActivateProc ASAPI (*GetWindowActivateProc)();

}
ADMMacHostSuite2;	

// -----------------------------------------------------------------------------

#define kADMMacHostSuiteVersion kADMMacHostSuiteVersion2	// for historical reasons
typedef ADMMacHostSuite2 ADMMacHostSuite;					// for historical reasons

#endif


// =============================================================================
//		* ADM Windows Host Suite
// =============================================================================

#ifdef WIN_ENV

// -----------------------------------------------------------------------------
// dhearst 6/22/00
// ADMHWNDOptimizationMode enumeration:
// Used to specify a global setting in ADM (on Windows only) for how it
// handles the use of system resources. Designed to help on Win9x, where
// there is a hard limit to the number of usable system resources and Adobe
// apps just keep using more palettes...

typedef enum
{
	kADMDoNotOptimizeHWNDUsage = 0,		// Classic ADM performance
	kADMAlwaysOptimizeHWNDUsage = 1,	// Always release HWNDs for all non-visible dialogs
	kADMOptimizeHWNDUsageByDialog = 2,	// Only release HWNDs for non-visible dialogs that have specifically requested optimization
	kADMDummyHWNDOptimization = 0xFFFFFFFF
}
ADMHWNDOptimizationMode;


#define kADMWinHostSuite "ADM Win Host Suite"
#define kADMWinHostSuiteVersion4 4

// -----------------------------------------------------------------------------

typedef ASErr ASAPI (*ADMModalMessageProc)(ASUInt32 inMessage, ASUInt32 inWParam,
			ASInt32 inLParam);

typedef void ASAPI (*ADMTextFocusChangedProc)(ASBoolean inSetFocus);

// -----------------------------------------------------------------------------

typedef struct ADMWinHostSuite4
{

	// This suite version (4) was introduced during development of PS 6.0/IR 3.0
	// Should be frozen when PS 6.0 ships.	// dhearst 6/20/00

	ASBoolean ASAPI (*HandleADMMessage)(ASWindowRef inFrameWnd, ASUInt16 inMessage,
				ASUInt16 inWParam, ASInt32 inLParam);

	ASWindowRef ASAPI (*GetCurrentOwnerWindow)();

	ASBoolean ASAPI (*IsADMMessage)(const struct tagMSG* inMessage);

	ASWindowRef ASAPI (*GetPlatformAppWindow)();
	void ASAPI (*SetPlatformAppWindow)(ASWindowRef inAppWindow);

	ADMModalMessageProc ASAPI (*GetModalMessageProc)();							
	void ASAPI (*SetModalMessageProc)(ADMModalMessageProc inModalMessageProc);

	ADMTextFocusChangedProc ASAPI (*GetTextFocusChangedProc)();						
	void ASAPI (*SetTextFocusChangedProc)(ADMTextFocusChangedProc inFocusChangedProc);

	void ASAPI (*SetHWNDOptimizationMode)(ADMHWNDOptimizationMode inOptimizationMode);

	void ASAPI (*AllowHWNDOptimizationForDialog)(ADMDialogRef inDialog,
				ASBoolean inAllowOptimization);
}
ADMWinHostSuite4;

#endif


// -----------------------------------------------------------------------------
//	Old versions of suites

#ifndef __ADMHostOld__
#ifdef MAC_ENV
#include "ADMHostOld.h"
#else
#include ".\Legacy\ADMHostOld.h"
#endif
#endif

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------

#endif	// __ADMHost__
