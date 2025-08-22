#pragma once

#include <windows.h>
#include "handle_error.h"

typedef struct {
    wchar_t sourcePath[MAX_PATH];
    wchar_t destinationPath[MAX_PATH];
} ThreadArgs, *pThreadArgs;

DWORD WINAPI ThreadCopyFile(LPVOID lpParameter);
