#include "MainApp.h"
#include <iostream>
#include <fcntl.h>
#include <io.h>

using namespace std;

BOOL CALLBACK enumWindowsProc(__in HWND hwnd, __in LPARAM lParam) {
	vector<Window>& windows = *reinterpret_cast<vector<Window>*>(lParam);

	Window window(hwnd);
	window.init();
	if (window.isApplicable())
		windows.push_back(window);

	return TRUE;
}

MainApp::MainApp() {
}

MainApp::~MainApp() {
}

bool MainApp::saveWindows() {
	BOOL success = EnumWindows(enumWindowsProc, reinterpret_cast<LPARAM>(&windows));
	return true;
}

void MainApp::run() {
	// Set console output to wide char mode
	int res = _setmode(_fileno(stdout), _O_U16TEXT);

	saveWindows();
	for (Window& window : windows) {
		wcout << window.title << endl;
		wcout << window.moduleName << endl;
		wcout << L"..................................." << endl;
	}
	wcout << L"App done." << endl;
}