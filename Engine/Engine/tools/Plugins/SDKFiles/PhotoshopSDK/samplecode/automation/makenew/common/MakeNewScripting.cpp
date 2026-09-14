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
//		MakeNewScripting.cpp
//
//	Description:
//		Create a new document.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "MakeNew.h"

//-------------------------------------------------------------------------------
//	Prototypes.
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//
//	ReadScriptParams
//
//	Checks the parameters against scripting-returned parameters, if any, and
//	updates the globals to match ones provided by the scripting system.
//
//-------------------------------------------------------------------------------
SPErr ReadScriptParams(PIActionParameters* actionParams)
{
	SPErr error = kSPNoError;

	if (sPSActionDescriptor != NULL )
		{ // Make sure we have a valid suite before trying this.
		PIActionDescriptor descriptor = actionParams->descriptor;
		
		// If we got a valid descriptor, grab our key out of it:
		if (descriptor != NULL)
			{
			error = sPSActionDescriptor->GetInteger
				(
				descriptor,
				keyMyWidth,
				&gWidth
				);
				
			error = sPSActionDescriptor->GetInteger
				(
				descriptor,
				keyMyHeight,
				&gHeight
				);
				
			error = sPSActionDescriptor->GetInteger
				(
				descriptor,
				keyMyResolution,
				&gResolution
				);
			
			// We don't care about type, so declare a variable
			// that we'll ignore:
			DescriptorEnumTypeID enumType = enumNull;
			
			error = sPSActionDescriptor->GetEnumerated
				(
				descriptor,
				keyMyFill,
				&enumType,
				&gFill
				);
			
			error = sPSActionDescriptor->GetClass
				(
				descriptor,
				keyMyMode,
				&gMode
				);

			// Since we were handed this descriptor, it's our job to
			// free it, but we'll free it on SetReturnInfo, so we're okay.
			
			} // descriptor == NULL
		
		}

	return error;
	
} // end ReadScriptParams
		
//-------------------------------------------------------------------------------
//
//	WriteScriptParams
//
//	Takes our parameters from our global variables and writes them out to the
//	scripting system, which hands it all back to the host.
//
//-------------------------------------------------------------------------------

SPErr WriteScriptParams ( PIActionParameters* actionParams )
{
	SPErr				error = kSPNoError; // we'll return any error with this

	if (sPSActionDescriptor != NULL )//end if
		{ // Make sure we have a valid suite before doing this.
		PIActionDescriptor	descriptor = NULL;	// token to write our parameters to
		
		error = sPSActionDescriptor->Make(&descriptor);
		
		error = sPSActionDescriptor->PutInteger
			(
			descriptor,
			keyMyWidth,
			gWidth
			);
			
		error = sPSActionDescriptor->PutInteger
			(
			descriptor,
			keyMyHeight,
			gHeight
			);
			
		error = sPSActionDescriptor->PutInteger
			(
			descriptor,
			keyMyResolution,
			gResolution
			);

		error = sPSActionDescriptor->PutEnumerated
			(
			descriptor,
			keyMyFill,
			typeMyFill,
			gFill
			);
			
		error = sPSActionDescriptor->PutClass
			(
			descriptor,
			keyMyMode,
			gMode
			);
			
		// Now stuff our return descriptor for return to the host:
		if (error == 0 && actionParams != NULL)
			{
			if (actionParams->descriptor)
				sPSActionDescriptor->Free( actionParams->descriptor );

			actionParams->descriptor = descriptor;
			actionParams->recordInfo = plugInDialogOptional;
			}
		}
	
	return error;
		
} // end WriteScriptParams
// end MakeNewScripting.cpp
