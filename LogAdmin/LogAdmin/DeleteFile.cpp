// DeleteFile.cpp : implementation file
//

#include "stdafx.h"
#include "LogAdmin.h"
#include "LogAdminDlg.h"
#include "DeleteFile.h"
#include "afxdialogex.h"

#include "DeleteTool.h"

CDeleteFile *g_pDel;
CString sIndexValue;
CString DirValue;
int DelMode;
// CDeleteFile dialog

CLogAdminDlg *m_MainDlg;

IMPLEMENT_DYNAMIC(CDeleteFile, CDialogEx)

CDeleteFile::CDeleteFile(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
	m_MainDlg = (CLogAdminDlg*)AfxGetMainWnd();
	g_pDel = this;
}

CDeleteFile::~CDeleteFile()
{
}

void CDeleteFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_Cpos, m_Edit_CFree);
	DDX_Control(pDX, IDC_EDIT_CPULL, m_Edit_CPull);
	DDX_Control(pDX, IDC_EDIT_DPOS, m_Edit_DFree);
	DDX_Control(pDX, IDC_EDIT_DPULL, m_Edit_DPull);
	DDX_Control(pDX, IDC_STATIC_CDISK, m_sCtext);
	DDX_Control(pDX, IDC_STATIC_DDISK, m_SDtext);
	DDX_Control(pDX, IDC_LIST_FOLDER, m_ListFolder);
	DDX_Control(pDX, IDC_EDIT2, m_SampleEdit);


	SetListView(&m_ListFolder);
	CheckDisk(&m_Edit_CFree, &m_Edit_CPull, &m_sCtext, &m_Edit_DFree, &m_Edit_DPull, &m_SDtext);
	sIndexValue = _T("");
	DelMode = 0;
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnRegist);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDel2);
}


BEGIN_MESSAGE_MAP(CDeleteFile, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDeleteFile::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDeleteFile::OnBnClickedButtonAdd)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FOLDER, &CDeleteFile::OnNMClickListFolder)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDeleteFile message handlers

void CDeleteFile::OnBnClickedButtonAdd()
{
	CheckFolder(g_pDel->GetSafeHwnd(), _T(""), &m_ListFolder, &m_SampleEdit);
}

void CDeleteFile::OnBnClickedButtonDelete()
{
	if (DelMode == 1)
	{
		if (DeleteFile(sIndexValue) == TRUE)
		{
			TCHAR *TTemp = (TCHAR*)(LPCTSTR)sIndexValue;
			CString Temp;
			AfxMessageBox(_T("삭제성공"));
			PathRemoveFileSpec(TTemp);
			Temp = TTemp;
			CheckFolder(g_pDel->GetSafeHwnd(), Temp, &m_ListFolder, &m_SampleEdit);
		}
		else
			AfxMessageBox(_T("삭제실패"));
	}
	else if (DelMode == 2)
	{
		if (DeleteDirectoryFile(sIndexValue) == TRUE)
		{
			AfxMessageBox(_T("삭제성공"));
			CheckFolder(g_pDel->GetSafeHwnd(), DirValue, &m_ListFolder, &m_SampleEdit);
		}
		else-
			AfxMessageBox(_T("삭제실패"));
	}
	else
	{
		AfxMessageBox(_T("선택된 데이터가 없습니다."));
	}
}



void CDeleteFile::OnNMClickListFolder(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int idx = pNMListView->iItem;

	// 선택된 아이템값의 아이템을 (0,1 ... n 번째 인덱스) 한개 가져온다.
	sIndexValue = m_ListFolder.GetItemText(idx, 1);
	if (sIndexValue != _T("Folder"))
	{
		DelMode = 1;
		sIndexValue = m_ListFolder.GetItemText(idx, 2) + "\\" + m_ListFolder.GetItemText(idx, 0) + m_ListFolder.GetItemText(idx, 1);
	}
	else
	{
		DelMode = 2;
		sIndexValue = m_ListFolder.GetItemText(idx, 2) + "\\" + m_ListFolder.GetItemText(idx, 0);
		DirValue = m_ListFolder.GetItemText(idx, 2);
	}
}

void CDeleteFile::OnClose()
{
	//for (int i = 0; i < ProcList.size(); i++)
	//{
	//	AfxMessageBox(ProcList[i].procName);
	//}
	((CLogAdminDlg*)GetParent())->DelFolderCheck = FALSE;
	DestroyWindow();
	delete this;
}



HBRUSH CDeleteFile::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CDeleteFile::OnEraseBkgnd(CDC* pDC)
{
	/// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(50, 50, 50));
	return TRUE;
}


BOOL CDeleteFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_btnDel2.LoadBitmaps(IDB_BITMAP12, IDB_BITMAP11);
	m_btnDel2.SizeToContent();

	m_btnRegist.LoadBitmaps(IDB_BITMAP14, IDB_BITMAP13);
	m_btnRegist.SizeToContent();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
