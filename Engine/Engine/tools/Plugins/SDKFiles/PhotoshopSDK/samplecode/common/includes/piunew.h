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
 
#ifndef __PIUNew_h__	// Defined yet?
#define __PIUNew_h__	// Only define once.

#include "PIDefines.h"
#include "pitypes.h"
#include "SPBasic.h"

#if Macintosh
	#include <typeinfo>
	using namespace std;
#endif

extern SPBasicSuite * sSPBasic;


/** Include this in your plug in to over ride the standard ways to get
    memory. You only need to define a global sSPBasic pointer and include
	PIUNew.cpp. DO NOTE USE new/delete malloc/free from the system OS.
	You will end up fighting with Photoshop for memory.

	Use the buffer procs for large image requests.

*/
void * operator new(size_t s);
void operator delete(void * p);
void * operator new[](size_t s);
void operator delete[](void * p);

#endif // __PIUNew_h__
// end PIUNew.h
