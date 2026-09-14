// ADOBE SYSTEMS INCORPORATED
// Copyright  2002 - 2002 Adobe Systems Incorporated
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
//		FormatPlugin.cpp
//
//	Description:
//		Format plug in derived class source file for any Format plug in.
//		Derive your Format plug in from this class.
//
//
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "FormatPlugin.h"



//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
const bool FormatPlugin::Message(const int16 selector, void * record)
{ 

	formatRecord = static_cast<FormatRecord *>(record);

	switch (selector)
	{
		case formatSelectorAbout:
		{
			AboutRecord * aboutRecord = static_cast<AboutRecord *>(record);
			About(aboutRecord);
			break;
		}
		
		case formatSelectorReadPrepare:
			ReadPrepare();
			break;
		case formatSelectorReadStart:
			ReadStart();
			break;
		case formatSelectorReadContinue:
			ReadContinue();
			break;
		case formatSelectorReadFinish:
			ReadFinish();
			break;
		
		case formatSelectorOptionsPrepare:
			OptionsPrepare();
			break;
		case formatSelectorOptionsStart:
			OptionsStart();
			break;
		case formatSelectorOptionsContinue:
			OptionsContinue();
			break;
		case formatSelectorOptionsFinish:
			OptionsFinish();
			break;

		case formatSelectorEstimatePrepare:
			EstimatePrepare();
			break;
		case formatSelectorEstimateStart:
			EstimateStart();
			break;
		case formatSelectorEstimateContinue:
			EstimateContinue();
			break;
		case formatSelectorEstimateFinish:
			EstimateFinish();
			break;

		case formatSelectorWritePrepare:
			WritePrepare();
			break;
		case formatSelectorWriteStart:
			WriteStart();
			break;
		case formatSelectorWriteContinue:
			WriteContinue();
			break;
		case formatSelectorWriteFinish:
			WriteFinish();
			break;
	
		case formatSelectorFilterFile:
			FilterFile();
			break;

		default:
			break;
	}

	if (selector == formatSelectorAbout ||
		selector == formatSelectorReadFinish ||
		selector == formatSelectorOptionsFinish ||
		selector == formatSelectorEstimateFinish ||
		selector == formatSelectorWriteFinish)
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
		FormatRecord * formatRecord = static_cast<FormatRecord *>(record);
		suite = formatRecord->sSPBasic;
	}
	return (suite);
}
// end FormatPlugin.cpp	

