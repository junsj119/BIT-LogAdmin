
// LogAdmin.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CLogAdminApp:
// �� Ŭ������ ������ ���ؼ��� LogAdmin.cpp�� �����Ͻʽÿ�.
//

class CLogAdminApp : public CWinApp
{
public:
	CLogAdminApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CLogAdminApp theApp;