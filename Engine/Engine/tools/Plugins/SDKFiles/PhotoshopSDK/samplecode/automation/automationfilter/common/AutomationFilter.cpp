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
//		AutomationFilter.cpp
//
//	Description:
//		An automation plug in that uses a hidden filter plug in to
//		do something very boring.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes 
//-------------------------------------------------------------------------------
#include "PhotoshopSDK.h" 
#include "AutomationFilter.h"

#define AFERROR(FUNC) if (true) { error = FUNC; if (error) throw(this); } else

//-------------------------------------------------------------------------------
//	Globals
//-------------------------------------------------------------------------------

// everyone needs access to the sPSBasic pointer 
// new and delete operators for example
SPBasicSuite * sSPBasic = NULL;
SPPluginRef	gPlugInRef = NULL;

// there is a problem with the SuspendHistory() so we
// need a global error value to solve the problem
SPErr gError = kSPNoError;

//-------------------------------------------------------------------------------
// Funciton Prototypes
//-------------------------------------------------------------------------------

// helper routine for the SuspendHistory()
SPErr Execute(void * parameters);



//-------------------------------------------------------------------------------
//
//	AutomationFilterPlugin::AutomationFilterPlugin
//	
//	The constructor which happens in a unique way. Via the AllocatePlugin which
//	gets called from PISPMain.cpp.
//
//-------------------------------------------------------------------------------
AutomationFilterPlugin::AutomationFilterPlugin(SPPluginRef pluginRef)
  : AutomationPlugin(pluginRef), numChannels(0), numLayers(0), command(0), 
  hasBackground(false), mainDialog(NULL), tileWidth(0), tileHeight(0)
{
	channelNames.clear();
	layerNames.clear();
}



//-------------------------------------------------------------------------------
//
//	AllocatePlugin
//	
//	The real constructor hear. You have to have one of these so the AutomationMain.cpp
//	AutoPluginMain routine can create your class.
//
//-------------------------------------------------------------------------------
AutomationPlugin * AllocatePlugin(SPPluginRef pluginRef)
{
	return new AutomationFilterPlugin(pluginRef);
}



//-------------------------------------------------------------------------------
//
//	AutomationFilterPlugin::UnloadPlugin
//	
//	Photoshop has asked us to give up our memory and prepare to be unloaded.
//
//-------------------------------------------------------------------------------
SPErr AutomationFilterPlugin::UnloadPlugin(SPInterfaceMessage * message) 
{ 
	vector<Ptr>::iterator name_iter = channelNames.begin();
	vector<Ptr>::iterator name_end = channelNames.end();
	while (name_iter != name_end)
	{
		delete [] *name_iter;
		name_iter++;
	}

	name_iter = layerNames.begin();
	name_end = layerNames.end();
	while (name_iter != name_end)
	{
		delete [] *name_iter;
		name_iter++;
	}

	return (AutomationPlugin::UnloadPlugin(message)); 
}



//-------------------------------------------------------------------------------
//
//	AutomationFilterPlugin::DoIt
//	
//	Do what we need to do. In a round about way this will get to our Execute()
//	routine. We do it this way so only one entry shows up in the history palette.
//	Pretty cool huh!
//
//-------------------------------------------------------------------------------
SPErr AutomationFilterPlugin::DoIt(PSActionsPlugInMessage * message)
{
	Auto_Ref reference;

	ASZString	nameAsZString = NULL;

	AFERROR(sPSUIHooks->GetPluginName(message->d.self, &nameAsZString));
				
	AFERROR(sPSActionReference->PutEnumerated(reference.get(), 
											  classDocument, 
											  typeOrdinal, 
											  enumTarget));
	
	// If your execute routine error's you will not get the
	// error value from SuspendHistory for version 5.5 of Photoshop.
	// This was fixed for Photoshop 6.0. I am going to use a global error value
	// to work around this problem.
	sPSActionControl->SuspendHistory(reference.get(), 
									::Execute, 
									this,
									nameAsZString);
	
	error = gError;
	gError = kSPNoError;

	sASZString->Release(nameAsZString);

	return (error);
}




//-------------------------------------------------------------------------------
//
//	Execute
//	
//	A helper routine. Get back inside the class and Execute from inside there.
//
//-------------------------------------------------------------------------------
SPErr Execute(void * parameters)
{
	// try/catch this also because there is a layer between you and the 
	// SuspendHistory() call. In Photoshop 5.5 the error return value is not
	// passed back to SuspendHistory() show use a global value instead.
	try
	{
		AutomationFilterPlugin * autoPlugin = static_cast<AutomationFilterPlugin *> (parameters);
		if (autoPlugin == NULL)
		{
			gError = kSPBadParameterError;
			throw(gError);
		}
		gError = autoPlugin->Execute();
	}

	// I could throw the plugin
	catch(AutomationFilterPlugin * p)
	{
		gError = p->GetError();
	}

	// I am throwing SPErr's
	catch(SPErr thrownError)
	{
		gError = thrownError;
	}

	// Somebody could throw anything
	catch(...)
	{
		gError = kSPLogicError;
	}

	return (gError);
}



//-------------------------------------------------------------------------------
//
//	AutomationFilterPlugin::Execute
//	
//	DoIt -> Execute() to this Execute. Now we can get down to business.
//
//-------------------------------------------------------------------------------
SPErr AutomationFilterPlugin::Execute(void)
{
	AFERROR(GetDocumentInfo());

	AFERROR(ExecuteHiddenFilter(enumInfo, NULL, 0, 0));

	mainDialog = new AutomationFilterDialog(this);

	int item = mainDialog->Modal(GetPluginRef(), NULL, 16001);
	if (item == kDCancel_Button)
	{
		// you would think this would be kSPUserCanceledError
		// but no Photoshop doesn't like that
		error = userCanceledErr;
	}

	delete mainDialog;

	return (error);
}



//-------------------------------------------------------------------------------
//
//	AutomationFilterPlugin::GetChannelNames
//	
//	Get all the channel names into a vector. The dialog will use them later.
//
//-------------------------------------------------------------------------------
SPErr AutomationFilterPlugin::GetChannelNames(void)
{
	uint32 stringLength;
	Ptr stringValue = NULL;

	for (int32 channelIndex = 1; channelIndex <= numChannels; channelIndex++)
	{
		char tempName[256];
		stringLength = 255;
		
		error = PIUGetInfoByIndex(channelIndex,
			                      classChannel,
								  keyChannelName,
								  tempName,
								  &stringLength);
		if (error) break;

		stringValue = new char[stringLength+1];

		strcpy(stringValue, tempName);
		
		channelNames.push_back(stringValue);
	}

	return (error);
}



//-------------------------------------------------------------------------------
//
//	AutomationFilterPlugin::GetLayerNames
//	
//	Get all the layer names into a vector. The dialog will use them later.
//
//-------------------------------------------------------------------------------
SPErr AutomationFilterPlugin::GetLayerNames(void)
{
	Auto_Desc result(false); // don't create one we pass into Get(...) routine
	Auto_Ref reference;

	uint32 stringLength;
	Boolean hasKey;
	Ptr stringValue;
	int32 layerIndex;

	AFERROR(sPSActionReference->PutProperty(reference.get(), 
		                                    classBackgroundLayer, 
											keyBackground));

	AFERROR(sPSActionReference->PutEnumerated(reference.get(), 
											  classDocument, 
											  typeOrdinal, 
											  enumTarget));

	// ignore the error on purpose, we may not have a background layer
	(void)sPSActionControl->Get(&result, reference.get());
	if (result.get() != NULL)
	{
		hasKey = false;
		
		AFERROR(sPSActionDescriptor->HasKey(result.get(), keyName, &hasKey));

		if (hasKey)
		{

			AFERROR(sPSActionDescriptor->GetStringLength(result.get(),
				                                         keyName,
														 &stringLength));

			stringValue = new char[stringLength+1];

			AFERROR(sPSActionDescriptor->GetString(result.get(),
				                                   keyName,
												   stringValue,
												   stringLength+1));

			layerNames.push_back(stringValue);
			hasBackground = true;
		}
	}

	for (layerIndex = 1; layerIndex <= numLayers; layerIndex++)
	{
		char tempName[256];
		stringLength = 255;
		
		AFERROR(PIUGetInfoByIndex(layerIndex,
			                      classLayer,
								  keyName,
								  tempName,
								  &stringLength));

		stringValue = new char[stringLength+1];

		strcpy(stringValue, tempName);
		
		layerNames.push_back(stringValue);
	}

	return (error);
}



//-------------------------------------------------------------------------------
//
//	AutomationFilterPlugin::GetDocumentInfo
//	
//  Find out everything we need to know about this document.
//
//-------------------------------------------------------------------------------
SPErr AutomationFilterPlugin::GetDocumentInfo(void)
{
	Auto_Desc result(false);
	Auto_Ref reference;

	AFERROR(sPSActionReference->PutEnumerated(reference.get(), 
											  classDocument, 
											  typeOrdinal, 
											  enumTarget));
	
	AFERROR(sPSActionControl->Get(&result, reference.get()));

	AFERROR(sPSActionDescriptor->GetInteger(result.get(),
											keyNumberOfChannels,
											&numChannels));

	AFERROR(GetChannelNames());
	
	AFERROR(sPSActionDescriptor->GetInteger(result.get(),
											keyNumberOfLayers,
											&numLayers));

	AFERROR(GetLayerNames());
	
	return error;
}



//-------------------------------------------------------------------------------
//
//	AutomationFilterPlugin::ExecuteHiddenFilter
//	
//  Run the hidden filter given the following command.
//	You have the source to the hidden filter so you can add new commands.
//
//-------------------------------------------------------------------------------
SPErr AutomationFilterPlugin::ExecuteHiddenFilter(DescriptorEnumID command, 
												  Ptr nameOfChannel,
												  int16 typeOfChannel,
												  uint8 percent)
{
	Auto_Desc descriptor;
	Auto_Desc result(false);

	DescriptorTypeID hiddenEventID;
	DescriptorEnumID enumID;
	DescriptorEnumTypeID typeID;
	
	AFERROR(sPSActionDescriptor->PutEnumerated(descriptor.get(), 
		                                       keyCommand, 
											   typeCommand, 
											   command));

	if (command == enumWrite)
	{
		if (nameOfChannel == NULL)
			error = sPSActionDescriptor->PutInteger(descriptor.get(),
													keyType,
													(int32)typeOfChannel);
		else
			error = sPSActionDescriptor->PutString(descriptor.get(),
												   keyChannelName,
												   nameOfChannel);
		if (error) throw(this);
		
		AFERROR(sPSActionDescriptor->PutInteger(descriptor.get(), keyPercent, percent));
				
	} // writing pass extra parameters
		
	AFERROR(sPSActionControl->StringIDToTypeID(HiddenUniqueString, 
		                                       &hiddenEventID));
	
	error = sPSActionControl->Play(&result, 
		                           hiddenEventID, 
								   descriptor.get(), 
								   plugInDialogSilent);
	
	if (result.get() != NULL && error == noErr)
		error = sPSActionDescriptor->GetEnumerated(result.get(), 
		                                       keyResult, 
											   &typeID, 
											   &enumID);
	
	if (enumID != enumOK || error)
	{
		error = -1;
		uint32 stringLength;
		if (sPSActionDescriptor->GetStringLength(result.get(), 
			                                     keyMessage, 
												 &stringLength) == 0)
		{
			vector<char> message(stringLength+1);
			if (&message[0] != NULL)
			{
				(void)sPSActionDescriptor->GetString(result.get(), 
					 								 keyMessage, 
													 &message[0], 
													 stringLength+1);
			}
		}
	}

	// pull the tile data out of the return descriptor
	if (command == enumInfo)
	{
		Boolean hasKey;
		hasKey = false;

		(void)sPSActionDescriptor->HasKey(result.get(), keyWidth, &hasKey);
		
		if (hasKey)
		{
			AFERROR(sPSActionDescriptor->GetInteger(result.get(), keyWidth, &tileWidth));
		}

		hasKey = false;
		(void)sPSActionDescriptor->HasKey(result.get(), keyHeight, &hasKey);

		if (hasKey)
		{
			AFERROR(sPSActionDescriptor->GetInteger(result.get(), keyHeight, &tileHeight));
		}
	}

	return error;
}



//-------------------------------------------------------------------------------
//
//	AutomationFilterPlugin::GetLayerName
//	
//	Return the name of the layer at the given index.
//
//-------------------------------------------------------------------------------
Ptr AutomationFilterPlugin::GetLayerName(int32 index)
{
	Ptr name = NULL;
	if (index >= 0 && index < numLayers + hasBackground)
		name = layerNames[index];
	return (name);
}



//-------------------------------------------------------------------------------
//
//	AutomationFilterPlugin::GetChannelName
//	
//	Return the name of the channel at the given index.
//
//-------------------------------------------------------------------------------
Ptr AutomationFilterPlugin::GetChannelName(int32 index)
{
	Ptr name = NULL;
	if (index >= 0 && index < numChannels)
		name = channelNames[index];
	return (name);
}



//-------------------------------------------------------------------------------
//
//	AutomationFilterPlugin::ExecuteFilter
//	
//	Set up the document to target the layer and channel and blast some pixels.
//
// NOTE: You have to blast the transparency layer or your pixel data will not
// show.
//
//-------------------------------------------------------------------------------
void AutomationFilterPlugin::ExecuteFilter(Ptr layerName, Ptr channelName, uint8 percentWhite)
{
	Boolean isBackground = true;

	PIUSelectByName(classLayer, layerName);
	PIUSelectByName(classChannel, channelName);

	PIUGetInfo(classLayer, keyBackground, &isBackground, NULL);

	if (!isBackground)
	{
		// you are on a layer so set the transparency layer
		ExecuteHiddenFilter(enumWrite, NULL, ctTransparency, 100);
	}

	ExecuteHiddenFilter(enumWrite, channelName, ctUnspecified, percentWhite);
}
// end AutomationFilter.cpp



