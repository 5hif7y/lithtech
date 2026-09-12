#pragma once
#include "Platform/platform.h"
#include <type_traits>
#include <iltcommon.h>
class HostCommon : public ILTCommon {
public:
LTRESULT GetObjectType(HOBJECT hObj, uint32 *type) override { return LT_OK; }
LTRESULT CompressVector(const LTVector& vec, CompVector& CompV) override { return LT_OK; }
LTRESULT UncompressVector(const CompVector& CompV, LTVector& vec) override { return LT_OK; }
LTRESULT CompressRotation(const LTRotation& rot, CompRot& CompR) override { return LT_OK; }
LTRESULT Parse(ConParse *pParse) override { return LT_OK; }
LTRESULT GetModelAnimUserDims(HOBJECT hObject, LTVector *pDims, HMODELANIM hAnim) override { return LT_OK; }
LTRESULT GetRotationVectors(LTRotation &rot, LTVector &up, LTVector &right, LTVector &forward) override { return LT_OK; }
LTRESULT SetupEuler(LTRotation &rot, float pitch, float yaw, float roll) override { return LT_OK; }
LTRESULT CreateMessage(ILTMessage_Write* &pMsg) override { return LT_OK; }
LTRESULT GetPointStatus(const LTVector *pPoint) override { return LT_OK; }
LTRESULT GetPointShade(const LTVector *pPoint, LTVector *pColor) override { return LT_OK; }
LTRESULT GetPolyTextureFlags(HPOLY hPoly, uint32 *pFlags) override { return LT_OK; }
LTRESULT GetPolyPlane(HPOLY hPoly, LTPlane *pPlane) override { return LT_OK; }
LTRESULT UncompressRotation(const CompRot& CompR, LTRotation& rot) override { return LT_OK; }
LTRESULT SetObjectResource(HOBJECT hObj, EObjectResource eType, uint32 nIndex, const char* pszResource) override { return LT_OK; }
LTRESULT SetObjectFilenames(HOBJECT hObj, ObjectCreateStruct* pStruct) override { return LT_OK; }
LTRESULT GetObjectFlags(const HOBJECT hObj, const ObjFlagType flagType, uint32 &dwFlags) override { return LT_OK; }
LTRESULT SetObjectFlags(HOBJECT hObj, const ObjFlagType flagType, uint32 dwFlags, uint32 dwMask) override { return LT_OK; }
LTRESULT GetAttachmentObjects(HATTACHMENT hAttachment, HOBJECT &hParent, HOBJECT &hChild) override { return LT_OK; }
LTRESULT GetAttachments(HLOCALOBJ hObj, HLOCALOBJ *inList, uint32 inListSize, uint32 &outListSize, uint32 &outNumAttachments) override { return LT_OK; }
LTRESULT GetAttachedModelNodeTransform(HATTACHMENT hAttachment, HMODELNODE hNode, LTransform &transform) override { return LT_OK; }
LTRESULT GetAttachmentTransform(HATTACHMENT hAttachment, LTransform &transform, bool bWorldSpace) override { return LT_OK; }
LTRESULT GetAttachedModelSocketTransform(HATTACHMENT hAttachment, HMODELSOCKET hSocket, LTransform &transform) override { return LT_OK; }
LTRESULT NumAttachments(HLOCALOBJ hObject, uint32 &dwAttachCount) override { return LT_OK; }
const char* _InterfaceImplementation() override { return ""; }
};
