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
//	Includes 
//-------------------------------------------------------------------------------
#ifndef __AUTOMATIONFILTER_H__
#define __AUTOMATIONFILTER_H__

#include "PIDefines.h"
#include "SPHost.h"
#include "PIUGet.h"
#include "PIUSelect.h"
#include "piactionsplugin.h"
#include "piuihookssuite.h"
#include "pihandlesuite.h"
#include "AutomationPlugin.h"

#include "HiddenCommands.h"
#include "AutomationFilterUI.h"
#include "aszstringsuite.h"
#include "PIUSuites.h"

#if __PIMac__
	#include <string.h>
#endif
#include <vector>
#include <time.h>

using namespace std;

// forward decleration, see AutomationFilterUI.h
class AutomationFilterDialog;


/** Version 2 of AutomationFilter and Hidden.
    The Hidden plug in now does all of the pixel work.
	This enables us to work on much bigger files. The Hidden plug in still
	passes information back to this plug in, tiling information for example.
	Passing all the pixel data is a bad idea so I removed it.
*/
class AutomationFilterPlugin : public AutomationPlugin {

private:
	int32 numChannels;
	vector<Ptr> channelNames;
	int32 numLayers;
	vector<Ptr> layerNames;
	DescriptorEnumID command;
	bool hasBackground;
	int32 tileHeight;
	int32 tileWidth;

	/// Dialog for this plug in
	AutomationFilterDialog * mainDialog;

	/// AutomationPlugin base class function over rides
	virtual SPErr DoIt(PSActionsPlugInMessage * message);
	virtual SPErr UnloadPlugin(SPInterfaceMessage * message);

	/// Functions specific to the AutomationFilter plug in
	SPErr GetDocumentInfo(void);
	SPErr ExecuteHiddenFilter(DescriptorEnumID command, 
		                      Ptr nameOfChannel,
							  int16 typeOfChannel,
							  uint8 percent);
	SPErr GetChannelNames(void);
	SPErr GetLayerNames(void);

public:

	AutomationFilterPlugin(SPPluginRef pluginRef);
	~AutomationFilterPlugin() {}

	/// Functions needed by AutomationFilterDialog
	SPErr Execute(void);
	Ptr GetLayerName(int32 index);
	Ptr GetChannelName(int32 index);
	void ExecuteFilter(Ptr layerName, Ptr channelName, uint8 percentWhite);

	/// Return the number of layers in this document, I count the background as a layer
	int32 NumberOfLayers(void) { return (numLayers + hasBackground); }

	/// Return the number of channels in this document
	int32 NumberOfChannels(void) { 	return numChannels; }

	/// Return the tile height for this document
	int32 GetTileHeight(void) { return tileHeight; }

	/// Return the tile width for this document
	int32 GetTileWidth(void) { return tileWidth; }
};
#endif
// end AutomationFilter.h

