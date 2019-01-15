// SocCom.cpp: 구현 파일
//

#include "stdafx.h"
#include "testserver.h"
#include "SocCom.h"


// CSocCom

CSocCom::CSocCom()
{
}

CSocCom::~CSocCom()
{
}


// CSocCom 멤버 함수

void CSocCom::Init(HWND hWnd)
{
	m_hWnd = hWnd;
}

void CSocCom::OnReceive(int nErrorCode)
{
	SendMessage(m_hWnd, UM_RECEIVE, 0, 0);
	CSocket::OnReceive(nErrorCode);
}