#pragma once
#include "afxcmn.h"
#include <vector>
#include "Process.h"
#include "afxext.h"
using namespace std;
// ProcSetting dialog

#define WM_USER_MSG WM_USER + 10

class ProcSetting : public CDialogEx
{
	DECLARE_DYNAMIC(ProcSetting)

public:
	ProcSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~ProcSetting();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnClose();

	CListCtrl m_RucProcList;

	//vector<ProcMember> ProcList;
	CWnd* m_pParent;
	afx_msg void OnBnClickedButton5();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBitmapButton m_btnDel;
	CBitmapButton m_btnSearch;
	CBitmapButton m_btnSave;
	virtual BOOL OnInitDialog();
};