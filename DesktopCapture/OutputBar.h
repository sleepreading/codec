// OutputBar.h : interface of the COutputBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTBAR_H__2A6545AB_446C_4A16_BF98_9A78789D7F7A__INCLUDED_)
#define AFX_OUTPUTBAR_H__2A6545AB_446C_4A16_BF98_9A78789D7F7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputBar : public CBCGPDockingControlBar
{
public:
	COutputBar();

// Attributes
protected:
	CBCGPTabWnd	m_wndTabs;

	CListCtrl	m_wndList1;
	CListCtrl	m_wndList2;
	CListCtrl	m_wndList3;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COutputBar();

// Generated message map functions
protected:
	//{{AFX_MSG(COutputBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTBAR_H__2A6545AB_446C_4A16_BF98_9A78789D7F7A__INCLUDED_)
