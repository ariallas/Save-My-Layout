#include "Window.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include <dwmapi.h>
#include <psapi.h>

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
}

bool Window::isApplicable() {
	return title.length() != 0;
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
	if (!GetWindowPlacement(hwnd, &windowPlacement)) {
		showLastWinapiError();
		return false;
	}
	return true;
}

bool Window::retrieveWindowInfo() {
	windowInfo.cbSize = sizeof(WINDOWINFO);
	if (!GetWindowInfo(hwnd, &windowInfo)) {
		showLastWinapiError();
		return false;
	}
	return true;
}

bool Window::retrieveWindowModuleFileName() {
	DWORD dwPID;
	GetWindowThreadProcessId(hwnd, &dwPID);
	HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
	if (!handle) {
		// Returns 5 (ERROR_ACCESS_DENIED) if process is ran as admin
		// showLastWinapiError();
		return false;
	}
	WCHAR buffer[MAX_PATH];
	//if (GetProcessImageFileName(handle, buffer, MAX_PATH))
	if (!GetModuleFileNameEx(handle, NULL, buffer, MAX_PATH)) {
		showLastWinapiError();
		return false;
	}
	moduleName = buffer;
	return true;
}

void Window::showLastWinapiError() {
	showWinapiError(GetLastError());
}

void Window::showWinapiError(DWORD errorCode) {
	wstring error = to_wstring(errorCode);
	error = L"Win API error code: " + error;
	showError(error);
}

void Window::showError(wstring error) {
	wcout << error << endl;
}