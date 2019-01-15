

// Sejong_NaviView.cpp: CSejongNaviView 클래스의 구현
//

#include "stdafx.h"
#include "stdlib.h"
#include "SejongNavi.h"

#include "Point.h"
#include "SejongNaviDoc.h"
#include "SejongNaviView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CSejongNaviView

IMPLEMENT_DYNCREATE(CSejongNaviView, CView)

BEGIN_MESSAGE_MAP(CSejongNaviView, CView)
	//AFX_MSG_MAP(CSejong_NaviView)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_MESSAGE(UM_RECEIVE, OnReceive)
END_MESSAGE_MAP()

// CSejongNaviView 생성/소멸

CSejongNaviView::CSejongNaviView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	nState = 0;            // 기본은 선택모드 : 0
	b_socket = TRUE;
	b_endSocket = FALSE;
	m_rowNum = 0; // 초기화

	
}

CSejongNaviView::~CSejongNaviView()
{

}

BOOL CSejongNaviView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CSejongNaviView 그리기

void CSejongNaviView::OnDraw(CDC* pDC)
{
	
	CenterWindow(GetDesktopWindow());
	pDoc = GetDocument();
	
	ASSERT_VALID(pDoc);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if (!m_MemDC)
	{
		m_MemDC.CreateCompatibleDC(pDC);
		m_MemBitmap.CreateCompatibleBitmap(pDC, 1024, 1024);
		//1024*1024크기의 비트맵 설정
		m_MemDC.SelectObject(&m_MemBitmap);
	}
	else
	{
		pDC->BitBlt(0, 0, 1024, 1024, &m_MemDC, 0, 0, SRCCOPY);
		//(0, 0)위치에서 시작하는 1024*1024크기의 비트맵 생성 
	}
	
	
}


// CSejongNaviView 진단

#ifdef _DEBUG
void CSejongNaviView::AssertValid() const
{
	CView::AssertValid();
}

void CSejongNaviView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSejongNaviDoc* CSejongNaviView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSejongNaviDoc)));
	return (CSejongNaviDoc*)m_pDocument;
}
#endif //_DEBUG


// CSejongNaviView 메시지 처리기

// 커서가 건물에 닿을 시 클릭 이미지로 전환
void CSejongNaviView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (b_endSocket) {
		Point *p = pDoc->p;
		if (nState == 0)
		{
			DrawBack();                  // 배경
			if (pDoc->nStart != 0)
			{
				Draw(pDoc->nStart + 100);         // p[nStart] 위치에 클릭그림 출력
				DrawTexts(570, 470, pDoc->nStart);   // 시작위치
			}
			int i = pDoc->RefreshMove(point);
			if (i <= pDoc->n_rowNum) {
				Draw(i);               // 파란색 점
				Draw(i + 200);            // 정보	
				pDoc->nLast = i;
				Invalidate(0);
			}//draw
		}
		CView::OnMouseMove(nFlags, point);
	}
}


void CSejongNaviView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (b_endSocket) {
		if (nState == 0) // 선택
		{
			int i = pDoc->RefreshClick(point);
			if (i == 0) {
				if (pDoc->getnStart() != 0) Draw(pDoc->getnStart() + 100);   // p[nStart]위치에 클릭그림 출력
				DrawTexts(570, 470, pDoc->getnStart());            // 시작위치
				Invalidate(0);                         // 화면갱신
			}
			if (i == 1) {
				DrawBack();                        // 배경            덮기: 
				DrawTexts(570, 470, 0);               // 시작위치 정보   덮기: 입력대기
				Invalidate(0);                     // 화면갱신
			}
			if (i == 2) {
				nState = 1;										// 애니
				Draw(pDoc->getnStart() + 100);                  // 시작점에 클릭그림 출력
				Draw(pDoc->getnEnd() + 100);                  // 도착점에 클릭그림 출력
				DrawTexts(570, 470, pDoc->getnStart());         // 시작위치 정보
				DrawTexts(570, 560, pDoc->getnEnd());            // 도착위치 정보
				Invalidate(0);                     // 화면갱신
				DrawTexts(600, 650, -1);
				Invalidate(0);
				SetTimer(2, 10, NULL);   // 타이머 x,y변경 / 출력 / 끝까지가면 결과로
			}
		}
		else if (nState == 1)      // 애니
		{
			nState = 2;            // 강제 결과 전환
			KillTimer(2);         // 애니모드 타이머 정지
			SetTimer(3, 100, NULL);   // 결과모드 타이머 시작 : for(;;)문으로 한꺼번에 그리기
		}
		else if (nState == 2)      // 결과
		{
			pDoc->setnFlag(TRUE);
			pDoc->setbTemp(TRUE); // 전체그리기
			// 선택
			nState = 0;      // 선택
			DrawBack();      // 배경
			DrawTexts(570, 470, 0);            // 시작위치 정보
			DrawTexts(570, 560, 0);            // 도착위치 정보
			DrawTexts(570, 650, 0);            // 최단시간
			Invalidate(0);   // 화면갱신
			pDoc->setResult(); //초기화루틴(nStart,nEnd, 등등의 변수 초기화)
			KillTimer(3);   // 결과모드 정지
		}
		CView::OnLButtonDown(nFlags, point);
	}
}


void CSejongNaviView::OnTimer(UINT nIDEvent)
{
	if (b_endSocket) {
		// 점 이동출력
		switch (nIDEvent)
		{
		case 1:   // 초기배경 출력
			DrawBack();
			Invalidate(0);
			KillTimer(1);
			break;
		case 2:   //이동, 이동위치 출력
		{
			if (pDoc->getLastStart() == 0)                  // 초기출력
			{
				pDoc->TimerInitialization();
				// 이동한 위치에 출력
				CPen pen;
				CBrush brush;
				pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				brush.CreateSolidBrush(RGB(255, 255, 255));
				m_MemDC.SelectObject(&pen);
				m_MemDC.SelectObject(&brush);
				m_MemDC.Ellipse((int)pDoc->getLastPosX() - 7, (int)pDoc->getLastPosY() - 7, (int)pDoc->getLastPosX() + 7, (int)pDoc->getLastPosY() + 7); // 건물 포인트
				Invalidate(0);
			}
			else //애니출력
			{
				pDoc->setAnimation();


				// 이동한 위치에 출력
				CPen pen1, pen2;
				CBrush brush1, brush2;
				pen1.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				brush1.CreateSolidBrush(RGB(255, 0, 0));
				m_MemDC.SelectObject(&pen1);
				m_MemDC.SelectObject(&brush1);
				m_MemDC.Ellipse((int)((int)pDoc->getLastPosX() - 5), (int)((int)pDoc->getLastPosY() - 5), (int)((int)pDoc->getLastPosX() + 5), (int)((int)pDoc->getLastPosY() + 5));


				pen2.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				brush2.CreateSolidBrush(RGB(255, 255, 255));
				m_MemDC.SelectObject(&pen2);
				m_MemDC.SelectObject(&brush2);
				m_MemDC.Ellipse(pDoc->getPoint()[pDoc->getnStart()].x - 7, pDoc->getPoint()[pDoc->getnStart()].y - 7, pDoc->getPoint()[pDoc->getnStart()].x + 7, pDoc->getPoint()[pDoc->getnStart()].y + 7);
				m_MemDC.Ellipse(pDoc->getPoint()[pDoc->getnEnd()].x - 7, pDoc->getPoint()[pDoc->getnEnd()].y - 7, pDoc->getPoint()[pDoc->getnEnd()].x + 7, pDoc->getPoint()[pDoc->getnEnd()].y + 7);
				Invalidate(0);

				BOOL b = pDoc->drawCircle();
				if (b == TRUE) {
					nState = 2;
					KillTimer(2);
				}
			}
			break;
		}

		case 3:   // 전체경로 출력
			if (pDoc->getbTemp())
			{
				pDoc->setbTemp(0);
				DrawAllPath();   //for(;;) 안에서 m_MemDC에 위치 바꿔가며 출력

			}
			Invalidate(0);
			break;
		}
		CView::OnTimer(nIDEvent);
	}
}

void CSejongNaviView::DrawBack()
{
	if (b_endSocket) {
		CDC MainDC;
		MainDC.CreateCompatibleDC(&m_MemDC);
		CBitmap m_Bit1, *m_Bit;
		m_Bit1.LoadBitmap(IDB_BITMAP1);

		m_Bit = MainDC.SelectObject(&m_Bit1);
		m_MemDC.BitBlt(1, 1, 506, 689, &MainDC, 0, 0, SRCCOPY);


		for (int i = 0; i < pDoc->n_rowNum; i++) {
			if (pDoc->getPoint(i).bBuilding == TRUE)
			{
				CBitmap m_Bit, *m_temp;

				m_Bit.LoadBitmap(IDB_BITMAP2);
				m_temp = MainDC.SelectObject(&m_Bit);

				int tempx = 10;
				int tempy = 10;
				m_MemDC.BitBlt(pDoc->getPoint(i).x, pDoc->getPoint(i).y, tempx, tempy, &MainDC, 0, 0, SRCCOPY);

			}
		}
		MainDC.SelectObject(m_Bit1);
		MainDC.SelectObject(m_Bit);
	}
}

void CSejongNaviView::DrawTexts(int x, int y, int index)
{
	if (b_endSocket) {
		CString text;
		char buffer1[10];
		char buffer2[10];
		int temp1;
		int temp2;

		if (index != -1)
		{
			text = pDoc->p[index].cBulidName;
			m_MemDC.TextOut(x, y, text); // 시작/종료
		}
		else
		{
			temp1 = pDoc->getnDist(pDoc->getnEnd()) / 60; // 분
			temp2 = pDoc->getnDist(pDoc->getnEnd()) % 60; // 초
			itoa(temp1, buffer1, 10);
			itoa(temp2, buffer2, 10);
			strcat(buffer1, "분");
			strcat(buffer2, "초");
			strcat(buffer1, buffer2);
			m_MemDC.TextOut(x, y, (CString)buffer1);   // 결과
		}
	}
}

void CSejongNaviView::Draw(int index)
{
	if (b_endSocket) {

		CDC MainDC;
		MainDC.CreateCompatibleDC(&m_MemDC);
		CBitmap m_Bit, *m_temp;

		if (index < 100) 
		{
			if (pDoc->p[index].bBuilding == TRUE)
				m_Bit.LoadBitmap(IDB_BITMAP28);
		}
		else if (index > 100 && index < 200) 
		{
			if (pDoc->p[index - 100].bBuilding == TRUE)
				m_Bit.LoadBitmap(IDB_BITMAP3);
		}
		else {
			switch (index) 
			{
			// 관련 설명 칸 
			case 201:      m_Bit.LoadBitmap(IDB_BITMAP4);    break;
			case 202:      m_Bit.LoadBitmap(IDB_BITMAP5);    break;
			case 206:      m_Bit.LoadBitmap(IDB_BITMAP6);    break;
			case 207:      m_Bit.LoadBitmap(IDB_BITMAP7);    break;
			case 208:      m_Bit.LoadBitmap(IDB_BITMAP8);    break;
			case 211:      m_Bit.LoadBitmap(IDB_BITMAP9);    break;
			case 213:      m_Bit.LoadBitmap(IDB_BITMAP10);   break;
			case 214:      m_Bit.LoadBitmap(IDB_BITMAP11);   break;
			case 220:      m_Bit.LoadBitmap(IDB_BITMAP12);   break; 
			case 222:      m_Bit.LoadBitmap(IDB_BITMAP13);   break; 
			case 224:      m_Bit.LoadBitmap(IDB_BITMAP14);   break; 
			case 225:      m_Bit.LoadBitmap(IDB_BITMAP15);   break; 
			case 226:      m_Bit.LoadBitmap(IDB_BITMAP16);   break; 
			case 229:      m_Bit.LoadBitmap(IDB_BITMAP17);   break; 
			case 230:      m_Bit.LoadBitmap(IDB_BITMAP18);   break; 
			case 231:      m_Bit.LoadBitmap(IDB_BITMAP19);   break; 
			case 234:      m_Bit.LoadBitmap(IDB_BITMAP20);   break; 
			case 237:      m_Bit.LoadBitmap(IDB_BITMAP21);   break;
			case 244:      m_Bit.LoadBitmap(IDB_BITMAP22);   break;
			case 249:      m_Bit.LoadBitmap(IDB_BITMAP23);   break;
			case 251:      m_Bit.LoadBitmap(IDB_BITMAP24);   break;
			case 253:      m_Bit.LoadBitmap(IDB_BITMAP25);  break;
			case 254:      m_Bit.LoadBitmap(IDB_BITMAP26);  break;
			case 255:      m_Bit.LoadBitmap(IDB_BITMAP27);  break;
			}
		}
		m_temp = MainDC.SelectObject(&m_Bit);

		if (index > 200) // 정보
		{
			index -= 200;
			m_MemDC.BitBlt(507, 0, 507 + 243, 689, &MainDC, 0, 0, SRCCOPY);
		}
		else // 건물
		{
			if (index > 100)
			{
				index -= 100;
			}

			int tempx = 10;
			int tempy = 10;
			m_MemDC.BitBlt(pDoc->getPoint(index).x, pDoc->getPoint(index).y, tempx, tempy, &MainDC, 0, 0, SRCCOPY);
		}

		MainDC.SelectObject(m_temp);
	}
}


int CSejongNaviView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (b_socket)
	{
		AfxSocketInit();
		m_socCom.Create();
		m_socCom.Init(this->m_hWnd);
		m_socCom.Connect("localhost", 5000);

		char pTmp[256];
		memset(pTmp, '\0', 256);
		pTmp[0] = '0';

		m_socCom.Send(pTmp, 256);
		b_socket = FALSE;
	}
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetTimer(1, 100, NULL);

	return 0;
	
}


void CSejongNaviView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}


void CSejongNaviView::DrawAllPath()
{
	if (b_endSocket) {
		for (;;)
		{
			pDoc->setAnimation();
			//이동한 위치에 출력
			CPen pen1, pen2;
			CBrush brush1, brush2;
			pen1.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			brush1.CreateSolidBrush(RGB(255, 0, 0));
			m_MemDC.SelectObject(&pen1);
			m_MemDC.SelectObject(&brush1);
			m_MemDC.Ellipse((int)(pDoc->getLastPosX() - 5), (int)(pDoc->getLastPosY() - 5), (int)(pDoc->getLastPosX() + 5), (int)(pDoc->getLastPosY() + 5));

			pen2.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			brush2.CreateSolidBrush(RGB(255, 255, 255));
			m_MemDC.SelectObject(&pen2);
			m_MemDC.SelectObject(&brush2);
			m_MemDC.Ellipse(pDoc->getPoint(pDoc->getnStart()).x - 7, pDoc->getPoint(pDoc->getnStart()).y - 7, pDoc->getPoint(pDoc->getnStart()).x + 7, pDoc->getPoint(pDoc->getnStart()).y + 7);
			m_MemDC.Ellipse(pDoc->getPoint(pDoc->getnEnd()).x - 7, pDoc->getPoint(pDoc->getnEnd()).y - 7, pDoc->getPoint(pDoc->getnEnd()).x + 7, pDoc->getPoint(pDoc->getnEnd()).y + 7);
			Invalidate(0);
			BOOL b = pDoc->drawCircle();
			if (b == TRUE) break;
		}
	}
}

//오른쪽 버튼을 눌러 프로그램 종료
void CSejongNaviView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (IDYES == MessageBox(_T("종료하시겠습니까?"), _T("세종대 길안내 프로그램"), MB_YESNO | MB_ICONQUESTION))
	{
		PostQuitMessage(0);
	}
	CView::OnRButtonDown(nFlags, point);
}

LPARAM CSejongNaviView::OnReceive(UINT wParam, LPARAM lParam)
{
	char pTmp[256];
	//CString strTmp;

	memset(pTmp, '\0', 256);

	m_socCom.Receive(pTmp, 256);

	// 처음 열 갯수 받아올때 (첫 숫자가 0인 경우는 0뿐이 없음 010이라던가 존재하지 않음)
	if (pTmp[0] == '0')
	{
		m_rowNum = atoi(pTmp + 1);
		// 열갯수만큼 포인터 할당시키는함수 함수(int i)
		memset(pTmp, '\0', 256);
		pTmp[0] = '1';
		m_socCom.Send(pTmp, 256);
		pDoc->InitPoint(m_rowNum+1);
	}
	else if (m_rowNum + 1 > atoi(pTmp))
	{
		int n_id = atoi(pTmp);
		int nPosX = atoi(pTmp + 5);
		int nPosY = atoi(pTmp + 10);
		int nRectLeft = atoi(pTmp + 15);
		int nRectTop = atoi(pTmp + 20);
		int nRectRight = atoi(pTmp + 25);
		int nRectBottom = atoi(pTmp + 30);
		int nConnectNode1 = atoi(pTmp + 35);
		int nConnectNode2 = atoi(pTmp + 40);
		int nConnectNode3 = atoi(pTmp + 45);
		int nConnectNode4 = atoi(pTmp + 50);
		int nDistanceNode1 = atoi(pTmp + 55);
		int nDistanceNode2 = atoi(pTmp + 60);
		int nDistanceNode3 = atoi(pTmp + 65);
		int nDistanceNode4 = atoi(pTmp + 70);
		BOOL bBuilding = atoi(pTmp + 100);
		CString cBulidName = NULL;
		if (bBuilding == 1)
		{
			cBulidName = pTmp + 120;
		}
		// 함수 호출로 데이터 삽입
		pDoc->InitPointArray(n_id
			, nPosX
			, nPosY
			, nRectLeft
			, nRectTop
			, nRectRight
			, nRectBottom
			, nConnectNode1
			, nConnectNode2
			, nConnectNode3
			, nConnectNode4
			, nDistanceNode1
			, nDistanceNode2
			, nDistanceNode3
			, nDistanceNode4
			, bBuilding
			, cBulidName);

		memset(pTmp, '\0', 256);

		sprintf(pTmp, _T("%d"), n_id + 1);
		m_socCom.Send(pTmp, 256);
	}
	else
	{
		memset(pTmp, '\0', 256);
		sprintf(pTmp, _T("%d"), -1);
		m_socCom.Send(pTmp, 256);
		m_socCom.Close();
		b_endSocket = TRUE;
	}

	return TRUE;
}