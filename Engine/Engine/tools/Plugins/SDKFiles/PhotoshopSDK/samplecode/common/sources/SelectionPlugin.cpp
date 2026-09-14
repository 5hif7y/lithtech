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
//		SelectionPlugin.cpp
//
//	Description:
//		Selection plug in derived class source file for any Selection plug in.
//		Derive your Selection plug in from this class.
//
//
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "SelectionPlugin.h"



//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
const bool SelectionPlugin::Message(const int16 selector, void * record)
{ 

	selectionRecord = static_cast<SelectionRecord *>(record);

	switch (selector)
	{
		case selectionSelectorAbout:
		{
			AboutRecord * aboutRecord = static_cast<AboutRecord *>(selectionRecord);
			About(aboutRecord);
			break;
		}
		
		case selectionSelectorExecute:
			Execute();
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
		SelectionRecord * selectionRecord = static_cast<SelectionRecord *>(record);
		suite = selectionRecord->sSPBasic;
	}
	return (suite);
}
// end SelectionPlugin.cpp	

