#pragma once
#include "Platform/platform.h"
class CClientFXMgr {
public:
    bool Init(void* p) { (void)p; return true; }
    void Term() {}
    void SetCamera(void* p) { (void)p; }
    void Update(float) {}
    void Render() {}
    bool LoadFX(const char*) { return true; }
};
