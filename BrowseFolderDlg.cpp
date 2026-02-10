// BrowseFolderDlg.cpp: Implementierung der Klasse CBrowseFolderDlg.
//
// 1.0 written by Ralf Rosenberger Sonntag, 6. Februar 2000
// 2.0 written by Ralf Rosenberger Freitag, 11. Februar 2000 
//     "SelectCallback" eingefügt
// 3.0 written by Ralf Rosenberger Samstag, 16. Dezember 2000
//     Klassendesign verbessert
// 4.0 written by Ralf Rosenberger Dienstag, 26. Dezember 2000
//     Flags werden im Konstruktor initialisiert
// 5.0 written by Ralf Rosenberger Mittwoch, 21. November 2001
//     Unicode-Konform gemacht
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BrowseFolderDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CBrowseFolderDlg::CBrowseFolderDlg(LPCTSTR lpszFolder,int iSpecialFolder,HWND hWndParent,UINT uFlags)
{
	m_hWndParent = hWndParent;
	m_iSpecialFolder = iSpecialFolder;

	if(lpszFolder)
		m_strFolder = lpszFolder;

	m_uFlags = uFlags;

	ZeroMemory(&m_bi,sizeof(BROWSEINFO));
}

CBrowseFolderDlg::~CBrowseFolderDlg()
{
}

int CBrowseFolderDlg::DoModal()
{
IMalloc *pMalloc;
LPITEMIDLIST pidl;

    //Den Start-Folder festlegen.
    SHGetSpecialFolderLocation(m_hWndParent,m_iSpecialFolder,&pidl);

	m_bi.pidlRoot  = pidl;
    m_bi.hwndOwner = m_hWndParent; 

	if(!m_strFolder.IsEmpty())
		{
		m_bi.lpfn   = (BFFCALLBACK)BrowseCallbackProc;
		m_bi.lParam = (LPARAM)LPCTSTR(m_strFolder);
		}

	m_bi.ulFlags = m_uFlags;

	if(!m_bi.lpszTitle)
		m_bi.lpszTitle = _T("Wählen Sie einen Ordner aus!");

    pidl = SHBrowseForFolder(&m_bi);
    if(pidl) 
		{
		TCHAR szFolder[MAX_PATH];

		//Ermittle den Namen des Ordners 
		if(SHGetPathFromIDList(pidl,szFolder))
			m_strFolder = szFolder;
		}
    else return IDCANCEL;

	if(SHGetMalloc(&pMalloc) == NOERROR)
		{
		pMalloc->Free(pidl);
		pMalloc->Release();
		}
 
return IDOK;
}

void CBrowseFolderDlg::GetChosenFolder(LPTSTR lpszFolder, int iSize)
{
	if(lpszFolder && iSize)
		lstrcpyn(lpszFolder,LPCTSTR(m_strFolder),iSize);
}

void CBrowseFolderDlg::GetChosenFolder(CString &strFolder) const
{
	strFolder = m_strFolder;
}

//extern "C"
int CALLBACK BrowseCallbackProc(HWND hWnd,UINT uMsg,LPARAM lParam, LPARAM lpData) 
	{
	switch(uMsg)
		{
		case BFFM_INITIALIZED:
			SendMessage(hWnd,BFFM_SETSELECTION,(WPARAM)TRUE,lpData);
			break;
		}

	return 0;
	}
