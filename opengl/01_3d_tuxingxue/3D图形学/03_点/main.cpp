#include <windows.h>
#include <tchar.h>
#include "Raster.h"

LRESULT CALLBACK    windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return  DefWindowProc(hWnd, msg, wParam, lParam);
}

int     WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//  1 ע�ᴰ����
	::WNDCLASSEXA winClass;
	winClass.lpszClassName = "Raster";
	winClass.cbSize = sizeof(::WNDCLASSEX);
	winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	winClass.lpfnWndProc = windowProc;
	winClass.hInstance = hInstance;
	winClass.hIcon = 0;
	winClass.hIconSm = 0;
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)(BLACK_BRUSH);
	winClass.lpszMenuName = NULL;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	RegisterClassExA(&winClass);

	//  2 ��������
	HWND    hWnd = CreateWindowEx(
		NULL,
		_T("Raster"),
		_T("Raster"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0,
		0,
		256,
		256,
		0,
		0,
		hInstance,
		0
	);

	UpdateWindow(hWnd);
	ShowWindow(hWnd, SW_SHOW);

	RECT rt = { 0 };
	GetClientRect(hWnd, &rt);// �õ��ͻ���
	int width = rt.right - rt.left;
	int height = rt.bottom - rt.top;
	void *buffer = 0;
	HDC hDC = GetDC(hWnd);
	HDC hMem = ::CreateCompatibleDC(hDC);

	// һ��ͼƬ
	BITMAPINFO	bmpInfor;
	bmpInfor.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfor.bmiHeader.biWidth = 256;
	bmpInfor.bmiHeader.biHeight = 256;
	bmpInfor.bmiHeader.biPlanes = 1;
	bmpInfor.bmiHeader.biBitCount = 32;// ����λ
	bmpInfor.bmiHeader.biCompression = BI_RGB;
	bmpInfor.bmiHeader.biSizeImage = 0;
	bmpInfor.bmiHeader.biXPelsPerMeter = 0;
	bmpInfor.bmiHeader.biYPelsPerMeter = 0;
	bmpInfor.bmiHeader.biClrUsed = 0;
	bmpInfor.bmiHeader.biClrImportant = 0;

	// ����λͼ
	HBITMAP	hBmp = CreateDIBSection(hDC, &bmpInfor, DIB_RGB_COLORS, (void**)&buffer, 0, 0);
	SelectObject(hMem, hBmp);// dc�൱�ڻ��壬hbmp�൱��ֵ����ֵ���Ƹ�����
	CELL::Raster    raster;

	MSG     msg = { 0 };
	while (true)
	{
		if (msg.message == WM_DESTROY || msg.message == WM_CLOSE || msg.message == WM_QUIT)
		{
			break;
		}
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// ��ͼ
		raster.clear();
		for (int i = 0; i < 100; ++i)
		{
			raster.drawPoint(rand() % 256, rand() % 256, CELL::Rgba(255, 0, 0), 4);
		}

		memcpy(buffer, raster._buffer, sizeof(raster._buffer));
		BitBlt(hDC, 0, 0, width, height, hMem, 0, 0, SRCCOPY);
	}

	return  0;
}