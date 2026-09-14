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
//		Idle.c
//
//
//	Description:
//		This file contains the source and functions for the
//		Selection module Idle, a very simple example of a module
//		that just idles and sends update events.
//
//	Use:
//		This import module is good as an example of a module
//		that forces the background application to redraw
//		its windows.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "PIDefines.h"
#include "Idle.h"

//-------------------------------------------------------------------------------
//	Globals -- Define global variables for plug-in scope.
//-------------------------------------------------------------------------------

SPBasicSuite * sSPBasic = NULL;

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
//		PISelectionParams *selectionParamBlock		Host provides pointer to parameter
//													block containing pertinent data
//													and callbacks from the host.
//													See PISelection.h.
//
//	Outputs:
//		PISelectionParams *selectionParamBlock		Host provides pointer to parameter
//													block containing pertinent data
//													and callbacks from the host.
//													See PISelection.h.
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
						             void *selectionParamBlock,
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

	if (selector == selectionSelectorAbout)
	{
		sSPBasic = ((AboutRecordPtr)selectionParamBlock)->sSPBasic;
		DoAbout((AboutRecordPtr)selectionParamBlock);
	}
	else
	{ // do the rest of the process as normal:

		sSPBasic = ((PISelectionParams*)selectionParamBlock)->sSPBasic;

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
									 (uint32)selectionParamBlock,
									 ((PISelectionParams*)selectionParamBlock)->handleProcs,
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

		DoExecute(globals);
					
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
	
	// Initialize global variables:
	gQueryForParameters = TRUE;
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
//		gUseProgress		Default: true.
//
//		gSendUpdates		Default: true.
//
//		gIdleAmount			Default: 2.0 seconds.
//
//-------------------------------------------------------------------------------

void ValidateParameters (GPtr globals)
{
	gUseProgress = true;
	gSendUpdates = true;
	gIdleAmount = 2.0;
	
} // end ValidateParameters

//-------------------------------------------------------------------------------
//
//	DoExecute
//
//	Main routine.  In this case, pop the UI then call DoIdle.
//
//	Inputs:
//		GPtr globals		Pointer to global structure.
//
//	Outputs:
//		gResult				Returns noErr if completed without error.
//							Most probable error return, if any, is
//							not enough memory err.
//
//-------------------------------------------------------------------------------

void DoExecute (GPtr globals)
{	
	// If stuff hasn't been initialized that we ened, do it, then
	// go check if we've got scripting commands to override our
	// default settings:
	ValidateParameters (globals);
	gQueryForParameters = ReadScriptParams (globals);

	if (gQueryForParameters)
	{ // Go ahead and pop the UI:
	
		gQueryForParameters = FALSE;
		if (!DoUI (globals))
			gResult = userCanceledErr;
			
	} // end gQueryForParameters
	
	if (gResult == noErr)
	{
		DoIdle (globals);
	
		// Now copy the existing selection over so that we don't blow
		// it away:	
		CopyExistingSelection (globals);
		
		WriteScriptParams (globals);
	}
	
} // end DoExecute

//-------------------------------------------------------------------------------
//
//	DoIdle
//
//	This is the main idling routine.  Its job is to idle for a certain amount
//	of seconds.  During those seconds it can try to send events to force
//	the application to update its open windows; display the progress
//	indicator for long idles, or a combination of either.
//
//	Inputs:
//		GPtr globals			Pointer to global structure
//
//		Boolean gUseProgress	TRUE = Display progress indicator.
//								FALSE = Suppress display.
//
//		Boolean	gSendUpdates	TRUE = Send update events to application.
//								FALSE = Send no events, just idle.
//
//		double gIdleAmount		Seconds and parts of seconds to idle.
//
//	Outputs:
//		gResult					Returns noErr if completed without error.
//
//		Idles for the set amount of seconds in gIdleAmount, displaying
//		a progress indicator (if gUseProgress) and sending updates
//		(if gSendUpdates).
//
//-------------------------------------------------------------------------------

void DoIdle (GPtr globals)
{
	const uint32 ticksStart = IdleGetTicks(); // see UIMac or UIWin
	const double ticksRate = IdleGetTicksRate();
	const double progressRate = IdleGetProgressRate();
	const uint32 ticksEnd = (uint32)((double)ticksStart+(gIdleAmount*ticksRate));

	uint32 currTicks = ticksStart+1;
	
	const int32 total = (int32)((ticksEnd - ticksStart) / progressRate); 
	int32 done = 0;
	
	while ((currTicks < ticksEnd) && (gResult == noErr))
	{
		if (gUseProgress)
		{
			done = (int32)((currTicks - ticksStart) / progressRate);
			UpdateProgress(done, total);
		}
	
		if (TestAbort())
		{
			gResult = noErr;
			currTicks = ticksEnd;
		}
		else
		{
			currTicks = IdleGetTicks();
		}

		if (gSendUpdates)
			IdleSendUpdate (globals);
		
	} // end while
	
} // end DoIdle

//-------------------------------------------------------------------------------
//
//	CopyExistingSelection
//
//	This copies the existing selection into the newSelection structure so that
//	any existing selections won't be blown away by this NOOP plug-in.
//
//	Inputs:
//		GPtr globals			Pointer to global structure
//
//	Outputs:
//		gResult					Returns noErr if completed without error.
//
//		Copies existing selection to newSelection.
//
//
//-------------------------------------------------------------------------------

void CopyExistingSelection (GPtr globals)
{
	// Check to see if Channel Ports are even available:
	if (ChannelPortAvailable(NULL))
	{
		// First, grab a pointer to the general document info:
		const ReadImageDocumentDesc *doc = gStuff->documentInfo;
		
		// Now grab a pointer to the actual selection:
		const ReadChannelDesc *currSel = doc->selection;				
		
		if (currSel != NULL)
		{ // Yes, we have a selection.
		
			// Get the port to the selection that we'll return.
			WriteChannelDesc *newSel = gStuff->newSelection;

			// Make a local copy of the current selection's bounds:
			const VRect limit = currSel->bounds;

			// Now we need an id for a buffer and its size.  We'll
			// pick something small and manageable and iterate
			// through the image by that size (always good to
			// go in chunks, not rows or columns):
			const uint16 kBlockRows = 64;
			const uint16 kBlockCols = 64;
			const uint16 kBufferSize = kBlockRows * kBlockCols;
			BufferID pixelsBuffer = 0;
			
			// Now allocate the buffer:
			gResult = AllocateBuffer(kBufferSize, &pixelsBuffer);
			if (gResult == noErr)
			{ // Allocation went fine.
			
				// Create a pointer to the pixels that are going
				// to go in the buffer by locking the buffer down:
				Ptr pixels = LockBuffer (pixelsBuffer, false);
				
				if (pixels != NULL)
				{ // Buffer lock was successful.
				
					// Need a general descriptor for the pixels
					// called a PixelMemoryDesc.  See PIGeneral.h:
					PixelMemoryDesc pixeldata;

					// And we need our rows and cols local iterators:
					int32 row, col;
					
					// Now fill out the general pixel descriptor:
					pixeldata.data = pixels;
					pixeldata.rowBits = kBlockCols * currSel->depth;
					pixeldata.colBits = currSel->depth;
					pixeldata.bitOffset = 0;
					pixeldata.depth = currSel->depth;
					
					// Now iterate through, going a block at a time:
					for (row = limit.top; row < limit.bottom; row += kBlockRows)
					{
						for (col = limit.left; col < limit.right; col += kBlockCols)
						{
							PSScaling scaling;
							VRect wrote;
							VRect area;
					
							if (TestAbort())
							{
								gResult = userCanceledErr;
								return;
							}
							
							area.top = row;
							area.bottom = row + kBlockRows;
							if (limit.bottom < area.bottom)
								area.bottom = limit.bottom;
								
							area.left = col;
							area.right = col + kBlockCols;
							if (limit.right < area.right)
								area.right = limit.right;
								
							scaling.sourceRect = scaling.destinationRect = area;
							
							gResult = ReadPixels(currSel->port,
												   &scaling,
												   &area,
												   &pixeldata,
												   &wrote);
							
							if (gResult != noErr)
								return;
								
							gResult = WritePixels(newSel->port,
												&wrote,
												&pixeldata);
					
						} // col
					
					} // row
					
					UnlockBuffer (pixelsBuffer);
					FreeBuffer (pixelsBuffer);
					pixelsBuffer = 0;

					// Now that we've copied all the selection to the new
					// selection's port, fill out the rest of the info:								
					newSel->bounds = currSel->bounds;
					newSel->depth = currSel->depth;
					newSel->next = NULL;
					newSel->tileSize = currSel->tileSize;
					newSel->tileOrigin = currSel->tileOrigin;
					newSel->contextInfo = currSel->contextInfo;
					newSel->channelType = currSel->channelType;
					newSel->name = currSel->name;
					
				} // pixels
			
			} // gResult   

		} // currSel
		
	} // channelPort
		
} // end CopyExistingSelection

//-------------------------------------------------------------------------------

// end Idle.c
