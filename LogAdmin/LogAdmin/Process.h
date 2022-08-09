#pragma once
#include "afxcmn.h"

struct ProcMember
{
	CString procName;
	CString procPath;
	CString procMode;
	BOOL procClose;
	BOOL procOverlap;
	CString rbtCnt;
};