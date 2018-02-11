﻿////////////////////////////////////////////////////////////////////////////////
#pragma once
////////////////////////////////////////////////////////////////////////////////

#include <view\TrabeImageCtrl.h>

////////////////////////////////////////////////////////////////////////////////

class MainWindow : public CWindowImpl<MainWindow, CWindow, CFrameWinTraits>
{
public:
	DECLARE_WND_CLASS(_T("Trabe Window Class"))

	//--------------------------------------------------------------------------
	enum { IDC_BTN_LOAD = 10, IDC_TEXT_PIXEL, IDC_PIC };

	CButton         m_btnLoad;
	TrabeImageCtrl  m_imageCtrl;
	CStatic         m_txtPixel;
	//--------------------------------------------------------------------------

public:
//message handler
	BEGIN_MSG_MAP(MainWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
//------------------------------------------------------------------------------
		COMMAND_HANDLER(IDC_BTN_LOAD, BN_CLICKED, OnBtnLoadClicked)
		NOTIFY_HANDLER(IDC_PIC, ICN_PIXEL, OnImageCtrlPixel)
//------------------------------------------------------------------------------
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//----------------------------------------------------------------------
		m_btnLoad.Create(m_hWnd, rcDefault, _T("Load"),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_BTN_LOAD);
		m_txtPixel.Create(m_hWnd, rcDefault, _T(""),
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_TEXT_PIXEL);
		m_imageCtrl.Create(m_hWnd, rcDefault, NULL,
						WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0,
						IDC_PIC);
		m_imageCtrl.UpdateScroll();
		//----------------------------------------------------------------------
		return 0;
	}
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 1;
	}
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		::PostQuitMessage(0);
		return 0;
	}
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CDCHandle dc((HDC)wParam);
		RECT rcClient;
		GetClientRect(&rcClient);
		CBrush bsh;
		bsh.CreateSolidBrush(RGB(64, 224, 225));
		dc.FillRect(&rcClient, bsh);
		return 1;
	}
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//----------------------------------------------------------------------
		if( wParam != SIZE_MINIMIZED ) {
			int w = LOWORD(lParam);
			int h = HIWORD(lParam);
			int x = 10;
			int y = 10;
			m_btnLoad.SetWindowPos(NULL, x, y, 60, 40, SWP_NOACTIVATE | SWP_NOZORDER);
			y += (40 + 10);
			m_txtPixel.SetWindowPos(NULL, x, y, 60, 90, SWP_NOACTIVATE | SWP_NOZORDER);
			x += (60 + 10);
			m_imageCtrl.SetWindowPos(NULL, x, y, w - x - 10, h - y - 10, SWP_NOACTIVATE | SWP_NOZORDER);
			m_imageCtrl.UpdateScroll();
		}
		//----------------------------------------------------------------------
		bHandled = FALSE;
		return 1;
	}
	//--------------------------------------------------------------------------
	LRESULT OnBtnLoadClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CFileDialog dlg(TRUE);
		if( dlg.DoModal() == IDOK ) {//弹出对话框
			CWaitCursor wac;
			CImage& image = m_imageCtrl.GetImage();
			image.Destroy();
			m_imageCtrl.UpdateScroll();
			if( FAILED(image.Load(dlg.m_szFileName)) ) {
				AtlMessageBox(NULL, _T("error load image"), _T("error"), MB_OK);
				return 0;
			}
			m_imageCtrl.UpdateScroll();
		}
		return 0;
	}
	LRESULT OnImageCtrlPixel(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled)
	{
		NMIMAGEPIXEL* pnm = (NMIMAGEPIXEL*)pNMHDR;
		if( pnm->rgb == CLR_INVALID ) {
			m_txtPixel.SetWindowText(_T(""));
		}
		else {
			CString str;
			str.Format(_T("X: %d\r\nY: %d\r\nR: %u\r\nG: %u\r\nB: %u"),
				pnm->x, pnm->y,
				(UINT)(GetRValue(pnm->rgb)), (UINT)(GetGValue(pnm->rgb)), (UINT)(GetBValue(pnm->rgb))
				);
			m_txtPixel.SetWindowText(str);
		}
		return 0;
	}
	//--------------------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////
