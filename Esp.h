#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include "offsets.h"
#include "vector3.h"
using namespace offsets_;
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

struct Vec4 { float x, y, z, w; };
class ent {
public:
	union {
		DEFINE_MEMBER_N(bool,inativo,offsets_::m_bDormant);
		DEFINE_MEMBER_N(int, health, offsets_::m_iHealth);
		DEFINE_MEMBER_N(int, team, offsets_::m_iTeamNum);
		DEFINE_MEMBER_N(Vec3, m_vecOrigin, offsets_::m_vecOrigin);
		DEFINE_MEMBER_N(bool, inativo, offsets_::m_bDormant);
		DEFINE_MEMBER_N(int, boneArr, offsets_::m_dwBoneMatrix);
	};
};

class EntList_ {
public:
	struct ent* ent;
	char pad[12];
};

class Entlist {
public:
	EntList_ entlist[64];
};

class Esp
{
public:
	uintptr_t entListAddr = dwEntityList;
	uintptr_t engine = (uintptr_t)GetModuleHandle(L"engine.dll");
	uintptr_t client = (uintptr_t)GetModuleHandle(L"client.dll");
	ent* localPlayer = (ent*)(client + dwLocalPlayer);
	Entlist* entlist = (Entlist*)(client + dwEntityList);
	ID3DXLine* m_Line;
	float viewMatrix[16];
	ID3DXFont* font;
	void update();
	bool isValid(ent* ent);
	bool Wrld2Screen(Vec3 pos, Vec3& screen);
	Vec3 bonePos(ent*, int bone);
};

