#pragma once
#include <windows.h>

class Window {
public:
	Window(int width, int height, const char* name);
	~Window();
	void show();
	void update(void);
public:
	int width;
	int height;
	HWND window;
	HDC hdc;
	HDC screenHDC;
	LPVOID ptr;
};
