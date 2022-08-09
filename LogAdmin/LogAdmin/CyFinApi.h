// CyFinApi.h : main header file for the CyFinApi DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCyFinApiApp
// See CyFinApi.cpp for the implementation of this class
//

class CCyFinApiApp : public CWinApp
{
public:
	CCyFinApiApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
