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
//		AutomationPlugin.h
//
//	Description:
//		Base class for all automation plugins
//
//-------------------------------------------------------------------------------

#ifndef __AutomationPlugin_h__
#define __AutomationPlugin_h__

#include <stdlib.h>

#include "PIDefines.h"
#include "SPTypes.h"
#include "admTypes.h"
#include "SPAccess.h"
#include "SPInterf.h"
#include "piactions.h"
#include "piactionsplugin.h"
#include "PIUSuites.h"
#include "PIUI.h"
#include "pihandlesuite.h"


#ifdef __PIMac__
	#define HANDLE Handle
#endif

#define kUnhandledMsgErr	'!MSG'		// This isn't really an error

/// This is global from the input parameters you don't need to acquire
extern SPBasicSuite * sSPBasic;

/** An Automation plug in. Derive your automation plug in from this class.
    Override the virtual functions that you need. You probably only need 
	the DoIt function, the base class will do the rest.
*/
class AutomationPlugin
{
private:
	void ReleaseSuites(void);

	AutomationPlugin(); // don't write this one
	AutomationPlugin(AutomationPlugin &rhs); // don't write this one

protected:
	SPPluginRef pluginRef;
	SPErr error;
		
public:
	AutomationPlugin(SPPluginRef inPluginRef) : pluginRef(inPluginRef) {}
	virtual ~AutomationPlugin();
	
	/// Get the reference to this plug in
	SPPluginRef GetPluginRef(void)	{ return pluginRef; }

	/// See if this call is the "Reload" call
	ASBoolean IsReloadMsg(const char * caller, const char * selector);

	/// See if this call is the "Unload" call
	ASBoolean IsUnloadMsg(const char * caller, const char * selector);

	/// Photoshop messages
	virtual SPErr DoIt(PSActionsPlugInMessage *) { return kNoErr; }
	
	/// Start up your plug in. Only called for persistent plug ins
	virtual ASErr StartupPlugin(SPInterfaceMessage *) { return kNoErr; }

	/// Shut down your plug in. Only called for persistent plug ins
	virtual ASErr ShutdownPlugin(SPInterfaceMessage *) { return kNoErr; }

	/// Unload your plug in. Plug in is about to leave memory.
	virtual ASErr UnloadPlugin(SPInterfaceMessage * message);

	/// Reload (Load) your plug in. Your plug in is now in memory.
	virtual ASErr ReloadPlugin(SPInterfaceMessage * message);

	/// Display your about box.
	virtual ASErr About(SPInterfaceMessage * message);

	/// Messages that come in to this plug in
	virtual ASErr Message(const char * caller, const char * selector, void * message);

	/// Error to report back to Photoshop
	SPErr GetError(void) { return error; }
};

/// Your plug in must supply this routine. Allocates your derived class.
extern AutomationPlugin * AllocatePlugin(SPPluginRef pluginRef);

#endif
