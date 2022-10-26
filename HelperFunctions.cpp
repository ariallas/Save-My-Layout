#include "HelperFunctions.h"
#include <stdexcept>
#include <string>

std::string win32ErrorToString(DWORD errorCode) {

}

void win32Check(BOOL success) {
	if (!success)
		throw std::runtime_error(win32ErrorToString(GetLastError()));
}