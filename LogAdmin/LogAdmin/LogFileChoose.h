#pragma once
#include "afxcmn.h"
#include "afxext.h"


// CLogFileChoose dialog

class CLogFileChoose : public CDialogEx
{
	DECLARE_DYNAMIC(CLogFileChoose)

public:
	CLogFileChoose(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogFileChoose();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGFILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListLogFile;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnNMClickListLogfile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonLogaccept();

	afx_msg void OnBnClickedButtonLogaccept2();
	afx_msg void OnBnClickedButtonFolder();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBitmapButton m_btnOk;
	CBitmapButton m_btnFolder;
};
