// ADOBE SYSTEMS INCORPORATED
// Copyright  2002 - 2002 Adobe Systems Incorporated
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
//		PlusPlus.cpp
//
//	Description:
//		This file contains the source and functions
//		for the Color Picker example module PlusPlus.
//
//	Use:
//		PlusPlus provides another color picker (available
//		from File: Preferences: General) that cycles through
//		four primary colors.
//
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "PickerPlugin.h"

class PlusPlus : public PickerPlugin {
	void Pick(void);
public:
	PlusPlus() : PickerPlugin() {}
	virtual ~PlusPlus() {}
};

//-------------------------------------------------------------------------------
//	Globals -- Define global variables for plug-in scope.
//-------------------------------------------------------------------------------
SPBasicSuite * sSPBasic = NULL;


//-------------------------------------------------------------------------------
// The colors you get are very odd. 16 bit colors. The palette shows 1,0,0 but
// you get 0x0102, 0x0000, 0x0000. So I grab the first eight bits and increment
// that. I then duplicate it back into the 16 bit color.
//-------------------------------------------------------------------------------
void PlusPlus::Pick(void)
{
	pickerRecord->pickParms.resultSpace = plugIncolorServicesRGBSpace;

	// convert the given color to RGB
	if (pickerRecord->pickParms.sourceSpace != plugIncolorServicesRGBSpace)
	{
		ColorServicesInfo	csInfo;

		csInfo.selector = plugIncolorServicesConvertColor;
		csInfo.sourceSpace = pickerRecord->pickParms.sourceSpace;
		csInfo.resultSpace = plugIncolorServicesRGBSpace;
		csInfo.reservedSourceSpaceInfo = NULL;
		csInfo.reservedResultSpaceInfo = NULL;
		csInfo.reserved = NULL;
		csInfo.selectorParameter.pickerPrompt = NULL;
		csInfo.infoSize = sizeof(csInfo);

		for (int16 a = 0; a < 4; a++)
			csInfo.colorComponents[a] = pickerRecord->pickParms.colorComponents[a];

		if (!(pickerRecord->colorServices(&csInfo)))
			for (int16 b = 0; b < 4; b++)
				pickerRecord->pickParms.colorComponents[b] = csInfo.colorComponents[b];
	}				   

	uint8 smallColor[4];
	for (int16 c = 0; c < 4; c++)
		smallColor[c] = static_cast<uint8>(pickerRecord->pickParms.colorComponents[c] >> 8);


	smallColor[0]++;

	bool rolled = false;

	if (smallColor[0] == 0)
	{
		rolled = true;
		smallColor[1]++;
	}

	if (smallColor[1] == 0 && rolled)
	{
		smallColor[2]++;
	}

	for (int16 d = 0; d < 4; d++)
		pickerRecord->pickParms.colorComponents[d] = smallColor[d] * 0x0101;
}


//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
Plugin * AllocatePlugin(void)
{
	return new PlusPlus;
}
// end PlusPlus.cpp
