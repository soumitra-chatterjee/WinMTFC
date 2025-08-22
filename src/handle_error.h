#pragma once

#include <windows.h>
#include <strsafe.h>

void LastErrorMsgGUI(const wchar_t* errorPoint);
void LastErrorMsgConsole(const wchar_t* errorPoint);
void CustomErrorMsgConsole(const wchar_t* errorMsg);
void CustomOKMsgConsole(const wchar_t* okMsg);
void CustomSuccessMsgConsole(const wchar_t* successMsg);