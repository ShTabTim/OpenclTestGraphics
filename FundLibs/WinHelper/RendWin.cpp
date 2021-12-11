#pragma once

#include <Windows.h>
#include "RendWin.h"

unsigned char* CPUrender::getBuff() { return buff; }
size_t CPUrender::getW() { return w; }
size_t CPUrender::getH() { return h; }

void CPUrender::init(size_t w, size_t h, HDC hdc) {
	this->hdc = hdc;
	this->w = w;
	this->h = h;
	buff = new BYTE[w * h * 3];

	memset(buff, 0, sizeof(BYTE) * w * h * 3);
	memset(&buffInfo, 0, sizeof(BITMAPINFOHEADER));

	buffInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	buffInfo.bmiHeader.biPlanes = 1;
	buffInfo.bmiHeader.biBitCount = 24;
	buffInfo.bmiHeader.biCompression = BI_RGB;
	buffInfo.bmiHeader.biWidth = w;
	buffInfo.bmiHeader.biHeight = h;
}
void CPUrender::finit() { if (buff != NULL) delete[] buff; }

void CPUrender::rend() { if (buff != NULL) StretchDIBits(hdc, 0, 0, w, h, 0, 0, w, h, buff, &buffInfo, DIB_RGB_COLORS, SRCCOPY); }

void CPUrender::DrawOnBuffer(size_t index, color col) {
	index *= 3;
	buff[index + 2] = col.r;
	buff[index + 1] = col.g;
	buff[index] = col.b;
}
void CPUrender::DrawPixel(int x, int y, color col) { if (x >= 0 && x < w && y >= 0 && y < h) DrawOnBuffer(y * w + x, col); }
void CPUrender::DrawLine(int x1, int y1, int x2, int y2, color col) {
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1; dy = y2 - y1;
	dx1 = abs(dx); dy1 = abs(dy);
	px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
	if (dy1 <= dx1) {
		if (dx >= 0) {
			x = x1;
			y = y1;
			xe = x2;
		}
		else {
			x = x2;
			y = y2;
			xe = x1;
		}

		DrawPixel(x, y, col);

		for (i = 0; x < xe; i++) {
			x = x + 1;
			if (px < 0)
				px = px + 2 * dy1;
			else {
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			DrawPixel(x, y, col);
		}
	}
	else {
		if (dy >= 0) {
			x = x1;
			y = y1;
			ye = y2;
		}
		else {
			x = x2;
			y = y2;
			ye = y1;
		}

		DrawPixel(x, y, col);

		for (i = 0; y < ye; i++) {
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else {
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}
			DrawPixel(x, y, col);
		}
	}
}
