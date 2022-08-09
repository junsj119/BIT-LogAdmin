// LogFileChoose.cpp : implementation file
//

#include "stdafx.h"
#include "LogAdmin.h"
#include "LogAdminDlg.h"
#include "LogFileChoose.h"
#include "DeleteTool.h"
#include "afxdialogex.h"
#include "Log.h"
#include "Tool.h"
#include"AddGraph.h"


// CLogFileChoose dialog

CLogFileChoose *g_pLogChs;
CString FileDir;
CString str_ResultDir;
BOOL bSelectitem;

IMPLEMENT_DYNAMIC(CLogFileChoose, CDialogEx)

CLogFileChoose::CLogFileChoose(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOGFILE, pParent)
{
	g_pLogChs = this;
	FileDir = _T("");
	str_ResultDir = _T("");
	bSelectitem = FALSE;
}

CLogFileChoose::~CLogFileChoose()
{
}

void CLogFileChoose::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOGFILE, m_ListLogFile);
	DDX_Control(pDX, IDC_BUTTON_LOGACCEPT, m_btnOk);
	DDX_Control(pDX, IDC_BUTTON_FOLDER, m_btnFolder);
}


BEGIN_MESSAGE_MAP(CLogFileChoose, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LOGFILE, &CLogFileChoose::OnNMClickListLogfile)
	ON_BN_CLICKED(IDC_BUTTON_LOGACCEPT, &CLogFileChoose::OnBnClickedButtonLogaccept)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, &CLogFileChoose::OnBnClickedButtonFolder)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CLogFileChoose message handlers

BOOL CLogFileChoose::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_btnOk.LoadBitmaps(IDB_BITMAP21, IDB_BITMAP22);
	m_btnOk.SizeToContent();

	m_btnFolder.LoadBitmaps(IDB_BITMAP24, IDB_BITMAP23);
	m_btnFolder.SizeToContent();
	CRect rect;

	m_ListLogFile.GetClientRect(&rect);
	//리스트 컨트롤 컬럼 추가
	m_ListLogFile.InsertColumn(0, _T("FileName"), LVCFMT_LEFT, 100);
	m_ListLogFile.InsertColumn(1, _T("Extension"), LVCFMT_LEFT, 250);

	DWORD dwStyle = m_ListLogFile.GetExtendedStyle();

	m_ListLogFile.SetExtendedStyle(dwStyle
		| LVS_EX_FULLROWSELECT	//뭘 눌러도 선택이 되게
		| LVS_EX_GRIDLINES); 		//그리드 라인

	// TODO:  Add extra initialization here
	//LogFileFind(g_pLogChs->GetSafeHwnd(),&m_ListLogFile);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogFileChoose::OnNMClickListLogfile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int idx = pNMListView->iItem;
	str_ResultDir = FileDir + "\\" + m_ListLogFile.GetItemText(idx, 0) + m_ListLogFile.GetItemText(idx, 1);
	if (m_ListLogFile.GetItemText(idx, 1) == _T(".txt"))
		bSelectitem = TRUE;
	else if (m_ListLogFile.GetItemText(idx, 1) == _T(".xls"))
		bSelectitem = FALSE;
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CLogFileChoose::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	((CLogAdminDlg*)GetParent())->b_LogFile = FALSE;
	//CDialogEx::OnClose();
	DestroyWindow();
	delete this;
}

CAddGraph a;
void CLogFileChoose::OnBnClickedButtonLogaccept()
{
	// TODO: Add your control notification handler code here
	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
	m_MainDlg->SetLogFile(str_ResultDir, bSelectitem);
	a.LoadPath(str_ResultDir);

	//AfxMessageBox(str_ResultDir);
	OnClose();
	//AfxMessageBox(FileDir);
}


void CLogFileChoose::OnBnClickedButtonFolder()
{
	// TODO: Add your control notification handler code here
	FileDir = FolderChoice(g_pLogChs->GetSafeHwnd(), _T("C:\\Temp\\LogFolder"));
	LogFileFind(g_pLogChs->GetSafeHwnd(), &m_ListLogFile, FileDir);
}


HBRUSH CLogFileChoose::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CLogFileChoose::OnEraseBkgnd(CDC* pDC)
{
	/// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(50, 50, 50));
	return TRUE;
}
