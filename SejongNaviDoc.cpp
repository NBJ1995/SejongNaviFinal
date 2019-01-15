
// Sejong_NaviDoc.cpp: CSejongNaviDoc 클래스의 구현
//

#include "stdafx.h"
#include "SejongNavi.h"

#include "SejongNaviDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CSejongNaviDoc

IMPLEMENT_DYNCREATE(CSejongNaviDoc, CDocument)

BEGIN_MESSAGE_MAP(CSejongNaviDoc, CDocument)
END_MESSAGE_MAP()


// CSejongNaviDoc 생성/소멸

CSejongNaviDoc::CSejongNaviDoc() noexcept
{
	n_rowNum = 0;

	nStart = 0;
	nEnd = 0;
	nLast = 0;

	LastStart = 0;      // 최근 시작점 초기화
	LastEnd = 0;      // 최근 목표점 초기화
	LastPosX = 0;      // 현재 위치 초기화
	LastPosY = 0;      // 현재 위치 초기화

	nFlag = 1;
	bTemp = 1;
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
}

CSejongNaviDoc::~CSejongNaviDoc()
{
	delete[] p;
}

BOOL CSejongNaviDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}


// CSejongNaviDoc serialization

void CSejongNaviDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

// CSejongNaviDoc 진단

#ifdef _DEBUG
void CSejongNaviDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSejongNaviDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSejongNaviDoc 명령
void CSejongNaviDoc::InitPointArray(int n_id
	, int nPosX
	, int nPosY
	, int nRectLeft
	, int nRectTop
	, int nRectRight
	, int nRectBottom
	, int nConnectNode1
	, int nConnectNode2
	, int nConnectNode3
	, int nConnectNode4
	, int nDistanceNode1
	, int nDistanceNode2
	, int nDistanceNode3
	, int nDistanceNode4
	, BOOL bBuilding
	, CString cBulidName)
{
	p[n_id].x = nPosX;
	p[n_id].y = nPosY;
	p[n_id].rect.left = nRectLeft;
	p[n_id].rect.top = nRectTop;
	p[n_id].rect.right = nRectRight;
	p[n_id].rect.bottom = nRectBottom;
	p[n_id].m_nNext[0][0] = nConnectNode1;
	p[n_id].m_nNext[1][0] = nConnectNode2;
	p[n_id].m_nNext[2][0] = nConnectNode3;
	p[n_id].m_nNext[3][0] = nConnectNode4;
	p[n_id].m_nNext[0][1] = nDistanceNode1;
	p[n_id].m_nNext[1][1] = nDistanceNode2;
	p[n_id].m_nNext[2][1] = nDistanceNode3;
	p[n_id].m_nNext[3][1] = nDistanceNode4;
	p[n_id].bBuilding = bBuilding;
	if (p[n_id].bBuilding)
		p[n_id].cBulidName = cBulidName;
}

void CSejongNaviDoc::InitPoint(int n)
{
	p = new Point[n];
	nDist = new int[n];
	nPrev = new int[n];
	n_rowNum = n;
	p[0].cBulidName = "                        ";
}

int CSejongNaviDoc::RefreshMove(CPoint point)
{
	int i;
	for (i = 1; i <= n_rowNum; i++) {      // 모든
		if (p[i].bBuilding == 1 && isInRectangle(point,i) && nLast != i && i != nStart && i != nEnd)
		{ // 최근 건물과 같지 않을경우 return
			return i;					
		}
	}
}

int CSejongNaviDoc::RefreshClick(CPoint point) {

	for (int i = 1; i <= n_rowNum; i++)
	{
		if (p[i].bBuilding == 1 && isInRectangle(point, i))// 해당 영역일 경우
		{
			if (nStart == 0)  // 시작점선택
			{
				nStart = i;
				return 0;
			}
			else if (nStart == i)   // 취소할경우
			{
				nStart = 0; // 시작점 취소
				return 1;

			}
			else  // 종료점선택
			{
				nEnd = i;                        // 종료점 설정
				Proc();							// [클릭,정보]출력
				return 2;// 애니
			}
		}
	}
}

void CSejongNaviDoc::Proc()
{
	for (int j = 1; j < n_rowNum; j++)
	{
		p[j].m_bState = false; //방문하지 않은 상태로 전체 노드들 초기화
		nPrev[j] = 0; // 소스에서 최적경로 이전 노드
		nDist[j] = _max; // nStart -> j 까지 계산된 최단 거리, 알려지지 않은 거리는 MAX 로 초기화
	}

	nDist[nStart] = 0;//출발지의 거리는 0

	for (int i = 0; i < n_rowNum; i++)
	{
		min = _max;// 최소 거리를 갖는 노드를 탐색
		for (int j = 1; j < n_rowNum; j++)
		{
			if (nDist[j] < min  &&  p[j].m_bState == false)
			{
				minIndex = j;
				min = nDist[j];
			}
		}
		p[minIndex].m_bState = true;

		if (min == _max) break; //이웃노드 탐색 및 거리저장

		for (int j = 1; j < n_rowNum; j++)
		{
			for (int z = 0; z < 4; z++)
			{
				if (p[j].m_nNext[z][0] == minIndex)
				{
					if (nDist[j] > nDist[minIndex] + p[j].m_nNext[z][1]) // 더 빠른 경로를 발견할 경우
					{
						nDist[j] = nDist[minIndex] + p[j].m_nNext[z][1]; // 최소거리값 업데이트
						nPrev[j] = minIndex; //최적경로에 추가
					}
					break;
				}
			}
		}
	}

	// 최단 경로 추적 
	int path[100], path_cnt = 0;
	int i;
	minIndex = nEnd;
	while (1)
	{
		path[path_cnt++] = minIndex;
		if (minIndex == nStart)break;
		minIndex = nPrev[minIndex];
	}

	// 최단 경로 정렬
	nSortCount = 0;
	for (i = path_cnt - 1; i >= 1; i--)
	{
		nSort[nSortCount++] = path[i];
	}
	nSort[nSortCount++] = path[i];// 여기까지 nSort에는 경로가 순서대로 들어가고, nSortCount는 경로의 개수가 들어간다.
}

bool CSejongNaviDoc::isInRectangle(CPoint point,int i) {
	if (p[i].rect.left <= point.x   && point.x <= p[i].rect.right + p[i].rect.left
		&&   p[i].rect.top <= point.y   && point.y <= p[i].rect.bottom + p[i].rect.top) return TRUE;
	return FALSE;
}

void CSejongNaviDoc::TimerInitialization() {
	nSortCount = 1;                    // 최단경로중 다음 목표점을 가리키는 변수
	LastStart = nStart;                // 최근 시작점 초기화
	LastEnd = nSort[nSortCount++];     // 최근 목표 초기화
	LastPosX = (float)p[LastStart].x;  // 현재 위치 초기화
	LastPosY = (float)p[LastStart].y;  // 현재 위치 초기화
}

void CSejongNaviDoc::setResult() {

	nStart = 0;
	nEnd = 0;
	nLast = 0;

	LastStart = 0;      // 최근 시작점 초기화
	LastEnd = 0;      // 최근 목표점 초기화
	LastPosX = 0;      // 현재 위치 초기화
	LastPosY = 0;      // 현재 위치 초기화

	for (int d = 0; d < n_rowNum; d++)
	{
		nDist[d] = 0;      //모든 거리 맥스화
		nPrev[d] = 0;
		nSort[d] = 0;
	}
	nSortCount = 0;      // nSort의 top을 관리
	mx = 0;
	my = 0;
}

void CSejongNaviDoc::setAnimation() {

	if (nFlag) {
		nFlag = 0;

		mx = p[LastEnd].x - LastPosX;
		my = p[LastEnd].y - LastPosY;

		for (;;)
		{
			mx = mx / 2;
			my = my / 2;

			if (mx*mx + my * my <= 1)
			{
				if (mx*mx + my * my >= 0.5)   break;
				else
				{
					mx = mx * 3 / 2;
					my = my * 3 / 2;
					break;
				}

			}
		}
	}
	// x,y 변경
	LastPosX += mx;
	LastPosY += my;
}

BOOL CSejongNaviDoc::drawCircle() {

	if (abs((int)(LastPosX - p[LastEnd].x)) <= 2 && abs((int)(LastPosY - p[LastEnd].y)) <= 2)
	{
		LastStart = LastEnd;            // 최근 시작점 초기화
		LastEnd = nSort[nSortCount++];       // 최근 목표 초기화
		LastPosX = (float)p[LastStart].x;   // 현재 위치 초기화
		LastPosY = (float)p[LastStart].y;   // 현재 위치 초기화
		nFlag = 1;

		// 최종 목적지 도달시
		if (LastStart == nEnd)
		{
			return TRUE;
		}
	}
	else return FALSE;
}