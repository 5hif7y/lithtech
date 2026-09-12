#pragma once
#include "Platform/platform.h"
#include <type_traits>
#include <iltwidgetmanager.h>
class HostWidgetManager : public ILTWidgetManager {
public:
CUI_RESULTTYPE SetCurrentSkin(HTEXTURE hTex) override { return CUI_RESULTTYPE(); }
HTEXTURE GetCurrentSkin() override { return HTEXTURE(); }
void Init(uint16 scrWidth, uint16 scrHeight) override {}
void Term() override {}
bool IsInitted() override { return false; }
CUIBase* CreateWidget(CUIBase* pParent, CUI_WIDGETTYPE type, CUIRECT* pRect, CUIBase* pRelativeTo, uint32 flags) override { return nullptr; }
CUI_RESULTTYPE DestroyWidget(CUIBase* pWidget) override { return CUI_RESULTTYPE(); }
CUIBase* GetRootWidget() override { return nullptr; }
CUIBase* GetWidgetAtCoords(int16 x, int16 y) override { return nullptr; }
CUI_RESULTTYPE SetFocusWidget(CUIBase* pWidget) override { return CUI_RESULTTYPE(); }
CUIBase* GetFocusWidget() override { return nullptr; }
bool IsWidgetValid(CUIBase* pWidget) override { return false; }
void Update() override {}
CUI_RESULTTYPE EnqueueMessage(CUIMessage* pMessage) override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE DequeueMessage(CUIMessage* pMessage) override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE PeekMessage(CUIMessage* pMessage) override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE SetUserCallback(CUI_CALLBACKTYPE pUserCallback, void* pUserCallbackObject, void* pUserCallbackData) override { return CUI_RESULTTYPE(); }
CUI_CALLBACKTYPE GetUserCallback(void** ppUserCallbackObject, void** ppUserCallbackData) override { return CUI_CALLBACKTYPE(); }
void* GetUserCallbackObject() override { return nullptr; }
void* GetUserCallbackData() override { return nullptr; }
CUI_RESULTTYPE FocusNext() override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE FocusUp() override { return CUI_RESULTTYPE(); }
CUI_RESULTTYPE FocusDown() override { return CUI_RESULTTYPE(); }
const char* _InterfaceImplementation() override { return ""; }
};
