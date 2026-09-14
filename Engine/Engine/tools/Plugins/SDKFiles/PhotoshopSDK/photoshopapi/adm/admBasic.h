/***********************************************************************/
/*                                                                     */
/* ADMBasic.h                                                          */
/* ADM Basic Suite                                                     */
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

#ifndef __ADMBasic__
#define __ADMBasic__
 
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

#define kADMMaxFilterLength 256

typedef enum
{
	kADMNoAnswer = 0,
	kADMYesAnswer,
	kADMCancelAnswer,
	kADMDummyAnswer = 0xFFFFFFFF
}
ADMAnswer;

typedef enum
{
	kADMLeftAnswer = 0,
	kADMRightAnswer,
	kADMMiddleAnswer,
	kADMDummyChoice = 0xFFFFFFFF
}
ADMChoice;

// -----------------------------------------------------------------------------

#ifdef MAC_ENV

	typedef struct
	{
		ASUInt32* types;
		ASInt16 numTypes;
	}
	ADMPlatformFileTypesSpecification;

	typedef struct
	{
		ASUInt32* types;
		ASInt16 numTypes;
		char filter[kADMMaxFilterLength];
	}
	ADMPlatformFileTypesSpecification3;

#else

	typedef struct
	{
		char filter[kADMMaxFilterLength];
	}
	ADMPlatformFileTypesSpecification, ADMPlatformFileTypesSpecification3;

#endif


// =============================================================================
//		* ADM Basic Suite
// =============================================================================

#define kADMBasicSuite "ADM Basic Suite"
#define kADMBasicSuiteVersion8 8

// -----------------------------------------------------------------------------

typedef struct ADMBasicSuite8
{

	// *** This suite is OPEN.
	// *** It will ship with Illustrator 10.

	ASErr ASAPI (*GetIndexString)(SPPluginRef inPluginRef, ASInt32 inStringID,
				ASInt32 inStringIndex, char* outString, ASInt32 inMaxLen);

	ASErr ASAPI (*GetIndexStringW)(SPPluginRef inPluginRef, ASInt32 inStringID,
				ASInt32 inStringIndex, ASUnicode* outString, ASInt32 inMaxLen);

	void ASAPI (*Beep)();

	void ASAPI (*ErrorAlert)(const char* inErrorString);
	void ASAPI (*ErrorAlertW)(const ASUnicode* inErrorString);

	void ASAPI (*MessageAlert)(const char* inMessageString);
	void ASAPI (*MessageAlertW)(const ASUnicode* inMessageString);

	ADMAnswer ASAPI (*QuestionAlert)(const char* inQuestionString);
	ADMAnswer ASAPI (*QuestionAlertW)(const ASUnicode* inQuestionString);

	ADMAnswer ASAPI (*YesNoAlert)(const char* inQuestionString);
	ADMAnswer ASAPI (*YesNoAlertW)(const ASUnicode* inQuestionString);

	ASBoolean ASAPI (*GetScreenDimensions)(const ASPoint* inPoint, ASRect* outDimensions);

	void ASAPI (*EnableToolTips)(ASBoolean inEnable);
	ASBoolean ASAPI (*AreToolTipsEnabled)();

	void ASAPI (*ValueToString)(float inValue, char* outText, ASInt32 inMaxLen,
				ADMUnits inUnits, ASInt32 inPrecision, ASBoolean inAlwaysAppendUnits);

	void ASAPI (*ValueToStringW)(float inValue, ASUnicode* outText, ASInt32 inMaxLen,
				ADMUnits inUnits, ASInt32 inPrecision, ASBoolean inAlwaysAppendUnits);

	ASBoolean ASAPI (*StringToValue)(const char* inText, float* outValue, ADMUnits inUnits);
	ASBoolean ASAPI (*StringToValueW)(const ASUnicode* inText, float* outValue, ADMUnits inUnits);

	ASBoolean ASAPI (*StandardGetFileDialog)(const char* inMessage,
				const ADMPlatformFileTypesSpecification3* inFilter,
				const SPPlatformFileSpecification* inStartingDir,
				const char* inStartingFile, SPPlatformFileSpecification* outResult);

	ASBoolean ASAPI (*StandardGetFileDialogW)(const ASUnicode* inMessage,
				const ADMPlatformFileTypesSpecification3* inFilter,
				const SPPlatformFileSpecification* inStartingDir,
				const char* inStartingFile, SPPlatformFileSpecification* outResult);

	ASBoolean ASAPI (*StandardGetDirectoryDialog)(const char* inMessage,
				const SPPlatformFileSpecification* inStartingDir,
				SPPlatformFileSpecification* outResult);

	ASBoolean ASAPI (*StandardGetDirectoryDialogW)(const ASUnicode* inMessage,
				const SPPlatformFileSpecification* inStartingDir,
				SPPlatformFileSpecification* outResult);

	ASBoolean ASAPI (*StandardPutFileDialog)(const char* inMessage,
				const ADMPlatformFileTypesSpecification3* inFilter,
				const SPPlatformFileSpecification* inStartingDir,
				const char* inStartingFile, SPPlatformFileSpecification* outResult);

	ASBoolean ASAPI (*StandardPutFileDialogW)(const ASUnicode* inMessage,
				const ADMPlatformFileTypesSpecification3* inFilter,
				const SPPlatformFileSpecification* inStartingDir,
				const char* inStartingFile, SPPlatformFileSpecification* outResult);

	ASBoolean ASAPI (*ChooseColor)(ASPoint inWhere, const ASRGBColor* inColor,
				ASRGBColor* outColor);

	void ASAPI (*SetAppUnits)(ADMUnits inAppUnits, ADMUnits inActualUnits);
	ADMUnits ASAPI (*GetAppUnits)(ADMUnits inAppUnits);
	
	void ASAPI (*SetNumbersArePoints)(ASBoolean inPoints);
	ASBoolean ASAPI (*GetNumbersArePoints)();

	void ASAPI (*SetDefaultIncrements)(ADMUnits inUnits, float inSmallIncrement,
				float inLargeIncrement);

	void ASAPI (*GetDefaultIncrements)(ADMUnits inUnits, float* outSmallIncrement,
				float* outLargeIncrement);

	void ASAPI (*AboutBox)(SPPluginRef inPlugin, const char* inText1, const char* inText2);
	void ASAPI (*AboutBoxW)(SPPluginRef inPlugin, const ASUnicode* inText1, const ASUnicode* inText2);

	ASBoolean ASAPI (*AreToolTipsSticky)(ADMDialogRef inDialog);
	void ASAPI (*GetToolTipDelays)(ASInt32* outPopupDelay, ASInt32* outPopdownDelay);

	ASBoolean ASAPI (*GetWorkspaceBounds)(ASRect* outDimensions);
				// main screen less system menus, task bars, etc

	ASBoolean ASAPI (*GetPaletteLayoutBounds)(ASRect* outDimensions);
				// inset of standard application window

	void ASAPI (*SetPlatformCursor)(SPPluginRef inPluginRef, ASInt32 inCursorID);

	void ASAPI (*PluginAboutBox)(const char* inTitle, const char* inText);
	void ASAPI (*PluginAboutBoxW)(const ASUnicode* inTitle, const ASUnicode* inText);
	
	void ASAPI (*SetAlertButtonText)(const char* inLeftChoice, const char* inMiddleChoice,
				const char* inRightChoice);

	void ASAPI (*SetAlertButtonTextW)(const ASUnicode* inLeftChoice, const ASUnicode* inMiddleChoice,
				const ASUnicode* inRightChoice);

	void ASAPI (*ShowToolTip)(const ASPoint* inWhere, const char* inTip);
	void ASAPI (*ShowToolTipW)(const ASPoint* inWhere, const ASUnicode* inTip);
	void ASAPI (*HideToolTip)();
	
	ASErr ASAPI (*CreateMenu)(ADMListRef* outMenu);
	ASErr ASAPI (*DisplayMenu)(ADMListRef inMenu, ADMDialogRef inDialog, ASPoint inDialogPoint);
	ASErr ASAPI (*DestroyMenu)(ADMListRef inMenu);

	ASBoolean ASAPI (*GetLastADMError)(ASInt32* outError, ASInt32* outErrorData);
	ASBoolean ASAPI (*ADMColorToRGBColor)(ADMColor inADMColor, ASRGBColor* outRGBColor);

	void ASAPI (*SetAppFPS)(float inFPS);
	float ASAPI (*GetAppFPS)();

	void ASAPI (*LightweightErrorAlert)(const char* inErrorString);
	void ASAPI (*LightweightErrorAlertW)(const ASUnicode* inErrorString);

	ASBoolean ASAPI (*LookUpZString)(SPPluginRef inPluginRef,
				const char* inString, char* outString, ASUInt32* ioBufferSize);
}
ADMBasicSuite8;


// -----------------------------------------------------------------------------
//	Old versions of suites

#ifndef __ADMBasicOld__
#ifdef MAC_ENV
#include "ADMBasicOld.h"
#else
#include ".\Legacy\ADMBasicOld.h"
#endif
#endif

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------

#endif	// __ADMBasic__
