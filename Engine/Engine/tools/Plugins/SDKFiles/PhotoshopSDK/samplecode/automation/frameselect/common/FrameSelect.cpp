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
//		FrameSelect.cpp
//
//	Description:
//		This insets, outsets, or creates a selection based on
//		top, left, bottom, and right parameters.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "FrameSelect.h"

SPBasicSuite* sSPBasic = NULL;
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

PIUDRect_t gRect;	// double top, left, bottom, right.
double gAmount;		// amount
RulerUnits gRulerUnits;	// pixels, picas, etc.

RulerUnits 		gDocumentRulerUnits;
PIUDPoint_t 	gDocumentBounds;
double			gDocumentMinBound;
PIUDPoint_t		gDocumentOrigin;
double			gDocumentResolution;

//-------------------------------------------------------------------------------
//	Prototypes.
//-------------------------------------------------------------------------------

// Main routine:
static SPErr Execute (PIActionParameters*);

// Initialize our parameters:
static void Initialize (void);

// Convert from any base to or from unitDistance:
static double ConvertToUnitDistance
	(
	const double amount, 
	const double amountBasePerInch
	);
	
static double ConvertFromUnitDistance
	(
	const double amount, 
	const double amountBasePerInch
	);
	
static double ConvertUnits
	(
	/* IN */	const double value,				// Value to convert.
	/* IN */	const double resolutionPerInch,	// Resolution of document in pixels per inch.
	/* IN */	const double documentBound,		// For percent, a bound of the document in unitDistance.
	/* IN */	const RulerUnits inUnits,		// Incoming units.
	/* IN */	const RulerUnits outUnits		// Desired outgoing units.
	);

// Get the document bounds:
static PIUDPoint_t GetDocumentBounds (void);

// Get the origin point of the document:
static PIUDPoint_t GetDocumentOrigin (void);
	
// Get the resolution fo the document:
static double GetDocumentResolution (void);

// Set the selection:
static SPErr SetSelection
	(
	/* IN */	const VRect inRect
	);

// Return the document's ruler units setting:
static RulerUnits GetDocumentRulerUnits (void);

// Make the new selection:
static SPErr MakeNewSelection (void);

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

	PIUSuitesRelease ();

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
static SPErr Execute(PIActionParameters* actionParams)
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
			error = MakeNewSelection();
	if (error == kSPNoError)
			WriteScriptParams(actionParams);
			
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
	
	gRect.top = 0.0;
	gRect.left = 0.0;
	gRect.bottom = 0.0;
	gRect.right = 0.0;

	gAmount = 0.0;
	
	gDocumentRulerUnits = GetDocumentRulerUnits();
	gRulerUnits = gDocumentRulerUnits;
	
	gDocumentOrigin = GetDocumentOrigin();
	
	gDocumentResolution = GetDocumentResolution();

	gDocumentBounds = GetDocumentBounds();
	
	gDocumentMinBound = gDocumentBounds.v;
	if (gDocumentBounds.h < gDocumentMinBound)
		gDocumentMinBound = gDocumentBounds.h;

	}

//-------------------------------------------------------------------------------
//
//	GetDocumentRulerUnits
//
//	Return the units of the current document.
//
//-------------------------------------------------------------------------------
static RulerUnits GetDocumentRulerUnits (void)
	{
	SPErr error = kSPNoError;
	
	RulerUnits returnRulerUnits = kRulerPixels;
	
	PIActionReference reference = NULL;
	
	error = sPSActionReference->Make(&reference);
	
	if (error == kSPNoError && reference != NULL)
		{
		error = sPSActionReference->PutEnumerated
			(
			reference,
			classApplication,
			typeOrdinal,
			enumTarget
			);
		}
		
	PIActionDescriptor result = NULL;
			
	error = sPSActionControl->Get
		(
		&result,
		reference
		);
		
	if (error == kSPNoError && result != NULL)
		{
		// Got all the document properties.  Grab the ones
		// I'm interested in.
		DescriptorEnumID value = enumRulerPixels;
		DescriptorTypeID type = typeRulerUnits;
		
		error = sPSActionDescriptor->GetEnumerated
			(
			result,
			keyRulerUnits,
			&type,
			&value
			);
			
		if (error == kSPNoError && type == typeRulerUnits)
			{
			switch (value)
				{
				case enumRulerPixels:
					returnRulerUnits = kRulerPixels;
					break;
				case enumRulerInches:
					returnRulerUnits = kRulerInches;
					break;
				case enumRulerCm:
					returnRulerUnits = kRulerCm;
					break;
				case enumRulerPicas:
					returnRulerUnits = kRulerPicas;
					break;
				case enumRulerPoints:
					returnRulerUnits = kRulerPoints;
					break;
				case enumRulerPercent:
					returnRulerUnits = kRulerPercent;
					break;
				default:
					// Error, don't know this enum.
					break;
				}
			}
		}
		
	if (result != NULL)
		{
		error = sPSActionDescriptor->Free(result);
		result = NULL;
		}
		
	if (reference != NULL)
		{
		error = sPSActionReference->Free(reference);
		reference = NULL;
		}
		
	return returnRulerUnits;
	
	} // end GetDocumentUnits
	
//-------------------------------------------------------------------------------
//
//	GetDocumentBounds
//
//	Return the bounds of the current document.
//
//-------------------------------------------------------------------------------
static PIUDPoint_t GetDocumentBounds (void)
	{
	PIUDPoint_t outPoint = { 0.0, 0.0 };
	
	PIActionReference reference = NULL;
	
	SPErr error = sPSActionReference->Make(&reference);
	
	if (error == kSPNoError && reference != NULL)
		{
		error = sPSActionReference->PutEnumerated
			(
			reference,
			classDocument,
			typeOrdinal,
			enumTarget
			);
		}
		
	PIActionDescriptor result = NULL;
			
	error = sPSActionControl->Get
		(
		&result,
		reference
		);
		
	if (error == kSPNoError && result != NULL)
		{
		// Got all the document properties.  Grab the ones
		// I'm interested in.
		double value = 0.0;
		DescriptorUnitID unit = unitNone;
		
		error = sPSActionDescriptor->GetUnitFloat
			(
			result,
			keyWidth,
			&unit,
			&value
			);
			
		if (error == kSPNoError)
			{
			outPoint.h = value;
			
			error = sPSActionDescriptor->GetUnitFloat
				(
				result,
				keyHeight,
				&unit,
				&value
				);
			}
			
		if (error == kSPNoError)
			{
			outPoint.v = value;
			}
		}
		
	if (result != NULL)
		{
		error = sPSActionDescriptor->Free(result);
		result = NULL;
		}
		
	if (reference != NULL)
		{
		error = sPSActionReference->Free(reference);
		reference = NULL;
		}
		
	return outPoint;
	
	} // end GetDocumentBounds

//-------------------------------------------------------------------------------
//
//	GetDocumentOrigin
//
//	Return the origin of the current document.
//
//-------------------------------------------------------------------------------
static PIUDPoint_t GetDocumentOrigin (void)
	{
	PIUDPoint_t outPoint = { 0.0, 0.0 };
	
	PIActionReference reference = NULL;
	
	SPErr error = sPSActionReference->Make(&reference);
	
	if (error == kSPNoError && reference != NULL)
		{
		error = sPSActionReference->PutEnumerated
			(
			reference,
			classDocument,
			typeOrdinal,
			enumTarget
			);
		}
		
	PIActionDescriptor result = NULL;
			
	error = sPSActionControl->Get
		(
		&result,
		reference
		);
		
	if (error == kSPNoError && result != NULL)
		{
		// Got all the document properties.  Grab the ones
		// I'm interested in.
		int32 value = 0;
		DescriptorUnitID unit = unitNone;
		
		error = sPSActionDescriptor->GetInteger
			(
			result,
			keyRulerOriginH,
			&value
			);
			
		if (error == kSPNoError)
			{
			// These are 16.16 (specified in PIProperties.h), so
			// we must convert them:
			outPoint.h = 	(double)(((double)value) / (double)65536.0);

			error = sPSActionDescriptor->GetInteger
				(
				result,
				keyRulerOriginV,
				&value
				);
			}
			
		if (error == kSPNoError)
			{
			// These are 16.16 (specified in PIProperties.h), so
			// we must convert them:
			outPoint.v = 	(double)(((double)value) / (double)65536.0);
			}
		}
		
	if (result != NULL)
		{
		error = sPSActionDescriptor->Free(result);
		result = NULL;
		}
		
	if (reference != NULL)
		{
		error = sPSActionReference->Free(reference);
		reference = NULL;
		}
		
	return outPoint;
	
	} // end GetDocumentBounds

//-------------------------------------------------------------------------------
//
//	GetDocumentResolution
//
//	Return the resolution of the current document in pixels per inch.
//
//-------------------------------------------------------------------------------
static double GetDocumentResolution (void)
	{
	double resolution = kUnitDistanceBasePerInch;
	
	SPErr error = kSPNoError;
	
	PIActionReference reference = NULL;
	
	error = sPSActionReference->Make(&reference);
	
	if (error == kSPNoError && reference != NULL)
		{
		error = sPSActionReference->PutEnumerated
			(
			reference,
			classDocument,
			typeOrdinal,
			enumTarget
			);
		}
		
	PIActionDescriptor result = NULL;
			
	error = sPSActionControl->Get
		(
		&result,
		reference
		);
		
	if (error == kSPNoError && result != NULL)
		{
		// Got all the document properties.  Grab the ones
		// I'm interested in.
		double value = 0.0;
		DescriptorUnitID unit = unitNone;
		
		error = sPSActionDescriptor->GetUnitFloat
			(
			result,
			keyResolution,
			&unit,
			&value
			);
			
		if (error == kSPNoError)
			{
			resolution = value;		// Will always be pixels.
			}
		}
		
	if (result != NULL)
		{
		error = sPSActionDescriptor->Free(result);
		result = NULL;
		}
		
	if (reference != NULL)
		{
		error = sPSActionReference->Free(reference);
		reference = NULL;
		}
		
	return resolution;

	}

//-------------------------------------------------------------------------------
//
//	SetSelection
//
//	Change the current selection.
//
//-------------------------------------------------------------------------------
static SPErr SetSelection
	(
	/* IN */	const VRect inRect
	)
	{
	SPErr error = kSPNoError;
	
	PIActionReference reference = NULL;
	
	error = sPSActionReference->Make(&reference);
	
	if (error == kSPNoError && reference != NULL)
		{
		error = sPSActionReference->PutProperty
			(
			reference,
			// Normally this would be classProperty, but a selection is
			// actually an alpha channel:
			classChannel,
			keySelection
			);
		}
		
	// We have to construct a descriptor of classRectangle because
	// that is what the selection key is:
	PIActionDescriptor rectangle = NULL;
	
	error = sPSActionDescriptor->Make(&rectangle);
	
		if (error == kSPNoError && rectangle != NULL)
			{
			double value = (double)inRect.top;
			
			error = sPSActionDescriptor->PutUnitFloat
				(
				rectangle,
				keyTop,
				unitDistance,
				value
				);
				
			value = (double)inRect.left;
			
			error = sPSActionDescriptor->PutUnitFloat
				(
				rectangle,
				keyLeft,
				unitDistance,
				value
				);

			value = (double)inRect.bottom;
			
			error = sPSActionDescriptor->PutUnitFloat
				(
				rectangle,
				keyBottom,
				unitDistance,
				value
				);

			value = (double)inRect.right;
			
			error = sPSActionDescriptor->PutUnitFloat
				(
				rectangle,
				keyRight,
				unitDistance,
				value
				);
		
			} // error or rectangle was null

	// Now lets stuff all this into a descriptor:
	
	PIActionDescriptor descriptor = NULL;
	
	error = sPSActionDescriptor->Make(&descriptor);
	
	if (error == kSPNoError && descriptor != NULL)
		{
		
		// "keyTarget" should always be the first key in, if there is one, which
		// there is in this case because we are doing the command:
		// "set selection to <rect>".
		error = sPSActionDescriptor->PutReference
			(
			descriptor,
			keyTarget,
			reference
			);
			
		// Now we put the new, changed information, which is an object
		// of classRectangle, using the key "keyTo":
		sPSActionDescriptor->PutObject
			(
			descriptor,
			keyTo,
			classRectangle,
			rectangle
			);
		}
		
	// Now lets set this information:
	
	PIActionDescriptor result = NULL;
	const PIDialogPlayOptions options = plugInDialogDontDisplay;
		
	error = sPSActionControl->Play
		(
		&result,
		eventSet,
		descriptor,
		options
		);
	
	if (error != kSPNoError)
		{
		// Something went wrong with the execution.  Look inside
		// descriptor for keyMessage, which will be text of error.
		}
		
	// Now clean up all the descriptors/etc. we may have created:
	
	if (reference != NULL)
		{
		error = sPSActionReference->Free(reference);
		reference = NULL;
		}
		
	if (rectangle != NULL)
		{
		error = sPSActionDescriptor->Free(rectangle);
		rectangle = NULL;
		}
	
	if (descriptor != NULL)
		{
		error = sPSActionDescriptor->Free(descriptor);
		descriptor = NULL;
		}
		
	if (result != NULL)
		{
		error = sPSActionDescriptor->Free(result);
		result = NULL;
		}
		
	return error;
	}


//-------------------------------------------------------------------------------
//
//	MakeNewSelection
//
//	Do any required logic then change the current selection.
//
//-------------------------------------------------------------------------------
static SPErr MakeNewSelection (void)
	{
	SPErr error = kSPNoError;

	// First set up initial blank rect:
	
	VRect newRect = { 0, 0, 0, 0 };
	
	// Now assign it to the base document bounds:	
	
	newRect.bottom = (int32)gDocumentBounds.v;
	newRect.right = (int32)gDocumentBounds.h;

	// Then offset it for the origin being somewhere else:
	
	newRect.top -= (int32)gDocumentOrigin.v;
	newRect.left -= (int32)gDocumentOrigin.h;
	newRect.bottom -= (int32)gDocumentOrigin.v;
	newRect.right -= (int32)gDocumentOrigin.h;
	
	// Now convert our rect to its pixel counterpart:
	
 	PIUDRect_t convertRect = ConvertRectToUnitDistance
		(
		gRect,					// Incoming rect.
		gDocumentResolution,	// Resolution.
		gDocumentBounds,		// Bounds for percentage.
		gRulerUnits				// Incoming units (user picks from UI).
		);	
	
	// Then finally offset it by our "frame" amount:

	newRect.top += (int32)convertRect.top;
	newRect.left += (int32)convertRect.left;
	newRect.bottom += (int32)convertRect.bottom;
	newRect.right += (int32)convertRect.right;

	error = SetSelection(newRect);
		
	return error;
	
	}

//-------------------------------------------------------------------------------
//
//	SetRectFromGutter
//
//	A gutter amount to an actual offset rect.
//
//-------------------------------------------------------------------------------
SPErr SetRectFromGutter
	(
	/* IN */	const double gutter,
	/* OUT */	PIUDRect_t* outRect
	)
	{
	SPErr error = kSPNoError;
	
	if (outRect != NULL)
		{
		outRect->top = gutter;
		outRect->left = gutter;
		outRect->bottom = -gutter;
		outRect->right = -gutter;
		}
	else
		{
		error = kSPBadParameterError;
		}
		
	return error;
	
	}

//-------------------------------------------------------------------------------
//
//	ConvertUnits
//
//	Converts from one unit of measurement to another.
//
//-------------------------------------------------------------------------------
static double ConvertUnits
	(
	/* IN */	const double value,				// Value to convert.
	/* IN */	const double resolutionPerInch,	// Resolution of document in pixels per inch.
	/* IN */	const double documentBound,		// For percent, a bound of the document in unitDistance.
	/* IN */	const RulerUnits inUnits,		// Incoming units.
	/* IN */	const RulerUnits outUnits		// Desired outgoing units.
	)
	{
	const double kOnePointInInches = 0.01380;
	const double kPointsPerInch = 72.46376;
	const double kPicasPerInch = kOnePointInInches*12.0;
	const double kCentemetersPerInch = 2.540;
	
	double outValue = 0.0;
	double valueInUnitDistance = 0.0;
	
	// First, we'll convert the inUnits to a standard value of pixels
	// at 72 pixels per inch.  This is unitDistance.
	switch (inUnits)
		{
		case kRulerPixels:
			{
			valueInUnitDistance = value / resolutionPerInch * kUnitDistanceBasePerInch;
			break;
			}
		case kRulerInches:
			{
			valueInUnitDistance = value * resolutionPerInch;
			break;
			}
		case kRulerCm:
			{
			valueInUnitDistance = value * kCentemetersPerInch * resolutionPerInch;
			break;
			}
		case kRulerPoints:
			{
			valueInUnitDistance = value * kPointsPerInch * resolutionPerInch;
			break;
			}
		case kRulerPicas:
			{
			valueInUnitDistance = value * kPicasPerInch * resolutionPerInch;
			break;
			}
		case kRulerPercent:
			{
			valueInUnitDistance = value * documentBound;
			break;
			}
		default:
			{
			// Error, unknown case.
			break;
			}
		} // inUnits
		
	// Now convert our number (in 72 pixels per inch) to our requested out value:
	switch (outUnits)
		{
		case kRulerPixels:
			{
			outValue = (valueInUnitDistance / kUnitDistanceBasePerInch) * resolutionPerInch;
			break;
			}
		case kRulerInches:
			{
			outValue = valueInUnitDistance / kUnitDistanceBasePerInch;
			break;
			}
		case kRulerCm:
			{
			outValue = (valueInUnitDistance / kUnitDistanceBasePerInch) * kCentemetersPerInch;
			break;
			}
		case kRulerPoints:
			{
			outValue = (valueInUnitDistance / kUnitDistanceBasePerInch) * kPointsPerInch;
			break;
			}
		case kRulerPicas:
			{
			outValue = (valueInUnitDistance / kUnitDistanceBasePerInch) * kPicasPerInch;
			break;
			}
		case kRulerPercent:
			{
			outValue = valueInUnitDistance / documentBound;
			break;
			}
		default:
			{
			// Error, unknown case.
			break;
			}
		} // outUnits
		
		return outValue;
		
	} // ConvertUnits

//-------------------------------------------------------------------------------
//
//	ConvertToUnitDistance
//
//	Converts a distance and returns it in standard
//	unitDistance units (72 dpi).
//
//-------------------------------------------------------------------------------
static double ConvertToUnitDistance
	(
	const double amount, 
	const double amountBasePerInch
	)
	{
	return ((amount / amountBasePerInch) * kUnitDistanceBasePerInch);
	}

//-------------------------------------------------------------------------------
//
//	ConvertFromUnitDistance
//
//	Converts from unitDistance units (72 dpi) to a standard
//	base per inch.
//
//-------------------------------------------------------------------------------
static double ConvertFromUnitDistance
	(
	const double amount, 
	const double amountBasePerInch
	)
	{
	return ((amount / kUnitDistanceBasePerInch) * amountBasePerInch);
	}

//-------------------------------------------------------------------------------
//
//	ConvertFromUnitDistanceToUnits
//
//	Converts from unitDistance units (72 dpi) to a standard
//	base per inch of a given units.
//
//-------------------------------------------------------------------------------
double ConvertFromUnitDistanceToUnits
	(
	const double value, 				// Incoming value.
	const double resolutionPerInch,		// Resolution of document in pixels per inch.
	const double documentBound,			// For percent.
	const RulerUnits outUnits			// Desired units.
	)
	{
	double outValue = ConvertFromUnitDistance(value, resolutionPerInch);

	outValue = ConvertUnits
		(
		outValue,
		resolutionPerInch,
		documentBound,
		kRulerPixels,			// unitDistance is a special pixels.
		outUnits
		);

	return outValue;
	
	}

//-------------------------------------------------------------------------------
//
//	ConvertFromUnitsToUnitDistance
//
//	Converts to unitDistance units (72 dpi) from a standard
//	base per inch of a given units.
//
//-------------------------------------------------------------------------------
double ConvertFromUnitsToUnitDistance
	(
	const double value, 				// Incoming value.
	const double resolutionPerInch,		// Resolution of document in pixels per inch.
	const double documentBound,			// For percent.
	const RulerUnits inUnits			// Desired units.
	)
	{
	double outValue = ConvertToUnitDistance(value, resolutionPerInch);

	outValue = ConvertUnits
		(
		outValue,
		resolutionPerInch,
		documentBound,
		inUnits,
		kRulerPixels			// unitDistance is a special pixels.
		);

	return outValue;
	
	}

//-------------------------------------------------------------------------------
//
//	ConvertRect
//
//	Converts a rectangle from one unit base to another.
//
//-------------------------------------------------------------------------------
PIUDRect_t ConvertRect
	(
	/* IN */		const PIUDRect_t inRect,			// Input rect.
	/* IN */		const double resolutionPerInch,		// Resolution of document in pixels per inch.
	/* IN */		const PIUDPoint_t documentBounds,	// For percent, a bounds of the document in unitDistance.
	/* IN */		const RulerUnits inUnits,			// Incoming units.
	/* IN */		const RulerUnits outUnits			// Desired outgoing units.
	)
	{
	PIUDRect_t outRect = { 0.0, 0.0, 0.0, 0.0 };
	
	outRect.top = ConvertUnits
		(
		inRect.top,
		resolutionPerInch,
		documentBounds.v,
		inUnits,
		outUnits
		);
		
	outRect.left = ConvertUnits
		(
		inRect.left,
		resolutionPerInch,
		documentBounds.h,
		inUnits,
		outUnits
		);
		
	outRect.bottom = ConvertUnits
		(
		inRect.bottom,
		resolutionPerInch,
		documentBounds.v,
		inUnits,
		outUnits
		);
		
	outRect.right = ConvertUnits
		(
		inRect.right,
		resolutionPerInch,
		documentBounds.h,
		inUnits,
		outUnits
		);
		
	return outRect;
	
	}

//-------------------------------------------------------------------------------
//
//	ConvertRectToUnitDistance
//
//	Converts a rectangle from one unit base to unitDistance, which is a
//	special pixels unit at an absolute 72 dpi.
//
//-------------------------------------------------------------------------------
PIUDRect_t ConvertRectToUnitDistance
	(
	/* IN */		const PIUDRect_t inRect,			// Input rect.
	/* IN */		const double resolutionPerInch,		// Resolution of document in pixels per inch.
	/* IN */		const PIUDPoint_t documentBounds,	// For percent, a bounds of the document in unitDistance.
	/* IN */		const RulerUnits inUnits			// Incoming units.
	)
	{
	PIUDRect_t outRect = ConvertRect
		(
		inRect,
		resolutionPerInch,
		documentBounds,
		inUnits,
		kRulerPixels
		);
	
	outRect.top = ConvertToUnitDistance
		(
		outRect.top,
		resolutionPerInch
		);
		
	outRect.left = ConvertToUnitDistance
		(
		outRect.left,
		resolutionPerInch
		);

	outRect.bottom = ConvertToUnitDistance
		(
		outRect.bottom,
		resolutionPerInch
		);
				
	outRect.right = ConvertToUnitDistance
		(
		outRect.right,
		resolutionPerInch
		);
		
	return outRect;
	
	}
	
//-------------------------------------------------------------------------------
//
//	ConvertRectFromUnitDistance
//
//	Converts a rectangle from unitDistance, which is a
//	special pixels unit at an absolute 72 dpi, to any given
//	ruler units.
//
//-------------------------------------------------------------------------------
PIUDRect_t ConvertRectFromUnitDistance
	(
	/* IN */		const PIUDRect_t inRect,			// Input rect.
	/* IN */		const double resolutionPerInch,		// Resolution of document in pixels per inch.
	/* IN */		const PIUDPoint_t documentBounds,	// For percent, a bounds of the document in unitDistance.
	/* IN */		const RulerUnits outUnits			// Desired units.
	)
	{
	PIUDRect_t outRect = { 0.0, 0.0, 0.0, 0.0 };
	
	outRect.top = ConvertFromUnitDistance
		(
		inRect.top,
		resolutionPerInch
		);
		
	outRect.left = ConvertFromUnitDistance
		(
		inRect.left,
		resolutionPerInch
		);

	outRect.bottom = ConvertFromUnitDistance
		(
		inRect.bottom,
		resolutionPerInch
		);
				
	outRect.right = ConvertFromUnitDistance
		(
		inRect.right,
		resolutionPerInch
		);
		
	outRect = ConvertRect
		(
		outRect,
		resolutionPerInch,
		documentBounds,
		kRulerPixels,
		outUnits
		);
	
	return outRect;
	
	}
// end FrameSelect.cpp
