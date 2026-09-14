// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2001 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this 
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
//-------------------------------------------------------------------
/*
 * Name:
 *	PIGetFileListSuite.h
 *
 * Copyright 1986-2000 Adobe Systems Incorporated.
 * All Rights Reserved.
 *
 * Purpose:
 *	This file contains all the public definitions and
 *	structures for GetFileList Suite.
 *
 *	Use the GetFileList Suite :
 *	1) To get a list of file handles from FSSpec.
 *	If the second parameter is true (recursive ), it will go through all the folders and subfolders and 
 *	put all the files in a list and sort them and remove the duplicate entries.  The return value
 *	PIActionDescriptor will have PIActionList which contains all the file handles 
 *	in the right order.
 *	2) To get a list of browser names in the Helper\Preview In folder.  All the 
 *	browser names will be ZStrings in PIActionList.
 *	3) Browse a given url with a given index for the browsers in the "Preview In" folder.
 *	   The index parameter is zero based.
 *	4) Browse a url with the default browser.
 *	5) Get the browser's SPPlatformFileSpecification using GetBrowserFileSpec method.  The method
 *	   takes the zero_based index for the browser.
 *	
 *
 * Distribution:
 *	PUBLIC
 *
 * Version history:
 *		Version 1.0.0	2/16/1999	SAK		Created for Photoshop 5.5.
 *			Created by Sarah Kong.
 */

#ifndef __PIGetFileListSuite__
#define __PIGetFileListSuite__

//-------------------------------------------------------------------------------
//	Includes.
//-------------------------------------------------------------------------------
#include "pigeneral.h"

//-------------------------------------------------------------------------------
//	C++ wrapper.
//-------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------
//	UI GetFileList suite.
//-------------------------------------------------------------------------------
#define kPSGetFileListSuite 			"Photoshop GetFileList Suite for Plug-ins"

//-------------------------------------------------------------------------------
//	
//-------------------------------------------------------------------------------
typedef SPAPI SPErr (*GetFileHandleListProc) (PIActionDescriptor& des,FSSpec* fileSpec, bool recurse);
typedef SPAPI SPErr (*GetBrowserNameListProc) (PIActionDescriptor& des);
typedef SPAPI SPErr (*BrowseUrlWithIndexBrowserProc) (uint16 index, const char* url);
typedef SPAPI SPErr (*BrowseUrlProc) (const char* url);
typedef SPAPI SPErr (*GetBrowserFileSpecProc)(uint16 index, SPPlatformFileSpecification* fileSpec);
typedef	SPAPI SPErr (*GetDefaultSystemScriptProc)(int16& script);
typedef SPAPI SPErr (*HasDoubleByteInStringProc)(const char* charString, bool& hasDoubleByte);
typedef SPAPI SPErr (*GetWebTemplateFolderSpecProc)(SPPlatformFileSpecification* fileSpec);
typedef SPAPI SPErr (*GetWebTemplateListProc) (PIActionDescriptor& styleList, bool recurse);
typedef SPAPI SPErr (*GetWindowsFileTypesProc) (char * fileTypeList, int bufferSize);
typedef SPAPI SPErr (*GetMacFileTypesProc) (OSType * typeList, int &numTypes, int bufferSize);
typedef SPAPI SPErr (*GetPresetsFolderSpecProc) (SPPlatformFileSpecification *fileSpec);
typedef SPAPI SPErr (*InvalidateFileBrowserCacheProc) (const SPPlatformFileSpecification *fileSpec);


//-------------------------------------------------------------------------------
//	 GetFileList suite version 1.
//-------------------------------------------------------------------------------

#define kPSGetFileListSuiteVersion1		1
#define kPSGetFileListSuiteVersion2		2
#define kPSGetFileListSuiteVersion3		3
#define kPSGetFileListSuiteVersion4		4
#define kPSGetFileListSuiteVersion5		5

typedef struct 
	{
	GetFileHandleListProc 			GetFileHandleList;
	GetBrowserNameListProc			GetBrowserNameList;
	BrowseUrlWithIndexBrowserProc	BrowseUrlWithIndexBrowser;
	BrowseUrlProc					BrowseUrl;
	GetBrowserFileSpecProc			GetBrowserFileSpec;
	} PSGetFileListSuite1;

typedef struct 
	{
	GetFileHandleListProc 			GetFileHandleList;
	GetBrowserNameListProc			GetBrowserNameList;
	BrowseUrlWithIndexBrowserProc	BrowseUrlWithIndexBrowser;
	BrowseUrlProc					BrowseUrl;
	GetBrowserFileSpecProc			GetBrowserFileSpec;
	GetDefaultSystemScriptProc		GetDefaultSystemScript;
	} PSGetFileListSuite2;

typedef struct 
	{
	GetFileHandleListProc 			GetFileHandleList;
	GetBrowserNameListProc			GetBrowserNameList;
	BrowseUrlWithIndexBrowserProc	BrowseUrlWithIndexBrowser;
	BrowseUrlProc					BrowseUrl;
	GetBrowserFileSpecProc			GetBrowserFileSpec;
	GetDefaultSystemScriptProc		GetDefaultSystemScript;
	HasDoubleByteInStringProc		HasDoubleByteInString;					
	} PSGetFileListSuite3;

typedef struct 
	{
	GetFileHandleListProc 			GetFileHandleList;
	GetBrowserNameListProc			GetBrowserNameList;
	BrowseUrlWithIndexBrowserProc	BrowseUrlWithIndexBrowser;
	BrowseUrlProc					BrowseUrl;
	GetBrowserFileSpecProc			GetBrowserFileSpec;
	GetDefaultSystemScriptProc		GetDefaultSystemScript;
	HasDoubleByteInStringProc		HasDoubleByteInString;
	GetWebTemplateFolderSpecProc	GetWebTemplateFolderSpec;
	GetWebTemplateListProc			GetWebTemplateList;
	} PSGetFileListSuite4;

typedef struct 
	{
	GetFileHandleListProc 			GetFileHandleList;
	GetBrowserNameListProc			GetBrowserNameList;
	BrowseUrlWithIndexBrowserProc	BrowseUrlWithIndexBrowser;
	BrowseUrlProc					BrowseUrl;
	GetBrowserFileSpecProc			GetBrowserFileSpec;
	GetDefaultSystemScriptProc		GetDefaultSystemScript;
	HasDoubleByteInStringProc		HasDoubleByteInString;
	GetWebTemplateFolderSpecProc	GetWebTemplateFolderSpec;
	GetWebTemplateListProc			GetWebTemplateList;
	GetWindowsFileTypesProc			GetWindowsFileTypes;
	GetMacFileTypesProc				GetMacFileTypes;
	GetPresetsFolderSpecProc		GetPresetsFolderSpec;
	InvalidateFileBrowserCacheProc	InvalidateFileBrowserCache;
	} PSGetFileListSuite5;

//-------------------------------------------------------------------------------
//	C++ wrapper.
//-------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

//-------------------------------------------------------------------------------
#endif	// PIGetFileListSuite
