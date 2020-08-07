// DirectDraw.h: interface for the CDirectDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRECTDRAW_H__F5F4A7C5_F39F_4851_8BF8_A466EA99613F__INCLUDED_)
#define AFX_DIRECTDRAW_H__F5F4A7C5_F39F_4851_8BF8_A466EA99613F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <ddraw.h>
#pragma comment (lib, "ddraw.lib")


class CDirectDraw  
{
	IDirectDraw* m_lpDD; // DirectDraw对象
	IDirectDrawSurface* m_lpDDSPrimary; // DirectDraw主页面
	IDirectDrawSurface* m_lpDDSBack; // DirectDraw缓冲页面

	DWORD m_dwWidth;
	DWORD m_dwHeight;

public:
	CDirectDraw() : m_lpDD(NULL), m_lpDDSPrimary(NULL), m_dwWidth(0),m_dwHeight(0){};
	CDirectDraw(HWND hWnd,DWORD dwCooperativeLevel, DWORD dwWidth,DWORD dwHeight,DWORD dwBitsPerPixel) 
		: m_lpDD(NULL), m_lpDDSPrimary(NULL),m_dwWidth(0),m_dwHeight(0) { 
		if (InitDDraw(hWnd,dwCooperativeLevel, dwWidth,dwHeight,dwBitsPerPixel))
			throw ("InitDDraw error!");
	};
	virtual ~CDirectDraw() { FreeDDraw(); };

	void OutputMsg(const char* szMsg);
	bool InitDDraw(HWND hWnd,DWORD dwCooperativeLevel, DWORD dwWidth,DWORD dwHeight,DWORD dwBitsPerPixel);



private:
	void FreeDDraw(  ) {
		if( m_lpDD != NULL ) {	
			if( m_lpDDSPrimary != NULL ) {
				m_lpDDSPrimary->Release();
				m_lpDDSPrimary = NULL;
			}
			m_lpDD->Release();
			m_lpDD = NULL;
		}
	};

};

#endif // !defined(AFX_DIRECTDRAW_H__F5F4A7C5_F39F_4851_8BF8_A466EA99613F__INCLUDED_)
