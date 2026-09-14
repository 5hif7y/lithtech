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
//		PathsToPostScript.cpp
//
//	Copyright 1990-1992, Thomas Knoll.
//	All Rights Reserved.
//
//	Description:
//		This file contains the functions and source
//		for the Export module PathsToPostScript, a module that
//		creates a file with PostScript information
//		detailing a specific path, or all paths.  It
//		is taken from the source of the "Paths to Illustrator"
//		plug-in that ships with the product.
//
//	Use:
//		This module shows how to convert paths accessed
//		from the properties suite into PostScript and save
//		them to a file.  You access it via
//		File>>Export>>PathsToPostScript.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes 
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "PathsToPostScript.h"

//-------------------------------------------------------------------------------
//	Prototypes.
//-------------------------------------------------------------------------------

void InitGlobals (Ptr globalPtr);		  	// Initialize globals.
void ValidateParameters (GPtr globals);		// Validate parameters.
void DoPrepare (GPtr globals);
void DoStart (GPtr globals);				// Main routine.
void DoContinue (GPtr globals);
void DoFinish (GPtr globals);
void DoInitialRect (GPtr globals);

Boolean WriteResString (GPtr globals, short resID);
Boolean WriteReplaceString (GPtr globals, short resID, Str255 s2);
Boolean CheckRange (GPtr globals, int32 scalar, int32 pixels);
Boolean ConvertSavedPoint (GPtr globals,
						  int32 savedX,
						  int32 savedY,
						  Fixed *x,
						  Fixed *y);
Boolean WriteToFile (GPtr globals, void *data, long count);
Boolean WriteString (GPtr globals, Str255 s);
Boolean WriteChar (GPtr globals, unsigned char c);
Boolean WriteNewLine (GPtr globals);
Boolean WriteSpace (GPtr globals);
Boolean WriteInteger (GPtr globals, short x);
void ConvertFixed (Fixed value, int16 places, Str255 s);
Boolean WriteFixed (GPtr globals, Fixed x, int16 places);
Boolean WriteBoundingBox (GPtr globals);
Boolean WriteFileHeader (GPtr globals);
Boolean WritePoint (GPtr globals, Fixed x, Fixed y);
Boolean WriteMoveTo (GPtr globals, Fixed x, Fixed y);
Boolean WriteLineTo (GPtr globals, Fixed x, Fixed y);
Boolean WriteSegment (GPtr globals,
					 Fixed startX,
					 Fixed startY,
					 Fixed fwdX,
					 Fixed fwdY,
					 Fixed bwdX,
					 Fixed bwdY,
					 Fixed endX,
					 Fixed endY,
					 Boolean cusp);
Boolean WriteNewPath (GPtr globals, Boolean closeOldPath);
void AdvanceOneElement (SavedPathElement **path, int32 *elements);
void SkipUnidentifiedElements (SavedPathElement **path, int32 *elements);
Boolean GetNextElement (SavedPathElement **path,
					   int32 *elements,
					   int16 *kind);
Boolean WriteSubPath (GPtr globals,
					 SavedPathElement **path, 
					 int32 *elements);
Boolean WritePath (GPtr globals, Handle savedPath);
Handle GetIndexedPath (GPtr globals, int32 index);
Boolean WriteIndexedPath (GPtr globals, int32 index);
Boolean WriteAllPaths (GPtr globals);
Boolean WriteFileTrailer (GPtr globals);
Boolean ConvertToPoints (GPtr globals,
						int16 pixels,
						Fixed resolution,
						Fixed *value);
Boolean CalculateBoundingBox (GPtr globals);
Boolean WriteBody (GPtr globals);
Fixed DoFixMul (int32 x, int32 y);
int32 DoFloor (double x);
Handle FixupPath(GPtr globals, Handle path, int howMany);
void FixupOne( char *data, SavedPathElement **path );
unsigned32 SwapLong( char *data );
unsigned16 SwapWord( char *data );

//-------------------------------------------------------------------------------
//	Globals -- Define global variables for plug-in scope.
//-------------------------------------------------------------------------------

SPBasicSuite * sSPBasic = NULL;

const int32 kSizeSavedPath = sizeof(SavedPathElement);

//-------------------------------------------------------------------------------
//
//	PluginMain / main
//
//	All calls to the plug-in module come through this routine.
//	It must be placed first in the resource.  To achieve this,
//	most development systems require this be the first routine
//	in the source.
//
//	The entrypoint will be "pascal void" for Macintosh,
//	"void" for Windows.
//
//	Inputs:
//		const short selector						Host provides selector indicating
//													what command to do.
//
//		ExportRecord *exportParamBlock				Host provides pointer to parameter
//													block containing pertinent data
//													and callbacks from the host.
//													See PIExport.h.
//
//	Outputs:
//		ExportRecord *exportParamBlock				Host provides pointer to parameter
//													block containing pertinent data
//													and callbacks from the host.
//													See PIExport.h.
//
//		long *data									Use this to store a handle to our
//													global parameters structure, which
//													is maintained by the host between
//													calls to the plug-in.
//
//		short *result								Return error result or noErr.  Some
//													errors are handled by the host, some
//													are silent, and some you must handle.
//													See PIGeneral.h.
//
//-------------------------------------------------------------------------------

DLLExport MACPASCAL void PluginMain (const short selector,
						             void *exportParamBlock,
						             long *data,
						             short *result)
{
	//---------------------------------------------------------------------------
	//	(1) Check for about box request.
	//
	// 	The about box is a special request; the parameter block is not filled
	// 	out, none of the callbacks or standard data is available.  Instead,
	// 	the parameter block points to an AboutRecord, which is used
	// 	on Windows.
	//---------------------------------------------------------------------------

	if (selector == exportSelectorAbout)
	{
		sSPBasic = ((AboutRecord*)exportParamBlock)->sSPBasic;
		DoAbout((AboutRecordPtr)exportParamBlock);
	}
	else
	{ // do the rest of the process as normal:

		sSPBasic = ((ExportRecordPtr)exportParamBlock)->sSPBasic;

		Ptr globalPtr = NULL;		// Pointer for global structure
		GPtr globals = NULL; 		// actual globals

		//-----------------------------------------------------------------------
		//	(2) Allocate and initalize globals.
		//
		// 	AllocateGlobals requires the pointer to result, the pointer to the
		// 	parameter block, a pointer to the handle procs, the size of our local
		// 	"Globals" structure, a pointer to the long *data, a Function
		// 	Proc (FProcP) to the InitGlobals routine.  It automatically sets-up,
		// 	initializes the globals (if necessary), results result to 0, and
		// 	returns with a valid pointer to the locked globals handle or NULL.
		//-----------------------------------------------------------------------
		
		globalPtr = AllocateGlobals ((uint32)result,
									 (uint32)exportParamBlock,
									 ((ExportRecordPtr)exportParamBlock)->handleProcs,
									 sizeof(Globals),
						 			 data,
						 			 InitGlobals);
		
		if (globalPtr == NULL)
		{ // Something bad happened if we couldn't allocate our pointer.
		  // Fortunately, everything's already been cleaned up,
		  // so all we have to do is report an error.
		  
		  *result = memFullErr;
		  return;
		}
		
		// Get our "globals" variable assigned as a Global Pointer struct with the
		// data we've returned:
		globals = (GPtr)globalPtr;

		//-----------------------------------------------------------------------
		//	(3) Dispatch selector.
		//-----------------------------------------------------------------------

		switch (selector)
		{
			case exportSelectorPrepare:
				DoPrepare(globals);
				break;
			case exportSelectorStart:
				DoStart(globals);
				break;
			case exportSelectorContinue:
				DoContinue(globals);
				break;
			case exportSelectorFinish:
				DoFinish(globals);
				break;
		}
							
			
		//-----------------------------------------------------------------------
		//	(4) Unlock data, and exit resource.
		//
		//	Result is automatically returned in *result, which is
		//	pointed to by gResult.
		//-----------------------------------------------------------------------	
		
		// unlock handle pointing to parameter block and data so it can move
		// if memory gets shuffled:
		if ((Handle)*data != NULL)
			PIUnlockHandle((Handle)*data);
	
	} // about selector special		
	
} // end PluginMain

//-------------------------------------------------------------------------------
//
//	InitGlobals
//	
//	Initalize any global values here.  Called only once when global
//	space is reserved for the first time.
//
//	Inputs:
//		Ptr globalPtr		Standard pointer to a global structure.
//
//	Outputs:
//		Initializes any global values with their defaults.
//
//-------------------------------------------------------------------------------

void InitGlobals (Ptr globalPtr)
{	
	// create "globals" as a our struct global pointer so that any
	// macros work:
	GPtr globals = (GPtr)globalPtr;

	gPopUpValue = 0;
	
	// Initialize global variables:
	ValidateParameters (globals);
	
} // end InitGlobals


//-------------------------------------------------------------------------------
//
//	ValidateParameters
//
//	Initialize parameters to default values.
//
//	Inputs:
//		GPtr globals		Pointer to global structure.
//
//	Outputs:
//		gAliasHandle		Default: NULL.
//
//		gPopUpValue			Default: NULL.
//
//		gQueryForParameters	Default: TRUE.
//
//-------------------------------------------------------------------------------

void ValidateParameters (GPtr globals)
{
	gQueryForParameters = true;
	gAliasHandle = NULL; // no handle, yet
	if (gPopUpValue < 1)
		gPopUpValue = 0;
	
} // end ValidateParameters

//-------------------------------------------------------------------------------
//
//	DoPrepare
//
//	Initialize parameters to default values.
//
//	Inputs:
//
//	Outputs:

/* We don't particularly need any memory. */

void DoPrepare (GPtr globals)
{
	
	gStuff->maxData = 0;
	
}

/*****************************************************************************/

/* Display the point-out-of range error message. */

Boolean CheckRange (GPtr globals, int32 scalar, int32 pixels)
{
		
	if (scalar < 0) scalar = -scalar;
	
	scalar = scalar >> 24;
	
	pixels = pixels >> 16;
	
	if (scalar * pixels >= 0x00008000L) // magic number
	{
		DisplayStopAlert (globals, kAIRange);
		return FALSE;
	}
		
	return TRUE;
	
}


/*****************************************************************************/

Fixed DoFixMul (int32 x, int32 y)
{
	double xd = Fixed16ToDouble(x);
	double yd = Fixed16ToDouble(y);
	double pd = xd*yd;
	return DoubleToFixed16(pd);
}

/*****************************************************************************/

Boolean ConvertSavedPoint (GPtr globals,
						  int32 savedX,
						  int32 savedY,
						  Fixed *x,
						  Fixed *y)
{
	
	if (CheckRange (globals, savedX, gDocWidth) &&
		CheckRange (globals, savedY, gDocHeight))
		{
		
		*x = DoFixMul (savedX, gDocWidth >> 8);
		*y = DoFixMul (kSavedOne - savedY, gDocHeight >> 8);
		
		return TRUE;
		
		}
		
	else
		return FALSE;
	
}

/*****************************************************************************/

void AppendStringAndString (Str255 s, Str255 append)
{	
	AppendString(s, append, 1, append[0]);
}

/*****************************************************************************/

void AppendStringAndCharacter (Str255 s, unsigned char c)
{	
	AppendCharToString(s, c);
}

/*****************************************************************************/

Boolean WriteToFile (GPtr globals, void *data, long count)
	{
	

	if (gResult == noErr)

		gResult = FSWrite (gFRefNum, &count, data);

	
	return (gResult == noErr);
	
	}

/*****************************************************************************/

Boolean WriteString (GPtr globals, Str255 s)
	{
	
	if (s[0] == 0)
		return TRUE;
	
	return WriteToFile (globals, &(s[1]), s[0]);
	
	}

/*****************************************************************************/

Boolean WriteChar (GPtr globals, unsigned char c)
	{
	
	return WriteToFile (globals, &c, 1);
	
	}

/*****************************************************************************/

Boolean WriteNewLine (GPtr globals)
	{
	
	return WriteChar (globals, 0x0D);
	
	}
	
/*****************************************************************************/

Boolean WriteSpace (GPtr globals)
	{
	
	return WriteChar (globals, 0x20);
	
	}
	
/*****************************************************************************/

Boolean WriteResString (GPtr globals, short resID)
{
	
	Str255 	s = "";
	
	PIGetString( (int32)resID, s);
	return WriteString (globals, s);
}

/*****************************************************************************/

Boolean WriteReplaceString (GPtr globals, short resID, Str255 s2)
{
	
	Str255 	s = "";

	PIGetString((int32)resID, s);
	PIParamText(s, s2, NULL, NULL); // replaces ^0 with s2
	
	return WriteString (globals, s);
}

/*****************************************************************************/

Boolean WriteInteger (GPtr globals, short x)
	{
	
	Str255 s;
	
	NumToString (x, s);
	
	return WriteString (globals, s);
	
	}

/*****************************************************************************/

void ConvertFixed (Fixed value, int16 places, Str255 s)
{
	double x = 0;
	long l = 0;

	if (places > 0)

	{ // use decimal converter
		x = PIFixedToDouble(value, kHard16);
		DoubleToString (s, x, places);

	}

	else

	{ // no decimal number required

		l = fixed2long(value);

		NumToString(l, s);

	}
}
/*****************************************************************************/

Boolean WriteFixed (GPtr globals, Fixed x, int16 places)
{
	
	Str255 s;
	
	ConvertFixed (x, places, s);
	
	return WriteString (globals, s);
	
}

/*****************************************************************************/

Boolean WriteBoundingBox (GPtr globals)
{
	

	return WriteResString	(globals, kPSBoundingBox)		&&
		   WriteFixed 		(globals, 0, 0) 				&&
		   WriteSpace		(globals)						&&
		   WriteFixed		(globals, 0, 0)					&&
		   WriteSpace 		(globals) 						&&
		   WriteFixed 		(globals, gDocWidth, 0) 		&&
		   WriteSpace 		(globals)						&&
		   WriteFixed 		(globals, gDocHeight, 0) 		&&
		   WriteNewLine 	(globals) 						&&

	       WriteResString	(globals, kPSHiResBoundingBox) 	&&
		   WriteFixed 		(globals, 0, 4) 				&&
		   WriteSpace		(globals)						&&
		   WriteFixed		(globals, 0, 4)					&&
		   WriteSpace 		(globals) 						&&
		   WriteFixed 		(globals, gDocWidth, 4) 		&&
		   WriteSpace 		(globals)						&&
		   WriteFixed 		(globals, gDocHeight, 4) 		&&
		   WriteNewLine 	(globals) 						&&
	   
	       WriteResString	(globals, kPSCropmarks) 		&&
		   WriteFixed 		(globals, 0, 0) 				&&
		   WriteSpace		(globals)						&&
		   WriteFixed		(globals, 0, 0)					&&
		   WriteSpace 		(globals) 						&&
		   WriteFixed 		(globals, gDocWidth, 0) 		&&
		   WriteSpace 		(globals)						&&
		   WriteFixed 		(globals, gDocHeight, 0) 		&&
		   WriteNewLine 	(globals);		   
}

/*****************************************************************************/

Boolean WriteTitle (GPtr globals)
{

	Str255 s = "";



	SmallFileName(s, gStuff->filename); // returns small file name as pascal
	return WriteReplaceString(globals, kPSTitle, s);
}

/*****************************************************************************/

Boolean WriteFileHeader (GPtr globals)
{
	
	return WriteResString		(globals, kPSHeader)		&&
		   WriteTitle			(globals)					&&
		   WriteResString		(globals, kPSProcSets)		&&
		   WriteBoundingBox		(globals)					&&
		   WriteResString		(globals, kPSBegin);
}

/*****************************************************************************/

Boolean WritePoint (GPtr globals, Fixed x, Fixed y)
{
	
	return WriteFixed		(globals, x, 4) 		&&
		   WriteSpace		(globals)		 		&&
		   WriteFixed 		(globals, y, 4) 		&&
		   WriteSpace 		(globals);
	
}

/*****************************************************************************/

Boolean WriteMoveTo (GPtr globals, Fixed x, Fixed y)
{
	
	return WritePoint 		(globals, x, y) 		&&
           WriteChar 		(globals, 'm') 			&&
		   WriteNewLine 	(globals);
	
}

/*****************************************************************************/

Boolean WriteLineTo (GPtr globals, Fixed x, Fixed y)
	{
	
	return WritePoint 		(globals, x, y) 		&&
           WriteChar 		(globals, 'l') 			&&
		   WriteNewLine 	(globals);
	
	}

/*****************************************************************************/

Boolean WriteSegment (GPtr globals,
					 Fixed startX,
					 Fixed startY,
					 Fixed fwdX,
					 Fixed fwdY,
					 Fixed bwdX,
					 Fixed bwdY,
					 Fixed endX,
					 Fixed endY,
					 Boolean cusp)
{
	
	Boolean startTangent = (startX != fwdX) || (startY != fwdY);
	Boolean endTangent   = (endX   != bwdX) || (endY   != bwdY);
	
	Boolean result = TRUE;
	
	unsigned char cmd;
	
	if (startTangent)
		result = result && WritePoint (globals, fwdX, fwdY);
		
	if (endTangent)
		result = result && WritePoint (globals, bwdX, bwdY);
		
	result = result && WritePoint (globals, endX, endY);
	
	if (startTangent && endTangent)
		cmd = 'C';
	else if (startTangent)
		cmd = 'Y';
	else if (endTangent)
		cmd = 'V';
	else
		cmd = 'L';
		
	if (!cusp)
		cmd += 'a' - 'A';
		
	return result && WriteChar (globals, cmd) && WriteNewLine (globals);
	
}

/*****************************************************************************/

Boolean WriteNewPath (GPtr globals, Boolean closeOldPath)
{
	

	return	WriteChar 	(globals, (char)(closeOldPath ? 'n' : 'N')) 	&&
			WriteNewLine	(globals);
}

/*****************************************************************************/

void AdvanceOneElement (SavedPathElement **path, int32 *elements)
{
	
	if (*elements > 1)
		{
		--(*elements);
		++(*path);
		}
	
	else
		{
		*path = NULL;
		*elements = 0;
		}
	
}

/*****************************************************************************/

void SkipUnidentifiedElements (SavedPathElement **path, int32 *elements)
{
	
	while (*path != NULL && *elements > 0)
		{
		
		int16 kind = (**path).kind;
		
		if (kMinPathKind <= kind && kind <= kMaxPathKind)
			return;
			
		AdvanceOneElement (path, elements);
		
		}
	
}

/*****************************************************************************/

Boolean GetNextElement (SavedPathElement **path,
					   int32 *elements,
					   int16 *kind)
{
	
	AdvanceOneElement (path, elements);
	
	SkipUnidentifiedElements (path, elements);
	
	if (*path != NULL && *elements > 0)
		{
		*kind = (**path).kind;
		return TRUE;
		}
	
	return FALSE;
	
}

/*****************************************************************************/

Boolean WriteSubPath (GPtr globals,
					 SavedPathElement **path, 
					 int32 *elements)
{
	
	int16 kind;
	Boolean closed;
	
	Fixed finalBwdX;
	Fixed finalBwdY;
	Fixed initialX;
	Fixed initialY;
	Fixed prevX;
	Fixed prevY;
	Fixed fwdX;
	Fixed fwdY;
	Fixed bwdX;
	Fixed bwdY;
	Fixed curX;
	Fixed curY;
	Boolean finalCusp;
	
	int32 length;
	
	int16 cuspKind;
	int16 smoothKind;

	char z = 0;
	
	SkipUnidentifiedElements (path, elements);
	
	if (*path == NULL || *elements == 0)
		return TRUE;
		
	/* Process the subpath header. */
		
	kind = (**path).kind;
	length = ((**path).value [0]) >> 16;
	
	if ((kind != kOpenSubpathLength && kind != kClosedSubpathLength) ||
		length <= 0)
		{
		DisplayStopAlert (globals, kAIInvalidPath);
		return FALSE;
		}
		
	closed = (kind == kClosedSubpathLength);
	
	if (closed)
		{
		cuspKind = kCuspClosedKnot;
		smoothKind = kSmoothClosedKnot;
		}
	else
		{
		cuspKind = kCuspOpenKnot;
		smoothKind = kSmoothOpenKnot;
		}
		
	/* Process the first knot. */
	
	if (!GetNextElement (path, elements, &kind) ||
		(kind != cuspKind && kind != smoothKind))
		{
		DisplayStopAlert (globals, kAIInvalidPath);
		return FALSE;
		}
		
	if (!ConvertSavedPoint (globals,
							(**path).value [3],
							(**path).value [2],
							&initialX,
							&initialY) ||
		!ConvertSavedPoint (globals,
							(**path).value [1],
							(**path).value [0],
							&finalBwdX,
							&finalBwdY) ||
		!ConvertSavedPoint (globals,
							(**path).value [5],
							(**path).value [4],
							&fwdX,
							&fwdY))
		return FALSE;


	finalCusp = (kind == cuspKind);
		
	if (!WriteMoveTo (globals, initialX, initialY))
		return FALSE;
		
	prevX = initialX;
	prevY = initialY;
		
	while (--length > 0)
		{
		
		if (!GetNextElement (path, elements, &kind) ||
			(kind != cuspKind && kind != smoothKind))
			{
			DisplayStopAlert (globals, kAIInvalidPath);
			return FALSE;
			}
			
		if (!ConvertSavedPoint (globals,
								(**path).value [1],
								(**path).value [0],
								&bwdX,
								&bwdY) ||
			!ConvertSavedPoint (globals,
								(**path).value [3],
								(**path).value [2],
								&curX,
								&curY) ||
			!WriteSegment (globals,
						   prevX,
						   prevY,
						   fwdX,
						   fwdY,
						   bwdX,
						   bwdY,
						   curX,
						   curY,
						   (Boolean)(kind == cuspKind)) ||
			!ConvertSavedPoint (globals,
								(**path).value [5],
								(**path).value [4],
								&fwdX,
								&fwdY))
			return FALSE;
			
		prevX = curX;
		prevY = curY;
		
		}
		
	if (closed && !WriteSegment (globals,
								 prevX,
								 prevY,
								 fwdX,
								 fwdY,
								 finalBwdX,
								 finalBwdY,
								 initialX,
								 initialY,
								 finalCusp))
		return FALSE;
		
	return WriteNewPath (globals, closed);
	
}
							 
/*****************************************************************************/

Boolean WritePath (GPtr globals, Handle savedPath)
{
	
	int32 elements = PIGetHandleSize(savedPath) / kSizeSavedPath;
	SavedPathElement *path;
	
	Boolean notdone = TRUE;
	
	if ( PIGetHandleSize(savedPath) < (int32)( elements * kSizeSavedPath ) )
	{
		DisplayStopAlert (globals, kAIInvalidPath);
		return FALSE;
	}
		
	PILockHandle (savedPath, FALSE);
	
	path = (SavedPathElement *) *savedPath;

	while (notdone && path != NULL && elements > 0)
		{
		notdone = WriteSubPath (globals, &path, &elements);
		AdvanceOneElement (&path, &elements);
		}
		
	PIUnlockHandle (savedPath);
		
	return notdone;
	
}


/*****************************************************************************/

unsigned16 SwapWord( char *data )

{
	unsigned16 tmp = 0;
	short loop;

	// to swap the bytes, do #if MSWindows for (loop = 1; loop >= 0; loop--)
	// don't need to, though, because this does the right thing
	for (loop = 0; loop < 2; loop++)
	{
		tmp <<= 8;
		tmp |= (unsigned8)data[loop];
	}
	return tmp;
}



/*****************************************************************************/

unsigned32 SwapLong( char *data )
{
	unsigned32 tmp = 0;
	short loop;

	// to swap the bytes, do #if MSWindows for (loop = 3; loop >= 0; loop--)
	// don't need to, though, because, for some reason, this works
	for (loop = 0; loop < 4; loop++)
	{
		tmp <<= 8;
		tmp |= (unsigned8)data[loop];
	}
	return tmp;
}

/*****************************************************************************/

void FixupOne( char *data, SavedPathElement **path )
{
	short x = 0;

	(**path).kind = SwapWord( &data[0] );

	for (x = 0; x < 6; x++)
		(**path).value[x] = SwapLong( &data[2 + (x * 4)] );
}

/*****************************************************************************/

Handle FixupPath(GPtr globals, Handle path, int howMany )

{
	short x = 0;

	Handle h = PINewHandle(howMany * kSizeSavedPath);

	if (h != NULL)
	{ // got our new handle.  Go to town with it.

		char *data;
		SavedPathElement *newpath;

		data = *path;
		newpath = (SavedPathElement *) *h;
		
		while (howMany)
		{
			FixupOne( data, &newpath );
			
			howMany--;
			data += kHardPathLength;
			++newpath;
		}
		PIDisposeHandle( path ); // done with this
	}
	return h;
}


/*****************************************************************************/

Handle GetIndexedPath (GPtr globals, int32 index)
{
	
	Handle pathData;
	
	if (gResult != noErr)
		return NULL;
	
	gResult = GetComplex(propPathContents, index, &pathData);
	
	if (gResult == noErr)
		return FixupPath(globals, pathData, 

				(PIGetHandleSize( pathData ) / kHardPathLength) );
	else
		return NULL;

}

/*****************************************************************************/

Boolean WriteIndexedPath (GPtr globals, int32 index)
{
	
	Handle pathData = GetIndexedPath (globals, index);
	
	if (pathData != NULL)
		{
		Boolean result = WritePath (globals, pathData);
		/* When we get a handle back from the property routines, it is our
		   responsibility to dispose of it. */
		PIDisposeHandle (pathData);
		return result;
		}
	return FALSE;
}

/*****************************************************************************/

Boolean GetPathCount (GPtr globals, int32 *count)
{
	
	if (gResult != noErr)
		return FALSE;
	
	gResult = GetSimple(propNumberOfPaths, count);

	if (gResult == noErr)
		return TRUE;
		
	*count = 0;
	
	return FALSE;

}

/*****************************************************************************/

int16 GetPathIndex (GPtr globals, OSType pathProperty)
{
	
	int32 	simple;
	OSErr	result = noErr;
	
	if (gResult != noErr) return -1;

	result = GetSimple(pathProperty, &simple);

	if (result != noErr)
		return -1;
		
	return (int16) simple;
	
	}

/*****************************************************************************/

Boolean GetPathName (GPtr globals, int32 index, Str255 s)
{
	
	Handle complex;

	gResult = GetComplex(propPathName, index, &complex);
	
	if (gResult != noErr)
		return FALSE;
		
	if (complex == NULL)
		{
		gResult = errPlugInHostInsufficient;
		return FALSE;
		}
		
	PIHandle2PString(complex, s);

	// We were given this handle.  It's our job to dispose it.
	PIDisposeHandle (complex);
	
	return TRUE;
	
}

/*****************************************************************************/

Boolean WriteAllPaths (GPtr globals)
	{
	
	int32 pathCount;
	int32 index;
	
	if (!GetPathCount (globals, &pathCount))
		return FALSE;
		
	for (index = 0; index < pathCount; ++index)
		{
		
		UpdateProgress (index, pathCount);
		
		if (TestAbort ())
			{
			gResult = userCanceledErr; // exit gracefully
			return FALSE;
			}
			
		if (!WriteIndexedPath (globals, index))
			return FALSE;
			
		}
			
	return TRUE;
	
	}

/*****************************************************************************/

Boolean WriteFileTrailer (GPtr globals)
{
	return WriteResString (globals, kPSTrailer);
}

/*****************************************************************************/



int32 DoFloor (double x)

{

	int32 retn = 0;



	if (x < 0)

		retn = (int32)(x-1); // floor -3.8 -> -4, etc.

	else

		retn = (int32)(x); // floor 2.9 -> 2, etc.



	return retn;

}



/*****************************************************************************/

Boolean ConvertToPoints (GPtr globals,
						int16 pixels,
						Fixed resolution,
						Fixed *value)
	{
	
	double xValue;
	
	if (pixels * 72.0 / (resolution / 65536.0) > 32767.0)
		{
		DisplayStopAlert (globals, kAITooBig);
		return FALSE;
		}
		
	xValue = pixels * (72.0 / (resolution / 65536.0));
		
	*value = (Fixed) (int32) DoFloor (xValue * 65536.0 + 0.5);
	
	return TRUE;
	}

/*****************************************************************************/

Boolean CalculateBoundingBox (GPtr globals)
	{
	
	return ConvertToPoints (globals,
							gStuff->imageSize.v,
							gStuff->imageVRes,
							&gDocHeight) &&
		   ConvertToPoints (globals,
							gStuff->imageSize.h,
							gStuff->imageHRes,
							&gDocWidth);
	
	}

/*****************************************************************************/

Boolean WriteBody (GPtr globals)
	{
	
	if (gPopUpValue == iDocBounds)
		return TRUE;
	else if (gPopUpValue == iAllPaths)
		return WriteAllPaths (globals);
	else if (gPopUpValue >= iPathStart)
		return WriteIndexedPath (globals, gPopUpValue - iPathStart);
	else
		return FALSE;
	
	}
	
/*****************************************************************************/

/* Requests pointer to the first part of the image to be filtered. */

void DoStart (GPtr globals)
{
	
	PISetRect (&gStuff->theRect, 0, 0, 0, 0);
	gStuff->hiPlane = 0;
	gStuff->loPlane = 0;
	
	if (!WarnPropertyAvailable())
	{ 
		gResult = errPlugInHostInsufficient; // exit w/error
		return;
	}
		
	ValidateParameters (globals);
	/* if stuff hasn't been initialized that we need, do it,
		then go check if we've got scripting commands to
		override our settings */

	gQueryForParameters = ReadScriptParams (globals);
	
	(void) (CalculateBoundingBox (globals) &&
		    DoUI (globals) &&
			CreateExportFile (globals) &&
		    WriteFileHeader (globals) &&
		    WriteBody (globals) &&
		    WriteFileTrailer (globals));

	

	CloseExportFile (globals);
	// always do this	
}

/*****************************************************************************/

// Filters the area and requests the next area. We never call this since we
// do all our work in DoStart.  But just in case, we'll reset everything.

void DoContinue (GPtr globals)
{
	PISetRect (&gStuff->theRect, 0, 0, 0, 0);
	gResult = userCanceledErr; // exit gracefully
}

/*****************************************************************************/

/* This routine will always be called if DoStart does not return an error
   (even if DoContinue returns an error or the user aborts the operation).
   This allows the module to perform any needed cleanup.  None is required
   in this example. */

void DoFinish (GPtr globals)
{
	WriteScriptParams (globals);
}

//-------------------------------------------------------------------------------

// end PathsToPostScript.cpp
