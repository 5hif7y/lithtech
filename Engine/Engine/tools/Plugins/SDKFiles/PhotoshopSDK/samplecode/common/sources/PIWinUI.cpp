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
//-------------------------------------------------------------------------------
//
//	File:
//		PIWinUI.cpp
//
//	Description:
//		Windows only code for the UI elements
//
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#include "PhotoshopSDK.h"

using namespace std;

void CenterDialog(HWND hDlg);

class PIAboutBox : public PIDialog {
private:
	virtual void Init(void);
	virtual void Notify(int32 item);
	virtual void Message(UINT wMsg, WPARAM wParam, LPARAM lParam);
public:
	PIAboutBox() : PIDialog() {}
	~PIAboutBox() {}

};

DLLExport BOOL WINAPI GenericProc(HWND hDlg, 
								  UINT wMsg, 
								  WPARAM wParam, 
								  LPARAM lParam);


int PIDialog::Modal(SPPluginRef plugin, char *, int ID)
{
	dialogID = ID;
	pluginRef = plugin;

	int itemHit = -1;

	itemHit = DialogBoxParam(GetDLLInstance(pluginRef),
		                     MAKEINTRESOURCE(dialogID),
						     GetActiveWindow(),
						     GenericProc,
						     (long)this);

	return itemHit;
}

PIItem PIDialog::GetItem(int32 index)
{
	return (GetDlgItem(dialog, index));
}

void PIComboBox::Clear(void)
{
	SendMessage(item, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
}

void PIComboBox::AppendItem(const char * in)
{
	SendMessage(item, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)in);
}

void PIComboBox::SetCurrentSelection(int16 index)
{
	SendMessage(item, CB_SETCURSEL, (WPARAM)index, (LPARAM)0);
}

char * PIComboBox::GetCurrentSelection(void)
{
	char * returnValue = NULL;
	int index = (INT)SendMessage(item, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (index == CB_ERR) return returnValue;
	int strLen = (INT)SendMessage(item, CB_GETLBTEXTLEN, (WPARAM)index, (LPARAM)0);
	if (strLen == CB_ERR) return returnValue;
	returnValue = new char[strLen + 1];
	if (returnValue == NULL) return returnValue;
	SendMessage(item, CB_GETLBTEXT, (WPARAM)index, (LPARAM)(LPCSTR)returnValue);
	return returnValue;
}

void PIText::SetText(const char * in)
{
	SendMessage(item, WM_SETTEXT, (WPARAM)0, (LPARAM)(LPCTSTR)in);
}

char * PIText::GetText(void)
{
	char * returnValue = NULL;
	int strLen = (INT)SendMessage(item, WM_GETTEXTLENGTH, (WPARAM)0, (LPARAM)0);
	if (strLen == 0) return returnValue;
	strLen++;
	returnValue = new char[strLen];
	if (returnValue == NULL) return returnValue;
	SendMessage(item, WM_GETTEXT, (WPARAM)strLen, (LPARAM)(LPCSTR)returnValue);
	return returnValue;
}

void PIRadioGroup::SetGroupRange(int32 first, int32 last)
{
	firstItem = first;
	lastItem = last;
}

void PIRadioGroup::SetSelected(int32 s)
{
	if (firstItem == -1 || lastItem == -1 || dialog == NULL) return;
	if (s > lastItem || s < firstItem) return;
	CheckRadioButton(dialog, firstItem, lastItem, s);
}

int32 PIRadioGroup::GetSelected(void)
{
	int32 item = -1;
	if (firstItem == -1 || lastItem == -1 || dialog == NULL) return item;
	for (item = firstItem; item <= lastItem; item++)
		if (IsDlgButtonChecked(dialog, item))
			break;
	if (item > lastItem) return -1;
	return item;
}

PIListBox::PIListBox() : item(NULL) {}

PIListBox::PIListBox(PIItem in) : item(in) {}

PIListBox::~PIListBox() {}

void PIListBox::Clear()
{
	SendMessage(item, LB_RESETCONTENT, 0, 0);
}
	
char * PIListBox::GetCurrentSelectionText(void)
{
	char * returnText = NULL;
	int32 index = GetCurrentSelectionIndex();
	int32 textLength = SendMessage(item, LB_GETTEXTLEN, index, 0);
	if (textLength > 0)
	{
		returnText = new char[textLength + 1];
		SendMessage(item, LB_GETTEXT, index, (LPARAM)returnText);
	}
	return returnText;
}

int32 PIListBox::GetCurrentSelectionIndex(void)
{
	return SendMessage(item, LB_GETCURSEL, 0, 0);
}

int32 PIListBox::GetUserData(int32 index)
{
	return SendMessage(item, LB_GETITEMDATA, index, 0);
}

void PIListBox::SetUserData(int32 index, int32 data)
{
	SendMessage(item, LB_SETITEMDATA, index, data);
}

int32 PIListBox::AppendItem(const char * in)
{
	return SendMessage(item, LB_ADDSTRING, 0, (LPARAM)in);
}

void PIListBox::SetCurrentSelection(int32 index)
{
	SendMessage(item, LB_SETCURSEL, index, 0);
}

void PICheckBox::SetText(const char * in)
{
	SendMessage(item, WM_SETTEXT, (WPARAM)0, (LPARAM)(LPCTSTR)in);
}

void PICheckBox::SetChecked(bool checked)
{
	SendMessage(item, BM_SETCHECK, checked, 0);
}

bool PICheckBox::GetChecked(void)
{
	return SendMessage(item, BM_GETCHECK, 0, 0) == BST_CHECKED;
}


PIItem PIGetDialogItem(PIDialogPtr dialog, int16 itemNumber)
{
	return (GetDlgItem(dialog, itemNumber));
}


DLLExport BOOL WINAPI GenericProc(HWND hDlg, 
								  UINT wMsg, 
								  WPARAM wParam, 
								  LPARAM lParam)
{
	static PIDialog * owner = NULL;

	switch (wMsg)
	{
		case WM_INITDIALOG:
		{
			CenterDialog(hDlg);
			owner = static_cast<PIDialog *>((void*)(lParam));
			if (owner == NULL) return FALSE;
			owner->SetDialog(hDlg);
			owner->Init();
			return TRUE;
			break;
		}
		case WM_COMMAND:
		{
			int item = LOWORD (wParam);
			int cmd = HIWORD (wParam);
			if (owner != NULL)
				owner->Notify(item);
			if ((item == 1 || item == 2) && cmd == BN_CLICKED)
				EndDialog(hDlg, item);
			return TRUE;
			break;
		}
		case WM_DESTROY:
		{
			owner = NULL;
			return FALSE;
		}
		default:
			// You may get messages before WM_INITDIALOG
			if (owner != NULL)
				owner->Message(wMsg, wParam, lParam);
			else
				DefWindowProc(hDlg, wMsg, wParam, lParam);
			return FALSE;
			break;
	}
	return TRUE;
}

/* Centers a dialog template 1/3 of the way down on the main screen */

void CenterDialog(HWND hDlg)
{
	int  nHeight;
    int  nWidth;
    int  nTitleBits;
    RECT rcDialog;
    RECT rcParent;
    int  xOrigin;
    int  yOrigin;
    int  xScreen;
    int  yScreen;
    HWND hParent = GetParent(hDlg);

    if  (hParent == NULL)
        hParent = GetDesktopWindow();

    GetClientRect(hParent, &rcParent);
    ClientToScreen(hParent, (LPPOINT)&rcParent.left); 
    ClientToScreen(hParent, (LPPOINT)&rcParent.right);

    // Center on Title: title bar has system menu, minimize,  maximize bitmaps
    // Width of title bar bitmaps - assumes 3 of them and dialog has a sysmenu
    nTitleBits = GetSystemMetrics(SM_CXSIZE);

    // If dialog has no sys menu compensate for odd# bitmaps by sub 1 bitwidth
    if  ( ! (GetWindowLong(hDlg, GWL_STYLE) & WS_SYSMENU))
        nTitleBits -= nTitleBits / 3;

    GetWindowRect(hDlg, &rcDialog);
    nWidth  = rcDialog.right  - rcDialog.left;
    nHeight = rcDialog.bottom - rcDialog.top;

    xOrigin = max(rcParent.right - rcParent.left - nWidth, 0) / 2
            + rcParent.left - nTitleBits;
    xScreen = GetSystemMetrics(SM_CXSCREEN);
    if  (xOrigin + nWidth > xScreen)
        xOrigin = max (0, xScreen - nWidth);

	yOrigin = max(rcParent.bottom - rcParent.top - nHeight, 0) / 3
            + rcParent.top;
    yScreen = GetSystemMetrics(SM_CYSCREEN);
    if  (yOrigin + nHeight > yScreen)
        yOrigin = max(0 , yScreen - nHeight);

    SetWindowPos(hDlg, NULL, xOrigin, yOrigin, nWidth, nHeight, SWP_NOZORDER);
}

void PIAboutBox::Init()
{
	char message[kStr255InitLen] = "";
	LoadString(GetDLLInstance(), GetID(), message, kStr255MaxLen);
	SetDlgItemText(GetDialog(), MESSAGE, message);
}

void PIAboutBox::Notify(int32 item)
{
	if (item == ADOBEURL || item == PARTNERSURL)
	{
		auto_ptr<char> url(new char[kStr255InitLen]);
		GetDlgItemText(GetDialog(), item, url.get(), kStr255InitLen);
		ostringstream stringStream;
		stringStream << "http://" << url.get();
		sPSFileList->BrowseUrl(stringStream.str().c_str());
	}
}

void PIAboutBox::Message(UINT wMsg, WPARAM wParam, LPARAM)
{
	switch (wMsg)
	{
		case WM_CHAR:
			{
			TCHAR chCharCode = (TCHAR) wParam;
			if (chCharCode == VK_ESCAPE || chCharCode == VK_RETURN)
				EndDialog(GetDialog(), 0);
			}
			break;
 
		case WM_LBUTTONDOWN:
            EndDialog(GetDialog(), 0);
            break;
	}
}

void DoAbout(SPPluginRef plugin, int aboutID)
{
	PIAboutBox aboutBox;
	aboutBox.Modal(plugin, "Generic About Box", aboutID);
}
// end PIWinUI.cpp
