#pragma once
#include "Shape.h"
#include<vector>
#include<list>
#include <algorithm>

#include <functional>
using namespace std;
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8
class EDGE
{
public:
	EDGE(int x1, double dx1, int ymax1){ xi = x1; dx = dx1; ymax = ymax1; }
	double xi;//边的下端点x坐标
	double dx;//直线斜率的倒数
	int ymax;//y值
	bool operator <(EDGE b)//重载排列关系
	{
		return xi<b.xi;
	}
	bool operator >(EDGE b)//重载排列关系
	{
		return xi>b.xi;
	}
	bool operator ==(EDGE b)//重载等于号
	{
		return xi==b.xi;
	}
	bool operator !=(EDGE b)//重载不等于号
	{
		return xi!=b.xi;
	}
};
class CLine :public CShape
{
public:
	CLine();
	CLine(const CPoint& pt1, const CPoint& pt2);
	~CLine();
	void StartPoint(const CPoint& pt){ m_ptStart = pt; }
	void EndPoint(const CPoint& pt){ m_ptEnd = pt; }
	void AddPoint(const CPoint& pt);
	CPoint  GetFirstVertex(){ if (m_Vertex.size() != 0) return m_Vertex[0]; }
	void Draw(CDC* pDC);
	void DrawPolygon(CDC* pDC);
	void InterBresenhamline(const CPoint& ptStart, const CPoint& ptEnd, CDC* pDC);
	void DDALine(const CPoint& ptStart, const CPoint& ptEnd, CDC* pDC);
	void MidPointLine(const CPoint& ptStart, const CPoint& ptEnd, CDC* pDC);
	void PolygonFill(CDC* pDC);
	void CS_LineClip(const CRect& clipRect);
private:
	CPoint  m_ptEnd;
	CPoint  m_ptStart;
	int     m_nWeight;
	std::vector<CPoint> m_Vertex;
};

