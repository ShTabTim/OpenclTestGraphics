#pragma once

#include <Windows.h>
typedef struct color {
	BYTE r, g, b;
} color;
class CPUrender {
private:
	BYTE* buff;
	BITMAPINFO buffInfo;
	size_t w, h;
	HDC hdc;
public:
	unsigned char* getBuff();
	size_t getW();
	size_t getH();

	void init(size_t w, size_t h, HDC hdc);
	void finit();

	void rend();

	void DrawOnBuffer(size_t index, color col);
	void DrawPixel(int x, int y, color col);
	void DrawLine(int x1, int y1, int x2, int y2, color col);
};
