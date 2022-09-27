#pragma once
#include "Window.h"
#include <vector>

class MainApp
{
public:
	MainApp();
	~MainApp();

	void run();
	bool saveWindows();
private:
	std::vector<Window> windows;
};

