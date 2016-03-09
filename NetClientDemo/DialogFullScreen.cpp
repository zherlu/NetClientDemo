// DialogFullScreen.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NetClientDemo.h"
#include "DialogFullScreen.h"
#include "NetClientDemoDlg.h"
#include "../include/NetClient.h"
#include "../Include/AVPlayer.h"


// CDialogFullScreen �Ի���

IMPLEMENT_DYNAMIC(CDialogFullScreen, CDialog)

CDialogFullScreen::CDialogFullScreen(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFullScreen::IDD, pParent)
{
	m_pParent = pParent;
}

CDialogFullScreen::~CDialogFullScreen()
{
}

void CDialogFullScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogFullScreen, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDialogFullScreen ��Ϣ�������

BOOL CDialogFullScreen::OnInitDialog()
{
	CDialog::OnInitDialog();

	MoveWindow(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), TRUE);

	if(((CGSNetClientDemoDlg *)m_pParent)->m_hLiveRTSPHandle){
		NET_ClientSetWnd(((CGSNetClientDemoDlg *)m_pParent)->m_hLiveRTSPHandle, m_hWnd);
		NET_ClientRefreshWnd(((CGSNetClientDemoDlg *)m_pParent)->m_hLiveRTSPHandle);
	}
	else if(((CGSNetClientDemoDlg *)m_pParent)->m_hPlaybackHandle){
		NET_ClientSetWnd(((CGSNetClientDemoDlg *)m_pParent)->m_hPlaybackHandle, m_hWnd);
		NET_ClientRefreshWnd(((CGSNetClientDemoDlg *)m_pParent)->m_hPlaybackHandle);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDialogFullScreen::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	RECT rect;
	GetWindowRect(&rect);

	CBrush brush(RGB(10, 10, 10));
	dc.FillRect(&rect, &brush);

	if(NULL != ((CGSNetClientDemoDlg *)m_pParent)->m_hLiveRTSPHandle)
	{
		NET_ClientRefreshWnd(((CGSNetClientDemoDlg *)m_pParent)->m_hLiveRTSPHandle);
	}
	else if(NULL != ((CGSNetClientDemoDlg *)m_pParent)->m_hPlaybackHandle)
	{
		NET_ClientRefreshWnd(((CGSNetClientDemoDlg *)m_pParent)->m_hPlaybackHandle);
	}
}
