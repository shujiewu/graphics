// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// GraphicsView.h : CGraphicsView 类的接口
//
#pragma once
#include"Shape.h"
#include"Line.h"
#include"Circle.h"
enum DRAW_TYPE
{
	DRAW_NONE,
	DRAW_LINE = ID_BUTTON_LINE,
	DRAW_ROUND = ID_BUTTON_ROUND,
	DRAW_ELLIPSE = ID_BUTTON_ELLIPSE,
	DRAW_POLYGON = ID_BUTTON_POLYGON
};


class CGraphicsView : public CView
{
protected: // 仅从序列化创建
	CGraphicsView();
	DECLARE_DYNCREATE(CGraphicsView)

// 特性
public:
	CGraphicsDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CGraphicsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	
	afx_msg void OnShapeType(UINT id);
	afx_msg void OnUpdateShapeType(CCmdUI *pCmdUI);

	afx_msg void OnAlgorithmType(UINT id);
	afx_msg void OnUpdateAlgorithmType(CCmdUI *pCmdUI);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
	CDC m_memDC;
	CBitmap m_Bmp;
	CShape *m_shape;
	CPtrArray m_ptrArray;
	DRAW_TYPE m_drawType;
	Algorithm_Type m_Algorithm;
	CRect    m_ClipBox;  //图形的外接矩形
	CRectTracker m_tracker;
	CRectTracker m_rectTracker;
	CRectTracker m_RectTracker;
	BOOL m_IsChosen;
public:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
public:
	afx_msg void OnButtonColor();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

#ifndef _DEBUG  // GraphicsView.cpp 中的调试版本
inline CGraphicsDoc* CGraphicsView::GetDocument() const
   { return reinterpret_cast<CGraphicsDoc*>(m_pDocument); }
#endif

