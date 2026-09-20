#ifndef __ASDataStream__
#define __ASDataStream__

/*
 *        Name:	ASDataStream.h
 *   $Revision: 6 $
 *      Author:	 
 *        Date:	   
 *     Purpose:	Adobe AS 2.0 Data Filter Suite.
 *
 * Copyright (c) 1986-1998 Adobe Systems Incorporated, All Rights Reserved.
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

#ifndef __ASNameSpace__
#include "asnamespace.h"
#endif

#ifndef _SPFiles__
#include "SPFiles.h"
#endif

#ifndef __SPPlugins__
#include "SPPlugs.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN


/*******************************************************************************
 **
 **	Constants
 **
 **/

#define kASDataStreamSuite		"AS Data Filter Suite"
#define kASDataStreamSuiteVersion	2


#define kASDataStreamErr			'DFER'


/*******************************************************************************
 **
 **	Types
 **
 **/
 
#ifndef ASDataStreamRef
typedef struct t_ASDataStream* ASDataStreamRef;
#endif

/*******************************************************************************
 **
 **	Suite
 **
 **/

typedef struct {

	ASAPI ASErr (*LinkDataStream) (ASDataStreamRef prev, ASDataStreamRef next);
	ASAPI ASErr (*UnlinkDataStream) (ASDataStreamRef next, ASDataStreamRef* prev);
	ASAPI ASErr (*ReadDataStream) (ASDataStreamRef filter, char* store, long* count);
	ASAPI ASErr (*WriteDataStream) (ASDataStreamRef filter, char* store, long* count);
	ASAPI ASErr (*SeekDataStream) (ASDataStreamRef filter, long* count);
	ASAPI ASErr (*MarkDataStream) (ASDataStreamRef filter, long* count);
	ASAPI ASErr (*NewFileDataStream) (SPPlatformFileSpecification* spec, char* mode, long creator, long type, ASDataStreamRef* filter);
	ASAPI ASErr (*NewBufferDataStream) (long size, ASDataStreamRef* filter);
	ASAPI ASErr (*NewHexdecDataStream) (char* state, long shift, ASDataStreamRef* filter);
	ASAPI ASErr (*NewBlockDataStream) (void* address, long size, ASDataStreamRef* filter);
	ASAPI ASErr (*NewResourceDataStream) (SPPluginRef plugin, long type, long id, char* name, ASDataStreamRef* filter);

} ASDataStreamSuite;


#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif


#endif
