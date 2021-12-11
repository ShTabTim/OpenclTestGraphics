#include "FundLibs/clHelper/clHL.h"
#include "FundLibs/WinHelper/winHelper.h"
#include "FundLibs/WinHelper/RendWin.h"

#define ScrWW 512
#define ScrHH 512

clContext cont;
clProgram rend;
clKernel fncRend;
clMemBuf<BYTE*> buf;

size_t global_work_size[1] = { ScrWW * ScrHH };

winClass ws;
hWin w1;
CPUrender cr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nCmdShow) {
	ws.init(hInst, pCmdLine, nCmdShow, L"CRDCL", WndProc);

	w1.setXY(CW_USEDEFAULT, CW_USEDEFAULT);
	w1.setWH(512, 512);
	w1.init(&ws, L"OpenCL graphics test", WS_OVERLAPPEDWINDOW, nullptr);
	w1.show(false);

	cr.init(w1.getW(), w1.getH(), GetDC(w1.getWin()));

	
	cont.init();

	rend.init(&cont);
	rend.create("render.cl");
	rend.build();

	fncRend.init(&cont, &rend);
	fncRend.create("render");

	buf.init(&cont);
	buf.setMem(cr.getBuff());
	buf.create(sizeof(BYTE) * ScrWW * ScrHH * 3);
	buf.write(0, sizeof(BYTE) * ScrWW * ScrHH * 3);

	fncRend.setArg(buf.getsId(), 0);



	MSG msg;
	int gResult;
	while (1)
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				gResult = GetMessage(&msg, nullptr, 0, 0);
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			fncRend.doFnc(global_work_size);
			buf.read(0, sizeof(BYTE) * ScrWW * ScrHH * 3);
			cr.rend();
		}
	cr.finit();
	if (gResult == -1)
		return -1;
	else
		return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE:
		if (hWnd == w1.getWin())
			PostQuitMessage(69);
		break;
	case WM_KEYDOWN:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}