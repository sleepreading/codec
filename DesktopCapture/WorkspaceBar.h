// WorkspaceBar.h : interface of the CWorkspaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACEBAR_H__1041266C_F7EE_4FD3_8DAB_A0AD4DC7A798__INCLUDED_)
#define AFX_WORKSPACEBAR_H__1041266C_F7EE_4FD3_8DAB_A0AD4DC7A798__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWorkspaceBar : public CBCGPDockingControlBar
{
public:
	CWorkspaceBar();

// Attributes
protected:
	CTreeCtrl	m_wndTree;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkspaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkspaceBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CWorkspaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACEBAR_H__1041266C_F7EE_4FD3_8DAB_A0AD4DC7A798__INCLUDED_)
