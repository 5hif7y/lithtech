#pragma once
#include "Platform/platform.h"
#include <type_traits>
#include <iltmessage.h>
#include <iltrefcount.h>
class HostMessageWrite : public ILTMessage_Write {
public:
void Reset() override {}
ILTMessage_Read* Read() override { return nullptr; }
uint32 Size() const override { return uint32(); }
void WriteBits(uint32 nValue, uint32 nSize) override {}
void WriteBits64(uint64 nValue, uint32 nSize) override {}
void WriteData(const void *pData, uint32 nSize) override {}
void WriteMessage(const ILTMessage_Read *pMsg) override {}
void WriteMessageRaw(const ILTMessage_Read *pMsg) override {}
void WriteString(const char *pString) override {}
void WriteHString(HSTRING hString) override {}
void WriteCompLTVector(const LTVector &vVec) override {}
void WriteCompPos(const LTVector &vPos) override {}
void WriteCompLTRotation(const LTRotation &cRotation) override {}
void WriteHStringFormatted(int nStringCode, ...) override {}
void WriteHStringArgList(int nStringCode, va_list *pList) override {}
void WriteStringAsHString(const char *pString) override {}
void WriteObject(HOBJECT hObj) override {}
void WriteYRotation(const LTRotation &cRotation) override {}
void Free() override {}
};
