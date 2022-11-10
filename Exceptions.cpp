#include "Exceptions.h"
using namespace std;

win32_runtime_error::win32_runtime_error(const std::wstring& msg) 
	:runtime_error("WIN32 Runtime Error"), message(msg) {}