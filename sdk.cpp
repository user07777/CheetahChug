#include <iostream>
#include "sdk.h"
#define IFACE_OK = 0
void* getInterface(const wchar_t* dll, const char* interface_) {
	//uintptr_t myInterfaceAddr = (uintptr_t)GetModuleHandle(L"engine.dll") + 0x2fc230;
	//createInterface  CreatemyInterface = (createInterface)myInterfaceAddr;
	//int engineTraceReturnCode = FF_DONTCARE + 2;
	//CreatemyInterface("EngineTraceServer004", &engineTraceReturnCode);
	//std::cout << engineTraceReturnCode;

	createInterface itfc = (createInterface)GetProcAddress(GetModuleHandle(dll), "CreateInterface");
	int retCode = FF_DONTCARE + 2; // IFACE_OK = SUCESS
	void* interFace = itfc(interface_, &retCode);
	//std::cout << retCode;
	return interFace;
}
