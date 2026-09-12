#pragma once
#include "Platform/platform.h"
// Stub ClientFXMgr.h for Linux demos (graphify 5+ demos, any signature via templates)
class CClientFXMgr {
public:
    template<typename... Args> bool Init(Args... ) { return true; }
    template<typename... Args> void Term(Args... ) {}
    template<typename... Args> void SetCamera(Args... ) {}
    template<typename... Args> void Update(Args... ) {}
    template<typename... Args> void Render(Args... ) {}
    template<typename... Args> bool LoadFX(Args... ) { return true; }
    template<typename... Args> void ShutdownAllFX(Args... ) {}
    template<typename... Args> void UpdateAllActiveFX(Args... ) {}
    template<typename... Args> void RenderAllActiveFX(Args... ) {}
    template<typename... Args> void OnRendererShutdown(Args... ) {}
    template<typename... Args> void OnSpecialEffectNotify(Args... ) {}
    template<typename... Args> void OnObjectRemove(Args... ) {}
    template<typename... Args> void AddSpecialEffect(Args... ) {}
    template<typename... Args> void RemoveSpecialEffect(Args... ) {}
    template<typename... Args> bool CreateClientFX(Args... ) { return false; }
    template<typename... Args> void ShutdownClientFX(Args... ) {}
};
extern CClientFXMgr* g_pClientFXMgr;
// Minimal attach-list node (mirrors Engine/Engine/clientfx/Shared/ClientFXMgr.h)
class CClientFXInstance {
public:
    void Show() {}
    void Hide() {}
};
struct CClientFXLink {
    bool IsValid() const { return false; }
    CClientFXInstance* GetInstance() { return nullptr; }
};
struct CLIENTFX_LINK_NODE {
    CLIENTFX_LINK_NODE* m_pNext = nullptr;
    CClientFXLink m_Link;
    CLIENTFX_LINK_NODE() {}
    ~CLIENTFX_LINK_NODE() { DeleteList(); }
    void AddToEnd(CLIENTFX_LINK_NODE* pNode) {
        if (m_pNext) m_pNext->AddToEnd(pNode);
        else m_pNext = pNode;
    }
    void DeleteList() {
        CLIENTFX_LINK_NODE* p = m_pNext; m_pNext = nullptr;
        delete p;
    }
};
