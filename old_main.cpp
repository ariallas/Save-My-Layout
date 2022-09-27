#include <iostream>
#include <time.h>
#include <stdio.h>
#include <windows.h>
#include <WinUser.h>
#include <string>
#include <fcntl.h>
#include <io.h>
#include <dwmapi.h>
#include <psapi.h>
using namespace std;

#pragma comment (lib, "dwmapi.lib")

bool _GetWindowPlacement(HWND hwnd, WINDOWPLACEMENT& windowPlacement) {
	windowPlacement.length = sizeof(WINDOWPLACEMENT);
	if (!GetWindowPlacement(hwnd, &windowPlacement))
		wcout << GetLastError();
		return false;
	return true;
}

BOOL IsUsefulWindow(HWND hwnd, wstring title) {
	if (!IsWindowVisible(hwnd) || !title.length())
		return FALSE;
	LONG winStyle = GetWindowLong(hwnd, GWL_STYLE);
	LONG winExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	BOOL bIsCloaked = FALSE;
	DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &bIsCloaked, sizeof(BOOL));

	return (winStyle & WS_CAPTION) && !bIsCloaked && !(winExStyle & WS_EX_TOOLWINDOW);
	//if ((GetWindow(hwnd, GW_OWNER) == NULL || (winExStyle & WS_EX_APPWINDOW)) && !(winExStyle & WS_EX_TOOLWINDOW))
}

void GetExWindowInfo(HWND hwnd) {
	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-windowinfo
	WINDOWINFO windowInfo;
	windowInfo.cbSize = sizeof(WINDOWINFO);
	if (!GetWindowInfo(hwnd, &windowInfo))
		wcout << GetLastError();
	// wcout << (windowInfo.dwStyle & WS_CAPTION) << endl << (windowInfo.dwExStyle & WS_EX_TOOLWINDOW) << endl;

	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-windowplacement
	// The coordinates used in a WINDOWPLACEMENT structure should be used only by the GetWindowPlacement and SetWindowPlacement functions
	// Passing workspace coordinates to functions which expect screen coordinates (such as SetWindowPos) will result in the window appearing in the wrong location.
	WINDOWPLACEMENT windowPlacement;
	_GetWindowPlacement(hwnd, windowPlacement);

	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowrect
	RECT windowRect;
	if (!GetWindowRect(hwnd, &windowRect))
		wcout << GetLastError();
	
	// Get process name
	DWORD dwPID;
	GetWindowThreadProcessId(hwnd, &dwPID);
	HANDLE Handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
	if (Handle) {
		WCHAR Buffer[MAX_PATH];
		//if (GetProcessImageFileName(Handle, Buffer, MAX_PATH))
		if (GetModuleFileNameEx(Handle, NULL, Buffer, MAX_PATH))
			wcout << Buffer << endl;
	}
}

BOOL CALLBACK enumWindowsProc(__in HWND hwnd, __in LPARAM lParam) {
	int length = GetWindowTextLength(hwnd);
	if (length == 0)
		return TRUE;
	
	TCHAR* buffer;
	buffer = new TCHAR[length + 1];
	memset(buffer, 0, (length + 1) * sizeof(CHAR));
	GetWindowText(hwnd, buffer, length + 1);
	wstring windowTitle = wstring(buffer);
	delete[] buffer;
	
	if (IsUsefulWindow(hwnd, windowTitle)) {
		wcout << hwnd << L": " << windowTitle << endl;
		GetExWindowInfo(hwnd);
		if (windowTitle == L"MobaXterm") {
			WINDOWPLACEMENT windowPlacement;
			_GetWindowPlacement(hwnd, windowPlacement);
			Sleep(7000);
			SetWindowPlacement(hwnd, &windowPlacement);
		}
		wcout << L".........................................." << endl;
	}
	
	return TRUE;
}

int old_main() {
	int res = _setmode(_fileno(stdout), _O_U16TEXT);
	BOOL success = EnumWindows(enumWindowsProc, NULL);



	//HWND hwnd = FindWindowA(NULL, "Untitled - Notepad");
	//SetWindowPos(hwnd, 0, 100, 100, 0, 0, SWP_NOSIZE);
	cin.get();
	return 0;
}