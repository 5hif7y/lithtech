/***********************************************************************/
/*                                                                     */
/* ADMHostInit.h                                                       */
/* ADM Host Initialization Suite                                       */
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
/* Started by Matt Foster, 08/07/1997                                  */
/*                                                                     */
/***********************************************************************/

#ifndef __ADMHostInit__
#define __ADMHostInit__

#ifndef __ADMHost__
#include "admHost.h"
#endif


// -----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


// -----------------------------------------------------------------------------
//	Host options

#define kADMEnableFloatingPalettes (1L << 0)
			// 1 by default, palettes are enabled and ADM is always in memory.
			// Disabling palettes allows only modal operations and ADM can be
			// unloaded. Photoshop 5.x uses this to disable floating palettes.

#define kADMUseFrontWindowAsAppWindow (1L << 1)
			// 1 by default, Windows only. If app window is undefined, ADM will
			// use whatever window is frontmost at startup as the parent for all
			// windows. If 0, the host must set the app window. Used in Premiere 5.0.

#define kADMUseMinMaxOnBadValue (1L << 2)
			// 0 by default, when a entered value exceeds its min or max, it
			// resets the last good value. If this option is set, it sets the
			// value to the min or max/ Toggles between Illustrator behavior
			// (the default) and Photoshop 5.x.

#define kADMUseMacGSAppearance (1L << 3)
			// 0 by default. If 1 and Mac OS 8 is running, dialogs will have the
			// platinum look, else lighter grey look. All apps shipped after
			// Premiere 5 should have this set.

#define kADMUseRomanFontForKorean (1L << 4)
			// DEPRECATED: use kADMForceRomanFont instead. 0 by default. If set,
			// the Roman ADM UI font will be used for text on Korean systems
			// instead of the system script font (for non-localized Korean).
			// ImageReady 1.0 did not localize to Korean.

#define kADMUseRomanFontForChinese (1L << 5)
			// DEPRECATED: use kADMForceRomanFont instead. 0 by default. If set,
			// the Roman ADM UI font will be used for text on Chinese systems
			// instead of the system script font (for non-localized Chinese).
			// Note that it is assumed Chinese is either localized for all or
			// character sets or none. So this flag currently controls Simplified
			// and Traditional scripts. ImageReady 1.0 did not localize to Chinese.

#define kADMUseDialogHelpForUndefinedItemHelpID	 (1L << 6)
			// This determines how help is handled when an item has an undefined
			// help ID. By default (0), it will try to call the ASHelp function
			// DialogHelp() with the dialog ID and item ID. If this flag is set,
			// it will try to call ContextHelp() with the dialog's help ID.
			// This affects the format of the help file. Illustrator 7 uses the
			// default, ImageReady 1.0 uses the dialog IDs.

#define kADMUseDialogHelpForUndefinedEntryHelpID (1L << 7)
			// This determines how help is handled when an entry has an undefined
			// help ID. By default (0), it will try to call the ASHelp function
			// PaletteMenuHelp() with the dialog ID and item ID. If this flag is
			// set, it will try to call ContextHelp() with the dialog's help ID.
			// This affects the format of the help file. Illustrator 7 uses the
			// default, ImageReady 1.0 uses the dialog IDs.

#define kADMTrackerGetsDoubleByteInput (1L << 8)
			// 0 by default. If set, trackers treat double-byte characters as
			// single entities rather than as two separate key input events.

#define kADMAllowPaletteMenuClicksInModalState (1L << 9)
			// 0 by default, which is to only allow palette menu selections for the
			// front-most window. If set, any tab palette window will receive the
			// click. For use by Photoshop style application managed modal state.

#define kADMDisallowLanguageChangeDialog (1L << 10)
			// If 1, ignores the hot key to change languages.

#define kADMForceRomanFont (1L << 11)
			// 0 by default. If set, text on non-Roman systems will use the Roman
			// ADM UI font instead of the system script font. This allows non-localized
			// dialog layouts to be used on Japanese, Korean and Chinese systems,
			// avoiding truncation of text that no longer fits in a given area. This
			// setting can be overridden for individual objects by specifying
			// kADMIgnoreRomanFontForcing in the "options" of the Create functions.
			// Note that this will affect every dialog in every plug-in. To get the
			// same effect on a per-dialog basis, use kADMForceRomanFontForDialog. 

#define kADMDontUseCustomFloatersForPalettes (1L << 12)
			// 0 by default. If set, floating windows will not use ADM's TP
			// (Tabbed Palette) customized windows, and will instead use
			// standard system-provided floating windows without modification.
			// Consequently, tabs and other behavior will not be available.
			// Used by Acrobat.

#define kADMUseDialogFontAsDefaultForPalettes (1L << 13)
			// 0 by default. If set, the larger "dialog" font will be used as
			// the default font for floating windows (which normally use the
			// "palette" font. Used by Acrobat.

#define kADMPaletteControlsBehaveLikeModals (1L << 14)
			// 0 by default. If set, controls in floating windows will behave
			// more like they do in modal dialogs: Return is like hitting OK,
			// Escape is like hitting Cancel, and controls remain selected.
			// Used by Acrobat.

#define kADMDontUseADMFonts (1L << 15)
			// 0 by default. If set, do not load the ADM fonts and set the
			// font semaphore. Used by Acrobat, which does not use the fonts.

#define kADMWindowsAreExternalToClientApp (1L << 16)
			// 0 by default. If set, the client app is itself a plug-in managed
			// by another app, so windows are external to ADM's client. Used by
			// Acrobat for browser plug-ins.

#define kADMUseMLTEIfAvailable (1L << 17)
			// 0 by default. If set, the Carbon build will use MLTE (Multi-
			// Lingual Text Edit) if it is available. MLTE is not used by
			// default because it is currently problematic. Used by Acrobat.

#define kADMModalDialogsHaveCloseBoxHostOption (1L << 18)
			// 0 by default. If set, ADM Modal dialogs on Windows will have a 
			// close box on the top right hand corner. Also there is a dialog
			// option that a user can use on a dialog by dialog basis
			// dagashe:09/29/00:added for Acrobat 5.0 bug #382265

#define kADMUsePopupDisplayForEntriesOption (1L << 19)
			//0 by default. If set, ADM entries in ADM Lists and ADM Hierarchy 
			//lists will show a popup when the the mouse is hovered over the 
			//entry, if the text of the entry is cut off.


// =============================================================================
//		* ADM Host Initialization Suite
// =============================================================================
//	The host of ADM provides this suite for ADM to use at initialization.
//	These values can also be set/reset later if desired using the ADM Host suites.
//
//	Most suites must be fully initialized. Since this suite is intended to be
//	used by ADM to communicate with the host to obtain overrides to its defaults,
//	any of these function pointers can be NULL and ADM will simply use the default
//	behavior until it is changed later. The exception to this is the UI resource
//	directory on Windows and the host options. They are checked once at startup
//	and never again.
 
#define kADMHostInitSuite "ADM Host Init Suite"
#define kADMHostInitSuiteVersion3 3

// -----------------------------------------------------------------------------

typedef struct ADMHostInitSuite3
{
	// *** This suite is FROZEN.
	// *** It shipped with Acrobat 5.0.

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

	ASBoolean ASAPI (*GetIndexString)(ASInt16 inStringID,
			ASInt16 inStringIndex, char *outString, ASInt16 inMaxLength);

	ASBoolean ASAPI (*GetIndexStringW)(ASInt16 inStringID,
			ASInt16 inStringIndex, ASUnicode* outString, ASInt32 inMaxLength);

}
ADMHostInitSuite3;


// -----------------------------------------------------------------------------
//	Old versions of suites

#ifndef __ADMHostInitOld__
#ifdef MAC_ENV
#include "ADMHostInitOld.h"
#else
#include ".\Legacy\ADMHostInitOld.h"
#endif
#endif

// -----------------------------------------------------------------------------

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------

#endif	// __ADMHostInit__
