// DlgCap.cpp : implementation file
//

#include "stdafx.h"
#include "Cap.h"
#include "DlgCap.h"
#include "DlgProxy.h"
#include <MATH.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
static ColorSpaceConversions g_GenConv ;
CBAccept_Param_s	g_CBAcceptParam;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CButton	m_11;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDOK, m_11);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCap dialog

IMPLEMENT_DYNAMIC(CDlgCap, CBCGPDialog);

CDlgCap::CDlgCap(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgCap::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCap)
	m_checkCapCursor = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_wPortSrv = 1986;
	m_bPlaying = FALSE;
	m_hDrawDIB = NULL;
	m_pWndVideoShow = NULL;
	m_iSizeVFrame = -1;
	m_pAutoProxy = NULL;
	m_nFrame = 0;
	m_bIsPause = FALSE;
	m_bRecing = FALSE;
	m_bWinEnc = FALSE;
	m_bFirstTime = TRUE;
	m_bFirstTime2= FALSE;
	memset(&m_aviStrmInfo,0,0);
	m_pAviFile = NULL;
	m_pAviStrm = NULL;
	m_pAviStrmComp = NULL;
	m_pCompOptions[0] = &m_CompressOption;

	m_PlayInfo.pY = (CHAR*)calloc(1,1366*768*4);	// make sure it big enough
	m_PlayInfo.pU = (CHAR*)calloc(1,1366*768);
	m_PlayInfo.pV = (CHAR*)calloc(1,1366*768);

	m_EncData.pDataEnc = (uint8_t *)calloc(1,1366*768*4);

	for (int i=0;i<4;i++)
	{
		m_PlayInfo.pRGB[i] = (unsigned char*)calloc(1,1366*768*4);
	}

	m_pBufHeader = (char*)calloc(1,32);

	m_strAviPathName = "D:\\test.avi";
	m_pBufAFrame = (CHAR*)calloc(1,1024*768*10);
//	m_ffScale.SetAttribute(SWS_PF_ABGR,SWS_PF_YUV420P);
//	m_flYUV.Open("D:\\test.yuv",CFile::modeWrite | CFile::modeCreate);
//	m_flRGB.Open("D:\\test.rgb",CFile::modeWrite | CFile::modeCreate);
}

CDlgCap::~CDlgCap()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
	if (m_pBufAFrame)
		free(m_pBufAFrame);
	if (m_pBufHeader)
	{
		free(m_pBufHeader);
	}
}

void CDlgCap::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCap)
	DDX_Control(pDX, IDC_editBaud, m_editBaud);
	DDX_Control(pDX, IDC_checkCapCursor, m_ctlCheckCapCursor);
	DDX_Control(pDX, IDC_btnRecord, m_btnRecord);
	DDX_Control(pDX, IDC_btnPlay, m_btnPlay);
	DDX_Control(pDX, IDC_btnWinEncoder, m_btnWinEnc);
	DDX_Control(pDX, IDC_editFPS, m_editFPS);
	DDX_Control(pDX, IDC_editFileName, m_editAviName);
	DDX_Control(pDX, IDC_editSavePath, m_editPath);
	DDX_Check(pDX, IDC_checkCapCursor, m_checkCapCursor);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgCap, CBCGPDialog)
	//{{AFX_MSG_MAP(CDlgCap)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnCapPic, OnbtnCapPic)
	ON_BN_CLICKED(IDC_btnRecord, OnbtnRecord)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnStop, OnbtnStop)
	ON_BN_CLICKED(IDC_btnPause, OnbtnPause)
	ON_BN_CLICKED(IDC_btnWinEncoder, OnbtnWinEncoder)
	ON_BN_CLICKED(IDC_btnPlay, OnbtnPlay)
	ON_BN_CLICKED(IDC_btnNetSend, OnbtnNetSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCap message handlers

BOOL CDlgCap::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	
	m_btnWinEnc.SetFaceColor(RGB(140,230,254));
	m_btnWinEnc.EnableWinXPTheme();
	m_btnPlay.EnableWinXPTheme();
	
	m_editPath.SetWindowText("d:\\");
	m_editAviName.SetWindowText("test");
	m_editFPS.SetWindowText("10");
	m_editBaud.SetWindowText("4000000");
	m_ctlCheckCapCursor.SetCheck(1)	;

	m_pWndVideoShow = GetDlgItem(IDC_picOutVideo);
	if (!m_pWndVideoShow) {
		MessageBox("error");
		return TRUE;
	}

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgCap::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CBCGPDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgCap::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CBCGPDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlgCap::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CDlgCap::OnClose() 
{
	OnbtnStop();
	
	if (CanExit())
		CBCGPDialog::OnClose();
}

void CDlgCap::OnOK() 
{
	if (CanExit())
		CBCGPDialog::OnOK();
}

void CDlgCap::OnCancel() 
{
	if (CanExit())
		CBCGPDialog::OnCancel();
}

BOOL CDlgCap::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BYTE* CDlgCap::DoGetDIBits(BITMAPINFO& bi/*OUT*/, CDC* pDc/*NULL*/)
{
	if (!pDc)
		pDc = GetDesktopWindow()->GetDC();
	
	BOOL bRet = FALSE;
	CRect rc;
	CDC  memDC;	// 兼容DC,屏幕
	CBitmap bmp; 	BITMAP bitmap;	// 兼容位图,屏幕
	HBITMAP hBmpOld;
	
	GetDesktopWindow()->GetClientRect(rc);
	bRet = memDC.CreateCompatibleDC(pDc);
	if (!bRet) {
		TRACE("$$$$ CreateCompatibleDC() error $$$$\n");
		return NULL;
	}

	bRet = bmp.CreateCompatibleBitmap(pDc,rc.Width(),rc.Height());
	if (!bRet) {
		TRACE("$$$$ CreateCompatibleBitmap() error $$$$\n");
		return NULL;
	}

	hBmpOld = (HBITMAP)memDC.SelectObject((HBITMAP)bmp);
	bmp.GetBitmap(&bitmap);
	
	// 将桌面拷贝到兼容DC
	memDC.BitBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,pDc,0,0,SRCCOPY);
	if(m_ctlCheckCapCursor.GetCheck())
	{
		CPoint point;
		GetCursorPos(&point);
		HICON hicon = (HICON)GetCursor();
		memDC.DrawIcon(point,hicon);
	}
	
	DWORD dwImgSize = (bitmap.bmWidthBytes * bitmap.bmHeight + 1) * bitmap.bmBitsPixel / 8;
	BYTE* lpData = new BYTE[dwImgSize];
	int panelsize  = 0;
	if (bitmap.bmBitsPixel < 16)					// current : 32
		panelsize = pow (2,bitmap.bmBitsPixel*sizeof(RGBQUAD));
	
	// localalloc 与 globalalloc 在win32中已没有区别, LPTR = (LMEM_FIXED | LMEM_ZEROINIT)
	// If biHeight is negative, indicating a top-down DIB, 
	// biCompression must be either BI_RGB or BI_BITFIELDS. Top-down DIBs cannot be compressed
	BITMAPINFOHEADER *pbih = (BITMAPINFOHEADER*)LocalAlloc(LPTR,sizeof(BITMAPINFO)+panelsize);
	pbih->biBitCount       = bitmap.bmBitsPixel;	// 32 ARGB !=PIX_FMT_RGB32_1
	pbih->biHeight         = m_bWinEnc ? bitmap.bmHeight : -bitmap.bmHeight;
	pbih->biWidth          = bitmap.bmWidth;
	pbih->biPlanes         = bitmap.bmPlanes;
	pbih->biSize           = sizeof(BITMAPINFOHEADER);
	pbih->biCompression    = 0;				// 不压缩 使用 BI_RGB(0L) 表示
	pbih->biSizeImage      = 0;				// 如果是不压缩的格式,此值可以为零
	
	// 将兼容位图的信息拷贝到 lpData 中,如果 lpData==NULL将填充(根据其中的bih信息确定如何填充) BITMAPINFO 结构体
	bi.bmiHeader = *pbih;
	GetDIBits(memDC.m_hDC,bmp,0,bitmap.bmHeight,lpData,&bi,DIB_RGB_COLORS);
	
	TRACE("### image size:%u, width:%d, height:%d, bitcount:%d ###\n",bi.bmiHeader.biSizeImage,bi.bmiHeader.biWidth,
		bi.bmiHeader.biHeight,bi.bmiHeader.biBitCount);

	memDC.SelectObject(hBmpOld);
	LocalFree(pbih);
	bmp.DeleteObject();

	GetDesktopWindow()->ReleaseDC(pDc);

	return lpData;
}


void CDlgCap::PlayVideoFile()
{
	// 默认的播放文件是 D:\\test.avi
	m_decoder.close();
	m_decoder.open(m_strAviPathName);

	if (m_hDrawDIB)
	{
		DrawDibClose(m_hDrawDIB);
		m_hDrawDIB = NULL;
	}
	m_hDrawDIB = DrawDibOpen() ;

	AfxBeginThread(ThreadPlayVideoFile,this);
}


void CDlgCap::ShowPicture()
{
	CRect rcVideo;
	::GetClientRect(m_pWndVideoShow->GetSafeHwnd(),rcVideo);
	int nDestW = rcVideo.Width();
	int nDestH = rcVideo.Height();
	int nSrcW = 1366 ;
	int nSrcH = 768;
	HDC hDC = NULL;
	int  nYUVLen = 0;
	BOOL bRet = FALSE;
	BITMAPINFOHEADER bmih={0};

	while (true)
	{
		// decode and play
		int signal = m_decoder.decodeFrame();
		if (signal != 0) return;

		nSrcW = m_decoder.m_iWidth;
		nSrcH = m_decoder.m_iHeight;
		bmih.biPlanes= 1; 
		bmih.biBitCount = 32; 
		bmih.biSize  = sizeof(BITMAPINFOHEADER) ;	
		bmih.biWidth = nSrcW;	
		bmih.biHeight = -nSrcH;		// 不然就倒立,
		
		memcpy(m_pBufAFrame, m_decoder.getVideoFrame(), m_decoder.getVideoFrameSize());
		
		// this is a pure yuv data,解码之后的yuv数据
		// m_flYUV.WriteHuge(m_pBufAFrame,m_decoder.getVideoFrameSize());	
		
		// 转化为 RGB 显示
		m_pSwsContext = sws_getContext(
			nSrcW, nSrcH, PIX_FMT_YUV420P ,
			nSrcW, nSrcH, PIX_FMT_RGB32 ,
			0x1,				// SWS_SA_FAST_BILINEAR
			NULL, NULL, NULL);
		if (NULL == m_pSwsContext) {
			return ;
		}
		
		int aDstStride[4] = {nSrcW*4, 0,0,0};
		sws_scale(m_pSwsContext, m_decoder.m_data, m_decoder.m_linesize, 0, nSrcH,
				  m_PlayInfo.pRGB, aDstStride);
		
		if (NULL != m_pSwsContext)
		{
			sws_freeContext(m_pSwsContext);
			m_pSwsContext = NULL;		
		}

		hDC = ::GetDC( m_pWndVideoShow->GetSafeHwnd() ) ;
		
		::SetStretchBltMode(hDC,HALFTONE) ;
		::StretchDIBits(hDC,0,0,nDestW,nDestH,
						rcVideo.left,rcVideo.top,nSrcW,nSrcH, 
						m_PlayInfo.pRGB[0], 
						(BITMAPINFO *)&bmih, DIB_RGB_COLORS,SRCCOPY);	
		TRACE("\n--- DrawDibDraw() return: %d, VideoPic.left:%d,right:%d,width:%d ---\n",bRet,rcVideo.left,rcVideo.right,rcVideo.Width());
		
		::ReleaseDC(m_pWndVideoShow->GetSafeHwnd(),hDC);
		Sleep(20);
	}
}

UINT CDlgCap::ThreadPlayVideoFile( LPVOID lpParam )
{
	CDlgCap* pDlg = (CDlgCap*)lpParam;
	if (!pDlg) {
		return -11;
	}

	pDlg->m_bPlaying = TRUE;

	pDlg->ShowPicture();

	if (pDlg->m_flYUV.m_hFile != CFile::hFileNull)
	{
		pDlg->m_flYUV.Close();
	}

	pDlg->m_bPlaying = FALSE;
	pDlg->m_btnPlay.EnableWindow(TRUE);
	pDlg->m_btnRecord.EnableWindow(TRUE);

	return 0;
}


//////////////////////////////////////////////////////////////////////////
void CDlgCap::Cap2Bmp(CString &strName)
{
	BYTE* lpData = NULL;
	BITMAPINFO bInfo;

	lpData = DoGetDIBits(bInfo);
	if (!lpData) {
		return;
	}

	// sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO) + pbih->biClrUsed * sizeof(RGBQUAD);
	// 14 +  18
	BITMAPFILEHEADER bfh;											//位图文件头
	bfh.bfType      = ((WORD)('M'<< 8)|'B');
	bfh.bfSize      = 32 + bInfo.bmiHeader.biSizeImage;				// 32 <==> 54
	bfh.bfOffBits   = 32;
	bfh.bfReserved1 = bfh.bfReserved2 = 0;
	CFile file;
	if(file.Open(strName,CFile::modeCreate|CFile::modeWrite))
	{
		file.WriteHuge(&bfh,sizeof(BITMAPFILEHEADER));
		file.WriteHuge(&bInfo.bmiHeader,sizeof(BITMAPINFOHEADER));
		file.WriteHuge(lpData,bInfo.bmiHeader.biSizeImage);
		file.Close();
	}

	delete []lpData;
}


// void RGB24_to_YV12(unsigned char * in, unsigned char * out,int w, int h);
// void YV12_to_RGB24(unsigned char *src0,unsigned char *src1,unsigned char *src2,unsigned char *dst_ori,int width,int height);
// 注意给的码率太低就会模糊
void CDlgCap::Cap2Avi(CString& strName,DWORD dwFps,DWORD dwBaud/*400000*/)
{
	HRESULT hr = S_FALSE;
	BYTE* lpData = NULL;
	BITMAPINFO bi;
	
	lpData = DoGetDIBits(bi);
	if (!lpData) {
		return;
	}
	int iPicHeight = bi.bmiHeader.biHeight > 0 ? bi.bmiHeader.biHeight : -bi.bmiHeader.biHeight;
	
	// write the pure rgb data
	// 写 RGB 数据失败,可能格式不对
	//m_flRGB.WriteHuge(lpData, bi.bmiHeader.biSizeImage);

	if (m_bWinEnc)	// windows encoder
	{
		if (m_nFrame == 0)									//第一次写入数据
		{		
			// lpHandler = null 系统将自动从注册表中选择一个默认的CLSID
			AVIFileOpen(&m_pAviFile,strName, OF_WRITE | OF_CREATE,NULL);
			
			memset(&m_aviStrmInfo, 0, sizeof(m_aviStrmInfo));
			m_aviStrmInfo.fccType    = streamtypeVIDEO;		//视频录制
			m_aviStrmInfo.fccHandler = 0;
			m_aviStrmInfo.dwScale    = 1;
			m_aviStrmInfo.dwRate     = dwFps;				//帧率
			TRACE("\n--- stream rate: %d",dwFps);
			m_aviStrmInfo.dwSuggestedBufferSize = bi.bmiHeader.biSizeImage;
			
			SetRect(&m_aviStrmInfo.rcFrame,0,0,bi.bmiHeader.biWidth,iPicHeight);
			hr = AVIFileCreateStream(m_pAviFile, &m_pAviStrm, &m_aviStrmInfo);
			
			m_pCompOptions[0]->fccType = streamtypeVIDEO;
			m_pCompOptions[0]->fccHandler = mmioStringToFOURCC("MSVC", 0);
			m_pCompOptions[0]->dwQuality = 7500;
			m_pCompOptions[0]->dwFlags = AVICOMPRESSF_VALID | AVICOMPRESSF_KEYFRAMES;
			m_pCompOptions[0]->dwBytesPerSecond = 0;
			m_pCompOptions[0]->lpFormat = 0;
			m_pCompOptions[0]->cbFormat = 0;
			m_pCompOptions[0]->dwInterleaveEvery = 0;
			AVIMakeCompressedStream(&m_pAviStrmComp,m_pAviStrm,&m_CompressOption,NULL);
			
			AVIStreamSetFormat(m_pAviStrmComp,0,&bi.bmiHeader,sizeof(BITMAPINFOHEADER));
		}
		
		hr = AVIStreamWrite(m_pAviStrmComp, m_nFrame, 1, (LPBYTE)lpData,
							bi.bmiHeader.biSizeImage, AVIIF_KEYFRAME,NULL,NULL);
		
		m_nFrame++;
		
	}
	else		// ffmpeg encoder
	{
		// encoder
		if (m_bFirstTime)
		{
			m_bFirstTime = FALSE;
			m_encoder.setVideoParam(bi.bmiHeader.biWidth, iPicHeight, PIX_FMT_RGB32, dwBaud, dwFps);
			m_encoder.open(strName);
		}
		m_encoder.writeVideoFrame( lpData );
	}
	delete []lpData;
}

void CDlgCap::OnbtnCapPic() 
{
	// TODO: Add your control notification handler code here
	CString path,name,m_number;
	m_editPath.GetWindowText(path);
	CTime time = CTime::GetCurrentTime();
	name = path + "\\" + time.Format("%Y%m%d%H%M%S") + ".bmp";

	::PlaySound(MAKEINTRESOURCE(IDR_Capture),::GetModuleHandle(NULL),SND_RESOURCE|SND_NODEFAULT|SND_NOWAIT|SND_SYNC);
	Cap2Bmp(name);
}

void CDlgCap::OnbtnRecord() 
{
	// TODO: Add your control notification handler code here
	if (m_bRecing) return;
	
	CString rate,name,path,baud;
	m_editAviName.GetWindowText(name);
	m_editPath.GetWindowText(path);
	m_editFPS.GetWindowText(rate);
	m_editBaud.GetWindowText(baud);

	
	path.TrimRight(' ');
	path.TrimRight('\\');
	path = path + "\\";
	m_strAviPathName = path+name+".avi";
	m_dwFps = atoi(rate);
	m_dwBuad= atoi(baud);
	
	if (m_btnWinEnc)
	{
		AVIFileInit();
	}

	SetTimer(1,1000/m_dwFps,NULL);

	m_bRecing = TRUE;

	m_btnWinEnc.EnableWindow(FALSE);
	m_btnPlay.EnableWindow(FALSE);
}

void CDlgCap::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		Cap2Avi(m_strAviPathName,m_dwFps,m_dwBuad);		
	}

	if (nIDEvent == 2)
	{
		KillTimer(2);
		AfxBeginThread(ThreadSendVideoData,this);
	}
	
	CBCGPDialog::OnTimer(nIDEvent);
}

void CDlgCap::OnbtnStop() 
{
	// TODO: Add your control notification handler code here
	if (m_bRecing)
	{
		m_bRecing = FALSE;

		KillTimer(1);

		if (m_bWinEnc)
		{
			if (m_pAviFile)
			{
				// 注意释放的顺序及要求:缺一不可且顺序一致!
				AVIStreamRelease(m_pAviStrmComp);
				AVIStreamRelease(m_pAviStrm);
				AVIFileRelease(m_pAviFile);
				AVIFileExit();
				m_nFrame =0;
			}
		}

		m_decoder.close();
		m_encoder.close();
//		m_flRGB.Close();

		m_btnWinEnc.EnableWindow(TRUE);
		m_btnPlay.EnableWindow(TRUE);
	}	
}

void CDlgCap::OnbtnPause() 
{
	// TODO: Add your control notification handler code here
	m_bIsPause = !m_bIsPause;
	m_bIsPause ? KillTimer(1) : SetTimer(1,1000/m_dwFps,NULL);	
}

void CDlgCap::OnbtnWinEncoder() 
{
	// TODO: Add your control notification handler code here
	m_bWinEnc = !m_bWinEnc;

	m_bWinEnc ? m_btnWinEnc.SetTextColor(RGB(255,0,0)) : m_btnWinEnc.SetTextColor(RGB(0,0,0));
}

void CDlgCap::OnbtnPlay() 
{
	// TODO: Add your control notification handler code here
	if (m_strAviPathName.IsEmpty())
		return;
	
	if (m_bPlaying)
	{
		MessageBox("In Playing!");
		return;
	}

	PlayVideoFile();

	m_btnPlay.EnableWindow(FALSE);
	m_btnRecord.EnableWindow(FALSE);
}


//////////////////////////////////////////////////////////////////////////
void CDlgCap::OnbtnNetSend() 
{
	// TODO: Add your control notification handler code here
	// 开启监听套接字
	QuickStartTCPServer(m_wPortSrv, CB_Accept);

	CString rate,baud;
	m_editFPS.GetWindowText(rate);
	m_editBaud.GetWindowText(baud);
	m_dwFps = atoi(rate);
	m_dwBuad= atoi(baud);
}


//////////////////////////////////////////////////////////////////////////

void CDlgCap::CB_Accept( SOCKET sListenSock, SOCKET nNewSock,SOCKADDR_IN *pInAddrCl/*=NULL*/, void *pUPDInData/*=NULL*/, DWORD dwUDPInDataLen/*=NULL*/ )
{
	if (pInAddrCl)
	{
		g_CBAcceptParam.sockListen = sListenSock;
		g_CBAcceptParam.sockNew = nNewSock;
		g_CBAcceptParam.pAddrClient = *pInAddrCl;
		
		// 发送到缓冲队列,准备向网络发送
		::SetTimer(AfxGetMainWnd()->GetSafeHwnd(),2,4,NULL);
	}
}

UINT CDlgCap::ThreadSendVideoData( LPVOID lpParam )
{
	CDlgCap* pDlg = (CDlgCap*)lpParam;
	if (!pDlg) {
		return 12;
	}

	while (true)
	{
		pDlg->Cap2Que();
	}
	
	return 0;
}

void CDlgCap::SendVideoData()
{
	
}

void CDlgCap::Cap2Que()
{
	HRESULT		hr = S_FALSE;
	BYTE*		lpData = NULL;
	BITMAPINFO	bi;
	DWORD		dwSendLen;
	
	lpData = DoGetDIBits(bi);
	if (!lpData) {
		return;
	}

	int iPicHeight = bi.bmiHeader.biHeight > 0 ? bi.bmiHeader.biHeight : -bi.bmiHeader.biHeight;

	// encoder
	if (!m_bFirstTime2)
	{
		m_encoder.setVideoParam(bi.bmiHeader.biWidth, iPicHeight, PIX_FMT_RGB32, m_dwBuad, m_dwFps);
		m_encoder.open();
		m_bFirstTime2 = TRUE;
	}
	m_encoder.encodeVideoFrame ( lpData );		// 如果出错将抛出异常
	m_EncData.length =  m_encoder.getVideoFrameSize();
	if (m_EncData.length > 0)
	{
		memcpy(m_EncData.pDataEnc, m_encoder.getVideoEncodedBuffer(),m_EncData.length);
	}

	// 1, 使用队列,令启发送线程

	// 2, 不使用队列
	sprintf(m_pBufHeader,"%06X",m_EncData.length) ;

	if (4 == ForceSend(g_CBAcceptParam.sockNew,m_pBufHeader,4,5000))
	{
		TRACE("\n---> ForceSend() length: %d --->\n",dwSendLen);
		dwSendLen = ForceSend(g_CBAcceptParam.sockNew,m_EncData.pDataEnc,m_EncData.length);
	}

	delete []lpData;
}
