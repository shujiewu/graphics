// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// GraphicsView.h : CGraphicsView ��Ľӿ�
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
protected: // �������л�����
	CGraphicsView();
	DECLARE_DYNCREATE(CGraphicsView)

// ����
public:
	CGraphicsDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CGraphicsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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
	CRect    m_ClipBox;  //ͼ�ε���Ӿ���
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

#ifndef _DEBUG  // GraphicsView.cpp �еĵ��԰汾
inline CGraphicsDoc* CGraphicsView::GetDocument() const
   { return reinterpret_cast<CGraphicsDoc*>(m_pDocument); }
#endif

