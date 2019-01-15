
// testserverDlg.h: 헤더 파일
//

#pragma once


// CtestserverDlg 대화 상자
class CtestserverDlg : public CDialogEx
{
// 생성입니다.
public:
	CtestserverDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	//추가 파트
	CSocCom *m_socCom;
	CSocServer m_socServer;
	CDatabase g_dbTest;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_serverText;
	afx_msg void OnBnClickedButtonStart();
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lParam);
	afx_msg LPARAM OnAccept(UINT wParam, LPARAM lParam);
	BOOL OpenDatabase();
	int n_rows;
};
