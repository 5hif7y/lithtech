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
//		FilterPlugin.cpp
//
//	Description:
//		Filter plug in derived class source file for any filter plug in.
//		Derive your filter plug in from this class.
//
//
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "FilterPlugin.h"

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
const bool FilterPlugin::Message(const int16 selector, void * record)
{ 
	filterRecord = static_cast<FilterRecord *>(record);

	switch (selector)
	{
		case filterSelectorAbout:
		{
			AboutRecord * aboutRecord = static_cast<AboutRecord *>(record);
			About(aboutRecord);
			break;
		}
		case filterSelectorParameters:
			Parameters();
			break;
		case filterSelectorPrepare:
			Prepare();
			break;
		case filterSelectorStart:
			Start();
			break;
		case filterSelectorContinue:
			Continue();
			break;
		case filterSelectorFinish:
			Finish();
			break;
		default:
			break;
	}

	if (selector == filterSelectorFinish || selector == filterSelectorAbout)
		return Done;

	return NotDone;
}
 


//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
SPBasicSuite * GetSPBasic(const int16 selector, void * record)
{
	SPBasicSuite * suite = NULL;

	if (selector == plugInSelectorAbout)
	{
		AboutRecord * aboutRecord = static_cast<AboutRecord *>(record);
		suite = aboutRecord->sSPBasic;
	}
	else
	{
		FilterRecord * filterRecord = static_cast<FilterRecord *>(record);
		suite = filterRecord->sSPBasic;
	}
	return (suite);
}
// end FilterPlugin.cpp	

