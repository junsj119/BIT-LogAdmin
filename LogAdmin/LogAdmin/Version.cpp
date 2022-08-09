// Version.cpp : implementation file
//

#include "stdafx.h"
#include "LogAdmin.h"
#include "LogAdminDlg.h"
#include "Version.h"
#include "afxdialogex.h"


// CVersion dialog

IMPLEMENT_DYNAMIC(CVersion, CDialogEx)

CVersion::CVersion(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

CVersion::~CVersion()
{
}

void CVersion::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_procVerCB);
	DDX_Control(pDX, IDC_BUTTON1, m_btnVersion);
}


BEGIN_MESSAGE_MAP(CVersion, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CVersion::OnBnClickedButton1)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL CVersion::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_procVerCB.AddString(_T("Choose the Process"));
	m_procVerCB.SetCurSel(0);

	m_btnVersion.LoadBitmaps(IDB_BITMAP21, IDB_BITMAP22);
	m_btnVersion.SizeToContent();

	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;
	for (int i = 0; i < m_MainDlg->ProcList.size(); i++)
	{
		CString str = m_MainDlg->ProcList[i].procName;

		m_procVerCB.AddString(str);
	}	

	return TRUE;  // return TRUE unless you set the focus to a control			  // EXCEPTION: OCX Property Pages should return FALSE
}

// CVersion message handlers
void CVersion::OnClose()
{
	((CLogAdminDlg*)GetParent())->VersionCheck = FALSE;
	DestroyWindow();
	delete this;
}

//m_procVerCB


void CVersion::OnBnClickedButton1()
{
	CLogAdminDlg* m_MainDlg = (CLogAdminDlg*)theApp.m_pMainWnd;

	CString strTemp, strComboName, selectprocName;

	//���õ� ���μ����̸� ��������
	m_procVerCB.GetLBText(m_procVerCB.GetCurSel(), selectprocName);

	//���⼭ �޺� �ڽ� ������ �̸� ��������
	strTemp = selectprocName;
	AfxExtractSubString(strComboName, strTemp, 0, '.');

	CStdioFile verfile1, verfile2;

	if (strComboName == _T("Program1"))
	{
		//�ؽ�Ʈ ������ �����ҋ� UTF-8 �� �ƴ� > ANSI�� �����ؾ� �ѱ��� �ȱ����� ����
		if (verfile1.Open(_T("c:\\Temp\\Program1_Version.txt"), CFile::modeRead | CFile::typeText)) {
			CString display_str, str;
			// ���Ͽ� ���̻� �ؽ�Ʈ ������ ���������� ���پ� �ݺ��ؼ� �о str ������ �����Ѵ�.
			while (verfile1.ReadString(str)) {
				// �о���� ������ �ؽ�Ʈ�� display_str ������ �߰��Ѵ�.
				display_str += str;
				// ��Ƽ���� �Ӽ��� ���� ����Ʈ ��Ʈ���� ���� ���� "\r\n" �� �־���� �ٹٲ��� ���ش�.
				// ReadString �Լ��� ������ �ؽ�Ʈ�� ������ �ش����� �����ִ� "\n" �� �о ������
				// ������ �ؽ�Ʈ�� ���ٳ��� "\n" �� �ִٰ� ������ �ݵ�� "\n" �� �ٿ��� �մϴ�.
				display_str += "\r\n";
			}
			// display_str ������ ����Ǿ� �ִ� ������ IDC_MY_TEXT ����Ʈ ��Ʈ�ѿ� ����Ѵ�.
			SetDlgItemText(IDC_EDIT2, display_str);
			// �۾��� ��ģ�Ŀ� ������ �ݴ´�.
			verfile1.Close();
		}
	}
	else if (strComboName == _T("Program2"))
	{
		//�ؽ�Ʈ ������ �����ҋ� UTF-8 �� �ƴ� > ANSI�� �����ؾ� �ѱ��� �ȱ����� ����
		if (verfile1.Open(_T("c:\\Temp\\Program2_Version.txt"), CFile::modeRead | CFile::typeText)) {
			CString display_str, str;
			// ���Ͽ� ���̻� �ؽ�Ʈ ������ ���������� ���پ� �ݺ��ؼ� �о str ������ �����Ѵ�.
			while (verfile1.ReadString(str)) {
				// �о���� ������ �ؽ�Ʈ�� display_str ������ �߰��Ѵ�.
				display_str += str;
				// ��Ƽ���� �Ӽ��� ���� ����Ʈ ��Ʈ���� ���� ���� "\r\n" �� �־���� �ٹٲ��� ���ش�.
				// ReadString �Լ��� ������ �ؽ�Ʈ�� ������ �ش����� �����ִ� "\n" �� �о ������
				// ������ �ؽ�Ʈ�� ���ٳ��� "\n" �� �ִٰ� ������ �ݵ�� "\n" �� �ٿ��� �մϴ�.
				display_str += "\r\n";
			}
			// display_str ������ ����Ǿ� �ִ� ������ IDC_MY_TEXT ����Ʈ ��Ʈ�ѿ� ����Ѵ�.
			SetDlgItemText(IDC_EDIT2, display_str);
			// �۾��� ��ģ�Ŀ� ������ �ݴ´�.
			verfile1.Close();
		}
	}

}


HBRUSH CVersion::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CVersion::OnEraseBkgnd(CDC* pDC)
{
	/// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(50, 50, 50));
	return TRUE;
}
