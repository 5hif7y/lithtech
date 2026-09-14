// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2003 Adobe Systems Incorporated
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
//		Propetizer.cpp
//
//	Description:
//		This file contains the routines and source
//		for the Filter module Propetizer, a module that
//		examines and displays Photoshop properties.
//
//	Use:
//		This is a basic module to exemplify all the typical
//		functions a filter module will do with a special
//		emphasis on assessing properties.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#include "PhotoshopSDK.h"
#include "Propetizer.h"
#include "PropertyUtils.h"

void FileToHandle(const string & fileName, Handle & h);
void HandleToFile(const string & fileName, const Handle h);
void FileToString(const string & fileName, string & inString);
void StringToFile(const string & fileName, const string & outString);


SPBasicSuite * sSPBasic = NULL;
SPPluginRef gPluginRef = NULL;

PropetizerPlugin::PropetizerPlugin()
	: FilterPlugin(), fBigNudgeV(10.0), fBigNudgeH(10.0), fRulerOriginV(0.0), 
	  fRulerOriginH(0.0), fGridMajor(0.0), fGridMinor(0.0), 
	  fWatchSuspension(false), fCopyright(false), fURL(""), fXMP(""),
	  fWatermark(false), fSliceID(0), fSliceIndex(0), fBits(0)
{
}



//-------------------------------------------------------------------------------
//
//	PropetizerPlugin::~PropetizerPlugin
//
//	Get rid of anything we made during this plug in life cycle.
//
//-------------------------------------------------------------------------------
PropetizerPlugin::~PropetizerPlugin()
{
}



//-------------------------------------------------------------------------------
//
//	PropetizerPlugin::Start
//
//	This is where all the action is.
//
//-------------------------------------------------------------------------------
void PropetizerPlugin::Start(void)
{
	GetProperties();

	bool playInProgress;
	bool popDialog;

	if (!PIGetPlayInProgress(playInProgress))
	{
		if (playInProgress)
		{
			popDialog = ReadScriptParameters();
		}
		else
		{
			popDialog = true;
		}
	}
	else
	{
		popDialog = true;
	}
	
	if (popDialog)
	{
		if (DoUI() == 1) // the OK button
		{
			error = SetProperties();
		}
	}
	else 
	{
		error = SetProperties();
	}

	if (!error)
	{
		WriteScriptParameters();
	}
}



int16 PropetizerPlugin::GetProperties(void)
{
	(void) PIGetBigNudge(fBigNudgeH, fBigNudgeV);
	(void) PIGetRulerOrigin(fRulerOriginH, fRulerOriginV);
	(void) PIGetGrid(fGridMajor, fGridMinor);
	(void) PIGetWatchSuspension(fWatchSuspension);
	(void) PIGetCopyright(fCopyright);
	(void) PIGetURL(fURL);
	(void) PIGetWatermark(fWatermark);
	(void) PIGetSelectedSliceID(fSliceID);
	(void) PIGetSelectedSliceIndex(fSliceIndex);
	(void) PIGetXMP(fXMP);

	return 0;
}

int16 PropetizerPlugin::SetProperties(void)
{
	double d1, d2;
	int32 i;
	bool b;
	string s;

	if (!PIGetBigNudge(d1, d2))
	{
		if (d1 != fBigNudgeH && d2 != fBigNudgeV)
		{
			if (!error)
				error = PISetBigNudge(fBigNudgeH, fBigNudgeV);
			if (!error)
			{
				fBits.set(iNudgeH) = true;
				fBits.set(iNudgeV) = true;
			}
		}
		else if (d1 != fBigNudgeH)
		{
			if (!error)
				error = PISetBigNudge(fBigNudgeH, d2);
			if (!error)
				fBits.set(iNudgeH) = true;
		}
		else if (d2 != fBigNudgeV)
		{
			if (!error)
				error = PISetBigNudge(d1, fBigNudgeV);
			if (!error)
				fBits.set(iNudgeV) = true;
		}
	}

	if (!PIGetRulerOrigin(d1, d2))
	{
		if (d1 != fRulerOriginH && d2 != fRulerOriginV)
		{
			if (!error)
				error = PISetRulerOrigin(fRulerOriginH, fRulerOriginV);
			if (!error)
			{
				fBits.set(iHorizontal) = true;
				fBits.set(iVertical) = true;
			}
		}
		else if (d1 != fRulerOriginH)
		{
			if (!error)
				error = PISetRulerOrigin(fRulerOriginH, d2);
			if (!error)
				fBits.set(iHorizontal) = true;
		}
		else if (d2 != fRulerOriginV)
		{
			if (!error)
				error = PISetRulerOrigin(d1, fRulerOriginV);
			if (!error)
				fBits.set(iVertical) = true;
		}
	}

	if (!PIGetGrid(d1, d2))
	{
		if (d1 != fGridMajor && d2 != fGridMinor)
		{
			if (!error)
				error = PISetGrid(fGridMajor, fGridMinor);
			if (!error)
			{
				fBits.set(iGridMajor) = true;
				fBits.set(iGridMinor) = true;
			}
		}
		else if (d1 != fGridMajor)
		{
			if (!error)
				error = PISetGrid(fGridMajor, d2);
			if (!error)
				fBits.set(iGridMajor) = true;
		}
		else if (d2 != fGridMinor)
		{
			if (!error)
				error = PISetGrid(d1, fGridMinor);
			if (!error)
				fBits.set(iGridMinor) = true;
		}
	}

	if (!PIGetWatchSuspension(i))
		if (i != fWatchSuspension)
		{
			if (!error)
				error = PISetWatchSuspension(fWatchSuspension);
			if (!error)
				fBits.set(iWatch) = true;
		}

	if (!PIGetCopyright(b))
		if (b != fCopyright && fCopyright)
		{
			if (!error)
				error = PISetCopyright(fCopyright);
			if (!error)
				fBits.set(iCopyright) = true;
		}

	if (!PIGetURL(s))
		if (s.compare(fURL) != 0)
		{
			if (!error)
				error = PISetURL(fURL);
			if (!error)
				fBits.set(iURL) = true;
		}

	if (!PIGetWatermark(b))
		if (b != fWatermark && fWatermark)
		{
			if (!error)
				error = PISetWatermark();
			if (!error)
				fBits.set(iWater) = true;
		}

	if (!PIGetSelectedSliceID(i))
		if (i != fSliceID)
		{
			if (!error)
				error = PISetSelectedSliceID(fSliceID);
			if (!error)
				fBits.set(iSliceID) = true;
		}

	if (!PIGetSelectedSliceIndex(i))
		if (i != fSliceIndex)
		{
			if (!error)
				error = PISetSelectedSliceIndex(fSliceIndex);
			if (!error)
				fBits.set(iSliceIndex) = true;
		}

	if (!PIGetXMP(s))
		if (s.compare(fXMP) != 0)
		{
			if (!error)
				error = PISetXMP(fXMP);
			if (!error)
				fBits.set(iXMP) = true;
		}

	return error;
}

//-------------------------------------------------------------------------------
//
//	Plugin * AllocatePlugin(void)
//
//	This is how our plug in gets created. Using the Plugin base class the
//	StandardMain.cpp code creates our plug in with this call.
//
//-------------------------------------------------------------------------------
Plugin * AllocatePlugin(void)
{
	return new PropetizerPlugin;
}

void FileToHandle(const string & fileName, Handle & h)
{
	h = NULL;
	vector<char> data;
	ifstream inFile(fileName.c_str(), ios::in|ios::binary);
	do
	{
		char c;
		inFile.read(&c, 1);
		if (inFile.gcount())
		{
			data.push_back(c);
		}
	} while (inFile.gcount());

	int32 s = data.size();
	if (s)
	{
		h = sPSHandle->New(s);
		if (h != NULL)
		{
			Ptr p = NULL;
			sPSHandle->SetLock(h, true, &p, NULL);
			if (p != NULL)
			{
				for(int32 a = 0; a < s; a++, p++)
					*p = data.at(a);
				sPSHandle->SetLock(h, false, &p, NULL);
			}
			else
			{
				sPSHandle->Dispose(h);
				h = NULL;
			}
		}
	}
}

void HandleToFile(const string & fileName, const Handle h)
{
	ofstream outFile(fileName.c_str(), ios::out|ios::binary);
	
	int32 s = sPSHandle->GetSize(h);
	
	if (s > 0)
	{
		Ptr p = NULL;
		sPSHandle->SetLock(h, true, &p, NULL);
		if (p != NULL)
		{
			Ptr o = p;
			for (int32 a = 0; a < s; a++, o++)
				outFile << *o;
			sPSHandle->SetLock(h, false, &p, NULL);
		}
	}
}

void FileToString(const string & fileName, string & inString)
{
	ifstream inFile(fileName.c_str(), ios::in);
	do
	{
		char c;
		inFile.read(&c, 1);
		if (inFile.gcount())
		{
			inString += c;
		}
	} while (inFile.gcount());
}

void StringToFile(const string & fileName, const string & outString)
{
	ofstream outFile(fileName.c_str(), ios::out);

	for (uint32 i =0; i < outString.length(); i++)
		outFile << outString[i];
}
// end Propetizer.cpp

