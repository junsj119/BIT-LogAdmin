#pragma once
#include "afxwin.h"
#include "afxext.h"


// CVersion dialog

class CVersion : public CDialogEx
{
	DECLARE_DYNAMIC(CVersion)

public:
	CVersion(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVersion();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	CComboBox m_procVerCB;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBitmapButton m_btnVersion;
};
