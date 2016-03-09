#pragma once
#include "afxwin.h"


// CDialogAddress �Ի���

class CDialogAddress : public CDialog
{
	DECLARE_DYNAMIC(CDialogAddress)

public:
	CDialogAddress(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogAddress();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADDRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CString		m_StrIPAddress;
	ULONG			m_CurChannel;
	ULONG			m_Protocol;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	virtual BOOL OnInitDialog();
public:
	CComboBox m_comboChannel;
public:
	CEdit m_ctrlIP;
public:
	CComboBox m_comboProtocol;
};
