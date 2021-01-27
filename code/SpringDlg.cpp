
// SpringDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Spring.h"
#include "SpringDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ALARM_PREMIN 8

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSpringDlg 对话框



CSpringDlg::CSpringDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SPRING_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	iCounter300 = 0;
	bNotPaint = true;
	bKilled = false;
	iKilled = 0;
}

void CSpringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DIE, Button_Die);
	DDX_Control(pDX, IDC_BUTTON_LIVE, Button_Live);
	DDX_Control(pDX, IDC_STATIC_TIMER, m_Static_Timer);
	DDX_Control(pDX, IDC_STATIC_IMG, m_Static_Img);
	DDX_Control(pDX, IDC_CHECK_AUTOSTART, m_Check_AutoStart);
	DDX_Control(pDX, IDC_STATIC_KILLED, m_Static_Killed);
}

BEGIN_MESSAGE_MAP(CSpringDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DIE, &CSpringDlg::OnBnClickedButtonDie)
	ON_BN_CLICKED(IDC_BUTTON_LIVE, &CSpringDlg::OnBnClickedButtonLive)
	ON_WM_TIMER()
	ON_WM_NCPAINT()
	ON_BN_CLICKED(IDC_CHECK_AUTOSTART, &CSpringDlg::OnBnClickedCheckAutostart)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSpringDlg 消息处理程序

BOOL CSpringDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	HKEY hKey;
	//找到系统的启动项
	CString lpRun = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	//打开启动项
	long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_ALL_ACCESS, &hKey);
	if (lRet != ERROR_SUCCESS)
		m_Check_AutoStart.SetCheck(0);
	
	//找到程序自身路径
	TCHAR pFileName[MAX_PATH] = {};
	GetModuleFileName(NULL, pFileName, MAX_PATH);
	//判断是否已经设置开机启动
	TCHAR PowerBoot[MAX_PATH] = {};
	DWORD nLongth = MAX_PATH;
	long result = RegGetValue(hKey, NULL, _T("PowerBoot"), RRF_RT_REG_SZ, 0, PowerBoot, &nLongth);
	if (result == ERROR_SUCCESS)
	{
		m_Check_AutoStart.SetCheck(1);
	}
	else
		m_Check_AutoStart.SetCheck(0);
	//关闭注册表
	RegCloseKey(hKey);
	

	m_Static_Timer.GetWindowRect(&rStaticTimer);
	m_Static_Img.GetWindowRect(&rStaticImg);
	Button_Live.GetWindowRect(&rButtonLive);
	Button_Die.GetWindowRect(&rButtonDie);
	m_Check_AutoStart.GetWindowRect(&rCheckAuto);

	int nScreenWidth, nScreenHeight;//保存分辨率
	//获取分辨率
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	SetWindowPos(&this->wndTopMost, 0, 0, nScreenWidth, nScreenHeight, SWP_HIDEWINDOW);
	int wx = (nScreenWidth - rStaticImg.Width()) / 2;
	int hy = (nScreenHeight - rStaticImg.Height()) / 2;
	rStaticImg.left += wx;
	rStaticImg.right += wx;
	rStaticImg.top += hy;
	rStaticImg.bottom += hy;

	rStaticTimer.left += wx;
	rStaticTimer.right += wx;
	rStaticTimer.top += hy;
	rStaticTimer.bottom += hy;

	rButtonLive.left += wx;
	rButtonLive.right += wx;
	rButtonLive.top += hy;
	rButtonLive.bottom += hy;

	rButtonDie.left += wx;
	rButtonDie.right += wx;
	rButtonDie.top += hy;
	rButtonDie.bottom += hy;

	rCheckAuto.left += wx;
	rCheckAuto.right += wx;
	rCheckAuto.top += hy;
	rCheckAuto.bottom += hy;

	rStaticKilled.left = rButtonDie.left;
	rStaticKilled.right = rButtonDie.right;
	rStaticKilled.top = rButtonDie.top - 25;
	rStaticKilled.bottom = rButtonDie.top -5;


	m_Static_Killed.MoveWindow(&rStaticKilled);
	CString mStrKilled;
	mStrKilled.Format(_T("%ld"), iKilled);
	m_Static_Killed.SetWindowText(mStrKilled);
	if(iKilled == 0)
		m_Static_Killed.ShowWindow(SW_HIDE);
	m_Static_Timer.MoveWindow(&rStaticTimer);
	m_Static_Img.MoveWindow(&rStaticImg);
	Button_Live.MoveWindow(&rButtonLive);
	Button_Die.MoveWindow(&rButtonDie);
	m_Check_AutoStart.MoveWindow(&rCheckAuto);

	// Start Timer
	SetTimer(1, 60000, NULL);
	int iMinute = CTime::GetCurrentTime().GetMinute();
	if (iMinute == 30 - ALARM_PREMIN || iMinute == 60 - ALARM_PREMIN)
	{
		ShowWindow(SW_SHOW);
		Button_Die.ShowWindow(SW_SHOW);
		Button_Live.ShowWindow(SW_SHOW);
		m_Static_Timer.SetWindowText(_T("300s"));
		iCounter300 = 300;
		m_Static_Timer.ShowWindow(SW_SHOW);
		SetTimer(2, 1000, NULL);
	}	
	else 
	{
		ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);//移除任务栏图标显示
		ShowWindow(SW_HIDE);
		m_Static_Timer.ShowWindow(SW_HIDE);
		Button_Die.ShowWindow(SW_HIDE);
		Button_Live.ShowWindow(SW_HIDE);		
	}
	
	


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSpringDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSpringDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
		CPaintDC dc(this);
		GetClientRect(rect);
		if(bKilled)
			dc.FillSolidRect(rect, RGB(255,0, 0)); //设置为红色背景
		else
			dc.FillSolidRect(rect, RGB(255, 255, 255)); //设置为红色背景

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSpringDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CSpringDlg::OnBnClickedButtonDie()
{
	bKilled = true;
	m_Check_AutoStart.ShowWindow(SW_HIDE);
	m_Static_Timer.ShowWindow(SW_HIDE);
	this->SetBackgroundColor(RGB(255, 0, 0));
	Sleep(500);
	ShowWindow(SW_HIDE);
	bKilled = false;
	iKilled++;
	
	int iRGB = 0;
	if (iKilled < 2) iRGB = 0;
	else if (iKilled < 4) iRGB = 51;
	else if (iKilled < 6) iRGB = 102;
	else if (iKilled < 8) iRGB = 153;
	else if (iKilled < 10) iRGB = 204;
	else iRGB = 255;
	this->SetBackgroundColor(RGB(255, 255 - iRGB, 255 - iRGB));
	CString mStrKilled;
	mStrKilled.Format(_T("%ld"), iKilled);
	m_Static_Killed.SetWindowText(mStrKilled);
	m_Static_Killed.ShowWindow(SW_SHOW);

}


void CSpringDlg::OnBnClickedButtonLive()
{
	bKilled = false;
	Button_Die.ShowWindow(SW_HIDE);
	Button_Live.ShowWindow(SW_HIDE);
	m_Check_AutoStart.ShowWindow(SW_HIDE);
}


void CSpringDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	if (nIDEvent == 1) {
		int iMinute = CTime::GetCurrentTime().GetMinute();
		if (iMinute == 30 - ALARM_PREMIN || iMinute == 60 - ALARM_PREMIN) {
			ShowWindow(SW_SHOW);
			Button_Die.ShowWindow(SW_SHOW);
			Button_Live.ShowWindow(SW_SHOW);
			m_Static_Timer.SetWindowText(_T("300s"));
			iCounter300 = 300;
			m_Static_Timer.ShowWindow(SW_SHOW);
			m_Check_AutoStart.ShowWindow(SW_SHOW);
			SetTimer(2, 1000, NULL);
		}
	}
	else if (nIDEvent == 2) {
		if (iCounter300 == 0) {
			KillTimer(2);
			ShowWindow(SW_HIDE);

		}
		else if (iCounter300 > 0) {
			iCounter300--;
			CString str;
			str.Format(_T("%ds"), iCounter300);
			m_Static_Timer.SetWindowText(str);
		}
	}

}


void CSpringDlg::OnNcPaint()
{
	if (bNotPaint)
	{
		ShowWindow(SW_HIDE);
		bNotPaint = false;
	}
	else
	{
		CDialogEx::OnNcPaint();
	}
}





void CSpringDlg::OnBnClickedCheckAutostart()
{
	// TODO: 在此添加控件通知处理程序代码
	int nChecked = m_Check_AutoStart.GetCheck();

	HKEY hKey;
	//找到系统的启动项
	CString lpRun = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	//打开启动项
	long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_ALL_ACCESS, &hKey);
	if (lRet != ERROR_SUCCESS) {
		m_Check_AutoStart.SetCheck(0);
	}
		
	//找到程序自身路径
	TCHAR pFileName[MAX_PATH] = {};
	GetModuleFileName(NULL, pFileName, MAX_PATH);
	//判断是否已经设置开机启动
	TCHAR PowerBoot[MAX_PATH] = {};
	DWORD nLongth = MAX_PATH;
	long result = RegGetValue(hKey, NULL, _T("PowerBoot"), RRF_RT_REG_SZ, 0, PowerBoot, &nLongth);

	if (result == ERROR_SUCCESS && nChecked == 0)
	{
		//取消自动启动
		lRet = RegDeleteValue(hKey, _T("PowerBoot"));
		if (lRet == ERROR_SUCCESS)
		{
			m_Check_AutoStart.SetCheck(0);
		}
	}
	else if (nChecked == 1)
	{
		//设置自启
		lRet = RegSetValueEx(hKey, _T("PowerBoot"), 0, REG_SZ, (LPBYTE)pFileName, (lstrlen(pFileName) + 1) * sizeof(TCHAR));
		if (lRet == ERROR_SUCCESS)
		{
			m_Check_AutoStart.SetCheck(1);
		}
	}
	//关闭注册表
	RegCloseKey(hKey);


}


HBRUSH CSpringDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (IDC_STATIC_KILLED == pWnd->GetDlgCtrlID())
	{
		if(iKilled<6)
			pDC->SetTextColor(RGB(255, 0, 0));	//红色
		else
			pDC->SetTextColor(RGB(255, 255, 255));
	}
	

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CSpringDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return;
}
