
// ini���� program1.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Cini����program1App:
// �� Ŭ������ ������ ���ؼ��� ini���� program1.cpp�� �����Ͻʽÿ�.
//

class Cini����program1App : public CWinApp
{
public:
	Cini����program1App();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern Cini����program1App theApp;