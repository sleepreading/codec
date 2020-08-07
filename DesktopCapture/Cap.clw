; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Cap.h"
ODLFile=Cap.odl

ClassCount=4
Class1=CAppCap
Class2=CDlgCap
Class3=CAboutDlg
Class4=CCapDlgAutoProxy

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_CAP_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_CAP_DIALOG (English (U.S.))

[CLS:CAppCap]
Type=0
HeaderFile=Cap.h
ImplementationFile=Cap.cpp
Filter=N

[CLS:CDlgCap]
Type=0
HeaderFile=DlgCap.h
ImplementationFile=DlgCap.cpp
Filter=D
BaseClass=CBCGPDialog
VirtualFilter=dWC
LastObject=IDC_editBaud

[CLS:CAboutDlg]
Type=0
HeaderFile=DlgCap.h
ImplementationFile=DlgCap.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDOK

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[CLS:CCapDlgAutoProxy]
Type=0
HeaderFile=DlgProxy.h
ImplementationFile=DlgProxy.cpp
BaseClass=CCmdTarget
Filter=N

[DLG:IDD_CAP_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CDlgCap

[DLG:IDD_CAP_DIALOG (English (U.S.))]
Type=1
Class=CDlgCap
ControlCount=21
Control1=IDC_btnRecord,button,1342242816
Control2=IDC_btnStop,button,1342242816
Control3=IDC_btnPause,button,1342242816
Control4=IDC_btnSelPath,button,1342242816
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_editFPS,edit,1350639744
Control10=IDC_editFileName,edit,1350631552
Control11=IDC_editSavePath,edit,1350631552
Control12=IDC_checkCapCursor,button,1342242819
Control13=IDC_stcFreeSpace,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_btnCapPic,button,1342242816
Control16=IDC_btnWinEncoder,button,1342242816
Control17=IDC_picOutVideo,static,1342177287
Control18=IDC_btnPlay,button,1342242816
Control19=IDC_btnNetSend,button,1342242816
Control20=IDC_STATIC,static,1342308352
Control21=IDC_editBaud,edit,1350639744

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

