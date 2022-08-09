// ProcSetting.cpp : implementation file
//

#include "stdafx.h"
#include "LogAdmin.h"
#include "LogAdminDlg.h"
#include "ProcSetting.h"
#include "afxdialogex.h"

#include "Tool.h"
#include "Process.h"
#include"LogSaveSet.h"

// ProcSetting dialog

//CLogAdminDlg *m_MainDlg = (CLogAdminDlg*)AfxGetMainWnd();

ProcSetting* m_Proset;
int nCount = 0;

IMPLEMENT_DYNAMIC(ProcSetting, CDialogEx)

ProcSetting::ProcSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_Proset = this;
	m_pParent = pParent;
}

ProcSetting::~ProcSetting()
{
}

void ProcSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_RucProcList);

	CRect rect;

	m_RucProcList.GetClientRect(&rect);
	//리스트 컨트롤 컬럼 추가
	m_RucProcList.InsertColumn(0, _T("Program"), LVCFMT_LEFT, 100);
	m_RucProcList.InsertColumn(1, _T("Process Path"), LVCFMT_LEFT, 250);
	m_RucProcList.InsertColumn(2, _T("Check Mdoe"), LVCFMT_LEFT, 100);

	DWORD dwStyle = m_RucProcList.GetExtendedStyle();

	m_RucProcList.SetExtendedStyle(dwStyle
		| LVS_EX_FULLROWSELECT	//뭘 눌러도 선택이 되게
		| LVS_EX_GRIDLINES		//그리드 라인
		| LVS_EX_CHECKBOXES);   //체크박스
	DDX_Control(pDX, IDC_BUTTON5, m_btnDel);
	DDX_Control(pDX, IDC_BUTTON1, m_btnSearch);
	DDX_Control(pDX, IDC_BUTTON2, m_btnSave);
}


BEGIN_MESSAGE_MAP(ProcSetting, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ProcSetting::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ProcSetting::OnBnClickedButton2)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON5, &ProcSetting::OnBnClickedButton5)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// ProcSetting message handlers


void ProcSetting::OnBnClickedButton1()
{
	CString strchkMode = _T("Process Name");

	CString str = _T("All files(*.*)|*.*|"); // 모든 파일 표시
											 // _T("Excel 파일 (*.xls, *.xlsx) |*.xls; *.xlsx|"); 와 같이 확장자를 제한하여 표시
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		//CString	strName = strPathName.Right(strPathName.GetLength() - strPathName.ReverseFind('\\'));
		CString	strName = PathFindFileName(strPathName);
		// 파일 경로를 가져와 사용할 경우, Edit Control에 값 저장

		m_RucProcList.InsertItem(0, strName);
		m_RucProcList.SetItemText(0, 1, strPathName);
		m_RucProcList.SetItemText(0, 2, strchkMode);

		//////////////////////////////////////////////////////////////////
		//리스트 내에 중복값이면 빼라
		CString str, sub_str, str_first_add, str_second_add;

		// m_my_list에 있는 데이터의 총 갯수를 구한다.
		int count = m_RucProcList.GetItemCount();

		// 리스트의 마지막 인덱스부터 역순으로 올라가면서
		// 두 컬럼의 문자열을 얻어서 더한 문자열을 str_first_add에 대입한다.
		for (int i = count - 1; i >= 0; i--)
		{ // 마지막 순서 부터 -1 씩
			str = m_RucProcList.GetItemText(i, 0); // 파일 이름 현재 선택한 값
			sub_str = m_RucProcList.GetItemText(i, 1); // 파일 위치
			str_first_add = str + sub_str; // 파일 이름 + 파일 위치

										   // 중복 체크를 위해서 비교를 위해 i - 1 에 해당하는 인덱스로부터 역순으로 문자열을 얻어온다.
			for (int j = i - 1; j >= 0; j--) { // j는 i의 -1번부터 -1하며 비교
				str = m_RucProcList.GetItemText(j, 0);
				sub_str = m_RucProcList.GetItemText(j, 1);
				str_second_add = str + sub_str;

				// 위에서 얻어 온 str_first_add와 str_second_add이 같다면,
				if (str_first_add == str_second_add) {

					// 해당 아이템을 삭제한다.
					m_RucProcList.DeleteItem(j);
					// 총 인덱스 수가 변경됨에 따라 i값도 감소시킨다.
					i--;
				}

			}

		}

#pragma endregion

	}
}


//프로세스 저장
void ProcSetting::OnBnClickedButton2()
{
	ProcMember proc;
	CLogAdminDlg *wd = ((CLogAdminDlg*)GetParent());

	for (int i = 0; i < m_RucProcList.GetItemCount(); i++)
	{
		proc.procName =  m_RucProcList.GetItemText(i, 0);
		proc.procPath = m_RucProcList.GetItemText(i, 1);
		proc.procMode = m_RucProcList.GetItemText(i, 2);
		proc.procClose = FALSE;
		proc.procOverlap = FALSE;
		proc.rbtCnt = _T("0");

		wd->m_procListComboBox.AddString(proc.procName);

		wd->ListCtrlView(proc);

		CString strRealName;
		AfxExtractSubString(strRealName, proc.procName, 0, '.');
		wd->SimpleListNameSet(strRealName);
	}
	OnClose();
}

void ProcSetting::OnClose()
{
	//for (int i = 0; i < ProcList.size(); i++)
	//{
	//	AfxMessageBox(ProcList[i].procName);
	//}
	((CLogAdminDlg*)GetParent())->ProcSetCheck = FALSE;
	DestroyWindow();
	delete this;
}


//Delete
void ProcSetting::OnBnClickedButton5()
{
	int nCount = m_RucProcList.GetItemCount();

	for (int i = nCount - 1; i >= 0; i--)
	{
		if (m_RucProcList.GetCheck(i) == TRUE)
		{
			m_RucProcList.DeleteItem(i);
			AfxMessageBox(_T("Delete."));
		}
	}
}


HBRUSH ProcSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL ProcSetting::OnEraseBkgnd(CDC* pDC)
{
	/// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(50, 50, 50));
	return TRUE;
}


BOOL ProcSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_btnDel.LoadBitmaps(IDB_BITMAP16, IDB_BITMAP15);
	m_btnDel.SizeToContent();

	m_btnSave.LoadBitmaps(IDB_BITMAP17, IDB_BITMAP18);
	m_btnSave.SizeToContent();

	m_btnSearch.LoadBitmaps(IDB_BITMAP19, IDB_BITMAP20);
	m_btnSearch.SizeToContent();

	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
