// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "Cap.h"
#include "DlgProxy.h"
#include "DlgCap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCapDlgAutoProxy

IMPLEMENT_DYNCREATE(CCapDlgAutoProxy, CCmdTarget)

CCapDlgAutoProxy::CCapDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CDlgCap, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CDlgCap*) AfxGetApp()->m_pMainWnd;
	m_pDialog->m_pAutoProxy = this;
}

CCapDlgAutoProxy::~CCapDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CCapDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CCapDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CCapDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CCapDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CCapDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ICap to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {98868B10-D612-494F-94FB-5DBD63ADD5A3}
static const IID IID_ICap =
{ 0x98868b10, 0xd612, 0x494f, { 0x94, 0xfb, 0x5d, 0xbd, 0x63, 0xad, 0xd5, 0xa3 } };

BEGIN_INTERFACE_MAP(CCapDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CCapDlgAutoProxy, IID_ICap, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {02FF0E88-56D4-4DE1-B7CC-C97CA8AD5AC7}
IMPLEMENT_OLECREATE2(CCapDlgAutoProxy, "Cap.Application", 0x2ff0e88, 0x56d4, 0x4de1, 0xb7, 0xcc, 0xc9, 0x7c, 0xa8, 0xad, 0x5a, 0xc7)

/////////////////////////////////////////////////////////////////////////////
// CCapDlgAutoProxy message handlers
