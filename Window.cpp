#include "Window.h"
#include "HelperFunctions.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include <dwmapi.h>
#include <psapi.h>
#include <fcntl.h>

#pragma comment (lib, "dwmapi.lib")
using namespace std;

Window::Window(HWND hwnd) : hwnd(hwnd) {
}

Window::~Window() {
}

void Window::init() {
	retrieveWindowTitle();
	if (title.length() == 0)
		return;

	retrieveWindowPlacement();
	retrieveWindowInfo();
	retrieveWindowModuleFileName();
	retrieveWindowAttributes();
}

bool Window::isApplicable() {
	bool negatives = isCloaked || (windowInfo.dwExStyle & WS_EX_TOOLWINDOW);
	bool positives = IsWindowVisible(hwnd) && title.length() && moduleName.length();
	return !negatives && positives;
}

bool Window::isDocked()
{
	if (!isRestored())
		return false;
	RECT r1 = windowInfo.rcWindow, r2 = windowPlacement.rcNormalPosition;
	return !( (r1.right - r1.left) == (r2.right - r2.left) &&
		      (r1.bottom - r1.top) == (r2.bottom - r2.top) );
}

bool Window::isRestored()
{
	UINT cmd = windowPlacement.showCmd;
	return cmd != SW_SHOWMAXIMIZED && cmd != SW_SHOWMINIMIZED && cmd != SW_MINIMIZE;
}

bool Window::restoreWindowPlacement() {
	if (!isDocked()) {
		win32Check(SetWindowPlacement(hwnd, &windowPlacement));
	}

	RECT r = windowInfo.rcWindow;
	int cx = r.right - r.left, cy = r.bottom - r.top;
	win32Check(SetWindowPos(hwnd, NULL, r.left, r.top, cx, cy, SWP_NOZORDER));

	return true;
}

bool Window::retrieveWindowTitle() {
	int titleLength = GetWindowTextLength(hwnd);
	WCHAR* buffer;
	buffer = new WCHAR[titleLength + 1];
	GetWindowText(hwnd, buffer, titleLength + 1);
	title = wstring(buffer);
	delete[] buffer;
	return true;
}

bool Window::retrieveWindowPlacement() {
	windowPlacement.length = sizeof(WINDOWPLACEMENT);
	win32Check(GetWindowPlacement(hwnd, &windowPlacement));
	return true;
}

bool Window::retrieveWindowInfo() {
	windowInfo.cbSize = sizeof(WINDOWINFO);
	win32Check(GetWindowInfo(hwnd, &windowInfo));
	return true;
}

bool Window::retrieveWindowModuleFileName() {
	DWORD dwPID;
	GetWindowThreadProcessId(hwnd, &dwPID);
	// Returns 5 (ERROR_ACCESS_DENIED) if process is ran as admin
	HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
	win32Check((BOOL)handle);
	WCHAR buffer[MAX_PATH];
	//if (GetProcessImageFileName(handle, buffer, MAX_PATH))
	win32Check(GetModuleFileNameEx(handle, NULL, buffer, MAX_PATH));
	moduleName = buffer;
	return true;
}

bool Window::retrieveWindowAttributes() {
	BOOL bIsCloaked = FALSE;
	// Returns COM error code if fails
	DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &bIsCloaked, sizeof(BOOL));
	isCloaked = bIsCloaked;
	return true;
}

//void Window::showLastWinapiError() {
//	showWinapiError(GetLastError());
//}
//
//void Window::showWinapiError(DWORD errorCode) {
//	wstring error = to_wstring(errorCode);
//	error = L"Win API error code: " + error;
//	showError(error);
//}
//
//void Window::showError(wstring error) {
//	wcout << error << endl;
//}