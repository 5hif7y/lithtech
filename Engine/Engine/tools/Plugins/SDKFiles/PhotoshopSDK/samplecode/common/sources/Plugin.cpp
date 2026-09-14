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
//		Plugin.cpp
//
//	Description:
//		Base class implementation for Plugin. This is the base class for FilterPlugin
//		and all the other plug-in types. You need to derive from the type of
//		plug-in you are developing for.
//
//
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "plugin.h"

// signal whether the plug in is done using these flags
const bool Done = true;
const bool NotDone = false;

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
Plugin::Plugin() : error(0)
{ 
}



//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
Plugin::~Plugin() 
{ 
	ReleaseSuites(); 
}



//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
const bool Plugin::About(AboutRecord * aboutRecord)
{
	DoAbout(static_cast<SPPluginRef>(aboutRecord->plugInRef), AboutID);
	return Done;
}



//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void Plugin::ReleaseSuites(void)
{
	PIUSuitesRelease();
}
// end Plugin.cpp