#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxext.h"


// CDeleteFile dialog

class CDeleteFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDeleteFile)

public:
	CDeleteFile(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeleteFile();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonAdd();

	void OnClose();

	CEdit m_Edit_CFree;
	CEdit m_Edit_CPull;
	CEdit m_Edit_DFree;
	CEdit m_Edit_DPull;
	CStatic m_sCtext;
	CStatic m_SDtext;
	CListCtrl m_ListFolder;
	CEdit m_SampleEdit;
	afx_msg void OnNMClickListFolder(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBitmapButton m_btnRegist;
	CBitmapButton m_btnDel2;
	virtual BOOL OnInitDialog();
};
