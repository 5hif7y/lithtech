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
//		AutomationMain.cpp
//
//	Description:
//		This is the main entrypoint for Photoshop to call.
//		All you need to do is derive a class from AutomationPlugin
//		overwrite the routines you want and your done.
//
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "AutomationPlugin.h"
#include "PICA2PSErrorMap.h"

// global variable to tell if this plug in got unloaded from memory and
// the AutomationPlugin did not get deleted, this is bad and will mess
// up the vtable. We guard against this with the following variable.
static bool unloaded = true;


//-------------------------------------------------------------------------------
//
//	AutoPluginMain
//	
//	Routine that gets called during the execution of a Photoshop plug in.
//	See the documentation and sample code for the caller/selector pairs
//	that occur.
//
//-------------------------------------------------------------------------------
DLLExport SPAPI SPErr AutoPluginMain(const char * caller, const char * selector, void * message)
{
	SPErr error = kNoErr;

	SPMessageData * msgData = static_cast<SPMessageData *>(message);
		
	AutomationPlugin * plugin = static_cast<AutomationPlugin *>(msgData->globals);
	
	bool shouldDelete = false;

	try 
	{
		sSPBasic = msgData->basic;

		if (plugin != NULL && unloaded)
			throw((OSType)kBadParameterErr);

		if (plugin == NULL)
		{
			plugin = AllocatePlugin(msgData->self);
			
			if (plugin != NULL)
			{
				msgData->globals = plugin;
				unloaded = false;
			}
			else
			{
				error = kOutOfMemoryErr;
				throw((OSType)error);
			}
			
		}

		error = plugin->Message(caller, selector, message);
		if (error) throw((OSType)error);

		if (plugin->IsUnloadMsg(caller, selector))
			shouldDelete = true;
	}

	// set the error and throw your plug in
	catch (AutomationPlugin * p)
	{
		error = p->GetError();
		shouldDelete = true;
	}

	catch (OSType inError)
	{
		error = inError;

		if (plugin != NULL && ( plugin->IsUnloadMsg(caller, selector) || 
		     					plugin->IsReloadMsg(caller, selector) ))
			shouldDelete = true;
	}

	catch (int16 inError)
	{
		error = kBadParameterErr;

		int16 index = -1;

		while (spErrorMap[++index].suitePeaErrorCode != kSPNoError)
			if (spErrorMap[index].photoshopErrorCode == inError)
				error = spErrorMap[index].suitePeaErrorCode;

		if (plugin != NULL && ( plugin->IsUnloadMsg(caller, selector) || 
		     					plugin->IsReloadMsg(caller, selector) ))
			shouldDelete = true;
	}
	
	catch (...)
	{
		error = kBadParameterErr;

		if (plugin != NULL && ( plugin->IsUnloadMsg(caller, selector) || 
		     					plugin->IsReloadMsg(caller, selector) ))
			shouldDelete = true;
	}

	if (shouldDelete)
	{
		delete plugin;
		msgData->globals = plugin = NULL;
		unloaded = true;
	}

	return error;
}
// end AutomationMain.cpp
