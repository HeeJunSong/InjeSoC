
// SoC Project.h : SoC Project ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CSoCProjectApp:
// �� Ŭ������ ������ ���ؼ��� SoC Project.cpp�� �����Ͻʽÿ�.
//

class CSoCProjectApp : public CWinApp
{
public:
	CSoCProjectApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSoCProjectApp theApp;
