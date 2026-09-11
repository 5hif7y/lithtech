#include "ltmodule.h"
#include "iltbaseclass.h"
#include "LTEulerAngles.h"
#include "ltrotation.h"
// Stubs for Engine linking so demos become ELF executables (graphify 18 demos)
CAPIHolderBase::CAPIHolderBase(const char*, int) {}
CAPIHolderBase::~CAPIHolderBase() {}
void CInterfaceDatabase::AddHolder(CAPIHolderBase*) {}
void CInterfaceDatabase::RemoveHolder(CAPIHolderBase*) {}
void CInterfaceDatabase::AddAPI(IBase*, const char*, int) {}
void CInterfaceDatabase::RemoveAPI(IBase*, const char*) {}
void CInterfaceDatabase::DatabaseItemCountInc() {}
void CInterfaceDatabase::DatabaseItemCountDec() {}
// Math stubs
EulerAngles Eul_FromQuat(LTRotation& q, int order) { (void)q; (void)order; EulerAngles e; e.Init(0,0,0,0); return e; }
