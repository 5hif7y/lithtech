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
//		Template.cpp
//
//	Description:
//
//		Template is a "starter" automation plug-in, which has the essential 
//		components of an automation plug-in.  Template simply pops a dialog.
//
//		Template has:
//
//		* a menuitem which pops a dialog
//		* About menu item, identifying the Template plug-in.
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes -- Use precompiled headers if compiling with CodeWarrior.
//-------------------------------------------------------------------------------
#include "PhotoshopSDK.h"
#include "admBasic.h"

//-------------------------------------------------------------------------------
//	Globals -- Define global variables for plug-in scope.
//-------------------------------------------------------------------------------

SPBasicSuite			*sSPBasic = NULL;// the Basic Suite

//-------------------------------------------------------------------------------
//
//	AutoPluginMain
//
//	All calls to the plug-in module come through this routine.
//	It must be placed first in the resource. To achieve this,
//	most development systems require this be the first routine
//	in the source.
//
//	The entrypoint will be "pascal void" for Macintosh,
//	"void" for Windows.
//
//-------------------------------------------------------------------------------

DLLExport SPAPI SPErr AutoPluginMain(const char * caller, // who is calling 
								     const char * selector, // what do they want
								     void * data)// what is the message
{
	SPErr error = kSPNoError;
	
	//all messages contain a SPMessageData*
	SPMessageData * basicMessage = (SPMessageData *) data;
	
	sSPBasic = basicMessage->basic;


	// check for SP interface callers
	if (sSPBasic->IsEqual((char*)caller, kSPInterfaceCaller))
	{
		if (sSPBasic->IsEqual((char*)selector, kSPInterfaceAboutSelector))
		{
			DoAbout(basicMessage->self, AboutID);
		}
	}

	// Photoshop is calling
	if (sSPBasic->IsEqual((char*)caller, kPSPhotoshopCaller))
	{
		// the one and only message 
		if (sSPBasic->IsEqual((char*)selector, kPSDoIt))
		{
			//pop a dialog to show plug-in works
			DoAbout(basicMessage->self, AboutID);
		}
	}

	// clean up, free memory
	PIUSuitesRelease();

	return error;

}/*end AutoPluginMain*/
// end Template.cpp