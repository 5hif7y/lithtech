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
//		Propetizer.h
//
//	Description:
//		This file contains the header prototypes and macros
//		for the Filter module Propetizer, a module that
//		examines and displays grids and guide settings,
//		then makes pseudo-resource history entries when
//		you change them.
//
//	Use:
//		This is a basic module to exemplify all the typical
//		functions a filter module will do with a special
//		emphasis on assessing properties and writing
//		pseudo-resources.
//
//-------------------------------------------------------------------------------

#ifndef __Propetizer_H__			// Has this been defined yet?
#define __Propetizer_H__			// Only include once by predefining it.

#include "pifilter.h"				// Filter Photoshop header file.
#include "FilterPlugin.h"
#include "PIUtilities.h"			// SDK Utility Library.
#include "piproperties.h"			// Properties Photoshop suite.
#include "PropetizerTerminology.h"	// Terminology for this plug-in.

enum {
	iNudgeH = 0,
	iNudgeV,
	iHorizontal,
	iVertical,
	iGridMajor,
	iGridMinor,
	iWatch,
	iCopyright,
	iURL,
	iWater,
	iSliceID,
	iSliceIndex,
	iXMP
};

class PropetizerPlugin : public FilterPlugin {
private:

	double fBigNudgeV;
	double fBigNudgeH;

	double fRulerOriginV;
	double fRulerOriginH;

	double fGridMajor;
	double fGridMinor;

	int32 fWatchSuspension;

	bool fCopyright;

	string fURL;

	bool fWatermark;

	int32 fSliceID;
	int32 fSliceIndex;

	string fXMP;

	bitset<13> fBits;

	virtual void Start(void);

	int32 DoUI(void);
	
	bool ReadScriptParameters(void);
	void WriteScriptParameters(void);

	int16 GetProperties(void);
	int16 SetProperties(void);

public:
	PropetizerPlugin();
	virtual ~PropetizerPlugin();

	void SetBigNudgeH(double in) { fBigNudgeH = in; }
	double GetBigNudgeH(void) { return fBigNudgeH; }
	void SetBigNudgeV(double in) { fBigNudgeV = in; }
	double GetBigNudgeV(void) { return fBigNudgeV; }
	void SetRulerOriginH(double in) { fRulerOriginH = in; }
	double GetRulerOriginH(void) { return fRulerOriginH; }
	void SetRulerOriginV(double in) { fRulerOriginV = in; }
	double GetRulerOriginV(void) { return fRulerOriginV; }
	void SetGridMajor(double in) { fGridMajor = in; }
	double GetGridMajor(void) { return fGridMajor; }
	void SetGridMinor(double in) { fGridMinor = in; }
	double GetGridMinor(void) { return fGridMinor; }
	void SetWatchSuspension(int32 in) { fWatchSuspension = in; }
	int32 GetWatchSuspension(void) { return fWatchSuspension; }
	void SetCopyright(bool in) { fCopyright = in; }
	bool GetCopyright(void) { return fCopyright; }
	void SetURL(string in) { fURL = in; }
	string GetURL(void) { return fURL; }
	void SetWatermark(bool in) { fWatermark = in; }
	bool GetWatermark(void) { return fWatermark; }
	void SetSliceID(int32 in) { fSliceID = in; }
	int32 GetSliceID(void) { return fSliceID; }
	void SetSliceIndex(int32 in) { fSliceIndex = in; }
	int32 GetSliceIndex(void) { return fSliceIndex; }
	void SetXMP(string inXMP) { fXMP = inXMP; }
	string GetXMP(void) { return fXMP; }
};


extern const string sEmptyString;


//-------------------------------------------------------------------------------

#endif // __Propetizer_H__
