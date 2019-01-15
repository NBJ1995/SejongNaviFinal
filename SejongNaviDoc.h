

// Sejong_NaviDoc.h: CSejongNaviDoc 클래스의 인터페이스
//

#include"Point.h"

#define _max 10000
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSejongNaviDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CSejongNaviDoc() noexcept;
	DECLARE_DYNCREATE(CSejongNaviDoc)
	int nStart;
	int nEnd;

	int min;
	int minIndex;

	int* nDist;		//모든 디스턴스 맥스화
	int* nPrev;

	Point *p;  // 최단거리 노드 초기화 포인터
		// 타이머에서 애니메이션출력시 단기목표와 현재위치
	int LastStart;		// 단기시작점 인덱스
	int LastEnd;		// 단기목표점 인덱스

	int nLast;			// 프로그램에서 하나

	double LastPosX;    // 현재 점의 위치
	double LastPosY;	// 현재 점의 위치

	int nSort[100];		// Proc에서 거꾸로 정렬된 경로배열을 논리적으로 맞는 순서(역순)로 재배열
	int nSortCount;		// nSort의 top을 관리

	//출력 위한 변수
	bool nFlag;			// mx,my를 초기화하는 변수
	double mx, my;		// 이동크기벡터
	bool bTemp;

	int n_rowNum;
	// 특성입니다.
public:

	// 작업입니다.
public:
	int getnStart() { return nStart; }
	void setnStart(int i) { nStart = i; }
	int getnEnd() { return nEnd; }
	void setnEnd(int i) { nEnd = i; }
	int getMin() { return min; }
	int getMinIndex() { return minIndex; }
	int getnDist(int i) { return nDist[i]; }
	int getnPrev(int i) { return nPrev[i]; }
	int getLastStart() { return LastStart; }
	void setLastStart(int i) { LastStart = i; }
	int getLastEnd() { return LastEnd; }
	void setLastEnd(int i) { LastEnd = i; }
	double getLastPosX() { return LastPosX; }
	void setLastPosX(double d) { LastPosX = d; }
	double getLastPosY() { return LastPosY; }
	void setLastPosY(double d) { LastPosY = d; }
	int getnSort(int i) { return nSort[i]; }
	int getnSortCount() { return nSortCount; }
	void setnSortCount(int i) { nSortCount = i; }
	bool getnFlag() { return nFlag; }
	void setnFlag(bool b) { nFlag = b; }
	double getMx() { return mx; }
	void setMx(double d) { mx = d; }
	double getMy() { return my; }
	void setMy(double d) { my = d; }
	bool getbTemp() { return bTemp; }
	void setbTemp(bool b) { bTemp = b; }
	int getnLast() { return nLast; }
	void setnLast(int i) { nLast = i; }
	Point* getPoint() { return p; }
	Point getPoint(int i) { return p[i]; }
	// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	void InitPoint(int n);
	void InitPointArray(int n_id
	,int nPosX
	,int nPosY
	,int nRectLeft
	,int nRectTop
	,int nRectRight
	,int nRectBottom
	,int nConnectNode1
	,int nConnectNode2 
	,int nConnectNode3
	,int nConnectNode4
	,int nDistanceNode1
	,int nDistanceNode2
	,int nDistanceNode3
	,int nDistanceNode4 
	,BOOL bBuilding
	,CString cBulidName);
	int RefreshMove(CPoint point);
	int RefreshClick(CPoint point);
	void Proc();
	bool isInRectangle(CPoint point, int i);
	void setResult();
	void TimerInitialization();
	void setAnimation();
	BOOL drawCircle();
		// 구현입니다.
public:
	virtual ~CSejongNaviDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

};
