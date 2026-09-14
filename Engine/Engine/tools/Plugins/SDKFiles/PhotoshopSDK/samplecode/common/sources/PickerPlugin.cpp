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
//		PickerPlugin.cpp
//
//	Description:
//		Picker plug in derived class source file for any Picker plug in.
//		Derive your Picker plug in from this class.
//
//
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "PickerPlugin.h"



//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
const bool PickerPlugin::Message(const int16 selector, void * record)
{ 

	pickerRecord = static_cast<PIPickerParams *>(record);

	switch (selector)
	{
		case pickerSelectorAbout:
		{
			AboutRecord * aboutRecord = reinterpret_cast<AboutRecord *>(pickerRecord);
			About(aboutRecord);
			break;
		}
		
		case pickerSelectorPick:
			Pick();
			break;

		default:
			break;
	}

	return Done;
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
		PIPickerParams * pickerRecord = static_cast<PIPickerParams *>(record);
		suite = pickerRecord->sSPBasic;
	}
	return (suite);
}
// end PickerPlugin.cpp	

