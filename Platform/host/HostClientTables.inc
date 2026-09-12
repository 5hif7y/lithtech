#pragma once
#include "Platform/platform.h"
#include <iltclient.h>
#include <iltcsbase.h>
// Auto-generated table fill for ILTClient
static FileEntry * HostClient_GetFileList(const char *pDirName) { return nullptr; }
static void HostClient_FreeFileList(FileEntry *pHead) {}
static LTRESULT HostClient_WriteConfigFileFields(const char *pFilename, uint32 nNumValues, const char** pValues) { return LT_OK; }
static LTRESULT HostClient_WriteConfigFile(const char *pFilename) { return LT_OK; }
static LTRESULT HostClient_ReadConfigFile(const char *pFilename) { return LT_OK; }
static void HostClient_GetAxisOffsets(LTFLOAT *offsets) {}
static void HostClient_PlayJoystickEffect(const char *pEffectName, float x, float y) {}
static bool HostClient_InitMusic(const char *szMusicDLL) { return false; }
static bool HostClient_LoadSong(const char *szSong) { return false; }
static void HostClient_DestroyAllSongs(void) {}
static bool HostClient_PlayList(const char *szPlayList, const char *szTransition, bool bLoop, uint32 dwBoundaryFlags) { return false; }
static void HostClient_StopMusic(uint32 dwBoundaryFlags) {}
static bool HostClient_PauseMusic(void) { return false; }
static bool HostClient_ResumeMusic(void) { return false; }
static bool HostClient_AddSongToPlayList(const char *szPlayList, const char *szSong) { return false; }
static void HostClient_DeletePlayList(const char *szPlayList) {}
static short HostClient_GetMusicVolume(void) { return short(); }
static void HostClient_SetMusicVolume(short wVolume) {}
static LTRESULT HostClient_GetSoundTimer(HLTSOUND hSound, LTFLOAT *fTimer) { return LT_OK; }
static LTRESULT HostClient_GetSoundData(HLTSOUND hSound, int16 *&pSixteenBitData, int8 *&pEightBitData, uint32 *dwSamplesPerSecond, uint32 *dwChannels) { return LT_OK; }
static LTRESULT HostClient_GetSoundOffset(HLTSOUND hSound, uint32 * dwOffset, uint32 * dwSize) { return LT_OK; }
static bool HostClient_IsDone(HLTSOUND pSoundHandle) { return false; }
static void HostClient_GetListener(bool *bListenerInClient, LTVector *pPos, LTRotation *pRot) {}
static bool HostClient_IntersectSegment(IntersectQuery *pQuery, IntersectInfo *pInfo) { return false; }
static bool HostClient_IntersectSweptSphere(const LTVector& vStart, const LTVector& vEnd, float fRadius, LTVector& vFinalPos, LTVector& vNormal) { return false; }
static bool HostClient_CastRay(IntersectQuery *pQuery, IntersectInfo *pInfo) { return false; }
static void HostClient_SetObjectPosAndRotation(HLOCALOBJ hObj, const LTVector *pPos, const LTRotation *pRotation) {}
static LTRESULT HostClient_FindObjectsInSphere(const LTVector *pCenter, float radius, HLOCALOBJ *inObjects, uint32 nInObjects, uint32 *nOutObjects, uint32 *nFound) { return LT_OK; }
static LTRESULT HostClient_FindObjectsInBox(const LTVector *pCenter, float radius, HLOCALOBJ *inObjects, uint32 nInObjects, uint32 *nOutObjects, uint32 *nFound) { return LT_OK; }
static LTRESULT HostClient_GetGameMode(int *mode) { return LT_OK; }
static LTRESULT HostClient_StartGame(StartGameRequest *pRequest) { return LT_OK; }
static LTRESULT HostClient_GetLocalClientID(uint32 *pID) { return LT_OK; }
static void HostClient_Disconnect(void) {}
static bool HostClient_IsConnected(void) { return false; }
static void HostClient_Shutdown(void) {}
static void HostClient_ShutdownWithMessage(const char *pMsg, ...) {}
static LTRESULT HostClient_FlipScreen(uint32 flags) { return LT_OK; }
static LTRESULT HostClient_Start3D(void) { return LT_OK; }
static LTRESULT HostClient_RenderCamera(HLOCALOBJ hCamera, float fFrameTime) { return LT_OK; }
static LTRESULT HostClient_ClearScreen(LTRect *pClearRect, uint32 flags, LTRGB* pClearColor) { return LT_OK; }
static LTRESULT HostClient_RenderObjects(HLOCALOBJ hCamera, HLOCALOBJ *pObjects, int nObjects, float fFrameTime) { return LT_OK; }
static LTRESULT HostClient_MakeCubicEnvMap(HLOCALOBJ hCamera, uint32 nSize, const char* pszFilePrefix) { return LT_OK; }
static LTRESULT HostClient_StartOptimized2D(void) { return LT_OK; }
static LTRESULT HostClient_EndOptimized2D(void) { return LT_OK; }
static LTRESULT HostClient_SetOptimized2DBlend(LTSurfaceBlend blend) { return LT_OK; }
static LTRESULT HostClient_GetOptimized2DBlend(LTSurfaceBlend &blend) { return LT_OK; }
static LTRESULT HostClient_End3D(uint32 flags) { return LT_OK; }
static LTRESULT HostClient_GetOptimized2DColor(HLTCOLOR &hColor) { return LT_OK; }
static LTRESULT HostClient_SetOptimized2DColor(HLTCOLOR hColor) { return LT_OK; }
static RMode * HostClient_GetRenderModes(void) { return nullptr; }
static void HostClient_RelinquishRenderModes(RMode *pModes) {}
static LTRESULT HostClient_GetRenderMode(RMode *pMode) { return LT_OK; }
static LTRESULT HostClient_SetRenderMode(RMode *pMode) { return LT_OK; }
static LTRESULT HostClient_ShutdownRender(uint32 flags) { return LT_OK; }
static HLTFONT HostClient_CreateFont(const char *pFontName, int width, int height, bool bItalic, bool bUnderline, bool bBold) { return HLTFONT(); }
static void HostClient_DeleteFont(HLTFONT hFont) {}
static LTRESULT HostClient_SetFontExtraSpace(HLTFONT hFont, int pixels) { return LT_OK; }
static LTRESULT HostClient_GetFontExtraSpace(HLTFONT hFont, int &pixels) { return LT_OK; }
static LTRESULT HostClient_RegisterConsoleProgram(const char *pName, ConsoleProgramFn fn) { return LT_OK; }
static LTRESULT HostClient_UnregisterConsoleProgram(const char *pName) { return LT_OK; }
static HCONSOLEVAR HostClient_GetConsoleVar(const char *pName) { return HCONSOLEVAR(); }
static HLTCOLOR HostClient_CreateColor(float r, float g, float b, bool bTransparent) { return HLTCOLOR(); }
static void HostClient_DeleteColor(HLTCOLOR hColor) {}
static HLTCOLOR HostClient_SetupColor1(float r, float g, float b, bool bTransparent) { return HLTCOLOR(); }
static HLTCOLOR HostClient_SetupColor2(float r, float g, float b, bool bTransparent) { return HLTCOLOR(); }
static LTRESULT HostClient_GetBorderSize(HSURFACE hSurface, HLTCOLOR hColor, LTRect *pRect) { return LT_OK; }
static LTRESULT HostClient_OptimizeSurface(HSURFACE hSurface, HLTCOLOR hTransparentColor) { return LT_OK; }
static LTRESULT HostClient_UnoptimizeSurface(HSURFACE hSurface) { return LT_OK; }
static HSURFACE HostClient_GetScreenSurface(void) { return HSURFACE(); }
static LTRESULT HostClient_CreateHeightmapFromBitmap(const char *pBitmapName, uint32* pWidth, uint32* pHeight, uint8** ppData) { return LT_OK; }
static HSURFACE HostClient_CreateSurfaceFromBitmap(char *pBitmapName) { return HSURFACE(); }
static LTRESULT HostClient_FreeHeightmap(uint8* pData) { return LT_OK; }
static HSURFACE HostClient_CreateSurfaceFromString(HLTFONT hFont, HSTRING hString, HLTCOLOR hForeColor, HLTCOLOR hBackColor, int extraPixelsX, int extraPixelsY) { return HSURFACE(); }
static HSURFACE HostClient_CreateSurface(uint32 width, uint32 height) { return HSURFACE(); }
static LTRESULT HostClient_DeleteSurface(HSURFACE hSurface) { return LT_OK; }
static void * HostClient_GetSurfaceUserData(HSURFACE hSurface) { return nullptr; }
static void HostClient_SetSurfaceUserData(HSURFACE hSurface, void *pUserData) {}
static LTRESULT HostClient_GetPixel(HSURFACE hSurface, uint32 x, uint32 y, HLTCOLOR *color) { return LT_OK; }
static LTRESULT HostClient_SetPixel(HSURFACE hSurface, uint32 x, uint32 y, HLTCOLOR color) { return LT_OK; }
static void HostClient_GetStringDimensions(HLTFONT hFont, HSTRING hString, int *sizeX, int *sizeY) {}
static void HostClient_DrawStringToSurface(HSURFACE hDest, HLTFONT hFont, HSTRING hString, LTRect *pRect, HLTCOLOR hForeColor, HLTCOLOR hBackColor) {}
static void HostClient_GetSurfaceDims(HSURFACE hSurf, uint32 *pWidth, uint32 *pHeight) {}
static bool HostClient_DrawBitmapToSurface(HSURFACE hDest, const char *pSourceBitmapName, const LTRect *pSrcRect, int destX, int destY) { return false; }
static LTRESULT HostClient_DrawSurfaceMasked(HSURFACE hDest, HSURFACE hSrc, HSURFACE hMask, LTRect *pSrcRect, int destX, int destY, HLTCOLOR hColor) { return LT_OK; }
static LTRESULT HostClient_DrawSurfaceSolidColor(HSURFACE hDest, HSURFACE hSrc, LTRect *pSrcRect, int destX, int destY, HLTCOLOR hTransColor, HLTCOLOR hFillColor) { return LT_OK; }
static LTRESULT HostClient_DrawSurfaceToSurface(HSURFACE hDest, HSURFACE hSrc, LTRect *pSrcRect, int destX, int destY) { return LT_OK; }
static LTRESULT HostClient_DrawSurfaceToSurfaceTransparent(HSURFACE hDest, HSURFACE hSrc, LTRect *pSrcRect, int destX, int destY, HLTCOLOR hColor) { return LT_OK; }
static LTRESULT HostClient_ScaleSurfaceToSurface(HSURFACE hDest, HSURFACE hSrc, LTRect *pDestRect, LTRect *pSrcRect) { return LT_OK; }
static LTRESULT HostClient_ScaleSurfaceToSurfaceTransparent(HSURFACE hDest, HSURFACE hSrc, LTRect *pDestRect, LTRect *pSrcRect, HLTCOLOR hColor) { return LT_OK; }
static LTRESULT HostClient_ScaleSurfaceToSurfaceSolidColor(HSURFACE hDest, HSURFACE hSrc, LTRect *pDestRect, LTRect *pSrcRect, HLTCOLOR hTransColor, HLTCOLOR hFillColor) { return LT_OK; }
static LTRESULT HostClient_WarpSurfaceToSurface(HSURFACE hDest, HSURFACE hSrc, LTWarpPt *pCoords, int nCoords) { return LT_OK; }
static LTRESULT HostClient_WarpSurfaceToSurfaceTransparent(HSURFACE hDest, HSURFACE hSrc, LTWarpPt *pCoords, int nCoords, HLTCOLOR hColor) { return LT_OK; }
static LTRESULT HostClient_WarpSurfaceToSurfaceSolidColor(HSURFACE hDest, HSURFACE hSrc, LTWarpPt *pCoords, int nCoords, HLTCOLOR hTransColor, HLTCOLOR hFillColor) { return LT_OK; }
static LTRESULT HostClient_TransformSurfaceToSurface(HSURFACE hDest, HSURFACE hSrc, LTFloatPt *pOrigin, int destX, int destY, float angle, float scaleX, float scaleY) { return LT_OK; }
static LTRESULT HostClient_TransformSurfaceToSurfaceTransparent(HSURFACE hDest, HSURFACE hSrc, LTFloatPt *pOrigin, int destX, int destY, float angle, float scaleX, float scaleY, HLTCOLOR hColor) { return LT_OK; }
static LTRESULT HostClient_FillRect(HSURFACE hDest, LTRect *pRect, HLTCOLOR hColor) { return LT_OK; }
static LTRESULT HostClient_GetSurfaceAlpha(HSURFACE hSurface, float &alpha) { return LT_OK; }
static LTRESULT HostClient_SetSurfaceAlpha(HSURFACE hSurface, float alpha) { return LT_OK; }
static void HostClient_StartCounter(LTCounter *pCounter) {}
static uint32 HostClient_EndCounter(LTCounter *pCounter) { return uint32(); }
static void HostClient_SetInputState(bool bOn) {}
static LTRESULT HostClient_ClearInput(void) { return LT_OK; }
static DeviceBinding * HostClient_GetDeviceBindings(uint32 nDevice) { return nullptr; }
static void HostClient_FreeDeviceBindings(DeviceBinding* pBindings) {}
static bool HostClient_ClearBinding(const char *pDeviceName, const char *pTriggerName) { return false; }
static bool HostClient_AddBinding(const char *pDeviceName, const char *pTriggerName, const char *pActionName, float rangeLow, float rangeHigh) { return false; }
static LTRESULT HostClient_StartDeviceTrack(uint32 nDevices, uint32 nBufferSize) { return LT_OK; }
static LTRESULT HostClient_TrackDevice(DeviceInput* pInputArray, uint32* pnInOut) { return LT_OK; }
static LTRESULT HostClient_EndDeviceTrack(void) { return LT_OK; }
static DeviceObject * HostClient_GetDeviceObjects(uint32 nDeviceFlags) { return nullptr; }
static void HostClient_FreeDeviceObjects(DeviceObject* pList) {}
static LTRESULT HostClient_GetDeviceName(uint32 nDeviceType, char* pStrBuffer, uint32 nBufferSize) { return LT_OK; }
static LTRESULT HostClient_IsDeviceEnabled(const char* strDeviceName, bool* pIsEnabled) { return LT_OK; }
static LTRESULT HostClient_EnableDevice(const char* strDeviceName) { return LT_OK; }
static uint32 HostClient_GetNumRawDevices(void) { return uint32(); }
static LTRESULT HostClient_GetRawDeviceData(uint32 deviceNum, void* pData) { return LT_OK; }
static uint32 HostClient_GetRawDeviceDataSize(uint32 deviceNum) { return uint32(); }
static uint32 HostClient_GetRawDeviceActuatorDataSize(uint32 deviceNum) { return uint32(); }
static LTRESULT HostClient_SetRawDeviceActuatorData(uint32 deviceNum, void* pData) { return LT_OK; }
static float HostClient_GetGameTime(void) { return 0.0f; }
static float HostClient_GetGameFrameTime(void) { return 0.0f; }
static void HostClient_DebugOut(const char *pMsg, ...) {}
static LTRESULT HostClient_GetSkyDef(SkyDef *pDef) { return LT_OK; }
static bool HostClient_IsCommandOn(int commandNum) { return false; }
static void HostClient_RunConsoleString(const char *pString) {}
static HLOCALOBJ HostClient_GetClientObject(void) { return HLOCALOBJ(); }
static void HostClient_GetGlobalLightScale(LTVector *pScale) {}
static void HostClient_SetGlobalLightScale(const LTVector *pScale) {}
static void HostClient_OffsetGlobalLightScale(const LTVector *pOffset) {}
static HLOCALOBJ HostClient_CreateObject(ObjectCreateStruct *pStruct) { return HLOCALOBJ(); }
static LTRESULT HostClient_GetObjectScale(HLOCALOBJ hObj, LTVector *pScale) { return LT_OK; }
static LTRESULT HostClient_SetObjectScale(HLOCALOBJ hObj, const LTVector *pScale) { return LT_OK; }
static void HostClient_SetObjectRotation(HLOCALOBJ hObj, const LTRotation *pRotation) {}
static void HostClient_GetObjectColor(HLOCALOBJ hObject, float *r, float *g, float *b, float *a) {}
static void HostClient_SetObjectColor(HLOCALOBJ hObject, float r, float g, float b, float a) {}
static void * HostClient_GetObjectUserData(HLOCALOBJ hObj) { return nullptr; }
static void HostClient_SetObjectUserData(HLOCALOBJ hObj, void *pData) {}
static LTRESULT HostClient_Get3DCameraPt(HLOCALOBJ hCamera, int sx, int sy, LTVector *pOut) { return LT_OK; }
static void HostClient_GetCameraFOV(HLOCALOBJ hObj, float *pX, float *pY) {}
static void HostClient_SetCameraFOV(HLOCALOBJ hObj, float fovX, float fovY) {}
static void HostClient_GetCameraRect(HLOCALOBJ hObj, bool *bFullscreen, int *left, int *top, int *right, int *bottom) {}
static void HostClient_SetCameraRect(HLOCALOBJ hObj, bool bFullscreen, int left, int top, int right, int bottom) {}
static bool HostClient_GetCameraLightAdd(HLOCALOBJ hCamera, LTVector *pAdd) { return false; }
static bool HostClient_SetCameraLightAdd(HLOCALOBJ hCamera, const LTVector *pAdd) { return false; }
static LTRESULT HostClient_SetupParticleSystem(HLOCALOBJ hObj, const char *pTextureName, float gravityAccel, uint32 flags, float particleRadius) { return LT_OK; }
static void HostClient_AddParticles(HLOCALOBJ hObj, uint32 nParticles, const LTVector *pMinOffset, const LTVector *pMaxOffset, const LTVector *pMinVelocity, const LTVector *pMaxVelocity, const LTVector *pMinColor, const LTVector *pMaxColor, float minLifetime, float maxLifetime) {}
static bool HostClient_GetParticles(HLOCALOBJ hObj, LTParticle **pHead, LTParticle **pTail) { return false; }
static void HostClient_RemoveParticle(HLOCALOBJ hSystem, LTParticle *pParticle) {}
static LTRESULT HostClient_OptimizeParticles(HLOCALOBJ hSystem) { return LT_OK; }
static LTRESULT HostClient_SortParticles(HLOCALOBJ hSystem, const LTVector& vDir, uint32 nNumIters) { return LT_OK; }
static LTRESULT HostClient_SetParticleSystemEffectShaderID(HLOCALOBJ hObj, uint32 EffectShaderID) { return LT_OK; }
static HLTLINE HostClient_GetNextLine(HLOCALOBJ hObj, HLTLINE hPrev) { return HLTLINE(); }
static void HostClient_GetLineInfo(HLTLINE hLine, LTLine *pLine) {}
static void HostClient_SetLineInfo(HLTLINE hLine, LTLine *pLine) {}
static HLTLINE HostClient_AddLine(HLOCALOBJ hObj, LTLine *pLine) { return HLTLINE(); }
static void HostClient_RemoveLine(HLOCALOBJ hObj, HLTLINE hLine) {}
static bool HostClient_SetupVolumeEffect(HLOCALOBJ hObj, VolumeEffectInfo& info) { return false; }
static LTRESULT HostClient_SetVolumeEffectEffectShaderID(HLOCALOBJ hObj, uint32 EffectShaderID) { return LT_OK; }
static bool HostClient_GetParticleBlockersInAABB(const LTVector& pos, const LTVector& dims, std::vector<uint32>& indices) { return false; }
static bool HostClient_GetParticleBlockerEdgesXZ(uint32 index, LTPlane& blockerPlane, uint32& numEdges, LTPlane*& edgePlanes) { return false; }
static bool HostClient_SetupPolyGrid(HLOCALOBJ hObj, uint32 width, uint32 height, uint32 nPGFlags, bool* pEnabledVerts) { return false; }
static LTRESULT HostClient_SetPolyGridTexture(HLOCALOBJ hObj, const char *pFilename) { return LT_OK; }
static LTRESULT HostClient_SetPolyGridEnvMap(HLOCALOBJ hObj, const char *pFilename) { return LT_OK; }
static LTRESULT HostClient_GetPolyGridTextureInfo(HLOCALOBJ hObj, float *xPan, float *yPan, float *xScale, float *yScale, float *fBaseReflection, float *fVolumeIOR) { return LT_OK; }
static LTRESULT HostClient_SetPolyGridTextureInfo(HLOCALOBJ hObj, float xPan, float yPan, float xScale, float yScale, float fBaseReflection, float fVolumeIOR) { return LT_OK; }
static LTRESULT HostClient_GetPolyGridInfo(HLOCALOBJ hObj, char **pBytes, uint32 *pWidth, uint32 *pHeight, PGColor **pColorTable) { return LT_OK; }
static LTRESULT HostClient_SetPolyGridEffectShaderID(HLOCALOBJ hObj, uint32 EffectShaderID) { return LT_OK; }
static LTRESULT HostClient_InitNetworking(const char *pDriver, uint32 dwFlags) { return LT_OK; }
static LTRESULT HostClient_GetServiceList(NetService* &pListHead) { return LT_OK; }
static LTRESULT HostClient_FreeServiceList(NetService *pListHead) { return LT_OK; }
static LTRESULT HostClient_SelectService(HNETSERVICE hNetService) { return LT_OK; }
static LTRESULT HostClient_GetSessionList(NetSession* &pListHead, const char *pInfo) { return LT_OK; }
static LTRESULT HostClient_FreeSessionList(NetSession *pListHead) { return LT_OK; }
static LTRESULT HostClient_AddInternetDriver(void) { return LT_OK; }
static LTRESULT HostClient_RemoveInternetDriver(void) { return LT_OK; }
static LTRESULT HostClient_IsLobbyLaunched(const char* sDriver) { return LT_OK; }
static LTRESULT HostClient_GetLobbyLaunchInfo(const char* sDriver, void** ppLobbyLaunchData) { return LT_OK; }
static LTRESULT HostClient_GetVersionInfoExt(LTVERSIONINFOEXT* pVersionInfo) { return LT_OK; }
static LTRESULT HostClient_GetPerformanceInfo(LTPERFORMANCEINFO* pPerformanceInfo) { return LT_OK; }
static LTRESULT HostClient_SetConsoleEnable(bool bEnable) { return LT_OK; }
static void HostClient_GetLightColor(HLOCALOBJ hObj, float *r, float *g, float *b) {}
static void HostClient_SetLightColor(HLOCALOBJ hObj, float r, float g, float b) {}
static float HostClient_GetLightRadius(HLOCALOBJ hObj) { return 0.0f; }
static void HostClient_SetLightRadius(HLOCALOBJ hObj, float radius) {}
static LTRESULT HostClient_ClipSprite(HLOCALOBJ hObj, HPOLY hPoly) { return LT_OK; }
static LTRESULT HostClient_GetPolyTextureFlags(HPOLY hPoly, uint32 *pFlags) { return LT_OK; }
static LTRESULT HostClient_SetModelHook(ModelHookFn fn, void *pUser) { return LT_OK; }
static LTRESULT HostClient_GetEngineHook(char *pName, void **pData) { return LT_OK; }
static LTRESULT HostClient_QueryGraphicDevice(LTGraphicsCaps* pCaps) { return LT_OK; }
static LTRESULT HostClient_GetTcpIpAddress(char* sAddress, uint32 dwBufferSize, uint16 &hostPort) { return LT_OK; }
inline void HostClient_Fill(ILTClient* o) {
    o->GetFileList = &HostClient_GetFileList;
    o->FreeFileList = &HostClient_FreeFileList;
    o->WriteConfigFileFields = &HostClient_WriteConfigFileFields;
    o->WriteConfigFile = &HostClient_WriteConfigFile;
    o->ReadConfigFile = &HostClient_ReadConfigFile;
    o->GetAxisOffsets = &HostClient_GetAxisOffsets;
    o->PlayJoystickEffect = &HostClient_PlayJoystickEffect;
    o->InitMusic = &HostClient_InitMusic;
    o->LoadSong = &HostClient_LoadSong;
    o->DestroyAllSongs = &HostClient_DestroyAllSongs;
    o->PlayList = &HostClient_PlayList;
    o->StopMusic = &HostClient_StopMusic;
    o->PauseMusic = &HostClient_PauseMusic;
    o->ResumeMusic = &HostClient_ResumeMusic;
    o->AddSongToPlayList = &HostClient_AddSongToPlayList;
    o->DeletePlayList = &HostClient_DeletePlayList;
    o->GetMusicVolume = &HostClient_GetMusicVolume;
    o->SetMusicVolume = &HostClient_SetMusicVolume;
    o->GetSoundTimer = &HostClient_GetSoundTimer;
    o->GetSoundData = &HostClient_GetSoundData;
    o->GetSoundOffset = &HostClient_GetSoundOffset;
    o->IsDone = &HostClient_IsDone;
    o->GetListener = &HostClient_GetListener;
    o->IntersectSegment = &HostClient_IntersectSegment;
    o->IntersectSweptSphere = &HostClient_IntersectSweptSphere;
    o->CastRay = &HostClient_CastRay;
    o->SetObjectPosAndRotation = &HostClient_SetObjectPosAndRotation;
    o->FindObjectsInSphere = &HostClient_FindObjectsInSphere;
    o->FindObjectsInBox = &HostClient_FindObjectsInBox;
    o->GetGameMode = &HostClient_GetGameMode;
    o->StartGame = &HostClient_StartGame;
    o->GetLocalClientID = &HostClient_GetLocalClientID;
    o->Disconnect = &HostClient_Disconnect;
    o->IsConnected = &HostClient_IsConnected;
    o->Shutdown = &HostClient_Shutdown;
    o->ShutdownWithMessage = &HostClient_ShutdownWithMessage;
    o->FlipScreen = &HostClient_FlipScreen;
    o->Start3D = &HostClient_Start3D;
    o->RenderCamera = &HostClient_RenderCamera;
    o->ClearScreen = &HostClient_ClearScreen;
    o->RenderObjects = &HostClient_RenderObjects;
    o->MakeCubicEnvMap = &HostClient_MakeCubicEnvMap;
    o->StartOptimized2D = &HostClient_StartOptimized2D;
    o->EndOptimized2D = &HostClient_EndOptimized2D;
    o->SetOptimized2DBlend = &HostClient_SetOptimized2DBlend;
    o->GetOptimized2DBlend = &HostClient_GetOptimized2DBlend;
    o->End3D = &HostClient_End3D;
    o->GetOptimized2DColor = &HostClient_GetOptimized2DColor;
    o->SetOptimized2DColor = &HostClient_SetOptimized2DColor;
    o->GetRenderModes = &HostClient_GetRenderModes;
    o->RelinquishRenderModes = &HostClient_RelinquishRenderModes;
    o->GetRenderMode = &HostClient_GetRenderMode;
    o->SetRenderMode = &HostClient_SetRenderMode;
    o->ShutdownRender = &HostClient_ShutdownRender;
    o->CreateFont = &HostClient_CreateFont;
    o->DeleteFont = &HostClient_DeleteFont;
    o->SetFontExtraSpace = &HostClient_SetFontExtraSpace;
    o->GetFontExtraSpace = &HostClient_GetFontExtraSpace;
    o->RegisterConsoleProgram = &HostClient_RegisterConsoleProgram;
    o->UnregisterConsoleProgram = &HostClient_UnregisterConsoleProgram;
    o->GetConsoleVar = &HostClient_GetConsoleVar;
    o->CreateColor = &HostClient_CreateColor;
    o->DeleteColor = &HostClient_DeleteColor;
    o->SetupColor1 = &HostClient_SetupColor1;
    o->SetupColor2 = &HostClient_SetupColor2;
    o->GetBorderSize = &HostClient_GetBorderSize;
    o->OptimizeSurface = &HostClient_OptimizeSurface;
    o->UnoptimizeSurface = &HostClient_UnoptimizeSurface;
    o->GetScreenSurface = &HostClient_GetScreenSurface;
    o->CreateHeightmapFromBitmap = &HostClient_CreateHeightmapFromBitmap;
    o->CreateSurfaceFromBitmap = &HostClient_CreateSurfaceFromBitmap;
    o->FreeHeightmap = &HostClient_FreeHeightmap;
    o->CreateSurfaceFromString = &HostClient_CreateSurfaceFromString;
    o->CreateSurface = &HostClient_CreateSurface;
    o->DeleteSurface = &HostClient_DeleteSurface;
    o->GetSurfaceUserData = &HostClient_GetSurfaceUserData;
    o->SetSurfaceUserData = &HostClient_SetSurfaceUserData;
    o->GetPixel = &HostClient_GetPixel;
    o->SetPixel = &HostClient_SetPixel;
    o->GetStringDimensions = &HostClient_GetStringDimensions;
    o->DrawStringToSurface = &HostClient_DrawStringToSurface;
    o->GetSurfaceDims = &HostClient_GetSurfaceDims;
    o->DrawBitmapToSurface = &HostClient_DrawBitmapToSurface;
    o->DrawSurfaceMasked = &HostClient_DrawSurfaceMasked;
    o->DrawSurfaceSolidColor = &HostClient_DrawSurfaceSolidColor;
    o->DrawSurfaceToSurface = &HostClient_DrawSurfaceToSurface;
    o->DrawSurfaceToSurfaceTransparent = &HostClient_DrawSurfaceToSurfaceTransparent;
    o->ScaleSurfaceToSurface = &HostClient_ScaleSurfaceToSurface;
    o->ScaleSurfaceToSurfaceTransparent = &HostClient_ScaleSurfaceToSurfaceTransparent;
    o->ScaleSurfaceToSurfaceSolidColor = &HostClient_ScaleSurfaceToSurfaceSolidColor;
    o->WarpSurfaceToSurface = &HostClient_WarpSurfaceToSurface;
    o->WarpSurfaceToSurfaceTransparent = &HostClient_WarpSurfaceToSurfaceTransparent;
    o->WarpSurfaceToSurfaceSolidColor = &HostClient_WarpSurfaceToSurfaceSolidColor;
    o->TransformSurfaceToSurface = &HostClient_TransformSurfaceToSurface;
    o->TransformSurfaceToSurfaceTransparent = &HostClient_TransformSurfaceToSurfaceTransparent;
    o->FillRect = &HostClient_FillRect;
    o->GetSurfaceAlpha = &HostClient_GetSurfaceAlpha;
    o->SetSurfaceAlpha = &HostClient_SetSurfaceAlpha;
    o->StartCounter = &HostClient_StartCounter;
    o->EndCounter = &HostClient_EndCounter;
    o->SetInputState = &HostClient_SetInputState;
    o->ClearInput = &HostClient_ClearInput;
    o->GetDeviceBindings = &HostClient_GetDeviceBindings;
    o->FreeDeviceBindings = &HostClient_FreeDeviceBindings;
    o->ClearBinding = &HostClient_ClearBinding;
    o->AddBinding = &HostClient_AddBinding;
    o->StartDeviceTrack = &HostClient_StartDeviceTrack;
    o->TrackDevice = &HostClient_TrackDevice;
    o->EndDeviceTrack = &HostClient_EndDeviceTrack;
    o->GetDeviceObjects = &HostClient_GetDeviceObjects;
    o->FreeDeviceObjects = &HostClient_FreeDeviceObjects;
    o->GetDeviceName = &HostClient_GetDeviceName;
    o->IsDeviceEnabled = &HostClient_IsDeviceEnabled;
    o->EnableDevice = &HostClient_EnableDevice;
    o->GetNumRawDevices = &HostClient_GetNumRawDevices;
    o->GetRawDeviceData = &HostClient_GetRawDeviceData;
    o->GetRawDeviceDataSize = &HostClient_GetRawDeviceDataSize;
    o->GetRawDeviceActuatorDataSize = &HostClient_GetRawDeviceActuatorDataSize;
    o->SetRawDeviceActuatorData = &HostClient_SetRawDeviceActuatorData;
    o->GetGameTime = &HostClient_GetGameTime;
    o->GetGameFrameTime = &HostClient_GetGameFrameTime;
    o->DebugOut = &HostClient_DebugOut;
    o->GetSkyDef = &HostClient_GetSkyDef;
    o->IsCommandOn = &HostClient_IsCommandOn;
    o->RunConsoleString = &HostClient_RunConsoleString;
    o->GetClientObject = &HostClient_GetClientObject;
    o->GetGlobalLightScale = &HostClient_GetGlobalLightScale;
    o->SetGlobalLightScale = &HostClient_SetGlobalLightScale;
    o->OffsetGlobalLightScale = &HostClient_OffsetGlobalLightScale;
    o->CreateObject = &HostClient_CreateObject;
    o->GetObjectScale = &HostClient_GetObjectScale;
    o->SetObjectScale = &HostClient_SetObjectScale;
    o->SetObjectRotation = &HostClient_SetObjectRotation;
    o->GetObjectColor = &HostClient_GetObjectColor;
    o->SetObjectColor = &HostClient_SetObjectColor;
    o->GetObjectUserData = &HostClient_GetObjectUserData;
    o->SetObjectUserData = &HostClient_SetObjectUserData;
    o->Get3DCameraPt = &HostClient_Get3DCameraPt;
    o->GetCameraFOV = &HostClient_GetCameraFOV;
    o->SetCameraFOV = &HostClient_SetCameraFOV;
    o->GetCameraRect = &HostClient_GetCameraRect;
    o->SetCameraRect = &HostClient_SetCameraRect;
    o->GetCameraLightAdd = &HostClient_GetCameraLightAdd;
    o->SetCameraLightAdd = &HostClient_SetCameraLightAdd;
    o->SetupParticleSystem = &HostClient_SetupParticleSystem;
    o->AddParticles = &HostClient_AddParticles;
    o->GetParticles = &HostClient_GetParticles;
    o->RemoveParticle = &HostClient_RemoveParticle;
    o->OptimizeParticles = &HostClient_OptimizeParticles;
    o->SortParticles = &HostClient_SortParticles;
    o->SetParticleSystemEffectShaderID = &HostClient_SetParticleSystemEffectShaderID;
    o->GetNextLine = &HostClient_GetNextLine;
    o->GetLineInfo = &HostClient_GetLineInfo;
    o->SetLineInfo = &HostClient_SetLineInfo;
    o->AddLine = &HostClient_AddLine;
    o->RemoveLine = &HostClient_RemoveLine;
    o->SetupVolumeEffect = &HostClient_SetupVolumeEffect;
    o->SetVolumeEffectEffectShaderID = &HostClient_SetVolumeEffectEffectShaderID;
    o->GetParticleBlockersInAABB = &HostClient_GetParticleBlockersInAABB;
    o->GetParticleBlockerEdgesXZ = &HostClient_GetParticleBlockerEdgesXZ;
    o->SetupPolyGrid = &HostClient_SetupPolyGrid;
    o->SetPolyGridTexture = &HostClient_SetPolyGridTexture;
    o->SetPolyGridEnvMap = &HostClient_SetPolyGridEnvMap;
    o->GetPolyGridTextureInfo = &HostClient_GetPolyGridTextureInfo;
    o->SetPolyGridTextureInfo = &HostClient_SetPolyGridTextureInfo;
    o->GetPolyGridInfo = &HostClient_GetPolyGridInfo;
    o->SetPolyGridEffectShaderID = &HostClient_SetPolyGridEffectShaderID;
    o->InitNetworking = &HostClient_InitNetworking;
    o->GetServiceList = &HostClient_GetServiceList;
    o->FreeServiceList = &HostClient_FreeServiceList;
    o->SelectService = &HostClient_SelectService;
    o->GetSessionList = &HostClient_GetSessionList;
    o->FreeSessionList = &HostClient_FreeSessionList;
    o->AddInternetDriver = &HostClient_AddInternetDriver;
    o->RemoveInternetDriver = &HostClient_RemoveInternetDriver;
    o->IsLobbyLaunched = &HostClient_IsLobbyLaunched;
    o->GetLobbyLaunchInfo = &HostClient_GetLobbyLaunchInfo;
    o->GetVersionInfoExt = &HostClient_GetVersionInfoExt;
    o->GetPerformanceInfo = &HostClient_GetPerformanceInfo;
    o->SetConsoleEnable = &HostClient_SetConsoleEnable;
    o->GetLightColor = &HostClient_GetLightColor;
    o->SetLightColor = &HostClient_SetLightColor;
    o->GetLightRadius = &HostClient_GetLightRadius;
    o->SetLightRadius = &HostClient_SetLightRadius;
    o->ClipSprite = &HostClient_ClipSprite;
    o->GetPolyTextureFlags = &HostClient_GetPolyTextureFlags;
    o->SetModelHook = &HostClient_SetModelHook;
    o->GetEngineHook = &HostClient_GetEngineHook;
    o->QueryGraphicDevice = &HostClient_QueryGraphicDevice;
    o->GetTcpIpAddress = &HostClient_GetTcpIpAddress;
}
