#pragma once


// CDialogFullScreen �Ի���

class CDialogFullScreen : public CDialog
{
	DECLARE_DYNAMIC(CDialogFullScreen)

public:
	CDialogFullScreen(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogFullScreen();

// �Ի�������
	enum { IDD = IDD_DIALOG_FULL_SCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	void	*m_pParent;
	afx_msg void OnPaint();
};
