#include "HelperFunctions.h"
#include <stdexcept>
#include <string>

using namespace std;

std::wstring win32ErrorToString(DWORD errorCode) {
	LPTSTR errorText = NULL;
	FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText,
		0,
		NULL);
	wstring errorTextSting = errorText;
	LocalFree(errorText);
	return L"WIN32 error " + to_wstring(errorCode) + L": " + errorTextSting;
}

void win32Check(BOOL success) {
	if (!success) {
		DWORD errorCode = GetLastError();
		throw win32_runtime_error(errorCode, win32ErrorToString(errorCode));
	}
}