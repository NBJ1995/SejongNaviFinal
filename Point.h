#pragma once

#if _MSC_VER > 1000
#endif // _MSC_VER > 1000

class Point
{
public:
	bool	m_bState;			// 경유 여부 판단 T/F
	int		m_nNext[4][2];		// 다음 지점에 대한 정보 최대4개의 연결지점 // 2개의정보 (0: 인덱스 /1: 거리)
	bool	bBuilding;			// 건물인지 여부
	int		x;			// 점의 위치
	int		y;			// 점의 위치
	CRect	rect;	    // 이벤트 영역 CRec 사각형에 관한 클래스 
	CString cBulidName;
public:
	Point();
	virtual ~Point();

};
