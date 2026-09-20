#if !defined(AFX_PhotoshopSDK_H__1A1AFD4C_9CB0_49F7_BD66_22E2FF19B519__INCLUDED_)
#define AFX_PhotoshopSDK_H__1A1AFD4C_9CB0_49F7_BD66_22E2FF19B519__INCLUDED_

#include "PIDefines.h"

// Derective for Codewarrior to build the pre-compiled header file
#if (defined(__PIMWCWMacPPC__) && !(defined(MakingPreCompiledHeader)))
	#if _DEBUG
		#include "PhotoshopSDKDebug.ch"
	#else
		#include "PhotoshopSDKRelease.ch"
	#endif
#else

#if MSWindows
// using maps will cause tons and tons of this name is too long warnings
// I'll just make a comment that is really long for you to enjoy
#pragma warning (disable:4786)
#endif

// Common includes
#include <time.h>
#include <stdlib.h>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>

// Operating system includes
#if __PIWin__ // Windows only
	#include <limits.h>
	#include <string.h>
	#include <stdio.h>
	#include <windows.h>
	#include <windowsx.h>
	#include <errno.h>

	typedef HWND DialogPtr;
	typedef HWND DialogTHndl;
#else // Macintosh and others
	#include <Dialogs.h>
	#include <Types.h>
	#include <Gestalt.h>
	#include <TextUtils.h>
	#include <OSUtils.h>
	#include <Menus.h>
	#include <string.h>
#endif

// Sweet Pea includes
#include "SPAccess.h"       
#include "SPAdapts.h"       
#include "SPBasic.h"        
#include "SPBckDbg.h"
#include "SPBlocks.h"       
#include "SPCaches.h"       
#include "SPCOM.h"          
#include "SPConfig.h"
#include "SPErrorCodes.h"   
#include "SPErrors.h"       
#include "SPFiles.h"        
#include "SPHost.h"
#include "SPInterf.h"       
#include "SPMData.h"        
#include "SPObject.h"       
#include "SPPiPL.h"
#include "SPPlugs.h"        
#include "SPProps.h"        
#include "SPRuntme.h"       
#include "SPStrngs.h"
#include "SPSTSPrp.h"       
#include "SPSuites.h"       
#include "SPTypes.h"        

// Photoshop includes
#include "pigeneral.h"                 
#include "piproperties.h"              
#include "pistringterminology.h"
#include "piterminology.h"             
#include "pitypes.h"
#include "piabout.h"                   
#include "piactions.h"                 
#include "piactionsplugin.h"
#include "pibuffersuite.h"             
#include "pichannelportoperations.h"
#include "pichannelportssuite.h"       
#include "picolorspacesuite.h"
#include "pierrorsuite.h"              
#include "piexport.h"
#include "pifilter.h"                  
#include "pigetfilelistsuite.h"
#include "pigetpathsuite.h"            
#include "pihandlesuite.h"
#include "pipicker.h"                  
#include "piprogresssuite.h"
#include "piselection.h"               
#include "piuihookssuite.h"   
#ifdef __PIWin__
	#ifndef AliasHandle
		typedef Handle					AliasHandle;
	#endif
#endif
#include "pialiassuite.h"         
#include "aszstringsuite.h"

// SDK includes
#include "DialogUtilities.h"
#include "FileUtilities.h"       
#include "FilterBigDocument.h"
#include "PICA2PSErrorMap.h"     
#ifdef _PIWin__
	#include "PIDLLInstance.h"       
#endif

#include "PIUActions.h"          
#include "PIUActionUtils.h"
#include "PIUFile.h"             
#include "PIUGet.h"              
#include "PIUI.h"
#include "PIUNew.h"              
#include "PIUSelect.h"           
#include "PIUSuites.h"
#include "PIUtilities.h"
#include "PropertyUtils.h"

#include "plugin.h"

#endif // #if (defined(__PIMWCWMacPPC__) && !(defined(MakingPreCompiledHeader)))
#endif //  !defined(AFX_PhotoshopSDK_H__1A1AFD4C_9CB0_49F7_BD66_22E2FF19B519__INCLUDED_)