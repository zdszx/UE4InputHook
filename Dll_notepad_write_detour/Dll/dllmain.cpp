// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "detours.h"
// Address of the real WriteFile API
BOOL(WINAPI* True_WriteFile)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED) = WriteFile;

// Our intercept function
BOOL WINAPI HookedWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
    const char* pBuf = "Hijacked by timb3r";
    return True_WriteFile(hFile, pBuf, 19, lpNumberOfBytesWritten, lpOverlapped);
}


//BOOL(WINAPI* True_TranslateMessage)(MSG) = TranslateMessage;
//BOOL(WINAPI* TruePeekMessage)(LPMSG,HWND,UINT,UINT,UINT) = PeekMessage;
//BOOL WINAPI HookedPeekMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
//{   return TruePeekMessage(lpMsg, hWnd, WM_KEYFIRST, wMsgFilterMax, wRemoveMsg);}


LRESULT(WINAPI* TrueDispatchMessageW)(_In_ CONST MSG* lpMsg) = DispatchMessage;
LRESULT WINAPI HookedDispatchMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{   
    if (lpMsg->message == WM_CHAR) {
        unsigned char scancode = ((unsigned char*)&lpMsg->lParam)[2];
        unsigned int virtualKey = MapVirtualKey(scancode, MAPVK_VSC_TO_VK);
        if (lpMsg->wParam == 0x41)lpMsg->wParam = 0x57;  //A->W
    }//如下所述需要改一套消息事件才能重建，接另一个输入源原封不动送

    if (lpMsg->message == WM_KEYFIRST || lpMsg->message == WM_KEYUP || lpMsg->message == WM_POWERBROADCAST) {
        lpMsg->message = WM_POWERBROADCAST; //MSG结构体只这样改够吗？有一套组合的消息事件
        lpMsg->wParam = PBT_APMSUSPEND;
    }
    
   //直接定一个msg就把所有都拦住了，要接另一个输入源
    TrueDispatchMessageW(lpMsg);
    return 0;
}


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID p)
{
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)TrueDispatchMessageW, HookedDispatchMessageW);

        LONG lError = DetourTransactionCommit();
        if (lError != NO_ERROR) {
            MessageBox(HWND_DESKTOP, L"Failed to detour", L"timb3r", MB_OK);
            return FALSE;
        }
    }
    break;

    case DLL_PROCESS_DETACH:
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)TrueDispatchMessageW, HookedDispatchMessageW);

        LONG lError = DetourTransactionCommit();
        if (lError != NO_ERROR) {
            MessageBox(HWND_DESKTOP, L"Failed to detour", L"timb3r", MB_OK);
            return FALSE;
        }
    }
    break;
    }
    return TRUE;
}


