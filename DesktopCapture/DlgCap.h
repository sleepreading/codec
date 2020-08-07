// DlgCap.h : header file
//

#if !defined(AFX_DLGCAP_H__72A02441_7842_4730_A8C6_B4E99E1E0A0B__INCLUDED_)
#define AFX_DLGCAP_H__72A02441_7842_4730_A8C6_B4E99E1E0A0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCapDlgAutoProxy;

#include "QueueBuf.h"

#include "FFmpegEncoder.h"
#include "FFmpegDecoder.h"

#ifdef _DEBUG
#pragma comment (lib,"lib/FFmpegWrapperD.lib")
#else
#pragma comment (lib,"lib/FFmpegWrapperR.lib")
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCap dialog
typedef struct PlayInfo_s
{
	CHAR	*pY,*pU,*pV;
	uint8_t *pRGB[4];
	int		iWidth,iHeight;
} PlayInfo_s;

typedef struct CBAccept_Param_s
{
	SOCKET	sockListen;
	SOCKET	sockNew;
	sockaddr_in pAddrClient;
}CBAccept_Param_s;

class CDlgCap : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgCap);
	friend class CCapDlgAutoProxy;
	
// Construction
public:
	CDlgCap(CWnd* pParent = NULL);	// standard constructor
	virtual ~CDlgCap();

	//////////////////////////////////////////////////////////////////////////
	void Cap2Bmp(CString &strName);		// 抓图
	void Cap2Avi(CString& strName, DWORD dwFps, DWORD dwBaud=400000);	// 录像
	void Cap2Que();						// 录像并发送到网络

	void PlayVideoFile();				// 本地文件播放

	static UINT ThreadPlayVideoFile(LPVOID lpParam);
	static UINT ThreadSendVideoData(LPVOID lpParam);
	static void CB_Accept(SOCKET sListenSock, SOCKET nNewSock,SOCKADDR_IN *pInAddrCl=NULL, void *pUPDInData=NULL, DWORD dwUDPInDataLen=NULL);

	PlayInfo_s	m_PlayInfo;
	EncData_s	m_EncData;
	volatile BOOL m_bPlaying;

private:
	BYTE* DoGetDIBits(BITMAPINFO& bi/*OUT*/, CDC* pDc=NULL);
	void ShowPicture();
	void SendVideoData();
	
//////////////////////////////////////////////////////////////////////////
	char*				m_pBufHeader;
	
	CQueueBuf			m_bufSend;
	WORD				m_wPortSrv;
	HDRAWDIB			m_hDrawDIB;
	UINT m_nFrame;

	CFile m_flYUV;
	CFile m_flRGB;
	FFmpegEncoder		m_encoder;
	FFmpegDecoder		m_decoder;
//	CFFScale			m_ffScale;
	SwsContext*			m_pSwsContext;		//SWS对象
	CHAR*				m_pBufAFrame;
	int					m_iSizeVFrame;
	CWnd*				m_pWndVideoShow;

	AVICOMPRESSOPTIONS	m_CompressOption;
	AVICOMPRESSOPTIONS*	m_pCompOptions[1];

	AVISTREAMINFO	m_aviStrmInfo;
	PAVIFILE		m_pAviFile;
	PAVISTREAM		m_pAviStrm; 			//视频数据流
	PAVISTREAM		m_pAviStrmComp;		//压缩视频数据流

	CString			m_strAviPathName;
	DWORD			m_dwFps;
	DWORD			m_dwBuad;
	BOOL			m_bRecing;
	BOOL			m_bIsPause;
	BOOL			m_bWinEnc;
	BOOL			m_bFirstTime;
	BOOL			m_bFirstTime2;
	//////////////////////////////////////////////////////////////////////////

// Dialog Data
	//{{AFX_DATA(CDlgCap)
	enum { IDD = IDD_CAP_DIALOG };
	CEdit	m_editBaud;
	CButton	m_ctlCheckCapCursor;
	CButton	m_btnRecord;
	CBCGPButton	m_btnPlay;
	CBCGPButton	m_btnWinEnc;
	CEdit	m_editFPS;
	CEdit	m_editAviName;
	CEdit	m_editPath;
	BOOL	m_checkCapCursor;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCapDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	//{{AFX_MSG(CDlgCap)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnbtnCapPic();
	afx_msg void OnbtnRecord();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnbtnStop();
	afx_msg void OnbtnPause();
	afx_msg void OnbtnWinEncoder();
	afx_msg void OnbtnPlay();
	afx_msg void OnbtnNetSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCAP_H__72A02441_7842_4730_A8C6_B4E99E1E0A0B__INCLUDED_)
