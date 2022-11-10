#pragma once
#include <stdexcept>
#include <string>

class win32_runtime_error : public std::runtime_error {
public:
	win32_runtime_error(const std::wstring& msg);
	std::wstring message;
};