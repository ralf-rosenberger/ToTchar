#if !defined(AFX_OPTIONSDLG_H__E72542A2_2DCE_11D6_9E81_F84427703A51__INCLUDED_)
#define AFX_OPTIONSDLG_H__E72542A2_2DCE_11D6_9E81_F84427703A51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld COptionsDlg 

class COptionsDlg : public CDialog
{
// Konstruktion
public:
	COptionsDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_OPTIONS };
	CString	m_strExtensions;
	BOOL	m_bSubFolders;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(COptionsDlg)
		// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_OPTIONSDLG_H__E72542A2_2DCE_11D6_9E81_F84427703A51__INCLUDED_
