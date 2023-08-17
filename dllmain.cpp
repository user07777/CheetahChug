// dllmain.cpp : Define o ponto de entrada para o aplicativo DLL.
#include <iostream>
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#include <TlHelp32.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <hack.cpp>
#include "MemMan.h"
#include "offsets.h"
#include "vector3.h"
#include "draw.h"
#include "sdk.h"
#include "Esp.h"
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

using namespace offsets_;
MemMan MemClass;
bool exit_ = true;
Esp* esp;

struct clRender {
    BYTE r, g, b;
} Render;


struct offsets
{
    uintptr_t localPlayer = dwLocalPlayer;
    uintptr_t forceLMB = dwForceAttack;
    uintptr_t entityList = dwEntityList;
    uintptr_t crosshair = m_iCrosshairId;
    uintptr_t team = m_iTeamNum;
    uintptr_t health = m_iHealth;
    uintptr_t glowObj = offsets_::dwGlowObjectManager;
    uintptr_t clientState = dwClientState;
    uintptr_t viewAngles = dwClientState_ViewAngles;
    uintptr_t shotsFired = m_iShotsFired;
    uintptr_t aimPunch = m_aimPunchAngle;

} offset;

struct variables
{
    uintptr_t localPlayer;
    uintptr_t gameModule;
    uintptr_t engine;
    Vec3 origin;
    Vec3 viewOffset;
    int myTeam;
} val;
void bright() {
    float  b = 6.0f;
    int p = MemClass.readMem<int>(val.engine + model_ambient_min);
    int xorP = *(int*)&b ^ p;
    MemClass.writeMem<int>(val.engine + model_ambient_min, xorP);
}
void glow() {
    clRender clrender{};
    clrender.r = 89;
    clrender.g = 255;
    clrender.b = 0;
    uintptr_t _glowObj = MemClass.readMem<uintptr_t>(val.gameModule + dwGlowObjectManager);
    for (short int i = 1; i < 64; i++) {
        uintptr_t entity = MemClass.readMem<uintptr_t>(val.gameModule + dwEntityList + i * 0x10);
        if (entity != NULL) {
            int glowIndex__ = MemClass.readMem<int>(entity + 0x10488);
            int entityTeam = MemClass.readMem<int>(entity + m_iTeamNum);
            int hp = MemClass.readMem<int>(entity + m_iHealth);
            MemClass.writeMem<clRender>(entity + m_clrRender, clrender);
            if (entityTeam == val.myTeam) {
                // std::cout << std::hex << _glowObj + ((glowIndex__ * 0x38) + 0x4) << "\n";
                MemClass.writeMem<float>(_glowObj + ((glowIndex__ * 0x38) + 0x8), 0.0f);
                MemClass.writeMem<float>(_glowObj + ((glowIndex__ * 0x38) + 0xC), 2.0f);
                MemClass.writeMem<float>(_glowObj + ((glowIndex__ * 0x38) + 0x10), 2.0f);
                MemClass.writeMem<float>(_glowObj + ((glowIndex__ * 0x38) + 0x14), 1.3f);
            }
            else {
                MemClass.writeMem<float>(_glowObj + ((glowIndex__ * 0x38) + 0x8), 2.0f);
                MemClass.writeMem<float>(_glowObj + ((glowIndex__ * 0x38) + 0xC), 0.0f);
                MemClass.writeMem<float>(_glowObj + ((glowIndex__ * 0x38) + 0x10), 2.0f);
                MemClass.writeMem<float>(_glowObj + ((glowIndex__ * 0x38) + 0x14), 1.3f);
            }
            if (MemClass.readMem<bool>(entity + m_bIsDefusing) == true) {
                MemClass.writeMem<float>(_glowObj + ((glowIndex__ * 0x38) + 0x8), 2.0f);
                MemClass.writeMem<float>(_glowObj + ((glowIndex__ * 0x38) + 0xC), 2.0f);
                MemClass.writeMem<float>(_glowObj + ((glowIndex__ * 0x38) + 0x10), 2.0f);
                MemClass.writeMem<float>(_glowObj + ((glowIndex__ * 0x38) + 0x14), 1.3f);
            }
            MemClass.writeMem<bool>(_glowObj + ((glowIndex__ * 0x38) + 0x28), true);
            MemClass.writeMem<bool>(_glowObj + ((glowIndex__ * 0x38) + 0x29), false);
            MemClass.writeMem<bool>(_glowObj + ((glowIndex__ * 0x38) + 0x2d), true);

        }
    }
}

Vec3 old;
void rcs() {
    uintptr_t clientState = MemClass.readMem<int>(val.engine + dwClientState);
    Vec3 punchAngle = MemClass.readMem<Vec3>(val.localPlayer + m_aimPunchAngle);
    Vec3 Vangles = MemClass.readMem<Vec3>(clientState + dwClientState_ViewAngles);
    Vec3 rcs;
    rcs = (Vangles + old) - (punchAngle * 2);
    MemClass.writeMem<Vec3>(clientState + dwClientState_ViewAngles, rcs);
    old = punchAngle * 2;
}
void shoot()
{
    MemClass.writeMem<int>(val.gameModule + offset.forceLMB, 5);
    Sleep(20);
    MemClass.writeMem<int>(val.gameModule + offset.forceLMB, 4);
}

bool checkTBot()
{
    int crosshair = MemClass.readMem<int>(val.localPlayer + offset.crosshair);
    if (crosshair != 0 && crosshair < 64)
    {
        uintptr_t entity = MemClass.readMem<uintptr_t>(val.gameModule + offset.entityList + ((crosshair - 1) * 0x10));
        int eTeam = MemClass.readMem<int>(entity + offset.team);
        int eHealth = MemClass.readMem<int>(entity + offset.health);
        if (eTeam != val.myTeam && eHealth > 0)
            return true;
        else
            return false;
    }
    else
        return false;
}
HMODULE m0d;
FILE* ff;
void handleTBot()
{
    if (checkTBot()) {
        //Sleep(0.3);
        shoot();
    }
}
float GetDist(Vec3 Dt) {
    Vec3 myPos = val.origin;
    Vec3 D = Vec3{
        Dt.x - myPos.x,
        Dt.y - myPos.y,
        Dt.z - myPos.z,

    };
    return sqrt(D.x * D.x + D.y * D.y + D.z * D.z);
}
double PI = 3.14159265358;
void aimAt(Vec3 target) {
    uintptr_t clientState = MemClass.readMem<int>(val.engine + dwClientState);
    Vec3 punchAngle = MemClass.readMem<Vec3>(val.localPlayer + m_aimPunchAngle);
    Vec3 Vangles = MemClass.readMem<Vec3>(clientState + dwClientState_ViewAngles);
    Vec3 origin = MemClass.readMem<Vec3>(val.localPlayer + m_vecOrigin);
    Vec3 viewOffset = MemClass.readMem<Vec3>(val.localPlayer + m_vecViewOffset);
    Vec3 myPos = origin + viewOffset;
    Vec3 deltaVec = { target.x - myPos.x, target.y - myPos.y, target.z - myPos.z };
    float deltaVecLength = sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);
    Vec3 HackAngles{};

    float pitch = -asin(deltaVec.z / deltaVecLength) * (180 / PI);
    float yaw = atan2(deltaVec.y, deltaVec.x) * (180 / PI);
    HackAngles.x = pitch;
    HackAngles.y = yaw;
    HackAngles.z = 0;
    HackAngles.normal();
    MemClass.writeMem<Vec3>(clientState + dwClientState_ViewAngles, HackAngles);
}
void hack() {
    bool bt = false, keyHeld = false, bglow = false, bradar = false, bhop = false, brecoil = false, baimbot = false;
    int proc = MemClass.getProcess(L"csgo.exe");
    int distIndex = -1;
    int closestDist = 100000;
    val.gameModule = MemClass.getModule(proc, L"client.dll");
    val.engine = MemClass.getModule(proc, L"engine.dll");
    uintptr_t clientState_ = MemClass.readMem<int>(val.engine + dwClientState);
    val.localPlayer = MemClass.readMem<uintptr_t>(val.gameModule + offset.localPlayer);
    if (val.localPlayer == NULL) {
        while (val.localPlayer == NULL) {
            val.localPlayer = MemClass.readMem<uintptr_t>(val.gameModule + offset.localPlayer);
            val.viewOffset = MemClass.readMem<Vec3>(clientState_ + dwClientState_ViewAngles);
        }
    }
    while (true) {
        //std::cout << "Trigger=f1\nglow=f2 \nradar=f3\nbhop=f4\nRecoil=f5\naimbot=f6";
        val.myTeam = MemClass.readMem<int>(val.localPlayer + offset.team);
        if (GetAsyncKeyState(VK_F1) & 1)
        {
            keyHeld = true;
            bt = !bt;
        }
        if (GetAsyncKeyState(VK_F2) & 1) {
            bglow = !bglow;
        }
        if (GetAsyncKeyState(VK_F3) & 1) {
            bradar = !bradar;
        }
        if (GetAsyncKeyState(VK_F4) & 1) {
            bhop = !bhop;
        }
        if (GetAsyncKeyState(VK_F5) & 1) {
            brecoil = !brecoil;
        }
        if (GetAsyncKeyState(VK_F6) & 1) {
            baimbot = !baimbot;
        }
        if (bt) {
            handleTBot();
        }
        if (bglow) {
            //std::cout << "glow\n";
            glow();
        }
        if (bradar) {
            for (short int i = 1; i < 64; i++) {
                uintptr_t entity = MemClass.readMem<uintptr_t>(val.gameModule + dwEntityList + i * 0x10);
                if (entity != NULL) {
                    MemClass.writeMem<bool>(entity + m_bSpotted, true);
                }
            }
        }
        if (bhop) {
            int flag = MemClass.readMem<int>(val.localPlayer + offsets_::m_fFlags);
            if (GetAsyncKeyState(VK_SPACE) && flag & (1 << 0)) {
                MemClass.writeMem<int>(val.localPlayer + dwForceJump, 0);
            }
        }
        Vec3 old = { 0,0,0 };
        if (brecoil) {
            int shots = MemClass.readMem<int>(val.localPlayer + m_iShotsFired);
            //std::cout << aimpunch.x  << ","  << aimpunch.y << "\n";
            if (shots > 1) {
                rcs();
            }

        }
        if (baimbot) {
            val.origin = MemClass.readMem<Vec3>(val.localPlayer + m_vecOrigin);
            for (short int i = 1; i < 64; i++) {
                uintptr_t entity = MemClass.readMem<uintptr_t>(val.gameModule + dwEntityList + i * 0x10);
                if (entity != NULL) {
                    int team = MemClass.readMem<int>(entity + m_iTeamNum);
                    int health = MemClass.readMem<int>(entity + m_iHealth);
                    if (team != val.myTeam) {
                        float currentDist = GetDist(MemClass.readMem<Vec3>(entity + m_vecOrigin));
                        if (MemClass.readMem<std::int32_t>(entity + m_iTeamNum) == val.myTeam)
                            continue;

                        if (MemClass.readMem<bool>(entity + m_bDormant))
                            continue;

                        if (MemClass.readMem<std::int32_t>(entity + m_lifeState))
                            continue;

                        if (currentDist < closestDist) {
                            Vec3 Bone{};
                            uintptr_t BoneM = MemClass.readMem<uintptr_t>(entity + m_dwBoneMatrix);
                            //std::cout <<  "\n" << std::hex << (entity + m_dwBoneMatrix) + 0x30 * 8 << "\n";
                            if (health > 0) {
                                Bone.x = MemClass.readMem<float>(BoneM + 0x30 * 8 + 0x0C); //head
                                Bone.y = MemClass.readMem<float>(BoneM + 0x30 * 8 + 0x1C); // head
                                Bone.z = MemClass.readMem<float>(BoneM + 0x30 * 8 + 0x2C); // head
                                aimAt(Bone);
                            }
                        }
                    }
                }
            }
        }
    }
    }

//directx hook
HWND window;
int windowWidth = 0;
int windowHeight = 0;
bool snapLine = false, bEsp=false;


BOOL CALLBACK enumwind(HWND handle, LPARAM lp) {
    DWORD PID;
    GetWindowThreadProcessId(handle, &PID);
    if (GetCurrentProcessId() != PID) return TRUE;
    window = handle;
    return FALSE;
}
HWND getprocesswindow() {
    window = NULL;
    EnumWindows(enumwind, NULL);
    RECT size;
    GetWindowRect(window, &size);
    return window;
}
bool getDirectxDevice(void** pTable,size_t size) {
    if (!pTable) return false;
    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pD3D) return false;
    IDirect3DDevice9* dummyDvc = nullptr;
    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = false;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = getprocesswindow();
    HRESULT dummy = pD3D->CreateDevice(
        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &dummyDvc);
    if (dummy != S_OK) {
        d3dpp.Windowed = !d3dpp.Windowed;
        HRESULT dummy = pD3D->CreateDevice(
        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &dummyDvc);
        if (dummy != S_OK) {
            pD3D->Release();
            return false;
        }
    }
    memcpy(pTable, *(void***)(dummyDvc), size);
    dummyDvc->Release();
    pD3D->Release();
    return true;
}

LPDIRECT3DDEVICE9 pDev_;
typedef HRESULT(APIENTRY* tEnd)(LPDIRECT3DDEVICE9 pDev);
tEnd OriginalEnd = nullptr;
//-----------------------------------------------------------------------------
//DRAW functs
void drawFilledRect(int x, int y, int width, int height, D3DCOLOR color) {
    D3DRECT rect = { x,y,x + width,y + height };
    pDev_->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);

}
//----------------------------------------------
void drawtext(const char* txt, float x, float y, D3DCOLOR color) {
    RECT recta;
    if (!esp->font) {
        D3DXCreateFont(pDev_, 25, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Arial", &esp->font);
    }
    SetRect(&recta, x + 1, y + 1, x + 1, y + 1);
    esp->font->DrawTextA(0, txt, -1, &recta, DT_CENTER | DT_NOCLIP, BLACK(255));
    SetRect(&recta, x + 1, y + 1, x + 1, y + 1);
    esp->font->DrawTextA(0, txt, -1, &recta, DT_CENTER | DT_NOCLIP, color);
}
//-----------------------------------------------------------------------------
D3DCOLOR color = GREEN(0);
//menu options
Vec3 menuSize = {250,300};
bool Bmenu = true;
char menuName[] = "CheetahChug";
char options[] = "Trigger= f1\nglow=f2 \nradar=f3\nbhop=f4\nRecoil=f5\naimbot=f6\nsnapLine=f7\nEsp=f8\n";
bool bt = false, keyHeld = false, bglow = false, bradar = false, bhop = false, brecoil = false, baimbot = false;
//-----------------------------------------------------------------------------------------------------------

VOID APIENTRY hooked_directx(LPDIRECT3DDEVICE9 o_pDev) {
    if (!pDev_)  pDev_ = o_pDev;
    CDraw draw;
    draw.GetDevice(pDev_);
    if (Bmenu) {
        //"Trigger= f1\nglow=f2 \nradar=f3\nbhop=f4\nRecoil=f5\naimbot=f6\nsnapLine=f7\nEsp=f8\n"
        draw.BoxFilled(windowWidth - menuSize.x, 0, menuSize.x, menuSize.y, BLACK(80));
        drawtext(menuName, windowWidth - menuSize.x / 2, 0, CYAN(255));
        if(!bt) drawtext("Trigger=f1\n", windowWidth - menuSize.x / 2, 30, WHITE(255));
        else drawtext("Trigger=f1\n", windowWidth - menuSize.x / 2, 30, GREEN(255));

        if (!bglow) drawtext("glow=f2\n", windowWidth - menuSize.x / 2, 50, WHITE(255));
        else  drawtext("glow=f2\n", windowWidth - menuSize.x / 2, 50, GREEN(255));


        if (!bradar)  drawtext("radar=f3\n", windowWidth - menuSize.x / 2, 70, WHITE(255));
        else drawtext("radar=f3\n", windowWidth - menuSize.x / 2, 70, GREEN(255));

        if (!bhop) drawtext("bhop=f4\n", windowWidth - menuSize.x / 2, 90, WHITE(255));
        else drawtext("bhop=f4\n", windowWidth - menuSize.x / 2, 90, GREEN(255));
        if (!brecoil) drawtext("recoil=f5\n", windowWidth - menuSize.x / 2, 110, WHITE(255));
        else drawtext("recoil=f5\n", windowWidth - menuSize.x / 2, 110, GREEN(255));
        if (!baimbot) drawtext("aimbot=f6\n", windowWidth - menuSize.x / 2, 130, WHITE(255));
        else drawtext("aimbot=f6\n", windowWidth - menuSize.x / 2, 130, GREEN(255));
        if (!snapLine) drawtext("snapLine=f7\n", windowWidth - menuSize.x / 2, 150, WHITE(255));
        else drawtext("snapLine=f7\n", windowWidth - menuSize.x / 2, 150, GREEN(255));
        if (!bEsp) drawtext("esp=f8\n", windowWidth - menuSize.x / 2, 170, WHITE(255));
        else drawtext("esp=f8\n", windowWidth - menuSize.x / 2, 170, GREEN(255));

        drawtext("close menu=insert\n", windowWidth - menuSize.x / 2, 190, WHITE(255));
        drawtext("exit=f10\n", windowWidth - menuSize.x / 2, 210, WHITE(255));


    }




    //draw cross hair
    draw.Circle(windowWidth / 2, windowHeight / 2, 5, 1, full, false, 10, WHITE(255));
    draw.CircleFilled(windowWidth / 2, windowHeight / 2, 1.5, 1, full, 10, WHITE(255));
    //------------------------------
    // snap line
    if (snapLine) {
        ent* localPlayesr = (ent*)MemClass.readMem<uintptr_t>(val.gameModule + offset.localPlayer);
        for (int i = 0; i < 64; i++) {
            ent* entity = esp->entlist->entlist[i].ent;
            if (entity == localPlayesr) continue;
            if (!esp->isValid(entity)) {
                continue;
            }
            if (entity->team == localPlayesr->team) {
                color = YELLOW(255);
            }
            else {
                color = RED(255);
            }
            Vec3 ent_pos;
            if (esp->Wrld2Screen(entity->m_vecOrigin, ent_pos))
                draw.Line(ent_pos.x, ent_pos.y, windowWidth / 2 - 70, windowHeight, 1, true, color);

        }
    }
    //------------------------------
    if (bEsp) {
        ent* localPlayesr = (ent*)MemClass.readMem<uintptr_t>(val.gameModule + offset.localPlayer);
        for (int i = 0; i < 64; i++) {
            ent* entity = esp->entlist->entlist[i].ent;
            if (entity == localPlayesr) continue;
            if (!esp->isValid(entity)) {
                continue;
            }
            if (entity->team == localPlayesr->team) {
                color = YELLOW(255);
            }
            else {
                color = RED(255);
            }
            Vec3 headpos = esp->bonePos(entity, 8);
            Vec3 feetPos = esp->bonePos(entity, 72);
            Vec3 ent_pos,head2dpos, feet2d;
            if (esp->Wrld2Screen(entity->m_vecOrigin, ent_pos)) {
                if (esp->Wrld2Screen(headpos, head2dpos)) {
                    if (esp->Wrld2Screen(feetPos, feet2d)) {
                        float Height = abs(head2dpos.y - ent_pos.y);
                        float Width = Height / 2;
                        ent_pos.x -= Width / 2;

                        draw.Box(ent_pos.x, ent_pos.y-Height, Width, Height, 1.3f, color);
                    }
                    //606 =  ent_pos.y-128
                    //draw.Box(ent_pos.x-35, ent_pos.y-128 , 70, 128, 1.3f, color);

                }
            }
        }
    }
    //------------------------------

    OriginalEnd(pDev_);
}

//------------------------------------------------------------------





DWORD WINAPI mainThread(HMODULE mod){
    //TraceRay( const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace )
    m0d = mod;
    //AllocConsole();
    //FILE* f;
    //freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "Trigger= f1\nglow=f2 \nradar=f3\nbhop=f4\nRecoil=f5\naimbot=f6\nsnapLine=f7\nEsp=f8\n";
    int proc = MemClass.getProcess(L"csgo.exe");
    int distIndex = -1;
    int closestDist = 999;
    val.gameModule = MemClass.getModule(proc, L"client.dll");
    val.engine = MemClass.getModule(proc, L"engine.dll");
    void* EngineTraceServer004 = getInterface(L"engine.dll", "EngineTraceServer004");
    uintptr_t clientState_ = MemClass.readMem<int>(val.engine + dwClientState);
    val.localPlayer = MemClass.readMem<uintptr_t>(val.gameModule + offset.localPlayer);
//-------------------------------------------------------------------------------------------------------------------------
//directx hook
    void* DxDvc[119];
    BYTE EndSceneByte[7]{ 0 };
    if (getDirectxDevice(DxDvc, sizeof(DxDvc))) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 10);
        windowWidth = *(int*)((uintptr_t)GetModuleHandle(L"engine.dll") + 0x38EB18C);
        windowHeight = *(int*)((uintptr_t)GetModuleHandle(L"engine.dll") + 0x38EB190);
        std::cout << "[+] DXD9 Hooked!\n";
        SetConsoleTextAttribute(hConsole, 7);
        memcpy(EndSceneByte, (char*)DxDvc[42], 7);
        OriginalEnd = (tEnd)helper::trampHook32((BYTE*)(DxDvc[42]), (BYTE*)hooked_directx, 7);
    }
    esp = new Esp();

//-------------------------------------------------------------------------------------------------------------------------
    if (val.localPlayer == NULL) {
        while (val.localPlayer == NULL) {
            if (GetAsyncKeyState(VK_F10)) break;
            val.localPlayer = MemClass.readMem<uintptr_t>(val.gameModule + offset.localPlayer);
            val.viewOffset = MemClass.readMem<Vec3>(clientState_ + dwClientState_ViewAngles);
        }
    }
    bright();
    while (!GetAsyncKeyState(VK_F10)) {
        esp->update();
        val.myTeam = MemClass.readMem<int>(val.localPlayer + offset.team);
        if (GetAsyncKeyState(VK_F1) & 1)
        {
            keyHeld = true;
            bt = !bt;
        }
        if (GetAsyncKeyState(VK_F2) & 1) {
            bglow = !bglow;
        }
        if (GetAsyncKeyState(VK_F3) & 1) {
            bradar = !bradar;
        }
        if (GetAsyncKeyState(VK_F4) & 1) {
            bhop = !bhop;
        }
        if (GetAsyncKeyState(VK_F5) & 1) {
            brecoil = !brecoil;
        }
        if (GetAsyncKeyState(VK_F6) & 1) {
            baimbot = !baimbot;
        }
        if (GetAsyncKeyState(VK_F7) & 1) {
            snapLine = !snapLine;
        }
        if (GetAsyncKeyState(VK_F8) & 1) {
            bEsp = !bEsp;
        }
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            Bmenu = !Bmenu;
        }
        if (bt) {
            handleTBot();
        }
        if (bglow) {
            //std::cout << "glow\n";
            glow();
        }
        if (bradar) {
            for (short int i = 1; i < 64; i++) {
                uintptr_t entity = MemClass.readMem<uintptr_t>(val.gameModule + dwEntityList + i * 0x10);
                if (entity != NULL) {
                    MemClass.writeMem<bool>(entity + m_bSpotted, true);
                }
            }
        }
        if (bhop) {
            int flag = MemClass.readMem<int>(val.localPlayer + offsets_::m_fFlags);
            if (GetAsyncKeyState(VK_SPACE) && flag & (1 << 0)) {
                MemClass.writeMem<int>(val.localPlayer + dwForceJump, 0);
            }
        }
        Vec3 old = { 0,0,0 };
        if (brecoil) {
            int shots = MemClass.readMem<int>(val.localPlayer + m_iShotsFired);
            //std::cout << aimpunch.x  << ","  << aimpunch.y << "\n";
            if (shots > 1) {
                rcs();
            }

        }
        if (baimbot) {
            val.origin = MemClass.readMem<Vec3>(val.localPlayer + m_vecOrigin);
            for (short int i = 1; i < 64; i++) {
                uintptr_t entity = MemClass.readMem<uintptr_t>(val.gameModule + dwEntityList + i * 0x10);
                if (entity != NULL) {
                    int team = MemClass.readMem<int>(entity + m_iTeamNum);
                    int health = MemClass.readMem<int>(entity + m_iHealth);
                    if (team != val.myTeam) {
                        float currentDist = GetDist(MemClass.readMem<Vec3>(entity + m_vecOrigin));
                        if (MemClass.readMem<std::int32_t>(entity + m_iTeamNum) == val.myTeam)
                            continue;

                        if (MemClass.readMem<bool>(entity + m_bDormant))
                            continue;

                        if (MemClass.readMem<std::int32_t>(entity + m_lifeState))
                            continue;

                        if (currentDist < closestDist) {
                            Vec3 Bone{};
                            uintptr_t BoneM = MemClass.readMem<uintptr_t>(entity + m_dwBoneMatrix);
                            //std::cout <<  "\n" << std::hex << (entity + m_dwBoneMatrix) + 0x30 * 8 << "\n";
                            if (health > 0) {
                                Bone.x = MemClass.readMem<float>(BoneM + 0x30 * 7 + 0x0C); //head
                                Bone.y = MemClass.readMem<float>(BoneM + 0x30 * 7 + 0x1C); // head
                                Bone.z = MemClass.readMem<float>(BoneM + 0x30 * 7 + 0x2C); // head
                                aimAt(Bone);
                            }
                        }
                    }
                }
            }
        }
        Sleep(1);
    }
    //-------------------------------------------------------------------------------------------------------------------------
    Sleep(1000);
    bEsp = false;
    Bmenu = false;
    snapLine = false;
    helper::memPatchInt((BYTE*)DxDvc[42], EndSceneByte, 7);
    FreeLibraryAndExitThread(mod, 0);
    return 0;
}






BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)mainThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}