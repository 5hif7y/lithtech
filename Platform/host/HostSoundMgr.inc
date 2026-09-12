#pragma once
#include "Platform/platform.h"
#include <type_traits>
#include <iltsoundmgr.h>
class HostSoundMgr : public ILTClientSoundMgr {
public:
LTRESULT GetSound3DProviderLists(Sound3DProvider *&pSound3DProviderList, bool bVerify, uint32 uiMax3DVoices) override { return LT_OK; }
LTRESULT ReleaseSound3DProviderList(Sound3DProvider *pSound3DProviderList) override { return LT_OK; }
LTRESULT InitSound(InitSoundInfo *pSoundInfo) override { return LT_OK; }
LTRESULT SetListenerDoppler(float fVolume) override { return LT_OK; }
LTRESULT GetVolume(uint16 &nVolume) override { return LT_OK; }
LTRESULT SetVolume(uint16 nVolume) override { return LT_OK; }
LTRESULT UpdateVolumeSettings() override { return LT_OK; }
LTRESULT SetReverbProperties(ReverbProperties *pReverbProperties) override { return LT_OK; }
LTRESULT GetReverbProperties(ReverbProperties *pReverbProperties) override { return LT_OK; }
LTRESULT SetSoundOcclusion(HLTSOUND hSound, LTFLOAT fLevel) override { return LT_OK; }
LTRESULT GetSoundOcclusion(HLTSOUND hSound, LTFLOAT *pLevel) override { return LT_OK; }
LTRESULT SetSoundObstruction(HLTSOUND hSound, LTFLOAT fLevel) override { return LT_OK; }
LTRESULT GetSoundObstruction(HLTSOUND hSound, LTFLOAT *pLevel) override { return LT_OK; }
LTRESULT SetSoundFilter(const char *pFilter) override { return LT_OK; }
LTRESULT SetSoundFilterParam(const char *pParam, float fValue) override { return LT_OK; }
LTRESULT EnableSoundFilter(bool bEnable) override { return LT_OK; }
LTRESULT SetSoundPosition(HLTSOUND hSound, LTVector *pPos) override { return LT_OK; }
LTRESULT GetSoundPosition(HLTSOUND hSound, LTVector *pPos) override { return LT_OK; }
LTRESULT PauseSounds() override { return LT_OK; }
LTRESULT ResumeSounds() override { return LT_OK; }
LTRESULT SetSoundClassMultiplier(uint8 nSoundClass, float fMult, bool bUseGlobalVolume) override { return LT_OK; }
LTRESULT GetSoundClassMultiplier(uint8 nSoundClass, float* pfMult) override { return LT_OK; }
LTRESULT SetListener(bool bListenerInClient, LTVector *pPos, LTRotation *pRot, bool bTeleport) override { return LT_OK; }
LTRESULT PlaySound(PlaySoundInfo *pPlaySoundInfo, HLTSOUND &hResult) override { return LT_OK; }
LTRESULT GetSoundDuration(HLTSOUND hSound, LTFLOAT &fDuration) override { return LT_OK; }
LTRESULT IsSoundDone(HLTSOUND hSound, bool &bDone) override { return LT_OK; }
LTRESULT KillSound(HLTSOUND hSound) override { return LT_OK; }
LTRESULT KillSoundLoop(HLTSOUND hSound) override { return LT_OK; }
const char* _InterfaceImplementation() override { return ""; }
};
