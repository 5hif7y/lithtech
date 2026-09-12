#pragma once
#include "Platform/platform.h"
#include <type_traits>
#include <iltstream.h>
class HostStream : public ILTStream {
public:
void Release() override {}
LTRESULT Read(void *pData, uint32 size) override { return LT_OK; }
LTRESULT ReadString(char *pStr, uint32 maxBytes) override { return LT_OK; }
LTRESULT ErrorStatus() override { return LT_OK; }
LTRESULT SeekTo(uint32 offset) override { return LT_OK; }
LTRESULT GetPos(uint32 *offset) override { return LT_OK; }
LTRESULT GetLen(uint32 *len) override { return LT_OK; }
LTRESULT WriteStream(ILTStream &dsSource, uint32 dwMin, uint32 dwMax) override { return LT_OK; }
LTRESULT Write(const void *pData, uint32 size) override { return LT_OK; }
LTRESULT WriteString(const char *pStr) override { return LT_OK; }
};
