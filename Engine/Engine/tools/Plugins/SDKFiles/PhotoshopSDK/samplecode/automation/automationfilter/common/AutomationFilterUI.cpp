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
//		AutomationFilterUI.cpp
//
//	Description:
//		An automation plug in that uses a hidden filter plug in to
//		do something very boring.
//
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes 
//-------------------------------------------------------------------------------
#include "PhotoshopSDK.h"
#include "AutomationFilterUI.h"

//-------------------------------------------------------------------------------
//
//	AutomationFilterDialog::Init
//	
//
//-------------------------------------------------------------------------------
void AutomationFilterDialog::Init(void)
{
	PIDialogPtr dialog = GetDialog();

	PIItem item = PIGetDialogItem(dialog, kDLayer_List);
	layerList.SetItem(item);
	
	layerList.Clear();
	
	int entries = _instance->NumberOfLayers();
	int counter = 0;
	while (counter < entries)
	{
		Ptr layerName = _instance->GetLayerName(counter);
		layerList.AppendItem(layerName);
		counter++;
	}
	
	layerList.SetCurrentSelection(0);

	item = PIGetDialogItem(dialog, kDChannel_List);
	channelList.SetItem(item);
	
	channelList.Clear();

	entries = _instance->NumberOfChannels();
	counter = 0;
	while (counter < entries)
	{
		Ptr channelName = _instance->GetChannelName(counter);
		channelList.AppendItem(channelName);
		counter++;
	}

	channelList.SetCurrentSelection(0);

	ostringstream stringStream;

	item = PIGetDialogItem(dialog, kDTileHeight_Static);
	tileHeight.SetItem(item);
	stringStream << "Tile Height: " << _instance->GetTileHeight();
	tileHeight.SetText(stringStream.str().c_str());

	stringStream.str("");

	item = PIGetDialogItem(dialog, kDTileWidth_Static);
	tileWidth.SetItem(item);
	stringStream << "Tile Width: " << _instance->GetTileWidth();
	tileWidth.SetText(stringStream.str().c_str());
}



//-------------------------------------------------------------------------------
//
//	AutomationFilterDialog::Notify
//	
//  Anytime something happens we get notified. It would be nice to have each
//	button with its own notify routine but you need to make statics so the 
//	compiler is happy and it's just a pain in the
//
//  We only care about changes to the buttons.
//
//-------------------------------------------------------------------------------
void AutomationFilterDialog::Notify(int32 item)
{
	uint8 percentWhite;

	switch (item)
	{
		case kD0_Button:
			percentWhite = 0;
			break;
		case kD50_Button:
			percentWhite = 50;
			break;
		case kD100_Button:
			percentWhite = 100;
			break;
		case kDRandom_Button:
			percentWhite = kRandomPercent;
			break;
		default:
			return;
			break;
	}

	auto_ptr<char> layerName(layerList.GetCurrentSelection());
	if (layerName.get() == NULL) return;

	auto_ptr<char> channelName(channelList.GetCurrentSelection());
	if (channelName.get() == NULL) return;

	_instance->ExecuteFilter(layerName.get(), channelName.get(), percentWhite);
}
// end AutomationFilterUI.cpp