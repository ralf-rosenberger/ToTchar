// Engine.h: Schnittstelle für die Klasse CEngine.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINE_H__09D9ED2C_2D32_11D6_9E81_F3B3A6C73B51__INCLUDED_)
#define AFX_ENGINE_H__09D9ED2C_2D32_11D6_9E81_F3B3A6C73B51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define ISBEGINTRUE(i)	(  i == ' ' || i == '\t'	\
				        || i == '\r'|| i == '\n'	\
						|| i == '(' || i == ')'		\
						|| i == ';' || i == ','		\
						|| i == '{' || i == '}'		\
						|| i == '='	|| i == ':'		\
						|| i == '<' || i == '>'		\
						|| i == '+' || i == '-'		\
						|| i == '*' || i == '/'		\
						|| i == '.' || i == '"'		\
						|| i == '\0')

#define ISENDTRUE(i)	(  i == ' ' || i == '\t'	\
				        || i == '\r'|| i == '\n'	\
						|| i == ')' || i == '('		\
						|| i == ';' || i == ','		\
						|| i == '}' || i == '{'		\
						|| i == '='	|| i == ':'		\
						|| i == '<' || i == '>'		\
						|| i == '+' || i == '-'		\
						|| i == '*' || i == '/'		\
						|| i == '.' || i == '"'		\
						|| i == '\0')

class CEngine  
{
public:
	bool m_bErr;
	CWinThread* m_pThread;
	bool DoWork(void);
	void Close(void);
	bool SetFile(LPCTSTR lpszFile,bool bBak = true);
	bool Work(CWnd *pWnd);
	CEngine(LPCTSTR lpszFile = NULL,bool bBak = true);
	virtual ~CEngine();

private:
	bool IsInComment(LPCTSTR p);
	void GetKeywords(void);
	CStringArray m_strReplaceArray;
	CStringArray m_strKeywordsArray;
	bool ReplaceKeyword(LPCTSTR lpszKeyword,LPCTSTR lpszReplace);
	bool IsInclude(LPCTSTR p,int& iLineCount);
	void SkipComment(LPCTSTR *ppBuf,LPTSTR *ppTmpBuf = NULL);
	void SkipLine(LPCTSTR *ppBuf,LPTSTR *ppTmpBuf = NULL);
	void SkipSpace(LPCTSTR *ppBuf,LPTSTR *ppTmpBuf = NULL);
	bool IsExternC(LPCTSTR lpszBuf);
	bool IsUnicodeMacro(LPCTSTR lpszBuf);
	HANDLE m_heap;
	CFile fileTmp;
	CFile fileSource;
};

#endif // !defined(AFX_ENGINE_H__09D9ED2C_2D32_11D6_9E81_F3B3A6C73B51__INCLUDED_)
