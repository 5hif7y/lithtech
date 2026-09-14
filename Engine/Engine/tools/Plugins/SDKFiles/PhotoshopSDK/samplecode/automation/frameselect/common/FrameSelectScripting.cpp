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
//		FrameSelectScripting.cpp
//
//	Description:
//		Inset or outset current selection.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------


#include "PhotoshopSDK.h"
#include "FrameSelect.h"

//-------------------------------------------------------------------------------
//	Prototypes.
//-------------------------------------------------------------------------------
static bool UseAmountOnly (void);

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
			DescriptorTypeID type = typeNull;
			PIActionDescriptor rectangle = NULL;
			Boolean hasKey = false;
			
			error = sPSActionDescriptor->HasKey
				(
				descriptor,
				keyMyGutter,
				&hasKey
				);
				
			if (hasKey)
				{
				double value = 0.0;
				DescriptorUnitID unit = unitNone;

				error = sPSActionDescriptor->GetUnitFloat
					(
					descriptor,
					keyMyGutter,
					&unit,
					&value
					);
					
				gAmount = value;
				SetRectFromGutter(gAmount, &gRect);
				}
			else
				{ // Look for actual rectangle:
				error = sPSActionDescriptor->GetObject
					(
					descriptor,
					keyOffset,
					&type,
					&rectangle
					);
					
				if (error == kSPNoError && rectangle != NULL)
					{
					double value = 0.0;
					DescriptorUnitID unit = unitNone;
					
					error = sPSActionDescriptor->GetUnitFloat
						(
						rectangle,
						keyTop,
						&unit,
						&value
						);
						
					if (error == kSPNoError)
						{
						gRect.top = value;
						gAmount = value;
						
						error = sPSActionDescriptor->GetUnitFloat
							(
							rectangle,
							keyLeft,
							&unit,
							&value
							);
						}
					
					if (error == kSPNoError)
						{
						gRect.left = value;
						
						error = sPSActionDescriptor->GetUnitFloat
							(
							rectangle,
							keyBottom,
							&unit,
							&value
							);
						}
						
					if (error == kSPNoError)
						{
						gRect.bottom = value;
						
						error = sPSActionDescriptor->GetUnitFloat
							(
							rectangle,
							keyRight,
							&unit,
							&value
							);
						}
						
					if (error == kSPNoError)
						{
						gRect.right = value;
						}
						
					} // rectangle was NULL or we had an error.
					
					// Just in case, we'll still Free it outside
					// our check:
					if (rectangle != NULL)
						{
						error = sPSActionDescriptor->Free(rectangle);
						rectangle = NULL;
						}
					
					// Remember, we're required to free anything we get,
					// including objects.
					
				} // Done looking for rectangle.
				
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

SPErr WriteScriptParams (PIActionParameters* actionParams)
{
	SPErr				error = kSPNoError; // we'll return any error with this

	if(sPSActionDescriptor != NULL)
		{ // Make sure we have a valid suite before doing this.
		
		PIActionDescriptor rectangle = NULL; // token to write our rectangle object to
			
		if (!UseAmountOnly())
			{
			
			error = sPSActionDescriptor->Make(&rectangle);
			
			if (error == kSPNoError && rectangle != NULL)
				{
				double value = gRect.top;
				
				error = sPSActionDescriptor->PutUnitFloat
					(
					rectangle,
					keyTop,
					unitDistance,
					value
					);
					
				value = gRect.left;
				
				error = sPSActionDescriptor->PutUnitFloat
					(
					rectangle,
					keyLeft,
					unitDistance,
					value
					);

				value = gRect.bottom;
				
				error = sPSActionDescriptor->PutUnitFloat
					(
					rectangle,
					keyBottom,
					unitDistance,
					value
					);

				value = gRect.right;
				
				error = sPSActionDescriptor->PutUnitFloat
					(
					rectangle,
					keyRight,
					unitDistance,
					value
					);
			
				} // error or rectangle was null
			
			} // UseAmountOnly

		PIActionDescriptor	descriptor = NULL;	// token to write our parameters to
		
		error = sPSActionDescriptor->Make(&descriptor);
		
		if (error == kSPNoError && descriptor != NULL)
			{
			
			if (!UseAmountOnly())
				{
				error = sPSActionDescriptor->PutObject
					(
					descriptor,
					keyOffset,
					classRectangle,
					rectangle
					);
				}
			else
				{
				error = sPSActionDescriptor->PutUnitFloat
					(
					descriptor,
					keyMyGutter,
					unitDistance,
					gAmount
					);
				}
				
			// Now stuff our return descriptor for return to the host:
			if (error == kSPNoError && actionParams != NULL)
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
		
		// Either way, free the rectangle we made:			
		if (rectangle != NULL)
			{
			error = sPSActionDescriptor->Free(rectangle);
			rectangle = NULL;
			}			
			
		}
	
	return error;
		
} // end WriteScriptParams
	
//-------------------------------------------------------------------------------
//
//	UseAmountOnly
//
//	Determines whether to use the rect or the amount values.
//
//-------------------------------------------------------------------------------

static bool UseAmountOnly (void)
	{
	return
		(
		gRect.top == gAmount &&
		gRect.left == gAmount &&
		gRect.bottom == -gAmount &&
		gRect.right == -gAmount
		);
	}	
	
//-------------------------------------------------------------------------------

// end FrameSelectScripting.cpp
