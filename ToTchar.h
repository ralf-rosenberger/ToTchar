// ToTchar.h : Haupt-Header-Datei für die Anwendung TOTCHAR
//

#if !defined(AFX_TOTCHAR_H__09D9ED22_2D32_11D6_9E81_F3B3A6C73B51__INCLUDED_)
#define AFX_TOTCHAR_H__09D9ED22_2D32_11D6_9E81_F3B3A6C73B51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CToTcharApp:
// Siehe ToTchar.cpp für die Implementierung dieser Klasse
//

class CToTcharApp : public CWinApp
{
public:
	CToTcharApp();

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CToTcharApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	//{{AFX_MSG(CToTcharApp)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_TOTCHAR_H__09D9ED22_2D32_11D6_9E81_F3B3A6C73B51__INCLUDED_)
