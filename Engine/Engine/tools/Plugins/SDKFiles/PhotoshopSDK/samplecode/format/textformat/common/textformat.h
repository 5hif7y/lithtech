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
//		TextFormat.h
//
//	Description:
//
//	Use:
//
//-------------------------------------------------------------------------------

#ifndef __TextFormat_H__	// Has this not been defined yet?
#define __TextFormat_H__	// Only include this once by predefining it

#include "PIDefines.h"
#include "piformat.h"					// Format Photoshop header file.
#include "PIUtilities.h"				// SDK Utility library.
#include "FileUtilities.h"				// File Utility library.
#include "FormatPlugin.h"


//-------------------------------------------------------------------------------
//	Structure -- FileHeader
//-------------------------------------------------------------------------------
class TextFormatPlugin : public FormatPlugin {

	virtual void ReadPrepare(void);
	virtual void ReadStart(void);
	virtual void ReadFinish(void);

public:
	TextFormatPlugin();
	virtual ~TextFormatPlugin() {}
};


#endif // __TextFormat_H__
