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
//		FormatPlugin.h
//
//	Description:
//		FormatPlugin class definition.
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#ifndef __FormatPlugin_H__
#define __FormatPlugin_H__

#include "PIDefines.h"
#include "piabout.h"
#include "pitypes.h"
#include "piformat.h"
#include "plugin.h"


/** A Format plug in. Derive your format plug in from this class.
    Override the virtual functions as necessary.
*/
class FormatPlugin : public Plugin {
private:

	FormatPlugin(FormatPlugin &); // don't define this
	
protected:

	/** Passed in via the Message routine. 
	    Derived classes can access this protected member.
	*/
	FormatRecord * formatRecord;

	/// Prepare your plug in to read this file
	virtual void ReadPrepare(void) {}

	/** Start reading the file. 
	    Use advanceState to read the whole file during this stage.
	*/
	virtual void ReadStart(void) {}

	/** Obsolete.
	    advanceState and ReadStart can read the entire file.
	*/
	virtual void ReadContinue(void) {}

	/// Write scripting parameters and clean up
	virtual void ReadFinish(void) {}

	virtual void OptionsPrepare(void) {}
	virtual void OptionsStart(void) {}
	virtual void OptionsContinue(void) {}
	virtual void OptionsFinish(void) {}

	virtual void EstimatePrepare(void) {}
	virtual void EstimateStart(void) {}
	virtual void EstimateContinue(void) {}
	virtual void EstimateFinish(void) {}

	/// Prepare your plug in to read this file
	virtual void WritePrepare(void) {}

	/** Start writing the file. 
	    Use advanceState to write the whole file during this stage.
	*/
	virtual void WriteStart(void) {}

	/** Obsolete.
	    advanceState and WriteStart can write the entire file.
	*/
	virtual void WriteContinue(void) {}

	/// Write scripting parameters and clean up
	virtual void WriteFinish(void) {}

	/// Do a quick check to determine if you can read this file.
	virtual void FilterFile(void) {}

public:
	
	FormatPlugin() : Plugin(), formatRecord(NULL) {}
	virtual ~FormatPlugin() {}

	/// Process the message based on the selector
	const bool Message(const int16 selector, void * record);

};

#endif
// end FormatPlugin.h
