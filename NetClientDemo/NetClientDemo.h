// GSNetClientDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGSNetClientDemoApp:
// �йش����ʵ�֣������ GSNetClientDemo.cpp
//

class CGSNetClientDemoApp : public CWinApp
{
public:
	CGSNetClientDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGSNetClientDemoApp theApp;