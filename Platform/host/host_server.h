#pragma once
// Host server-side engine (Fase B): object table, ClassDef factory,
// ILTServer/ILTCommon/ILTModel/ILTSoundMgr implementations, prop bags.
//
// Included ONLY by host_sealhunter.cpp (single TU: out-of-line ILTModel
// base fallbacks live here without ODR risk).
#include <iltserver.h>
#include <ltserverobj.h>
#include <iservershell.h>
#include <iltcommon.h>
#include <iltmodel.h>
#include <iltsoundmgr.h>
#include <ltobjectcreate.h>
#include "serverinterfaces.h"
#include "ltservershell.h"
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cstdlib>

namespace Host {

// ---- object table ----
struct ServerObj {
    LPBASECLASS obj = nullptr;
    ClassDef* cls = nullptr;
    LTVector pos;
    LTRotation rot;
    LTVector scale;
    float nextUpdate = 0.0f;
    bool useTimer = false;
    bool active = true;
    int state = 0;
    uint8 blockPri = 0;
    uint32 id = 0;
    std::string name;
    std::map<std::string, std::string> props;
    ServerObj() { pos.Init(); rot.Init(); scale.Init(1.0f, 1.0f, 1.0f); }
};

struct ServerWorld {
    std::vector<std::unique_ptr<ServerObj>> objs;
    ServerObj* current = nullptr; // prop context during MID_PRECREATE
    ServerObj worldSentinel;
    float time = 0.0f;
    float frameDt = 1.0f / 60.0f;
    uint32 totalTicks = 0;
    bool clientEntered = false;
    uint32 nextId = 1;
    void* clientUserData = nullptr;
    std::string sessionName = "SealHunter";
    uint32 animSeq = 1;
    static ServerWorld& instance() {
        static ServerWorld w;
        return w;
    }
    static HOBJECT toH(const ServerObj* o) {
        return reinterpret_cast<HOBJECT>(const_cast<ServerObj*>(o));
    }
    static ServerObj* fromH(HOBJECT h) {
        ServerWorld& w = instance();
        for (auto& o : w.objs)
            if (o.get() == reinterpret_cast<ServerObj*>(h) && o->active) return o.get();
        if (reinterpret_cast<ServerObj*>(h) == &w.worldSentinel) return &w.worldSentinel;
        return nullptr;
    }
    static ServerObj* fromHL(HLOCALOBJ h) {
        return fromH(reinterpret_cast<HOBJECT>(h));
    }
};

// ---- ClassDef factory ----
inline ClassDef* FindClassDef(const char* name) {
    if (!name) return nullptr;
    for (__ClassDefiner* d = __g_ClassDefinerHead; d; d = d->m_pNext) {
        if (d->m_pClass && d->m_pClass->m_ClassName && !strcmp(d->m_pClass->m_ClassName, name))
            return d->m_pClass;
    }
    return nullptr;
}

// ---- ILTServer function-pointer members (static impls) ----
static HCLASS S_GetClass(const char* pName) {
    return reinterpret_cast<HCLASS>(FindClassDef(pName));
}
static LPBASECLASS S_CreateObject(HCLASS hClass, ObjectCreateStruct* pStruct);
static LPBASECLASS S_CreateObjectProps(HCLASS hClass, ObjectCreateStruct* pStruct, const char* pszProps) {
    (void)pszProps;
    return S_CreateObject(hClass, pStruct);
}
static const char* S_PropLookup(const char* pPropName, std::string& out) {
    ServerObj* cur = ServerWorld::instance().current;
    if (!pPropName || !cur) return nullptr;
    auto it = cur->props.find(pPropName);
    if (it == cur->props.end()) return nullptr;
    out = it->second;
    return out.c_str();
}
static LTRESULT S_GetPropString(const char* pPropName, char* pRet, int maxLen) {
    std::string v;
    if (!S_PropLookup(pPropName, v) || !pRet || maxLen <= 0) return LT_NOTFOUND;
    strncpy(pRet, v.c_str(), (size_t)(maxLen - 1));
    pRet[maxLen - 1] = 0;
    return LT_OK;
}
static LTRESULT S_GetPropReal(const char* pPropName, float* pRet) {
    std::string v;
    if (!S_PropLookup(pPropName, v) || !pRet) return LT_NOTFOUND;
    *pRet = (float)atof(v.c_str());
    return LT_OK;
}
static LTRESULT S_GetPropVector(const char* pPropName, LTVector* pRet) {
    std::string v;
    if (!S_PropLookup(pPropName, v) || !pRet) return LT_NOTFOUND;
    float x = 0, y = 0, z = 0;
    if (sscanf(v.c_str(), "%f %f %f", &x, &y, &z) != 3) return LT_NOTFOUND;
    pRet->Init(x, y, z);
    return LT_OK;
}
static LTRESULT S_GetPropColor(const char* pPropName, LTVector* pRet) {
    return S_GetPropVector(pPropName, pRet);
}
static LTRESULT S_GetPropFlags(const char* pPropName, uint32* pRet) {
    std::string v;
    if (!S_PropLookup(pPropName, v) || !pRet) return LT_NOTFOUND;
    *pRet = (uint32)strtoul(v.c_str(), nullptr, 0);
    return LT_OK;
}
static LTRESULT S_GetPropBool(const char* pPropName, bool* pRet) {
    std::string v;
    if (!S_PropLookup(pPropName, v) || !pRet) return LT_NOTFOUND;
    *pRet = (v == "1" || v == "true" || v == "TRUE");
    return LT_OK;
}
static LTRESULT S_GetPropLongInt(const char* pPropName, int32* pRet) {
    std::string v;
    if (!S_PropLookup(pPropName, v) || !pRet) return LT_NOTFOUND;
    *pRet = (int32)strtol(v.c_str(), nullptr, 0);
    return LT_OK;
}
static LTRESULT S_GetPropRotation(const char* pPropName, LTRotation* pRet) {
    (void)pPropName; (void)pRet;
    return LT_NOTFOUND;
}
static LTRESULT S_GetPropRotationEuler(const char* pPropName, LTVector* pAngles) {
    return S_GetPropVector(pPropName, pAngles);
}
static LTRESULT S_GetPropGeneric(const char* pPropName, GenericProp* pProp) {
    (void)pPropName; (void)pProp;
    return LT_NOTFOUND;
}
static LTRESULT S_DoesPropExist(const char* pPropName, int32* pPropType) {
    std::string v;
    if (!S_PropLookup(pPropName, v)) return LT_NOTFOUND;
    if (pPropType) *pPropType = 0;
    return LT_OK;
}
static void S_SetNextUpdate(HOBJECT hObj, LTFLOAT nextUpdate) {
    ServerObj* o = ServerWorld::fromH(hObj);
    if (!o) return;
    o->nextUpdate = ServerWorld::instance().time + nextUpdate;
    o->useTimer = true;
}
static HOBJECT S_ObjectToHandle(LPBASECLASS pObject) {
    if (!pObject) return nullptr;
    for (auto& o : ServerWorld::instance().objs)
        if (o->obj == pObject && o->active) return ServerWorld::toH(o.get());
    return nullptr;
}
static LPBASECLASS S_HandleToObject(HOBJECT hObject) {
    ServerObj* o = ServerWorld::fromH(hObject);
    return o ? o->obj : nullptr;
}
static void S_SetObjectPos(HOBJECT hObj, const LTVector* pos) {
    ServerObj* o = ServerWorld::fromH(hObj);
    if (o && pos) o->pos = *pos;
}
static LTRESULT S_SetObjectRotation(HOBJECT hObj, const LTRotation* pRotation) {
    ServerObj* o = ServerWorld::fromH(hObj);
    if (!o || !pRotation) return LT_INVALIDPARAMS;
    o->rot = *pRotation;
    return LT_OK;
}
static LTRESULT S_RotateObject(HOBJECT hObj, const LTRotation* pRotation) {
    return S_SetObjectRotation(hObj, pRotation);
}
static void S_TiltToPlane(HOBJECT hObj, const LTVector* pNormal) {
    (void)hObj; (void)pNormal;
}
static LTRESULT S_TeleportObject(HOBJECT hObj, const LTVector* pNewPos) {
    S_SetObjectPos(hObj, pNewPos);
    return LT_OK;
}
static LTRESULT S_GetLastCollision(CollisionInfo* pInfo) {
    (void)pInfo;
    return LT_NOTFOUND;
}
// Ground plane y=0: seals fall until they hit it (no world geometry loaded).
static bool S_IntersectSegment(IntersectQuery* pQuery, IntersectInfo* pInfo) {
    if (!pQuery || !pInfo) return false;
    float fy = pQuery->m_From.y, ty = pQuery->m_To.y;
    if ((fy > 0.0f && ty > 0.0f) || (fy < 0.0f && ty < 0.0f)) return false;
    float denom = fy - ty;
    if (denom == 0.0f) return false;
    float t = fy / denom;
    pInfo->m_Point.x = pQuery->m_From.x + (pQuery->m_To.x - pQuery->m_From.x) * t;
    pInfo->m_Point.y = 0.0f;
    pInfo->m_Point.z = pQuery->m_From.z + (pQuery->m_To.z - pQuery->m_From.z) * t;
    pInfo->m_hObject = ServerWorld::toH(&ServerWorld::instance().worldSentinel);
    return true;
}
static bool S_CastRay(IntersectQuery* pQuery, IntersectInfo* pInfo) {
    return S_IntersectSegment(pQuery, pInfo);
}
static ObjectList* S_CreateObjectList() { return nullptr; }
static ObjectLink* S_AddObjectToList(ObjectList* pList, HOBJECT hObj) {
    (void)pList; (void)hObj;
    return nullptr;
}
static void S_RemoveObjectFromList(ObjectList* pList, HOBJECT hObj) {
    (void)pList; (void)hObj;
}
static void S_RelinquishList(ObjectList* pList) { (void)pList; }
static ObjectList* S_GetPointAreas(const LTVector* pPoint) {
    (void)pPoint;
    return nullptr;
}
static ObjectList* S_GetBoxIntersecters(const LTVector* pMin, const LTVector* pMax) {
    (void)pMin; (void)pMax;
    return nullptr;
}
static ObjectList* S_FindObjectsTouchingSphere(const LTVector* pPosition, float radius) {
    (void)pPosition; (void)radius;
    return nullptr;
}
static HOBJECT S_GetNextObject(HOBJECT hObj) {
    auto& objs = ServerWorld::instance().objs;
    bool retNext = (hObj == nullptr);
    for (auto& o : objs) {
        if (!o->active) continue;
        if (retNext) return ServerWorld::toH(o.get());
        if (ServerWorld::toH(o.get()) == hObj) retNext = true;
    }
    return nullptr;
}
static HOBJECT S_GetNextInactiveObject(HOBJECT hObj) {
    (void)hObj;
    return nullptr;
}
static LTRESULT S_GetObjectScale(HOBJECT hObj, LTVector* pScale) {
    ServerObj* o = ServerWorld::fromH(hObj);
    if (!o || !pScale) return LT_INVALIDPARAMS;
    *pScale = o->scale;
    return LT_OK;
}
static void S_ScaleObject(HOBJECT hObj, const LTVector* pNewScale) {
    ServerObj* o = ServerWorld::fromH(hObj);
    if (o && pNewScale) o->scale = *pNewScale;
}
static void S_SetObjectState(HOBJECT hObj, int state) {
    ServerObj* o = ServerWorld::fromH(hObj);
    if (o) o->state = state;
}
static int S_GetObjectState(HOBJECT hObj) {
    ServerObj* o = ServerWorld::fromH(hObj);
    return o ? o->state : 0;
}
static void S_SetBlockingPriority(HOBJECT hObj, uint8 pri) {
    ServerObj* o = ServerWorld::fromH(hObj);
    if (o) o->blockPri = pri;
}
static uint8 S_GetBlockingPriority(HOBJECT hObj) {
    ServerObj* o = ServerWorld::fromH(hObj);
    return o ? o->blockPri : 0;
}
static LTRESULT S_ClipSprite(HOBJECT hObj, HPOLY hPoly) {
    (void)hObj; (void)hPoly;
    return LT_OK;
}
static void S_GetLightColor(HOBJECT hObj, float* r, float* g, float* b) {
    (void)hObj;
    if (r) *r = 1.0f; if (g) *g = 1.0f; if (b) *b = 1.0f;
}
static void S_SetLightColor(HOBJECT hObj, float r, float g, float b) {
    (void)hObj; (void)r; (void)g; (void)b;
}
static float S_GetLightRadius(HOBJECT hObj) {
    (void)hObj;
    return 0.0f;
}
static void S_SetLightRadius(HOBJECT hObj, float radius) {
    (void)hObj; (void)radius;
}
static LTRESULT S_UpdateSessionName(const char* sName) {
    if (sName) ServerWorld::instance().sessionName = sName;
    return LT_OK;
}
static LTRESULT S_GetSessionName(char* sName, uint32 dwBufferSize) {
    if (!sName || dwBufferSize == 0) return LT_INVALIDPARAMS;
    strncpy(sName, ServerWorld::instance().sessionName.c_str(), dwBufferSize - 1);
    sName[dwBufferSize - 1] = 0;
    return LT_OK;
}
static LTRESULT S_SetModelNodeHideStatus(HOBJECT hObj, char* pNodeName, bool bHidden) {
    (void)hObj; (void)pNodeName; (void)bHidden;
    return LT_OK;
}
static LTRESULT S_GetModelNodeHideStatus(HOBJECT hObj, char* pNodeName, bool* bHidden) {
    (void)hObj; (void)pNodeName;
    if (bHidden) *bHidden = false;
    return LT_OK;
}
static const char* S_GetAnimName(HOBJECT hObject, HMODELANIM hAnim) {
    (void)hObject; (void)hAnim;
    return "";
}
static void S_SetModelPlaying(HOBJECT hObj, bool bPlaying) {
    (void)hObj; (void)bPlaying;
}
static bool S_GetModelPlaying(HOBJECT hObj) {
    (void)hObj;
    return false;
}
static bool S_GetPointShade(const LTVector* pPoint, LTVector* pColor) {
    (void)pPoint;
    if (pColor) pColor->Init(1.0f, 1.0f, 1.0f);
    return false;
}
static LTRESULT S_GetTcpIpAddress(char* sAddress, uint32 dwBufferSize, uint16& hostPort) {
    if (sAddress && dwBufferSize > 8) strcpy(sAddress, "127.0.0.1");
    hostPort = 0;
    return LT_OK;
}
static void S_StartCounter(LTCounter* pCounter) { (void)pCounter; }
static HCLIENT S_GetNextClient(HCLIENT hPrev) {
    (void)hPrev;
    return nullptr;
}
static HCLIENTREF S_GetNextClientRef(HCLIENTREF hRef) {
    (void)hRef;
    return nullptr;
}
static uint32 S_GetClientRefInfoFlags(HCLIENTREF hClient) {
    (void)hClient;
    return 0;
}
static bool S_GetClientRefName(HCLIENTREF hClient, char* pName, int maxLen) {
    (void)hClient;
    if (pName && maxLen > 0) pName[0] = 0;
    return false;
}
static HOBJECT S_GetClientRefObject(HCLIENTREF hClient) {
    (void)hClient;
    return nullptr;
}
static HCLIENT S_GetClientHandle(uint32 clientID) {
    (void)clientID;
    return reinterpret_cast<HCLIENT>((intptr_t)1);
}
static uint32 S_GetClientID(HCLIENT hClient) {
    (void)hClient;
    return 0;
}
static bool S_GetClientName(HCLIENT hClient, char* pName, int maxLen) {
    (void)hClient;
    if (pName && maxLen > 7) strcpy(pName, "Player");
    return true;
}
static bool S_SetClientName(HCLIENT hClient, const char* pName, int MaxLen) {
    (void)hClient; (void)pName; (void)MaxLen;
    return true;
}
static void S_SetClientInfoFlags(HCLIENT hClient, uint32 dwClientFlags) {
    (void)hClient; (void)dwClientFlags;
}
static uint32 S_GetClientInfoFlags(HCLIENT hClient) {
    (void)hClient;
    return 0;
}
static void S_SetClientUserData(HCLIENT hClient, void* pData) {
    (void)hClient;
    ServerWorld::instance().clientUserData = pData;
}
static void* S_GetClientUserData(HCLIENT hClient) {
    (void)hClient;
    return ServerWorld::instance().clientUserData;
}
static LTRESULT S_KickClient(HCLIENT hClient) {
    (void)hClient;
    return LT_OK;
}
static LTRESULT S_SetClientViewPos(HCLIENT hClient, const LTVector* pPos) {
    (void)hClient; (void)pPos;
    return LT_OK;
}
static LTRESULT S_AttachClient(HCLIENT hParent, HCLIENT hChild) {
    (void)hParent; (void)hChild;
    return LT_OK;
}
static LTRESULT S_DetachClient(HCLIENT hClient) {
    (void)hClient;
    return LT_OK;
}
static void S_RunGameConString(const char* pString) { (void)pString; }
static void S_SetGameConVar(const char* pName, const char* pVal) {
    (void)pName; (void)pVal;
}
static HCONVAR S_GetGameConVar(const char* pName) {
    (void)pName;
    return nullptr;
}
static bool S_UpperStrcmp(const char* pStr1, const char* pStr2) {
    (void)pStr1; (void)pStr2;
    return false;
}
static int S_Parse(const char* pCommand, const char** pNewCommandPos, char* argBuffer, char** argPointers, int* nArgs) {
    (void)pCommand; (void)pNewCommandPos; (void)argBuffer; (void)argPointers; (void)nArgs;
    return 0;
}
static LTRESULT S_CreateInterObjectLink(HOBJECT hOwner, HOBJECT hLinked) {
    (void)hOwner; (void)hLinked;
    return LT_OK;
}
static LTRESULT S_CreateAttachment(HOBJECT hParent, HOBJECT hChild, const char* pSocketName, LTVector* pOffset, LTRotation* pRotationOffset, HATTACHMENT* pAttachment) {
    (void)hParent; (void)hChild; (void)pSocketName; (void)pOffset; (void)pRotationOffset;
    if (pAttachment) *pAttachment = nullptr;
    return LT_ERROR;
}
static LTRESULT S_RemoveAttachment(HATTACHMENT hAttachment) {
    (void)hAttachment;
    return LT_OK;
}
static LTRESULT S_FindAttachment(HOBJECT hParent, HOBJECT hChild, HATTACHMENT* hAttachment) {
    (void)hParent; (void)hChild;
    if (hAttachment) *hAttachment = nullptr;
    return LT_ERROR;
}
static bool S_GetObjectColor(HOBJECT hObject, float* r, float* g, float* b, float* a) {
    (void)hObject;
    if (r) *r = 1.0f; if (g) *g = 1.0f; if (b) *b = 1.0f; if (a) *a = 1.0f;
    return false;
}
static bool S_SetObjectColor(HOBJECT hObject, float r, float g, float b, float a) {
    (void)hObject; (void)r; (void)g; (void)b; (void)a;
    return false;
}
static LTFLOAT S_Random(LTFLOAT lo, LTFLOAT hi) {
    if (hi <= lo) return lo;
    return lo + (hi - lo) * (rand() / (float)RAND_MAX);
}
static int S_IntRandom(int lo, int hi) {
    if (hi <= lo) return lo;
    return lo + rand() % (hi - lo + 1);
}
static LTFLOAT S_RandomScale(LTFLOAT scale) {
    return S_Random(0.0f, scale);
}
static void S_DebugOut(const char* pMsg, ...) {
    char buf[1024];
    va_list ap;
    va_start(ap, pMsg);
    vsnprintf(buf, sizeof(buf) - 1, pMsg, ap);
    va_end(ap);
    buf[sizeof(buf) - 1] = 0;
    printf("[srv] %s", buf);
    fflush(stdout);
}

// ---- CreateObject flow ----
static LPBASECLASS S_CreateObject(HCLASS hClass, ObjectCreateStruct* pStruct) {
    ClassDef* cd = reinterpret_cast<ClassDef*>(hClass);
    if (!cd || !cd->m_ConstructFn || cd->m_ClassObjectSize <= 0 || !pStruct) return nullptr;
    void* mem = new char[(size_t)cd->m_ClassObjectSize];
    cd->m_ConstructFn(mem);
    LPBASECLASS obj = reinterpret_cast<LPBASECLASS>(mem);
    auto so = std::unique_ptr<ServerObj>(new ServerObj());
    so->obj = obj;
    so->cls = cd;
    so->pos = pStruct->m_Pos;
    so->rot = pStruct->m_Rotation;
    ServerObj* raw = so.get();
    ServerWorld::instance().objs.push_back(std::move(so));
    obj->SetHOBJECT(reinterpret_cast<HOBJECT>(raw));
    ServerWorld::instance().current = raw;
    obj->EngineMessageFn(MID_PRECREATE, pStruct, PRECREATE_WORLDFILE);
    obj->EngineMessageFn(MID_OBJECTCREATED, nullptr, 0.0f);
    ServerWorld::instance().current = nullptr;
    return obj;
}

// ---- HostILTServer ----
class HostILTServer : public ILTServer {
public:
    const char* _InterfaceImplementation() override { return ""; }
    void apply() {
        GetClass = &S_GetClass;
        CreateObject = &S_CreateObject;
        CreateObjectProps = &S_CreateObjectProps;
        GetPropString = &S_GetPropString;
        GetPropVector = &S_GetPropVector;
        GetPropColor = &S_GetPropColor;
        GetPropReal = &S_GetPropReal;
        GetPropFlags = &S_GetPropFlags;
        GetPropBool = &S_GetPropBool;
        GetPropLongInt = &S_GetPropLongInt;
        GetPropRotation = &S_GetPropRotation;
        GetPropRotationEuler = &S_GetPropRotationEuler;
        GetPropGeneric = &S_GetPropGeneric;
        DoesPropExist = &S_DoesPropExist;
        SetNextUpdate = &S_SetNextUpdate;
        ObjectToHandle = &S_ObjectToHandle;
        HandleToObject = &S_HandleToObject;
        SetObjectPos = &S_SetObjectPos;
        SetObjectRotation = &S_SetObjectRotation;
        RotateObject = &S_RotateObject;
        TiltToPlane = &S_TiltToPlane;
        TeleportObject = &S_TeleportObject;
        GetLastCollision = &S_GetLastCollision;
        IntersectSegment = &S_IntersectSegment;
        CastRay = &S_CastRay;
        CreateObjectList = &S_CreateObjectList;
        AddObjectToList = &S_AddObjectToList;
        RemoveObjectFromList = &S_RemoveObjectFromList;
        RelinquishList = &S_RelinquishList;
        GetPointAreas = &S_GetPointAreas;
        GetBoxIntersecters = &S_GetBoxIntersecters;
        FindObjectsTouchingSphere = &S_FindObjectsTouchingSphere;
        GetNextObject = &S_GetNextObject;
        GetNextInactiveObject = &S_GetNextInactiveObject;
        GetObjectScale = &S_GetObjectScale;
        ScaleObject = &S_ScaleObject;
        SetObjectState = &S_SetObjectState;
        GetObjectState = &S_GetObjectState;
        SetBlockingPriority = &S_SetBlockingPriority;
        GetBlockingPriority = &S_GetBlockingPriority;
        ClipSprite = &S_ClipSprite;
        GetLightColor = &S_GetLightColor;
        SetLightColor = &S_SetLightColor;
        GetLightRadius = &S_GetLightRadius;
        SetLightRadius = &S_SetLightRadius;
        UpdateSessionName = &S_UpdateSessionName;
        GetSessionName = &S_GetSessionName;
        SetModelNodeHideStatus = &S_SetModelNodeHideStatus;
        GetModelNodeHideStatus = &S_GetModelNodeHideStatus;
        GetAnimName = &S_GetAnimName;
        SetModelPlaying = &S_SetModelPlaying;
        GetModelPlaying = &S_GetModelPlaying;
        GetPointShade = &S_GetPointShade;
        GetTcpIpAddress = &S_GetTcpIpAddress;
        StartCounter = &S_StartCounter;
        GetNextClient = &S_GetNextClient;
        GetNextClientRef = &S_GetNextClientRef;
        GetClientRefInfoFlags = &S_GetClientRefInfoFlags;
        GetClientRefName = &S_GetClientRefName;
        GetClientRefObject = &S_GetClientRefObject;
        GetClientHandle = &S_GetClientHandle;
        GetClientID = &S_GetClientID;
        GetClientName = &S_GetClientName;
        SetClientName = &S_SetClientName;
        SetClientInfoFlags = &S_SetClientInfoFlags;
        GetClientInfoFlags = &S_GetClientInfoFlags;
        SetClientUserData = &S_SetClientUserData;
        GetClientUserData = &S_GetClientUserData;
        KickClient = &S_KickClient;
        SetClientViewPos = &S_SetClientViewPos;
        AttachClient = &S_AttachClient;
        DetachClient = &S_DetachClient;
        RunGameConString = &S_RunGameConString;
        SetGameConVar = &S_SetGameConVar;
        GetGameConVar = &S_GetGameConVar;
        UpperStrcmp = &S_UpperStrcmp;
        Parse = &S_Parse;
        CreateInterObjectLink = &S_CreateInterObjectLink;
        CreateAttachment = &S_CreateAttachment;
        RemoveAttachment = &S_RemoveAttachment;
        FindAttachment = &S_FindAttachment;
        GetObjectColor = &S_GetObjectColor;
        SetObjectColor = &S_SetObjectColor;
        Random = &S_Random;
        IntRandom = &S_IntRandom;
        RandomScale = &S_RandomScale;
        DebugOut = &S_DebugOut;
    }
    // ILTCSBase (instance accessors defined after ServerIfaces, below)
    ILTCommon* Common() override;
    ILTPhysics* Physics() override;
    ILTTransform* GetTransformLT() override;
    ILTModel* GetModelLT() override;
    ILTSoundMgr* SoundMgr() override;
    void CPrint(const char* pMsg, ...) override {
        char buf[1024];
        va_list ap;
        va_start(ap, pMsg);
        vsnprintf(buf, sizeof(buf) - 1, pMsg, ap);
        va_end(ap);
        buf[sizeof(buf) - 1] = 0;
        printf("[srv] %s", buf);
        fflush(stdout);
    }
    bool GetContainerCode(HOBJECT hObj, uint16* pCode) override {
        (void)hObj;
        if (pCode) *pCode = 0;
        return false;
    }
    uint32 GetObjectContainers(HOBJECT hObj, HOBJECT* pContainerList, uint32 maxListSize) override {
        (void)hObj; (void)pContainerList; (void)maxListSize;
        return 0;
    }
    uint32 GetContainedObjects(HOBJECT hContainer, HOBJECT* pObjectList, uint32 maxListSize) override {
        (void)hContainer; (void)pObjectList; (void)maxListSize;
        return 0;
    }
    LTRESULT OpenFile(const char* pFilename, ILTStream** pStream) override {
        (void)pFilename;
        if (pStream) *pStream = nullptr;
        return LT_ERROR;
    }
    LTRESULT CopyFile(const char* pszSourceFile, const char* pszDestFile) override {
        (void)pszSourceFile; (void)pszDestFile;
        return LT_ERROR;
    }
    LTRESULT OpenMemoryStream(ILTStream** pStream, uint32 nCacheSize) override {
        (void)pStream; (void)nCacheSize;
        return LT_ERROR;
    }
    LTRESULT GetBlindObjectData(uint32 nNum, uint32 nId, uint8*& pData, uint32& nSize) override {
        (void)nNum; (void)nId; pData = nullptr; nSize = 0;
        return LT_ERROR;
    }
    LTRESULT FreeBlindObjectData(uint32 nNum, uint32 nId) override {
        (void)nNum; (void)nId;
        return LT_OK;
    }
    HSTRING FormatString(int messageCode, ...) override {
        (void)messageCode;
        char* s = (char*)malloc(1);
        if (s) s[0] = 0;
        return reinterpret_cast<HSTRING>(s);
    }
    HSTRING CopyString(HSTRING hString) override {
        const char* s = reinterpret_cast<const char*>(hString);
        char* c = (char*)malloc(s ? strlen(s) + 1 : 1);
        if (c) strcpy(c, s ? s : "");
        return reinterpret_cast<HSTRING>(c);
    }
    HSTRING CreateString(const char* pString) override {
        char* c = (char*)malloc(pString ? strlen(pString) + 1 : 1);
        if (c) strcpy(c, pString ? pString : "");
        return reinterpret_cast<HSTRING>(c);
    }
    void FreeString(HSTRING hString) override {
        free(reinterpret_cast<void*>(hString));
    }
    bool CompareStrings(HSTRING hString1, HSTRING hString2) override {
        return strcmp(reinterpret_cast<const char*>(hString1),
                      reinterpret_cast<const char*>(hString2)) == 0;
    }
    bool CompareStringsUpper(HSTRING hString1, HSTRING hString2) override {
#ifdef _WIN32
        return _stricmp(reinterpret_cast<const char*>(hString1),
                        reinterpret_cast<const char*>(hString2)) == 0;
#else
        return strcasecmp(reinterpret_cast<const char*>(hString1),
                          reinterpret_cast<const char*>(hString2)) == 0;
#endif
    }
    const char* GetStringData(HSTRING hString) override {
        return reinterpret_cast<const char*>(hString);
    }
    float GetVarValueFloat(HCONVAR hVar) override {
        (void)hVar;
        return 0.0f;
    }
    const char* GetVarValueString(HCONVAR hVar) override {
        (void)hVar;
        return "";
    }
    LTFLOAT GetTime() override { return ServerWorld::instance().time; }
    LTFLOAT GetFrameTime() override { return ServerWorld::instance().frameDt; }
    LTRESULT GetSourceWorldOffset(LTVector& vVector) override {
        vVector.Init(0.0f, 0.0f, 0.0f);
        return LT_OK;
    }
    LTRESULT RemoveObject(HOBJECT hObj) override {
        ServerWorld& w = ServerWorld::instance();
        for (auto it = w.objs.begin(); it != w.objs.end(); ++it) {
            if (ServerWorld::toH(it->get()) == hObj && (*it)->active) {
                (*it)->active = false;
                if ((*it)->obj) (*it)->obj->OnDeactivate();
                if ((*it)->cls && (*it)->cls->m_DestructFn)
                    (*it)->cls->m_DestructFn((*it)->obj);
                w.objs.erase(it);
                return LT_OK;
            }
        }
        return LT_ERROR;
    }
    LTRESULT GetLightGroupID(const char* pName, uint32* pResult) const override {
        (void)pName;
        if (pResult) *pResult = 0;
        return LT_ERROR;
    }
    LTRESULT GetOccluderID(const char* pName, uint32* pResult) const override {
        (void)pName;
        if (pResult) *pResult = 0;
        return LT_ERROR;
    }
    LTRESULT GetTextureEffectVarID(const char* pName, uint32 nStage, uint32* pResult) const override {
        (void)pName; (void)nStage;
        if (pResult) *pResult = 0;
        return LT_ERROR;
    }
    LTRESULT LinkObjRef(HOBJECT hObj, LTObjRef* pRef) override {
        (void)hObj; (void)pRef;
        return LT_OK;
    }
    LTRESULT SendTo(ILTMessage_Read* pMsg, const char* pAddr, uint16 nPort) override {
        (void)pMsg; (void)pAddr; (void)nPort;
        return LT_ERROR;
    }
    LTRESULT StartPing(const char* pAddr, uint16 nPort, uint32* pPingID) override {
        (void)pAddr; (void)nPort;
        if (pPingID) *pPingID = 0;
        return LT_ERROR;
    }
    LTRESULT GetPingStatus(uint32 nPingID, uint32* pStatus, uint32* pLatency) override {
        (void)nPingID;
        if (pStatus) *pStatus = 0; if (pLatency) *pLatency = 0;
        return LT_ERROR;
    }
    LTRESULT RemovePing(uint32 nPingID) override {
        (void)nPingID;
        return LT_OK;
    }
    LTRESULT GetObjectRotation(HLOCALOBJ hObj, LTRotation* pRotation) override {
        ServerObj* o = ServerWorld::fromHL(hObj);
        if (!o || !pRotation) return LT_INVALIDPARAMS;
        *pRotation = o->rot;
        return LT_OK;
    }
    LTRESULT GetObjectPos(HLOCALOBJ hObj, LTVector* pPos) override {
        ServerObj* o = ServerWorld::fromHL(hObj);
        if (!o || !pPos) return LT_INVALIDPARAMS;
        *pPos = o->pos;
        return LT_OK;
    }
    HMODELANIM GetAnimIndex(HOBJECT hObj, const char* pAnimName) override {
        (void)hObj; (void)pAnimName;
        return ++ServerWorld::instance().animSeq;
    }
    void SetModelAnimation(HOBJECT hObj, HMODELANIM hAnim) override {
        (void)hObj; (void)hAnim;
    }
    HMODELANIM GetModelAnimation(HOBJECT hObj) override {
        (void)hObj;
        return 1;
    }
    void SetModelLooping(HOBJECT hObj, bool bLoop) override {
        (void)hObj; (void)bLoop;
    }
    bool GetModelLooping(HOBJECT hObj) override {
        (void)hObj;
        return false;
    }
    LTRESULT ResetModelAnimation(HOBJECT hObj) override {
        (void)hObj;
        return LT_OK;
    }
    uint32 GetModelPlaybackState(HOBJECT hObj) override {
        (void)hObj;
        return 0;
    }
    uint32 GetPointContainers(const LTVector* pPoint, HOBJECT* pList, uint32 maxListSize) override {
        (void)pPoint; (void)pList; (void)maxListSize;
        return 0;
    }
    // ILTServer
    LTRESULT GetWorldBox(LTVector& lo, LTVector& hi) override {
        lo.Init(-5000.0f, 0.0f, -5000.0f);
        hi.Init(5000.0f, 1000.0f, 5000.0f);
        return LT_OK;
    }
    LTRESULT FindWorldModelObjectIntersections(HOBJECT hWorldModel, const LTVector& vNewPos,
        const LTRotation& rNewRot, BaseObjArray<HOBJECT>& objArray) override {
        (void)hWorldModel; (void)vNewPos; (void)rNewRot; (void)objArray;
        return LT_OK;
    }
    LTRESULT MoveObject(HOBJECT hObj, const LTVector* pNewPos) override {
        S_SetObjectPos(hObj, pNewPos);
        return LT_OK;
    }
    LTRESULT GetStandingOn(HOBJECT hObj, CollisionInfo* pInfo) override {
        (void)hObj; (void)pInfo;
        return LT_ERROR;
    }
    LTRESULT FindNamedObjects(const char* pName, BaseObjArray<HOBJECT>& objArray, uint32* nTotalFound) override {
        uint32 n = 0;
        if (pName) {
            for (auto& o : ServerWorld::instance().objs) {
                if (o->active && o->name == pName) {
                    objArray.AddObject(ServerWorld::toH(o.get()));
                    ++n;
                }
            }
        }
        if (nTotalFound) *nTotalFound = n;
        return LT_OK;
    }
    LTRESULT SendToObject(ILTMessage_Read* pMsg, HOBJECT hSender, HOBJECT hSendTo, uint32 flags) override {
        (void)pMsg; (void)hSender; (void)hSendTo; (void)flags;
        return LT_OK;
    }
    LTRESULT SendToServer(ILTMessage_Read* pMsg, HOBJECT hSender, uint32 flags) override {
        (void)pMsg; (void)hSender; (void)flags;
        return LT_OK;
    }
    LTRESULT SetObjectSFXMessage(HOBJECT hObject, ILTMessage_Read* pMsg) override {
        (void)hObject; (void)pMsg;
        return LT_OK;
    }
    LTRESULT SendToClient(ILTMessage_Read* pMsg, HCLIENT hSendTo, uint32 flags) override {
        (void)pMsg; (void)hSendTo; (void)flags;
        return LT_OK;
    }
    LTRESULT SendSFXMessage(ILTMessage_Read* pMsg, const LTVector& pos, uint32 flags) override {
        (void)pMsg; (void)pos; (void)flags;
        return LT_OK;
    }
    LTRESULT GetClientPing(HCLIENT hClient, float& ping) override {
        (void)hClient;
        ping = 0.0f;
        return LT_OK;
    }
    bool GetClientData(HCLIENT hClient, uint8* pData, int& maxLen) override {
        (void)hClient; (void)pData; (void)maxLen;
        return false;
    }
    bool SetClientData(HCLIENT hClient, uint8 const* pData, int len) override {
        (void)hClient; (void)pData; (void)len;
        return false;
    }
    LTRESULT GetClientAddr(HCLIENT hClient, uint8 pAddr[4], uint16* pPort) override {
        (void)hClient;
        if (pAddr) { pAddr[0] = 127; pAddr[1] = 0; pAddr[2] = 0; pAddr[3] = 1; }
        if (pPort) *pPort = 0;
        return LT_OK;
    }
    void BreakInterObjectLink(HOBJECT hOwner, HOBJECT hLinked) override {
        (void)hOwner; (void)hLinked;
    }
    void BreakInterObjectLink(HOBJECT hObj, HOBJECT hLinked, bool bNotify) override {
        (void)hObj; (void)hLinked; (void)bNotify;
    }
    LTRESULT GetObjectName(HOBJECT hObject, char* pName, uint32 nameBufSize) override {
        ServerObj* o = ServerWorld::fromH(hObject);
        if (!o || !pName || nameBufSize == 0) return LT_INVALIDPARAMS;
        strncpy(pName, o->name.c_str(), nameBufSize - 1);
        pName[nameBufSize - 1] = 0;
        return LT_OK;
    }
    LTRESULT GetNetFlags(HOBJECT hObj, uint32& flags) override {
        (void)hObj;
        flags = 0;
        return LT_OK;
    }
    LTRESULT SetNetFlags(HOBJECT hObj, uint32 flags) override {
        (void)hObj; (void)flags;
        return LT_OK;
    }
    LTRESULT GetSaveFileVersion(uint32& nSaveFileVersion) override {
        nSaveFileVersion = 0;
        return LT_OK;
    }
    LTRESULT GetNumClassProps(const HCLASS hClass, uint32& count) override {
        (void)hClass;
        count = 0;
        return LT_OK;
    }
    LTRESULT GetClassProp(const HCLASS hClass, const uint32 iProp, ClassPropInfo& info) override {
        (void)hClass; (void)iProp; (void)info;
        return LT_ERROR;
    }
    LTRESULT GetClassName(const HCLASS hClass, char* pName, uint32 maxNameBytes) override {
        ClassDef* cd = reinterpret_cast<ClassDef*>(hClass);
        if (!cd || !cd->m_ClassName || !pName || maxNameBytes == 0) return LT_INVALIDPARAMS;
        strncpy(pName, cd->m_ClassName, maxNameBytes - 1);
        pName[maxNameBytes - 1] = 0;
        return LT_OK;
    }
    LTRESULT RemoveObject(const HCLASS hClass, LPBASECLASS pObject) override {
        (void)hClass;
        return RemoveObject(S_ObjectToHandle(pObject));
    }
    LTRESULT ThreadLoadFile(const char* pFilename, uint32 type) override {
        (void)pFilename; (void)type;
        return LT_OK;
    }
    LTRESULT UnloadFile(const char* pFilename, uint32 type) override {
        (void)pFilename; (void)type;
        return LT_OK;
    }
    LTRESULT GetHPolyObject(const HPOLY hPoly, HOBJECT& hObject) override {
        (void)hPoly;
        hObject = nullptr;
        return LT_ERROR;
    }
};

// ---- HostMemMessage: in-memory bitstream implementing both
// ---- ILTMessage_Write and ILTMessage_Read (self-consistent pair; the engine
// ---- wire format doesn't matter here: SendTo* drops messages in Fase B/C).
class HostMemMessage : public ILTMessage_Write, public ILTMessage_Read {
public:
    std::vector<uint8> buf;
    uint32 wBits = 0;
    mutable uint32 rBits = 0;
    HostMemMessage() {}
    HostMemMessage(const HostMemMessage& o) : buf(o.buf), wBits(o.wBits), rBits(0) {}
    void Free() override { delete this; }
    // write side
    ILTMessage_Read* Read() override { rBits = 0; return this; }
    uint32 Size() const override { return (wBits + 7) / 8; }
    void WriteBits(uint32 nValue, uint32 nSize) override {
        for (int i = (int)nSize - 1; i >= 0; --i) WriteBit((nValue >> i) & 1);
    }
    void WriteBits64(uint64 nValue, uint32 nSize) override {
        for (int i = (int)nSize - 1; i >= 0; --i) WriteBit((uint32)((nValue >> i) & 1));
    }
    void WriteData(const void* pData, uint32 nBits) override {
        const uint8* p = (const uint8*)pData;
        for (uint32 i = 0; i < nBits; ++i)
            WriteBit((p[i >> 3] >> (7 - (i & 7))) & 1);
    }
    void WriteMessage(const ILTMessage_Read* pMsg) override {
        const HostMemMessage* m = dynamic_cast<const HostMemMessage*>(pMsg);
        uint32 n = m ? m->wBits : 0;
        WriteBits(n, 32);
        for (uint32 i = 0; i < n; ++i) WriteBit(m->getBit(i));
    }
    void WriteMessageRaw(const ILTMessage_Read* pMsg) override {
        const HostMemMessage* m = dynamic_cast<const HostMemMessage*>(pMsg);
        if (!m) return;
        for (uint32 i = 0; i < m->wBits; ++i) WriteBit(m->getBit(i));
    }
    void WriteString(const char* pString) override {
        uint32 n = pString ? (uint32)strlen(pString) : 0;
        if (n > 0xFFFF) n = 0xFFFF;
        WriteBits(n, 16);
        for (uint32 i = 0; i < n; ++i) WriteBits((uint8)pString[i], 8);
    }
    void WriteHString(HSTRING hString) override {
        WriteString(reinterpret_cast<const char*>(hString));
    }
    void WriteCompLTVector(const LTVector& v) override {
        WriteBits(floatBits(v.x), 32); WriteBits(floatBits(v.y), 32); WriteBits(floatBits(v.z), 32);
    }
    void WriteCompPos(const LTVector& v) override { WriteCompLTVector(v); }
    void WriteCompLTRotation(const LTRotation& q) override {
        for (int i = 0; i < 4; ++i) WriteBits(floatBits(q.m_Quat[i]), 32);
    }
    void WriteHStringFormatted(int nStringCode, ...) override {
        (void)nStringCode;
    }
    void WriteHStringArgList(int nStringCode, va_list* pList) override {
        (void)nStringCode; (void)pList;
    }
    void WriteStringAsHString(const char* pString) override { WriteString(pString); }
    void WriteObject(HOBJECT hObj) override {
        WriteBits((uint32)(uintptr_t)hObj, 32);
    }
    void WriteYRotation(const LTRotation& q) override { WriteCompLTRotation(q); }
    ILTMessage_Read* Clone() const override { return new HostMemMessage(*this); }
    ILTMessage_Read* SubMsg(uint32 nPos) const override { return SubMsg(nPos, wBits - nPos); }
    ILTMessage_Read* SubMsg(uint32 nPos, uint32 nLength) const override {
        HostMemMessage* m = new HostMemMessage();
        for (uint32 i = 0; i < nLength && nPos + i < wBits; ++i) m->WriteBit(getBit(nPos + i));
        return m;
    }
    void Reset() override {
        buf.clear();
        wBits = 0;
        rBits = 0;
    }
    ILTMessage_Read* ReadMessage() override {
        uint32 n = ReadBits(32);
        HostMemMessage* m = new HostMemMessage();
        for (uint32 i = 0; i < n; ++i) m->WriteBit(readBit());
        return m;
    }
    // read side
    void Seek(int32 nOffset) override { rBits = (uint32)((int32)rBits + nOffset); }
    void SeekTo(uint32 nPos) override { rBits = nPos; }
    uint32 Tell() const override { return rBits; }
    uint32 TellEnd() const override { return wBits; }
    bool EOM() const override { return rBits >= wBits; }
    uint32 ReadBits(uint32 nBits) override {
        uint32 v = 0;
        for (uint32 i = 0; i < nBits; ++i) v = (v << 1) | readBit();
        return v;
    }
    uint64 ReadBits64(uint32 nBits) override {
        uint64 v = 0;
        for (uint32 i = 0; i < nBits; ++i) v = (v << 1) | readBit();
        return v;
    }
    void ReadData(void* pData, uint32 nBits) override {
        uint8* p = (uint8*)pData;
        uint32 nBytes = (nBits + 7) / 8;
        for (uint32 i = 0; i < nBytes; ++i) p[i] = 0;
        for (uint32 i = 0; i < nBits; ++i)
            if (readBit()) p[i >> 3] |= (uint8)(1 << (7 - (i & 7)));
    }
    uint32 ReadString(char* pDest, uint32 nMaxLen) override {
        uint32 n = ReadBits(16);
        uint32 i = 0;
        for (; i < n && i + 1 < nMaxLen; ++i) pDest[i] = (char)ReadBits(8);
        for (; i < n; ++i) ReadBits(8);
        if (nMaxLen > 0) pDest[i < nMaxLen ? i : nMaxLen - 1] = 0;
        return n;
    }
    HSTRING ReadHString() override {
        char tmp[1024];
        ReadString(tmp, sizeof(tmp));
        char* c = (char*)malloc(strlen(tmp) + 1);
        if (c) strcpy(c, tmp);
        return reinterpret_cast<HSTRING>(c);
    }
    LTVector ReadCompLTVector() override {
        LTVector v; v.Init(u2f(ReadBits(32)), u2f(ReadBits(32)), u2f(ReadBits(32)));
        return v;
    }
    LTVector ReadCompPos() override { return ReadCompLTVector(); }
    LTRotation ReadCompLTRotation() override {
        LTRotation q;
        for (int i = 0; i < 4; ++i) q.m_Quat[i] = u2f(ReadBits(32));
        return q;
    }
    uint32 ReadHStringAsString(char* pDest, uint32 nMaxLen) override {
        return ReadString(pDest, nMaxLen);
    }
    HOBJECT ReadObject() override {
        return reinterpret_cast<HOBJECT>((uintptr_t)ReadBits(32));
    }
    LTRotation ReadYRotation() override { return ReadCompLTRotation(); }
    uint64 PeekBits64(uint32 nBits) const override {
        uint64 v = 0;
        for (uint32 i = 0; i < nBits; ++i) v = (v << 1) | getBit(rBits + i);
        return v;
    }
    uint32 PeekBits(uint32 nBits) const override {
        uint32 v = 0;
        for (uint32 i = 0; i < nBits; ++i) v = (v << 1) | getBit(rBits + i);
        return v;
    }
    ILTMessage_Read* PeekMessageA() const override {
        uint32 cur = rBits;
        uint32 n = 0;
        for (uint32 i = 0; i < 32; ++i) n = (n << 1) | getBit(cur++);
        HostMemMessage* m = new HostMemMessage();
        for (uint32 i = 0; i < n; ++i) m->WriteBit(getBit(cur++));
        return m;
    }
    void PeekData(void* pData, uint32 nBits) const override {
        peekInto(pData, nBits, rBits);
    }
    uint32 PeekString(char* pDest, uint32 nMaxLen) const override {
        uint32 cur = rBits;
        uint32 n = peekBits(cur, 16);
        uint32 i = 0;
        for (; i < n && i + 1 < nMaxLen; ++i) pDest[i] = (char)peekBits(cur, 8);
        for (; i < n; ++i) peekBits(cur, 8);
        if (nMaxLen > 0) pDest[i < nMaxLen ? i : nMaxLen - 1] = 0;
        return n;
    }
    HSTRING PeekHString() const override {
        char tmp[1024];
        PeekString(tmp, sizeof(tmp));
        char* c = (char*)malloc(strlen(tmp) + 1);
        if (c) strcpy(c, tmp);
        return reinterpret_cast<HSTRING>(c);
    }
    LTVector PeekCompLTVector() const override {
        uint32 cur = rBits;
        LTVector v;
        v.Init(peekF32(cur), peekF32(cur), peekF32(cur));
        return v;
    }
    LTVector PeekCompPos() const override {
        uint32 cur = rBits;
        LTVector v;
        v.Init(peekF32(cur), peekF32(cur), peekF32(cur));
        return v;
    }
    LTRotation PeekCompLTRotation() const override {
        uint32 cur = rBits;
        LTRotation q;
        for (int i = 0; i < 4; ++i) q.m_Quat[i] = peekF32(cur);
        return q;
    }
    uint32 PeekHStringAsString(char* pDest, uint32 nMaxLen) const override {
        return PeekString(pDest, nMaxLen);
    }
    HOBJECT PeekObject() const override {
        uint32 cur = rBits;
        return reinterpret_cast<HOBJECT>((uintptr_t)peekBits(cur, 32));
    }
    LTRotation PeekYRotation() const override {
        uint32 cur = rBits;
        LTRotation q;
        for (int i = 0; i < 4; ++i) q.m_Quat[i] = peekF32(cur);
        return q;
    }

private:
    void WriteBit(uint32 b) {
        uint32 i = wBits++;
        if ((i >> 3) >= buf.size()) buf.push_back(0);
        if (b) buf[i >> 3] |= (uint8)(1 << (7 - (i & 7)));
    }
    uint32 getBit(uint32 i) const {
        if ((i >> 3) >= buf.size()) return 0;
        return (buf[i >> 3] >> (7 - (i & 7))) & 1;
    }
    uint32 peekBits(uint32& cur, uint32 n) const {
        uint32 v = 0;
        for (uint32 i = 0; i < n; ++i) v = (v << 1) | getBit(cur++);
        return v;
    }
    float peekF32(uint32& cur) const { return u2f(peekBits(cur, 32)); }
    void peekInto(void* pData, uint32 nBits, uint32& cur) const {
        uint8* p = (uint8*)pData;
        uint32 nBytes = (nBits + 7) / 8;
        for (uint32 i = 0; i < nBytes; ++i) p[i] = 0;
        for (uint32 i = 0; i < nBits; ++i)
            if (peekBits(cur, 1)) p[i >> 3] |= (uint8)(1 << (7 - (i & 7)));
    }
    uint32 readBit() {
        uint32 b = getBit(rBits);
        if (rBits < wBits) ++rBits;
        return b;
    }
    static uint32 floatBits(float f) {
        uint32 u = 0;
        memcpy(&u, &f, 4);
        return u;
    }
    static float u2f(uint32 u) {
        float f = 0.0f;
        memcpy(&f, &u, 4);
        return f;
    }
};

// ---- HostILTCommon ----
class HostILTCommon : public ILTCommon {
public:
    const char* _InterfaceImplementation() override { return ""; }
    LTRESULT GetObjectType(HOBJECT hObj, uint32* type) override {
        (void)hObj;
        if (type) *type = 0;
        return LT_OK;
    }
    LTRESULT CompressVector(const LTVector& vec, CompVector& CompV) override {
        (void)vec; (void)CompV;
        return LT_OK;
    }
    LTRESULT UncompressVector(const CompVector& CompV, LTVector& vec) override {
        (void)CompV;
        vec.Init(0.0f, 0.0f, 0.0f);
        return LT_OK;
    }
    LTRESULT CompressRotation(const LTRotation& rot, CompRot& CompR) override {
        (void)rot; (void)CompR;
        return LT_OK;
    }
    LTRESULT Parse(ConParse* pParse) override {
        (void)pParse;
        return LT_OK;
    }
    LTRESULT GetModelAnimUserDims(HOBJECT hObject, LTVector* pDims, HMODELANIM hAnim) override {
        (void)hObject; (void)hAnim;
        if (!pDims) return LT_INVALIDPARAMS;
        pDims->Init(30.0f, 40.0f, 30.0f);
        return LT_OK;
    }
    LTRESULT GetRotationVectors(LTRotation& rot, LTVector& up, LTVector& right, LTVector& forward) override {
        (void)rot;
        up.Init(0.0f, 1.0f, 0.0f);
        right.Init(1.0f, 0.0f, 0.0f);
        forward.Init(0.0f, 0.0f, 1.0f);
        return LT_OK;
    }
    LTRESULT SetupEuler(LTRotation& rot, float pitch, float yaw, float roll) override {
        (void)pitch; (void)yaw; (void)roll;
        rot.Init();
        return LT_OK;
    }
    LTRESULT CreateMessage(ILTMessage_Write*& pMsg) override {
        pMsg = new HostMemMessage();
        return LT_OK;
    }
    LTRESULT GetPointStatus(const LTVector* pPoint) override {
        (void)pPoint;
        return LT_OK;
    }
    LTRESULT GetPointShade(const LTVector* pPoint, LTVector* pColor) override {
        (void)pPoint;
        if (pColor) pColor->Init(1.0f, 1.0f, 1.0f);
        return LT_OK;
    }
    LTRESULT GetPolyTextureFlags(HPOLY hPoly, uint32* pFlags) override {
        (void)hPoly;
        if (pFlags) *pFlags = 0;
        return LT_OK;
    }
    LTRESULT GetPolyPlane(HPOLY hPoly, LTPlane* pPlane) override {
        (void)hPoly; (void)pPlane;
        return LT_ERROR;
    }
    LTRESULT UncompressRotation(const CompRot& CompR, LTRotation& rot) override {
        (void)CompR;
        rot.Init();
        return LT_OK;
    }
    LTRESULT SetObjectResource(HOBJECT hObj, EObjectResource eType, uint32 nIndex, const char* pszResource) override {
        (void)hObj; (void)eType; (void)nIndex; (void)pszResource;
        return LT_OK;
    }
    LTRESULT SetObjectFilenames(HOBJECT hObj, ObjectCreateStruct* pStruct) override {
        (void)hObj; (void)pStruct;
        return LT_OK;
    }
    LTRESULT GetObjectFlags(const HOBJECT hObj, const ObjFlagType flagType, uint32& dwFlags) override {
        (void)hObj; (void)flagType;
        dwFlags = 0;
        return LT_OK;
    }
    LTRESULT SetObjectFlags(HOBJECT hObj, const ObjFlagType flagType, uint32 dwFlags, uint32 dwMask) override {
        (void)hObj; (void)flagType; (void)dwFlags; (void)dwMask;
        return LT_OK;
    }
    LTRESULT GetAttachmentObjects(HATTACHMENT hAttachment, HOBJECT& hParent, HOBJECT& hChild) override {
        (void)hAttachment;
        hParent = nullptr; hChild = nullptr;
        return LT_ERROR;
    }
    LTRESULT GetAttachments(HLOCALOBJ hObj, HLOCALOBJ* inList, uint32 inListSize, uint32& outListSize, uint32& outNumAttachments) override {
        (void)hObj; (void)inList; (void)inListSize;
        outListSize = 0; outNumAttachments = 0;
        return LT_OK;
    }
    LTRESULT GetAttachedModelNodeTransform(HATTACHMENT hAttachment, HMODELNODE hNode, LTransform& transform) override {
        (void)hAttachment; (void)hNode; (void)transform;
        return LT_ERROR;
    }
    LTRESULT GetAttachmentTransform(HATTACHMENT hAttachment, LTransform& transform, bool bWorldSpace) override {
        (void)hAttachment; (void)transform; (void)bWorldSpace;
        return LT_ERROR;
    }
    LTRESULT GetAttachedModelSocketTransform(HATTACHMENT hAttachment, HMODELSOCKET hSocket, LTransform& transform) override {
        (void)hAttachment; (void)hSocket; (void)transform;
        return LT_ERROR;
    }
    LTRESULT NumAttachments(HLOCALOBJ hObject, uint32& dwAttachCount) override {
        (void)hObject;
        dwAttachCount = 0;
        return LT_OK;
    }
};

// ---- HostILTModel (9 pure overrides; base fallbacks below cover the rest) ----
class HostILTModel : public ILTModelClient {
public:
    const char* _InterfaceImplementation() override { return ""; }
    LTRESULT CacheModelDB(const char* Filename, HMODELDB& hModelDB) override {
        (void)Filename;
        hModelDB = 0;
        return LT_OK;
    }
    LTRESULT UncacheModelDB(HMODELDB& hModelDB) override {
        hModelDB = 0;
        return LT_OK;
    }
    LTRESULT IsModelDBLoaded(HMODELDB hModelDB) override {
        (void)hModelDB;
        return LT_OK;
    }
    LTRESULT GetFilenames(HOBJECT hObj, char* pFilename, uint32 fileBufLen, char* pSkinName, uint32 skinBufLen) override {
        (void)hObj;
        if (pFilename && fileBufLen > 0) pFilename[0] = 0;
        if (pSkinName && skinBufLen > 0) pSkinName[0] = 0;
        return LT_OK;
    }
    LTRESULT GetModelDBFilename(HOBJECT hObj, char* pRetFilename, uint32 fileBufLen) override {
        (void)hObj;
        if (pRetFilename && fileBufLen > 0) pRetFilename[0] = 0;
        return LT_OK;
    }
    LTRESULT GetSkinFilename(HOBJECT hObj, uint32 skin_index, char* pRetFilenames, uint32 fileBufLen) override {
        (void)hObj; (void)skin_index;
        if (pRetFilenames && fileBufLen > 0) pRetFilenames[0] = 0;
        return LT_OK;
    }
    LTRESULT GetRenderStyle(HOBJECT hModel, uint32 hPiece, CRenderStyle** ppRenderStyle) override {
        (void)hModel; (void)hPiece;
        if (ppRenderStyle) *ppRenderStyle = nullptr;
        return LT_OK;
    }
    LTRESULT SetRenderStyle(HOBJECT pModel, uint32 hPiece, CRenderStyle* pRenderStyle) override {
        (void)pModel; (void)hPiece; (void)pRenderStyle;
        return LT_OK;
    }
    LTRESULT UncacheModelDB(const char* filename) override {
        (void)filename;
        return LT_OK;
    }
};

} // namespace Host — ILTModel fallbacks below define ::ILTModel (global SDK
  // class); qualified definitions inside the namespace are ill-formed on MSVC.

// ---- ILTModel base fallbacks (MSVC emits base vtables; real bodies live in
// ---- the full engine). Run only if g_pLTSModel is used (Fase B: never).
LTRESULT ILTModel::GetSocketTransform(HOBJECT hObj, HMODELSOCKET hSocket, LTransform &transform, bool bWorldSpace) { return 0; }
LTRESULT ILTModel::GetNumPieces(HOBJECT hObj, uint32 & NumPieces) { return 0; }
LTRESULT ILTModel::GetPiece(HOBJECT hObj, const char *pPieceName, HMODELPIECE &hPiece) { return 0; }
LTRESULT ILTModel::GetPieceHideStatus(HOBJECT hObj, HMODELPIECE hPiece, bool &bHidden) { return 0; }
LTRESULT ILTModel::SetPieceHideStatus(HOBJECT hObj, HMODELPIECE hPiece, bool bHidden) { return 0; }
LTRESULT ILTModel::GetNode(HOBJECT hObj, const char *pNodeName, HMODELNODE &hNode) { return 0; }
LTRESULT ILTModel::GetNodeName(HOBJECT hObj, HMODELNODE hNode, char *name, uint32 maxlen) { return 0; }
LTRESULT ILTModel::GetNodeTransform(HOBJECT hObj, HMODELNODE hNode, LTransform &transform, bool bWorldSpace) { return 0; }
LTRESULT ILTModel::GetNextNode(HOBJECT hObject, HMODELNODE hNode, HMODELNODE &pNext) { return 0; }
LTRESULT ILTModel::GetRootNode(HOBJECT hObj, HMODELNODE &hNode) { return 0; }
LTRESULT ILTModel::GetNumChildren(HOBJECT hObj, HMODELNODE hNode, uint32 &NumChildren) { return 0; }
LTRESULT ILTModel::GetChild(HOBJECT hObj, HMODELNODE parent, uint32 index, HMODELNODE &child) { return 0; }
LTRESULT ILTModel::GetParent(HOBJECT hObj, HMODELNODE node, HMODELNODE &parent) { return 0; }
LTRESULT ILTModel::GetNumNodes(HOBJECT hObj, uint32 &num_nodes) { return 0; }
LTRESULT ILTModel::GetBindPoseNodeTransform(HOBJECT hObj, HMODELNODE node, LTMatrix &mat) { return 0; }
LTRESULT ILTModel::AddNodeControlFn(HOBJECT hObj, HMODELNODE hNode, NodeControlFn pFn, void *pUserData) { return 0; }
LTRESULT ILTModel::AddNodeControlFn(HOBJECT hObj, NodeControlFn pFn, void *pUserData) { return 0; }
LTRESULT ILTModel::RemoveNodeControlFn(HOBJECT hObj, HMODELNODE hNode, NodeControlFn pFn, void* pUserData) { return 0; }
LTRESULT ILTModel::RemoveNodeControlFn(HOBJECT hObj, NodeControlFn pFn, void* pUserData) { return 0; }
LTRESULT ILTModel::UpdateMainTracker(HOBJECT hObj, float fUpdateDelta) { return 0; }
LTRESULT ILTModel::GetAnimLength(HOBJECT hModel, HMODELANIM hAnim, uint32 &length) { return 0; }
LTRESULT ILTModel::FindWeightSet(HOBJECT hObj, const char *pSetName, HMODELWEIGHTSET &hSet) { return 0; }
LTRESULT ILTModel::GetMainTracker(HOBJECT hModel, ANIMTRACKERID &TrackerID) { return 0; }
LTRESULT ILTModel::GetPlaybackState(HOBJECT hModel, ANIMTRACKERID TrackerID, uint32 &flags) { return 0; }
LTRESULT ILTModel::AddTracker(HOBJECT hModel, ANIMTRACKERID TrackerID) { return 0; }
LTRESULT ILTModel::RemoveTracker(HOBJECT hModel, ANIMTRACKERID TrackerID) { return 0; }
LTRESULT ILTModel::GetAnimIndex(HOBJECT hModel, const char *pAnimName, uint32 & anim_index) { return 0; }
LTRESULT ILTModel::GetCurAnim(HOBJECT hModel, ANIMTRACKERID TrackerID, HMODELANIM &hAnim) { return 0; }
LTRESULT ILTModel::SetCurAnim(HOBJECT hModel, ANIMTRACKERID TrackerID, HMODELANIM hAnim) { return 0; }
LTRESULT ILTModel::ResetAnim(HOBJECT hModel, ANIMTRACKERID TrackerID) { return 0; }
LTRESULT ILTModel::GetLooping(HOBJECT hModel, ANIMTRACKERID TrackerID) { return 0; }
LTRESULT ILTModel::SetLooping(HOBJECT hModel, ANIMTRACKERID TrackerID, bool bLooping) { return 0; }
LTRESULT ILTModel::GetPlaying(HOBJECT hModel, ANIMTRACKERID TrackerID) { return 0; }
LTRESULT ILTModel::SetPlaying(HOBJECT hModel, ANIMTRACKERID TrackerID, bool bPlaying) { return 0; }
LTRESULT ILTModel::GetCurAnimLength(HOBJECT hModel, ANIMTRACKERID TrackerID, uint32 &length) { return 0; }
LTRESULT ILTModel::GetCurAnimTime(HOBJECT hModel, ANIMTRACKERID TrackerID, uint32 &curTime) { return 0; }
LTRESULT ILTModel::SetCurAnimTime(HOBJECT hModel, ANIMTRACKERID TrackerID, uint32 curTime) { return 0; }
LTRESULT ILTModel::SetHintNode(HOBJECT hModel, ANIMTRACKERID TrackerID, HMODELNODE hNode) { return 0; }
LTRESULT ILTModel::SetAnimRate(HOBJECT hModel, ANIMTRACKERID TrackerID, float fRate) { return 0; }
LTRESULT ILTModel::GetAnimRate(HOBJECT hModel, ANIMTRACKERID TrackerID, float &fRate) { return 0; }
LTRESULT ILTModel::GetWeightSet(HOBJECT hModel, ANIMTRACKERID TrackerID, HMODELWEIGHTSET &hSet) { return 0; }
LTRESULT ILTModel::SetWeightSet(HOBJECT hModel, ANIMTRACKERID TrackerID, HMODELWEIGHTSET hSet) { return 0; }
LTRESULT ILTModel::GetNumLODs(HOBJECT hModel, HMODELPIECE hPiece, uint32 &num_lods) { return 0; }
LTRESULT ILTModel::GetLODValFromDist(HOBJECT hModel, HMODELPIECE hPiece, float dist, uint32 &lod_index) { return 0; }
LTRESULT ILTModel::ApplyAnimations(HOBJECT hModel) { return 0; }
LTRESULT ILTModel::GetNumModelOBBs(HOBJECT hObj, uint32 & num_obbs) { return 0; }
LTRESULT ILTModel::GetModelOBBCopy(HOBJECT hObj, ModelOBB *) { return 0; }
LTRESULT ILTModel::UpdateModelOBB(HOBJECT hObj, ModelOBB *) { return 0; }
LTRESULT ILTModel::GetSocket(LTObject *pObj, char const *pName, unsigned int &hSocket) { (void)pObj; (void)pName; hSocket = 0; return 0; }

namespace Host {
// ---- HostILTSoundMgr (EAX20 variants active, DX8 ones compiled out) ----
class HostILTSoundMgr : public ILTClientSoundMgr {
public:
    const char* _InterfaceImplementation() override { return ""; }
    LTRESULT PlaySound(PlaySoundInfo* pPlaySoundInfo, HLTSOUND& hResult) override {
        (void)pPlaySoundInfo;
        hResult = 0;
        return LT_OK;
    }
    LTRESULT GetSoundDuration(HLTSOUND hSound, LTFLOAT& fDuration) override {
        (void)hSound;
        fDuration = 0.0f;
        return LT_OK;
    }
    LTRESULT IsSoundDone(HLTSOUND hSound, bool& bDone) override {
        (void)hSound;
        bDone = true;
        return LT_OK;
    }
    LTRESULT KillSound(HLTSOUND hSound) override {
        (void)hSound;
        return LT_OK;
    }
    LTRESULT KillSoundLoop(HLTSOUND hSound) override {
        (void)hSound;
        return LT_OK;
    }
    LTRESULT GetSound3DProviderLists(Sound3DProvider*& pSound3DProviderList, bool bVerify, uint32 uiMax3DVoices) override {
        (void)bVerify; (void)uiMax3DVoices;
        pSound3DProviderList = nullptr;
        return LT_OK;
    }
    LTRESULT ReleaseSound3DProviderList(Sound3DProvider* pSound3DProviderList) override {
        (void)pSound3DProviderList;
        return LT_OK;
    }
    LTRESULT InitSound(InitSoundInfo* pSoundInfo) override {
        (void)pSoundInfo;
        return LT_OK;
    }
    LTRESULT SetListenerDoppler(float fVolume) override {
        (void)fVolume;
        return LT_OK;
    }
    LTRESULT GetVolume(uint16& nVolume) override {
        nVolume = 100;
        return LT_OK;
    }
    LTRESULT SetVolume(uint16 nVolume) override {
        (void)nVolume;
        return LT_OK;
    }
    LTRESULT UpdateVolumeSettings() override { return LT_OK; }
    LTRESULT SetReverbProperties(ReverbProperties* pReverbProperties) override {
        (void)pReverbProperties;
        return LT_OK;
    }
    LTRESULT GetReverbProperties(ReverbProperties* pReverbProperties) override {
        (void)pReverbProperties;
        return LT_OK;
    }
    LTRESULT SetSoundOcclusion(HLTSOUND hSound, LTFLOAT fLevel) override {
        (void)hSound; (void)fLevel;
        return LT_OK;
    }
    LTRESULT GetSoundOcclusion(HLTSOUND hSound, LTFLOAT* pLevel) override {
        (void)hSound;
        if (pLevel) *pLevel = 0.0f;
        return LT_OK;
    }
    LTRESULT SetSoundObstruction(HLTSOUND hSound, LTFLOAT fLevel) override {
        (void)hSound; (void)fLevel;
        return LT_OK;
    }
    LTRESULT GetSoundObstruction(HLTSOUND hSound, LTFLOAT* pLevel) override {
        (void)hSound;
        if (pLevel) *pLevel = 0.0f;
        return LT_OK;
    }
    LTRESULT SetSoundFilter(const char* pFilter) override {
        (void)pFilter;
        return LT_OK;
    }
    LTRESULT SetSoundFilterParam(const char* pParam, float fValue) override {
        (void)pParam; (void)fValue;
        return LT_OK;
    }
    LTRESULT EnableSoundFilter(bool bEnable) override {
        (void)bEnable;
        return LT_OK;
    }
    LTRESULT SetSoundPosition(HLTSOUND hSound, LTVector* pPos) override {
        (void)hSound; (void)pPos;
        return LT_OK;
    }
    LTRESULT GetSoundPosition(HLTSOUND hSound, LTVector* pPos) override {
        (void)hSound;
        if (pPos) pPos->Init(0.0f, 0.0f, 0.0f);
        return LT_OK;
    }
    LTRESULT PauseSounds() override { return LT_OK; }
    LTRESULT ResumeSounds() override { return LT_OK; }
    LTRESULT SetSoundClassMultiplier(uint8 nSoundClass, float fMult, bool bUseGlobalVolume) override {
        (void)nSoundClass; (void)fMult; (void)bUseGlobalVolume;
        return LT_OK;
    }
    LTRESULT GetSoundClassMultiplier(uint8 nSoundClass, float* pfMult) override {
        (void)nSoundClass;
        if (pfMult) *pfMult = 1.0f;
        return LT_OK;
    }
    LTRESULT SetListener(bool bListenerInClient, LTVector* pPos, LTRotation* pRot, bool bTeleport) override {
        (void)bListenerInClient; (void)pPos; (void)pRot; (void)bTeleport;
        return LT_OK;
    }
};

// ---- HostServerPhysics: dedicated server physics (no shared client state) ----
class HostServerPhysics : public ILTClientPhysics {
public:
    const char* _InterfaceImplementation() override { return ""; }
    LTRESULT IsWorldObject(HOBJECT hObj) override {
        (void)hObj;
        return LT_OK;
    }
    LTRESULT GetStairHeight(float& fHeight) override {
        fHeight = 0.0f;
        return LT_OK;
    }
    LTRESULT SetStairHeight(float fHeight) override {
        (void)fHeight;
        return LT_OK;
    }
    LTRESULT GetMass(HOBJECT hObj, float* m) override {
        (void)hObj;
        if (m) *m = 1.0f;
        return LT_OK;
    }
    LTRESULT SetMass(HOBJECT hObj, float m) override {
        (void)hObj; (void)m;
        return LT_OK;
    }
    LTRESULT GetFrictionCoefficient(HOBJECT hObj, float* u) override {
        (void)hObj;
        if (u) *u = 0.5f;
        return LT_OK;
    }
    LTRESULT SetFrictionCoefficient(HOBJECT hObj, float u) override {
        (void)hObj; (void)u;
        return LT_OK;
    }
    LTRESULT GetObjectDims(HOBJECT hObj, LTVector* d) override {
        (void)hObj;
        if (d) d->Init(30.0f, 40.0f, 30.0f);
        return LT_OK;
    }
    LTRESULT SetObjectDims(HOBJECT hObj, LTVector* d, uint32 flag) override {
        (void)hObj; (void)d; (void)flag;
        return LT_OK;
    }
    LTRESULT GetVelocity(HOBJECT hObj, LTVector* v) override {
        (void)hObj;
        if (v) v->Init(0.0f, 0.0f, 0.0f);
        return LT_OK;
    }
    LTRESULT SetVelocity(HOBJECT hObj, const LTVector* v) override {
        (void)hObj; (void)v;
        return LT_OK;
    }
    LTRESULT GetForceIgnoreLimit(HOBJECT hObj, float& f) override {
        (void)hObj;
        f = 0.0f;
        return LT_OK;
    }
    LTRESULT SetForceIgnoreLimit(HOBJECT hObj, float f) override {
        (void)hObj; (void)f;
        return LT_OK;
    }
    LTRESULT GetAcceleration(HOBJECT hObj, LTVector* a) override {
        (void)hObj;
        if (a) a->Init(0.0f, 0.0f, 0.0f);
        return LT_OK;
    }
    LTRESULT SetAcceleration(HOBJECT hObj, const LTVector* a) override {
        (void)hObj; (void)a;
        return LT_OK;
    }
    LTRESULT MoveObject(HOBJECT hObj, const LTVector* p, uint32 flag) override {
        (void)flag;
        S_SetObjectPos(hObj, p);
        return LT_OK;
    }
    LTRESULT GetStandingOn(HOBJECT hObj, CollisionInfo* info) override {
        (void)hObj; (void)info;
        return LT_ERROR;
    }
    LTRESULT GetGlobalForce(LTVector& a) override {
        a.Init(0.0f, 0.0f, 0.0f);
        return LT_OK;
    }
    LTRESULT SetGlobalForce(const LTVector& a) override {
        (void)a;
        return LT_OK;
    }
    LTRESULT UpdateMovement(MoveInfo* pInfo) override {
        (void)pInfo;
        return LT_OK;
    }
    LTRESULT MovePushObjects(HOBJECT hToMove, const LTVector& p, HOBJECT* hPushObjects, uint32 nPushObjects) override {
        (void)hToMove; (void)p; (void)hPushObjects; (void)nPushObjects;
        return LT_OK;
    }
    LTRESULT RotatePushObjects(HOBJECT hToMove, const LTRotation& q, HOBJECT* hPushObjects, uint32 nPushObjects) override {
        (void)hToMove; (void)q; (void)hPushObjects; (void)nPushObjects;
        return LT_OK;
    }
};

// ---- interface instances owned by the server world ----
struct ServerIfaces {
    HostILTServer ilt;
    HostILTCommon common;
    HostILTModel model;
    HostILTSoundMgr sound;
    HostServerPhysics sphysics;
};

inline ServerIfaces& serverIfaces() {
    static ServerIfaces ifs;
    return ifs;
}

inline ILTCommon* HostILTServer::Common() { return &serverIfaces().common; }
inline ILTPhysics* HostILTServer::Physics() { return &serverIfaces().sphysics; }
inline ILTTransform* HostILTServer::GetTransformLT() { return nullptr; }
inline ILTModel* HostILTServer::GetModelLT() { return &serverIfaces().model; }
inline ILTSoundMgr* HostILTServer::SoundMgr() { return &serverIfaces().sound; }

// Bind every holder the game expects (mirrors engine startup).
inline CLTServerShell*& serverShellPtr() {
    static CLTServerShell* p = nullptr;
    return p;
}
inline void setServerShell(CLTServerShell* s) { serverShellPtr() = s; }
inline void bindServerInterfaces() {
    srand(12345);
    serverIfaces().ilt.apply();
    g_pLTServer = &serverIfaces().ilt;
    g_pLTSCommon = &serverIfaces().common;
    g_pLTSPhysics = &serverIfaces().sphysics;
    g_pLTSModel = &serverIfaces().model;
    g_pLTSSoundMgr = &serverIfaces().sound;
    // Client->server message path (single shared bitstream format) and the
    // shell instance the client forwards to.
    g_routeToServer = [](ILTMessage_Read* m) {
        if (serverShellPtr() && m)
            serverShellPtr()->OnMessage(
                reinterpret_cast<HCLIENT>((intptr_t)1), m);
    };
    g_createClientMessage = []() -> ILTMessage_Write* {
        return new HostMemMessage();
    };
}

// Spawn a named server object and run its create/update handshake.
inline HOBJECT spawnServerObject(const char* className, const LTVector& pos,
                                 const char* objName,
                                 const std::map<std::string, std::string>& props) {
    HCLASS hClass = S_GetClass(className);
    if (!hClass) {
        printf("[srv] spawn: unknown class %s\n", className ? className : "(null)");
        return nullptr;
    }
    ObjectCreateStruct cs;
    cs.m_Pos = pos;
    cs.m_hClass = hClass;
    ServerObj* pre = nullptr;
    (void)pre;
    LPBASECLASS obj = S_CreateObject(hClass, &cs);
    if (!obj) return nullptr;
    HOBJECT h = S_ObjectToHandle(obj);
    ServerObj* so = ServerWorld::fromH(h);
    if (so) {
        if (objName) so->name = objName;
        so->props = props;
        obj->EngineMessageFn(MID_INITIALUPDATE, nullptr, 0.0f);
        S_SetNextUpdate(h, 0.001f);
    }
    return h;
}

// Advance server time and deliver due MID_UPDATEs.
inline void DrawServerWorld();
inline uint32 tickServerWorld(float dt) {
    ServerWorld& w = ServerWorld::instance();
    w.time += dt;
    w.frameDt = dt;
    uint32 ticked = 0;
    for (auto& o : w.objs) {
        if (!o->active || !o->useTimer || !o->obj) continue;
        if (w.time < o->nextUpdate) continue;
        o->useTimer = false;
        w.current = o.get();
        o->obj->EngineMessageFn(MID_UPDATE, nullptr, 0.0f);
        w.current = nullptr;
        ++ticked;
    }
    w.totalTicks += ticked;
    // Render phase piggyback: ticks run once per frame right before the
    // renderer draws, so pushed billboards land in the current frame.
    DrawServerWorld();
    return ticked;
}

inline uint32 serverObjectCount() {
    uint32 n = 0;
    for (auto& o : ServerWorld::instance().objs)
        if (o->active) ++n;
    return n;
}

// ---- Fase D: server objects as screen-space billboards ----
// The host has no .ltb model renderer yet, so seals draw as textured quads
// projected through the client camera (third-person follow cam, fovX=90).
static uint32 sealTextureId() {
    static uint32 id = 0;
    static bool tried = false;
    if (!tried) {
        tried = true;
        std::string base = ClientTuned::rezDir();
        std::string rel = "tex/seal/seal.tga";
        std::string p = base.empty() ? rel : base + "/" + rel;
        std::string rp;
        if (!resolveAsset(p, rp) && !base.empty()) resolveAsset(rel, rp);
        TgaImage img;
        if (!rp.empty() && loadTGA(rp, img) && VkBridge::renderer())
            id = VkBridge::renderer()->RegisterTexture(img.w, img.h, img.rgba.data());
        printf("[srv] seal texture id=%u (%s)\n", id, rp.c_str());
        fflush(stdout);
    }
    return id;
}
inline void DrawServerWorld() {
    // World visuals only while the client is in-world (never over the menu).
    if (!ServerWorld::instance().clientEntered) return;
    uint32 tex = sealTextureId();
    if (!tex || !VkBridge::renderer()) return;
    const ObjState* cam = nullptr;
    for (auto& kv : objects()) {
        if (kv.second.type == 5 /*OT_CAMERA*/) { cam = &kv.second; break; }
    }
    if (!cam) return;
    const float w = (float)VkBridge::width(), h = (float)VkBridge::height();
    LTVector campos = cam->pos;
    LTRotation camrot = cam->rot;
    const LTVector fwd = camrot.Forward();
    const LTVector right = camrot.Right();
    const LTVector up = camrot.Up();
    for (auto& o : ServerWorld::instance().objs) {
        if (!o->active || !o->obj) continue;
        if (o->name.compare(0, 4, "Seal") != 0) continue;
        const LTVector rel = o->pos - campos;
        const float cx = rel.Dot(right);
        const float cy = rel.Dot(up);
        const float cz = rel.Dot(fwd);
        if (cz < 5.0f) continue; // behind or inside the camera
        const float halfW = cz; // tan(45deg) = 1, fovX = 90deg
        const float halfH = cz * (h / w);
        const float sx = w * 0.5f + (cx / halfW) * (w * 0.5f);
        const float sy = h * 0.5f - (cy / halfH) * (h * 0.5f);
        const float s = 40.0f / halfH * (h * 0.5f); // 40-unit seal
        if (sx < -s || sx > w + s || sy < -s || sy > h + s) continue;
        VkBridge::quadTex(sx - s, sy - s, sx + s, sy + s,
                          0.0f, 0.0f, 1.0f, 1.0f,
                          1.0f, 1.0f, 1.0f, 1.0f, tex);
    }
}

// Mirror client world presence on the server: first client enter spawns the
// server player (CPlayerSrvr via OnClientEnterWorld), exit removes it.
inline void ServerOnClientEnterWorld(CLTServerShell* shell) {
    ServerWorld& w = ServerWorld::instance();
    if (!shell || w.clientEntered) return;
    w.clientEntered = true;
    HCLIENT c = reinterpret_cast<HCLIENT>((intptr_t)1);
    shell->OnAddClient(c);
    LPBASECLASS p = shell->OnClientEnterWorld(c);
    printf("[srv] client entered world player=%p\n", (void*)p);
    fflush(stdout);
}
inline void ServerOnClientExitWorld(CLTServerShell* shell) {
    ServerWorld& w = ServerWorld::instance();
    if (!shell || !w.clientEntered) return;
    w.clientEntered = false;
    HCLIENT c = reinterpret_cast<HCLIENT>((intptr_t)1);
    shell->OnClientExitWorld(c);
    shell->OnRemoveClient(c);
    printf("[srv] client exited world\n");
    fflush(stdout);
}

} // namespace Host
