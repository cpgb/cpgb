#include "stdafx.h"

HMODULE G_HINST_DLL = NULL;

HHOOK G_HK = NULL;

HWND G_DST_HWND = NULL;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        G_HINST_DLL = hinstDLL;
        DisableThreadLibraryCalls(hinstDLL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

static BOOL Spy(HWND hWnd) {
    
}

/**
 * 窗口回调函数
 *
 * @param hWnd 窗口句柄
 * @return
 */
LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam)
{
    CWPSTRUCT *cs = (CWPSTRUCT *)lParam;

    if (code >= 0 && cs)
    {
        if (cs->hwnd == G_DST_HWND && cs->message == WM_NULL &&
            cs->wParam == 0x19820820 && cs->lParam == 0x19831014)
        {
            BOOL ok = Spy(G_DST_HWND);
            if (ok)
            {
                // strncpy_s(g_hwndinfo, NUM_WNDINFO, str.c_str(), sizeof(g_hwndinfo) - 1);
                // g_hwndinfo[sizeof(g_hwndinfo) - 1] = 0;
            }
            return ok;
        }
    }
    return CallNextHookEx(G_HK, code, wParam, lParam);
}

/**
 * 安装钩子
 *
 * @param hWnd 窗口句柄
 * @return
 */
BOOL InstallHook(HWND hWnd)
{
    DWORD pid, tid;
    HMODULE hinstDLL = G_HINST_DLL;

    if (G_HK == NULL && IsWindow(hWnd) &&
        MKTRUE(tid = GetWindowThreadProcessId(hWnd, &pid)) &&
        pid != GetCurrentProcessId() && hinstDLL &&
        (G_HK = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, hinstDLL, tid)) != NULL)
    {
        G_DST_HWND = hWnd;
        return TRUE;
    }

    return FALSE;
}

/**
 * 卸载钩子
 * @return
 */
VOID UninstallHook()
{
    G_DST_HWND = NULL;
    if (G_HK)
    {
        UnhookWindowsHookEx(G_HK);
        G_HK = NULL;
    }
}
