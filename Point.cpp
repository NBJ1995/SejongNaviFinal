#include "stdafx.h"
#include "Point.h"
#include "SejongNavi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

Point::Point()
{
	m_bState = false;
	bBuilding = false;
	rect = 0, 0, 0, 0;
	cBulidName = "                        ";
	//���۽� ��� ���� ��������� �ʱ�ȭ
}

Point::~Point()
{
}
