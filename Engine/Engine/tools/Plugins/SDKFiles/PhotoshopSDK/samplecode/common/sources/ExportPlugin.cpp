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
//		ExportPlugin.cpp
//
//	Description:
//		Export plug in derived class source file for any Export plug in.
//		Derive your Export plug in from this class.
//
//
//-------------------------------------------------------------------------------

#include "ExportPlugin.h"



//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
const bool ExportPlugin::Message(const int16 selector, void * record)
{ 

	exportRecord = static_cast<ExportRecord *>(record);

	switch (selector)
	{
		case exportSelectorAbout:
		{
			AboutRecord * aboutRecord = static_cast<AboutRecord *>(record);
			About(aboutRecord);
			break;
		}
		case exportSelectorParameters:
			Parameters();
			break;
		case exportSelectorPrepare:
			Prepare();
			break;
		case exportSelectorStart:
			Start();
			break;
		case exportSelectorContinue:
			Continue();
			break;
		case exportSelectorFinish:
			Finish();
			break;
		default:
			break;
	}

	if (selector == exportSelectorAbout || selector == exportSelectorFinish)
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
		ExportRecord * ExportRecord = static_cast<ExportRecord *>(record);
		suite = ExportRecord->sSPBasic;
	}
	return (suite);
}
// end ExportPlugin.cpp	

