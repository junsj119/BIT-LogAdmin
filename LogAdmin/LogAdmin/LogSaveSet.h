#pragma once
#include "afxwin.h"
#include "afxext.h"


// CLogSaveSet dialog

class CLogSaveSet : public CDialogEx
{
	DECLARE_DYNAMIC(CLogSaveSet)

public:
	CLogSaveSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogSaveSet();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void OnClose();
	afx_msg void OnBnClickedButton1();

	CButton m_Button_Info;
	CButton m_Button_Error;
	CButton m_Button_Warning;
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonAutodelset();
	CEdit m_Edit_AutoSet;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedButtonAutodelset2();
	CComboBox m_procListComboBox2;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBitmapButton m_btnSave1;
	CBitmapButton m_btnSave2;
	CBitmapButton m_btnSave3;
};
