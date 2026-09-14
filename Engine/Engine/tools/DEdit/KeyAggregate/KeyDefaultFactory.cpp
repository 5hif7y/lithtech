#include "bdefs.h"
#include "KeyDefaultFactory.h"
#include "KeyDefaultAggregate.h"
#include "resource.h"
#include "OptionsControls.h"
#include "EditHelpers.h"

//the actual defaults
#include "MaxKeyAggregate.h"
#include "MayaKeyAggregate.h"
#include "DEditKeyAggregate.h"

CKeyDefaultAggregate* CKeyDefaultFactory::CreateDefault(const char* pszName)
{
	//see if it matches a default
	if(stricmp(pszName, "MAX") == 0)
	{
		return new CMaxKeyAggregate;
	}
	else if(stricmp(pszName, "Maya") == 0)
	{
		return new CMayaKeyAggregate;
	}
	else if(stricmp(pszName, "DEdit") == 0)
	{
		return new CDEditKeyAggregate;
	}

	//no match
	return NULL;
}

CString CKeyDefaultFactory::GetDefaultText(const char* pszName)
{
	CString rv;

	//see if it matches a default
	if(stricmp(pszName, "MAX") == 0)
	{
		rv.LoadString(IDS_MAX_STYLE_DESCRIPTION);
	}
	else if(stricmp(pszName, "Maya") == 0)
	{
		rv.LoadString(IDS_MAYA_STYLE_DESCRIPTION);
	}
	else if(stricmp(pszName, "DEdit") == 0)
	{
		rv.LoadString(IDS_DEDIT_STYLE_DESCRIPTION);
	}
	//no match
	else
	{
		rv.LoadString(IDS_DEFAULT_STYLE_DESCRIPTION);
	}

	return rv;
}

//this allows each configuration to update the global options. This should
//really only be called when someone switches an input style
void CKeyDefaultFactory::UpdateGlobalOptions(const char* pszName)
{
	COptionsControls* pControls = GetApp()->GetOptions().GetControlsOptions();

	//for each style, set the appropriate settings
	if(stricmp(pszName, "MAX") == 0)
	{
		pControls->SetOrbitAroundSel(true);
		pControls->SetZoomToCursor(true);
	}
	else if(stricmp(pszName, "Maya") == 0)
	{
	}
	else if(stricmp(pszName, "DEdit") == 0)
	{
		pControls->SetOrbitAroundSel(false);
		pControls->SetZoomToCursor(true);
	}
}
