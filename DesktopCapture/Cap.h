// Cap.h : main header file for the CAP application
//

#if !defined(AFX_CAP_H__38CF6AC1_23F3_4538_8A43_6CFDE8D9CD77__INCLUDED_)
#define AFX_CAP_H__38CF6AC1_23F3_4538_8A43_6CFDE8D9CD77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAppCap:
// See Cap.cpp for the implementation of this class
//

class CAppCap : public CWinApp,public CBCGPWorkspace
{
public:
	CAppCap();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppCap)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAppCap)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAP_H__38CF6AC1_23F3_4538_8A43_6CFDE8D9CD77__INCLUDED_)
