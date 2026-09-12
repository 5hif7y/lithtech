#pragma once
#include "Platform/platform.h"
#include <type_traits>
#include <iltmodel.h>
class HostModelClient : public ILTModelClient {
public:
LTRESULT GetRenderStyle(HOBJECT hModel, uint32 hPiece, CRenderStyle** ppRenderStyle) override { return LT_OK; }
LTRESULT SetRenderStyle(HOBJECT pModel, uint32 hPiece, CRenderStyle* pRenderStyle) override { return LT_OK; }
LTRESULT UncacheModelDB(const char *filename) override { return LT_OK; }
LTRESULT CacheModelDB(const char *Filename, HMODELDB & hModelDB) override { return LT_OK; }
LTRESULT UncacheModelDB(HMODELDB &hModelDB) override { return LT_OK; }
LTRESULT IsModelDBLoaded(HMODELDB hModelDB) override { return LT_OK; }
LTRESULT GetSocket(HOBJECT hObj, const char *pSocketName, HMODELSOCKET &hSocket) override { return LT_OK; }
LTRESULT GetSocketTransform(HOBJECT hObj, HMODELSOCKET hSocket, LTransform &transform, bool bWorldSpace) override { return LT_OK; }
LTRESULT GetNumPieces(HOBJECT hObj, uint32 & NumPieces) override { return LT_OK; }
LTRESULT GetPiece(HOBJECT hObj, const char *pPieceName, HMODELPIECE &hPiece) override { return LT_OK; }
LTRESULT GetPieceHideStatus(HOBJECT hObj, HMODELPIECE hPiece, bool &bHidden) override { return LT_OK; }
LTRESULT SetPieceHideStatus(HOBJECT hObj, HMODELPIECE hPiece, bool bHidden) override { return LT_OK; }
LTRESULT GetNode(HOBJECT hObj, const char *pNodeName, HMODELNODE &hNode) override { return LT_OK; }
LTRESULT GetNodeName(HOBJECT hObj, HMODELNODE hNode, char *name, uint32 maxlen) override { return LT_OK; }
LTRESULT GetNodeTransform(HOBJECT hObj, HMODELNODE hNode, LTransform &transform, bool bWorldSpace) override { return LT_OK; }
LTRESULT GetNextNode(HOBJECT hObject, HMODELNODE hNode, HMODELNODE &pNext) override { return LT_OK; }
LTRESULT GetRootNode(HOBJECT hObj, HMODELNODE &hNode) override { return LT_OK; }
LTRESULT GetNumChildren(HOBJECT hObj, HMODELNODE hNode, uint32 &NumChildren) override { return LT_OK; }
LTRESULT GetChild(HOBJECT hObj, HMODELNODE parent, uint32 index, HMODELNODE &child) override { return LT_OK; }
LTRESULT GetParent(HOBJECT hObj, HMODELNODE node, HMODELNODE &parent) override { return LT_OK; }
LTRESULT GetNumNodes(HOBJECT hObj, uint32 &num_nodes) override { return LT_OK; }
LTRESULT GetBindPoseNodeTransform(HOBJECT hObj, HMODELNODE node, LTMatrix &mat) override { return LT_OK; }
LTRESULT AddNodeControlFn(HOBJECT hObj, HMODELNODE hNode, NodeControlFn pFn, void *pUserData) override { return LT_OK; }
LTRESULT AddNodeControlFn(HOBJECT hObj, NodeControlFn pFn, void *pUserData) override { return LT_OK; }
LTRESULT RemoveNodeControlFn(HOBJECT hObj, HMODELNODE hNode, NodeControlFn pFn, void* pUserData) override { return LT_OK; }
LTRESULT RemoveNodeControlFn(HOBJECT hObj, NodeControlFn pFn, void* pUserData) override { return LT_OK; }
LTRESULT UpdateMainTracker(HOBJECT hObj, float fUpdateDelta) override { return LT_OK; }
LTRESULT GetAnimLength(HOBJECT hModel, HMODELANIM hAnim, uint32 &length) override { return LT_OK; }
LTRESULT FindWeightSet(HOBJECT hObj, const char *pSetName, HMODELWEIGHTSET &hSet) override { return LT_OK; }
LTRESULT GetMainTracker(HOBJECT hModel, ANIMTRACKERID &TrackerID) override { return LT_OK; }
LTRESULT GetPlaybackState(HOBJECT hModel, ANIMTRACKERID TrackerID, uint32 &flags) override { return LT_OK; }
LTRESULT AddTracker(HOBJECT hModel, ANIMTRACKERID TrackerID) override { return LT_OK; }
LTRESULT RemoveTracker(HOBJECT hModel, ANIMTRACKERID TrackerID) override { return LT_OK; }
LTRESULT GetAnimIndex(HOBJECT hModel, const char *pAnimName, uint32 & anim_index) override { return LT_OK; }
LTRESULT GetCurAnim(HOBJECT hModel, ANIMTRACKERID TrackerID, HMODELANIM &hAnim) override { return LT_OK; }
LTRESULT SetCurAnim(HOBJECT hModel, ANIMTRACKERID TrackerID, HMODELANIM hAnim) override { return LT_OK; }
LTRESULT ResetAnim(HOBJECT hModel, ANIMTRACKERID TrackerID) override { return LT_OK; }
LTRESULT GetLooping(HOBJECT hModel, ANIMTRACKERID TrackerID) override { return LT_OK; }
LTRESULT SetLooping(HOBJECT hModel, ANIMTRACKERID TrackerID, bool bLooping) override { return LT_OK; }
LTRESULT GetPlaying(HOBJECT hModel, ANIMTRACKERID TrackerID) override { return LT_OK; }
LTRESULT SetPlaying(HOBJECT hModel, ANIMTRACKERID TrackerID, bool bPlaying) override { return LT_OK; }
LTRESULT GetCurAnimLength(HOBJECT hModel, ANIMTRACKERID TrackerID, uint32 &length) override { return LT_OK; }
LTRESULT GetCurAnimTime(HOBJECT hModel, ANIMTRACKERID TrackerID, uint32 &curTime) override { return LT_OK; }
LTRESULT SetCurAnimTime(HOBJECT hModel, ANIMTRACKERID TrackerID, uint32 curTime) override { return LT_OK; }
LTRESULT SetHintNode(HOBJECT hModel, ANIMTRACKERID TrackerID, HMODELNODE hNode) override { return LT_OK; }
LTRESULT SetAnimRate(HOBJECT hModel, ANIMTRACKERID TrackerID, float fRate) override { return LT_OK; }
LTRESULT GetAnimRate(HOBJECT hModel, ANIMTRACKERID TrackerID, float &fRate) override { return LT_OK; }
LTRESULT GetWeightSet(HOBJECT hModel, ANIMTRACKERID TrackerID, HMODELWEIGHTSET &hSet) override { return LT_OK; }
LTRESULT SetWeightSet(HOBJECT hModel, ANIMTRACKERID TrackerID, HMODELWEIGHTSET hSet) override { return LT_OK; }
LTRESULT GetNumLODs(HOBJECT hModel, HMODELPIECE hPiece, uint32 &num_lods) override { return LT_OK; }
LTRESULT GetLODValFromDist(HOBJECT hModel, HMODELPIECE hPiece, float dist, uint32 &lod_index) override { return LT_OK; }
LTRESULT ApplyAnimations(HOBJECT hModel) override { return LT_OK; }
LTRESULT GetFilenames(HOBJECT hObj, char *pFilename, uint32 fileBufLen, char *pSkinName, uint32 skinBufLen) override { return LT_OK; }
LTRESULT GetModelDBFilename(HOBJECT hObj, char *pRetFilename, uint32 fileBufLen) override { return LT_OK; }
LTRESULT GetSkinFilename(HOBJECT hObj, uint32 skin_index, char *pRetFilenames, uint32 fileBufLen) override { return LT_OK; }
LTRESULT GetNumModelOBBs(HOBJECT hObj, uint32 & num_obbs) override { return LT_OK; }
LTRESULT GetModelOBBCopy(HOBJECT hObj, ModelOBB *) override { return LT_OK; }
LTRESULT UpdateModelOBB(HOBJECT hObj, ModelOBB *) override { return LT_OK; }
const char* _InterfaceImplementation() override { return ""; }
};
