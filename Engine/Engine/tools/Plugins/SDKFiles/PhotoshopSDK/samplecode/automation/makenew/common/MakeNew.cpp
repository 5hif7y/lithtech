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
//		MakeNew.cpp
//
//	Description:
//		This makes a new document.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "MakeNew.h"

SPBasicSuite			*sSPBasic = NULL;
SPPluginRef				gPlugInRef = NULL;


//-------------------------------------------------------------------------------
//	Globals -- Define global variables for plug-in scope.
//-------------------------------------------------------------------------------
int32 gWidth;
int32 gHeight;
int32 gResolution;
DescriptorEnumID gFill; // white, transparent, background
DescriptorClassID gMode; // RGB, CMYK, etc.

//-------------------------------------------------------------------------------
//	Prototypes.
//-------------------------------------------------------------------------------

// Initialize our parameters:
static void Initialize (void);

// Convert from any base to unitDistance:
static double ConvertToUnitDistance(const double amount, const double amountBasePerInch);
	
// Actually make a new document:
static SPErr MakeNewDocument (void);

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
	SPErr error	= kSPNoError;

	SPMessageData * basicMessage = (SPMessageData *) message;
	
	sSPBasic = basicMessage->basic;
	
	gPlugInRef = basicMessage->self;

	if (sSPBasic->IsEqual(caller, kSPInterfaceCaller))
	{
		if (sSPBasic->IsEqual(selector, kSPInterfaceAboutSelector))
		{
			DoAbout(gPlugInRef, AboutID);
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
	
	Initialize(); // Default parameters for all globals.
	
	ReadScriptParams(actionParams); // Override globals with new descriptor info.
	
	// Determine if we need to pop our dialog:
	PIDialogPlayOptions playInfo = actionParams->playInfo;
	
	if (playInfo == plugInDialogDisplay)// Others = plugInDialogDontDisplay / plugInDialogSilent
		{	// Go ahead and display a dialog:
		error = DoUI();
		}
	
	if (error == kSPNoError)
			error = MakeNewDocument();
	if (error == kSPNoError)
			WriteScriptParams( actionParams );
			
	// else if (error == userCanceledErr) return it so we don't record.
		
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
	gResolution = 72;
	gWidth = gResolution * 4;
	gHeight = gResolution * 3;
	gFill = enumWhite;
	gMode = classRGBColorMode;	
}

//-------------------------------------------------------------------------------
//
//	ConvertToUnitDistance
//
//	Converts a distance and returns it in standard
//	unitDistance units (72 dpi).
//
//-------------------------------------------------------------------------------
static double ConvertToUnitDistance(const double amount, const double amountBasePerInch)
{
	const double kUnitDistanceBasePerInch = 72.0;
	return ((amount / amountBasePerInch) * kUnitDistanceBasePerInch);
}

//-------------------------------------------------------------------------------
//
//	MakeNewDocument
//
//	Make an actual new document from our parameters.
//
//-------------------------------------------------------------------------------
static SPErr MakeNewDocument (void)
{
	SPErr error = kSPNoError;
	
	PIActionDescriptor descriptor = NULL;
	error = sPSActionDescriptor->Make (&descriptor);
	
	if (error == kSPNoError)
	{
		error = sPSActionDescriptor->PutUnitFloat
			(
			descriptor,
			keyWidth,
			unitDistance,
			ConvertToUnitDistance(gWidth, gResolution)
			);
	}
	
	if (error == kSPNoError)
	{		
		error = sPSActionDescriptor->PutUnitFloat
			(
			descriptor,
			keyHeight,
			unitDistance,
			ConvertToUnitDistance(gHeight, gResolution)
			);
	}
				
	if (error == kSPNoError)
	{		
		error = sPSActionDescriptor->PutUnitFloat
			(
			descriptor,
			keyResolution,
			unitDistance,
			gResolution
			);
	}
			
	if (error == kSPNoError)
	{		
		error = sPSActionDescriptor->PutEnumerated
			(
			descriptor,
			keyFill,
			typeFill,
			gFill
			);
	}

	if (error == kSPNoError)
	{		
		error = sPSActionDescriptor->PutClass
			(
			descriptor,
			keyMode,
			gMode
			);
	}
				
	if (error == kSPNoError)
	{
		PIActionDescriptor playDescriptor = NULL;
		sPSActionDescriptor->Make(&playDescriptor);
		
		sPSActionDescriptor->PutObject
			(
			playDescriptor,
			keyNew,
			classDocument,
			descriptor
			);
		
		// I'm creating a new document, the event I
		// want should be called eventNewDocument or
		// something, right?  Nope.  Use eventMake.

		PIActionDescriptor resultDescriptor = NULL;		

		error = sPSActionControl->Play
			(
			&resultDescriptor,
			eventMake,
			playDescriptor,
			plugInDialogDontDisplay
			);
			
		// Check for error here.  If an error occurred, there
		// will be a string in the descriptor, keyMessage, with
		// the error that occurred.
		
		if (resultDescriptor != NULL)
			sPSActionDescriptor->Free(resultDescriptor);
			
		if (playDescriptor != NULL)
			sPSActionDescriptor->Free(playDescriptor);
			
		if (descriptor != NULL)
			sPSActionDescriptor->Free(descriptor);	

	} // error

	return error;
	
} // end MakeNewDocument
//-------------------------------------------------------------------------------
// end MakeNew.cpp
