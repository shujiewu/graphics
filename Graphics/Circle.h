#pragma once
#include "Shape.h"
class CCircle :
	public CShape
{
public:
	CCircle();
	~CCircle();
	void StartPoint(const CPoint& pt){ m_ptStart = pt; }
	void EndPoint(const CPoint& pt){ m_ptEnd = pt; }
	virtual void Draw(CDC* pDC);
	void MidPointCircle(const CPoint& ptStart, const CPoint& ptEnd, CDC* pDC);
	void MidPointEllipse(const CPoint& ptStart, const CPoint& ptEnd, CDC* pDC);
	void InterBresenhamCircle(const CPoint& ptStart, const CPoint& ptEnd, CDC* pDC);
	void CirclePointsMid(int x, int y, int xinc, int yinc, CDC* pDC);
	void EllipsePointsMid(int x, int y, int xinc, int yinc, CDC* pDC);
	void CirclePointsBresenham(int x, int y, int xinc, int yinc, CDC* pDC);
private:
	CPoint  m_ptEnd;
	CPoint  m_ptStart;
	int     m_nWeight;
};

