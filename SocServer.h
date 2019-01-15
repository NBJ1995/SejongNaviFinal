#pragma once

// CSocServer 명령 대상

class CSocServer : public CSocket
{
public:
	CSocServer();
	virtual ~CSocServer();
	HWND m_hWnd;
	void Init(HWND hWnd);
	void OnAccept(int nErrorCode);
	CSocCom m_socCom;
	CSocCom* CSocServer::GetAcceptSocCom();
	virtual void OnConnect(int nErrorCode);
};

