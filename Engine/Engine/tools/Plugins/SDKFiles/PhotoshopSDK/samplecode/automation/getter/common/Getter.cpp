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
//		Getter.cpp
//
//	Description:
//		Get all the information you can from the Photoshop actions system. This
//		plug in uses the PIUActionUtils routines to dump this information to a 
//		log file called "Getter.log".
//	
//		NOTE: The "Getter.log" file makes it look like you can set the
//		information. This is not true. I am only using the PIUActionUtils routine
//		so I don't have to rewrite the same code into a different format.
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#include "PhotoshopSDK.h"

#define OLDP2C 1

#include "Getter.h"

bool HostIsElements(void);

//-------------------------------------------------------------------------------
//	Classes
//-------------------------------------------------------------------------------
// This is based on the AutomationPlugin class found in AutomationPlugin.h. 
// The AutoPluginMain routine is now in AutomationMain.cpp and this class gets 
// created and deleted depending on the messages.
class GetterPlugin : public AutomationPlugin {
private:

	// a message from Photoshop menu or action
	virtual SPErr DoIt(PSActionsPlugInMessage *message);
	virtual ASErr UnloadPlugin(SPInterfaceMessage *message);

	// don't create these classes, there should be only one
	GetterPlugin(); // don't write this one
	GetterPlugin(GetterPlugin &rhs); // don't write this one

public:
	// These are protected, only the Instance routine can call them.
	GetterPlugin(SPPluginRef pluginRef);
	virtual ~GetterPlugin();
};


//-------------------------------------------------------------------------------
//	Globals
//-------------------------------------------------------------------------------
// the basic suite, needed for the new and delete operators
SPBasicSuite *sSPBasic = NULL;

//-------------------------------------------------------------------------------
//
//	GetterPlugin::GetterPlugin
//
//	Derived class constructor. Make sure you initialize the base class.
//
//-------------------------------------------------------------------------------
GetterPlugin::GetterPlugin(SPPluginRef pluginRef) : AutomationPlugin(pluginRef)
{
}


//-------------------------------------------------------------------------------
//
//	GetterPlugin::UnloadPlugin
//
//	Your about to get unloaded from memory. Delete anything dynamically created.
//
//-------------------------------------------------------------------------------
ASErr GetterPlugin::UnloadPlugin(SPInterfaceMessage *message)
{
	return (AutomationPlugin::UnloadPlugin(message));
}


//-------------------------------------------------------------------------------
//
//	GetterPlugin::~GetterPlugin
//
//	Derived class destructor. Nothing to do for this class.
//  You should probably do clean up and memory overhead removal
//  during the UnloadPlugin call
//
//-------------------------------------------------------------------------------
GetterPlugin::~GetterPlugin()
{
}



//-------------------------------------------------------------------------------
//
//	AllocatePlugin
//	
//	The real constructor hear. You have to have one of these so the 
//	AutomationMain.cpp AutoPluginMain routine can create your class.
//
//-------------------------------------------------------------------------------
AutomationPlugin *AllocatePlugin(SPPluginRef pluginRef)
{
	return new GetterPlugin(pluginRef);
}



//-------------------------------------------------------------------------------
//
//	GetterPlugin::DoIt
//
//	Finally something interesting. You have been selected from your menu entry
//	or via the actions system to do your thing. Dump everything you can about
//	Photoshop by asking Photoshop. The output file is "Getter.log".
//
//-------------------------------------------------------------------------------
SPErr GetterPlugin::DoIt(PSActionsPlugInMessage * /*message*/)
{
	SPErr	error = 0;

	char logfilename[256];
	char* filename = "Getter.log";
	
	error = GetFullPathToDesktop(logfilename);
	if (error)
	{
		strcpy(logfilename, "");
		error = 0;
	}
	strcat(logfilename, filename);

	GetApplicationInfo(logfilename);
	GetLayerInfo(logfilename);
	GetActionInfo(logfilename);
	GetDocumentInfo(logfilename);
	GetChannelInfo(logfilename);
	GetBackgroundInfo(logfilename);

	// the above routines can do their job by indexing into the open objects
	// path info and history info are only available to the target document
	// 1. remember who is the current target
	// 2. target the other documents by indexing
	// 3. switch back to the original target 
	//    (use the ID it is the most reliable way to switch back)
	int32 currentDocumentID = 0;
	int32 numDocuments = 0;
	int32 docCounter = 0;

	// we are ignoring errors so this will not give an invalid error
	// message when there are no documents open
	PIUGetInfo(classDocument, keyDocumentID, &currentDocumentID, NULL);

	PIUGetInfo(classApplication, keyNumberOfDocuments, &numDocuments, NULL);

	if (numDocuments > 0)
	{
		for (docCounter = 1; docCounter <= numDocuments; docCounter++)
		{
			// this routine will error if we select the document
			// that is already selected, we don't care about this
			// as an error, keep chugging
			(void) PIUSelectByIndex(classDocument, docCounter);
			GetPathInfo(logfilename);
			GetHistoryInfo(logfilename);
		}
		// this routine will error if we select the document
		// that is already selected, we don't care about this
		// as an error, keep chugging
		(void) PIUSelectByID(classDocument, currentDocumentID);
	}

	return (error);
}

bool HostIsElements(void)
{
	SPErr error = 0; 
	// assume Photoshop
	bool hostIsElements = false;

	PIActionReference reference = NULL;
	DescriptorTypeID runtimeKeyID = 0;
	PIActionDescriptor result = NULL;
	PIActionDescriptor versionDesc = NULL;
	Boolean limitedValue, hasKey;
	int32 versionNum = 0;
	DescriptorClassID classID;

	// get all the keys for classApplication
	error = sPSActionReference->Make(&reference);
	if (error) goto returnError;

	error = sPSActionReference->PutEnumerated(reference, classApplication, typeOrdinal, enumTarget);
	if (error) goto returnError;

	error = sPSActionControl->Get(&result, reference);
	if (error) goto returnError;

	error = sPSActionControl->StringIDToTypeID(klimitedStr, &runtimeKeyID); // "limited"
	if (error) goto returnError;

	// best case scenario is that the classApplication has this key
	error = sPSActionDescriptor->HasKey(result, runtimeKeyID, &hasKey);
	if (error) goto returnError;

	if (hasKey)
	{
		error = sPSActionDescriptor->GetBoolean(result, runtimeKeyID, &limitedValue);
		if (!error)
		{
			hostIsElements = limitedValue != 0;
		}
	}
	else
	{
		error = sPSActionDescriptor->HasKey(result, keyHostVersion, &hasKey);
        if (error) goto returnError;

		if (hasKey)
		{
			error = sPSActionDescriptor->GetObject(result, keyHostVersion, &classID, &versionDesc);
            if (error) goto returnError;

			error = sPSActionDescriptor->HasKey(versionDesc, keyVersionMajor, &hasKey);
            if (error) goto returnError;

			if (hasKey)
			{
				error = sPSActionDescriptor->GetInteger(versionDesc, keyVersionMajor, &versionNum);
				if (error) goto returnError;
				
				hostIsElements = ((versionNum == 1) || (versionNum == 2));
			}
		}
	}

returnError:
	
     if (result != NULL) sPSActionDescriptor->Free(result);
     if (versionDesc != NULL) sPSActionDescriptor->Free(versionDesc);
     if (reference != NULL) sPSActionReference->Free(reference);

	return hostIsElements;
}
// end of Getter.cpp
