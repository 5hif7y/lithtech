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
//	Includes 
//-------------------------------------------------------------------------------
#ifndef __TEXTAUTO_H__
#define __TEXTAUTO_H__

#include "PIDefines.h"
#include "SPHost.h"
#include "PIUGet.h"
#include "PIUSelect.h"
#include "piactionsplugin.h"
#include "piuihookssuite.h"
#include "pihandlesuite.h"
#include "AutomationPlugin.h"

#include "aszstringsuite.h"
#include "PIUSuites.h"

#if __PIMac__
	#include <string.h>
#endif
#include <vector>
#include <time.h>

using namespace std;

class TextAutoPlugin : public AutomationPlugin {

private:

	bool fNotifierOn;

	void MakeTextLayer(string fullPath, VRect rect);
	
	void DeleteBackground(void);

	virtual ASErr StartupPlugin(SPInterfaceMessage * message);

	virtual ASErr ShutdownPlugin(SPInterfaceMessage * message);

public:

	TextAutoPlugin(SPPluginRef pluginRef);

	~TextAutoPlugin() {}

	void OpenEvent(DescriptorEventID event,
				   PIActionDescriptor descriptor,
				   PIDialogRecordOptions options);
};
#endif
// end TextAuto.h

