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
//		MakeSelectFill.cpp
//
//	Description:
//
//	MakeSelectFill is an automation plug-in that creates a new document, 
//	creates a rectangular selection on the document, and fills the document 
//	with the current foreground color.  MakeSelectFill illustrates how to 
//	incorporate Listener output into the Template plug-in code by adding 
//	function definitions and calling the functions from AutoPluginMain.  
//	Using the Listener plug-in alone doesn't provide completely accurate 
//	code for an automation plug-in.  You must rearrange some of the lines of 
//	code.  It may be useful for you to change the function names, descriptor 
//	variable names to something more meaningful.  Slightly different versions
//	of the MacDoc, RectangleSelection, and ColorFill functions were created
//	by the Listener	plug-in and modified for complete accuracy.
//
//	
//	MakeSelectFill has the following functionality:
//
//		* creates a new document
//		* creates rectangular-shaped selection
//		* fills the document with the current foreground color
//		* Pops About menu item, identifying the MakeSelectFill plug-in.
//		* Pops dialog at start and end of plug-in
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes -- Use precompiled headers if compiling with CodeWarrior.
//-------------------------------------------------------------------------------
#include "PhotoshopSDK.h"
#include "admBasic.h"


//-------------------------------------------------------------------------------
//	Function Prototypes.
//-------------------------------------------------------------------------------

SPErr MakeDoc(DescriptorClassID mode, 
			  double width, 
			  double height, 
			  double resolution, 
			  DescriptorEnumID fill);

SPErr RectangleSelection(double top, 
						 double left, 
						 double bottom, 
						 double right);

SPErr ColorFill(double horizontal, 
				double vertical, 
				int32 tolerance, 
				bool alias);

//-------------------------------------------------------------------------------
//	Globals -- Define global variables for plug-in scope.
//-------------------------------------------------------------------------------

SPBasicSuite * sSPBasic = NULL;// the Basic Suite
SPPluginRef gPlugInRef = NULL;

//-------------------------------------------------------------------------------
//
//	AutoPluginMain
//
//	All calls to the plug-in module come through this routine.
//	It must be placed first in the resource. To achieve this,
//	most development systems require this be the first routine
//	in the source.
//
//	The entrypoint will be "pascal void" for Macintosh,
//	"void" for Windows.
//
//-------------------------------------------------------------------------------

DLLExport SPAPI SPErr AutoPluginMain(const char * caller, // who is calling 
								     const char * selector, // what they want
								     void * data )// the message
{
	SPErr error = kSPNoError;
	
	// all messages contain a SPMessageData*
	SPMessageData * basicMessage = (SPMessageData *) data;

	sSPBasic = basicMessage->basic;
	gPlugInRef = basicMessage->self;

	// check for SP interface callers
	if (sSPBasic->IsEqual(caller, kSPInterfaceCaller))
	{
		if (sSPBasic->IsEqual(selector, kSPInterfaceAboutSelector))
		{
			DoAbout(basicMessage->self, AboutID);
		}
	}

	// Photoshop is calling
	if (sSPBasic->IsEqual(caller, kPSPhotoshopCaller))
	{
		// the one and only message 
		if (sSPBasic->IsEqual(selector, kPSDoIt))
		{
			//call three functions that were created by Listener
			error = MakeDoc(classRGBColorMode, 300, 400, 72, enumWhite);
			
			if (!error)
				error = RectangleSelection(60, 30.7, 200, 240.8);

			if (!error)
				error = ColorFill(180, 175, 255, true);
		}
	}

	// clean up, free memory
	PIUSuitesRelease();

	return error;

}/*end AutoPluginMain*/

//-------------------------------------------------------------------------------
//
//	MakeDoc
//
//	The MakeDoc function creates a new document of the given "width", "height",
//	"resolution", "fill", and "mode". 
//	"resolution" accepts values between 1.0 and 4285.714
//	"fill" accepts enumWhite, enumBackgroundColor, or enumTransparent
//	"mode" accepts classCMYKColorMode, classRGBColorMode, classLabColorMode, or 
//	classBitmapMode
//	Acceptable arguments for all key values can be found in PITerminology.h
//	Notice that at each function call, the code checks for error. If an 
//	error occurs, the descriptors and references	are freed and the function 
//	is terminated.
//
//-------------------------------------------------------------------------------
SPErr MakeDoc(DescriptorClassID mode, 
				double width, 
				double height, 
				double resolution, 
				DescriptorEnumID fill)
{

	SPErr error = kSPNoError;
	PIActionDescriptor makeResult = NULL;
	PIActionDescriptor makeDescriptor = NULL;
	PIActionDescriptor docDescriptor = NULL;

	error = sPSActionDescriptor->Make(&makeDescriptor);
	if (error) goto returnError;
	
	error = sPSActionDescriptor->Make(&docDescriptor);
	if (error) goto returnError;
	
	error = sPSActionDescriptor->PutClass(docDescriptor, 
											keyMode, 
											mode);
	if (error) goto returnError;
	
	error = sPSActionDescriptor->PutUnitFloat(docDescriptor, 
												keyWidth, 
												unitDistance, 
												width);
	if (error) goto returnError;
	
	error = sPSActionDescriptor->PutUnitFloat(docDescriptor, 
												keyHeight, 
												unitDistance, 
												height);
	if (error) goto returnError;

	error = sPSActionDescriptor->PutUnitFloat(docDescriptor, 
												keyResolution, 
												unitDensity, 
												resolution);
	if (error) goto returnError;

	error = sPSActionDescriptor->PutEnumerated(docDescriptor, 
												keyFill, 
												typeFill, 
												fill);
	if (error) goto returnError;

	error = sPSActionDescriptor->PutObject(makeDescriptor, 
											keyNew, 
											classDocument, 
											docDescriptor);
	if (error) goto returnError;

	error = sPSActionControl->Play(&makeResult, 
									eventMake, 
									makeDescriptor, 
									plugInDialogSilent);
	if (error) goto returnError;

	returnError:
	if (makeResult != NULL) sPSActionDescriptor->Free(makeResult);
	if (makeDescriptor != NULL)	sPSActionDescriptor->Free(makeDescriptor);
	if (docDescriptor != NULL) sPSActionDescriptor->Free(docDescriptor);

	return error;
}/*end MakeDoc*/

//-------------------------------------------------------------------------------
//
//	RectangleSelection
//
//	The RectangleSelection creates a rectangular-shaped selection frame on the 
//	current document. 
//	"top", "left", "bottom", and "right" designate the corners of the rectangle
//	Returns SPErr.
//	Notice that at each function call, the code checks for error. If an error 
//  occurs, the descriptors and references	are freed and the function is 
//	terminated.
//-------------------------------------------------------------------------------

SPErr RectangleSelection(double top, double left, double bottom, double right)
{
      SPErr error = kSPNoError;
     
      PIActionDescriptor result;
      PIActionDescriptor descriptor;
      PIActionReference reference;
      PIActionDescriptor rectDescriptor;
     
      error = sPSActionDescriptor->Make(&descriptor);
      if (error) goto returnError;
      error = sPSActionReference->Make(&reference);
      if (error) goto returnError;
      error = sPSActionDescriptor->Make(&rectDescriptor);
      if (error) goto returnError;
          
      error = sPSActionReference->PutProperty(reference, 
                                              classChannel, 
                                              keySelection);
      if (error) goto returnError;
      error = sPSActionDescriptor->PutReference(descriptor, 
                                                keyNull, 
                                                reference);
      if (error) goto returnError;
          
      error = sPSActionDescriptor->PutUnitFloat(rectDescriptor, 
                                                keyTop, 
                                                unitDistance, 
                                                top);
      if (error) goto returnError;
      error = sPSActionDescriptor->PutUnitFloat(rectDescriptor, 
                                                keyLeft, 
                                                unitDistance, 
                                                left);
      if (error) goto returnError;
      error = sPSActionDescriptor->PutUnitFloat(rectDescriptor, 
                                                keyBottom, 
                                                unitDistance, 
                                                bottom);
      if (error) goto returnError;
      error = sPSActionDescriptor->PutUnitFloat(rectDescriptor, 
                                                keyRight, 
                                                unitDistance, 
                                                right);
      if (error) goto returnError;
      error = sPSActionDescriptor->PutObject(descriptor, 
                                             keyTo, 
                                             classRectangle, 
                                             rectDescriptor);
      if (error) goto returnError;
	    error = sPSActionControl->Play(&result, 
                                     eventSet, 
                                     descriptor, 
                                     plugInDialogSilent);
      if (error) goto returnError;
     
      returnError:
      /*free descriptors and references*/
      if (result != NULL) sPSActionDescriptor->Free(result);
			if (descriptor != NULL)	sPSActionDescriptor->Free(descriptor);
			if (reference != NULL)	sPSActionReference->Free(reference);
			if (rectDescriptor != NULL) sPSActionDescriptor->Free(rectDescriptor);
      return error;

}/*end RectangleSelection*/

//-------------------------------------------------------------------------------
//
//	ColorFill
//
//	The ColorFill function fills the document with the current foreground color, 
//	at a given point, with the given alias setting.  
//	"tolerance" accepts an integer between 0 and 255
//	"horizontal" and "vertical" accepts a pixel measurement within document 
//	height and width.
//	Notice that at each function call, the code checks for error. If an error 
//  occurs, the descriptors and references	are freed and the function is 
//	terminated.
//-------------------------------------------------------------------------------
SPErr ColorFill(double horizontal, double vertical, int32 tolerance, bool alias)
{

	SPErr error = kSPNoError;
	//fill the document with the foreground color
	PIActionDescriptor fillResult = NULL;
	PIActionDescriptor fillDescriptor = NULL;
	PIActionDescriptor pointDescriptor = NULL;
	
	error = sPSActionDescriptor->Make(&fillDescriptor);
	if (error) goto returnError;

	error = sPSActionDescriptor->Make(&pointDescriptor);
	if (error) goto returnError;
	
	error = sPSActionDescriptor->PutUnitFloat(pointDescriptor, 
												keyHorizontal, 
												unitPixels, 
												horizontal);
	if (error) goto returnError;
	
	error = sPSActionDescriptor->PutUnitFloat(pointDescriptor, 
												keyVertical, 
												unitPixels, 
												vertical);
	if (error) goto returnError;
	
	error = sPSActionDescriptor->PutObject(fillDescriptor, 
											keyFrom, 
											classPoint, 
											pointDescriptor);
	if (error) goto returnError;
	
	error = sPSActionDescriptor->PutInteger(fillDescriptor, 
											keyTolerance, 
											tolerance);
	if (error) goto returnError;
	
	error = sPSActionDescriptor->PutBoolean(fillDescriptor, 
											keyAntiAlias, 
											alias);
	if (error) goto returnError;
	
	error = sPSActionDescriptor->PutEnumerated(fillDescriptor, 
												keyUsing, 
												typeFillContents, 
												enumForegroundColor);
	if (error) goto returnError;
	
	error = sPSActionControl->Play(&fillResult, 
									eventFill, 
									fillDescriptor, 
									plugInDialogSilent);
	if (error) goto returnError;

	returnError:

	//free all descriptors and references, if they exist
	if (fillResult != NULL)	sPSActionDescriptor->Free(fillResult);
	if (fillDescriptor != NULL)	sPSActionDescriptor->Free(fillDescriptor);
	if (pointDescriptor != NULL) sPSActionDescriptor->Free(pointDescriptor);

	return error;
}/*end ColorFill*/
// end MakeSelectFill.cpp