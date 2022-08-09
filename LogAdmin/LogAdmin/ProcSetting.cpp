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
	//����Ʈ ��Ʈ�� �÷� �߰�
	m_RucProcList.InsertColumn(0, _T("Program"), LVCFMT_LEFT, 100);
	m_RucProcList.InsertColumn(1, _T("Process Path"), LVCFMT_LEFT, 250);
	m_RucProcList.InsertColumn(2, _T("Check Mdoe"), LVCFMT_LEFT, 100);

	DWORD dwStyle = m_RucProcList.GetExtendedStyle();

	m_RucProcList.SetExtendedStyle(dwStyle
		| LVS_EX_FULLROWSELECT	//�� ������ ������ �ǰ�
		| LVS_EX_GRIDLINES		//�׸��� ����
		| LVS_EX_CHECKBOXES);   //üũ�ڽ�
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

	CString str = _T("All files(*.*)|*.*|"); // ��� ���� ǥ��
											 // _T("Excel ���� (*.xls, *.xlsx) |*.xls; *.xlsx|"); �� ���� Ȯ���ڸ� �����Ͽ� ǥ��
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		//CString	strName = strPathName.Right(strPathName.GetLength() - strPathName.ReverseFind('\\'));
		CString	strName = PathFindFileName(strPathName);
		// ���� ��θ� ������ ����� ���, Edit Control�� �� ����

		m_RucProcList.InsertItem(0, strName);
		m_RucProcList.SetItemText(0, 1, strPathName);
		m_RucProcList.SetItemText(0, 2, strchkMode);

		//////////////////////////////////////////////////////////////////
		//����Ʈ ���� �ߺ����̸� ����
		CString str, sub_str, str_first_add, str_second_add;

		// m_my_list�� �ִ� �������� �� ������ ���Ѵ�.
		int count = m_RucProcList.GetItemCount();

		// ����Ʈ�� ������ �ε������� �������� �ö󰡸鼭
		// �� �÷��� ���ڿ��� �� ���� ���ڿ��� str_first_add�� �����Ѵ�.
		for (int i = count - 1; i >= 0; i--)
		{ // ������ ���� ���� -1 ��
			str = m_RucProcList.GetItemText(i, 0); // ���� �̸� ���� ������ ��
			sub_str = m_RucProcList.GetItemText(i, 1); // ���� ��ġ
			str_first_add = str + sub_str; // ���� �̸� + ���� ��ġ

										   // �ߺ� üũ�� ���ؼ� �񱳸� ���� i - 1 �� �ش��ϴ� �ε����κ��� �������� ���ڿ��� ���´�.
			for (int j = i - 1; j >= 0; j--) { // j�� i�� -1������ -1�ϸ� ��
				str = m_RucProcList.GetItemText(j, 0);
				sub_str = m_RucProcList.GetItemText(j, 1);
				str_second_add = str + sub_str;

				// ������ ��� �� str_first_add�� str_second_add�� ���ٸ�,
				if (str_first_add == str_second_add) {

					// �ش� �������� �����Ѵ�.
					m_RucProcList.DeleteItem(j);
					// �� �ε��� ���� ����ʿ� ���� i���� ���ҽ�Ų��.
					i--;
				}

			}

		}

#pragma endregion

	}
}


//���μ��� ����
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
