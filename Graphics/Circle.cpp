#include "stdafx.h"
#include "Circle.h"


CCircle::CCircle()
{
}


CCircle::~CCircle()
{
}
void CCircle::Draw(CDC* pDC)
{
	m_nWeight = 10;
	switch (m_algorithmType)
	{
	case NONE:
		break;
	case MidCircle:
		MidPointCircle(m_ptStart, m_ptEnd, pDC);
		break;
	case BresenhamCircle:
		InterBresenhamCircle(m_ptStart, m_ptEnd, pDC);
		break;
	case MidEllipse:
		MidPointEllipse(m_ptStart, m_ptEnd, pDC);
		break;
	default:
		break;
	}
}
//中点画圆法
void CCircle::CirclePointsMid(int x, int y, int xinc, int yinc, CDC* pDC)
{
	pDC->SetPixel(x+xinc, y+yinc, m_Color);
	pDC->SetPixel(y + xinc,x + yinc,  m_Color);
	pDC->SetPixel(-x + xinc, y + yinc, m_Color);
	pDC->SetPixel( y + xinc,-x + yinc, m_Color);
	pDC->SetPixel(x + xinc, -y + yinc, m_Color);
	pDC->SetPixel(-y + xinc,x + yinc,  m_Color);
	pDC->SetPixel(-x + xinc, -y + yinc, m_Color);
	pDC->SetPixel(-y + xinc,-x + yinc,  m_Color);
}
void CCircle::MidPointCircle(const CPoint& ptStart, const CPoint& ptEnd, CDC* pDC)
{
	int x1 = ptStart.x, y1 = ptStart.y, x2 = ptEnd.x, y2 = ptEnd.y;
	double r = sqrt(double((x2 - x1)*(x2 - x1) / 4 + (y2 - y1)*(y2 - y1) / 4));
	int xinc, yinc;

	xinc = (abs(x2 + x1)) / 2;
	yinc = (abs(y2 + y1)) / 2;

	int x, y;
	float d;
	x = 0; y = r; d = 1.25 - r;
	CirclePointsMid(x, y, xinc, yinc, pDC); //显示圆弧上的八个对称点
	while (x <= y)
	{
		if (d<0)   d += 2 * x + 3;
		else    { d += 2 * (x - y) + 5;  y--; }
		x++;
		CirclePointsMid(x, y, xinc, yinc, pDC);
	}
}

//中点画椭圆法
void CCircle::EllipsePointsMid(int x, int y, int xinc, int yinc, CDC* pDC)
{
	pDC->SetPixel(x + xinc, y + yinc, m_Color);
	pDC->SetPixel(-x + xinc, y + yinc, m_Color);
	pDC->SetPixel(x + xinc, -y + yinc, m_Color);
	pDC->SetPixel(-x + xinc, -y + yinc, m_Color);
}
void CCircle::MidPointEllipse(const CPoint& ptStart, const CPoint& ptEnd, CDC* pDC)
{
	int x1 = ptStart.x, y1 = ptStart.y, x2 = ptEnd.x, y2 = ptEnd.y;
	int a = abs(x2 - x1) / 2;
	int b = abs(y2 - y1) / 2;

	int xinc, yinc;

	xinc = (abs(x2 + x1)) / 2;
	yinc = (abs(y2 + y1)) / 2;

	double taa = a*a;
	double tbb = b * b;
	double d = tbb + taa * (-b + 0.25);
	int x = 0;
	int y = b;
	EllipsePointsMid(x, y, xinc, yinc, pDC);
	while (tbb * (x + 1) < taa * (y - 0.5))
	{
		if (d < 0)
		{
			d += tbb * (2 * x + 3);
		}
		else
		{
			d += (tbb * (2 * x + 3) + taa * (-2 * y + 2));
			y--;
		}
		x++;
		EllipsePointsMid(x, y, xinc, yinc, pDC);
	}
	d = tbb*(x + 0.5)*(x + 0.5) + taa*(y - 1)*(y - 1) - taa*tbb;
	while (y > 0)
	{
		if (d < 0)
		{
			d += tbb * (2 * x + 2) + taa * (-2 * y + 3);
			x++;
		}
		else
		{
			d += taa * (-2 * y + 3);
		}
		y--;
		EllipsePointsMid(x, y, xinc, yinc, pDC);
	}
}


//Bresenham画圆法
void CCircle::CirclePointsBresenham(int x, int y, int xinc, int yinc, CDC* pDC)
{
	pDC->SetPixel(x + xinc, y + yinc, m_Color);
	pDC->SetPixel(-x + xinc, y + yinc, m_Color);
	pDC->SetPixel(x + xinc, -y + yinc, m_Color);
	pDC->SetPixel(-x + xinc, -y + yinc, m_Color);
}
void CCircle::InterBresenhamCircle(const CPoint& ptStart, const CPoint& ptEnd, CDC* pDC)
{
	int x1 = ptStart.x, y1 = ptStart.y, x2 = ptEnd.x, y2 = ptEnd.y;
	double r = sqrt(double((x2 - x1)*(x2 - x1) / 4 + (y2 - y1)*(y2 - y1) / 4));
	int xinc, yinc;
	xinc = (abs(x2 + x1)) / 2;
	yinc = (abs(y2 + y1)) / 2;

	int x, y, delta, delta1, delta2, direction;
	x = 0; y = r;
	delta = 2 * (1 - r);  //△d的初始值
	int Limit = 0;
	while (y >= Limit)
	{
		CirclePointsBresenham(x, y, xinc, yinc, pDC);
		if (delta <0)
		{
			delta1 = 2 * (delta + y) - 1;
			//                  取H点
			if (delta1 <= 0)direction = 1;
			else direction = 2; //取D点
		}
		else if (delta>0)
		{
			delta2 = 2 * (delta - x) - 1;
			//                  取D点
			if (delta2 <0) direction = 2;
			else direction = 3; //取V点
		}
		else
			direction = 2;
		switch(direction)
		{
			case 1:   
				x++;
				delta += 2 * x + 1;
				break;
			case 2:   
				x++;
				y--;
				delta += 2 * (x - y + 1);
				break;
			case 3:   
				y--;
				delta += (-2 * y + 1);
				break;
		}/* switch */
	}/*while*/
}/*Bresenham_Circle*/