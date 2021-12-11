#include "winHelper.h"

//winClass//
winClass::winClass() {}
void winClass::init(HINSTANCE hInst, LPSTR lpCmdLine, int nCmdShow, LPCWSTR pClassName, WNDPROC fWmdProc) {
	this->hInst = hInst;
	this->lpCmdLine = lpCmdLine;
	this->nCmdShow = nCmdShow;
	this->pClassName = pClassName;

	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = fWmdProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = nullptr;
	wc.hCursor = GetCursor();
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;

	RegisterClassEx(&wc);
}
//winClass//

//hWind//
hWin::hWin() {}
hWin::hWin(std::nullptr_t) { hWnd = nullptr; }
HWND hWin::getWin() { return hWnd; }
void hWin::setXY(int x, int y) { this->x = x; this->y = y; }
void hWin::setWH(int w, int h) { this->h = h; this->w = w; }
void hWin::init(winClass* wic, LPCWSTR name, DWORD dwStyle, hWin parent) {
	this->wic = wic;
	this->hWnd = CreateWindowEx(
		0,
		wic->pClassName,
		name,
		dwStyle,
		x, y, w, h,
		parent.getWin(), nullptr, wic->hInst, nullptr
	);
}
size_t hWin::getW() { return w; }
size_t hWin::getH() { return h; }
void hWin::show(bool Maximized) {
	RECT wRect, cRect;

	GetWindowRect(hWnd, &wRect);
	GetClientRect(hWnd, &cRect);

	wRect.right += w - cRect.right - wRect.left;
	wRect.bottom += h - cRect.bottom - wRect.top;;

	//wRect.left = dRect.right / 2 - wRect.right / 2;
	//wRect.top = dRect.bottom / 2 - wRect.bottom / 2;

	MoveWindow(hWnd, wRect.left, wRect.top, wRect.right, wRect.bottom, FALSE);

	ShowWindow(hWnd, Maximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
}
//hWind//