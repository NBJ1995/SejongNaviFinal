#pragma once

#if _MSC_VER > 1000
#endif // _MSC_VER > 1000

class Point
{
public:
	bool	m_bState;			// ���� ���� �Ǵ� T/F
	int		m_nNext[4][2];		// ���� ������ ���� ���� �ִ�4���� �������� // 2�������� (0: �ε��� /1: �Ÿ�)
	bool	bBuilding;			// �ǹ����� ����
	int		x;			// ���� ��ġ
	int		y;			// ���� ��ġ
	CRect	rect;	    // �̺�Ʈ ���� CRec �簢���� ���� Ŭ���� 
	CString cBulidName;
public:
	Point();
	virtual ~Point();

};
