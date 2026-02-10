; CLW-Datei enthält Informationen für den MFC-Klassen-Assistenten

[General Info]
Version=1
LastClass=CToTcharDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ToTchar.h"

ClassCount=4
Class1=CToTcharApp
Class2=CToTcharDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_TOTCHAR_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=COptionsDlg
Resource4=IDD_OPTIONS

[CLS:CToTcharApp]
Type=0
HeaderFile=ToTchar.h
ImplementationFile=ToTchar.cpp
Filter=N
LastObject=CToTcharApp

[CLS:CToTcharDlg]
Type=0
HeaderFile=ToTcharDlg.h
ImplementationFile=ToTcharDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_MYHELP

[CLS:CAboutDlg]
Type=0
HeaderFile=ToTcharDlg.h
ImplementationFile=ToTcharDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TOTCHAR_DIALOG]
Type=1
Class=CToTcharDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LISTFILES,SysListView32,1350664213
Control4=IDC_MYHELP,button,1342242816
Control5=IDC_ADDFILE,button,1342242816
Control6=IDC_ADDFOLDER,button,1342242816
Control7=IDC_OPTIONS,button,1342242816

[DLG:IDD_OPTIONS]
Type=1
Class=COptionsDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDITEXTENSIONS,edit,1350631552
Control5=IDC_CHECKSUBFOLDERS,button,1342242819

[CLS:COptionsDlg]
Type=0
HeaderFile=OptionsDlg.h
ImplementationFile=OptionsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=COptionsDlg
VirtualFilter=dWC

