// GSNetClientDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NetClientDemo.h"
#include "NetClientDemoDlg.h"
#include "DialogAddress.h"
#include "DialogFullScreen.h"
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "../Include/NetClient.h"
#include "../include/avplayer.h"
#include "../include/search.h"


#pragma comment(lib, "../lib/NetClient")
#pragma comment(lib, "../lib/avplayer")
#pragma comment(lib, "../lib/search")

HANDLE		g_playerHandle = 0;
HWND		g_playerHwnd = NULL;//IDC_STATIC_3

 void  CALLBACK DataCallBack ( HANDLE handle, char* pBuf, long nSize, STREAM_INFO* pBufInfo, void *lpContext  )
{
	//文件头
	if (pBufInfo->type == STREAM_HEADER )
	{
		OpenStream(pBufInfo, &g_playerHandle);
		if (g_playerHandle)
		{
			Play(g_playerHandle, g_playerHwnd);
		}
		
	}
	else if (pBufInfo->type == STREAM_VIDEO)//视频
	{	
		if (g_playerHandle)
		{
			InputData(g_playerHandle, pBuf, nSize, pBufInfo);
		}
		
	}
	else if (pBufInfo->type == STREAM_AUDIO)//音频
	{
		if (g_playerHandle)
		{
			InputData(g_playerHandle, pBuf, nSize, pBufInfo);
		}
		
	}
}

 void CALLBACK MsgCallBack(HANDLE handle, long msgType,  void*lpStruct, void *lpContext)
 {
	switch(msgType)
	{
	case MSG_LINK_OK:
		TRACE("MSG_LINK_OK\n");
		break;

	case MSG_LINK_CONNECTING:
		TRACE("MSG_LINK_CONNECTING\n");
		break;

	case MSG_LINK_RECONNECT:
		TRACE("MSG_LINK_RECONNECT\n");
		break;

	case MSG_LINK_FAILED:
		TRACE("MSG_LINK_FAILED\n");
		break;

	case MSG_LINK_UNAUTHORIZED:
		TRACE("MSG_LINK_OK\n");
		break;

		//录像状态void*lpStruct = char*filename
	case MSG_RECORD_BEGIN:
		TRACE("MSG_RECORD_BEGIN  file %s\n", (char*)lpStruct);
		break;

	case MSG_RECORD_WRITE_ERROR:
		TRACE("MSG_RECORD_WRITE_ERROR  file %s\n", (char*)lpStruct);
		break;

	case MSG_RECORD_FORMAT_CHANGED://此时已经停止录像, 如果需要录像 则重新调用开启录像
		TRACE("MSG_RECORD_FORMAT_CHANGED  file %s\n", (char*)lpStruct);
		break;

	case MSG_RECORD_PACKET_FINISH:
		TRACE("MSG_RECORD_PACKET_FINISH  file %s\n", (char*)lpStruct);
		break;

		//报警
	case MSG_MOTION_DETECT :
		TRACE("MSG_MOTION_DETECT\n");
		break;

	case MSG_MOTION_DETECT_END:
		TRACE("MSG_MOTION_DETECT END...\n");
		break;

	case MSG_VIDEO_LOST:
		TRACE("MSG_VIDEO_LOST\n");
		break;

	case MSG_ALARM_IN:
		TRACE("MSG_ALARM_IN  io index %d\n", *(int*)lpStruct);
		break;

	case MSG_SHOT_FINISHED:
		TRACE("snapshot finished  %s\n", (char*)lpStruct);
		break;

	case MSG_UPGRADE_POS:
		TRACE("upgrade:pos %d", *(int*)lpStruct);
	default:
		break;
	}
 }


#define TIMER_ID_REPLAY			1001


void WINAPI OSDCallbackFun(HANDLE handle, HDC hDC, void* lpContext)
{
	if (handle != 0)
	{
		//TRACE("OSDCallbackFun\n");
	}
}


void CALLBACK SearchCallBackFun(NetParam* pParam)
{
	TRACE("ip_value: %s\n", pParam->ip_value);
}

WNDPROC	wpOrigButtonProc;
#define WM_MYBUTTON		(WM_USER+1)
#define BC_DOWN			10
#define BC_UP			11
LRESULT CALLBACK MyButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hWndParent;
	LONG id;
	hWndParent = GetParent(hWnd);
	id = GetWindowLong(hWnd, GWL_ID);
	WPARAM _wParam;
	LPARAM _lParam;
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		_wParam = (ULONG)((ULONG)id | ((ULONG)(BC_DOWN<<16)));
		_lParam = (LPARAM)hWnd;
		SendMessage(hWndParent, WM_COMMAND, _wParam, _lParam);
		break;
	case WM_LBUTTONUP:
		_wParam = (ULONG)((ULONG)id | ((ULONG)(BC_UP<<16)));
		_lParam = (LPARAM)hWnd;
		SendMessage(hWndParent, WM_COMMAND, _wParam, _lParam);
		break;
	}

	return CallWindowProc(wpOrigButtonProc, hWnd, uMsg, wParam, lParam); 
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGSNetClientDemoDlg 对话框


CGSNetClientDemoDlg::CGSNetClientDemoDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGSNetClientDemoDlg::IDD, pParent)
, m_nCacheFrame(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGSNetClientDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_BRIGHTNESS, m_ctrlSpinBrightness);
	DDX_Control(pDX, IDC_EDIT_BRIGHTNESS, m_ctrlBrightness);
	DDX_Control(pDX, IDC_SPIN_Contrast, m_ctrlSpinContrast);
	DDX_Control(pDX, IDC_EDIT_Contrast, m_ctrlContrast);
	DDX_Control(pDX, IDC_SPIN_Saturation, m_ctrlSpinSaturation);
	DDX_Control(pDX, IDC_EDIT_Saturation, m_ctrlSaturation);
	DDX_Text(pDX, IDC_EDIT1, m_nCacheFrame);
}

BEGIN_MESSAGE_MAP(CGSNetClientDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	//	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FILE, &CGSNetClientDemoDlg::OnBnClickedButtonOpenFile)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_FILE, &CGSNetClientDemoDlg::OnBnClickedButtonCloseFile)
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_LIVE_RTSP, &CGSNetClientDemoDlg::OnBnClickedButtonLiveRtsp)
	ON_BN_CLICKED(IDC_BUTTON_STOP_LIVE_RTSP, &CGSNetClientDemoDlg::OnBnClickedButtonStopLiveRtsp)
	ON_BN_CLICKED(IDC_BUTTON_START_RECORD, &CGSNetClientDemoDlg::OnBnClickedButtonStartRecord)
	ON_BN_CLICKED(IDC_BUTTON_STOP_RECORD, &CGSNetClientDemoDlg::OnBnClickedButtonStopRecord)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CGSNetClientDemoDlg::OnBnClickedButtonCapture)
	ON_BN_CLICKED(IDC_BUTTON_FULL, &CGSNetClientDemoDlg::OnBnClickedButtonFull)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_WND, &CGSNetClientDemoDlg::OnBnClickedButtonRefreshWnd)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON_VP_GET, &CGSNetClientDemoDlg::OnBnClickedButtonVpGet)
	ON_BN_CLICKED(IDC_BUTTON_VP_SET, &CGSNetClientDemoDlg::OnBnClickedButtonVpSet)
	ON_BN_CLICKED(IDC_BUTTON_START_VIEW, &CGSNetClientDemoDlg::OnBnClickedButtonStartView)
	ON_BN_CLICKED(IDC_BUTTON_STOP_VIEW, &CGSNetClientDemoDlg::OnBnClickedButtonStopView)
	ON_BN_CLICKED(IDC_BUTTON_ALARM_RECORD, &CGSNetClientDemoDlg::OnBnClickedButtonAlarmRecord)
	ON_BN_CLICKED(IDC_BUTTON_FAST, &CGSNetClientDemoDlg::OnBnClickedButtonFast)
	ON_BN_CLICKED(IDC_BUTTON_SLOW, &CGSNetClientDemoDlg::OnBnClickedButtonSlow)
	ON_BN_CLICKED(IDC_BUTTON_NORMAL, &CGSNetClientDemoDlg::OnBnClickedButtonNormal)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CGSNetClientDemoDlg::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STEP, &CGSNetClientDemoDlg::OnBnClickedButtonStep)
	ON_BN_CLICKED(IDC_BUTTON_SEEK, &CGSNetClientDemoDlg::OnBnClickedButtonSeek)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUE, &CGSNetClientDemoDlg::OnBnClickedButtonContinue)
	ON_BN_CLICKED(IDC_BUTTON_START_TALK, &CGSNetClientDemoDlg::OnBnClickedButtonStartTalk)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_IN, &CGSNetClientDemoDlg::OnBnClickedButtonZoomIn)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_OUT, &CGSNetClientDemoDlg::OnBnClickedButtonZoomOut)
	ON_BN_CLICKED(IDC_BUTTON_SOUND, &CGSNetClientDemoDlg::OnBnClickedButtonSound)
	ON_BN_CLICKED(IDC_BUTTON3, &CGSNetClientDemoDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CGSNetClientDemoDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CGSNetClientDemoDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CGSNetClientDemoDlg 消息处理程序

BOOL CGSNetClientDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_hPlaybackHandle = NULL;
	m_hLiveRTSPHandle = NULL;

	//搜索库函数,注意调用Search_Release
	Search_Inital();
	Search_RegistCallback(SearchCallBackFun);

	NET_Startup();
	NET_SetWaitTime();


	Initial();
	g_playerHwnd = GetDlgItem(IDC_STATIC_3)->GetSafeHwnd();
	
	GetDlgItem(IDC_BUTTON_STOP_LIVE_RTSP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP_RECORD)->EnableWindow(FALSE);

	CButton *pBtnLeft = (CButton *)GetDlgItem(IDC_BUTTON_PTZ_LEFT);
	CButton *pBtnRight = (CButton *)GetDlgItem(IDC_BUTTON_PTZ_RIGHT);
	CButton *pBtnUp = (CButton *)GetDlgItem(IDC_BUTTON_PTZ_UP);
	CButton *pBtnDown = (CButton *)GetDlgItem(IDC_BUTTON_PTZ_DOWN);
	CButton *pBtnZoomIn  = (CButton *)GetDlgItem(IDC_BUTTON_ZOOM_IN);
	CButton *pBtnZoomOut = (CButton *)GetDlgItem(IDC_BUTTON_ZOOM_OUT);
	wpOrigButtonProc = (WNDPROC)SetWindowLong(pBtnLeft->m_hWnd, GWL_WNDPROC, (LRESULT)MyButtonProc);
	wpOrigButtonProc = (WNDPROC)SetWindowLong(pBtnRight->m_hWnd, GWL_WNDPROC, (LRESULT)MyButtonProc);
	wpOrigButtonProc = (WNDPROC)SetWindowLong(pBtnUp->m_hWnd, GWL_WNDPROC, (LRESULT)MyButtonProc);
	wpOrigButtonProc = (WNDPROC)SetWindowLong(pBtnDown->m_hWnd, GWL_WNDPROC, (LRESULT)MyButtonProc);
	wpOrigButtonProc = (WNDPROC)SetWindowLong(pBtnZoomIn->m_hWnd, GWL_WNDPROC, (LRESULT)MyButtonProc);
	wpOrigButtonProc = (WNDPROC)SetWindowLong(pBtnZoomOut->m_hWnd, GWL_WNDPROC, (LRESULT)MyButtonProc);

	

	m_channel = 0;

	m_strURL = "192.168.1.168";

	CRegistry reg;
	if(!reg.VerifyKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\NetClientDemo"))
	{
		reg.CreateKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\NetClientDemo");
		reg.Write("URL", "192.168.83.252");
		reg.Write("CHANNEL", 0);
		reg.Write("PROTOCOL", 0);
	}
	reg.Open( HKEY_LOCAL_MACHINE, "SOFTWARE\\NetClientDemo" );
	reg.Read("URL", m_strURL);
	reg.Read("CHANNEL", m_channel);
	reg.Read("PROTOCOL", m_protocol);

	m_ctrlBrightness.SetWindowText("128");
	m_ctrlSpinBrightness.SetBuddy(GetDlgItem(IDC_EDIT_BRIGHTNESS));
	m_ctrlSpinBrightness.SetRange(0, 255);

	m_ctrlContrast.SetWindowText("128");
	m_ctrlSpinContrast.SetBuddy(GetDlgItem(IDC_EDIT_Contrast));
	m_ctrlSpinContrast.SetRange(0, 255);

	m_ctrlSaturation.SetWindowText("128");
	m_ctrlSpinSaturation.SetBuddy(GetDlgItem(IDC_EDIT_Saturation));
	m_ctrlSpinSaturation.SetRange(0, 255);

	//CRect rc;
	//GetDlgItem(IDC_STATIC_1)->GetClientRect(rc);
	//int height = rc.right*720/1280;
	//rc.MoveToXY(10,10);
	//rc.bottom = rc.top + height;
	//GetDlgItem(IDC_STATIC_1)->MoveWindow(rc);

#ifndef _DEBUG
	::SetWindowPos(   GetSafeHwnd(),   HWND_TOPMOST,   0,   0,   0,   0,   SWP_NOMOVE|SWP_NOSIZE);
#endif

	//SetTimer(100, 10000, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGSNetClientDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

//void CGSNetClientDemoDlg::OnPaint()
//{
//	if (IsIconic())
//	{
//		CPaintDC dc(this); // 用于绘制的设备上下文
//
//		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
//
//		// 使图标在工作矩形中居中
//		int cxIcon = GetSystemMetrics(SM_CXICON);
//		int cyIcon = GetSystemMetrics(SM_CYICON);
//		CRect rect;
//		GetClientRect(&rect);
//		int x = (rect.Width() - cxIcon + 1) / 2;
//		int y = (rect.Height() - cyIcon + 1) / 2;
//
//		// 绘制图标
//		dc.DrawIcon(x, y, m_hIcon);
//	}
//	else
//	{
//		CDialog::OnPaint();
//	}
//}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CGSNetClientDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGSNetClientDemoDlg::OnBnClickedButtonOpenFile()
{
	//GetDlgItem(IDC_STATIC_1)->MoveWindow(0, 0, 700, 400);

	if(NULL != m_hPlaybackHandle)
	{
		Stop(m_hPlaybackHandle);
		CloseRecFile(m_hPlaybackHandle);
		m_hPlaybackHandle = NULL;
	}

	CString m_filename;
	char Filter[]="avi文件(*.avi)|*.avi|au文件(*.au)|*.au|All(*.*)|*.*||";
	CFileDialog dlgOpen(TRUE,0,0,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,(LPCTSTR)Filter,NULL);
	if(dlgOpen.DoModal() == IDOK)
	{
		m_filename=dlgOpen.GetPathName();
	}
	else
	{
		return ;//不处理, 直接返回
	}
	OpenRecFile(m_filename.GetBuffer(), &m_hPlaybackHandle);
	if(m_hPlaybackHandle == NULL)	
	{
		AfxMessageBox("OpenFile FAILED");
	}
	else
	{
		Play(m_hPlaybackHandle, GetDlgItem(IDC_STATIC_1)->m_hWnd);
		SetMSGCallBack(m_hPlaybackHandle, MsgCallBack, NULL);
	}

	long total = 0;
	GetFileTotalTime(m_hPlaybackHandle, &total);
	SetDlgItemInt(IDC_STATIC_TOTAL_TIME, total, FALSE);
	SetTimer(TIMER_ID_REPLAY, 1000, NULL);
}


void CGSNetClientDemoDlg::OnBnClickedButtonCloseFile()
{	
	if(NULL != m_hPlaybackHandle)
	{
		KillTimer(TIMER_ID_REPLAY);
		Stop(m_hPlaybackHandle);
		CloseRecFile(m_hPlaybackHandle);
		m_hPlaybackHandle = NULL;
		GetDlgItem(IDC_BUTTON_OPEN_FILE)->EnableWindow(TRUE); 
	}
}

void CGSNetClientDemoDlg::OnClose()
{
	if( m_hPlaybackHandle)
	{
		Stop(m_hPlaybackHandle);
		CloseRecFile(m_hPlaybackHandle);
		m_hPlaybackHandle = NULL;
	}

	if( m_hLiveRTSPHandle)
	{
		NET_ClientStop(m_hLiveRTSPHandle);
		m_hLiveRTSPHandle = NULL;
	}

	if (g_playerHandle)
	{
		//注意顺序, 和调用的时候成对使用openstrem->play->stop->closestream
		Stop(g_playerHandle);
		CloseStream(g_playerHandle);
		g_playerHandle = NULL;
	}


	//release dll
	Search_Release();

	NET_Cleanup();

	Free();
	CDialog::OnClose();
}

BOOL CGSNetClientDemoDlg::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}

void CGSNetClientDemoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//::MapWindowPoints(GetDlgItem(IDC_STATIC_1)->m_hWnd, m_hWnd, (LPPOINT)&rect, 2);
	//if(NULL == m_hLiveRTSPHandle || !GSNET_ClientRefreshWnd(m_hLiveRTSPHandle))
	{
		CBrush brush(RGB(10, 10, 10));
		CWnd *pWnd = GetDlgItem(IDC_STATIC_1);
		RECT rect;
		if(pWnd){
			pWnd->GetClientRect(&rect);
			CDC *pDC = pWnd->GetDC();
			if(pDC){
				pDC->FillRect(&rect, &brush);
				pDC->SetBkMode(TRANSPARENT);
				pWnd->ReleaseDC(pDC);
			}
		}

		//pWnd = GetDlgItem(IDC_STATIC_2);
		//if(pWnd){
		//	pWnd->GetClientRect(&rect);
		//	CDC *pDC = pWnd->GetDC();
		//	if(pDC){
		//		pDC->FillRect(&rect, &brush);
		//		pDC->SetBkMode(TRANSPARENT);
		//		pWnd->ReleaseDC(pDC);
		//	}
		//}

		//pWnd = GetDlgItem(IDC_STATIC_3);
		//if(pWnd){
		//	pWnd->GetClientRect(&rect);
		//	CDC *pDC = pWnd->GetDC();
		//	if(pDC){
		//		pDC->FillRect(&rect, &brush);
		//		pDC->SetBkMode(TRANSPARENT);
		//		pWnd->ReleaseDC(pDC);
		//	}
		//}

		//pWnd = GetDlgItem(IDC_STATIC_4);
		//if(pWnd){
		//	pWnd->GetClientRect(&rect);
		//	CDC *pDC = pWnd->GetDC();
		//	if(pDC){
		//		pDC->FillRect(&rect, &brush);
		//		pDC->SetBkMode(TRANSPARENT);
		//		pWnd->ReleaseDC(pDC);
		//	}
		//}
	}

	if(NULL == m_hPlaybackHandle || !NET_ClientRefreshWnd(m_hPlaybackHandle)){
		CBrush brush(RGB(10, 10, 10));
		CWnd *pWnd = GetDlgItem(IDC_STATIC_1);
		RECT rect;
		if(pWnd){
			pWnd->GetClientRect(&rect);
			CDC *pDC = pWnd->GetDC();
			if(pDC){
				pDC->FillRect(&rect, &brush);
				pDC->SetBkMode(TRANSPARENT);
				pWnd->ReleaseDC(pDC);
			}
		}
	}
}

void CGSNetClientDemoDlg::OnBnClickedButtonLiveRtsp()
{
	RECT rc={0,0,0,0};
	::GetWindowRect(NULL, &rc);

	CDialogAddress dlg;
	dlg.m_StrIPAddress = m_strURL;
	dlg.m_CurChannel = m_channel;
	dlg.m_Protocol = m_protocol;
	if(IDCANCEL == dlg.DoModal()){
		return;
	}

	m_strURL   = dlg.m_StrIPAddress;
	m_channel  = dlg.m_CurChannel;
	m_protocol = dlg.m_Protocol;

	CRegistry reg;
	if(!reg.VerifyKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\NetClientDemo"))
	{
		reg.CreateKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\NetClientDemo");
		reg.Close();
	}
	reg.Open( HKEY_LOCAL_MACHINE, "SOFTWARE\\NetClientDemo" );
	reg.Write("URL", m_strURL);
	reg.Write("CHANNEL", m_channel);
	reg.Write("PROTOCOL", m_protocol);

	CHANNEL_CLIENTINFO channelInfo;
	memset(&channelInfo, 0, sizeof(CHANNEL_CLIENTINFO));
	sprintf(channelInfo.host, "%s", m_strURL);
	//sprintf(channelInfo.host, "%s", "192.168.1.80");
	channelInfo.port = 554;//RTSP port
	channelInfo.webport = 80;//http port
	channelInfo.protocol = dlg.m_Protocol;
	channelInfo.stream = dlg.m_CurChannel;
	channelInfo.hVideoWnd = GetDlgItem(IDC_STATIC_1)->m_hWnd;

	channelInfo.model = 0x00;//注意这个地方的设置

	channelInfo.msgFun = MsgCallBack;

	strcpy(channelInfo.username, "admin");
	strcpy(channelInfo.password, "admin");
	
	m_hLiveRTSPHandle = NET_ClientStart(&channelInfo, DataCallBack);


	if(m_hLiveRTSPHandle)
	{
		GetDlgItem(IDC_BUTTON_LIVE_RTSP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STOP_LIVE_RTSP)->EnableWindow(TRUE);

	}
	else
	{
		AfxMessageBox("NET_ClientStart failed");
	}
}

void CGSNetClientDemoDlg::OnBnClickedButtonStopLiveRtsp()
{
	if(NULL != m_hLiveRTSPHandle)
	{
		NET_ClientStop(m_hLiveRTSPHandle);
		m_hLiveRTSPHandle = NULL;

		if (g_playerHandle)
		{
			Stop(g_playerHandle);
			CloseStream(g_playerHandle);
			g_playerHandle = NULL;
		}

		GetDlgItem(IDC_BUTTON_LIVE_RTSP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP_LIVE_RTSP)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_START_RECORD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP_RECORD)->EnableWindow(FALSE);

		Invalidate();
	}
}

void CGSNetClientDemoDlg::OnBnClickedButtonStartRecord()
{
	char filename[256];
	if(NULL == m_hLiveRTSPHandle) 
		return;
	sprintf(filename,"d:\\Record\\Normal\\2010-11-03\\test_%d.avi", ::GetTickCount());
	NET_ClientStartRecord(m_hLiveRTSPHandle, filename , 0);

	GetDlgItem(IDC_BUTTON_START_RECORD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP_RECORD)->EnableWindow(TRUE);
}

void CGSNetClientDemoDlg::OnBnClickedButtonStopRecord()
{
	if(NULL == m_hLiveRTSPHandle) 
		return;

	NET_ClientStopRecord(m_hLiveRTSPHandle);

	GetDlgItem(IDC_BUTTON_START_RECORD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP_RECORD)->EnableWindow(FALSE);
}

void CGSNetClientDemoDlg::OnBnClickedButtonCapture()
{
	if(NULL == m_hLiveRTSPHandle)
		return;

	char path[256];
	sprintf(path, "d:/test-%d.jpg", GetTickCount());


	if(NULL != m_hLiveRTSPHandle)
	{
		NET_ClientCapturePicture(m_hLiveRTSPHandle, path);
	}
	else if (NULL != m_hPlaybackHandle)
	{
		Snapshot(m_hPlaybackHandle, path);
	}

}

void CGSNetClientDemoDlg::OnBnClickedButtonFull()
{
	CDialogFullScreen dlg(this);
	dlg.DoModal();

	if(NULL != m_hLiveRTSPHandle){
		NET_ClientSetWnd(m_hLiveRTSPHandle, GetDlgItem(IDC_STATIC_1)->m_hWnd);
		NET_ClientRefreshWnd(m_hLiveRTSPHandle);
	}
	else if(NULL != m_hPlaybackHandle){
		NET_ClientSetWnd(m_hPlaybackHandle, GetDlgItem(IDC_STATIC_1)->m_hWnd);
		NET_ClientRefreshWnd(m_hPlaybackHandle);
	}
}

void CGSNetClientDemoDlg::OnBnClickedButtonRefreshWnd()
{
	if(NULL != m_hLiveRTSPHandle)
		NET_ClientRefreshWnd(m_hLiveRTSPHandle);
}


LRESULT CGSNetClientDemoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_TIMER)
	{
		if(wParam == TIMER_ID_REPLAY)
		{
			if(NULL != m_hPlaybackHandle)
			{
				long ls;
				GetPlayTime(m_hPlaybackHandle, &ls);
				SetDlgItemInt(IDC_STATIC_CUR_TIME, ls, FALSE);
			}
		}
		else if (wParam == 100)
		{

		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CGSNetClientDemoDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	//TRACE("id=%d, code=%d\n", nID, nCode);
	if(nID == IDC_BUTTON_PTZ_LEFT){
		if(nCode == BC_DOWN){
			if(NULL != m_hLiveRTSPHandle)
				NET_ClientPTZCtrl(m_hLiveRTSPHandle, PAN_LEFT, 10);
		}else if(nCode == BC_UP){
			if(NULL != m_hLiveRTSPHandle)
				NET_ClientPTZCtrl(m_hLiveRTSPHandle, PTZ_STOP, 0);
		}
	}
	else if(nID == IDC_BUTTON_PTZ_RIGHT){
		if(nCode == BC_DOWN){
			if(NULL != m_hLiveRTSPHandle)
				NET_ClientPTZCtrl(m_hLiveRTSPHandle, PAN_RIGHT, 10);
		}else if(nCode == BC_UP){
			if(NULL != m_hLiveRTSPHandle)
				NET_ClientPTZCtrl(m_hLiveRTSPHandle, PTZ_STOP, 0);
		}
	}
	else if(nID == IDC_BUTTON_PTZ_UP){
		if(nCode == BC_DOWN){
			if(NULL != m_hLiveRTSPHandle)
				NET_ClientPTZCtrl(m_hLiveRTSPHandle, TILT_UP, 10);
		}else if(nCode == BC_UP){
			if(NULL != m_hLiveRTSPHandle)
				NET_ClientPTZCtrl(m_hLiveRTSPHandle, PTZ_STOP, 0);
		}
	}
	else if(nID == IDC_BUTTON_PTZ_DOWN){
		if(nCode == BC_DOWN){
			if(NULL != m_hLiveRTSPHandle)
				NET_ClientPTZCtrl(m_hLiveRTSPHandle, TILT_DOWN, 10);
		}else if(nCode == BC_UP){
			if(NULL != m_hLiveRTSPHandle)
				NET_ClientPTZCtrl(m_hLiveRTSPHandle, PTZ_STOP, 0);
		}
	}
	else if(nID == IDC_BUTTON_ZOOM_IN){
		if(nCode == BC_DOWN){
			if(NULL != m_hLiveRTSPHandle)
				NET_ClientPTZCtrl(m_hLiveRTSPHandle, PTZ_ZOOM_IN, 10);
		}
		else if(nCode == BC_UP){
			if(NULL != m_hLiveRTSPHandle)
				NET_ClientPTZCtrl(m_hLiveRTSPHandle, PTZ_STOP, 0);
		}
	}
	else if(nID == IDC_BUTTON_ZOOM_OUT){
		if(nCode == BC_DOWN){
			if(NULL != m_hLiveRTSPHandle)
				NET_ClientPTZCtrl(m_hLiveRTSPHandle, PTZ_ZOOM_OUT, 10);
		}
		else if(nCode == BC_UP){
			if(NULL != m_hLiveRTSPHandle)
				NET_ClientPTZCtrl(m_hLiveRTSPHandle, PTZ_STOP, 0);
		}
	}
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CGSNetClientDemoDlg::HitTest(int nID, CPoint point)
{
	CRect rect;
	CWnd *pWnd = GetDlgItem(nID);
	if(pWnd){
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		return PtInRect(&rect, point);
	}

	return FALSE;
}

void CGSNetClientDemoDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//CString strMsg;
	//strMsg.Format("(%d, %d)", point.x, point.y);
	//AfxMessageBox(strMsg);
	Invalidate();
	if(HitTest(IDC_STATIC_1, point)){
		if(NULL != m_hPlaybackHandle){			
			NET_ClientSetWnd(m_hPlaybackHandle, GetDlgItem(IDC_STATIC_1)->m_hWnd);			
		}
	}
	else if(HitTest(IDC_STATIC_2, point))
	{
		if(NULL != m_hPlaybackHandle){
			NET_ClientSetWnd(m_hPlaybackHandle, GetDlgItem(IDC_STATIC_2)->m_hWnd);
		}
	}
	else if(HitTest(IDC_STATIC_3, point)){
		if(NULL != m_hPlaybackHandle){
			NET_ClientSetWnd(m_hPlaybackHandle, GetDlgItem(IDC_STATIC_3)->m_hWnd);
		}
	}
	else if(HitTest(IDC_STATIC_4, point)){
		if(NULL != m_hPlaybackHandle){
			NET_ClientSetWnd(m_hPlaybackHandle, GetDlgItem(IDC_STATIC_4)->m_hWnd);
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CGSNetClientDemoDlg::OnBnClickedButtonVpGet()
{
	BYTE cbBrightness, cbContrast, cbSaturation, cbSharpness;
	if(NULL != m_hLiveRTSPHandle)
	{
		if(!NET_ClientGetVideoParam(m_hLiveRTSPHandle, &cbBrightness, &cbContrast, &cbSaturation, &cbSharpness))
		{
			AfxMessageBox("GetVideoParam failed");
			return;
		}
		m_ctrlSpinBrightness.SetPos(cbBrightness);
		m_ctrlSpinContrast.SetPos(cbContrast);
		m_ctrlSpinSaturation.SetPos(cbSaturation);
	}
}

void CGSNetClientDemoDlg::OnBnClickedButtonVpSet()
{
	if(NULL != m_hLiveRTSPHandle)
	{
		if(!NET_ClientSetVideoParam(m_hLiveRTSPHandle, 
			m_ctrlSpinBrightness.GetPos(),
			m_ctrlSpinContrast.GetPos(), 
			m_ctrlSpinSaturation.GetPos(),
			128,
			0
			))
		{
			AfxMessageBox("SetVideoParam failed");
			return;
		}
	}
}

void CGSNetClientDemoDlg::OnBnClickedButtonStartView()
{
	if(NULL != m_hLiveRTSPHandle)
	{
		NET_ClientSetWnd(m_hLiveRTSPHandle, GetDlgItem(IDC_STATIC_1)->GetSafeHwnd());
	}
}

void CGSNetClientDemoDlg::OnBnClickedButtonStopView()
{
	if(NULL != m_hLiveRTSPHandle)
	{
		NET_ClientSetWnd(m_hLiveRTSPHandle, NULL);
	}
}

void CGSNetClientDemoDlg::OnBnClickedButtonAlarmRecord()
{
	//if(m_hLiveRTSPHandle) 
		//NET_ClientStartRecord_Another(m_hLiveRTSPHandle, "D:\\GS_Record\\ALARM\\2008-11-03\\test777.h264", 20);
}

void CGSNetClientDemoDlg::OnBnClickedButtonFast()
{
	if(m_hPlaybackHandle) 
		SetSpeed(m_hPlaybackHandle, 2);

}

void CGSNetClientDemoDlg::OnBnClickedButtonSlow()
{
	if(m_hPlaybackHandle) 
		SetSpeed(m_hPlaybackHandle, -2);

}

void CGSNetClientDemoDlg::OnBnClickedButtonNormal()
{
	if(m_hPlaybackHandle) 
		SetSpeed(m_hPlaybackHandle, 0);

}

void CGSNetClientDemoDlg::OnBnClickedButtonPause()
{
	if(m_hPlaybackHandle) 
		Pause(m_hPlaybackHandle, 1);

}

void CGSNetClientDemoDlg::OnBnClickedButtonStep()
{	
	if(m_hPlaybackHandle)
	{
		OneByOne(m_hPlaybackHandle);
	}
}

void CGSNetClientDemoDlg::OnBnClickedButtonSeek()
{


	if(m_hPlaybackHandle)
	{
		SetPlayTime(m_hPlaybackHandle, GetDlgItemInt(IDC_EDIT_SEEK));
	}
}

void CGSNetClientDemoDlg::OnBnClickedButtonContinue()
{
	if(m_hPlaybackHandle)
	{
		Pause(m_hPlaybackHandle, 0);
	}
}

void CGSNetClientDemoDlg::OnBnClickedButtonStartTalk()
{
	CString strTitle;

	if(m_hLiveRTSPHandle != NULL)
	{
		GetDlgItem(IDC_BUTTON_START_TALK)->GetWindowText(strTitle);
		if(strTitle == "StartTalk"){
			if(NET_ClientStartTalk(m_hLiveRTSPHandle))
				GetDlgItem(IDC_BUTTON_START_TALK)->SetWindowText("StopTalk");
		}
		else{
			NET_ClientStopTalk(m_hLiveRTSPHandle);
			GetDlgItem(IDC_BUTTON_START_TALK)->SetWindowText("StartTalk");
		}
	}
}

void CGSNetClientDemoDlg::OnBnClickedButtonZoomIn()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CGSNetClientDemoDlg::OnBnClickedButtonZoomOut()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CGSNetClientDemoDlg::OnBnClickedButtonSound()
{
	static BOOL bSound = FALSE;
	if(m_hLiveRTSPHandle){
		if(!bSound)
			NET_ClientPlayAudio(m_hLiveRTSPHandle);
		else
			NET_ClientStopAudio(m_hLiveRTSPHandle);
		bSound = !bSound;
	}
}


void CGSNetClientDemoDlg::OnBnClickedButton3()
{
	UpdateData(TRUE);
	if(m_hLiveRTSPHandle)
	{
		NET_SetRestoreFrame(m_hLiveRTSPHandle, m_nCacheFrame);
	}
}

//测试搜索
void CGSNetClientDemoDlg::OnBnClickedButton4()
{
	Search_StartSearch();
}

void CGSNetClientDemoDlg::OnBnClickedButton5()
{
	long ret = 0;
	ret = NET_Upgrade("192.168.1.123", 80, "c:\\ar0331_box_nowdr_1.0.0.30.package", MsgCallBack, this);
	if (ret != 0)
	{
		if (ret == -5)
		{
			AfxMessageBox("open file failed ");
		}
		else if (ret == -100)
		{
			AfxMessageBox("net error!");
		}
		else if (ret == 1)
		{
			AfxMessageBox("upgrade file error");
		}
		else if (ret == 2)
		{
			AfxMessageBox("Check file failed");
		}
	}
}
