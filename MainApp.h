#pragma once
#include "Window.h"
#include "Exceptions.h"
#include <vector>

class MainApp
{
public:
	MainApp();
	~MainApp();

	void run();
	bool saveWindows();
	bool restoreWindows();
private:
	std::vector<Window> windows;
};

