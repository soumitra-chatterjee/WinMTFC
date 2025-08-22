#include "file_copy.h"

DWORD WINAPI ThreadCopyFile(LPVOID lpParameter) {
    ThreadArgs* pArgs = (ThreadArgs*) lpParameter;
    
    LPVOID lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlenW((LPCWSTR) pArgs->sourcePath) + lstrlenW((LPCWSTR) pArgs->destinationPath) + 30) * sizeof(WCHAR)); 
    StringCchPrintfW((LPWSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(WCHAR),
        L"\n> ThreadCopyFile [%s --- %s]\n", 
        pArgs->sourcePath, pArgs->destinationPath); 

    if( CopyFileW(pArgs->sourcePath, pArgs->destinationPath, FALSE) == 0 ) {
        LastErrorMsgConsole(lpDisplayBuf); // failed
    } 
    else {
        CustomOKMsgConsole(lpDisplayBuf);
    }

    if(lpDisplayBuf) LocalFree(lpDisplayBuf);
    return 0;
}