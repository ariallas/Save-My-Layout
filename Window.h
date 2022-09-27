#pragma once
#include <Windows.h>
#include <string>

class Window
{
public:
	Window(HWND windowHwnd);
	~Window();

	void init();
	bool isApplicable();

	std::wstring title, moduleName;
private:
	WINDOWPLACEMENT windowPlacement;
	WINDOWINFO windowInfo;
	HWND hwnd;

	bool retrieveWindowTitle();
	bool retrieveWindowPlacement();
	bool retrieveWindowInfo();
	bool retrieveWindowModuleFileName();

	void showLastWinapiError();
	void showWinapiError(DWORD errorCode);
	void showError(std::wstring error);
};