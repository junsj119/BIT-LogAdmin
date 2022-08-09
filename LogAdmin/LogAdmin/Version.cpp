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

	//선택된 프로세스이름 가져오기
	m_procVerCB.GetLBText(m_procVerCB.GetCurSel(), selectprocName);

	//여기서 콤보 박스 설정한 이름 가져오기
	strTemp = selectprocName;
	AfxExtractSubString(strComboName, strTemp, 0, '.');

	CStdioFile verfile1, verfile2;

	if (strComboName == _T("Program1"))
	{
		//텍스트 파일을 저장할떄 UTF-8 이 아닌 > ANSI로 저장해야 한글이 안깨지고 나옴
		if (verfile1.Open(_T("c:\\Temp\\Program1_Version.txt"), CFile::modeRead | CFile::typeText)) {
			CString display_str, str;
			// 파일에 더이상 텍스트 내용이 없을때까지 한줄씩 반복해서 읽어서 str 변수에 저장한다.
			while (verfile1.ReadString(str)) {
				// 읽어들인 한줄의 텍스트를 display_str 변수에 추가한다.
				display_str += str;
				// 멀티라인 속성을 가진 에디트 컨트롤은 줄의 끝에 "\r\n" 가 있어야지 줄바꿈을 해준다.
				// ReadString 함수가 한줄의 텍스트를 읽을때 해당줄의 끝에있는 "\n" 은 읽어서 버리기
				// 때문에 텍스트의 한줄끝에 "\n" 이 있다고 할지라도 반드시 "\n" 을 붙여야 합니다.
				display_str += "\r\n";
			}
			// display_str 변수에 저장되어 있는 내용을 IDC_MY_TEXT 에디트 컨트롤에 출력한다.
			SetDlgItemText(IDC_EDIT2, display_str);
			// 작업을 마친후에 파일을 닫는다.
			verfile1.Close();
		}
	}
	else if (strComboName == _T("Program2"))
	{
		//텍스트 파일을 저장할떄 UTF-8 이 아닌 > ANSI로 저장해야 한글이 안깨지고 나옴
		if (verfile1.Open(_T("c:\\Temp\\Program2_Version.txt"), CFile::modeRead | CFile::typeText)) {
			CString display_str, str;
			// 파일에 더이상 텍스트 내용이 없을때까지 한줄씩 반복해서 읽어서 str 변수에 저장한다.
			while (verfile1.ReadString(str)) {
				// 읽어들인 한줄의 텍스트를 display_str 변수에 추가한다.
				display_str += str;
				// 멀티라인 속성을 가진 에디트 컨트롤은 줄의 끝에 "\r\n" 가 있어야지 줄바꿈을 해준다.
				// ReadString 함수가 한줄의 텍스트를 읽을때 해당줄의 끝에있는 "\n" 은 읽어서 버리기
				// 때문에 텍스트의 한줄끝에 "\n" 이 있다고 할지라도 반드시 "\n" 을 붙여야 합니다.
				display_str += "\r\n";
			}
			// display_str 변수에 저장되어 있는 내용을 IDC_MY_TEXT 에디트 컨트롤에 출력한다.
			SetDlgItemText(IDC_EDIT2, display_str);
			// 작업을 마친후에 파일을 닫는다.
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
