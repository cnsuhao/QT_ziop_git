/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File:  $
  * @author:  
  * @version: $Revision:  $
  *
  * Last modification: $DateTime: $
  * Last modified by:  $Author:  $
  *
  *
  */

#if !defined(CFILEFINDER_H___3DA15816_CA7B_43CB_A66C_A735EBE85849__INCLUDED_)
#define CFILEFINDER_H___3DA15816_CA7B_43CB_A66C_A735EBE85849__INCLUDED_

#include <string>
#include <Windows.h>

typedef unsigned __int64 ULONGLONG;

class CFileFinder
{
public:
	CFileFinder();
	virtual ~CFileFinder();

// Attributes
public:
	ULONGLONG GetLength() const;
	virtual std::string GetFileName() const;
	virtual std::string GetFilePath() const;
	virtual std::string GetFileTitle() const;
	virtual std::string GetFileURL() const;
	virtual std::string GetRoot() const;

	virtual int MatchesMask(unsigned long  dwMask) const;

	virtual int IsDots() const;
	// these aren't virtual because they all use MatchesMask(), which is
	int IsReadOnly() const;
	int IsDirectory() const;
	int IsCompressed() const;
	int IsSystem() const;
	int IsHidden() const;
	int IsTemporary() const;
	int IsNormal() const;
	int IsArchived() const;

// Operations
	void Close();
	virtual int FindFile(const char* pstrName = NULL, unsigned long  dwUnused = 0);
	virtual int FindNextFile();

protected:
	virtual void CloseContext();

// Implementation
protected:
	void* m_pFoundInfo;
	void* m_pNextInfo;
	HANDLE m_hContext;
	std::string m_strRoot;
	char m_chDirSeparator;     // not '\\' for Internet classes

};

#endif //CFILEFINDER_H___3DA15816_CA7B_43CB_A66C_A735EBE85849__INCLUDED_