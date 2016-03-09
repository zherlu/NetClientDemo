// GSNetClientDemoDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


#define WM_GSMSG_ID			(WM_USER+100)


// CGSNetClientDemoDlg 对话框
class CGSNetClientDemoDlg : public CDialog
{


// 构造
public:
	CGSNetClientDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GSNETCLIENTDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
//	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpenFile();
public:
	afx_msg void OnBnClickedButtonCloseFile();

public:
	HANDLE m_hPlaybackHandle;
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonLiveRtsp();

	HANDLE m_hLiveRTSPHandle;
	afx_msg void OnBnClickedButtonStopLiveRtsp();
	afx_msg void OnBnClickedButtonStartRecord();
	afx_msg void OnBnClickedButtonStopRecord();
	afx_msg void OnBnClickedButtonCapture();
	afx_msg void OnBnClickedButtonFull();
public:
	afx_msg void OnBnClickedButtonRefreshWnd();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	CString m_strURL;
	ULONG	  m_channel;
	ULONG	  m_protocol;

private:
	BOOL HitTest(int nID, CPoint point);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	CSpinButtonCtrl m_ctrlSpinBrightness;
public:
	CEdit m_ctrlBrightness;
public:
	CSpinButtonCtrl m_ctrlSpinContrast;
public:
	CEdit m_ctrlContrast;
public:
	CSpinButtonCtrl m_ctrlSpinSaturation;
public:
	CEdit m_ctrlSaturation;
public:
	afx_msg void OnBnClickedButtonVpGet();
public:
	afx_msg void OnBnClickedButtonVpSet();
public:
	afx_msg void OnBnClickedButtonStartView();
public:
	afx_msg void OnBnClickedButtonStopView();
public:
	afx_msg void OnBnClickedButtonAlarmRecord();
public:
	afx_msg void OnBnClickedButtonFast();
public:
	afx_msg void OnBnClickedButtonSlow();
public:
	afx_msg void OnBnClickedButtonNormal();
public:
	afx_msg void OnBnClickedButtonPause();
public:
	afx_msg void OnBnClickedButtonStep();
public:
	afx_msg void OnBnClickedButtonSeek();
public:
	afx_msg void OnBnClickedButtonContinue();
public:
	afx_msg void OnBnClickedButtonStartTalk();
public:
	afx_msg void OnBnClickedButtonZoomIn();
public:
	afx_msg void OnBnClickedButtonZoomOut();
public:
	afx_msg void OnBnClickedButtonSound();
	UINT m_nCacheFrame;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
