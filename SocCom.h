#pragma once

// CSocCom 명령 대상

class CSocCom : public CSocket
{
public:
	CSocCom();
	virtual ~CSocCom();
	void Init(HWND hWnd);
	void OnReceive(int nErrorCode);
	HWND m_hWnd;
};


