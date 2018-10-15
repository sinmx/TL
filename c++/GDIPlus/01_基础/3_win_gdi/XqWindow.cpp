#include "XqWindow.h"
#include <gdiplus.h>
#include <math.h>

#include <string>
#include <iostream>
#include <sstream>

using namespace Gdiplus;
using namespace std;

#pragma comment(lib, "GdiPlus.lib")

int width, height;
ULONG_PTR token;

// wchar_t to string
void Wchar_tToString(std::string& szDst, wchar_t *wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte������
	char *psText;  // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte���ٴ�����
	szDst = psText;// std::string��ֵ
	delete[]psText;// psText�����
}

// string to wstring
void StringToWstring(std::wstring& szDst, std::string str)
{
	std::string temp = str;
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL, 0);
	wchar_t * wszUtf8 = new wchar_t[len + 1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
	szDst = wszUtf8;
	std::wstring r = wszUtf8;
	delete[] wszUtf8;
}


CXqWindow::CXqWindow(HINSTANCE hInst)
	:m_HWnd(NULL),
	m_HInstance(hInst),
	m_IPosX(0),
	m_IPosY(0),
	m_IWidth(500),
	m_IHight(500)
{
	memset(&m_WClassEx, 0, sizeof(m_WClassEx));

	GdiplusStartupInput gdipinput;
	GdiplusStartup(&token, &gdipinput, NULL);
}

CXqWindow::~CXqWindow()
{
	if (this->m_HWnd != NULL && ::IsWindow(this->m_HWnd)) // C++��������֮ǰ�����ٴ��ڶ���
	{
		::DestroyWindow(this->m_HWnd);
	}
	GdiplusShutdown(token);
}

BOOL CXqWindow::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

HWND CXqWindow::GetHandle()
{
	return this->m_HWnd;
}
void Printf(HDC hdc)
{
	WCHAR buf[128];
	int len;
	len = wsprintfW(buf, L"width:%d height:%d\n", width, height);

	SolidBrush b(Color(255, 100, 200, 150));
	HBITMAP bg = CreateCompatibleBitmap(hdc, width, height);
	HDC memdc = CreateCompatibleDC(hdc);
	SelectObject(memdc, bg);

	FontFamily fm(L"Consolas");
	Font f(&fm, 12, 2, UnitPoint);

	Graphics mg(memdc);
	mg.SetSmoothingMode(SmoothingModeHighQuality);
	mg.Clear(Color(255, 128, 128, 128));

	mg.DrawString(buf, len, &f, PointF(0.0, 0.0), &b);

	if (!BitBlt(hdc, 0, 0, width, height, memdc, 0, 0, SRCCOPY))
		MessageBox(NULL, L"BitBlt failed!", L"warning", MB_ICONWARNING);

	DeleteObject(bg);
	DeleteDC(memdc);
}

void PrintString(HDC &hdc,wstring str)
{
	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 255));
	SolidBrush brush(Color(255, 0, 0, 255));
	FontFamily fontFamily(L"����");
	Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	graphics.DrawString(str.c_str(), -1, &font, PointF(5.0, 500.0), &brush);

}

void CXqWindow::OnDraw(HDC hdc)
{
#if 0 ///<-- 2-3 �������
	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 255));
	SolidBrush brush(Color(255, 0, 0, 255));
	FontFamily fontFamily(L"����");
	Font font(&fontFamily,24,FontStyleRegular,UnitPixel);
	graphics.DrawString(L"GDI+ ʵ��1", -1, &font, PointF(20.0, 20.0),&brush);
#endif

	
#if 0 ///<-- 2-5 �ڴ�ӡ���豸�����
	DOCINFO docinfo;
	ZeroMemory(&docinfo, sizeof(DOCINFO));// ����ĵ���Ϣ
	docinfo.cbSize = sizeof(DOCINFO);
	docinfo.lpszDocName = L"GDIPlusPrint";// �ĵ�����

	PRINTDLG printDlg;// ������ӡ�Ի�
	ZeroMemory(&printDlg, sizeof(PRINTDLG));
	printDlg.lStructSize = sizeof(PRINTDLG);
	printDlg.Flags = PD_RETURNDC;// ����dc

	if (!PrintDlg(&printDlg))
	{
		MessageBox(NULL, L"������ӡ�Ի���ʧ��", L"warning", MB_ICONWARNING);
		return;
	}
	else
	{
		// ��ʼ��¼�ĵ�
		StartDoc(printDlg.hDC, &docinfo);
		StartPage(printDlg.hDC);

		Graphics graphics(printDlg.hDC);// ʹ�ô�ӡ���豸�������������ͼƽ����

		// ����������ڴ�ӡ���豸����н���
		
		Image image(L"./gidplus_data/test.bmp");// ����ͼƬ
		graphics.DrawImage(&image, 0.0f, 0.0f);

		Pen pen(Color(255, 0, 0, 0));
		graphics.DrawRectangle(&pen, 200, 500, 200, 150);
		graphics.DrawEllipse(&pen, 200, 500, 400, 650);
		graphics.DrawLine(&pen, 200, 500, 400, 650);

		// ��ʼ��ӡ
		EndPage(printDlg.hDC);
		EndDoc(printDlg.hDC);
	}

	// �ͷŴ�ӡ��Դ
	if (printDlg.hDevMode)
	{
		GlobalFree(printDlg.hDevMode);
	}

	if (printDlg.hDevNames)
	{
		GlobalFree(printDlg.hDevNames);
	}
	if (printDlg.hDC)
	{
		GlobalFree(printDlg.hDC);
	}
#endif
	
#if 0 ///<-- 2-6 ����ֱ��
	Graphics graphics(hdc);
	Pen blackPen(Color(255, 0, 0, 0), 3);

	// ���λ��
	PointF point1(10.0f,10.0f);
	PointF point2(10.0f, 100.0f);
	PointF point3(50.0f, 50.0f);
	PointF point4(10.0f, 10.0f);
	PointF points[4] = { point1, point2, point3, point4 };
	PointF * pPoints = points;

	// ���ƶ�����
	graphics.DrawLines(&blackPen, pPoints, 4);
	Pen redPen(Color(255, 255, 0, 0), 3);

	graphics.DrawLine(&redPen, PointF(20.0f, 100.0f), PointF(100.0f, 20.0f));
	graphics.DrawLine(&redPen, 50, 20, 100, 100);
#endif
	
#if 0 ///<-- 2-7 ���ƾ���
	Graphics graphics(hdc);
	Pen blackPen(Color(255, 0, 0, 0),3);

	// �������
	RectF rect1(10.0f,10.0f,100.0f,50.0f);
	RectF rect2(40.0f, 40.0f, 100.0f, 50.0f);
	RectF rect3(80.0f, 4.0f, 50.0f, 100.0f);
	RectF rects[] = { rect1, rect2, rect3 };
	RectF * pRects = rects;
	//graphics.DrawRectangles(&blackPen,rects,3);
	graphics.DrawRectangles(&blackPen,pRects,3);
	Pen redPen(Color(255, 255, 0, 0), 3);
	RectF rect4(20.0f, 20.0f, 50.0f, 100.0f);
	graphics.DrawRectangle(&redPen, rect4);
#endif

#if 0 ///<-- 2-8 ���Ƽ����ߺͱ���������
	#if 0  // DrawCurve ��������
	Graphics graphics(hdc);
	Pen greenPen(Color::Green, 3);
	Pen redPen(Color::Red, 3);

	// �������߾����ĵ�
	Point point1(100, 100);
	Point point2(200,50);
	Point point3(700, 10);
	Point point4(500, 100);

	Point curvePoints[7] = { point1, point2, point3, point4 };
	// ��������
	graphics.DrawCurve(&greenPen, curvePoints, 7);
	
	// ʹ�ú�ɫ���ʻ�������ǿ��Ϊ1.3������
	graphics.DrawCurve(&redPen, curvePoints, 7, 1.3f);

	// �������ߵĶ����(��ɫ)
	SolidBrush redBrush(Color::Red);
	graphics.FillEllipse(&redBrush, Rect(95, 95, 10, 10));
	graphics.FillEllipse(&redBrush, Rect(195, 75, 10, 10));
	graphics.FillEllipse(&redBrush, Rect(395, 5, 10, 10));
	graphics.FillEllipse(&redBrush, Rect(795, 95, 10, 10));
	#endif

	#if 0 // ʹ��DrawColosedCurve��������
	Graphics graphics(hdc);
	Pen greenPen(Color::Green, 3);
	PointF point1(100.f, 100.f);
	PointF point2(200.f, 50.f);
	PointF point3(400.f, 10.f);
	PointF point4(500.f, 100.f);
	PointF point5(600.f, 200.f);
	PointF point6(700.f, 400.f);
	PointF point7(500.f, 500.f);
	PointF curvePoints[7] = { point1, point2, point3, point4, point5, point6, point7 };
	graphics.DrawClosedCurve(&greenPen, curvePoints, 7);
	// �������ߵĶ����(���)
	SolidBrush redBrush(Color::Red);
	graphics.FillEllipse(&redBrush, Rect(95, 95, 10, 10));
	graphics.FillEllipse(&redBrush, Rect(795, 95, 10, 10));
	graphics.FillEllipse(&redBrush, Rect(795, 495, 10, 10));
	graphics.FillEllipse(&redBrush, Rect(195, 45, 10, 10));
	graphics.FillEllipse(&redBrush, Rect(395, 5, 10, 10));
	graphics.FillEllipse(&redBrush, Rect(595, 195, 10, 10));
	graphics.FillEllipse(&redBrush, Rect(695, 395, 10, 10));

	#endif
#endif

#if 0 ///<-- 2-9 ���Ʊ���������
	// ����������ͨ��4����ָ�����ߣ������˵���������Ƶ㣬�������������˵㣬��ͨ�����Ƶ㣬���Ƶ��λ�ÿ������ߵ���������
	Graphics graphics(hdc);
	Pen greenPen(Color::Green);
	Pen redPen(Color::Red);

	// �����������
	Point startPoint(100, 100);
	
	// �����������Ƶ�
	Point contronlPoint1(200, 10);
	Point contronlPoint2(350, 50);

	// ���������յ�
	Point endPoint(500, 100);

	// ���������ĸ���λ�ã�����ɫ�����Ƶ���ɫ
	graphics.FillEllipse(&SolidBrush(Color::Red),100,100,10,10);
	graphics.FillEllipse(&SolidBrush(Color::Red), 500, 100, 10, 10);
	graphics.FillEllipse(&SolidBrush(Color::Green), 200, 10, 10, 10);
	graphics.FillEllipse(&SolidBrush(Color::Green), 350, 50, 10, 10);
	graphics.DrawBezier(&greenPen, startPoint, contronlPoint1, contronlPoint2, endPoint);// ���Ʊ���������
#endif
#if 0 ///<-- 2-10 ���ƶ����
	Graphics graphics(hdc);
	Pen blackPen(Color(255, 0, 0, 0), 3);

	// �������ζ˵�
	Point point1(100, 100);
	Point point2(200, 130);
	Point point3(150, 200);
	Point point4(45, 200);
	Point point5(0, 130);
	Point points[5] = { point1, point2, point3, point4, point5 };
	
	// ���ƶ����
	graphics.DrawPolygon(&blackPen, points, 5);
#endif

#if 0 ///<-- 2-11 ���ƻ���
	Graphics graphics(hdc);
	Pen redPen(Color::Red, 3);

	Rect ellipseRect(10, 10, 200, 100);
	REAL startAngle = 0.f;
	REAL sweepAngle = 90.f;
	// ���ƾ���
	graphics.DrawRectangle(&Pen(Color::Black, 3), ellipseRect);
	// ���ƻ���
	graphics.DrawArc(&redPen, ellipseRect, startAngle, sweepAngle);
#endif

#if 0 ///<-- 2-12 ���λ���
	Graphics graphics(hdc);
	Pen blackPen(Color(255, 0, 0, 0), 3);

	// ��������: ���� + �Ƕ�,�Ƕȣ�˳ʱ��
	Rect ellipseRect(0, 0, 200, 100);
	REAL startAngle = 0.f;
	REAL sweepAngle = 45.f;
	
	// ������Բ����(���̰뾶�����)
	graphics.DrawPie(&blackPen, ellipseRect, startAngle, sweepAngle);
#endif

#if 0 ///<-- 2-13 �������
	// �ȹ��컭ˢ��������������
	Graphics graphics(hdc);
	SolidBrush bluePen(Color::Blue);// Color(255,0,0,255)
	// ���߶����
	PointF point1(100.f, 100.f);
	PointF point2(200.f, 50.f);
	PointF point3(250.f, 200.f);
	PointF point4(50.f, 150.f);
	PointF points[4] = { point1, point2, point3, point4 };
	
	// �������
	graphics.FillClosedCurve(&bluePen, points, 4,FillModeAlternate,1.);
	
	// �������߶����
	for (int i = 0; i < 4;i++)
	{
		graphics.FillEllipse(&SolidBrush(Color::Red), RectF(points[i].X - 5, points[i].Y - 5, 10, 10));
	}

	// ������
	Rect ellipseRect(10, 240, 200, 100);
	graphics.FillRectangle(&bluePen, ellipseRect);
#endif

#if 0 ///<-- 2-14 �������������Ƭ ---- 2-15 ��,��Դ������ͼƬ����ѧϰ����

#endif
#if 0 ///<-- 2-16����ı�
	Graphics graphics(hdc);
	Font myFont(L"Arial", 26);
	RectF layoutRect(10.f, 10.f, 200.f, 50.f);

	// ����ˮƽ���кʹ�ֱ����
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	SolidBrush blackBrush(Color::Black);

	//string str("Hello GDI+");
	//wstring szDst;
	//StringToWstring(szDst,str);
	wstring szDst(L"��� GDI+");

	// ������
	graphics.DrawString(szDst.c_str(), szDst.size(), &myFont, layoutRect, &format, &blackBrush);
	// ���ƾ���
	graphics.DrawRectangle(&Pen(Color::Green, 3), layoutRect);
#endif

#if 0 ///<-- 3-1 ��ˢ�й���������
	// ��ˢû�п�ȣ������п��
	Graphics graphics(hdc);

	// ��������ͼƬ
	Image image(L"./gidplus_data/Texture.bmp");
	
	// ��������ˢ
	TextureBrush tBrush(&image);

	// ����ˢ���뻭�ʵĹ��캯��
	Pen texturePen(&tBrush, 30);
	graphics.DrawEllipse(&texturePen, 100, 20, 200, 100);
	cout << texturePen.GetWidth() << endl;

	wostringstream oss;
	oss << L"���ʿ�� : "<< texturePen.GetWidth();
	wstring str = oss.str();
	PrintString(hdc, str);// �������Ļ
#endif

#if 0 ///<-- 3-2 ���ʵ����ͷ��
	Graphics graphics(hdc);
	Pen blackPen(Color::Black, 5);
	
	// ���õ����ͻ���ֱ��
	/*
		DashStyleSolid,          // 0 ʵ��
		DashStyleDash,           // 1 ����
		DashStyleDot,            // 2 ����
		DashStyleDashDot,        // 3 �㻮��
		DashStyleDashDotDot,     // 4 ˫�㻮��
		DashStyleCustom          // 5 �������Զ��廭������
		*/
	int i = 0;
	for (i = 0; i < 6; i++)
	{
		blackPen.SetDashStyle((DashStyle)i);
		graphics.DrawLine(&blackPen, 10,30 * i + 20, 300, 30 * i + 20);
	}

	// ʹ���Զ�������
	REAL dashVals[4] =
	{
		5.f,// �߳�5����
		2.f,// ���2����
		15.f,// �߳�15����
		4.f// ���4����
	};

	blackPen.SetDashPattern(dashVals,4);
	blackPen.SetColor(Color::Red);
	graphics.DrawLine(&blackPen, 10, 30 * i + 40, 300, 30 * i + 40);

	i++;
	REAL dashVals2[6] =
	{
		5.f,// �߳�5����
		2.f,// ���2����
		10.f,// �߳�5����
		4.f,// ���2����
		8.f,// �߳�5����
		2.f// ���2����
	};

	blackPen.SetDashPattern(dashVals2, 6);// ��ȡ���ͷ��
	blackPen.SetColor(Color::Red);
	graphics.DrawLine(&blackPen, 10, 30 * i + 40, 600, 30 * i + 40);

	wostringstream oss;
	oss << L"���Ͷ�������� : " << blackPen.GetDashPatternCount();
	wstring str = oss.str();
	PrintString(hdc, str);// �������Ļ
#endif

#if 0 ///<-- 3-3 ���ʵ����ֶ��뷽ʽ
	// PenAlignment ���к�Ƕ��
	Graphics graphics(hdc);
	Pen redPen(Color::Red, 1);
	Pen blackPen(Color::Black, 8);
	Pen greenPen(Color::Green, 16);

	// ���������λ���
	graphics.DrawRectangle(&redPen, 10, 10, 100, 100);
	graphics.DrawRectangle(&redPen, 120, 10, 100, 100);

	// ���ö��뷽ʽΪ����, ��Բ
	graphics.DrawEllipse(&greenPen, 10, 10, 100, 100);
	graphics.DrawEllipse(&blackPen, 10, 10, 100, 100);

	// ���ö��뷽ʽΪInset
	greenPen.SetAlignment(PenAlignmentInset);
	blackPen.SetAlignment(PenAlignmentInset);
	graphics.DrawEllipse(&greenPen, 120, 10, 100, 100);
	graphics.DrawEllipse(&blackPen, 120, 10, 100, 100);
#endif

#if 0 ///<-- 3-4 ���ʵ����ź���ת�任
	Graphics graphics(hdc);

	// ������Ϊ5����ɫ����
	Pen pen(Color::Blue, 5);
	graphics.DrawEllipse(&pen, 50, 50, 200, 200);

	// �������ڴ�ֱ����������6����ˮƽ���򱣳ֲ���
	pen.ScaleTransform(1, 6);

	// ʹ��δ����ת����Ļ��ʻ�Բ
	graphics.DrawEllipse(&pen, 300, 50, 200, 200);

	// ���ν�������ת60�㡢120�㡢180��
	pen.RotateTransform(60,MatrixOrderAppend);
	graphics.DrawEllipse(&pen, 600, 50, 200, 200);

	pen.RotateTransform(60, MatrixOrderAppend);
	graphics.DrawEllipse(&pen, 50, 300, 200, 200);

	pen.RotateTransform(60, MatrixOrderAppend);
	graphics.DrawEllipse(&pen, 300, 300, 200, 200);

	pen.RotateTransform(90, MatrixOrderAppend);
	graphics.DrawEllipse(&pen, 600, 300, 200, 200);
#endif

#if 0 ///<-- 3-5 ���ʵ��Զ�����ñ
	Graphics graphics(hdc);
	GraphicsPath startPath, endPath;

	// ·�������һ������
	startPath.AddRectangle(Rect(-10, -5, 20, 10));

	// �����������ñ����ۺͼ�ͷ
	endPath.AddLine(0, -20, 10, 0);
	endPath.AddLine(0, -20, -10, 0);
	endPath.AddLine(0, -10, 10, 0);
	endPath.AddLine(0, -10, -10, 0);
	
	Rect rect(10, - 5, 20, 10);
	Pen pen(Color::Red, 2);
	// ��·����Ϊ��ñ���
	CustomLineCap custCap(NULL, &startPath);
	CustomLineCap endCap(NULL, &endPath);

	// ���û�������յ���Զ�����ñ
	pen.SetCustomStartCap(&custCap);
	pen.SetCustomEndCap(&endCap);
	graphics.DrawLine(&pen, 20, 30, 300, 30);

	//pen.SetStartCap(LineCapRound);
	//pen.SetEndCap(LineCapTriangle);
	//graphics.DrawLine(&pen, 20, 60, 200, 60);

	for (float i = 0.f; i < 6.28f;i+=0.15f)
	{
		graphics.DrawLine(&pen, 300.f, 300.f, 300+200.f*cos(i), 300+200*sin(i));
	}

#endif

#if 0 ///<-- 3-7 ��̬���ƺ���͸���ȵ�����
	Graphics graphics(hdc);
	Pen bluePen(Color::Blue);
	Pen redPen(Color::Red);

	// ��������
	int y = 256;
	for (int x = 0; x < 256; x+=5)
	{
		graphics.DrawLine(&bluePen, 0, y, x, 0);
		graphics.DrawLine(&redPen, 256, y, x, 256);
		y -= 5;
		Sleep(50);// ��ʱ����Ч��
	}

	// ͸���ȴ��ϵ��µݼ�
	for (y = 0; y < 256;y++)
	{
		Pen greenPen(Color(y,0,255,0));
		graphics.DrawLine(&greenPen,0,y,256, y);
		Sleep(20);// ��ʱ����Ч��
	}
	// ͸���ȴ����ҵݼ�
	for (int x = 0; x < 256; x++)
	{
		Pen greenPen(Color(x, 0, 255, 0));
		graphics.DrawLine(&greenPen, x, 100, x, 200);
		Sleep(20);// ��ʱ����Ч��
	}
#endif
#if 0 ///<-- 3-8 ��ɫ��ˢ�������
	// ��ˢֻ����������޿�ȳ��ȶ���
	Graphics graphics(hdc);
	SolidBrush greenBrush(Color::Green);
	PointF Point1(100.f, 100.f);
	PointF Point2(200.f, 50.f);
	PointF Point3(250.f, 200.f);
	PointF Point4(50.f, 150.f);
	PointF Point5(100.f, 100.f);

	// ���պ�����
	PointF points[4] = { Point1, Point2, Point3, Point4 };
	graphics.FillClosedCurve(&greenBrush, points, 4, FillModeAlternate, 1.);
	
	PointF Point6(400.f, 100.f);
	PointF Point7(500.f, 50.f);
	PointF Point8(550.f, 200.f);
	PointF Point9(350.f, 150.f);
	PointF Point10(400.f, 100.f);
	PointF pointsPoly[5] = { Point6, Point7, Point8, Point9, Point10 };
	// �����������
	graphics.FillPolygon(&greenBrush, pointsPoly, 5);
#endif

#if 1 // 3-9 �����Ҷ����
	Graphics graphics(hdc);

	// ������ɫ��ˢ
	SolidBrush greenBrush(Color::Green);

	// ָ�����ĵ�
	int cx = 300;
	int cy = 300;
	int leafLength = 100;// ����Ҷ��
	int leafNum = 5;// ҶƬ����
	float PI = 3.14;
	int x2, y2;
	int x, y, r;

	// ����ͼ��·������������Ҷ���߽߱���
	GraphicsPath tmpPath(FillModeAlternate);

	// �������߽߱����ݣ��Ƕȱ仯һ��ΪPI*2;
	for (float i = 0.f; i < PI * 2 + 0.f;i+=PI/180)
	{
		r = abs(leafLength*cos(leafNum*i));
		x = r*cos(i);
		y = r*sin(i);
		x2 = cx + x;
		y2 = cy + y;
		//graphics.DrawLine(&Pen, x2, y2, x2 - 1, y2 - 1);
		tmpPath.AddLine(x2, y2, x2, y2);
	}
	graphics.FillPath(&greenBrush, &tmpPath);

	// ��������������
	Pen pen(Color::Gray, 1);
	graphics.DrawLine(&pen, 0, cy, cx * 2, cy);
	graphics.DrawLine(&pen, cx, 0, cx, cy*2);

#endif
}

int CXqWindow:: GetWindth()
{
	return m_IWidth;
}

int CXqWindow::GetHight()
{
	return m_IHight;
}

void CXqWindow::OnLButtonDown(HWND hwnd, int x, int y)
{

	InvalidateRect(hwnd, NULL, FALSE);
}

void CXqWindow::OnLButtonUp(HWND hwnd, int x, int y)
{
	InvalidateRect(hwnd, NULL, FALSE);
}

void CXqWindow::OnMouseMove(HWND hwnd, int x, int y)
{
	InvalidateRect(hwnd, NULL, FALSE);
}

void CXqWindow::OnSize(HWND hwnd, int w, int h)
{
	width = w;
	height = h;
	InvalidateRect(hwnd, NULL, FALSE);
}

void OnCreate(HWND hwnd)
{
}

void CXqWindow::Create()
{
	m_WClassEx.cbSize = sizeof(WNDCLASSEX);
	m_WClassEx.lpfnWndProc = WndProc;
	m_WClassEx.hInstance = m_HInstance;
	m_WClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_WClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	m_WClassEx.lpszClassName = L"windows";
	m_WClassEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	m_WClassEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&m_WClassEx)) {
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return ;
	}

	m_HWnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"windows", L"GDI+ [feijianshen]", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		m_IPosX,
		m_IPosY,
		m_IWidth,
		m_IHight,
		NULL, NULL, m_HInstance, NULL);

	if (m_HWnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return ;
	}
}

void CXqWindow::Create(LPCWSTR lpszClassName)
{
	memset(&m_WClassEx, 0, sizeof(m_WClassEx));
	m_WClassEx.cbSize = sizeof(WNDCLASSEX);
	m_WClassEx.lpfnWndProc = WndProc;
	m_WClassEx.hInstance = m_HInstance;
	m_WClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_WClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	m_WClassEx.lpszClassName = lpszClassName;
	m_WClassEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	m_WClassEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&m_WClassEx)) {
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	m_HWnd = CreateWindowEx(WS_EX_CLIENTEDGE, lpszClassName, lpszClassName, WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		m_IPosX, 
		m_IPosY,
		m_IWidth,
		m_IHight,
		NULL, NULL, m_HInstance, NULL);

	if (m_HWnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return;
	}
}

void CXqWindow::MoveWindow(int x, int y, int width, int hight, BOOL bRepaint)
{
	::MoveWindow(m_HWnd,x, y, width, hight, bRepaint);
}
void CXqWindow::ShowWindow(int nCmdShow)
{
	::ShowWindow(m_HWnd, nCmdShow);
	UpdateWindow();
}

void CXqWindow::UpdateWindow(void)
{
	::UpdateWindow(m_HWnd);
}

WPARAM CXqWindow::MessageLoop(void)
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}



int CXqWindow::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}


LRESULT CALLBACK CXqWindow::WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) 
{
	switch (Message) {
	case WM_CREATE:
		OnCreate(hwnd);
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);
		OnDraw(ps.hdc);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		OnLButtonDown(hwnd, LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_LBUTTONUP:
	{
		OnLButtonUp(hwnd, LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		OnMouseMove(hwnd, LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_SIZE:
	{
		OnSize(hwnd, LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}