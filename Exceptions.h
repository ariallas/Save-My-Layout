#pragma once
#include <stdexcept>
#include <string>
#include <Windows.h>

class win32_runtime_error : public std::runtime_error {
public:
	win32_runtime_error(DWORD errorCode, const std::wstring& msg);
	std::wstring message;
	DWORD errorCode;
};