// DirectDraw.cpp: implementation of the CDirectDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirectDraw.h"

bool CDirectDraw::InitDDraw( HWND hWnd,DWORD dwCooperativeLevel, DWORD dwWidth,DWORD dwHeight,DWORD dwBitsPerPixel )
{
	FreeDDraw();
	m_dwWidth = dwWidth;
	m_dwHeight= dwHeight;
    if ( DirectDrawCreate( NULL, &m_lpDD, NULL ) != DD_OK ) //创建DirectCraw对象
		return false;
	
	// 窗口模式不可以翻页
    if ( m_lpDD->SetCooperativeLevel(hWnd,dwCooperativeLevel) != DD_OK)
	{
		m_lpDD->Release();
        return false;
	}
	
	// 设置显示模式
    if ( m_lpDD->SetDisplayMode( m_dwWidth, m_dwHeight, dwBitsPerPixel ) != DD_OK) 
	{
		m_lpDD->Release();
		return false;
	}
	
    // 创建主页面对象
	DDSURFACEDESC ddsd; //页面描述
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
//	ddsd.dwBackBufferCount = 1;

    if ( m_lpDD->CreateSurface( &ddsd, &m_lpDDSPrimary, NULL ) != DD_OK)
	{
		m_lpDD->Release();
		return false;
	}
/*
	// 取得缓冲页面
	DDSCAPS ddscaps;
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	if (m_lpDDSPrimary->GetAttachedSurface(&ddscaps,&m_lpDDSBack) != DD_OK)
	{
		m_lpDDSPrimary->Release();
		m_lpDD->Release();
		return false;
	}
*/	
    return true;
}

void CDirectDraw::OutputMsg(const char* szMsg)
{
	if (!m_lpDDSPrimary || !szMsg) return;

    HDC hdc;
    if ( m_lpDDSPrimary->GetDC(&hdc) != DD_OK) return;

    SetBkColor( hdc, RGB( 0, 0, 255 ) );
    SetTextColor( hdc, RGB( 255, 255, 0 ) );
    TextOut( hdc, m_dwWidth/2, m_dwHeight/2, szMsg, lstrlen(szMsg));

	HPEN pen = CreatePen(PS_SOLID,8,RGB(255,0,0));
	HBRUSH brush = ::CreateHatchBrush(HS_DIAGCROSS,RGB(255,216,156));
	SelectObject(hdc,pen);
	SelectObject(hdc,brush);

	::Rectangle(hdc,0,0,m_dwWidth/2, m_dwHeight/2);

	DeleteObject(pen);
	DeleteObject(brush);
}
