// LogSaveSet message handlers
// LogSaveSet.cpp : implementation file
//

#include "stdafx.h"
#include "LogAdmin.h"
#include "LogAdminDlg.h"
#include "LogSaveSet.h"
#include "afxdialogex.h"
#include"XLEzAutomation.h"


// CLogSaveSet dialog

CLogSaveSet* m_LSSHld;
BOOL bSaveDay = TRUE;

IMPLEMENT_DYNAMIC(CLogSaveSet, CDialogEx)

CLogSaveSet::CLogSaveSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{
	m_LSSHld = this;
}

CLogSaveSet::~CLogSaveSet()
{
}

void CLogSaveSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_Button_Info);
	DDX_Control(pDX, IDC_CHECK2, m_Button_Error);
	DDX_Control(pDX, IDC_CHECK3, m_Button_Warning);

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	DDX_Control(pDX, IDC_EDIT_AUTODELSET, m_Edit_AutoSet);
	DDX_Control(pDX, IDC_COMBO2, m_procListComboBox2);
	DDX_Control(pDX, IDC_BUTTON_AUTODELSET, m_btnSave1);
	DDX_Control(pDX, IDC_BUTTON1, m_btnSave2);
	DDX_Control(pDX, IDC_BUTTON_AUTODELSET2, m_btnSave3);
}


BEGIN_MESSAGE_MAP(CLogSaveSet, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CLogSaveSet::OnBnClickedButton1)

	ON_BN_CLICKED(IDC_BUTTON_AUTODELSET, &CLogSaveSet::OnBnClickedButtonAutodelset)
	ON_BN_CLICKED(IDC_RADIO1, &CLogSaveSet::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON_AUTODELSET2, &CLogSaveSet::OnBnClickedButtonAutodelset2)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL CLogSaveSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;

	m_btnSave1.LoadBitmaps(IDB_BITMAP6, IDB_BITMAP5);
	m_btnSave1.SizeToContent();

	m_btnSave2.LoadBitmaps(IDB_BITMAP6, IDB_BITMAP5);
	m_btnSave2.SizeToContent();

	m_btnSave3.LoadBitmaps(IDB_BITMAP6, IDB_BITMAP5);
	m_btnSave3.SizeToContent();

	bSaveDay = m_MainDlg->b_LogSaveDay;
	//ComboBox 초기화
	m_procListComboBox2.AddString(_T("All Application"));
	m_procListComboBox2.SetCurSel(0);

	for (int i = 0; i < m_MainDlg->ProcList.size(); i++)
	{
		CString str = m_MainDlg->ProcList[i].procName;

		m_procListComboBox2.AddString(str);
	}

	// TODO:  Add extra initialization here

	int nSaveSet = m_MainDlg->n_DelDay;
	CString strTemp;
	strTemp.Format(_T("%d"), nSaveSet);

	m_Edit_AutoSet.SetWindowText(strTemp);

	if (m_MainDlg->b_LogSave == TRUE)
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(false);		
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(true);		
	}

	if (bSaveDay == TRUE)
	{
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(false);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(false);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(true);
	}

	if (m_MainDlg->n_LogSaveSet == 0)
	{
		m_Button_Info.SetCheck(true);
		m_Button_Error.SetCheck(true);
		m_Button_Warning.SetCheck(true);
	}
	else if (m_MainDlg->n_LogSaveSet == 1)
	{
		m_Button_Info.SetCheck(true);
		m_Button_Error.SetCheck(false);
		m_Button_Warning.SetCheck(false);
	}
	else if (m_MainDlg->n_LogSaveSet == 2)
	{
		m_Button_Info.SetCheck(false);
		m_Button_Error.SetCheck(true);
		m_Button_Warning.SetCheck(false);
	}
	else if (m_MainDlg->n_LogSaveSet == 3)
	{
		m_Button_Info.SetCheck(false);
		m_Button_Error.SetCheck(false);
		m_Button_Warning.SetCheck(true);
	}
	else if (m_MainDlg->n_LogSaveSet == 4)
	{
		m_Button_Info.SetCheck(true);
		m_Button_Error.SetCheck(true);
		m_Button_Warning.SetCheck(false);
	}
	else if (m_MainDlg->n_LogSaveSet == 5)
	{
		m_Button_Info.SetCheck(true);
		m_Button_Error.SetCheck(false);
		m_Button_Warning.SetCheck(true);
	}
	else if (m_MainDlg->n_LogSaveSet == 6)
	{
		m_Button_Info.SetCheck(false);
		m_Button_Error.SetCheck(true);
		m_Button_Warning.SetCheck(true);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// CLogSaveSet message handlers
void CLogSaveSet::OnClose()
{
	((CLogAdminDlg*)GetParent())->LogSaveSet = FALSE;
	DestroyWindow();
	delete this;
}


//두번째 저장
void CLogSaveSet::OnBnClickedButton1()
{
	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
	/*
	m_Button_Info
	m_Button_Error
	m_Button_Warning
	*/
	//m_procListComboBox2
	//m_MainDlg->nSaveProg;
	//m_MainDlg->strSaveProg;
	BOOL setProc = FALSE;

	m_MainDlg->nSaveProg = m_procListComboBox2.GetCurSel();

	if (m_MainDlg->nSaveProg != 0)
	{
		//선택된 프로세스이름 가져오기
		m_procListComboBox2.GetLBText(m_procListComboBox2.GetCurSel(), m_MainDlg->strSaveProg);
	}

	if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck() == true)
	{
		m_MainDlg->b_LogSave = TRUE;
	}

	else
	{
		m_MainDlg->b_LogSave = FALSE;
	}


	if (m_Button_Info.GetCheck()== true && m_Button_Error.GetCheck() == false && m_Button_Warning.GetCheck() == false)
	{
		m_MainDlg->n_LogSaveSet = 1;
		//OnClose();
	}
	else if (m_Button_Info.GetCheck() == false && m_Button_Error.GetCheck() == true && m_Button_Warning.GetCheck() == false) //
	{
		m_MainDlg->n_LogSaveSet = 2;
		//OnClose();
	}
	else if (m_Button_Info.GetCheck() == false && m_Button_Error.GetCheck() == false && m_Button_Warning.GetCheck() == true)
	{
		m_MainDlg->n_LogSaveSet = 3;
		//OnClose();
	}
	else if (m_Button_Info.GetCheck() == true && m_Button_Error.GetCheck() == true && m_Button_Warning.GetCheck() == false)
	{
		m_MainDlg->n_LogSaveSet = 4;
		//OnClose();
	}
	else if (m_Button_Info.GetCheck() == true && m_Button_Error.GetCheck() == false && m_Button_Warning.GetCheck() == true)
	{
		m_MainDlg->n_LogSaveSet = 5;
		//OnClose();
	}
	else if (m_Button_Info.GetCheck() == false && m_Button_Error.GetCheck() == true && m_Button_Warning.GetCheck() == true)
	{
		m_MainDlg->n_LogSaveSet = 6;
		//OnClose();
	}
	else if (m_Button_Info.GetCheck() == true && m_Button_Error.GetCheck() == true && m_Button_Warning.GetCheck() == true)
	{
		m_MainDlg->n_LogSaveSet = 0;
		//OnClose();
	}
	else if (m_Button_Info.GetCheck() == false && m_Button_Error.GetCheck() == false && m_Button_Warning.GetCheck() == false)
	{
		AfxMessageBox(_T("저장 내용을 설정하세요."));
	}
}

void CLogSaveSet::OnBnClickedButtonAutodelset()
{
	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
	CString strTemp;
	m_Edit_AutoSet.GetWindowText(strTemp);
	int nSaveSet = _ttoi(strTemp);

	if (nSaveSet > 365)
	{
		AfxMessageBox(_T("날짜가 너무 깁니다."));
	}
	else if (nSaveSet > 0 && nSaveSet <= 365)
	{
		m_MainDlg->n_DelDay = nSaveSet;
	}
	else
	{
		AfxMessageBox(_T("잘못입력하셨습니다."));
	}
}


void CLogSaveSet::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
}


void CLogSaveSet::OnBnClickedButtonAutodelset2()
{
	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;

	if (((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck() == true)
	{
		bSaveDay = TRUE;
	}

	else
	{
		bSaveDay = FALSE;
	}
	m_MainDlg->b_LogSaveDay = bSaveDay;
	//OnClose();
}


HBRUSH CLogSaveSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(RGB(50, 50, 50));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	}

	return hbr;
}


BOOL CLogSaveSet::OnEraseBkgnd(CDC* pDC)
{
	/// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(50, 50, 50));
	return TRUE;
}
