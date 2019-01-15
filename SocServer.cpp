// SocServer.cpp: 구현 파일
//

#include "stdafx.h"
#include "testserver.h"
#include "SocServer.h"


// CSocServer

CSocServer::CSocServer()
{
}

CSocServer::~CSocServer()
{
}


// CSocServer 멤버 함수

void CSocServer::Init(HWND hWnd)
{
	m_hWnd = hWnd;
}

void CSocServer::OnAccept(int nErrorCode)
{
	Accept(m_socCom);
	SendMessage(m_hWnd, UM_ACCEPT, 0, 0);
	CSocket::OnAccept(nErrorCode);
}

CSocCom* CSocServer::GetAcceptSocCom()
{
	return &m_socCom;
}

void CSocServer::OnConnect(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CSocket::OnConnect(nErrorCode);
}
