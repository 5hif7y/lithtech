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
//		SelectionPlugin.h
//
//	Description:
//		SelectionPlugin class definition.
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#ifndef __SelectionPlugin_H__
#define __SelectionPlugin_H__

#include "PIDefines.h"
#include "piabout.h"
#include "pitypes.h"
#include "piselection.h"
#include "plugin.h"


/** Derive your Selection plug in from this class
	Override the virtual functions as necessary
*/
class SelectionPlugin : public Plugin {
private:

	SelectionPlugin(SelectionPlugin &); // don't define this
	
protected:

	PISelectionParams * selectionRecord;

	virtual void Execute(void) {}

public:
	
	SelectionPlugin() : Plugin(), selectionRecord(NULL) {}
	virtual ~SelectionPlugin() {}

	const bool Message(const int16 selector, void * record);
};

#endif
// end SelectionPlugin.h
