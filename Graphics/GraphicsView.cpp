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

// GraphicsView.cpp : CGraphicsView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Graphics.h"
#include "MainFrm.h"
#endif

#include "GraphicsDoc.h"
#include "GraphicsView.h"
#include <typeinfo.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRect rect;
BOOL IsLButtonDown = FALSE;//鼠标左键是否按下
CPoint pt_start;//橡皮筋区域的启始点（鼠标左键按下时）
CPoint pt_end;//橡皮筋区域的终止点（鼠标左键松开时）
// CGraphicsView

IMPLEMENT_DYNCREATE(CGraphicsView, CView)

BEGIN_MESSAGE_MAP(CGraphicsView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_BUTTON_LINE, ID_BUTTON_POLYGON, &CGraphicsView::OnShapeType)
	ON_UPDATE_COMMAND_UI_RANGE(ID_BUTTON_LINE, ID_BUTTON_POLYGON, &CGraphicsView::OnUpdateShapeType)
	ON_COMMAND_RANGE(ID_LINE_DDA, ID_BUTTON_LineClip, &CGraphicsView::OnAlgorithmType)
	ON_UPDATE_COMMAND_UI_RANGE(ID_LINE_DDA, ID_BUTTON_LineClip, &CGraphicsView::OnUpdateAlgorithmType)
	ON_COMMAND(ID_BUTTON_COLOR, &CGraphicsView::OnButtonColor)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CGraphicsView 构造/析构

CGraphicsView::CGraphicsView()
{
	// TODO:  在此处添加构造代码
	m_shape = NULL;
	m_drawType = DRAW_NONE;
	m_Algorithm = NONE;
	m_rectTracker.m_nStyle = CRectTracker::resizeOutside | CRectTracker::dottedLine;
	m_rectTracker.m_rect.SetRect(0, 0, 0, 0);
}

CGraphicsView::~CGraphicsView()
{
}

BOOL CGraphicsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CGraphicsView 绘制

void CGraphicsView::OnDraw(CDC* pDC)
{
	CGraphicsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
	CRect rcClient;
	GetClientRect(&rcClient);
	if (m_rectTracker.m_rect.bottom != m_rectTracker.m_rect.top)
	{
		m_rectTracker.Draw(&m_memDC);//若选择了该区域，则显示边框以及8个调整点
		m_ClipBox = m_rectTracker.m_rect;
	}
	else
	{
		m_rectTracker.m_rect.SetRect(0, 0, 0, 0);
		m_ClipBox = m_rectTracker.m_rect;
	}

	CLine* pd = new CLine;
	int n = m_ptrArray.GetSize();
	for (int i = 0; i < n; i++)
	{
		if (typeid(*(CShape*)m_ptrArray.GetAt(i)).name() == typeid(*pd).name())
		{
			((CLine*)m_ptrArray.GetAt(i))->CS_LineClip(m_ClipBox);
		}
		((CShape*)m_ptrArray.GetAt(i))->Draw(&m_memDC);
	}
	if (m_shape)//&&
	{

		m_shape->Draw(&m_memDC);

	}
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(),
		&m_memDC, 0, 0, SRCCOPY);
}


void CGraphicsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGraphicsView 诊断

#ifdef _DEBUG
void CGraphicsView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphicsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphicsDoc* CGraphicsView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphicsDoc)));
	return (CGraphicsDoc*)m_pDocument;
}
#endif //_DEBUG


// CGraphicsView 消息处理程序
int CGraphicsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CDC* pDC = GetDC();
	m_memDC.CreateCompatibleDC(pDC);
	m_memDC.SelectObject(&m_Bmp);
	ReleaseDC(pDC);
	return 0;
	return 0;
}

void CGraphicsView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	// TODO:  在此处添加消息处理程序代码
	if (m_Bmp.GetSafeHandle())
		m_Bmp.DeleteObject();

	CDC* pDC = GetDC();
	m_Bmp.CreateCompatibleBitmap(pDC, cx, cy);
	m_memDC.SelectObject(&m_Bmp);
	ReleaseDC(pDC);
}

BOOL CGraphicsView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;
	GetClientRect(&rcClient);
	CBrush brush(RGB(255, 255, 255));
	m_memDC.FillRect(&rcClient, &brush);
	return TRUE; //CView::OnEraseBkgnd(pDC);
	return CView::OnEraseBkgnd(pDC);
}
void CGraphicsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CGraphicsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	COLORREF color = RGB(255, 0, 0);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame)
		color = pFrame->GetSelectColor();

	if (m_Algorithm == LineClip)
	{
		Invalidate(TRUE);
		if (m_rectTracker.HitTest(point) < 0)//外部
		{
			IsLButtonDown = TRUE;
			CRectTracker temp;
			temp.TrackRubberBand(this, point, TRUE);
			temp.m_rect.NormalizeRect();

			//鼠标（矩形选框）起始位置
			pt_start = point;
			//鼠标（矩形选框）结束位置
			GetCursorPos(&pt_end);
			this->SendMessage(WM_LBUTTONUP, NULL, NULL);
		}
		//否则重置选框大小或位置
		else
		{
			m_rectTracker.Track(this, point, TRUE);
			m_rectTracker.m_rect.NormalizeRect();
			Invalidate();
		}
		return;
	}

	if (m_drawType == DRAW_LINE)
	{
		//if (m_shape=NULL)
		m_shape = new CLine();
		((CLine*)m_shape)->SetSelect(true);
		m_shape->SetColor(color);
		m_shape->SetAlgorithm(m_Algorithm);
		((CLine*)m_shape)->StartPoint(point);
		((CLine*)m_shape)->EndPoint(point);
		return;
	}
	else if (m_drawType == DRAW_ROUND)
	{
		m_shape = new CCircle();
		((CCircle*)m_shape)->SetSelect(true);
		m_shape->SetColor(color);
		m_shape->SetAlgorithm(m_Algorithm);
		((CCircle*)m_shape)->StartPoint(point);
		((CCircle*)m_shape)->EndPoint(point);
		return;
	}
	else if (m_drawType == DRAW_ELLIPSE)
	{
		m_shape = new CCircle();
		((CCircle*)m_shape)->SetSelect(true);
		m_shape->SetColor(color);
		m_shape->SetAlgorithm(m_Algorithm);
		((CCircle*)m_shape)->StartPoint(point);
		((CCircle*)m_shape)->EndPoint(point);
		return;
	}
	else if (m_drawType == DRAW_POLYGON)
	{
		if (m_shape == NULL)
			m_shape = new CLine();
		((CLine*)m_shape)->SetSelect(true);
		m_shape->SetColor(color);
		m_shape->SetAlgorithm(m_Algorithm);
		((CLine*)m_shape)->StartPoint(point);
		((CLine*)m_shape)->AddPoint(point);
		return;
	}

	Invalidate();
	CView::OnLButtonDown(nFlags, point);
}

void CGraphicsView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if ((m_drawType == DRAW_LINE || m_drawType == DRAW_POLYGON) && m_shape) //|| 
	{
		if (((CLine*)m_shape)->GetSelect())
		{
			((CLine*)m_shape)->EndPoint(point);
			Invalidate();
		}
	}
	else if (m_drawType == DRAW_ROUND&&m_shape)
	{
		if (((CCircle*)m_shape)->GetSelect())
		{
			((CCircle*)m_shape)->EndPoint(point);
			Invalidate();
		}
	}
	else if (m_drawType == DRAW_ELLIPSE&&m_shape)
	{
		if (((CCircle*)m_shape)->GetSelect())
		{
			((CCircle*)m_shape)->EndPoint(point);
			Invalidate();
		}
	}

	CView::OnMouseMove(nFlags, point);
}

void CGraphicsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_Algorithm == LineClip)
	{
		if (IsLButtonDown == TRUE)
		{
			rect.right = pt_end.x;
			rect.bottom = pt_end.y;
			ScreenToClient(&rect);
			pt_end.x = rect.right;
			pt_end.y = rect.bottom;

			m_rectTracker.m_rect.SetRect(pt_start.x, pt_start.y, pt_end.x, pt_end.y);
			m_rectTracker.m_rect.NormalizeRect();
			if (m_rectTracker.m_rect.bottom == m_rectTracker.m_rect.top)
			{
				m_rectTracker.m_rect.SetRect(0, 0, 0, 0);
				m_rectTracker.m_rect.NormalizeRect();
			}
			Invalidate();
			IsLButtonDown = FALSE;
		}
		return;
	}
	if (m_drawType == DRAW_LINE)
	{
		if (((CLine*)m_shape)->GetSelect())
		{
			((CLine*)m_shape)->SetSelect(false);

			m_ptrArray.Add(m_shape);
			Invalidate();
			m_shape = NULL;
		}
	}
	else if (m_drawType == DRAW_ROUND)
	{
		if (((CCircle*)m_shape)->GetSelect())
		{
			((CCircle*)m_shape)->SetSelect(false);
			m_ptrArray.Add(m_shape);
			Invalidate();
			m_shape = NULL;
		}
	}
	else if (m_drawType == DRAW_ELLIPSE)
	{
		if (((CCircle*)m_shape)->GetSelect())
		{
			((CCircle*)m_shape)->SetSelect(false);
			m_ptrArray.Add(m_shape);
			Invalidate();
			m_shape = NULL;
		}
	}
	CView::OnLButtonUp(nFlags, point);
}

void CGraphicsView::OnShapeType(UINT id)
{
	m_drawType = (DRAW_TYPE)id;
}
void CGraphicsView::OnUpdateShapeType(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_drawType == pCmdUI->m_nID);
}
void CGraphicsView::OnAlgorithmType(UINT id)
{
	m_Algorithm = (Algorithm_Type)id;
}
void CGraphicsView::OnUpdateAlgorithmType(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Algorithm == pCmdUI->m_nID);
}

void CGraphicsView::OnButtonColor()
{
	// TODO:  在此添加命令处理程序代码
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (m_shape && pFrame)
	{
		m_shape->SetColor(pFrame->GetSelectColor());
		Invalidate();
	}
}
void CGraphicsView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_drawType == DRAW_POLYGON)
	{
		((CLine*)m_shape)->EndPoint(((CLine*)m_shape)->GetFirstVertex());
		if (((CLine*)m_shape)->GetSelect())
		{
			((CLine*)m_shape)->SetSelect(false);
			m_ptrArray.Add(m_shape);
			Invalidate();
			m_shape = NULL;
		}
	}
	CView::OnLButtonDblClk(nFlags, point);
}

BOOL CGraphicsView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (pWnd == this && m_rectTracker.SetCursor(this, nHitTest))
		return TRUE;
	else
		return CView::OnSetCursor(pWnd, nHitTest, message);
}
