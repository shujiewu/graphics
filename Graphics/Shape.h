#pragma once
#include"resource.h"
enum Algorithm_Type
{
	NONE,
	DDA = ID_LINE_DDA,
	MID = ID_LINE_MID,
	BRESENHAM = ID_LINE_BRE,
	MidCircle = ID_Circle_Mid,
	BresenhamCircle = ID_Circle_Bresenham,
	MidEllipse = ID_Ellipse_Mid,
	ScanPolygon = ID_Polygon_Scan,
	LineClip = ID_BUTTON_LineClip
};
class CShape
{
public:
	CShape(){ m_Color = RGB(255, 0, 0);}
	bool GetSelect(){ return m_bSelect; }
	void SetSelect(bool bSelect){ m_bSelect = bSelect; }
	void SetColor(COLORREF color){ m_Color = color; };
	void SetAlgorithm(Algorithm_Type type){ m_algorithmType = type; }
	Algorithm_Type GetAlgorithm(){ return m_algorithmType; }
	virtual void Draw(CDC* pDC){};
	~CShape();
protected:
	bool     m_bSelect;     //图形被选择(活动)状态
	COLORREF m_Color;       //图形颜色
	Algorithm_Type m_algorithmType;
};

