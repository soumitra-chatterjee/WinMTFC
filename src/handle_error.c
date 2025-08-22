#include "handle_error.h"

void LastErrorMsgGUI(const wchar_t* errorPoint)
{ 
    // Retrieve the system error message for the last-error code
    LPVOID lpMsgBuf = NULL;
    LPVOID lpDisplayBuf = NULL;
    DWORD dw = GetLastError(); 

    if (FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR) &lpMsgBuf,
        0, NULL) == 0) 
    {
        // wprintf(L"FormatMessage() has failed\n"); // CRT functions should only be used if _beginthreadex is used
        MessageBoxW(NULL, L"FormatMessageW failed", L"Error", MB_OK|MB_ICONERROR);

    } else {

        lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
            (lstrlenW((LPCWSTR) lpMsgBuf) + lstrlenW((LPCWSTR)errorPoint) + 40) * sizeof(WCHAR)); 
        StringCchPrintfW((LPWSTR)lpDisplayBuf, 
            LocalSize(lpDisplayBuf) / sizeof(WCHAR),
            L"%s failed with Error %d: %s", 
            errorPoint, dw, lpMsgBuf); 
        MessageBoxW(NULL, (LPCWSTR) lpDisplayBuf, L"Error", MB_OK|MB_ICONERROR); 
    }
    if(lpMsgBuf) LocalFree(lpMsgBuf);
    if(lpDisplayBuf) LocalFree(lpDisplayBuf);
}
    

void LastErrorMsgConsole(const wchar_t* errorPoint)
{ 
    // data needed to access the last error and format it before it goes to the console
    LPVOID lpMsgBuf = NULL;
    LPVOID lpDisplayBuf =NULL;
    DWORD dw = GetLastError(); 
    HANDLE hr = GetStdHandle(STD_ERROR_HANDLE);
    DWORD nWritten = 0;

    // errors need to be red
    CONSOLE_SCREEN_BUFFER_INFO cInfo;
    BOOL cflag = GetConsoleScreenBufferInfo(hr, &cInfo);
    WORD cattr;
    if(! cflag) {
        MessageBoxW(NULL, L"GetConsoleScreenBufferInfo", L"Console Error", MB_OK|MB_ICONERROR);
    } else {
        cattr = cInfo.wAttributes;
        SetConsoleTextAttribute(hr, FOREGROUND_RED | FOREGROUND_INTENSITY);
    }


    if (FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR) &lpMsgBuf,
        0, NULL) == 0) 
    {
        WriteConsoleW(hr, L"\n FormatMessageW failed \n", 30, &nWritten, NULL);
        if(nWritten != (DWORD)30) {
            MessageBoxW(NULL, L"STD_ERROR_HANDLE → CONSOLE mismatch in count of unicode characters. (Expected: 22 characters)", L"Warning", MB_OK|MB_ICONWARNING);
        }

    } else {
        lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
            (lstrlenW((LPCWSTR) lpMsgBuf) + lstrlenW((LPCWSTR)errorPoint) + 40) * sizeof(WCHAR)); 
        StringCchPrintfW((LPWSTR)lpDisplayBuf, 
            LocalSize(lpDisplayBuf) / sizeof(WCHAR),
            L"\n%s failed with Error %d: %s \n", 
            errorPoint, dw, lpMsgBuf); 
        WriteConsoleW(hr, lpDisplayBuf, lstrlenW((LPCWSTR)lpDisplayBuf), &nWritten, NULL);
        if(nWritten != (DWORD)lstrlenW((LPCWSTR)lpDisplayBuf)) {
            MessageBoxW(NULL, L"STD_ERROR_HANDLE → CONSOLE : mismatch in count of unicode characters", L"Warning", MB_OK|MB_ICONWARNING);
        }
    }

    // put console attributes to its original state
    if(cflag) {
        SetConsoleTextAttribute(hr, cattr);
    }

    if (lpMsgBuf) LocalFree(lpMsgBuf);
    if (lpDisplayBuf) LocalFree(lpDisplayBuf);
}


void CustomErrorMsgConsole(const wchar_t* errorMsg) 
{
    HANDLE hr = GetStdHandle(STD_ERROR_HANDLE);
    DWORD nWritten = 0;

    // errors need to be red
    CONSOLE_SCREEN_BUFFER_INFO cInfo;
    BOOL cflag = GetConsoleScreenBufferInfo(hr, &cInfo);
    WORD cattr;
    if(! cflag) {
        MessageBoxW(NULL, L"GetConsoleScreenBufferInfo", L"Console Error", MB_OK|MB_ICONERROR);
    } else {
        cattr = cInfo.wAttributes;
        SetConsoleTextAttribute(hr, FOREGROUND_RED | FOREGROUND_INTENSITY);
    }

    WriteConsoleW(hr, errorMsg, (DWORD)lstrlenW(errorMsg), &nWritten, NULL);
    if(nWritten != (DWORD)lstrlenW(errorMsg)) {
        MessageBoxW(NULL, L"STD_ERROR_HANDLE → CONSOLE mismatch in count of unicode characters.", L"Warning", MB_OK|MB_ICONWARNING);
    }
    // put console attributes to its original state
    if(cflag) {
        SetConsoleTextAttribute(hr, cattr);
    }
}

void CustomOKMsgConsole(const wchar_t* okMsg) {
    
    HANDLE hr = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD nWritten = 0;
    WriteConsoleW(hr, okMsg, (DWORD)lstrlenW(okMsg), &nWritten, NULL);

    if(nWritten != (DWORD)lstrlenW(okMsg)) {
        MessageBoxW(NULL, L"STD_ERROR_HANDLE → CONSOLE mismatch in count of unicode characters.", L"Warning", MB_OK|MB_ICONWARNING);
    }
}

void CustomSuccessMsgConsole(const wchar_t* successMsg) 
{
    HANDLE hr = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD nWritten = 0;

    // errors need to be red
    CONSOLE_SCREEN_BUFFER_INFO cInfo;
    BOOL cflag = GetConsoleScreenBufferInfo(hr, &cInfo);
    WORD cattr;
    if(! cflag) {
        MessageBoxW(NULL, L"GetConsoleScreenBufferInfo", L"Console Error", MB_OK|MB_ICONERROR);
    } else {
        cattr = cInfo.wAttributes;
        SetConsoleTextAttribute(hr, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }

    WriteConsoleW(hr, successMsg, (DWORD)lstrlenW(successMsg), &nWritten, NULL);
    if(nWritten != (DWORD)lstrlenW(successMsg)) {
        MessageBoxW(NULL, L"STD_ERROR_HANDLE → CONSOLE mismatch in count of unicode characters.", L"Warning", MB_OK|MB_ICONWARNING);
    }
    // put console attributes to its original state
    if(cflag) {
        SetConsoleTextAttribute(hr, cattr);
    }
}