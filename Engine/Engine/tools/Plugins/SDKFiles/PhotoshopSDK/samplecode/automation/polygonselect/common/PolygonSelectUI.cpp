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
//		PolygonSelectUI.cpp
//
//	Copyright 1999-1999, Adobe Systems Incorporated.
//	All Rights Reserved.
//
//	Description:
//		This file contains the source code and routines for the
//		Actions module PolygonSelect.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "PolygonSelect.h"

using namespace std;

class PolygonSelectDialog : public PIDialog {
	PIText amountItem;
	PIText sidesItem;
	PIText angleItem;

	long amount;
	long sides;
	double angle;

	virtual void Init(void);
	virtual void Notify(int32 item);
public:
	PolygonSelectDialog(long a, long s, double ang) 
		                : PIDialog(), amountItem(), sidesItem(), angleItem(),
	                      amount(a), sides(s), angle(ang) {}
	~PolygonSelectDialog() {}

	double GetRotationAngle(void);
	long GetRadius(void);
	long GetNumberOfSides(void);
};

//-------------------------------------------------------------------------------
//	Definitions -- Dialog items
//-------------------------------------------------------------------------------
enum
	{
	kDNoUI = -1,		// Error.
	kDOk_button = 1,	// Must be one.
	kDCancel_button,	// Must be two.
	kDRadius_staticText,
	kDRadius_frame,
	kDAmount_staticText,
	kDAmount_editText,
	kDNumberSides_staticText,
	kDNumberSides_editText,
	kDRotation_staticText,
	kDRotation_frame,
	kDAngle_staticText,
	kDAngle_editText
	};
	
//-------------------------------------------------------------------------------
//
//	DoUI
//
//	Display the user interface.
//
//-------------------------------------------------------------------------------
SPErr DoUI ()
{
	SPErr error = noErr;
	
	int item = kDNoUI;	// Error value.
	
	PolygonSelectDialog dialog(gRadius, gNumberSides, gRotationAngle);
	item = dialog.Modal(gPlugInRef, "PolygonSelection", kDialogID);
	
	if (item != kDOk_button)
	{
		error = 'STOP'; //'STOP' = kUserCancel;
	}
	else
	{
		gRadius = dialog.GetRadius();
		gNumberSides = dialog.GetNumberOfSides();
		gRotationAngle = dialog.GetRotationAngle();
	}

	return error;
	
}

//-------------------------------------------------------------------------------
//
//	PolygonSelectDialog::Init
//
//	Handle initialization of the dialog.
//
//-------------------------------------------------------------------------------
void PolygonSelectDialog::Init(void)
{
	//Populate Radius and NumberSides field with initialized values
	//or previous user values.
	stringstream temp;

	amountItem.SetItem(PIGetDialogItem(GetDialog(), kDAmount_editText));
	temp << amount;
	amountItem.SetText(temp.str().c_str());

	temp.str("");

	sidesItem.SetItem(PIGetDialogItem(GetDialog(), kDNumberSides_editText));
	temp << sides;
	sidesItem.SetText(temp.str().c_str());

	temp.str("");

	angleItem.SetItem(PIGetDialogItem(GetDialog(), kDAngle_editText));
	temp << angle;
	angleItem.SetText(temp.str().c_str());
}

//-------------------------------------------------------------------------------
//
//	PolygonSelectDialog::Notify
//
//	If the user hits ok then store the dialog information
//
//-------------------------------------------------------------------------------
void PolygonSelectDialog::Notify(int32 item)
{
	char * tempText = NULL;

	if (item == kDOk_button)
	{
		tempText = amountItem.GetText();
		if (tempText != NULL)
		{
			amount = atoi(tempText);
			delete [] tempText;
			tempText = NULL;
		}

		tempText = sidesItem.GetText();
		if (tempText != NULL)
		{
			sides = atoi(tempText);
			delete [] tempText;
			tempText = NULL;
		}

		tempText = angleItem.GetText();
		if (tempText != NULL)
		{
			angle = atof(tempText);
			delete [] tempText;
			tempText = NULL;
		}
	}
}


//-------------------------------------------------------------------------------
//
//	PolygonSelectDialog::GetRadius
//
//	Return the radius value from the dialog
//
//-------------------------------------------------------------------------------
long PolygonSelectDialog::GetRadius(void)
{
	return amount;
}

//-------------------------------------------------------------------------------
//
//	PolygonSelectDialog::GetNumberOfSides
//
//	Return the sides value from the dialog
//
//-------------------------------------------------------------------------------
long PolygonSelectDialog::GetNumberOfSides(void)
{
	return sides;
}

//-------------------------------------------------------------------------------
//
//	PolygonSelectDialog::GetRotationAngle
//
//	Return the angle value from the dialog
//
//-------------------------------------------------------------------------------
double PolygonSelectDialog::GetRotationAngle(void)
{
	return angle;
}
// end PolygonSelectUI.cpp
