#pragma once
#include <Windows.h>

class winClass {
public:
	HINSTANCE hInst;
	LPSTR lpCmdLine;
	int nCmdShow;
	LPCWSTR pClassName;
	WNDCLASSEXW wc = { 0 };
	winClass();
	void init(HINSTANCE hInst, LPSTR lpCmdLine, int nCmdShow, LPCWSTR pClassName, WNDPROC fWndProc);
};

class hWin {
private:
	winClass* wic;
	HWND hWnd;
	int x, y;
	int w, h;
public:
	hWin();
	hWin(std::nullptr_t);
	HWND getWin();
	void setXY(int x, int y);
	void setWH(int w, int h);
	void init(winClass* wic, LPCWSTR name, DWORD dwStyle, hWin parent);
	size_t getW();
	size_t getH();
	void show(bool Maximized);
};