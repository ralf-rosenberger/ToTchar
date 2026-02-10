// ToTcharDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "ToTchar.h"
#include "ToTcharDlg.h"

#include <htmlhelp.h>
#include <libloaderapi.h>

#include "BrowseFolderDlg.h"
#include "Engine.h"
#include "OptionsDlg.h"
#include <Windows.h>
#include <commdlg.h>
#include <heapapi.h>
#include <CommCtrl.h>
#include <tchar.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// Keine Nachrichten-Handler
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToTcharDlg Dialogfeld

CToTcharDlg::CToTcharDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToTcharDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToTcharDlg)
		// HINWEIS: Der Klassenassistent fügt hier Member-Initialisierung ein
	//}}AFX_DATA_INIT
	// Beachten Sie, dass LoadIcon unter Win32 keinen nachfolgenden DestroyIcon-Aufruf benötigt
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_iOldCx = m_iOldCy = m_iMinCx = m_iMinCy = 0;

	m_bSubFolders = true;

	m_nFiles = 0;

	HDC hdc = ::GetDC(NULL);
	UINT uColourFlag;
	switch(::GetDeviceCaps(hdc,BITSPIXEL))
		{
		case 4:
			uColourFlag = ILC_COLOR16;
			break;

		case 8:
			uColourFlag = ILC_COLOR8;
			break;

		case 16:
			uColourFlag = ILC_COLOR16;
			break;

		case 32:
			uColourFlag = ILC_COLOR32;
			break;

		default:
			uColourFlag = ILC_COLOR;
		}

	::ReleaseDC(NULL,hdc);

	m_imageList.Create(GetSystemMetrics(SM_CXSMICON),
		               GetSystemMetrics(SM_CYSMICON),
					   ILC_MASK|uColourFlag,
					   0,0);
}

void CToTcharDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToTcharDlg)
	DDX_Control(pDX, IDC_LISTFILES, m_listFiles);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CToTcharDlg, CDialog)
	//{{AFX_MSG_MAP(CToTcharDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADDFILE, OnAddFile)
	ON_BN_CLICKED(IDC_ADDFOLDER, OnAddFolder)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_OPTIONS, OnOptions)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MYHELP, OnMyhelp)
	//}}AFX_MSG_MAP
	ON_WM_GETMINMAXINFO()
	ON_WM_DROPFILES()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToTcharDlg Nachrichten-Handler

BOOL CToTcharDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{	
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden
	
	// ZU ERLEDIGEN: Hier zusätzliche Initialisierung einfügen

	m_listFiles.SetImageList(&m_imageList,LVSIL_SMALL);

	m_listFiles.InsertColumn(0,"Datei");
	m_listFiles.SetColumnWidth(0,LVSCW_AUTOSIZE_USEHEADER);

	RECT rc,rcFrame;

	GetDlgItem(IDOK)->GetWindowRect(&rc);
	ScreenToClient(&rc);

	m_iMinCx = rc.right*3;

	GetWindowRect(&rcFrame);
	m_listFiles.GetWindowRect(&rc);
	ScreenToClient(&rc);

	m_iMinCy = rcFrame.bottom-rcFrame.top-rc.bottom+rc.top;

	LPCTSTR lpszSettings = "Settings";

	CString str = AfxGetApp()->GetProfileString(lpszSettings,"Extensions","c;cpp;h;hpp;");
	StringToArray(m_strExtensionArray,LPCTSTR(str));

	str = AfxGetApp()->GetProfileString(lpszSettings,"Maximize","FALSE");

	bool bMaximize = str == "TRUE";

	if(!bMaximize)
		{
		RECT *lpRc;
		UINT uSize = sizeof(RECT);

		if(AfxGetApp()->GetProfileBinary(lpszSettings,"Placement",(LPBYTE*)&lpRc,&uSize) && uSize == sizeof(RECT))
			MoveWindow(lpRc);
		}
	else
		ShowWindow(SW_MAXIMIZE);
	
	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void CToTcharDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// Wollen Sie Ihrem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch für Sie erledigt.

void CToTcharDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext für Zeichnen

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Symbol in Client-Rechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die Systemaufrufe fragen den Cursorform ab, die angezeigt werden soll, während der Benutzer
//  das zum Symbol verkleinerte Fenster mit der Maus zieht.
HCURSOR CToTcharDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CToTcharDlg::OnOK() 
{
BeginWaitCursor();

CEngine engine;
CString strFile;

bool bRet = false;

	for(int nItems = m_listFiles.GetItemCount(), i=0; i != nItems; i++)
		{
		strFile = m_listFiles.GetItemText(i,0);

		if(engine.SetFile(LPCTSTR(strFile)))
			{
			bRet = engine.Work(this);
			if(!bRet)
				{
				AfxMessageBox("Es ist ein Fehler aufgetreten !\nBeende Vorgang!");
				engine.Close();
				break;
				}

			engine.Close();
			}
		}

	if(bRet)
		AfxMessageBox("Vorgang wurde erfolgreich beendet!");

EndWaitCursor();

CDialog::OnOK();
}

void CToTcharDlg::OnAddFile() 
{
CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT,
				"C++ - Source (*.cpp;*.c;*.hpp;*.h)|*.cpp;*.c;*.hpp;*.h|Alle Dateien (*.*)|*.*||");
CString strFile;
const HANDLE hHeap = GetProcessHeap();
LPTSTR lpszBuf;

lpszBuf = (LPTSTR)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, MAX_PATH * MAX_PATH * sizeof(TCHAR));
if(!lpszBuf)
	AfxThrowMemoryException();

	dlg.m_ofn.lpstrFile = lpszBuf;
	dlg.m_ofn.nMaxFile = /*HeapSize(hHeap,HEAP_NO_SERIALIZE,lpszBuf)/sizeof(TCHAR)*/MAX_PATH * MAX_PATH;

	if(dlg.DoModal() == IDOK)
		{
		POSITION pos;

		pos = dlg.GetStartPosition();
		while(pos)
			{
			strFile = dlg.GetNextPathName(pos);
			AddFile(LPCTSTR(strFile));
			}
		}
}

void CToTcharDlg::OnAddFolder() 
{
CBrowseFolderDlg dlg;

	if(dlg.DoModal() == IDOK)
		{
		TCHAR szPath[MAX_PATH];

		BeginWaitCursor();

		dlg.GetChosenFolder(szPath);
		AddFolder(szPath);

		MessageBeep(MB_ICONINFORMATION);

		EndWaitCursor();
		}
}

bool CToTcharDlg::AddFolder(LPCTSTR lpszFolder)
{
HANDLE hFile;
WIN32_FIND_DATA wfd;
TCHAR szFile[MAX_PATH];
LPTSTR p;
bool bSlash = false;

	lstrcpy(szFile,lpszFolder);

	p = szFile + lstrlen(szFile)-1;
	if(*p == '\\')
		{
		bSlash = true;
		lstrcpy(++p,"*.*");
		}
	else
		{
		bSlash = false;
		lstrcpy(++p,"\\*.*");
		}

	ZeroMemory(&wfd,sizeof(WIN32_FIND_DATA));

	hFile = FindFirstFile(szFile,&wfd);
	if(hFile == INVALID_HANDLE_VALUE)
		return false;

	do
		{
		if(*wfd.cFileName != '.')
			{
			if(!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
				p = strrchr(wfd.cFileName,'.');
				if(p)
					{
					++p;

					INT_PTR ipSize = m_strExtensionArray.GetSize();

					for(INT_PTR i=0; i != ipSize; ++i)
						{
						if(!m_strExtensionArray[i].CompareNoCase(p))
							{
							lstrcpy(szFile,lpszFolder);
							if(!bSlash)
								lstrcat(szFile,"\\");

							lstrcat(szFile,wfd.cFileName);

							AddFile(szFile);
							}
						}
					}
				}
			else if(m_bSubFolders)
				{
				lstrcpy(szFile,lpszFolder);
				if(!bSlash)
					lstrcat(szFile,"\\");

				lstrcat(szFile,wfd.cFileName);

				AddFolder(szFile);
				}
			}

		}while(FindNextFile(hFile,&wfd));

	FindClose(hFile);

return true;
}

bool CToTcharDlg::AddFile(LPCTSTR lpszFile)
{
SHFILEINFO sfi;

	ZeroMemory(&sfi,sizeof(SHFILEINFO));
	SHGetFileInfo(lpszFile,0L,&sfi,sizeof(SHFILEINFO),SHGFI_ICON|SHGFI_SMALLICON);
	if(sfi.hIcon)
		{
		m_imageList.Add(sfi.hIcon);
		DestroyIcon(sfi.hIcon);
		}

	m_listFiles.InsertItem(m_nFiles,lpszFile,m_nFiles);
	m_listFiles.SetColumnWidth(0,LVSCW_AUTOSIZE);

	m_listFiles.SetSelectionMark(m_nFiles);

	++m_nFiles;

	/*CWnd *pWnd = GetDlgItem(IDC_REMOVEFILE);
	if(!pWnd->IsWindowEnabled())
		pWnd->EnableWindow();*/

return true;
}

void CToTcharDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
if(m_iMinCx && m_iMinCy)
	{
	lpMMI->ptMinTrackSize.x = m_iMinCx;
	lpMMI->ptMinTrackSize.y = m_iMinCy;
	}

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CToTcharDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	HDWP hDwp;
	CWnd *pWnd;
	RECT rc,rcFrame;
	
	if(m_iOldCx && cx && m_iOldCx != cx)
		{
		hDwp = BeginDeferWindowPos(5);

		pWnd = GetDlgItem(IDC_ADDFOLDER);
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		rc.left  += cx-m_iOldCx;
		rc.right += rc.left;

		hDwp = DeferWindowPos(hDwp,pWnd->GetSafeHwnd(),NULL,rc.left,rc.top,0,0,SWP_NOZORDER|SWP_NOSIZE);

		pWnd = GetDlgItem(IDCANCEL);
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		rc.left  += cx-m_iOldCx;
		rc.right += rc.left;

		hDwp = DeferWindowPos(hDwp,pWnd->GetSafeHwnd(),NULL,rc.left,rc.top,0,0,SWP_NOZORDER|SWP_NOSIZE);

		this->m_listFiles.GetWindowRect(&rc);
		ScreenToClient(&rc);

		rc.right += cx-m_iOldCx;
		rc.right-= rc.left;
		rc.bottom-= rc.top;
		hDwp = DeferWindowPos(hDwp,this->m_listFiles.GetSafeHwnd(),NULL,0,0,rc.right,rc.bottom,SWP_NOZORDER|SWP_NOMOVE);

		GetClientRect(&rcFrame);

		pWnd = GetDlgItem(IDC_OPTIONS);
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		rc.left = (rcFrame.right-(rc.right-rc.left))/2;

		hDwp = DeferWindowPos(hDwp,pWnd->GetSafeHwnd(),NULL,rc.left,rc.top,0,0,SWP_NOZORDER|SWP_NOSIZE);

		pWnd = GetDlgItem(IDC_MYHELP);
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		rc.left = (rcFrame.right-(rc.right-rc.left))/2;

		hDwp = DeferWindowPos(hDwp,pWnd->GetSafeHwnd(),NULL,rc.left,rc.top,0,0,SWP_NOZORDER|SWP_NOSIZE);

		EndDeferWindowPos(hDwp);
		}

	if(m_iOldCy && cy && m_iOldCy != cy)
		{
		hDwp = BeginDeferWindowPos(4);

		this->m_listFiles.GetWindowRect(&rc);
		ScreenToClient(&rc);

		rc.bottom += cy-m_iOldCy;
		rc.bottom -= rc.top;
		rc.right  -= rc.left;

		hDwp = DeferWindowPos(hDwp,this->m_listFiles.GetSafeHwnd(),NULL,0,0,rc.right,rc.bottom,SWP_NOZORDER|SWP_NOMOVE);

		pWnd = GetDlgItem(IDC_MYHELP);
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		rc.top += cy-m_iOldCy;
		
		hDwp = DeferWindowPos(hDwp,pWnd->GetSafeHwnd(),NULL,rc.left,rc.top,0,0,SWP_NOZORDER|SWP_NOSIZE);

		pWnd = GetDlgItem(IDCANCEL);
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		rc.top += cy-m_iOldCy;
		
		hDwp = DeferWindowPos(hDwp,pWnd->GetSafeHwnd(),NULL,rc.left,rc.top,0,0,SWP_NOZORDER|SWP_NOSIZE);

		pWnd = GetDlgItem(IDOK);
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		rc.top += cy-m_iOldCy;
		
		hDwp = DeferWindowPos(hDwp,pWnd->GetSafeHwnd(),NULL,rc.left,rc.top,0,0,SWP_NOZORDER|SWP_NOSIZE);

		EndDeferWindowPos(hDwp);
		}

	m_iOldCx = cx;
	m_iOldCy = cy;
}

void CToTcharDlg::OnOptions() 
{
COptionsDlg dlg;

	ArrayToString(dlg.m_strExtensions,m_strExtensionArray);
	dlg.m_bSubFolders = m_bSubFolders;

	if(dlg.DoModal() == IDOK)
		{
		StringToArray(m_strExtensionArray,LPCTSTR(dlg.m_strExtensions));

		m_bSubFolders = dlg.m_bSubFolders;
		}
}

void CToTcharDlg::ArrayToString(CString &str, const CStringArray &strArray)
{
INT_PTR ipSize = strArray.GetSize();

	for(INT_PTR i=0; i != ipSize; ++i)
		{
		if(!i)
			str = strArray[0];
		else
			str += strArray[i];

		str += _T(";");
		}
}

void CToTcharDlg::StringToArray(CStringArray &strArray, LPCTSTR lpszString)
{
strArray.RemoveAll();

LPTSTR p = _tcstok((LPTSTR)lpszString,_T(";"));
	while(p)
		{
		strArray.Add(p);

		p = _tcstok(NULL,_T(";"));
		}
}

void CToTcharDlg::OnDestroy() 
{
RECT rc;
bool bMaximize = false;
LPCTSTR lpszSettings = _T("Settings");

	CString str;
	ArrayToString(str,m_strExtensionArray);
	AfxGetApp()->WriteProfileString(lpszSettings,"Extensions",LPCTSTR(str));

	if(!IsZoomed())
		{
		if(!IsIconic())
			{
			GetWindowRect(&rc);
			AfxGetApp()->WriteProfileBinary(lpszSettings,"Placement",(LPBYTE)&rc,sizeof(RECT));
			}
		}
	else
		bMaximize = true;

	AfxGetApp()->WriteProfileString(lpszSettings,"Maximize",bMaximize ? "TRUE":"FALSE");

	CDialog::OnDestroy();
}

void CToTcharDlg::OnDropFiles(HDROP hDrop)
{
	UINT uFiles = DragQueryFile(hDrop,-1,NULL,0);
	while(uFiles)
		{
		char szFile[MAX_PATH];

		if(DragQueryFile(hDrop,--uFiles,szFile,sizeof(szFile)))
			{
			if(GetFileAttributes(szFile) == FILE_ATTRIBUTE_DIRECTORY)
				AddFolder(szFile);
			else
				AddFile(szFile);
			}
		}

	DragFinish(hDrop);
}

void CToTcharDlg::OnMyhelp() 
{
TCHAR szFile[MAX_PATH];
LPCTSTR lpszCantShowHelp = "Kann HTML-Hilfe nicht ausführen!";

if(GetModuleFileName(NULL,szFile,MAX_PATH))
	{
	LPTSTR p = strrchr(szFile,'\\');
	if(p)
		{
		lstrcpy(++p,"ToTchar.chm");

		if(!::HtmlHelp(GetSafeHwnd(),szFile,HH_DISPLAY_TOPIC,0L))
			AfxMessageBox(lpszCantShowHelp);
		}
	else
		AfxMessageBox(lpszCantShowHelp);
	}
else
	AfxMessageBox(lpszCantShowHelp);
}

BOOL CToTcharDlg::OnEraseBkgnd(CDC* pDC)
{
	if (CDialog::OnEraseBkgnd(pDC))
	{
		//draw size grip
		CRect rc;
		static CRect rcOld;

		GetClientRect(&rc);

		if (rcOld != rc)
		{
			InvalidateRect(&rcOld, TRUE);
			rcOld = rc;
		}

		if (!(GetStyle() & WS_MAXIMIZE))
		{
			int size = GetSystemMetrics(SM_CXVSCROLL);

			rc.left = rc.right - size;
			rc.top = rc.bottom - size;

			pDC->DrawFrameControl(&rc, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);

		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return CDialog::OnEraseBkgnd(pDC);
}
