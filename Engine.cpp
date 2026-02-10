// Engine.cpp: Implementierung der Klasse CEngine.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Engine.h"
#include <cstring>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

UINT WorkerThread(LPVOID pVoid)
	{
	CEngine *pEngine = (CEngine*)pVoid;

	pEngine->m_bErr = pEngine->DoWork();

	//Sleep(60000L);

	return 0xFFFF;
	}

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CEngine::CEngine(LPCTSTR lpszFile,bool bBak)
{
	m_heap = HeapCreate(HEAP_NO_SERIALIZE,0L,0L);
	if(!m_heap)
		AfxThrowMemoryException();

	if(lpszFile)
		SetFile(lpszFile,bBak);

	GetKeywords();
}

CEngine::~CEngine()
{
	CString strTmp = fileTmp.GetFilePath();
	DeleteFile(LPCTSTR(strTmp));

	HeapDestroy(m_heap);
}

bool CEngine::Work(CWnd *pWnd)
{
	m_pThread = AfxBeginThread((AFX_THREADPROC)WorkerThread,this);
	if(!m_pThread->m_hThread)
		return false;

	while(WaitForSingleObject(m_pThread->m_hThread,150L) == WAIT_TIMEOUT)
		{
		if(pWnd)
			pWnd->UpdateWindow();
		}

return m_bErr;
}

bool CEngine::IsUnicodeMacro(LPCTSTR lpszBuf)
{
LPCTSTR p;

//if("Hicon"_T("Test"))

	if(*(lpszBuf-1) == 'L' && ISBEGINTRUE(*(lpszBuf-2)))
		return true;

	while(*(--lpszBuf) == ' ' || *lpszBuf == '\r' || *lpszBuf == '\n' || *lpszBuf == '\t' || *lpszBuf == '\\');

	p = lpszBuf;

	if(*lpszBuf == '(')
		{
		while(*(--lpszBuf) == ' ' || *lpszBuf == '\r' || *lpszBuf == '\n' || *lpszBuf == '\t' || *lpszBuf == '\\');

		p = lpszBuf;

		if(*p == 'T' && *(p-1) == '_')
			{
			if(ISBEGINTRUE(*(p-2)))
				return true;
			else if(*(p-2) == '_')
				{
				if(ISBEGINTRUE(*(p-3)))
					return true;
				else if(*(p-3) == '_' && ISBEGINTRUE(*(p-4)))
					return true;
				}
			}

		p = lpszBuf;

		if(*p == 'T' && *(p-1) == 'X' && *(p-2) == 'E' && *(p-3) == 'T')
			{
			if(ISBEGINTRUE(*(p-4)))
				return true;
			else if(*(p-4) == '_')
				{
				if(ISBEGINTRUE(*(p-5)))
					return true;
				else if(*(p-5) == '_' && ISBEGINTRUE(*(p-6)))
					return true;
				}
			}
		}

return false;
}

bool CEngine::IsExternC(LPCTSTR p)
{
	if(*(p+1) == 'C' && *(p+2) == '"')
		{
		while(*(--p) == ' ' || *p == '\r' || *p == '\n' || *p == '\t' || *p == '\\');

		if(*p == 'n' && *(p-1) == 'r' && *(p-2) == 'e'
		&& *(p-3) == 't' && *(p-4) == 'x' && *(p-5) == 'e'
		&& ISBEGINTRUE(*(p-6)))
			return true;
		}

return false;
}

void CEngine::SkipLine(LPCTSTR *ppBuf, LPTSTR *ppTmpBuf)
{
	do
		{
		if(ppTmpBuf)
			*(*ppTmpBuf)++ = *(*ppBuf)++;
		else
			++(*ppBuf);

		}while(*(*ppBuf) != '\0' && *(*ppBuf) != '\r' && *(*ppBuf) != '\n');

	while(*(*ppBuf) == '\r' || *(*ppBuf) == '\n')
		{
		if(ppTmpBuf)
			*(*ppTmpBuf)++ = *(*ppBuf)++;
		else
			++(*ppBuf);
		}
}

void CEngine::SkipComment(LPCTSTR *ppBuf, LPTSTR *ppTmpBuf)
{
	if(ppTmpBuf)
		*(*ppTmpBuf)++ = *(*ppBuf)++;
	else
		++(*ppBuf);

	do
		{
		if(ppTmpBuf)
			*(*ppTmpBuf)++ = *(*ppBuf)++;
		else
			++(*ppBuf);

		}while(*(*ppBuf) != '\0' && (*(*ppBuf) != '*' && *((*ppBuf+1)) != '/'));

	if(ppTmpBuf)
		{
		*(*ppTmpBuf)++ = *(*ppBuf)++;
		*(*ppTmpBuf)++ = *(*ppBuf)++;
		}
	else
		*ppBuf += 2;
}

void CEngine::SkipSpace(LPCTSTR *ppBuf, LPTSTR *ppTmpBuf)
{
	do
		{
		if(ppTmpBuf)
			*(*ppTmpBuf)++ = *(*ppBuf)++;
		else
			++(*ppBuf);

		}while(*(*ppBuf) != '\0' && *(*ppBuf) == ' ');
}

bool CEngine::IsInclude(LPCTSTR p, int &iLineCount)
{
iLineCount = 1;

	while((*(++p) == ' ' || *p == '\t') && (*p != '\\' && *p != '\0'));

	while(*p != '\0' && *p == '\\')
		{
		++iLineCount;
		while((*(++p) == '\r' || *p == '\n' || *p == ' ' || *p == '\t') && *p != '\\');
		}

	if
	(
	*p == 'i' 
	&& *(p+1) == 'n' && *(p+2) == 'c' && *(p+3) == 'l'
    && *(p+4) == 'u' && *(p+5) == 'd' && *(p+6) == 'e'
	&& ISENDTRUE(*(p+7))
	)
		return true;

return false;
}

bool CEngine::ReplaceKeyword(LPCTSTR lpszKeyword, LPCTSTR lpszReplace)
{
LPTSTR lpszBuf,lpszTmpBuf,p,pBuf,pTmp;
DWORD dwFileSize;
UINT uReplaceLen = (UINT)lstrlen(lpszReplace);
int iLen;
bool bErr = false;

	lpszBuf = lpszTmpBuf = p = pTmp = pBuf = NULL;

	fileSource.SeekToBegin();
	fileTmp.SeekToBegin();

	fileTmp.SetLength(0L);

	dwFileSize = fileSource.GetLength();

	lpszBuf = (LPTSTR)HeapAlloc(m_heap,HEAP_ZERO_MEMORY,dwFileSize+2L);
	if(!lpszBuf)
		AfxThrowMemoryException();

	lpszTmpBuf = (LPTSTR)HeapAlloc(m_heap,HEAP_ZERO_MEMORY,dwFileSize);
	if(!lpszTmpBuf)
		AfxThrowMemoryException();

	fileSource.Read(lpszBuf+1,dwFileSize);

	*lpszBuf = lpszBuf[dwFileSize+1L] = '\0';
	pBuf = lpszBuf+1;

	pTmp = lpszTmpBuf;

	iLen = lstrlen(lpszKeyword);

	p = _tcsstr(pBuf,lpszKeyword);
	while(p)
		{
		if(!IsInComment(p))
			{
			if((*(p-1) == ' ' || *(p-1) == '\r' || *(p-1) == '\n' || *(p-1) == '\t' 
			|| *(p-1) == '(' || *(p-1) == ')' 
			|| *(p-1) == '{' || *(p-1) == '}' 
			|| *(p-1) == '[' || *(p-1) == ']'
			|| *(p-1) == ',' || *(p-1) == ';'
			|| *(p-1) == '+' || *(p-1) == '-' || *(p-1) == '*' || *(p-1) == '/') 
			&& (*(p+iLen) == ' ' || *(p+iLen) == '\r' || *(p+iLen) == '\n' || *(p+iLen) == '\t'
			|| *(p+iLen) == '(' || *(p+iLen) == ')' 
			|| *(p+iLen) == '{' || *(p+iLen) == '}' 
			|| *(p+iLen) == '[' || *(p+iLen) == ']'
			|| *(p+iLen) == ',' || *(p+iLen) == ';'
			|| *(p+iLen) == '+' || *(p+iLen) == '-' || *(p+iLen) == '*' || *(p+iLen) == '/'))
				{
				while(pBuf != p)
					*pTmp++ = *pBuf++;

				fileTmp.Write(lpszTmpBuf,(DWORD)(pTmp-lpszTmpBuf));

				fileTmp.Write(lpszReplace,uReplaceLen);

				*lpszTmpBuf = '\0';
				pTmp = lpszTmpBuf;

				p += iLen;
				pBuf = p;
				}
			else
				p += iLen;
			}
		else
			p += iLen;

		p = _tcsstr(p,lpszKeyword);
		}

	fileTmp.Write(pBuf,dwFileSize-(DWORD)(pBuf-lpszBuf)+1L);

	fileTmp.SeekToBegin();

	dwFileSize = fileTmp.GetLength();

	lpszBuf = (LPTSTR)HeapReAlloc(m_heap,HEAP_ZERO_MEMORY,lpszBuf,dwFileSize);
	if(!lpszBuf)
		AfxThrowMemoryException();

	fileTmp.Read(lpszBuf,dwFileSize);

	fileSource.SetLength(0L);

	fileSource.Write(lpszBuf,dwFileSize);

	HeapFree(m_heap,0L,lpszBuf);
	HeapFree(m_heap,0L,lpszTmpBuf);

return true;
}

void CEngine::GetKeywords()
{
CFile file;
TCHAR szPath[MAX_PATH];
DWORD dwFileSize;
LPTSTR lpszBuf,lpszKeyword;
register LPTSTR p,pKey;
LPCTSTR lpszFile = "Keywords.txt";
BYTE cSwitch = 0;

	if(!GetModuleFileName(NULL,szPath,MAX_PATH))
		AfxThrowFileException(CFileException::generic,0,lpszFile);

	p = strrchr(szPath,'\\');
	if(p)
		lstrcpy(++p,lpszFile);
	else
		AfxThrowFileException(CFileException::generic,0,lpszFile);

	file.Open(szPath,CFile::modeRead);

	lpszBuf = lpszKeyword = NULL;

	dwFileSize = file.GetLength();

	lpszBuf = (LPTSTR)HeapAlloc(m_heap,HEAP_ZERO_MEMORY,dwFileSize);
	if(!lpszBuf)
		AfxThrowMemoryException();

	lpszKeyword = (LPTSTR)HeapAlloc(m_heap,HEAP_ZERO_MEMORY,dwFileSize);
	if(!lpszKeyword)
		AfxThrowMemoryException();

	file.Read(lpszBuf,dwFileSize);

	lpszBuf[dwFileSize] = '\0';

	p = lpszBuf;
	pKey = lpszKeyword;
	while(*p != '\0')
		{
		if(*p == '\r' || *p == '\n')
			{
			if(pKey-lpszKeyword)
				{
				*pKey = '\0';

				if(!cSwitch)
					this->m_strKeywordsArray.Add(lpszKeyword);
				else
					this->m_strReplaceArray.Add(lpszKeyword);

				cSwitch ^= 1;

				*lpszKeyword = '\0';
				pKey = lpszKeyword;

				while(*p == '\r' || *p == '\n')
					++p;
				}
			}
		else if(*p == ';')
			SkipLine((LPCTSTR*)&p);
		else
			*pKey++ = *p++;
		}

	if(pKey-lpszKeyword)
		m_strReplaceArray.Add(lpszKeyword);

	p = lpszBuf;
	pKey = lpszKeyword;
}

bool CEngine::IsInComment(LPCTSTR p)
{
bool bNewLine = false;
bool bCommentEnd = false;

	while(*(--p) != '\0')
		{
		if(*p == '\n')
			bNewLine = true;

		if(*p == '/' && *(p-1) == '*')
			return false;
		else if(!bNewLine && *p == '/' && *(p-1) == '/')
			return true;
		else if(*p == '*' && *(p-1) == '/')
			return true;
		}

return false;
}

bool CEngine::SetFile(LPCTSTR lpszFile, bool bBak)
{
	if(bBak)
		{
		CString strErr;

		CString strBak = lpszFile;
		strBak += ".bak";

		if(!CopyFile(lpszFile,LPCTSTR(strBak),TRUE))
			{
			DWORD dwErr = GetLastError();

			if(dwErr == ERROR_FILE_EXISTS)
				{
				strErr.Format("Eine Backup-Datei mit dem Namen\n\"%s\"\nexistiert bereits!\n"
				              "Soll diese überschrieben werden?",LPCTSTR(strBak));

				if(AfxMessageBox(LPCTSTR(strErr),MB_ICONEXCLAMATION|MB_YESNO) == IDYES)
					{
					if(!CopyFile(lpszFile,LPCTSTR(strBak),FALSE))
						{
						strErr.Format("Kann Backup-Datei für die Datei\n\"%s\"\nnicht anlegen!\n"
							          "Soll der Vorgang abgebrochen werden?",lpszFile);

						if(AfxMessageBox(LPCTSTR(strErr),MB_ICONEXCLAMATION|MB_YESNO) == IDYES)
							return false;
						}
					}
				else
					return false;
				}
			else
				{
				if(!CopyFile(lpszFile,LPCTSTR(strBak),FALSE))
					{
					strErr.Format("Kann Backup-Datei für die Datei\n\"%s\"\nnicht anlegen!\n"
								  "Soll der Vorgang abgebrochen werden?",lpszFile);

					if(AfxMessageBox(LPCTSTR(strErr),MB_ICONEXCLAMATION|MB_YESNO) == IDYES)
						return false;
					}
				}
			}
		}

	fileSource.Open(lpszFile,CFile::modeReadWrite);

	TCHAR szTmpFile[MAX_PATH];

	lstrcpyn(szTmpFile,lpszFile,MAX_PATH);

	LPTSTR p = strrchr(szTmpFile,'.');
	if(p)
		{
		lstrcpy(++p,"tmp");
		fileTmp.Open(szTmpFile,CFile::modeCreate|CFile::modeReadWrite);
		}
	else
		AfxThrowFileException(CFileException::generic);

return true;
}

void CEngine::Close()
{
CString strTmp = fileTmp.GetFilePath();

	fileSource.Close();
	fileTmp.Close();

	DeleteFile(LPCTSTR(strTmp));
}

bool CEngine::DoWork()
{
DWORD dwFileSize = fileSource.GetLength();
LPTSTR lpszBuf,lpszTmpBuf;
register LPTSTR p,pTmp;

	lpszBuf = (LPTSTR)HeapAlloc(m_heap,HEAP_ZERO_MEMORY,dwFileSize+2L);
	if(!lpszBuf)
		AfxThrowMemoryException();

	lpszTmpBuf = (LPTSTR)HeapAlloc(m_heap,HEAP_ZERO_MEMORY,dwFileSize);
	if(!lpszTmpBuf)
		AfxThrowMemoryException();

	pTmp = lpszTmpBuf;

	fileSource.Read(lpszBuf+1,dwFileSize);

	*lpszBuf = lpszBuf[dwFileSize+1L] = '\0';

	p = lpszBuf+1;

	bool bStringEnd = true;
	int iLines;
	while(*p != '\0')
		{
		if(*p == '/' && *(p+1) == '/')
			{
			SkipLine((LPCTSTR*)&p,&pTmp);
			continue;
			}
		else if(*p == '/' && *(p+1) == '*')
			{
			SkipComment((LPCTSTR*)&p,&pTmp);
			continue;
			}
		if(*p == '#' && IsInclude(p,iLines))
			{
			while(iLines)
				{
				SkipLine((LPCTSTR*)&p,&pTmp);
				--iLines;
				}
			}
		else if(*p == '"' && *(p-1) == '\\' && *(p-2) != '\\')
			{
			*pTmp++ = *p++;
			continue;
			}
		else if(*p == '"')
			{
			if(bStringEnd)
				{
				if(IsUnicodeMacro(p))
					{
					*pTmp++ = *p++;

					while(*p != '\0')
						{
						if(*p == '"')
							{
							if(*(p-1) == '\\')
								{
								if(*(p-2) == '\\')
									break;
								}
							else
								break;
							}

						*pTmp++ = *p++;
						}

					if(*p != '\0')
						*pTmp++ = *p++;

					continue;
					}
				else if(IsExternC(p))
					{
					*pTmp++ = *p++;
					*pTmp++ = *p++;
					*pTmp++ = *p++;

					continue;
					}
				else
					{
					bStringEnd = false;

					fileTmp.Write(lpszTmpBuf,(DWORD)(pTmp-lpszTmpBuf));

					fileTmp.Write("_T(\"",4*sizeof(TCHAR));

					//ZeroMemory(lpszTmpBuf,dwFileSize);
					*lpszTmpBuf = '\0';
					pTmp = lpszTmpBuf;

					++p;
					} 
				}
			else
				{
				bStringEnd = true;

				fileTmp.Write(lpszTmpBuf,(DWORD)(pTmp-lpszTmpBuf));

				fileTmp.Write("\")",2*sizeof(TCHAR));

				*lpszTmpBuf = '\0';
				pTmp = lpszTmpBuf;

				++p;
				}
			}
		else if(bStringEnd && *p == 0x27 && *(p+2) == 0x27
			 || *(p+2) == '\\' && *(p+3) == 0x27
			 || *(p+1) == '\\' && *(p+2) == '0' && *(p+3) == 0x27)
			{
			if(IsUnicodeMacro(p))
				{
				*pTmp++ = *p++;
				*pTmp++ = *p++;
				*pTmp++ = *p++;

				continue;
				}

			fileTmp.Write(lpszTmpBuf,(DWORD)(pTmp-lpszTmpBuf));

			fileTmp.Write("_T('",4*sizeof(TCHAR));

			++p;

			fileTmp.Write((const void*)p,sizeof(TCHAR));

			if(*p == '\\')
				{
				++p;

				fileTmp.Write((const void*)p,sizeof(TCHAR));
				}

			fileTmp.Write("')",2*sizeof(TCHAR));

			p += 2;

			*lpszTmpBuf = '\0';
			pTmp = lpszTmpBuf;
			}
		else
			*pTmp++ = *p++;
		}

	fileTmp.Write(lpszTmpBuf,(DWORD)(pTmp-lpszTmpBuf));

	fileTmp.SeekToBegin();

	dwFileSize = fileTmp.GetLength();

	lpszBuf = (LPTSTR)HeapReAlloc(m_heap,HEAP_ZERO_MEMORY,lpszBuf,dwFileSize);
	if(!lpszBuf)
		AfxThrowMemoryException();

	fileTmp.Read(lpszBuf,dwFileSize);

	fileSource.SetLength(0L);

	fileSource.Write(lpszBuf,dwFileSize);

	for(register int i=0; i != m_strKeywordsArray.GetSize(); ++i)
		{
		/*AfxMessageBox(this->m_strKeywordsArray[i]);
		AfxMessageBox(this->m_strReplaceArray[i]);*/
		ReplaceKeyword(m_strKeywordsArray[i],m_strReplaceArray[i]);
		}

	HeapFree(m_heap,0L,lpszBuf);
	HeapFree(m_heap,0L,lpszTmpBuf);

return true;
}
