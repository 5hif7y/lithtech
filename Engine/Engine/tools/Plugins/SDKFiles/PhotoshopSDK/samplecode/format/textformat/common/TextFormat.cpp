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
//		TextFormat.h
//
//	Description:
//
//	Use:
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "TextFormat.h"

//-------------------------------------------------------------------------------
//	Globals
//-------------------------------------------------------------------------------

// everyone needs access to the sPSBasic pointer 
// new and delete operators for example
SPBasicSuite * sSPBasic = NULL;
// SPPluginRef	gPlugInRef = NULL;

//-------------------------------------------------------------------------------
//
//	TextAutoPlugin::TextAutoPlugin
//	
//	The constructor which happens in a unique way. Via the AllocatePlugin which
//	gets called from main routine.
//
//-------------------------------------------------------------------------------
TextFormatPlugin::TextFormatPlugin()
  : FormatPlugin()
{
}


void TextFormatPlugin::ReadPrepare(void)
{
}

void TextFormatPlugin::ReadStart(void)
{
	formatRecord->planes = 3;
	formatRecord->imageMode = plugInModeRGBColor;
	formatRecord->depth = 8;

	if (formatRecord->openForPreview)
	{
		formatRecord->imageSize.v = 256;
		formatRecord->imageSize.h = 256;
	}
	else
	{	
		formatRecord->imageSize.v = 10 * 72;
		formatRecord->imageSize.h = 8 * 72;
	}

}

void TextFormatPlugin::ReadFinish(void)
{
	PIDescriptorHandle h;
			
	PIDescriptorParameters * descParams = formatRecord->descriptorParameters;
	if (descParams == NULL) return;
	
	WriteDescriptorProcs * writeProcs = descParams->writeDescriptorProcs;
	if (writeProcs == NULL) return;

	PIWriteDescriptor token = writeProcs->openWriteDescriptorProc();
	if (token == NULL) return;
	
	sPSHandle->Dispose(descParams->descriptor);
	writeProcs->closeWriteDescriptorProc(token, &h);
	descParams->descriptor = h;
	
}


//-------------------------------------------------------------------------------
//
//	AllocatePlugin
//	
//	The real constructor hear. You have to have one of these so the	PluginMain 
//	routine can create your class.
//
//-------------------------------------------------------------------------------
Plugin * AllocatePlugin()
{
	return new TextFormatPlugin();
}

// end TextFormat.cpp