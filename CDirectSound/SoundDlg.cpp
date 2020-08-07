// SoundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sound.h"
#include "SoundDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoundDlg dialog

CSoundDlg::CSoundDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoundDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoundDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSoundDlg, CDialog)
	//{{AFX_MSG_MAP(CSoundDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Sound2, OnSound2)
	ON_BN_CLICKED(IDC_Sound1, OnSound1)
	ON_BN_CLICKED(IDC_BothSounds, OnBothSounds)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundDlg message handlers

BOOL CSoundDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	VERIFY(m_sndSound1.Create(IDR_SOUND1));
	VERIFY(m_sndSound2.Create(IDR_SOUND2));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSoundDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

HCURSOR CSoundDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSoundDlg::OnSound2() 
{
	m_sndSound1.Play();
}

void CSoundDlg::OnSound1() 
{
	m_sndSound2.Play();
}

void CSoundDlg::OnBothSounds() 
{
	m_sndSound1.Play();
	m_sndSound2.Play();
}
