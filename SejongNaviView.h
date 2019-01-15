
// SejongNaviView.h: CSejongNaviView 클래스의 인터페이스
//

#pragma once

#include "Point.h"
#include "math.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _max 10000


class CSejongNaviView : public CView
{
protected: // serialization에서만 만들어집니다.
	CSejongNaviView() noexcept;
	DECLARE_DYNCREATE(CSejongNaviView)

	// 특성입니다.
public:
	CSejongNaviDoc* GetDocument() const;
	int nState;			// 프로그램 상태(0:선택 / 1:애니 / 2:결과)
	CSocCom m_socCom;

	int m_rowNum; // 받아오는 줄 갯수
	BOOL b_socket;
	BOOL b_endSocket;

	CDC m_MemDC;
	//프린터 출력에 관계된 맴버 함수 포함
	CBitmap m_MemBitmap;
	//비트맵 관련 멤버함수 포함



	// 작업입니다.
public:

	// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	CSejongNaviDoc* pDoc;//document를 사용하기 위한 객체

	// 구현입니다.
public:
	void DrawAllPath();
	void DrawTexts(int x, int y, int index);
	void DrawBack();
	void Draw(int index);

	virtual ~CSejongNaviView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 생성된 메시지 맵 함수
protected:
	// AFX_MSG(CSejong_NaviView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
	afx_msg LPARAM OnReceive(UINT wParam, LPARAM lParam);
};

#ifndef _DEBUG  // Sejong_NaviView.cpp의 디버그 버전
inline CSejongNaviDoc* CSejongNaviView::GetDocument() const
{
	return reinterpret_cast<CSejongNaviDoc*>(m_pDocument);
}
#endif
