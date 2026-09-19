/***********************************************************************/
/*                                                                     */
/* ADMCustomResourceOld.h                                              */
/* Old versions of ADM Custom Resource Suite                           */
/*                                                                     */
/* ADOBE SYSTEMS INCORPORATED                                          */
/* Copyright 1996-2001 Adobe Systems Incorporated                      */
/* All Rights Reserved                                                 */
/*                                                                     */
/* NOTICE:  Adobe permits you to use, modify, and distribute this file */
/* in accordance with the terms of the Adobe license agreement         */
/* accompanying it. If you have received this file from a source other */
/* than Adobe, then your use, modification, or distribution of it      */
/* requires the prior written permission of Adobe.                     */
/*                                                                     */
/***********************************************************************/

#ifndef __ADMCustomResourceOld__
#define __ADMCustomResourceOld__


// =============================================================================
//		* ADM Custom Resource Suite, version 1
// =============================================================================

#define kADMCustomResourceSuiteVersion1	1

// -----------------------------------------------------------------------------

typedef struct ADMCustomResourceSuite1
{

	// *** This suite is FROZEN.
	// *** It shipped with Photoshop 5.5 and ImageReady 2.0.

	ASErr ASAPI (*AddCustomResource)(SPPluginRef inPlugin, ADMResourceType inType,
				const ADMCustomResourceInfo* inResInfo,
				ADMCustomResourceRef* outFormat);

	ASErr ASAPI (*RemoveCustomResource)(ADMResourceType inType, const char* inName,
				ASInt32 inVersion);

	ASErr ASAPI (*OpenResourceStream)(ADMCustomResourceRef inFormat, SPPluginRef inPlugin,
				ASInt32 inResID, const char* inResName, ASDataStreamRef* outResStream);

	ASErr ASAPI (*CloseResourceStream)(ASDataStreamRef inResStream);
	
	ASErr ASAPI (*FindAResource)(SPPluginRef inPlugin, ADMResourceType inType,
				ASInt32 inResID, const char* inResName, ADMResourceLocationInfo* outLocation);

	ASErr ASAPI (*CountCustomResources)(ASInt32* outCount);
	ASErr ASAPI (*GetNthCustomResource)(ASInt32 inIndex, ADMCustomResourceRef* outFormat);
	
	ASErr ASAPI (*GetResourcePlugin)(ADMCustomResourceRef inFormat, SPPluginRef* outPlugin);
	ASErr ASAPI (*GetResourceInfo)(ADMCustomResourceRef inFormat, ADMCustomResourceInfo* outInfo);
	ASErr ASAPI (*GetResourceType)(ADMCustomResourceRef inFormat, ADMResourceType* outType);

}
ADMCustomResourceSuite1;


// -----------------------------------------------------------------------------

#define kADMCustomResourceSuiteVersion2	2

// -----------------------------------------------------------------------------

typedef struct ADMCustomResourceSuite2
{

	// *** This suite is FROZEN for changes.
	// *** It shipped with Acrobat 5.0, After Effects 5.0

		// FIX_ME [scouten 08/04/99]: These function names would make more sense
		// if all instances of "CustomResource" were replaced with "ResourceFormat."
		// A future version of this suite will do that.

	ASErr ASAPI (*AddCustomResource)(SPPluginRef inPlugin, ADMResourceType inType,
				const ADMCustomResourceInfo* inResInfo,
				ADMCustomResourceRef* outFormat);

	ASErr ASAPI (*RemoveCustomResource)(ADMResourceType inType, const char* inName,
				ASInt32 inVersion);

	ASErr ASAPI (*OpenResourceStream)(ADMCustomResourceRef inFormat, SPPluginRef inPlugin,
				ASInt32 inResID, const char* inResName, ASDataStreamRef* outResStream);

	ASErr ASAPI (*CloseResourceStream)(ASDataStreamRef inResStream);
	
	ASErr ASAPI (*FindAResource)(SPPluginRef inPlugin, ADMResourceType inType,
				ASInt32 inResID, const char* inResName, ADMResourceLocationInfo* outLocation);

	ASErr ASAPI (*CountCustomResources)(ASInt32* outCount);
	ASErr ASAPI (*GetNthCustomResource)(ASInt32 inIndex, ADMCustomResourceRef* outFormat);
	
	ASErr ASAPI (*GetResourcePlugin)(ADMCustomResourceRef inFormat, SPPluginRef* outPlugin);
	ASErr ASAPI (*GetResourceInfo)(ADMCustomResourceRef inFormat, ADMCustomResourceInfo* outInfo);
	ASErr ASAPI (*GetResourceType)(ADMCustomResourceRef inFormat, ADMResourceType* outType);

	ASErr ASAPI (*FindCustomResource)(ADMResourceType inResType,
				const ADMCustomResourceInfo* inResInfo,	ADMCustomResourceRef *outResource);
}
ADMCustomResourceSuite2;



#endif	// __ADMCustomResourceOld__
