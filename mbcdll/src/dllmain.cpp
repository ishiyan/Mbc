#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#ifdef _DEBUG
#include "../include/crtDebug.h"
#endif

extern "C" BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD ulReasonForCall, LPVOID /*lpReserved*/)
{
    switch (ulReasonForCall)
    {
        case DLL_PROCESS_ATTACH:
            // Disable the DLL_THREAD_ATTACH / DLL_THREAD_DETACH notifications
            DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));
            #ifdef _DEBUG
            setWindowsCrtDebugFlags();
            #endif
    	    break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
        default:
            break;
    }
    return TRUE;
}
