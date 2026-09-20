#ifndef __ASNameSpace__
#define __ASNameSpace__

/*
 *        Name:	ASNameSpace.h
 *   $Revision: 4 $
 *      Author:	 
 *        Date:	   
 *     Purpose:	AS Name Space Suite (from Adobe Illustrator).
 *
 * Copyright (c) 1986-1996 Adobe Systems Incorporated, All Rights Reserved.
 *
 */


/*******************************************************************************
 **
 **	Imports
 **
 **/

#ifndef __ASTypes__
#include "asTypes.h"
#endif

#ifndef __SPStrings__
#include "SPStrngs.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


/*******************************************************************************
 **
 ** Constants
 **
 **/

#define kASNameSpaceSuite			"AS Name Space Suite"
#define kASNameSpaceSuiteVersion	2
#define kASNameSpaceVersion		kASNameSpaceSuiteVersion

#define kASNameSpaceErr	  		'NAME'


/*******************************************************************************
 **
 **	Types
 **
 **/

typedef struct _t_ASNameSpace* ASNameSpaceRef;
typedef struct t_ASDataStream* ASDataStreamRef;

#if __BUILD_PLUGIN__
#define kNSMaxNameLength		(100)
#define kNSMaxPathLength		((kNSMaxNameLength + 1) * 5)
#define kNSPathSeparator		'/'
#endif


/*******************************************************************************
 **
 **	Suite
 **
 **/

typedef struct {

	ASErr (*AllocateNameSpace) (SPStringPoolRef pool, ASNameSpaceRef* space);
	ASErr (*DisposeNameSpace) (ASNameSpaceRef space);

	ASErr (*SetValue) (ASNameSpaceRef space, char* path, char* type, ...);
	ASErr (*GetValue) (ASNameSpaceRef space, char* path, char* type, ...);

	ASErr (*GetType) (ASNameSpaceRef space, char* path, char** type);
	ASErr (*GetChangeCount) (ASNameSpaceRef space, char* path, long* count);
	ASErr (*RemoveValue) (ASNameSpaceRef space, char* path);
	ASErr (*CountPaths) (ASNameSpaceRef space, char* path, long* count);
	ASErr (*GetNthPath) (ASNameSpaceRef space, char* path, long n, char* nthPath);

	ASErr (*ParseValue) (ASNameSpaceRef space, char* path, ASDataStreamRef filter);
	ASErr (*FlushValue) (ASNameSpaceRef space, char* path, ASDataStreamRef filter);

} ASNameSpaceSuite;


#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

#endif
