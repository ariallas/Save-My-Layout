#include "HelperFunctions.h"
#include <stdexcept>
#include <string>

std::wstring win32ErrorToString(DWORD errorCode) {
	return L"123";
}

void win32Check(BOOL success) {
	if (!success)
		throw win32_runtime_error(win32ErrorToString(GetLastError()));
}