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
//		ListenerUI.cpp
//
//	Description:
//		This file contains the source code and routines for the
//		Automation module Listener, an example of a module
//		that uses the Actions suite in a persistent
//		Automation plug-in.
//
//-------------------------------------------------------------------------------
#include "PhotoshopSDK.h"
#include "ListenerUI.h"

// Some local variables to get from dialog displays to event IDs
struct ListenerEventArray
{
	int32	longVal;
	char*	strStr;
};

// I put these strings into the menu list at runtime
const struct ListenerEventArray useful[] = {
	eventSave,			"eventSave",
	eventGaussianBlur,	"eventGaussianBlur",
	eventCopy,			"eventCopy",
	eventCrop,			"eventCrop",
	0,					NULL
};


// Some local routine definitions
void EventToText(uint32 inputLong, char* returnString);
uint32 TextToEvent(const char* inputStr);

//-------------------------------------------------------------------------------
//
//	DoUIInit
//
//	Initialize the UI elemets
//
//-------------------------------------------------------------------------------
SPErr DoUIInit(PIDialogPtr dialog, 
			   PIWatchComboBox & watchFor, 
			   PISetComboBox & actionSet, 
			   PIActionComboBox & actionAction, 
			   PIInstalledStaticText & installedText)
{
	SPErr error = kSPNoError;
	
	PIItem item = PIGetDialogItem(dialog, kDWatchFor_editTextPopUp);
	watchFor.Init(item);

	item = PIGetDialogItem(dialog, kDPlaySet_editTextPopUp);
	actionSet.Init(item);

	item = PIGetDialogItem(dialog, kDPlayAction_editTextPopUp);
	actionAction.Init(item);

	item = PIGetDialogItem(dialog, kDList_staticText);
	installedText.Init(item);

	return error;
}

//-------------------------------------------------------------------------------
//
//	PIWatchComboBox::Init
//
//	Initialize the watch for combo box.
//
//-------------------------------------------------------------------------------
void PIWatchComboBox::Init(PIItem in)
{
	SetItem(in);

	Clear();
		
	unsigned short iter = 0;
	while (useful[iter].strStr != NULL)
	{
		AppendItem(useful[iter].strStr);
		
		if (gEventID == 0)
			gEventID = useful[0].longVal;
		iter++;
	}
		
	SetCurrentSelection(0);
}

//-------------------------------------------------------------------------------
//
//	PIWatchComboBox::Update
//
//	When the user changes the watch for combo box this routine will get called.
//	Update the global event ID.
//
//-------------------------------------------------------------------------------
void PIWatchComboBox::Update(void)
{
	char * text = GetCurrentSelection();
	int text_size = strlen(text) + 1;
	DescriptorEventID result = TextToEvent(text);
	if (result != 0)
		gEventID = result;
	if (text != NULL)
		delete [] text;
}

//-------------------------------------------------------------------------------
//
//	PISetComboBox::Init
//
//	Initialize the combo box with a list of the sets from the actions palette.
//
//-------------------------------------------------------------------------------
void PISetComboBox::Init(PIItem in)
{
	SetItem(in);

	if (gActionSet != NULL)
	{
		delete [] gActionSet;
		gActionSet = NULL;
	}
	
	Clear();
		
	// There's a bug in here that makes the count of the classActionSet wrong
	// so you can't do GetInfoFromPhotoshop(classActionSet, keyCount, &count, NULL);
	// So, loop through until GetInfoFromPhotoshopByIndex errors
	uint32 index = 1;
	int32 error = kSPNoError;
	while (error == kSPNoError)
	{
		char name[BigStrMaxLen];
		uint32 length = BigStrMaxLen - 1;
		error = PIUGetInfoByIndex(index,
						 		  classActionSet,
								  keyName,
								  name,
								  &length);
			
		if (error == kSPNoError)
		{
			AppendItem(name);
							
			// Create space and copy the name over
			if (gActionSet == NULL)
			{
				gActionSet = new char[length+1];
				if (gActionSet)
					strcpy(gActionSet, name);
			}
		}
		index++;
	}

	SetCurrentSelection(0);
}

//-------------------------------------------------------------------------------
//
//	PISetComboBox::Update
//
//	When the user changes the actions set combo box this gets called.
//  Update the gActionSet from the pop up menu. 
//
//-------------------------------------------------------------------------------
void PISetComboBox::Update(void)
{
	char * text = GetCurrentSelection();
	int text_size = strlen(text) + 1;
	if (gActionSet != NULL)
		delete [] gActionSet;
	gActionSet = new char[text_size];
	if (gActionSet != NULL)
		strcpy(gActionSet, text);
	if (text != NULL)
		delete [] text;
}

//-------------------------------------------------------------------------------
//
//	PIActionComboBox::Init
//
//	Set up the combo box with the list of actions in the current set
//  This is dependant on the gActionSet being valid from above. Ugh!
//  I call this routine during the UI init phase and if the gActionSet
//  has changed. The only difference being a valid PIItem in. 
//
//-------------------------------------------------------------------------------
void PIActionComboBox::Init(PIItem in)
{
	if (in != NULL)
		SetItem(in);
	
	if (gActionName != NULL)
	{
		delete [] gActionName;
		gActionName = NULL;
	}
	
	Clear();
		
	// There's a bug in here that makes items with single actions inside them
	// not appear correctly.  I'll track it down later:
	int32 actions = 0;
	uint32 index = 1;
	int32 error = kSPNoError;
	while (error == kSPNoError)
	{
		char name[BigStrMaxLen];
		uint32 length;

		error = PIUActionsGetName(gActionSet,
								  index,
							      name,
							      &length);

		if (error == kSPNoError)
		{
			AppendItem(name);
							
			// Create space and copy the name over
			if (gActionName == NULL)
			{
				gActionName = new char[length+1];
				if (gActionName)
					strcpy(gActionName, name);
			}
		}
		index++;
	} // actionsLoop
	
	SetCurrentSelection(0);
}

//-------------------------------------------------------------------------------
//
//	PIActionComboBox::Update
//
//	This gets called when the user changes the action combo box.
//  Set the gActionName to the current setting
//
//-------------------------------------------------------------------------------
void PIActionComboBox::Update(void)
{
	char * text = GetCurrentSelection();
	int text_size = strlen(text) + 1;
	if (gActionName != NULL)
		delete [] gActionName;
	gActionName = new char[text_size];
	if (gActionName)
		strcpy(gActionName, text);
	if (text != NULL)
		delete [] text;
}

//-------------------------------------------------------------------------------
//
//	PIInstalledStaticText::Init
//
//	Set the text of the static item with all of the current installed events.
//
//-------------------------------------------------------------------------------
void PIInstalledStaticText::Init(PIItem in)
{
	SetItem(in);
	
	const char* const displayString = "%s: \"%s\" of \"%s\".\r";
	const unsigned short displayString_size = strlen(displayString);
	char eventAsString[HashStrMaxLen];
	// Figure out the size of all the strings in the listener list:

	unsigned long size = 0;
		
	Listener_t* thisList = gListenerList;
	
	while (thisList != NULL)
	{
		EventToText(thisList->eventID, eventAsString);
		size += strlen(eventAsString);
		size += strlen(thisList->actionName);
		size += strlen(thisList->actionSet);
		size += displayString_size;
		thisList = thisList->next;
	}

	char* display = NULL;
	
	if (size != 0)
	{
		display = new char[size];
	}
	else
	{
		SetText("No notifiers installed.");
		return;
	}
		
	if (display != NULL)
	{
		*display = '\0';// Have to null terminate correctly to start with.
		
		thisList = gListenerList;
		
		while (thisList != NULL)
		{
			EventToText(thisList->eventID, eventAsString);
			char* tempDisplay = NULL;
			const unsigned short tempDisplay_size = strlen(eventAsString) +
				                                    strlen(thisList->actionName) +
													strlen(thisList->actionSet) +
													displayString_size;
			tempDisplay = new char[tempDisplay_size];
			if (tempDisplay != NULL)
			{
				sprintf(tempDisplay, displayString, eventAsString, thisList->actionName, thisList->actionSet );
				strcat(display, tempDisplay );
				delete [] tempDisplay;
			}

			thisList = thisList->next;
		}

		SetText(display);

		delete [] display;
		
	} // display 								
	else
	{
		SetText("Too many notifiers to display.");
	}
}

//-------------------------------------------------------------------------------
//
//	EventToText
//
//	Convert the event 'Hash' code to the string representation of it.
//
//-------------------------------------------------------------------------------
void EventToText(uint32 inputLong, char * returnString)
{
	if (returnString)
	{
		unsigned short index = 0;
		while(useful[index].strStr != NULL)
		{
			if (useful[index].longVal == inputLong)
			{
				strcpy(returnString, useful[index].strStr);
				break;
			}
			index++;
		}
	}
}

//-------------------------------------------------------------------------------
//
//	TextToEvent
//
//	Convert the string to the event 'Hash'
//
//-------------------------------------------------------------------------------
uint32 TextToEvent(const char* inputStr)
{
	uint32 returnLong = 0;
	if (inputStr)
	{
		unsigned short index = 0;
		while(useful[index].strStr != NULL)
		{
			if (strcmp(useful[index].strStr, inputStr) == 0)
			{
				returnLong = useful[index].longVal;
				break;
			}
			index++;
		}
	}
	return (returnLong);
}
// end ListenerUI.cpp