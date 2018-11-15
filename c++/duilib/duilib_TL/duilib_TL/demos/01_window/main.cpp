#include "stdafx.h"
//#include "HelloDuilib.h"
class CHelloDuilibWnd :
	public WindowImplBase
{
public:
	CHelloDuilibWnd(void);
	~CHelloDuilibWnd(void);

protected:
	virtual CDuiString GetSkinFolder() { return _T("skin"); };
	virtual CDuiString GetSkinFile()  { return _T("HelloDuilib.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T("HelloDuilib_Wnd"); };
};

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)

{
	CPaintManagerUI::SetInstance(hInstance);// ����XML��ʱ����Ҫʹ�øþ��ȥ��λEXE��·�������ܼ���XML��·��

	CHelloDuilibWnd* wnd = new CHelloDuilibWnd; // ���ɶ���
//	wnd->Create(NULL, NULL, UI_WNDSTYLE_DIALOG, 0); // ����DLG����
//	wnd->CenterWindow(); // ���ھ���
//	wnd->ShowWindow(); // ��ʾ
//	CPaintManagerUI::MessageLoop(); // ��Ϣѭ��

	//delete wnd; // ɾ������
}