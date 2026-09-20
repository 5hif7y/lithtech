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
//-------------------------------------------------------------------------------
//
//	File:
//		Plugin.h
//
//	Description:
//		Base class definition for Plugin. This is the base class for FilterPlugin
//		and all the other plug-in types. You need to derive from the type of
//		plug-in you are developing for.
//
//
//-------------------------------------------------------------------------------
#ifndef __Plugin_h__
#define __Plugin_h__

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#include "PIDefines.h"
#include "pitypes.h"
#include "piabout.h"
#include "PIUI.h"


//-------------------------------------------------------------------------------
//	Classes
//-------------------------------------------------------------------------------

/** Base class. See plug in types derived from this class for all 
    Standard Photoshop plugins if you want an Automation plugin see
	the AutomationPlugin class
	@see AutomationPlugin
*/
class Plugin {
private:
	
	/// Release the suites needed by the derived class
	void ReleaseSuites(void);

protected:

	/// error value for return to Photoshop
	int16 error;

	/// Show the about box, return Done or NotDone
	virtual const bool About(AboutRecord * aboutRecord);

public:
	Plugin();
	virtual ~Plugin();

	/// Process the message based on the selector, return Done or NotDone
	virtual const bool Message(const int16 selector, void * record) = 0;

	/// Return the error value to Photoshop
	const int16 GetError(void) { return error; }

};

/// Globally defined variables in your derived class
extern SPBasicSuite * sSPBasic;

extern const bool Done;
extern const bool NotDone;

/** The sSPBasic pointer is sacred and we need to make sure it is set before
	anything else in our plug in gets executed. It is needed by the new/delete
	operators and the Acquire/Release functions in the Plugin base class.
*/
extern SPBasicSuite * GetSPBasic(const int16 selector, void * record);

/// Your plug in must supply this routine. Allocates your derived class.
extern Plugin * AllocatePlugin(void);

/// Definition of the main entrypoint for all standard plugins
DLLExport MACPASCAL void PluginMain(const int16 selector,
								    void * record,
								    int32 * data,
								    int16 * result);

/// Definition of the main entrypoint for all automation plugins
DLLExport SPAPI SPErr AutoPluginMain(const char * caller, 
                                     const char * selector, 
                                     void * message);

#endif
// end Plugin.h