#pragma once
#include "Platform/platform.h"
#include <type_traits>
#include <iltclient.h>
#include <iltcsbase.h>
class HostLTClient : public ILTClient {
public:
LTRESULT GetObjectPos(HLOCALOBJ hObj, LTVector *pPos) override { return LT_OK; }
LTRESULT SetObjectPos(HLOCALOBJ hObj, const LTVector *pPos, bool bForce) override { return LT_OK; }
LTRESULT GetObjectRotation(HLOCALOBJ hObj, LTRotation *pRotation) override { return LT_OK; }
ILTVideoMgr* VideoMgr() override { return nullptr; }
ILTDrawPrim* GetDrawPrim() override { return nullptr; }
ILTTexInterface* GetTexInterface() override { return nullptr; }
ILTWidgetManager* GetWidgetManager() override { return nullptr; }
ILTFontManager* GetFontManager() override { return nullptr; }
ILTCursor* Cursor() override { return nullptr; }
ILTDirectMusicMgr* GetDirectMusicMgr() override { return nullptr; }
ILTBenchmarkMgr* GetBenchmarkMgr() override { return nullptr; }
LTRESULT GetPointShade(const LTVector *pPoint, LTVector *pColor) override { return LT_OK; }
LTRESULT GetRendererStats(LTRendererStats &refStats) override { return LT_OK; }
LTRESULT SetObjectRenderGroup(HOBJECT hObj, uint32 nGroup) override { return LT_OK; }
LTRESULT SetObjectRenderGroupEnabled(uint32 nGroup, bool bEnabled) override { return LT_OK; }
LTRESULT SetAllObjectRenderGroupEnabled() override { return LT_OK; }
LTRESULT AddGlowRenderStyleMapping(const char* pszSource, const char* pszMapTo) override { return LT_OK; }
LTRESULT SetGlowDefaultRenderStyle(const char* pszFilename) override { return LT_OK; }
LTRESULT SetNoGlowRenderStyle(const char* pszFilename) override { return LT_OK; }
bool AddVertexShader(const char *pFileName, int VertexShaderID, const uint32 *pVertexElements, uint32 VertexElementsSize, bool bCompileShader) override { return false; }
void RemoveVertexShader(int VertexShaderID) override {}
void RemoveAllVertexShaders() override {}
LTVertexShader* GetVertexShader(int VertexShaderID) override { return nullptr; }
bool AddPixelShader(const char *pFileName, int PixelShaderID, bool bCompileShader) override { return false; }
void RemovePixelShader(int PixelShaderID) override {}
void RemoveAllPixelShaders() override {}
LTPixelShader* GetPixelShader(int PixelShaderID) override { return nullptr; }
LTRESULT GetSConValueFloat(const char *pName, float &val) override { return LT_OK; }
LTRESULT GetSConValueString(const char *pName, char *valBuf, uint32 bufLen) override { return LT_OK; }
LTRESULT GetDeviceObjectName(char const* pszDeviceName, uint32 nDeviceObjectId, char* pszDeviceObjectName, uint32 nDeviceObjectNameLen) override { return LT_OK; }
LTRESULT GetGlobalLightDir(LTVector &dir) override { return LT_OK; }
LTRESULT SetGlobalLightDir(const LTVector &dir) override { return LT_OK; }
LTRESULT GetGlobalLightColor(LTVector &color) override { return LT_OK; }
LTRESULT SetGlobalLightColor(const LTVector& color) override { return LT_OK; }
LTRESULT GetGlobalLightConvertToAmbient(float& fConvertToAmbient) override { return LT_OK; }
LTRESULT SetGlobalLightConvertToAmbient(float fConvertToAmbient) override { return LT_OK; }
LTRESULT SendToServer(ILTMessage_Read *pMsg, uint32 flags) override { return LT_OK; }
LTRESULT GetServerIPAddress(uint8 pAddr[4], uint16 *pPort) override { return LT_OK; }
LTRESULT ProcessAttachments(HOBJECT hObj) override { return LT_OK; }
LTParticle* AddParticle(HLOCALOBJ hObj, const LTVector *pPos, const LTVector *pVelocity, const LTVector *pColor, float lifeTime) override { return nullptr; }
LTRESULT StartQuery(const char *pInfo) override { return LT_OK; }
LTRESULT UpdateQuery() override { return LT_OK; }
LTRESULT GetQueryResults(NetSession* &pListHead) override { return LT_OK; }
LTRESULT EndQuery() override { return LT_OK; }
LTRESULT IsLocalToServer(bool *bLocal) override { return LT_OK; }
LTRESULT GetLightGroupColor(uint32 nID, LTVector *pColor) const override { return LT_OK; }
LTRESULT SetLightGroupColor(uint32 nID, const LTVector &vColor) override { return LT_OK; }
LTRESULT GetOccluderEnabled(uint32 nID, bool *pEnabled) const override { return LT_OK; }
LTRESULT SetOccluderEnabled(uint32 nID, bool bEnabled) override { return LT_OK; }
LTRESULT SetTextureEffectVar(uint32 nID, uint32 nVar, float fVal) override { return LT_OK; }
LTRESULT GetSpriteControl(HLOCALOBJ hObj, ILTSpriteControl* &pControl) override { return LT_OK; }
LTRESULT SetSpriteEffectShaderID(HLOCALOBJ hObj, uint32 EffectShaderID) override { return LT_OK; }
LTRESULT GetCanvasFn(HOBJECT hCanvas, CanvasDrawFn &fn, void* &pUserData) override { return LT_OK; }
LTRESULT SetCanvasFn(HOBJECT hCanvas, CanvasDrawFn fn, void* pUserData) override { return LT_OK; }
LTRESULT GetCanvasRadius(HOBJECT hCanvas, float &radius) override { return LT_OK; }
LTRESULT SetCanvasRadius(HOBJECT hCanvas, float radius) override { return LT_OK; }
LTRESULT GetMaxRadiusInPoly(const HPOLY hPoly, const LTVector& vPos, float& fMaxRadius) override { return LT_OK; }
ILTCommon* Common() override { return nullptr; }
ILTPhysics* Physics() override { return nullptr; }
ILTTransform* GetTransformLT() override { return nullptr; }
ILTModel* GetModelLT() override { return nullptr; }
ILTSoundMgr* SoundMgr() override { return nullptr; }
void CPrint(const char *pMsg, ...) override {}
bool GetContainerCode(HOBJECT hObj, uint16 *pCode) override { return false; }
uint32 GetObjectContainers(HOBJECT hObj, HOBJECT *pContainerList, uint32 maxListSize) override { return uint32(); }
uint32 GetContainedObjects(HOBJECT hContainer, HOBJECT *pObjectList, uint32 maxListSize) override { return uint32(); }
LTRESULT OpenFile(const char *pFilename, ILTStream **pStream) override { return LT_OK; }
LTRESULT CopyFile(const char *pszSourceFile, const char *pszDestFile) override { return LT_OK; }
LTRESULT OpenMemoryStream(ILTStream **pStream, uint32 nCacheSize) override { return LT_OK; }
LTRESULT GetBlindObjectData(uint32 nNum, uint32 nId, uint8*& pData, uint32& nSize) override { return LT_OK; }
LTRESULT FreeBlindObjectData(uint32 nNum, uint32 nId) override { return LT_OK; }
HSTRING FormatString(int messageCode, ...) override { return HSTRING(); }
HSTRING CopyString(HSTRING hString) override { return HSTRING(); }
HSTRING CreateString(const char *pString) override { return HSTRING(); }
void FreeString(HSTRING hString) override {}
bool CompareStrings(HSTRING hString1, HSTRING hString2) override { return false; }
bool CompareStringsUpper(HSTRING hString1, HSTRING hString2) override { return false; }
const char* GetStringData(HSTRING hString) override { return ""; }
float GetVarValueFloat(HCONSOLEVAR hVar) override { return 0.0f; }
const char* GetVarValueString(HCONSOLEVAR hVar) override { return ""; }
LTFLOAT GetTime() override { return 0.0f; }
LTFLOAT GetFrameTime() override { return 0.0f; }
LTRESULT GetSourceWorldOffset(LTVector& vVector) override { return LT_OK; }
LTRESULT RemoveObject(HOBJECT hObj) override { return LT_OK; }
LTRESULT GetLightGroupID(const char *pName, uint32 *pResult) const override { return LT_OK; }
LTRESULT GetOccluderID(const char *pName, uint32 *pResult) const override { return LT_OK; }
LTRESULT GetTextureEffectVarID(const char *pName, uint32 nStage, uint32 *pResult) const override { return LT_OK; }
LTRESULT LinkObjRef(HOBJECT hObj, LTObjRef *pRef) override { return LT_OK; }
LTRESULT SendTo(ILTMessage_Read *pMsg, const char *pAddr, uint16 nPort) override { return LT_OK; }
LTRESULT StartPing(const char *pAddr, uint16 nPort, uint32 *pPingID) override { return LT_OK; }
LTRESULT GetPingStatus(uint32 nPingID, uint32 *pStatus, uint32 *pLatency) override { return LT_OK; }
LTRESULT RemovePing(uint32 nPingID) override { return LT_OK; }
HMODELANIM GetAnimIndex(HOBJECT hObj, const char *pAnimName) override { return HMODELANIM(); }
void SetModelAnimation(HOBJECT hObj, HMODELANIM hAnim) override {}
HMODELANIM GetModelAnimation(HOBJECT hObj) override { return HMODELANIM(); }
void SetModelLooping(HOBJECT hObj, bool bLoop) override {}
bool GetModelLooping(HOBJECT hObj) override { return false; }
LTRESULT ResetModelAnimation(HOBJECT hObj) override { return LT_OK; }
uint32 GetModelPlaybackState(HOBJECT hObj) override { return uint32(); }
uint32 GetPointContainers(const LTVector *pPoint, HOBJECT *pList, uint32 maxListSize) override { return uint32(); }
const char* _InterfaceImplementation() override { return ""; }
};
