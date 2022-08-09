
// ini���� program1Dlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ini���� program1.h"
#include "ini���� program1Dlg.h"
#include "afxdialogex.h"
#include "Server.h"
#include "Client.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CServer serverDlg;
CClient clientDlg;
CServer* g_pServer = NULL;
CClient* g_pClient = NULL;

int num;

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cini����program1Dlg ��ȭ ����



Cini����program1Dlg::Cini����program1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INIPROGRAM1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cini����program1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cini����program1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BUTTON1, &Cini����program1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// Cini����program1Dlg �޽��� ó����

BOOL Cini����program1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	num = 0;

	char *cBuf = NULL;
	cBuf = (char *)malloc(sizeof(char) * 256);
	memset(cBuf, 0x00, sizeof(cBuf));
	GetPrivateProfileString(_T("Program"), _T("Select"), _T("-"), (LPTSTR)cBuf, 256, _T("C:\\Temp\\ProjectSimul2.ini"));
	num = atoi(cBuf);

	if (num == 1)
	{
		serverDlg.DoModal();
		ShowWindow(SW_SHOWMINIMIZED);//! �ּ�ȭ�� ���ܾ� ȭ�鿡 ��Ÿ���� ����
		PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
	}
	else if (num == 2)
	{
		clientDlg.DoModal();
		ShowWindow(SW_SHOWMINIMIZED);//! �ּ�ȭ�� ���ܾ� ȭ�鿡 ��Ÿ���� ����
		PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
	}

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void Cini����program1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void Cini����program1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR Cini����program1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cini����program1Dlg::OnBnClickedButton1()
{
	if (IsDlgButtonChecked(IDC_RADIO1) == FALSE && IsDlgButtonChecked(IDC_RADIO2) == FALSE)
	{
		AfxMessageBox(_T("üũ�ϼ���."));
		WritePrivateProfileString(_T("Program"), _T("Select"), _T("0"), _T("C:\\Temp\\ProjectSimul2.ini"));
	}
	else if (IsDlgButtonChecked(IDC_RADIO1) == TRUE)
	{
		WritePrivateProfileString(_T("Program"), _T("Select"), _T("1"), _T("C:\\Temp\\ProjectSimul2.ini"));
	}
	else if (IsDlgButtonChecked(IDC_RADIO2) == TRUE)
	{
		WritePrivateProfileString(_T("Program"), _T("Select"), _T("2"), _T("C:\\Temp\\ProjectSimul2.ini"));
	}

	char *cBuf = NULL;
	cBuf = (char *)malloc(sizeof(char) * 256);
	memset(cBuf, 0x00, sizeof(cBuf));
	GetPrivateProfileString(_T("Program"), _T("Select"), _T("-"), (LPTSTR)cBuf, 256, _T("C:\\Temp\\ProjectSimul2.ini"));
	num = atoi(cBuf);

	if (num == 1)
	{
		serverDlg.DoModal();
		ShowWindow(SW_SHOWMINIMIZED);//! �ּ�ȭ�� ���ܾ� ȭ�鿡 ��Ÿ���� ����
		PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
	}
	else if (num == 2)
	{
		clientDlg.DoModal();
		ShowWindow(SW_SHOWMINIMIZED);//! �ּ�ȭ�� ���ܾ� ȭ�鿡 ��Ÿ���� ����
		PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
	}
}
// ** ����� �Ϸ�Ǹ� ���α׷��Ӱ� ������ ������Ѵ�.
// m_pTestDlg->DestroyWindow();
// m_pTestDlg = NULL;