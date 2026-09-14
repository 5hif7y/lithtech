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
//
//	File:
//		PolygonSelectScripting.cpp
//
//	Copyright 1999-1999, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Description:
//		Defines radius, number of sides, and angle of rotation.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "PolygonSelect.h"

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

SPErr ReadScriptParams (PIActionParameters* actionParams)
{
	SPErr error = kSPNoError;

	if (sPSActionDescriptor != NULL)
		{ // Make sure we have a valid suite before trying this.
			PIActionDescriptor descriptor = actionParams->descriptor;	
			
	// If we got a valid descriptor, grab our key out of it:
		if (descriptor != NULL)
			{
			DescriptorUnitID unit = unitPixels;
			error = sPSActionDescriptor->GetInteger
				(
				descriptor,
				keyRadius,
				&gRadius
				);
				
			error = sPSActionDescriptor->GetInteger
				(
				descriptor,
				keyNumberSides,
				&gNumberSides
				);

			unit = unitAngle;
			error = sPSActionDescriptor->GetUnitFloat
				(
				descriptor,
				keyAngle,
				&unit,
				&gRotationAngle
				);
		
			} // Making descriptor okay.

		else
		{
		gRadius = 100;
		gNumberSides = 4;
		gRotationAngle = 0.0;
		}
		
	}

	return error;
}
	
 // end ReadScriptParams
		
//-------------------------------------------------------------------------------
//
//	WriteScriptParams
//
//	Takes our parameters from our global variables and writes them out to the
//	scripting system, which hands it all back to the host.
//
//-------------------------------------------------------------------------------

SPErr WriteScriptParams (PIActionParameters* actionParams)
{
	SPErr				error = kSPNoError; // we'll return any error with this

	if (sPSActionDescriptor != NULL)
		{ // Make sure we have a valid suite before doing this.
		
		PIActionDescriptor	descriptor = NULL;	// token to write our parameters to
		
		error = sPSActionDescriptor->Make(&descriptor);
		
		if (error == kSPNoError && descriptor != NULL)
			{
			error = sPSActionDescriptor->PutInteger
					(
					descriptor,
					keyRadius,
					gRadius
					);
				
			error = sPSActionDescriptor->PutInteger
					(
					descriptor,
					keyNumberSides,
					gNumberSides
					);

			error = sPSActionDescriptor->PutUnitFloat
				(
				descriptor,
				keyAngle,
				unitAngle,
				gRotationAngle
				);

			// Now stuff our return descriptor for return to the host:
			if (error == 0 && actionParams != NULL)
				{
				if (actionParams->descriptor)
					sPSActionDescriptor->Free( actionParams->descriptor );

				actionParams->descriptor = descriptor;
				actionParams->recordInfo = plugInDialogOptional;
				}
		
			} // Making descriptor okay.
		else
			{
			// Oops.  Didn't work, so we didn't get a chance to stuff our
			// descriptor in the return descriptor, so go ahead and try
			// to free it (otherwise it becomes the responsibility of the
			// host):
			if (descriptor != NULL)
				{
				error = sPSActionDescriptor->Free(descriptor);
				descriptor = NULL;
				}
			}
		
		}
	
	return error;
		
} // end WriteScriptParams
	

// end PolygonSelectScripting.cpp
