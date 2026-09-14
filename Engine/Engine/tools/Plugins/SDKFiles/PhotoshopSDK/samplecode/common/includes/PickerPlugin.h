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
//		PickerPlugin.h
//
//	Description:
//		PickerPlugin class definition.
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#ifndef __PickerPlugin_H__
#define __PickerPlugin_H__

#include "PIDefines.h"
#include "piabout.h"
#include "pitypes.h"
#include "pipicker.h"
#include "plugin.h"


// make these global in your derived class
extern SPBasicSuite * sSPBasic;

/** A Picker plug in. Derive your picker plug in from this class.
    Override the virtual functions as necessary.
*/
class PickerPlugin : public Plugin {
private:

	PickerPlugin(PickerPlugin &); // don't define this
	
protected:

	/** Passed in via the Message routine. 
	    Derived classes can access this protected member.
	*/
	PIPickerParams * pickerRecord;

	/// The only thing a PickerPlugin needs to do. Pick a new color.
	virtual void Pick(void) {}

public:
	
	PickerPlugin() : Plugin(), pickerRecord(NULL) {}
	virtual ~PickerPlugin() {}

	/// Process the message based on the selector
	const bool Message(const int16 selector, void * record);

};

#endif
// end PickerPlugin.h
