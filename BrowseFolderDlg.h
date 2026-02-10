// BrowseFolderDlg.h: Schnittstelle für die Klasse CBrowseFolderDlg.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BROWSEFOLDERDLG_H__1335D741_D35A_11D4_9E7E_F27C2E29EC33__INCLUDED_)
#define AFX_BROWSEFOLDERDLG_H__1335D741_D35A_11D4_9E7E_F27C2E29EC33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>
#include <shlobj.h>

class CBrowseFolderDlg  
{
public:
	void GetChosenFolder(CString& strFolder) const;
	void GetChosenFolder(LPTSTR lpszFolder,int iSize = MAX_PATH);
	int DoModal(void);
	CBrowseFolderDlg(LPCTSTR lpszFolder = NULL,
		             int iSpecialFolder = CSIDL_DRIVES,
					 HWND hWndParent = NULL,
					 UINT uFlags = BIF_RETURNONLYFSDIRS);
	virtual ~CBrowseFolderDlg();
	BROWSEINFO m_bi;

private:
	UINT m_uFlags;
	int m_iSpecialFolder;
	CString m_strFolder;
	HWND m_hWndParent;
};

extern "C"
	{
	int CALLBACK BrowseCallbackProc(HWND hWnd,UINT uMsg,LPARAM lParam,LPARAM lpData); 	
	}

#endif // !defined(AFX_BROWSEFOLDERDLG_H__1335D741_D35A_11D4_9E7E_F27C2E29EC33__INCLUDED_)
