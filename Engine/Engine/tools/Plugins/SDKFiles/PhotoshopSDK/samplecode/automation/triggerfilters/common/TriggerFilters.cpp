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
//		TriggerFilters.cpp
//
//	Description:
//		This file contains the source code and routines for the
//		Actions module TriggerFilters, an example of a module
//		that uses ADM and the Actions suite to execute events
//		from the Filter menu.
//
//	Use:
//		This is a good example of a basic shell to pop a cross-
//		platform UI using ADM and trigger basic events.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "TriggerFilters.h"

SPBasicSuite * sSPBasic = NULL;
SPPluginRef gPlugInRef = NULL;

AutoSuite<ADMBasicSuite3> sADMBasic(kADMBasicSuite, kADMBasicSuiteVersion3);
AutoSuite<ADMDialogSuite5> sADMDialog(kADMDialogSuite, kADMDialogSuiteVersion5);
AutoSuite<ADMItemSuite> sADMItem(kADMItemSuite, kADMItemSuiteVersion);
AutoSuite<ADMNotifierSuite> sADMNotify(kADMNotifierSuite, kADMNotifierSuiteVersion);
AutoSuite<ADMListSuite> sADMList(kADMListSuite, kADMListSuiteVersion);
AutoSuite<ADMEntrySuite> sADMEntry(kADMEntrySuite, kADMEntrySuiteVersion);
AutoSuite<ADMTrackerSuite> sADMTrack(kADMTrackerSuite, kADMTrackerSuiteVersion);
AutoSuite<ADMDrawerSuite> sADMDrawer(kADMDrawerSuite, kADMDrawerSuiteVersion);


//-------------------------------------------------------------------------------
//	Globals -- Define global variables for plug-in scope.
//-------------------------------------------------------------------------------

char gOtherEvent[kMaxStr255Len] = "";
const unsigned short kMacMaxFileSpecNameLength = 64;

// Both cannot be on at once, but both can be off, indicating show dialog
// for event ONCE, then go silent after that.
bool gLogToFile = false;
SPPlatformFileSpecification gFile;

groupList_t* gLastGroupList 	= NULL;
groupList_t* gGroupList 		= NULL;
unsigned short gGroupList_size = 0;

showDialog_t gShowDialog = kShowDialogOnce;

typedef enum PIUFilePosition_t
	{
	kFromStart = 0,
	kFromPosition,
	kFromEnd
	} PIUFilePosition_t;
#define kPSErrorUnknownStringID	'!sid'
	
//-------------------------------------------------------------------------------
//	Prototypes.
//-------------------------------------------------------------------------------

// Initialize our parameters:
static void Initialize (void);
static SPErr ClearFileSpec(SPPlatformFileSpecification* fileSpec);
static SPErr ForceFileSpec(const char* const fileName, SPPlatformFileSpecification* fileSpec);
static long FileOpen(SPPlatformFileSpecification* fileSpec);
static SPErr FileSeek(const long fileRefNum,const PIUFilePosition_t	relativePosition,const long	byteOffset);
static SPErr FileWrite(const long		fileRefNum,const char* const		string,unsigned long*			inOutCount);
static SPErr FileClose(long*			fileRefNum);
//-------------------------------------------------------------------------------
//
//	AutoPluginMain / main
//
//	All calls to the plug-in module come through this routine.
//	It must be placed first in the resource.  To achieve this,
//	most development systems require this be the first routine
//	in the source.
//
//	The entrypoint will be "pascal void" for Macintosh,
//	"void" for Windows.
//
//-------------------------------------------------------------------------------
DLLExport SPAPI SPErr AutoPluginMain(const char* caller, const char* selector, void* message)
{

	SPErr error = kSPNoError;

	SPMessageData * basicMessage = (SPMessageData *) message;

	sSPBasic = basicMessage->basic;
	
	gPlugInRef = basicMessage->self;

	if (sSPBasic->IsEqual(caller, kSPInterfaceCaller))
	{
		if (sSPBasic->IsEqual(selector, kSPInterfaceAboutSelector))
		{
			DoAbout(basicMessage->self, AboutID);
		}
	}
	
	if (sSPBasic->IsEqual(caller, kPSPhotoshopCaller))
	{
		if (sSPBasic->IsEqual(selector, kPSDoIt))
		{
			PSActionsPlugInMessage * actionsMessage = (PSActionsPlugInMessage *) message;
			PIActionParameters * actionParams = actionsMessage->actionParameters;
			error = Execute (actionParams);
		}
	}

	PIUSuitesRelease();

	sADMBasic.Unload();
	sADMDialog.Unload();
	sADMItem.Unload();
	sADMNotify.Unload();
	sADMList.Unload();
	sADMEntry.Unload();
	sADMTrack.Unload();
	sADMDrawer.Unload();

	return error;
}

//-------------------------------------------------------------------------------
//
//	Execute
//
//	Main routine.
//
//-------------------------------------------------------------------------------
SPErr Execute(PIActionParameters* actionParams)
	{
	SPErr error = kSPNoError;
	
	Initialize(); // Reset parameters.
	
	MakeGroupList();
	
	ReadScriptParams(actionParams);
	
	// Determine if we need to pop our dialog:
	PIDialogPlayOptions playInfo = actionParams->playInfo;
	
	// Only interested in whether the host told us to pop the dialog.  If
	// its dialogDontDisplay or dialogSilent we'll act the same: silent.
	if (playInfo != plugInDialogDisplay)
		{
		// Force dialog to be silent:
		gShowDialog = kShowDialogNever;
		unsigned long ticks = 0;
		
		error = PlayFilter(&(gLastGroupList->lastEventList->descriptor), &ticks);
		DisplayResults(NULL, ticks, error);
		
		if (error == kSPNoError)
			WriteScriptParams(actionParams);		
		}
	else
		{
		error = DoUI();
		
		if (error == kSPNoError)
			WriteScriptParams(actionParams);
		// else if (error == userCanceledErr) return it so we don't record
		}
	
	FreeGroupList();
	
	return error;
	
	}	

//-------------------------------------------------------------------------------
//
//	Initialize
//
//	Reset parameters.
//
//-------------------------------------------------------------------------------
static void Initialize (void)
	{
	gOtherEvent[0] = '\0';
	gOtherEvent[1] = '\0';
	
	gLogToFile = false;
	
	ClearFileSpec(&gFile);
	
	gLastGroupList = NULL;
	gGroupList = NULL;
	gGroupList_size = 0;
	
	gShowDialog = kShowDialogOnce;
	}
//-------------------------------------------------------------------------------
static SPErr ClearFileSpec
	(
	/* IN/OUT */	SPPlatformFileSpecification* fileSpec
	)
	{
	SPErr error = kSPNoError;
	
	if (fileSpec != NULL)
		{
		#ifdef __PIMac__
			fileSpec->vRefNum = 0;
			fileSpec->parID = 0;
			fileSpec->name[0] = 0;
			fileSpec->name[1] = 0;
		#elif defined(__PIWin__)
			fileSpec->path[0] = 0;
		#endif
		}
	else
		{
		error = kSPBadParameterError;
		}
		
	return error;
	}	


//-------------------------------------------------------------------------------
//
//	MakeGroupList
//
//	Create group linked list menu, used for management and pop-up menu.  Once
//	this is created, it should be hung of the *data structure passed in the
//	messageData so it doesn't have to be recreated every time.  But, just to
//	exercise the API, I'm going to completely create and delete this list
//	every single execution.
//
//-------------------------------------------------------------------------------
SPErr MakeGroupList(void)
	{
	SPErr error = kSPNoError;
	
	// We have a special way to do this.  We've stored menu items in string
	// resources to make for easy changes.  The string resource will always
	// have this format:
	// Group Name|Resource ID|
	// -|0|
	// +
	//
	// If there is a + at the end, it will read into the next String
	// resource.
	// If there is a - for the name and 0 for the id, it will create a separator
	// there.
	//
	// All records must be delimited by "|", even the last one.
	
	bool done = false;
	
	groupList_t*	groupList = gGroupList;
	groupList_t*	newGroupList = NULL;
	
	const unsigned short 	groupList_size = sizeof(groupList_t);
	
	short id = kGroupMenuID;
	char convertString[kMaxStr255Len] = "";
		
	while (!done && (error == kSPNoError) && (id != 0))
		{
		char data[256] = "";
		sADMBasic->GetIndexString(gPlugInRef, id, 1, data, 256);

		unsigned short size = strlen(data);
		unsigned short position = 0;
		unsigned short partSize = 0;
			
		char* found = NULL;
			
		bool done2 = false;

		while (!done2 && (error == kSPNoError) && (position < size))
			{
			found = strstr(&data[position], kDelim);

			if (found != NULL)
				{
				// Lets make a block and cast it as an
				// eventList record and use it for this
				// info:
				newGroupList = new groupList_t;
						
				if (error == kSPNoError && newGroupList != NULL)
					{
					newGroupList->index = 0;
					newGroupList->name = NULL;
					newGroupList->id = NULL;
					newGroupList->eventList = NULL;
					newGroupList->size = 0;
					newGroupList->lastEventList = NULL;
					newGroupList->eventListDialogItem = NULL;
					newGroupList->next = NULL;
						
					partSize = found - &data[position];
					newGroupList->name = new char[partSize+1];
					if (newGroupList->name != NULL)
						{
						strncpy(newGroupList->name, &data[position], partSize);
						newGroupList->name[partSize] = 0;
						}

							
					position += partSize + 1; // Past delim.
						
					found = strstr(&data[position], kDelim);
						
					if (found != NULL)
						{
						float returnValue = 0.0;
						
						partSize = found - &data[position];
						strncpy(convertString, &data[position], partSize);
						convertString[partSize] = 0;

						sADMBasic->StringToValue
							(
							convertString,
							&returnValue,
							kADMNoUnits
							);
							
						newGroupList->id = (short)returnValue;
							
						position += partSize + 1; // Past delim.
							
						newGroupList->index = gGroupList_size;
							
						if (newGroupList->id != 0)
							{
							newGroupList->eventList = MakeFilterList
								(
								newGroupList->id,
								&(newGroupList->size)
								);
							}
							
						// Advance to next index:
						++gGroupList_size;
						
						// Fix the global pointer:	
						if (gGroupList == NULL)
							gGroupList = newGroupList;
						
						if (gLastGroupList == NULL)
							gLastGroupList = newGroupList;
								
						// Default to first record:
						newGroupList->lastEventList = newGroupList->eventList;
							
						// Advance the groupList pointer:
						if (groupList != NULL)
							groupList->next = newGroupList;
						
						groupList = newGroupList;
						newGroupList = NULL;
						
						if (position >= size)
							{
							done2 = true;
							if (strcmp(&data[size-1], (char*)kNext) == 0)
								{
								++id;
								}
							else
								{ // Done with both inner and outer loop.
								done = true;
								}
							}
						}
					else
						{
						// Failed to get ID.  This record
						// is invalid, so destroy it and return an error.
						error = kSPBadParameterError;
						if (newGroupList->name != NULL)
							{
							delete [] newGroupList->name;
							newGroupList->name = NULL;
							}
							
						if (newGroupList != NULL)
							{
							delete newGroupList;
							newGroupList = NULL;
							}
						}
						
					} // allocate block
				
				}
			else
				{
				// Found no match for the first delimiter, so
				// check for a "next resource" condition:
				if (strcmp(&data[size-1], (char*)kNext) == 0)
					{
					++id;
					done2 = true;
					}
				else
					{ // Done with both inner and outer loop.
					done2 = true;
					done = true;
					}
				}
			
			} // done2
		
		}	
	
	return error;
	
	}

//-------------------------------------------------------------------------------
//
//	MakeFilterList
//
//	Create filter linked list menu, used for management and pop-up menu.  Once
//	this is created, it should be hung of the *data structure passed in the
//	messageData so it doesn't have to be recreated every time.  But, just to
//	exercise the API, I'm going to completely create and delete this list
//	every single execution.
//
//-------------------------------------------------------------------------------
eventList_t* MakeFilterList(short id, unsigned short* index)
	{
	SPErr error = kSPNoError;
	
	// We have a special way to do this.  We've stored menu items in string
	// resources to make for easy changes.  The string resource will always
	// have this format:
	// Filter Name|ID  |
	// Filter Name#2|ID2 |
	// -|-|
	// +
	//
	// If there is a + at the end, it will read into the next String
	// resource.
	// If there is a - for the name and the id, it will create a separator
	// there.
	
	bool done = false;
	
	eventList_t*	returnEventList = NULL;
	eventList_t*	eventList = NULL;
	eventList_t*	newEventList = NULL;
	
	const unsigned short	eventList_size = sizeof(eventList_t);
	
	*index = 0;
			
	while (!done && (error == kSPNoError) && (id != 0))
		{
		char data[256] = "";
		sADMBasic->GetIndexString(gPlugInRef, id, 1, data, 256);
		
		unsigned short size  = strlen(data);
		unsigned short partSize = 0;
		unsigned short position = 0;
		char* found = NULL;
		bool done2 = false;

		while (!done2 && (error == kSPNoError) && (position < size))
			{
			found = strstr(&data[position], kDelim);
					
			if (found != NULL)
				{
				// Lets make a block and cast it as an
				// eventList record and use it for this
				// info:
				newEventList = new eventList_t;
						
				if (error == kSPNoError && newEventList != NULL)
					{
					newEventList->index = NULL;
					newEventList->name = NULL;
					newEventList->id = NULL;
					newEventList->descriptor = NULL;
					newEventList->next = NULL;
					
					partSize = found - &data[position];

					newEventList->name = new char[partSize+1];
					if (newEventList->name != NULL)
						{
						strncpy(newEventList->name, &data[position], partSize);
						newEventList->name[partSize] = 0;
						}
							
					position += partSize + 1; // Past delim.
						
					found  = strstr(&data[position], kDelim);
						
					if (found != NULL)
						{	
						partSize = found - &data[position];
						newEventList->id = new char[partSize+1];
						if (newEventList->id != NULL)
							{
							strncpy(newEventList->id, &data[position], partSize);
							newEventList->id[partSize] = 0;
							}
							
						position += partSize + 1; // Past delim.
							
						newEventList->index = (unsigned short) *index;
							
						// Advance to next index:
						++(*index);
							
						// Fix the global pointer:	
						if (returnEventList == NULL)
							returnEventList = newEventList;
							
						// Advance the eventList pointer:
						if (eventList != NULL)
							eventList->next = newEventList;
							
						eventList = newEventList;
						newEventList = NULL;
							
						if (position >= size)
							{
							done2 = true;
							if (strcmp(&data[size-1], (char*)kNext) == 0)
								{
								++id;
								}
							else
								{ // Done with both inner and outer loop.
								done = true;
								}
							}
						}
					else
						{
						// Failed to get ID.  This record
						// is invalid, so destroy it and return an error.
						error = kSPBadParameterError;
						
						if (newEventList->name != NULL)
							{
							delete [] newEventList->name;
							newEventList->name = NULL;
							}
							
						if (newEventList != NULL)
							{
							delete newEventList;
							newEventList = NULL;
							}
						}
						
					} // allocate block
				
				}
			else
				{
				// Found no match for the first delimiter, so
				// check for a "next resource" condition:
				if (strcmp(&data[size-1], (char*)kNext) == 0)
					{
					++id;
					done2 = true;
					}
				else
					{ // Done with both inner and outer loop.
					done2 = true;
					done = true;
					}
				}
			
			} // done2
		
		}	
	
	return returnEventList;
	
	}
	
//-------------------------------------------------------------------------------
//
//	FreeGroupList
//
//	Cleans up and disposes all in the group list.
//
//-------------------------------------------------------------------------------
void FreeGroupList (void)
	{
	groupList_t* groupList = gGroupList;
	
	while (groupList != NULL)
		{
		if (groupList->name != NULL)
			{
			delete [] groupList->name;
			groupList->name = NULL;
			}
			
		if (groupList->eventList != NULL)
			{
			FreeFilterList(&(groupList->eventList));
			groupList->eventList = NULL;
			}
			
		// We don't free lastEventList because that just points
		// somewhere in the middle of the list.
		
		groupList_t* nextGroupList = groupList->next;
		
		delete groupList;
		groupList = nextGroupList;
		}

	gGroupList = NULL;
	gLastGroupList = NULL;
	gGroupList_size = 0;
	
	}

//-------------------------------------------------------------------------------
//
//	FreeFilterList
//
//	Cleans up and disposes all in the filter list.
//
//-------------------------------------------------------------------------------
void FreeFilterList (eventList_t** beginEventList)
	{
	eventList_t* eventList = *beginEventList;
	
	while (eventList != NULL)
		{
		if (eventList->name != NULL)
			{
			delete [] eventList->name;
			eventList->name = NULL;
			}
			
		if (eventList->id != NULL)
			{
			delete [] eventList->id;
			eventList->id = NULL;
			}
			
		if (eventList->descriptor != NULL)
			{
			sPSActionDescriptor->Free(eventList->descriptor);
			eventList->descriptor = NULL;
			}
			
		eventList_t* nextEventList = eventList->next;
		
		delete eventList;
		eventList = nextEventList;
		}
		
	*beginEventList = NULL;
	}

//-------------------------------------------------------------------------------
//
//	DoUndo
//
//	Generic routine to trigger an undo event.
//
//-------------------------------------------------------------------------------
void DoUndo (void)
	{
	PIActionDescriptor			resultDescriptor = NULL;
	const DescriptorEventID		event = 'undo';
	const PIActionDescriptor	eventDescriptor = NULL;
	const PIDialogPlayOptions	eventOptions = plugInDialogSilent;

	SPErr error = sPSActionControl->Play
		(
		&resultDescriptor,
		event,
		eventDescriptor,
		eventOptions
		);

	if (resultDescriptor != NULL)
		sPSActionDescriptor->Free(resultDescriptor);
	}

//-------------------------------------------------------------------------------
//
//	PlayFilter
//
//	When user clicks Doit button, dispatch filter that is selected in pop-up.
//
//-------------------------------------------------------------------------------
SPErr PlayFilter (PIActionDescriptor* descriptor, unsigned long* ticks)
	{
	SPErr error = kSPNoError;

	PIActionDescriptor resultDescriptor;
	PIDialogPlayOptions eventOptions = plugInDialogDisplay;
	
	if (*descriptor != NULL && gShowDialog != kShowDialogAlways)
		eventOptions = plugInDialogDontDisplay;
		
	if (gShowDialog == kShowDialogNever)
		eventOptions = plugInDialogSilent;
	
	// We're going to time this, because we can.  I'll start the
	// ticks here, but it would be great if we're able to get
	// this closer to the Play command itself...	
	unsigned long timerStart = sPSUIHooks->TickCount ();
	unsigned long timerStop = timerStart; // At least for now.
	
	if (gLastGroupList != NULL &&	sPSActionControl != NULL 
		&&	sPSActionDescriptor != NULL) 
		{	
		char* id = gLastGroupList->lastEventList->id;
		
		if (strcmp(id, otherKey) == 0)
			{ // Other is different.  We use user-supplied event.
			id = gOtherEvent;
			}
		
		const unsigned short size  = strlen(id);
		DescriptorEventID runtimeID = eventNull;
	
		if (size > 0)
			{ // Got something.  Get a runtime id.
			
			if (size > 4)
				{ // Its a string.  Find a runtime id.
			
				error = sPSActionControl->StringIDToTypeID
					(
					id,
					&runtimeID
					);
					
				if (error != kSPNoError)
					{
					error = kPSErrorUnknownStringID;
					runtimeID = eventNull;
					}
				}
			else	// It's a 4-character code.
				{
				runtimeID = PIUCharToID(id);
				}
				
			if (runtimeID != eventNull)
				{
				// Cool, do it, and start our timer again:
				timerStart = sPSUIHooks->TickCount();
				
				error = sPSActionControl->Play
					(
					&resultDescriptor,
					runtimeID,
					*descriptor,
					eventOptions
					);
					
				timerStop = sPSUIHooks->TickCount();
				
				}
			}
		if (error == kSPNoError)
			{
			if (*descriptor != NULL)
				sPSActionDescriptor->Free(*descriptor);

			*descriptor = resultDescriptor;
			resultDescriptor = NULL;	
			}
		else if (resultDescriptor != NULL)
			sPSActionDescriptor->Free(resultDescriptor);
				
		} // index
	
		*ticks = timerStop - timerStart;
		return error;
				
	} // end PlayFilter

//-------------------------------------------------------------------------------
//
//	FindIDInEventList
//
//	This will find an ID in the event list.
//
//-------------------------------------------------------------------------------
eventList_t* FindIDInEventList
	(
	/* IN */		const char* const id,
	/* IN */		eventList_t* eventList
	)
	{
	eventList_t* eventListFound = NULL;
	
	while (eventListFound == NULL && eventList != NULL)
		{
		if (strcmp((char*)id, eventList->id) == 0)
			{ // Found it.
			eventListFound = eventList;
			}
		else
			{
			eventList = eventList->next;
			}
		}

	return eventListFound;
	}
		
//-------------------------------------------------------------------------------
//
//	FindNameInEventList
//
//	This will find a name in the event list.
//
//-------------------------------------------------------------------------------
eventList_t* FindNameInEventList
	(
	/* IN */		const char* const name,
	/* IN */		eventList_t* eventList
	)
	{
	eventList_t* eventListFound = NULL;
	
	while (eventListFound == NULL && eventList != NULL)
		{
		if (strcmp((char*)name, eventList->name) == 0)
			{ // Found it.
			eventListFound = eventList;
			}
		else
			{
			eventList = eventList->next;
			}
		}

	return eventListFound;
	}

//-------------------------------------------------------------------------------
//
//	FindIndexInEventList
//
//	This will find an index in the event list.
//
//-------------------------------------------------------------------------------
eventList_t* FindIndexInEventList
	(
	/* IN */		const unsigned short index,
	/* IN */		eventList_t* eventList
	)
	{
	eventList_t* eventListFound = NULL;
	
	while (eventListFound == NULL && eventList != NULL)
		{
		if (index == eventList->index)
			{ // Found it.
			eventListFound = eventList;
			}
		else
			{
			eventList = eventList->next;
			}
		}

	return eventListFound;
	}

//-------------------------------------------------------------------------------
//
//	FindIDInGroupList
//
//	This will find an ID in the group list.
//
//-------------------------------------------------------------------------------
groupList_t* FindIDInGroupList
	(
	/* IN */		const short id
	)
	{
	groupList_t* groupList = gGroupList;
	groupList_t* groupListFound = NULL;
	
	while (groupListFound == NULL && groupList != NULL)
		{
		if (id == groupList->id)
			{ // Found it.
			groupListFound = groupList;
			}
		else
			{
			groupList = groupList->next;
			}
		}

	return groupListFound;
	}
		
//-------------------------------------------------------------------------------
//
//	FindNameInGroupList
//
//	This will find a name in the group list.
//
//-------------------------------------------------------------------------------
groupList_t* FindNameInGroupList
	(
	/* IN */		const char* const name
	)
	{
	groupList_t* groupList = gGroupList;
	groupList_t* groupListFound = NULL;
	
	while (groupListFound == NULL && groupList != NULL)
		{
		if (strcmp((char*)name, groupList->name) == 0)
			{ // Found it.
			groupListFound = groupList;
			}
		else
			{
			groupList = groupList->next;
			}
		}

	return groupListFound;
	}

//-------------------------------------------------------------------------------
//
//	FindIndexInGroupList
//
//	This will find an index in the group list.
//
//-------------------------------------------------------------------------------
groupList_t* FindIndexInGroupList
	(
	/* IN */		const unsigned short index
	)
	{
	groupList_t* groupList = gGroupList;
	groupList_t* groupListFound = NULL;
	
	while (groupListFound == NULL && groupList != NULL)
		{
		if (index == groupList->index)
			{ // Found it.
			groupListFound = groupList;
			}
		else
			{
			groupList = groupList->next;
			}
		}

	return groupListFound;
	}
	
//-------------------------------------------------------------------------------
//
//	AppendToFile
//
//	This will save a string to the end of a file.
//
//-------------------------------------------------------------------------------
SPErr AppendToFile (const char* const string)
	{
	SPErr error = kSPNoError;
	
	#ifdef __PIWin__
		const char crlf[] = "\r\n";
	#else
		const char crlf[] = "\r";
	#endif

	unsigned long length = strlen(crlf);
	
	char filename[256] = "";
	sADMBasic->GetIndexString(gPlugInRef, kQueryDefaultFilenameID, 1, filename, 256);

	ForceFileSpec(filename, &gFile);
			
	long fileRefNum = FileOpen(&gFile);
		
	if (fileRefNum != 0)
		{
		error = FileSeek(fileRefNum, kFromEnd, 0);	// Append to end.
	
		if (error == kSPNoError)
			{
			error = FileWrite(fileRefNum, crlf, &length);
			if (error == kSPNoError)
				{
				length = strlen(string);
				error = FileWrite(fileRefNum, string, &length);
				}
			}
			
		// We may have had errors above, but try to close
		// the file anyway:
		error = FileClose(&fileRefNum);
		}
	
	return error;
	
	}

//-------------------------------------------------------------------------------
//
//	ValidFileSpec.
//
//	Returns whether a SPPlatformFileSpec is valid.
//
//-------------------------------------------------------------------------------
bool ValidFileSpec
	(
	/* IN */	const SPPlatformFileSpecification fileSpec
	)
	{
	bool isValid = false;
	
	#ifdef __PIMac__
	
	if (fileSpec.name[0] > 0 &&
		fileSpec.vRefNum != 0)
		isValid = true;
	
	#elif defined(__PIWin__)
	
	if (strlen(fileSpec.path) > 0)
		isValid = true;
	
	#endif
	
	return isValid;
	}
	
//-------------------------------------------------------------------------------
//
//	ForceFileSpec
//
//	Forces a PlatformFileSpec to a default file name, if the spec is not
//	valid.
//
//-------------------------------------------------------------------------------
static SPErr ForceFileSpec
	(
	/* IN */		const char* const fileName,
	/* IN/OUT */	SPPlatformFileSpecification* fileSpec
	)
	{
	SPErr error = kSPNoError;

	if (fileSpec != NULL && fileName != NULL)
		{
		if (!ValidFileSpec(*fileSpec))
			{
			// Not valid.  Do this.

			unsigned short length = strlen(fileName);

			#ifdef __PIMac__

				// Check length:
				if (length >= kMacMaxFileSpecNameLength)
					length = kMacMaxFileSpecNameLength-1;

				// Name is pascal string so length byte is byte 0:
				strncpy((char*)&fileSpec->name[1], fileName, length);
				fileSpec->name[0] = length;

				// If we have room, put a null at the end for safety:
				if (length < kMacMaxFileSpecNameLength)
					fileSpec->name[length+1] = 0;

			#elif defined(__PIWin__)

				// Check length:
				if (length >= kMaxPathLength)
					length = kMaxPathLength-1;

				// Name is a C string:
				strncpy(fileSpec->path, fileName, length);
				fileSpec->path[length] = 0;

			#endif

			} // ValidFileSpec.
		}
	else
		{
		error = kSPBadParameterError;
		}

	return error;
	}

//-------------------------------------------------------------------------------
//
//	FileOpen
//
//	Open existing file or create if none.  Will return a file handle for
//	windows, a file reference number for Mac, or 0 if failed.  Will even
//	try to massage the fileSpec if it can't find the file.
//
//-------------------------------------------------------------------------------
static long FileOpen
	(
	/* IN/OUT */	SPPlatformFileSpecification* fileSpec
	)
	{
	long	fileRefNum = 0;
	
	#ifdef __PIMac__
		OSErr error;

		// I'm having trouble getting the file created on OS X, 
		// this should create the file if one doesn't exist
		char fullPathToFile[256];
		Str255 fileName = "";
		Str255 directoryName = "";
		error = FileSpecToFullPath((FSSpec&)*fileSpec, directoryName, fullPathToFile);
		if (error) return error;
		FILE * f = fopen(fullPathToFile, "a");
		if (f) fclose(f);

		// now change the creator and type because fopen can't do that
		FInfo finderInfo;
		FSpGetFInfo( (const FSSpec *) fileSpec, &finderInfo );
		finderInfo.fdCreator = 'ttxt';
		finderInfo.fdType = 'TEXT';
		FSpSetFInfo( (const FSSpec *) fileSpec, &finderInfo );
			
		short new_vRefNum = 0;
		short result = 0;
		Boolean didOpen = PIOpenFile((FSSpec &)*fileSpec, &new_vRefNum, &result);
		if (!didOpen) return 0;
		if (result != noErr) return result;
		fileRefNum = new_vRefNum;
			
	#elif defined (__PIWin__)
		LPCSTR lpFileName								= fileSpec->path;
		DWORD dwDesiredAccess							= GENERIC_WRITE | GENERIC_READ;
		DWORD dwShareMode								= 0;
		LPSECURITY_ATTRIBUTES lpSecurityAttributes		= NULL;
		DWORD dwCreationDistribution					= OPEN_ALWAYS;
		DWORD dwFlagsAndAttributes						= FILE_ATTRIBUTE_NORMAL;
		HANDLE hTemplateFile							= NULL;
		
		HANDLE h = CreateFile
			(
			lpFileName,
			dwDesiredAccess,
			dwShareMode,
			lpSecurityAttributes,
			dwCreationDistribution,
			dwFlagsAndAttributes,
			hTemplateFile
			);
			
		if (h == INVALID_HANDLE_VALUE)
			{
			// Couldn't create.  Try removing all path info and creating
			// the file in the current position:
			_splitpath
				(
				fileSpec->path,
				NULL,			// drive,
				NULL,			// directory,
				fileSpec->path,	// filename
				NULL			// extension
				);
			
			h = CreateFile
				(
				lpFileName,
				dwDesiredAccess,
				dwShareMode,
				lpSecurityAttributes,
				dwCreationDistribution,
				dwFlagsAndAttributes,
				hTemplateFile
				);
			}
			
		if (h != INVALID_HANDLE_VALUE)
			{
			fileRefNum = (long)h;
			}
		
	#endif // Win
	
	return fileRefNum;
	
	} // end PIUFileOpen
//-------------------------------------------------------------------------------
//
//	FileSeek
//
//	Seek to a file position, relative to the PIUFilePosition.
//
//-------------------------------------------------------------------------------
static SPErr FileSeek
	(
	/* IN */	const long 		fileRefNum,
	/* IN */	const PIUFilePosition_t	relativePosition,
	/* IN */	const long				byteOffset
	)
	{
	SPErr error = kSPNoError;
	
	if (fileRefNum != 0)
		{	
		#ifdef __PIMac__
		
			short fRefNum = (short)fileRefNum;
			short posMode = 0;
			
			switch (relativePosition)
				{
				case kFromStart:
					posMode = fsFromStart;
					break;
					
				case kFromPosition:
					posMode = fsFromMark;
					break;
				
				case kFromEnd:
					posMode = fsFromLEOF;
					break;
				
				default:
					error = kSPBadParameterError;
					break;
				}
			
			if (error == kSPNoError)
				error = SetFPos(fRefNum, posMode, byteOffset);
				
		#elif defined(__PIWin__)
		
			HANDLE hFile				= (HANDLE)fileRefNum;
			LONG lDistanceToMove		= byteOffset;
			PLONG lpDistanceToMoveHigh  = NULL;
			DWORD dwMoveMethod = 0;

			switch (relativePosition)
				{
				case kFromStart:
					dwMoveMethod = FILE_BEGIN;
					break;
					
				case kFromPosition:
					dwMoveMethod = FILE_CURRENT;
					break;
				
				case kFromEnd:
					dwMoveMethod = FILE_END;
					break;
				
				default:
					error = kSPBadParameterError;
					break;
				}
			
			if (error == kSPNoError)
				{
				DWORD pos = SetFilePointer
					(
					hFile,
					lDistanceToMove,
					lpDistanceToMoveHigh,
					dwMoveMethod
					);
				
				if (pos == 0xFFFFFFFF)
					error = (long)GetLastError();
				}
			
		#endif
		} // fileRefNum
	else
		{
		error = kSPBadParameterError;
		}
	
	return error;
	
	} // end FileSeek
//-------------------------------------------------------------------------------
//
//	FileWrite
//
//	Write to file at current position, returning the count of
//	how many bytes were written.
//
//-------------------------------------------------------------------------------
static SPErr FileWrite
	(
	/* IN */		const long		fileRefNum,
	/* IN */		const char* const		string,
	/* IN/OUT */	unsigned long*			inOutCount
	)
	{
	SPErr error = kSPNoError;
	
	if (fileRefNum != 0 && string != NULL && inOutCount != NULL)
		{
		if (*inOutCount > 0)
			{
			#ifdef __PIMac__
		
				short fRefNum = (short)fileRefNum;
				long ioCount = (long)(*inOutCount);
			
				error = FSWrite(fRefNum, &ioCount, string);
				*inOutCount = (unsigned long)ioCount;
			
			#elif defined(__PIWin__)
			
				HANDLE hFile					= (HANDLE)fileRefNum;
				LPCVOID lpBuffer				= string;
				DWORD nNumberOfBytesToWrite		= (DWORD)*inOutCount;
				DWORD nNumberOfBytesWritten		= 0;
				LPDWORD lpNumberOfBytesWritten	= &nNumberOfBytesWritten;
				LPOVERLAPPED lpOverlapped		= NULL;

				BOOL success = WriteFile
					(
					hFile,
					lpBuffer,
					nNumberOfBytesToWrite,
					lpNumberOfBytesWritten,
					lpOverlapped
					);

				if (success)
					{
					*inOutCount = (long)nNumberOfBytesWritten;
					}
				else
					{
					error = (long)GetLastError();
					}
			#endif
			}
		else
			{ // inOutCount < 1:
			error = kSPBadParameterError;
			}
		}
	else
		{ // fileRef | string | inOutCount pointer bad:
		error = kSPBadParameterError;
		}
		
	return error;
	
	} // end FileWrite		
//-------------------------------------------------------------------------------
//
//	FileClose
//
//	Closes an open file, resetting its ref number:
//
//-------------------------------------------------------------------------------
static SPErr FileClose
	(
	/* IN/OUT */	long*			fileRefNum
	)
	{
	SPErr error = kSPNoError;
	
	if (*fileRefNum != 0 /* No file */)
		{
		#ifdef __PIMac__
			short fRefNum = (short)(*fileRefNum);
			error = FSClose(fRefNum);
			
		#elif defined __PIWin__
		
			HANDLE hObject = (HANDLE)(*fileRefNum);
			BOOL success = CloseHandle(hObject);
			
			if (!success)
				error = GetLastError();
			
		#endif
		
		*fileRefNum = 0; // Reset
		
		}
	else
		{
		error = kSPBadParameterError;
		}
		
	return error;
	
	} // end PIUFileClose
// end TriggerFilters.cpp
