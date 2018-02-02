
#include "stdafx.h"
#include "Line.h"

CLine::CLine()
{
	m_nWeight = 10;
	m_ptEnd = -1;
	m_ptStart = -1;
}
	
CLine::CLine(const CPoint& pt1, const CPoint& pt2)
{	
	m_ptEnd = pt2;
	m_ptStart = pt1;
	m_nWeight = 10;  
}

CLine::~CLine()
{
}
void CLine::Draw(CDC* pDC)
{
	
	m_nWeight = 10;
	switch (m_algorithmType)
	{
	case NONE:
		break;
	case DDA:
		DDALine(m_ptStart, m_ptEnd, pDC);
		break;
	case MID:
		MidPointLine(m_ptStart, m_ptEnd, pDC);
		break;
	case BRESENHAM:
		InterBresenhamline(m_ptStart, m_ptEnd, pDC);
		break;
	case ScanPolygon:
		DrawPolygon(pDC);
		break;
	default:
		break;
	}	
}
//增加多边形顶点
void CLine::AddPoint(const CPoint& pt)
{
	m_Vertex.push_back(pt);
}
//绘制多边形
void CLine::DrawPolygon(CDC* pDC)
{
	
	if (m_Vertex.size() != 0)//
	{
		for (size_t i = 0; i<m_Vertex.size()-1; ++i)
			InterBresenhamline(m_Vertex[i], m_Vertex[i + 1],pDC);
		if (m_ptEnd.x > 0 && m_ptEnd.y >0)
		{
			InterBresenhamline(m_Vertex[m_Vertex.size() - 1], m_ptEnd, pDC);
			//PolygonFill(pDC);
		}
		if (m_ptEnd.x == m_Vertex[0].x && m_ptEnd.y == m_Vertex[0].y)
		{
			InterBresenhamline(m_Vertex[m_Vertex.size() - 1], m_ptEnd, pDC);
			PolygonFill(pDC);
		}
			
	}
}
//多边形的扫描转换算法
void InsertNetListToAet(vector<EDGE> list1, vector<EDGE>& AET)
 {
	int Count1 = list1.size();
	int Count2 = AET.size();
	if (Count1 == 0) return;
	if (Count2 == 0)
    {
		AET = list1;
        return;
    }
     vector<EDGE> temp;
     int i = 0, j = 0;
	 while (i < Count1 && j < Count2)
     {
         if (list1[i] == AET[j])
         {
             i++;
             temp.push_back(AET[j]);
             j++;
             continue;
         }
         if (list1[i] < AET[j])
         {
			 temp.push_back(list1[i]);
             i++;
             continue;
         }
         if (list1[i] > AET[j])
         {
			 temp.push_back(AET[j]);
             j++;
             continue;
         }
     }
     while (i < Count1)
     {
		 temp.push_back(list1[i]);
         i++;
     }
     while (j < Count2)
     {
		 temp.push_back(AET[j]);
         j++;
     }
     AET = temp;
 }

void mysort(vector<EDGE>& net)
{
	for (int i = 1; i < net.size(); i++)
	{
		if (net[i - 1] > net[i])
		{
			EDGE temp = net[i];
			int j = i;
			while (j > 0 && net[j - 1] > temp)
			{
				net[j] = net[j - 1];
				j--;
			}
			net[j] = temp;
		}
	}
}
void CLine::PolygonFill(CDC* pDC)
{
	
	int ymax = 0, ymin = 0;//多边形y的最大值和最小值
	int i, j;
	int size = m_Vertex.size();
	for (i = 0; i<m_Vertex.size(); i++)
	{
		if (m_Vertex[i].y>ymax)
			ymax = m_Vertex[i].y;
	}
	ymin = ymax;
	for (i = 0; i<m_Vertex.size(); i++)
	{
		if (m_Vertex[i].y<ymin)
			ymin = m_Vertex[i].y;
	}
	vector<vector<EDGE>> NET(1024);//定义新边表
	vector<EDGE> AET;
	for (i = ymin; i < ymax; i++)
	{
		for (j = 0; j<m_Vertex.size(); j++)
		{
			if (m_Vertex[j].y == i)
			{
				if (m_Vertex[(j - 1 + size) % size].y>m_Vertex[j].y)
				{
					double dx = double(m_Vertex[(j - 1 + size) % size].x - m_Vertex[j].x) / double(m_Vertex[(j - 1 + size) % size].y - m_Vertex[j].y);
					EDGE nxet(m_Vertex[j].x, dx, m_Vertex[(j - 1 + size) % size].y);
					NET[i].push_back(nxet);
				}
				if (m_Vertex[(j + 1 + size) % size].y>m_Vertex[j].y)
				{
					double dx = double(m_Vertex[(j + 1 + size) % size].x - m_Vertex[j].x) / double(m_Vertex[(j + 1 + size) % size].y - m_Vertex[j].y);
					EDGE nxet(m_Vertex[j].x, dx, m_Vertex[(j + 1 + size) % size].y);
					NET[i].push_back(nxet);
				}
			}
		}
		InsertNetListToAet(NET[i], AET);

		for (j = 0; j < AET.size() - 1; j += 2)
		{
			for (int x = AET[j].xi; x <= AET[j + 1].xi; x++)
			{
				pDC->SetPixel(x, i, m_Color);
			}
		}
		for (auto it = AET.begin(); it != AET.end(); it++)   //for循环中不要it++
		{
			(*it).xi += (*it).dx;
		}
		for (auto it = AET.begin(); it != AET.end();)   //for循环中不要it++
		{
			if ((*it).ymax == i+1)
			{
				it=AET.erase(it);
			}
			else
			{
				it++;
			}
		}

		mysort(AET);
	}
}
//DDA算法
void CLine::DDALine(const CPoint& ptStart, const CPoint& ptEnd, CDC* pDC)
{
	int x1 = ptStart.x, y1 = ptStart.y, x2 = ptEnd.x, y2 = ptEnd.y;
	int dx = x2 - x1;
	int dy = y2 - y1;
	float x,y,k;
	int n,ux,uy;
	if (dx > 0)
		ux = 1;
	else
		ux = -1;
	if (dy > 0)
		uy = 1;
	else
		uy = -1;
	x = x1;
	y = y1;
	if (abs(dx) > abs(dy))
	{
		n = abs(dx);
		k = (float)dy / n;
		for (int i = 0; i <= n; i++)
		{
			pDC->SetPixel(x, int(y + 0.5), m_Color);
			y = y + k;
			x = x + ux;
		}
	}
	else
	{
		n = abs(dy);
		k = (float)dx / n;
		for (int i = 0; i <= n; i++)
		{
			pDC->SetPixel(int(x+0.5), y , m_Color);
			y = y + uy;
			x = x + k;
		}
	}
}
//中点画线算法
void CLine::MidPointLine(const CPoint& ptStart, const CPoint& ptEnd, CDC* pDC)
{

	int x1 = ptStart.x, y1 = ptStart.y, x2 = ptEnd.x, y2 = ptEnd.y;
	int a, b, d1, d2, d, x, y; 
	int ux, uy;
	if (x2<x1)
	{ 
		d = x1, x1 = x2, x2 = d; 
		d = y1, y1 = y2, y2 = d; 
	}
	int dx = x2 - x1;
	int dy = y2 - y1;

	a = y1 - y2, b = x2 - x1; 
	x = x1, y = y1;
	pDC->SetPixel(x, y, m_Color);
	if (abs(dx)>abs(dy))
	{
		if (dy>0)
		{
			d = 2 * a + b;
			d1 = 2 * a, d2 = 2 * (a + b);
			while (x<x2)
			{
				if (d <= 0)//中点在下方
				{
					x++, y++, d += d2;
				}
				else
				{
					x++, d += d1;
				}
				pDC->SetPixel(x, y, m_Color);
			}
		}
		else
		{
			d = 2 * a - b;
			d1 = 2 * a - 2 * b, d2 = 2 * a;
			while (x<x2)
			{
				if (d>0)
				{
					x++, y--, d += d1;
				}
				else
				{
					x++, d += d2;
				}
				pDC->SetPixel(x, y, m_Color);
			}
		}
	}
	else
	{
		if (dy > 0)
		{
			d = a + 2 * b;
			d1 = 2 * (a + b), d2 = 2 * b;
			while (y<y2)
			{
				if (d>0)
				{
					x++, y++, d += d1;
				}
				else
				{
					y++, d += d2;
				}
				pDC->SetPixel(x, y, m_Color);
			}
		}
		else
		{
			d = a - 2 * b; d1 = -2 * b, d2 = 2 * (a - b);
			while (y>y2)
			{
				if (d <= 0)
				{
					x++, y--, d += d2;
				}
				else
				{
					y--, d += d1;
				}
				pDC->SetPixel(x, y, m_Color);
			}
		}
	}

}
void CLine::InterBresenhamline(const CPoint& ptStart, const CPoint& ptEnd, CDC* pDC)
{
	int x1 = ptStart.x, y1 = ptStart.y, x2 = ptEnd.x, y2 = ptEnd.y;
	int i; 
	int ux, uy,e;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int x = x1; int y = y1;
	if (dx > 0)
		ux = 1;
	else
		ux = -1;
	if (dy > 0)
		uy = 1;
	else
		uy = -1;
	dx = abs(dx); dy = abs(dy);	
	if (dx > dy)
	{
		e = -dx;
		for (i = 0; i <= dx; i++)
		{
			pDC->SetPixel(x, y, m_Color);
			x += ux;
			e += 2*dy;
			if (e >= 0)
			{
				y +=uy;
				e = e - 2*dx;
			}
		}
	}		
	else
	{
		e = -dy;
		for (i = 0; i <= dy; i++)
		{
			pDC->SetPixel(x, y, m_Color);
			y += uy;
			e += 2*dx;
			if (e >= 0)
			{
				x += ux;
				e = e - 2*dy;
			}
		}
	}
}

//cohen-sutherland裁剪算法
int encode(float x, float y, int XL, int XR, int YB, int YT)
{
	int c = 0;
	if (x<XL)  c |= LEFT;
	if (x>XR)  c |= RIGHT;
	if (y<YB)  c |= BOTTOM;
	if (y>YT)  c |= TOP;
	return c;

}

void CLine::CS_LineClip(const CRect& clipRect)
{
	int XL = clipRect.left;
	int XR = clipRect.right;
	int YB = clipRect.top;
	int YT = clipRect.bottom;
	int code1, code2, code;
	float x = 0, y = 0;
	float x1 = m_ptStart.x, y1 = m_ptStart.y, x2 = m_ptEnd.x, y2 = m_ptEnd.y;
		
	code1 = encode(x1, y1, XL, XR, YB, YT);
	code2 = encode(x2, y2, XL, XR, YB, YT);
	while (code1 != 0 || code2 != 0)
	{
		if ((code1&code2) != 0) // 在同一 侧
			return;
		if (code1 != 0)
			code = code1;
		else
			code = code2;
		if ((LEFT&code) != 0)
		{
			x = XL;
			y = y1 + (y2 - y1)*(XL - x1) / (x2 - x1);
		}
		else if ((RIGHT&code) != 0)
		{
			x = XR;
			y = y1 + (y2 - y1)*(XR - x1) / (x2 - x1);
		}
		else if ((BOTTOM&code) != 0)
		{
			y = YB;
			x = x1 + (x2 - x1)*(YB - y1) / (y2 - y1);
		}
		else if ((TOP&code) != 0)
		{
			y = YT;
			x = x1 + (x2 - x1)*(YT - y1) / (y2 - y1);
		}
		if (code == code1)
		{
			x1 = x;
			y1 = y;
			code1 = encode(x1, y1, XL, XR, YB, YT);
		}
		else
		{
			x2 = x;
			y2 = y;
			code2 = encode(x1, y1, XL, XR, YB, YT);
		}
	}
	m_ptStart.x = x1;
	m_ptStart.y = y1;
	m_ptEnd.x = x2;
	m_ptEnd.y = y2;
}