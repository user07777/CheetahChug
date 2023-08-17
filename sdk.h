#pragma once
#include <Windows.h>
#include "Vector3.h"
#include "csgoVector.h"
#include "offsets.h"
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

typedef void*(_cdecl* createInterface)(const char* name, int* returnCode);

void*	getInterface(const wchar_t* dll, const char* interface_);

class _Ent
{
public:
    union
    {
        DEFINE_MEMBER_N(vec3, origin, offsets_::m_vecOrigin);
        DEFINE_MEMBER_N(int, clientId, 0x64);
        DEFINE_MEMBER_N(vec3, m_vecViewOffset, offsets_::m_vecViewOffset);
    };
}; //Size: 0x0284

class _EntListObj
{
public:
    class Ent* ent; //0x0000
    char pad_0004[12]; //0x0004
}; //Size: 0x0010

class _EntList
{
public:
    _EntListObj entListObjs[32]; //0x0000
}; //Size: 0x0200