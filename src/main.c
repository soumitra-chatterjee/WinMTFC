#include <stdio.h>
#include <windows.h>
#include "handle_error.h"
#include "file_copy.h"

#define MAX_THREADS 3

int wmain(int argc, LPWSTR argv[]) {
    // SYSTEM_INFO sysinfo;
    // GetSystemInfo(&sysinfo);
    // DWORD processorCount = sysinfo.dwNumberOfProcessors;
    // wprintf(L"Processor Count: %lu\n", processorCount);
    
    // LARGE_INTEGER start, end, freq;
    // QueryPerformanceFrequency(&freq);
    // QueryPerformanceCounter(&start);

    if (argc < 3) {
        CustomErrorMsgConsole(L"\n(Insufficient Arguments) Expected: <source-dir> <destination-dir>\n");
    }
    wchar_t* sourceFolderPath = argv[1];
    wchar_t* destFolderPath = argv[2];

    WCHAR tempSourceFolderPath[MAX_PATH];
    StringCchPrintfW(tempSourceFolderPath, MAX_PATH, L"%s", sourceFolderPath);

    size_t sourceLength, destLength;
    BOOL pathlenflag = TRUE;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written; 

    StringCchLengthW(sourceFolderPath, MAX_PATH, &sourceLength);
    StringCchLengthW(destFolderPath, MAX_PATH, &destLength);

    if(sourceLength <= (MAX_PATH - 3) && destLength <= (MAX_PATH-3)) {
        CustomOKMsgConsole(L"\nPath Length ...OK\n");
    } 
    if(sourceLength > (MAX_PATH - 3)) {
        CustomErrorMsgConsole(L"\n(Path Length) source folder path is too long, ... should be around 250 characters\n");
        pathlenflag = FALSE;
    }
    if(destLength > (MAX_PATH - 3)) {
        CustomErrorMsgConsole(L"\n(Path Length) destination folder path is too long, ... should be around 250 characters\n");
        pathlenflag = FALSE;
    }
    if(!pathlenflag) return (-1);

    // prepare to search in the source folder
    WIN32_FIND_DATAW ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    StringCchCatW(tempSourceFolderPath, MAX_PATH, L"\\*");
    hFind = FindFirstFileW(tempSourceFolderPath, &ffd);
    
    if(hFind == INVALID_HANDLE_VALUE) {
        LastErrorMsgConsole(L"> wmain > FindFirstFileW ");
        return (-1);
    }

    // Ensure destination folder exists
    // CreateDirectoryW(destFolderPath, NULL);

    // prepare for thread creation
    pThreadArgs pDataArray[MAX_THREADS];
    DWORD dwThreadIdArray[MAX_THREADS];
    HANDLE hThreadArray[MAX_THREADS];

    // pDataArray needs to be properly initialized i.e, set to NULL otherwise it might be misunderstood by the process later that some garbadge value is pointer to data object, it can make problem with cleanup code during heap freeing. 
    for(int i = 0; i < MAX_THREADS; i++) {
        pDataArray[i] = NULL;
    }

    int threadCount = 0;
    
    do {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue; // if it is a folder, do nothing(for now)
        }
            
        pDataArray[threadCount] = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ThreadArgs));

        if(pDataArray[threadCount] == NULL) {
            return (-2); // system out of memory
        }
        StringCchPrintfW(pDataArray[threadCount]->sourcePath, MAX_PATH, (LPCWSTR)L"%s\\%s", sourceFolderPath, ffd.cFileName);  
        StringCchPrintfW(pDataArray[threadCount]->destinationPath, MAX_PATH, (LPCWSTR)L"%s\\%s", destFolderPath, ffd.cFileName); 

        hThreadArray[threadCount] = CreateThread(
            NULL,
            0,
            ThreadCopyFile,
            pDataArray[threadCount],
            0,
            &dwThreadIdArray[threadCount]
        );
        
        if(hThreadArray[threadCount] == NULL) {
            // Thread creation failed for some reason
            LastErrorMsgConsole(L"> wmain > CreateThread ");
            ExitProcess(2); // Termination cleans up threads and memory and for DLL it makes sense than using 'return -1'
        }

        threadCount++;
        if(threadCount == MAX_THREADS) {
            WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);
            threadCount = 0;
            for(int i=0; i<MAX_THREADS; i++)
            {
                CloseHandle(hThreadArray[i]);
                if(pDataArray[i] != NULL)
                {
                    HeapFree(GetProcessHeap(), 0, pDataArray[i]);
                    pDataArray[i] = NULL;    // Ensure address is not reused.
                }
            }
        }
    }
    while (FindNextFileW(hFind, &ffd) != 0);
 
    DWORD dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES) 
    {
        CustomErrorMsgConsole(L"\ndo{...} while(FindNextFile...) has terminatied but last error is not ERROR_NO_MORE_FILES\n");
    }

    if(threadCount) {
        WaitForMultipleObjects(threadCount, hThreadArray, TRUE, INFINITE);
    }

    // CLEAN UP
    for(int i=0; i<MAX_THREADS; i++)
    {
        CloseHandle(hThreadArray[i]);
        if(pDataArray[i] != NULL)
        {
            HeapFree(GetProcessHeap(), 0, pDataArray[i]);
            pDataArray[i] = NULL;    // Ensure address is not reused.
        }
    }



    FindClose(hFind);
    CustomSuccessMsgConsole(L"\nAll Files have been copied\n");

    // QueryPerformanceCounter(&end);
    // double elapsedTime = (double) (end.QuadPart - start.QuadPart) / freq.QuadPart;
    // printf("\n time taken %.6f s", elapsedTime);

    return 0;
}