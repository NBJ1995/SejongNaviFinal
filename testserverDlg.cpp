
// testserverDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "testserver.h"
#include "testserverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CtestserverDlg 대화 상자



CtestserverDlg::CtestserverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTSERVER_DIALOG, pParent)
	, m_serverText(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_serverText);
}

BEGIN_MESSAGE_MAP(CtestserverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CtestserverDlg::OnBnClickedButtonStart)
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_MESSAGE(UM_ACCEPT, OnAccept)
END_MESSAGE_MAP()


// CtestserverDlg 메시지 처리기
BOOL CtestserverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	AfxSocketInit();
	m_serverText = "서버 상태 : 시작 안함";
	m_socCom = NULL;
	UpdateData(FALSE);

	bool b = OpenDatabase();
	// 데이터베이스 안열릴때 처리
	if (b == FALSE)
	{
		m_serverText = "서버 상태 : 데이터베이스 오류";
		UpdateData(FALSE);
		return FALSE;
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CtestserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CtestserverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CtestserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtestserverDlg::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_socServer.Create(5000);
	m_socServer.Listen();
	m_socServer.Init(this->m_hWnd);
	m_serverText = "서버 상태 : 시작";

	// 서버 처리가 끝날때까지 닫아둠.
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

LPARAM CtestserverDlg::OnReceive(UINT wParam, LPARAM lParam)
{
	char pTmp[256];
	CString strTmp;
	CString strSQL;
	CString temp;
	CRecordset recSet(&g_dbTest);

	memset(pTmp, '\0', 256);

	m_socCom->Receive(pTmp, 256);

	strTmp.Format("%s", pTmp);

	int iRows = atoi(strTmp);

	if (iRows > -1) {
		// 클라이언트 실행시 0이 옴. DB의 열 갯수를 보냄
		if (iRows == 0)
		{
			strSQL.Format(_T("SELECT * FROM [Rows] WHERE [ID] = 1"));
			recSet.Open(CRecordset::forwardOnly, strSQL);
			recSet.GetFieldValue(_T("sejongDBrows"), temp);
			n_rows = atoi(temp);

			// 제일 앞글자에 0을 붙여서 보냄
			sprintf(pTmp, _T("%d%s"), 0, temp);
			m_socCom->Send(pTmp, 256);
		}
		else if (n_rows +1 > iRows)
		{
			memset(pTmp, '\0', 256);

			// 제일 앞에 열 숫자를 넣음. 이 구간에서 iRows는 해당 ID의 것을 전부 보냄
			sprintf(pTmp, _T("%d"), iRows);

			strSQL.Format(_T("SELECT * FROM [sejongDB] WHERE [ID] = %d"), iRows);
			recSet.Open(CRecordset::forwardOnly, strSQL);
			recSet.GetFieldValue(_T("posX"), temp);
			sprintf(pTmp + 5, _T("%s"), temp);

			recSet.GetFieldValue(_T("posY"), temp);
			sprintf(pTmp + 10, _T("%s"), temp);

			recSet.GetFieldValue(_T("rectLeft"), temp);
			sprintf(pTmp + 15, _T("%s"), temp);

			recSet.GetFieldValue(_T("rectTop"), temp);
			sprintf(pTmp + 20, _T("%s"), temp);

			recSet.GetFieldValue(_T("rectRight"), temp);
			sprintf(pTmp + 25, _T("%s"), temp);

			recSet.GetFieldValue(_T("rectBottom"), temp);
			sprintf(pTmp + 30, _T("%s"), temp);

			recSet.GetFieldValue(_T("connectNode1"), temp);
			sprintf(pTmp + 35, _T("%s"), temp);

			recSet.GetFieldValue(_T("connectNode2"), temp);
			sprintf(pTmp + 40, _T("%s"), temp);

			recSet.GetFieldValue(_T("connectNode3"), temp);
			sprintf(pTmp + 45, _T("%s"), temp);

			recSet.GetFieldValue(_T("connectNode4"), temp);
			sprintf(pTmp + 50, _T("%s"), temp);

			recSet.GetFieldValue(_T("distanceNode1"), temp);
			sprintf(pTmp + 55, _T("%s"), temp);

			recSet.GetFieldValue(_T("distanceNode2"), temp);
			sprintf(pTmp + 60, _T("%s"), temp);

			recSet.GetFieldValue(_T("distanceNode3"), temp);
			sprintf(pTmp + 65, _T("%s"), temp);

			recSet.GetFieldValue(_T("distanceNode4"), temp);
			sprintf(pTmp + 70, _T("%s"), temp);

			recSet.GetFieldValue(_T("bBuliding"), temp);
			sprintf(pTmp + 100, _T("%s"), temp);
			
			// 빌딩인 경우에만 빌딩 이름을 넣음.
			if (temp == '1') {
				recSet.GetFieldValue(_T("bulidName"), temp);
				sprintf(pTmp + 120, _T("%s"), temp);
			}
			m_socCom->Send(pTmp, 256);
		}
		else
		{
			// DB의 rows값 이상일 때 그 값을 보내서 -1을 전송하게 하고 닫게한다.
			memset(pTmp, '\0', 256);
			sprintf(pTmp, _T("%d"), iRows);
			m_socCom->Send(pTmp, 256);
		}
	}
	else if (iRows = -1)
	{
		// iRows가 -1로 오면 이미 클라이언트도 close되었고 모든 값을 다 받은것을 확인함. 서버도 닫고 다시 버튼을 누를수 있게 되었고
		// 그 버튼을 누르면 새로운 클라이언트가 실행되어도 받을 수 있는 상태가 됨
		m_socCom->Close();
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		m_serverText = "서버 상태 : 완료 (다시 시작가능)";
		m_socCom = NULL;
		m_socServer.Close();
		UpdateData(FALSE);
	}

	return TRUE;
}
LPARAM CtestserverDlg::OnAccept(UINT wParam, LPARAM lParam)
{
	m_socCom = new CSocCom;

	m_socCom = m_socServer.GetAcceptSocCom();
	m_socCom->Init(this->m_hWnd);

	m_serverText = "서버 상태 : 연결됨";
	UpdateData(FALSE);

	return TRUE;
}

// 데이터베이스 열기
BOOL CtestserverDlg::OpenDatabase()
{
	TRY
	{
		// ODBC DSN이름을 test로 함. 수정하면 언제든지 사용 가능.
		g_dbTest.OpenEx(_T("DSN=test;UID=;PWD="),CDatabase::noOdbcDialog);
	}
		CATCH(CException, e)
	{
		TCHAR szCause[255];

		e->GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause, MB_ICONERROR);

		if (g_dbTest.IsOpen())
			g_dbTest.Close();

		return FALSE;
	}
	END_CATCH;

	return TRUE;
}