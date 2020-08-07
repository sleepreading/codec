#include "StdAfx.h"
#include "DirectDraw.h"
#pragma comment(linker,"/subsystem:windows")

CDirectDraw g_dd;
HWND g_hwnd; //���ھ��

LRESULT CALLBACK WinProc( HWND hWnd, UINT message, 
						 WPARAM wParam, LPARAM lParam )
{
    switch( message )
    {
	case WM_KEYDOWN://������Ϣ
		switch( wParam )
		{
		case VK_SPACE:
			g_dd.OutputMsg("hello world!");
			break;
		case VK_ESCAPE:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}
		break;
        case WM_DESTROY://�˳���Ϣ
            PostQuitMessage( 0 );
            break;
    }
	
    return DefWindowProc(hWnd, message, wParam, lParam);
}

BOOL InitWindow( HINSTANCE hInstance, int nCmdShow );

int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg; 
    
    if ( !InitWindow( hInstance, nCmdShow ) ) //��ʼ������
        return false;

	g_dd.InitDDraw(g_hwnd,DDSCL_NORMAL,800, 600, 8);

    //������Ϣѭ��
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
		DispatchMessage(&msg);
    }

    return msg.wParam;
}

//���ܣ����������ڡ�
static BOOL InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    WNDCLASS wc; //������ṹ
    //��䴰����ṹ
    wc.style = 0;
    wc.lpfnWndProc = WinProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( hInstance, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "dxHello";
	
    RegisterClass( &wc );//ע�ᴰ����
    g_hwnd = CreateWindowEx(0,"dxHello","",WS_POPUP,0, 0,
						GetSystemMetrics( SM_CXSCREEN ),GetSystemMetrics( SM_CYSCREEN ),
						NULL,NULL,hInstance,NULL );
    if( !g_hwnd ) return false;
	
    //��ʾ�����´���
    ShowWindow( g_hwnd, nCmdShow );
    UpdateWindow( g_hwnd );
	
    return true;
}

