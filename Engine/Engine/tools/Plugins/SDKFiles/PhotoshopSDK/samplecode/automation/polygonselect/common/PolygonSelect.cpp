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
//		PolygonSelect.cpp
//
//	Copyright 1999-1999, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Description:
//		This creates a polygon shaped selection with any number of sides
//		based on the radius (length from the center of document to each vertex.)
//		In addition, allows user to rotate the selection in degrees.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------


#include "PhotoshopSDK.h"
#include "PolygonSelect.h"

SPBasicSuite* sSPBasic = NULL;
SPPluginRef	gPlugInRef = NULL;

//-------------------------------------------------------------------------------
//	Globals -- Define global variables for plug-in scope.
//-------------------------------------------------------------------------------


long gRadius; // distance from center of polygon to a vertex.
long gNumberSides;// number of sides in polygon
double gRotationAngle;//rotation in degrees
PIUDPoint_t gDocumentCenter;//half the dimensions of the document
PIUDPoint_t 	gDocumentBounds;


//-------------------------------------------------------------------------------
//	Prototypes.
//-------------------------------------------------------------------------------

SPErr IteratePolygonVertices(void);

double GetReferenceAngle( double sumCentralAngles, int * quadrant);
void ConvertDegreesToRadians(double angleInDegrees, double * angleInRadians);
PIUDPoint_t CalculateVertex ( long radius, double referenceAngle, int quadrant);
void SetVertex (PIActionList list, PIUDPoint_t vertex);
void RotateSelection (void);

// Main routine:
static SPErr Execute (PIActionParameters* actionParams);

// Initialize our parameters:
static void Initialize (void);

// Get the document bounds:
static PIUDPoint_t GetDocumentBounds (void);

// Return the document's ruler units setting:
static RulerUnits GetDocumentRulerUnits (void);


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
static SPErr Execute(PIActionParameters* actionParams)
	{
	SPErr error = kSPNoError;
	
	Initialize(); // Default parameters for all globals.
	
	ReadScriptParams(actionParams); // Override globals with new descriptor info.
	
	// Determine if we need to pop our dialog:
	PIDialogPlayOptions playInfo = actionParams->playInfo;
	
	if (playInfo == plugInDialogDisplay)// Others = plugInDialogDontDisplay / plugInDialogSilent
		{
		// Go ahead and display a dialog:
		error = DoUI();
		}

	if (error == kSPNoError)
	{
			error = IteratePolygonVertices();
			RotateSelection();
	}
	if (error == kSPNoError)
				WriteScriptParams(actionParams);
		
		// else return userCanceledErr.	
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
	gDocumentBounds = GetDocumentBounds();
	gRadius = 0;
	gNumberSides = 0;
	gRotationAngle = 0.0;
	gDocumentCenter.h = (gDocumentBounds.h)/2;
	gDocumentCenter.v = (gDocumentBounds.v)/2;
	}

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
//	IteratePolygonVertices
//
//	Assigns the coordinates of each vertex.
//	Calls GetReferenceAngles, CalculateVertex, and SetVertex.
//	
//
//-------------------------------------------------------------------------------

SPErr IteratePolygonVertices(void)
	{
	long radius = gRadius;
	long numberSides = gNumberSides;
	PIUDPoint_t vertex = 	{0.0,0.0};
	PIUDPoint_t startingVertex = {0.0,0.0};
	double sumCentralAngle = 0.0;
	double oneCentralAngle = 360.0/numberSides;
	int i, quadrant;
	double referenceAngle = 0.0;
	

	PIActionDescriptor result;

	PIActionDescriptor mainDescriptor = NULL;
	sPSActionDescriptor->Make(&mainDescriptor);

	PIActionReference reference;
	sPSActionReference->Make(&reference);
	sPSActionReference->PutProperty(reference, classChannel, keySelection);
	sPSActionDescriptor->PutReference(mainDescriptor, keyNull, reference);


	SPErr error = kSPNoError;

	if (sPSActionDescriptor != NULL)
		{
		//make descriptor for list here
		PIActionList list = NULL;
		sPSActionList->Make(&list);

		//loop through the vertices and assign the points to list descriptor	

		for (i=0; i < numberSides; i++)
			{

			if (i == 0)
				sumCentralAngle = 0.0;
			else
				sumCentralAngle +=oneCentralAngle;

			//vertices are translated from polygon's center, which is based on the 
			//angle displacement from the maximum degrees in the quadrant in which
			//the vertex lies.

			referenceAngle = GetReferenceAngle(sumCentralAngle, &quadrant);

			//make translation from the center by adding/subtracting rcos/rsin

			vertex = CalculateVertex(radius, referenceAngle, quadrant);

			//save the first vertex
			if (i == 0)
				startingVertex = vertex;
			
			SetVertex(list, vertex);
			}
			//close the polygon
			SetVertex(list, startingVertex);

			
			PIActionDescriptor listDescriptor;
			sPSActionDescriptor->Make(&listDescriptor);

			
			if (error == kSPNoError && listDescriptor != NULL)
				{

				error = sPSActionDescriptor->PutList
				(
				listDescriptor,
				keyPoints,
				list
				);
				}

			if (error == kSPNoError)
				{

				error =  sPSActionDescriptor->PutObject
					(
					mainDescriptor,
					keyTo,
					classPolygon,
					listDescriptor
					);
				}

			if (error == kSPNoError)
				{
				error =  sPSActionDescriptor->PutBoolean
					(
					mainDescriptor,
					keyAntiAlias,
					true
					);
				}

			if (error == kSPNoError)
				{
				error =  sPSActionControl->Play
					(
					&result,
					eventSet,
					mainDescriptor,
					plugInDialogSilent
					);
				}
		}
	return error;
	}
			
//-------------------------------------------------------------------------------
//
//	GetReferenceAngle
//
//	Calculates difference between maximum degree in given quadrant 
//	and sumCentralAngle.
//	
//
//-------------------------------------------------------------------------------

double GetReferenceAngle( double sumCentralAngles, int * quadrant)
	{
	double referenceAngleInDegrees = 0.0;
	double referenceAngleInRadians = 0.0;

	if (sumCentralAngles ==0)
		{
		*quadrant = 0;
		referenceAngleInDegrees = 0.0;
		}
	else if (sumCentralAngles > 0.0 && sumCentralAngles <= 90.0)
		{
		*quadrant = 1;
		referenceAngleInDegrees = (90.0 - sumCentralAngles);
		}
	else if (sumCentralAngles > 90 && sumCentralAngles <= 180)
		{
		*quadrant = 2;
		referenceAngleInDegrees = (180.0 - sumCentralAngles);
		}
	else if (sumCentralAngles > 180.0 && sumCentralAngles <= 270.0)
		{
		*quadrant = 3;
		referenceAngleInDegrees = (270.0 - sumCentralAngles);
		}
	else if (sumCentralAngles > 270.0 && sumCentralAngles < 360.0)
		{
		*quadrant = 4;
		referenceAngleInDegrees = (360.0 - sumCentralAngles);
		}

	
	ConvertDegreesToRadians(referenceAngleInDegrees, &referenceAngleInRadians);
	
	return referenceAngleInRadians;
	}
//-------------------------------------------------------------------------------
//
//	ConvertDegreesToRadians
//
//	Converts angle from degree units to radian units. 
//	Trigonometric functions (sin, cos, tan,...) take radians. 
//	
//
//-------------------------------------------------------------------------------

void ConvertDegreesToRadians(double angleInDegrees, double * angleInRadians)
	{
	const double pi = 3.141592;

	double result = angleInDegrees*(pi/180.0);

	*angleInRadians = result;
	}
//-------------------------------------------------------------------------------
//
//	CalculateVertex
//
//	After we have the angle, we can calculate the horizontal 
//	and vertical translation from the center of the polygon 
//	to the vertex.
//
//-------------------------------------------------------------------------------

PIUDPoint_t CalculateVertex ( long radius, double referenceAngle, int quadrant)
	{
	double x = gDocumentCenter.h;
	double y = gDocumentCenter.v;
	PIUDPoint_t point = {0.0,0.0};


	switch (quadrant)
		{
		case 0:
			point.h = x;
			point.v = y - radius;
		
			break;

		case 1:
			point.h = x + (radius*cos(referenceAngle));
			point.v = y - (radius*sin(referenceAngle));
			break;

		case 2:
			point.h = x + (radius*sin(referenceAngle));
			point.v = y + (radius*cos(referenceAngle));
			break;

		case 3:
			point.h = x - (radius*cos(referenceAngle));
			point.v = y + (radius*sin(referenceAngle));
			break;

		case 4:
			point.h = x - (radius*sin(referenceAngle));
			point.v = y - (radius*cos(referenceAngle));
			break;

		default:
			break;
		}

	return point;
	}

//-------------------------------------------------------------------------------
//
//	SetVertex
//
//	Assigns horizontal and vertical pixel coordinates to vertex. 
//
//	
//
//-------------------------------------------------------------------------------

void SetVertex (PIActionList list, PIUDPoint_t vertex)
	{
	SPErr error = kSPNoError;

	PIActionDescriptor descriptor = NULL;
	error = sPSActionDescriptor->Make(&descriptor);

	if (error == kSPNoError && descriptor != NULL)
		{
		error = sPSActionDescriptor->PutUnitFloat
			(
			descriptor,
			keyHorizontal,
			unitPixels,
			vertex.h
			);

		error = sPSActionDescriptor->PutUnitFloat
			(
			descriptor,
			keyVertical,
			unitPixels,
			vertex.v
			);
		}

	if (error == kSPNoError)
		{
		sPSActionList->PutObject
		(
		list,
		classPoint,
		descriptor
		);
		}
	}

//-------------------------------------------------------------------------------
//
//	RotateSelection
//
//	Rotates the current selection by a given degrees. 
//
//	
//
//-------------------------------------------------------------------------------

void RotateSelection (void)
	{
	PIActionDescriptor result = NULL;
	PIActionDescriptor descriptor = NULL;
	sPSActionDescriptor->Make(&descriptor);

	PIActionReference reference = NULL;
	sPSActionReference->Make(&reference);

	sPSActionReference->PutProperty(
		reference,
		classChannel,
		keySelection
		);

	sPSActionDescriptor->PutReference(
		descriptor,
		keyNull,
		reference
		);

	sPSActionDescriptor->PutEnumerated(
		descriptor,
		keyFreeTransformCenterState,
		typeQuadCenterState,
		enumQCSAverage
		);

	PIActionDescriptor corner;
	sPSActionDescriptor->Make(&corner);

	sPSActionDescriptor->PutUnitFloat(
		corner,
		keyHorizontal,
		unitDistance,
		0
		);

	sPSActionDescriptor->PutUnitFloat(
		corner,
		keyVertical,
		unitDistance,
		0
		);

	sPSActionDescriptor->PutObject(
		descriptor,
		keyOffset,
		classOffset,
		corner
		);

	sPSActionDescriptor->PutUnitFloat(
		descriptor,
		keyAngle,
		unitAngle,
		gRotationAngle
		);

	sPSActionControl->Play(
		&result,
		eventTransform,
		descriptor,
		plugInDialogSilent
		);
	}
// end PolygonSelect.cpp
