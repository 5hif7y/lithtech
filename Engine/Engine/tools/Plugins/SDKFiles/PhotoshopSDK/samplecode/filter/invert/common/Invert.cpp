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
#include "PhotoshopSDK.h"

FilterRecord * gFilterRecord;
int32 * gData;
int16 * gResult;
SPBasicSuite * sSPBasic = NULL;

void DoAbout(void);
void DoParameters(void);
void DoPrepare(void);
void DoStart(void);
void DoContinue(void);
void DoFinish(void);

void InvertTile(void * tileData, VRect & tileRect, int32 rowBytes);

DLLExport SPAPI void PluginMain(const int16 selector,
								void * filterRecord,
								int32 * data,
								int16 * result)
{
	gData = data;
	gResult = result;
	
	if (selector == filterSelectorAbout)
	{
		sSPBasic = ((AboutRecordPtr)filterRecord)->sSPBasic;
	} 
	else 
	{
		gFilterRecord = (FilterRecordPtr)filterRecord;
		sSPBasic = gFilterRecord->sSPBasic;

		if (gFilterRecord->bigDocumentData != NULL)
			gFilterRecord->bigDocumentData->PluginUsing32BitCoordinates = true;
	}

	switch (selector)
	{
		case filterSelectorAbout:
			DoAbout();
			break;
		case filterSelectorParameters:
			DoParameters();
			break;
		case filterSelectorPrepare:
			DoPrepare();
			break;
		case filterSelectorStart:
			DoStart();
			break;
		case filterSelectorContinue:
			DoContinue();
			break;
		case filterSelectorFinish:
			DoFinish();
			break;
		default:
			*gResult = filterBadParameters;
			break;
	}
}

void DoAbout(void)
{
	AboutRecord * aboutRecord = reinterpret_cast<AboutRecord *>(gFilterRecord);
	DoAbout(static_cast<SPPluginRef>(aboutRecord->plugInRef), AboutID);
}

void DoParameters(void)
{
}

void DoPrepare(void)
{
	gFilterRecord->bufferSpace = 0;
	gFilterRecord->maxSpace = 0;
}

void DoStart(void)
{
	int16 tileHeight = gFilterRecord->outTileHeight;
	int16 tileWidth = gFilterRecord->outTileWidth;

	if (tileWidth == 0 || tileHeight == 0 || gFilterRecord->advanceState == NULL)
	{
		*gResult = filterBadParameters;
		return;
	}

	VRect outRect = GetOutRect();
	VRect filterRect = GetFilterRect();

	int32 imageVert = filterRect.bottom - filterRect.top;
	int32 imageHor = filterRect.right - filterRect.left;

	uint32 tilesVert = (tileHeight - 1 + imageVert) / tileHeight;
	uint32 tilesHoriz = (tileWidth - 1 + imageHor) / tileWidth;

	int16 layerPlanes = 0;
	if (gFilterRecord->filterCase > 2)
		layerPlanes = gFilterRecord->outLayerPlanes;
	else
		layerPlanes = gFilterRecord->planes;

	int32 progress_total = layerPlanes * tilesVert;
	int32 progress_complete = 0;

	for(int16 planeCount = 0; planeCount < layerPlanes; planeCount++)
	{
		gFilterRecord->outLoPlane = planeCount;
		gFilterRecord->outHiPlane = planeCount;

		for(uint16 vertTile = 0; vertTile < tilesVert; vertTile++)
		{
			for(uint16 horizTile = 0; horizTile < tilesHoriz; horizTile++)
			{
				outRect.top = vertTile * tileHeight;
				outRect.left = horizTile * tileWidth;
				outRect.bottom = outRect.top + tileHeight;
				outRect.right = outRect.left + tileWidth;

				if (outRect.bottom > imageVert)
					outRect.bottom = imageVert;
				if (outRect.right > imageHor)
					outRect.right = imageHor;

				SetOutRect(outRect);

				*gResult = gFilterRecord->advanceState();
				if (*gResult != kNoErr) return;

				outRect = GetOutRect();
				
				InvertTile(gFilterRecord->outData, 
					       outRect, 
						   gFilterRecord->outRowBytes);
			}

			gFilterRecord->progressProc(++progress_complete, progress_total);

			if (gFilterRecord->abortProc()) 
			{
				*gResult = userCanceledErr;
				goto done;
			}
		}
	}
done:
	outRect.top = 0;
	outRect.left = 0;
	outRect.bottom = 0;
	outRect.right = 0;
	SetOutRect(outRect);
}

void DoContinue(void)
{
	VRect outRect = { 0, 0, 0, 0};
	SetOutRect(outRect);
}

void DoFinish(void)
{
}

void InvertTile(void * tileData, VRect & tileRect, int32 rowBytes)
{
	uint8 * smallPixel = static_cast<uint8 *>(tileData);
	uint16 * largePixel = static_cast<uint16 *>(tileData);

	uint32 rectHeight = tileRect.bottom - tileRect.top;
	uint32 rectWidth = tileRect.right - tileRect.left;

	for(uint32 pixelY = 0; pixelY < rectHeight; pixelY++)
	{
		for(uint32 pixelX = 0; pixelX < rectWidth; pixelX++)
		{
			if (gFilterRecord->depth == 16)
			{
				*largePixel = 32768 - *largePixel;
				largePixel++;
			} 
			else
			{
				*smallPixel = 255 - *smallPixel;
				smallPixel++;
			}
		}
		smallPixel += rowBytes - rectWidth;
		largePixel += rowBytes / 2 - rectWidth;
	}
}
// end Invert.cpp