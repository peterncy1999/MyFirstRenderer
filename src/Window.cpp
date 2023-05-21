//reference
//https://github.com/microsoft/Windows-classic-samples/blob/main/Samples/Win7Samples/begin/LearnWin32/HelloWorld/cpp/main.cpp
//https://learn.microsoft.com/en-us/windows/win32/learnwin32/creating-a-window
//https://github.com/skywind3000/mini3d

#include "Window.h"
#include <iostream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void screen_dispatch(void) {
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

Window::Window(int width, int height, const char* name):width(width), height(height)
{
	HBITMAP screen_hb = NULL;
	HBITMAP screen_ob = NULL;
	ptr = 0;
	const wchar_t CLASS_NAME[] = L"Test Window Class";
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = GetModuleHandle(NULL);
	RegisterClass(&wc);

	//char* 转LPWSTR
	int dwLen = strlen(name) + 1;
	int nwLen = MultiByteToWideChar(CP_ACP, 0, name, dwLen, NULL, 0);//算出合适的长度
	LPWSTR windowname = new WCHAR[dwLen];
	MultiByteToWideChar(CP_ACP, 0, name, dwLen, windowname, nwLen);
	
	window = CreateWindowEx(
		0, // Optional window styles.
		CLASS_NAME,// Window class
		windowname, // Window text
		WS_OVERLAPPEDWINDOW, // Window style
		0, 0, width, height,    // Size and position
		NULL,       // Parent window    
		NULL,       // Menu
		wc.hInstance,  // Instance handle
		NULL        // Additional application data
		);

	hdc = GetDC(window);
	screenHDC = CreateCompatibleDC(hdc);
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), width, -height, 1, 32, BI_RGB, width * height * 4, 0, 0, 0, 0 } };
	screen_hb = CreateDIBSection(screenHDC, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	screen_ob = (HBITMAP)SelectObject(screenHDC, screen_hb);


	//UpdateWindow(window);
}

Window::~Window()
{

}

void Window::show() {
	ShowWindow(window, SW_NORMAL);
}

void Window::update() {
	HDC hDC = GetDC(window);
	BitBlt(hDC, 0, 0, width, height, screenHDC, 0, 0, SRCCOPY);
	ReleaseDC(window, hDC);
}
