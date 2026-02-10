// ToTcharDlg.h : Header-Datei
//

#include "resource.h"
#if !defined(AFX_TOTCHARDLG_H__09D9ED24_2D32_11D6_9E81_F3B3A6C73B51__INCLUDED_)
#define AFX_TOTCHARDLG_H__09D9ED24_2D32_11D6_9E81_F3B3A6C73B51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CToTcharDlg Dialogfeld

class CToTcharDlg : public CDialog
{
// Konstruktion
public:
	CToTcharDlg(CWnd* pParent = NULL);	// Standard-Konstruktor

// Dialogfelddaten
	//{{AFX_DATA(CToTcharDlg)
	enum { IDD = IDD_TOTCHAR_DIALOG };
	CListCtrl	m_listFiles;
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CToTcharDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Message-Map-Funktionen
	//{{AFX_MSG(CToTcharDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnAddFile();
	afx_msg void OnAddFolder();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOptions();
	afx_msg void OnDestroy();
	afx_msg void OnMyhelp();
	//}}AFX_MSG
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnDropFiles(HDROP hDrop);
	DECLARE_MESSAGE_MAP()
private:
	bool m_bSubFolders;
	int m_iMinCy;
	int m_iMinCx;
	int m_iOldCy;
	int m_iOldCx;
	CStringArray m_strExtensionArray;
	void StringToArray(CStringArray& strArray,LPCTSTR lpszString);
	void ArrayToString(CString& str, const CStringArray& strArray);
	CImageList m_imageList;
	UINT m_nFiles;
	bool AddFile(LPCTSTR lpszFile);
	bool AddFolder(LPCTSTR lpszFolder);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_TOTCHARDLG_H__09D9ED24_2D32_11D6_9E81_F3B3A6C73B51__INCLUDED_)
