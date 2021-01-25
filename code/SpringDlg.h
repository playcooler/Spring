
// SpringDlg.h: 头文件
//

#pragma once


// CSpringDlg 对话框
class CSpringDlg : public CDialogEx
{
// 构造
public:
	CSpringDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPRING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDie();
	CButton Button_Die;
	CButton Button_Live;
	afx_msg void OnBnClickedButtonLive();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_Static_Timer;
	UINT iCounter300;
	bool bNotPaint;
	afx_msg void OnNcPaint();
	CRect rStaticTimer, rStaticImg, rButtonLive,rButtonDie, rCheckAuto, rStaticKilled;
	CStatic m_Static_Img;
	CButton m_Check_AutoStart;
	afx_msg void OnBnClickedCheckAutostart();
	bool bKilled;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_Static_Killed;
	unsigned long iKilled;
};
