// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2002 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
//-------------------------------------------------------------------------------
//
//	File:
//		AutomationPlugin.cpp
//
//	Description:
//		Base class implementation. This is all the messages that can come from a
//		Photoshop automation plugin. Derive your plug in from this class, implement
//		the routine messages you need.
//
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "AutomationPlugin.h"



//-------------------------------------------------------------------------------
//
//	AutomationPlugin::~AutomationPlugin
//	
//	Release the suites. They get acquired during the reload message and released
//	during the unload message. This is probably not needed but ReleaseSuites()
//	is smart enough to do this only once.
//
//-------------------------------------------------------------------------------
AutomationPlugin::~AutomationPlugin()
{
	ReleaseSuites();
}



//-------------------------------------------------------------------------------
//
//	AutomationPlugin::About
//	
//	I can pop a simple about dialog box for you. Just put your two strings in
//	your resource file at iAboutBoxPluginName and iAboutBoxAuthor
//
//-------------------------------------------------------------------------------
SPErr AutomationPlugin::About(SPInterfaceMessage * message)
{
	DoAbout(message->d.self, AboutID);
	return(kNoErr);
}



//-------------------------------------------------------------------------------
//
//	AutomationPlugin::IsReloadMsg
//	
//	We need special logic in AutomationMain.cpp AutoPluginMain so ask the base class
//	if we are in the reload message.
//
//-------------------------------------------------------------------------------
ASBoolean AutomationPlugin::IsReloadMsg(const char * caller, const char * selector)
{
	return (sSPBasic->IsEqual(caller, kSPAccessCaller) &&
			 sSPBasic->IsEqual(selector, kSPAccessReloadSelector));
}



//-------------------------------------------------------------------------------
//
//	AutomationPlugin::IsUnloadMsg
//	
//	We need special logic in AutomationMain.cpp AutoPluginMain so ask the base class
//	if we are in the unload message.
//
//-------------------------------------------------------------------------------
ASBoolean AutomationPlugin::IsUnloadMsg(const char * caller, const char * selector)
{
	return (sSPBasic->IsEqual(caller, kSPAccessCaller) &&
			 sSPBasic->IsEqual(selector, kSPAccessUnloadSelector));
}



//-------------------------------------------------------------------------------
//
//	AutomationPlugin::Message
//	
//	This is where we farm out the message and make the calls to the routines.
//
//-------------------------------------------------------------------------------
SPErr AutomationPlugin::Message(const char * caller, const char * selector, void * message)
{
	SPErr error = kUnhandledMsgErr;

	// Sweet Pea messages for Photoshop

	if (sSPBasic->IsEqual(caller, kSPAccessCaller)) 
	{
		if (sSPBasic->IsEqual(selector, kSPAccessUnloadSelector))
			error = UnloadPlugin(static_cast<SPInterfaceMessage *> (message));
			
		else if (sSPBasic->IsEqual(selector, kSPAccessReloadSelector))
			error = ReloadPlugin(static_cast<SPInterfaceMessage *> (message));
	}
	else if (sSPBasic->IsEqual(caller, kSPInterfaceCaller)) 
	{	
		if (sSPBasic->IsEqual(selector, kSPInterfaceAboutSelector))
			error = About(static_cast<SPInterfaceMessage *> (message));

		else if (sSPBasic->IsEqual(selector, kSPInterfaceStartupSelector))
			error = StartupPlugin(static_cast<SPInterfaceMessage *> (message));

		else if (sSPBasic->IsEqual(selector, kSPInterfaceShutdownSelector))
			error = ShutdownPlugin(static_cast<SPInterfaceMessage *> (message));
	}
	else if (sSPBasic->IsEqual(caller, kPSPhotoshopCaller))
	{
		if (sSPBasic->IsEqual(selector, kPSDoIt))
			error = DoIt(static_cast<PSActionsPlugInMessage *> (message));
	}

	return error;
}



//-------------------------------------------------------------------------------
//
//	AutomationPlugin::UnloadPlugin
//	
//	This plug in is about to be unloaded from memory. Clean yourself up.
//	We only need to unload all the global suites via ReleaseSuites().
//
//-------------------------------------------------------------------------------
SPErr AutomationPlugin::UnloadPlugin(SPInterfaceMessage * /*message*/)
{
	ReleaseSuites();

	return kNoErr;
}



//-------------------------------------------------------------------------------
//
//	AutomationPlugin::ReloadPlugin
//	
//	This plug in just got loaded in to memory.
//
//-------------------------------------------------------------------------------
SPErr AutomationPlugin::ReloadPlugin(SPInterfaceMessage * /*message*/)
{
	return kNoErr;
}



//-------------------------------------------------------------------------------
//
//	AutomationPlugin::ReleaseSuites
//	
//	Release all the suites used
//
//-------------------------------------------------------------------------------
void AutomationPlugin::ReleaseSuites(void)
{
	PIUSuitesRelease();
}
// end AutomationPlugin.cpp
