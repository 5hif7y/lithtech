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
//		FilterPlugin.h
//
//	Description:
//		FilterPlugin class definition.
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#ifndef __FilterPlugin_H__
#define __FilterPlugin_H__

#include "PIDefines.h"
#include "piabout.h"
#include "pitypes.h"
#include "pifilter.h"
#include "plugin.h"

// make this global in your derived class
extern SPBasicSuite * sSPBasic;


/** A Filter plug in. Derive your filter plug in from this class. Override the 
	virtual functions as necessary.
*/
class FilterPlugin : public Plugin {
private:

	/// No need to define this
	FilterPlugin(const FilterPlugin &); 
	
protected:

	/** Passed in via the Message routine. Derived classes can access this 
		protected member.
	*/
	FilterRecord * filterRecord;

	/// Set up parameters for your plug in
	virtual void Parameters(void) {}

	/// Prepare your plug in
	virtual void Prepare(void) {}

	/** Process the pixels. Use the advanceState procedure to read/write 
		information. Better yet use the Channel Ports Suite.
	*/
	virtual void Start(void) {}

	/** Obsolete.
	    advanceState and ReadStart can read the entire file.
	*/
	virtual void Continue(void) {}

	/// Clean up and write your scripting parameters.
	virtual void Finish(void) {}

public:
	
	FilterPlugin() : Plugin(), filterRecord(NULL) {}
	virtual ~FilterPlugin() {}

	/** The sequence of calls from Photoshop for a filter plug in. Override the 
		virtual functions for each selector in your base class.
	*/
	const bool Message(const int16 selector, void * record);

};

#endif
// end FilterPlugin.h
