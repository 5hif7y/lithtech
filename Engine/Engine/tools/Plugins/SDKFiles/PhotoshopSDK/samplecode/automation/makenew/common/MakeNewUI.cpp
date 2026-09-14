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
//		MakeNewUI.cpp
//
//	Description:
//		This file contains the source code and routines for the
//		Actions module MakeNew, an example of a module
//		that uses the Actions suite to make a new document.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "MakeNew.h"

using namespace std;

// Use the _very_ simple PIDialog base class to display a dialog
class MakeNewDialog : public PIDialog {
	PIText widthItem;
	PIText heightItem;
	PIText resolutionItem;
	PIRadioGroup modeGroup;
	PIRadioGroup fillGroup;

	int32 width;
	int32 height;
	int32 resolution;
	DescriptorEnumID fill;
	DescriptorClassID mode;

	virtual void Init(void);
	virtual void Notify(int32 item);
public:
	MakeNewDialog(int32 w, int32 h, int32 r, DescriptorEnumID f, DescriptorClassID m) 
		: PIDialog(), widthItem(), heightItem(), resolutionItem(), modeGroup(), fillGroup(),
		  width(w), height(h), resolution(r), fill(f), mode(m) {}
	~MakeNewDialog() {}

	int32 GetWidth(void);
	int32 GetHeight(void);
	int32 GetResolution(void);
	DescriptorEnumID GetFill(void);
	DescriptorClassID GetMode(void);
};

//-------------------------------------------------------------------------------
//	Definitions -- Dialog items
//-------------------------------------------------------------------------------
enum
	{
	kDNoUI = -1,		// Error.
	kDOk_button = 1,	// Must be one.
	kDCancel_button,	// Must be two.
	kDWidth_staticText,
	kDWidth_editText,
	kDHeight_staticText,
	kDHeight_editText,
	kDResolution_staticText,
	kDResolution_editText,
	kDMode_staticText,
	kDMode_button_RGB,
	kDMode_button_CMYK,
	kDFill_staticText,
	kDFill_button_white,
	kDFill_button_transparent
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
	SPErr error = kSPNoError;
	
	int item = kDNoUI;

	MakeNewDialog dialog(gWidth, gHeight, gResolution, gFill, gMode);
	item = dialog.Modal(gPlugInRef, "MakeNew", kDialogID);

	if (item != kDOk_button)
	{
		// instead of kSPUserCanceledError
		// you'll get a warning dialog if you do
		error = userCanceledErr; 
	}
	else
	{
		gWidth = dialog.GetWidth();
		gHeight = dialog.GetHeight();
		gResolution = dialog.GetResolution();
		gFill = dialog.GetFill();
		gMode = dialog.GetMode();
	}

	return error;
	
}



//-------------------------------------------------------------------------------
//
//	
//
//	Handle initialization of the dialog.
//
//-------------------------------------------------------------------------------
void MakeNewDialog::Init(void)
{
	stringstream temp;

	widthItem.SetItem(PIGetDialogItem(GetDialog(), kDWidth_editText));
	temp << width;
	widthItem.SetText(temp.str().c_str());

	temp.str("");

	heightItem.SetItem(PIGetDialogItem(GetDialog(), kDHeight_editText));
	temp << height;
	heightItem.SetText(temp.str().c_str());

	temp.str("");

	resolutionItem.SetItem(PIGetDialogItem(GetDialog(), kDResolution_editText));
	temp << resolution;
	resolutionItem.SetText(temp.str().c_str());

	modeGroup.SetDialog(GetDialog());
	modeGroup.SetGroupRange(kDMode_button_RGB, kDMode_button_CMYK);

	int radioSelect = kDMode_button_RGB;
	if (mode != classRGBColorMode)
		radioSelect = kDMode_button_CMYK;
	modeGroup.SetSelected(radioSelect);
	
	fillGroup.SetDialog(GetDialog());
	fillGroup.SetGroupRange(kDFill_button_white, kDFill_button_transparent);

	radioSelect = kDFill_button_white;
	if (fill != enumWhite)
		radioSelect = kDFill_button_transparent;
	fillGroup.SetSelected(radioSelect);
}



//-------------------------------------------------------------------------------
//
//	MakeNewDialog::Notify
//
//	If the user hits ok then store the dialog information
//
//-------------------------------------------------------------------------------
void MakeNewDialog::Notify(int32 item)
{
	char * tempText = NULL;

	if (item == kDOk_button)
	{
		tempText = widthItem.GetText();
		if (tempText != NULL)
		{
			width = atoi(tempText);
			delete [] tempText;
			tempText = NULL;
		}

		tempText = heightItem.GetText();
		if (tempText != NULL)
		{
			height = atoi(tempText);
			delete [] tempText;
			tempText = NULL;
		}

		tempText = resolutionItem.GetText();
		if (tempText != NULL)
		{
			resolution = atoi(tempText);
			delete [] tempText;
			tempText = NULL;
		}

		int radioItem = modeGroup.GetSelected();
		if (radioItem == kDMode_button_RGB)
			mode = classRGBColorMode;
		else
			mode = classCMYKColorMode;

		radioItem = fillGroup.GetSelected();
		if (radioItem == kDFill_button_transparent)
			fill = enumTransparent;
		else
			fill = enumWhite;
	}
}



//-------------------------------------------------------------------------------
//
//	MakeNewDialog::GetWidth
//
//	Return the width value from the dialog
//
//-------------------------------------------------------------------------------
int32 MakeNewDialog::GetWidth(void)
{
	return width;
}



//-------------------------------------------------------------------------------
//
//	MakeNewDialog::GetHeight
//
//	Return the height value from the dialog
//
//-------------------------------------------------------------------------------
int32 MakeNewDialog::GetHeight(void)
{
	return height;
}



//-------------------------------------------------------------------------------
//
//	MakeNewDialog::GetResolution
//
//	Return the resolution value from the dialog
//
//-------------------------------------------------------------------------------
int32 MakeNewDialog::GetResolution(void)
{
	return resolution;
}



//-------------------------------------------------------------------------------
//
//	MakeNewDialog::GetMode
//
//	Return the mode value from the dialog
//
//-------------------------------------------------------------------------------
DescriptorEnumID MakeNewDialog::GetMode(void)
{
	return mode;
}



//-------------------------------------------------------------------------------
//
//	MakeNewDialog::GetFill
//
//	Return the fill value from the dialog
//
//-------------------------------------------------------------------------------
DescriptorClassID MakeNewDialog::GetFill(void)
{
	return fill;
}
// end MakeNewUI.cpp
