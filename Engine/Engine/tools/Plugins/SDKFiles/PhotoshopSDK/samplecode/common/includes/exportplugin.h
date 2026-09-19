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
//		ExportPlugin.h
//
//	Description:
//		ExportPlugin class definition.
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#ifndef __ExportPlugin_H__
#define __ExportPlugin_H__

#include "PIDefines.h"
#include "piabout.h"
#include "pitypes.h"
#include "piexport.h"
#include "plugin.h"

/** An Export plug in. Derive your export plug in from this class.
    Override the virtual functions as necessary.
*/
class ExportPlugin : public Plugin {
private:

	ExportPlugin(ExportPlugin &); // don't define this
	
protected:

	/** Passed in via the Message routine. 
	    Derived classes can access this protected member.
	*/
	ExportRecord * exportRecord;

	/// Set up parameters for your plug in
	virtual void Parameters(void) {}

	/// Prepare your plug in
	virtual void Prepare(void) {}

	/** Process the file. Use the advanceState procedure to 
	    read the entire file.
	*/
	virtual void Start(void) {}

	/** Obsolete.
	    advanceState and ReadStart can read the entire file.
	*/
	virtual void Continue(void) {}

	/// Clean up and write your scripting parameters.
	virtual void Finish(void) {}

public:
	
	ExportPlugin() : Plugin(), exportRecord(NULL) {}
	virtual ~ExportPlugin() {}

	const bool Message(const bool selector, void * record);
};

#endif
// end ExportPlugin.h
