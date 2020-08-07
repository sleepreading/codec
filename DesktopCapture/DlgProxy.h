// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__8E92ACE9_2331_4D60_B03B_498D8E1E58D8__INCLUDED_)
#define AFX_DLGPROXY_H__8E92ACE9_2331_4D60_B03B_498D8E1E58D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDlgCap;

/////////////////////////////////////////////////////////////////////////////
// CCapDlgAutoProxy command target

class CCapDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CCapDlgAutoProxy)

	CCapDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CDlgCap* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCapDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCapDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CCapDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CCapDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CCapDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__8E92ACE9_2331_4D60_B03B_498D8E1E58D8__INCLUDED_)
