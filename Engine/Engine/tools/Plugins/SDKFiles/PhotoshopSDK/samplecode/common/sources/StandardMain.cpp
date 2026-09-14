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
//-------------------------------------------------------------------------------
//
//	File:
//		StandardMain.cpp
//
//	Description:
//		Entry point for all standard plug ins for Photoshop. Standard plug ins
//		are all plug ins except the Automation plug in type.
//
//
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "plugin.h"

// this makes sure that the class created is deleted before a unload reload.
// if not your vtable pointers are hosed and your plug in will crash.
static bool unloaded = true;



//-------------------------------------------------------------------------------
//
//	PluginMain
//
// actual entrypoint for all standard plug ins, do not use this for automation
//
//-------------------------------------------------------------------------------
DLLExport MACPASCAL void PluginMain(const int16 selector,
								    void * record,
								    int32 * data,
								    int16 * result)
{
	Plugin * plugin = NULL;

	try 
	{
		// Plug in base class and derived class have dependancies on this global
		sSPBasic = GetSPBasic(selector, record);
		
		// see if we already have a plug in
		plugin = reinterpret_cast<Plugin *>(*data);

		// if we do and this code is already unloaded then we have vtable problems
		if (plugin != NULL && unloaded)
		{
			*result = paramErr;
			*data = NULL;
			return;
		}

		// no plugin make one from the global AllocatePlugin call
		if (plugin == NULL)
		{
			plugin = AllocatePlugin();
			if (plugin != NULL)
			{
				*data = reinterpret_cast<int32>(plugin);
				unloaded = false;
			}
			else
			{
				*result = memFullErr;
				*data = NULL;
				return;
			}
		}

		// plugin may be reporting errors here and not throwing
		*result = plugin->GetError();

		// respond to the selector, return value is if your done or not
		if (plugin->Message(selector, record) == Done)
		{
			delete plugin;
			*data = NULL;
		}
	}


	// set the error and throw your plug in
	catch (Plugin * p)
	{
		*result = p->GetError();
		delete p;
		*data = NULL;
	}

	// you can throw an OSType and I convert to int16 but this
	// is not recommended
	catch (OSType inError)
	{
		*result = paramErr;
		
		int16 index = -1;

		while (spErrorMap[++index].suitePeaErrorCode != kSPNoError)
			if (spErrorMap[index].suitePeaErrorCode == inError)
				*result = spErrorMap[index].photoshopErrorCode;
		delete plugin;
		*data = NULL;
	}

	// you can throw an int16 but this is not recommended
	// out of memory will get to here
	catch (int16 inError)
	{
		*result = inError;
		delete plugin;
		*data = NULL;
	}

	// something really bad happened
	catch (...)
	{
		*result = paramErr;
		delete plugin;
		*data = NULL;
	}

	// reset sSPBasic
	sSPBasic = NULL;
}
// end StandardMain.cpp