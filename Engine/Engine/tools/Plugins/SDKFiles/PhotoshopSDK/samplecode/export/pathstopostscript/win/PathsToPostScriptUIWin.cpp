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
/*
	File: PathsToPostScriptUIWin.cpp

	Copyright 1990-1991, Thomas Knoll.
	All rights reserved.

	Windows platform dependent C source file for Export example.
*/

#include "PhotoshopSDK.h"
#include "PathsToPostScript.h"

/*****************************************************************************/

const uint8 kDPopUp = 13;

/*****************************************************************************/

DLLExport UINT WINAPI ExportProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);
Boolean BuildPathMenu(GPtr globals, HWND hDlg, short *defaultItem);
void AppendMenuItem( HWND hDlg, Str255 itemText );
static void DeleteItemsTo (HWND hDlg, short limit);

/*****************************************************************************/

void DoAbout (AboutRecordPtr about)
{
	ShowAbout(about);
}

/*****************************************************************************/

void DisplayStopAlert (GPtr globals, short id)
{
	ShowStop (GetActiveWindow(), id, id, NULL, NULL);

	gResult = 1; // error and return
}


/*****************************************************************************/

Boolean DoUI (GPtr globals)
{
	char *my_lpstrFile = (char *)gStuff->filename;
	char my_filename[256];
	char my_lpstrFileTitle[256];
	char my_extension[_MAX_EXT] = ".ai\0\0";
	char my_lpstrInitialDir[256];
	char my_dir[256];
	const char *my_lpstrDefExt = "ai";
	const char *my_lpstrFilter = "Adobe Illustrator (*.ai)\0*.ai\0\0\0";
	const char *my_lpstrTitle = "IllustratorExport";
	const char *my_templateName = "EXPORTPARAM";
	PlatformData *platform;
	int nResult;
	int len = gStuff->filename[0];
	short hlen;
	OPENFILENAME lpofn;

	gStuff->filename[len+1] = 0; // make sure null terminated

	if (gAliasHandle != NULL)
	{
		/* we've been handed an aliasHandle. Use that instead of querying */
		PIHandle2PString(gAliasHandle, (unsigned char *)&my_lpstrInitialDir);
		hlen = my_lpstrInitialDir[0];

		if (my_lpstrInitialDir [hlen] == '/' || my_lpstrInitialDir [hlen] == '\\')
		{
			_splitpath((const char *)&gStuff->filename[1],
					   NULL, /* drive */
					   NULL, /* dir */
					   (Ptr)gStuff->filename,
					   NULL); /* ext */
			AppendString((unsigned char *)&my_lpstrInitialDir, gStuff->filename, 0, (short)strlen((const char *)gStuff->filename));
			AppendCharToString((unsigned char *)my_lpstrInitialDir, '.');
			AppendString((unsigned char *)&my_lpstrInitialDir, (const unsigned char *)my_lpstrDefExt, 0, (short)strlen(my_lpstrDefExt));
			hlen = my_lpstrInitialDir[0];
		}
		else my_lpstrInitialDir [hlen+1] = 0;
		
		gStuff->filename[ (gStuff->filename[0] = 0)+1 ] = 0;
		AppendString(gStuff->filename, 
					 (const unsigned char *)&my_lpstrInitialDir, 1,
					 hlen);
		gStuff->filename[ gStuff->filename[0]+1 ] = 0;
		/* makes string pascal and null terminated */

		/* This has to be a Photoshop handle, since it was handed
		   to us by the host, so dispose it (it'll be recreated
		   later.) */
		PIDisposeHandle((Handle)gAliasHandle);
		gAliasHandle = NULL;
	}
	else
		PICopy (my_lpstrInitialDir, &gStuff->filename[1], (short)gStuff->filename[0]);
	 
	_splitpath((const char *)&gStuff->filename[1],
			   my_lpstrInitialDir, /* drive */
			   my_dir, /* dir */
			   (Ptr)gStuff->filename,
			   my_extension); /* ext */
	strncat (my_lpstrInitialDir, my_dir, 255); 
	
	platform = (PlatformData *)((ExportRecordPtr) gStuff)->platformData;

	if (!gQueryForParameters)
	{
		strncat(my_lpstrInitialDir, (const char *)gStuff->filename, 255);
		strncat(my_lpstrInitialDir, ".", 2);
		strncat(my_lpstrInitialDir, my_lpstrDefExt, 5);
		strncpy((Ptr)gStuff->filename, my_lpstrInitialDir, 255);
		return TRUE;
	}
	else // gQueryForParameters
	{
		/* initialize OPENFILENAME structure */

		strncpy(my_filename, (const char *)gStuff->filename, 255);
		strncat(my_filename, ".", 2);
		strncat(my_filename, my_lpstrDefExt, 5);

		lpofn.lStructSize = sizeof(lpofn);
		lpofn.hwndOwner = (HWND)platform->hwnd;
		lpofn.hInstance = GetDLLInstance();
		lpofn.lpstrFilter = my_lpstrFilter;
		lpofn.lpstrCustomFilter = NULL;
		lpofn.nMaxCustFilter = 0;
		lpofn.nFilterIndex = 0;
		lpofn.lpstrFile = my_lpstrFile;
		lpofn.nMaxFile = 255;
		lpofn.lpstrFileTitle = my_lpstrFileTitle;
		lpofn.nMaxFileTitle = 255;
		lpofn.lpstrInitialDir = my_lpstrInitialDir;
		lpofn.lpstrTitle = my_lpstrTitle;
		lpofn.Flags = OFN_HIDEREADONLY | 
					  OFN_NOREADONLYRETURN | 
					  OFN_SHAREAWARE | 
					  OFN_PATHMUSTEXIST |
					  OFN_OVERWRITEPROMPT | 
					  OFN_ENABLEHOOK |
					  OFN_ENABLETEMPLATE;
					  /* OFN_LONGNAMES |
					  OFN_EXPLORER; */
		lpofn.nFileOffset = 0;
		lpofn.nFileExtension = 0;
		lpofn.lpstrDefExt = my_lpstrDefExt;
		lpofn.lCustData = (long)globals;
		lpofn.lpfnHook = ExportProc;
		lpofn.lpTemplateName = my_templateName;
		lpofn.lStructSize = sizeof(lpofn);

		if (!(nResult = GetSaveFileName(&lpofn)))
			gResult = userCanceledErr;
		else
			gSameNames = (strncmp(my_filename, my_lpstrFileTitle, 255) == noErr);

		return (nResult);
	}
}


/*****************************************************************************/

Boolean CreateExportFile (GPtr globals)
{
	
	OFSTRUCT ostr;
	OSErr  result;
	HFILE file;
	short	len = strlen((const char *)gStuff->filename);
	Str255	filecopy = "";
	char	my_extension[_MAX_EXT] = ".ai\0\0";
	/* Delete the file if it exists. */

	gFRefNum = 0;

	result = OpenFile((const char *)gStuff->filename,&ostr,(WORD)OF_DELETE);

	if (result == -1)
		{
		if (ostr.nErrCode != ENOENT)
			{
			gResult = openErr;
			return FALSE;
			}
		}

	/* Create a new file */

	file = OpenFile((const char *)gStuff->filename,&ostr,OF_CREATE|OF_WRITE|OF_SHARE_EXCLUSIVE);
	
	if (file == -1)
		{
		gResult = openErr;
		return FALSE;
		}

	else
	{ 
		gFRefNum = file;
		return TRUE;
	}

}
	
/*****************************************************************************/
// We're handed a string and a null-terminated filename.  Change it into the
// basic small file name as a pascal string.

void SmallFileName(Str255 s, const Str255 file)
{
	char	my_extension[_MAX_EXT] = ".ai\0\0";
	Str255	name = "";

	s[ (s[0]=0)+1 ] = 0; // clear out string
	
	_splitpath((const char *)file,
			   NULL, /* drive */
			   NULL, /* dir */
			   (Ptr)name, /* name */
			   my_extension);
	_makepath ((Ptr)name,
			   NULL, /* drive */
			   NULL, /* dir */
			   (Ptr)name,
			   my_extension);
	AppendString(s, name, 0, (short)strlen((const char *)name));
}


/*****************************************************************************/

void DeleteItemsTo (HWND hDlg, short limit)
{
	
	short item = (short) SendDlgItemMessage( hDlg, kDPopUp, CB_GETCOUNT, 0, 0L );
	
	while (item > limit)
	{
		SendDlgItemMessage( hDlg, kDPopUp, CB_DELETESTRING, item-1, 0L );
		--item;
	}
	
}

/*****************************************************************************/

void AppendMenuItem( HWND hDlg, Str255 itemText )
{	
	itemText[ itemText[0]+1 ] = 0; // must be null terminated
	SendDlgItemMessage( hDlg, kDPopUp, CB_ADDSTRING, 0, (LPARAM) (LPCSTR) &itemText[1] );	
}

/*****************************************************************************/

Boolean BuildPathMenu(GPtr globals, HWND hDlg, short *defaultItem)
{
	int32	pathCount;
	Str255	s = "";

	PIGetString(kDocBounds, s); // "Document bounds"
	AppendMenuItem (hDlg, s);
	
	PIGetString(kAllPaths, s); // "All paths"
	AppendMenuItem (hDlg, s);

	if (!GetPathCount (globals, &pathCount) || pathCount < 1)
	{
		DeleteItemsTo( hDlg, 1 );
		*defaultItem = 1;
	}
		
	else
	{
		int32 pathIndex;
		
		int16 workPathIndex = GetPathIndex (globals, propWorkPathIndex);
		int16 clippingPathIndex = GetPathIndex (globals, propClippingPathIndex);
		int16 targetPathIndex = GetPathIndex (globals, propTargetPathIndex);

		DeleteItemsTo (hDlg, (short)((pathCount == 1) ? (iAllPaths-1) : (iPathStart-1)));
		
		for (pathIndex = 0; pathIndex < pathCount; ++pathIndex)
			{
		
			if (!GetPathName (globals, pathIndex, s))
				return FALSE;
				
			AppendMenuItem (hDlg, s);
			
			}
			
		if (*defaultItem < 1)
		{ // default still hasn't been set.  Better do it.
			if (pathCount == 1 || targetPathIndex < 0)
				*defaultItem = iAllPaths;
			else
				*defaultItem = iPathStart + targetPathIndex;
		}		
	}
		
	SendDlgItemMessage( hDlg, kDPopUp, CB_SETCURSEL, (WPARAM)*defaultItem-1, (LPARAM)0 );

	return TRUE;
	
}

/*****************************************************************************/

Boolean CloseExportFile (GPtr globals)
{

	char *filename;
	char my_lpstrInitialDir[256];
	char my_dir[256];
	char my_extension[_MAX_EXT] = ".ai\0\0";
	int	len = 0;

	OFSTRUCT	ostr;
	
	if (gFRefNum != 0)
		_lclose(gFRefNum);

	/* Delete the file if we had an error. */

	if (gResult != noErr)
		{
		filename = (char *) gStuff->filename;
		OpenFile(filename,&ostr,(WORD)OF_DELETE);
		}
		
	/* Mark the file as clean. */
	
	gStuff->dirty = FALSE;
	
	/* Create alias for scripting system */

	strncpy(my_lpstrInitialDir, (const char *)gStuff->filename, 255);
	
	len = strlen((const char *)gStuff->filename)+1;

	if (gSameNames)
	{
		_splitpath((const char *)gStuff->filename,
				   my_lpstrInitialDir, /* drive */
				   my_dir, /* dir */
				   (Ptr)gStuff->filename, /* name */
				   my_extension); /* ext */
		strncat(my_lpstrInitialDir, my_dir, 255);
		len = strlen(my_lpstrInitialDir)+1;
	}
		
	if (len > 1)
	{ // filename got something.  Store it.
		my_lpstrInitialDir [len] = 0;

		/* If same names, then ignore filename.  Else, leave it in. */

		/* uncomment out to make a pascal string
		gStuff->filename[0] = len;
		strncpy(&gStuff->filename[1], my_lpstrInitialDir, len);
		*/
		strncpy((Ptr)gStuff->filename, my_lpstrInitialDir, len); // comment out for pascal
		gStuff->filename[len+1] = 0; // just in case


		gAliasHandle = PINewHandle(len+1);
		if (gAliasHandle != NULL)
		{
			HLock(gAliasHandle);
			strncpy(*gAliasHandle, (const char *)gStuff->filename, len+1);
			HUnlock(gAliasHandle);
		}
	} // filename nothing

	return TRUE;
	
}

/*****************************************************************************/
/* Dispose alias handle */

void DestroyAliasHandle (GPtr globals)
{
	if (gAliasHandle != NULL)
		PIDisposeHandle((Handle)gAliasHandle);
	gAliasHandle = NULL;
}

/*****************************************************************************/

/* Dialog box code to ask for export parameters. */

#if defined(__BORLANDC__)
#pragma argsused // lParam in next function not used, so disable warning
#endif

DLLExport UINT WINAPI ExportProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)   // Win32 Change
{
	int				item, cmd;	// Win32 Change
	static GPtr		globals = NULL;		  /* need to be static */
	LPOPENFILENAME	p = NULL;

    switch  (wMsg)
	{
		case WM_INITDIALOG:
			
			// On INITDIALOG lparam contains a pointer to the original
			// OPENFILENAME structure, where we stuck the pointer to our
			// globals, which we'll need for other calls.
			p = (LPOPENFILENAME)lParam;
			globals = (GPtr)(p->lCustData);

			// Now build the path menu itself
			BuildPathMenu(globals, hDlg, &gPopUpValue);

			CenterDialog(hDlg);
			
			return TRUE; // let OS do the rest
			break;
		case WM_COMMAND   :
	  		item = COMMANDID (wParam);              // WIN32 Change
            cmd = HIWORD (wParam);
			switch  (item)
			{
				case IDOK:
					if ( cmd == BN_CLICKED )
					{
						gPopUpValue =
							1+(short)SendDlgItemMessage(hDlg,
														kDPopUp,
														CB_GETCURSEL,
														0,0L);
					}
					// drop into default
				default:
					return FALSE;
					break;
            }
            break;
	}
    return  FALSE;
}

// end PathsToPostScriptUIWin.cpp