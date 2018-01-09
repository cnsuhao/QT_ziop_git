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

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-2001 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

////////////////////////////////////////////////////////////////////////////
// CFileFinderer implementation

#include "app/data_management/archive_manager/pmodel/src/FileFinder.h"
#include "core/utilities/src/TAAssert.h"
#include <mbstring.h>
#include <stdlib.h>

CFileFinder::CFileFinder()
{
	m_pFoundInfo = NULL;
	m_pNextInfo = NULL;
	m_hContext = NULL;
	m_chDirSeparator = '\\';
}

CFileFinder::~CFileFinder()
{
	Close();
}

void CFileFinder::Close()
{
	if (m_pFoundInfo != NULL)
	{
		delete m_pFoundInfo;
		m_pFoundInfo = NULL;
	}

	if (m_pNextInfo != NULL)
	{
		delete m_pNextInfo;
		m_pNextInfo = NULL;
	}

	if (m_hContext != NULL && m_hContext != INVALID_HANDLE_VALUE)
	{
		CloseContext();
		m_hContext = NULL;
	}
}

void CFileFinder::CloseContext()
{
	::FindClose(m_hContext);
	return;
}

int CFileFinder::FindFile(LPCTSTR pstrName /* = NULL */,
	DWORD dwUnused /* = 0 */)
{
	//UNUSED_ALWAYS(dwUnused);
	Close();
	m_pNextInfo = new WIN32_FIND_DATA;

	if (pstrName == NULL)
		pstrName = ("*.*");

	WIN32_FIND_DATA *pFindData = (WIN32_FIND_DATA *)m_pNextInfo;

	lstrcpyn(pFindData->cFileName, pstrName,sizeof(pFindData->cFileName) );

	m_hContext = ::FindFirstFile(pstrName, (WIN32_FIND_DATA*) m_pNextInfo);

	if (m_hContext == INVALID_HANDLE_VALUE)
	{
		DWORD dwTemp = ::GetLastError();
		Close();
		::SetLastError(dwTemp);
		return FALSE;
	}

	//LPTSTR pstrRoot = m_strRoot.GetBufferSetLength(_MAX_PATH);
	char pstrRoot[MAX_PATH];	
	LPCTSTR pstr = _fullpath(pstrRoot, pstrName, _MAX_PATH);  //TODO test
	m_strRoot = pstrRoot;

	// passed name isn't a valid path but was found by the API
	TA_Assert( pstr != NULL);
	if (pstr == NULL)
	{
		//m_strRoot.ReleaseBuffer(-1);
		Close();
		::SetLastError(ERROR_INVALID_NAME);
		return FALSE;
	}
	else
	{
		TCHAR strDrive[_MAX_DRIVE], strDir[_MAX_DIR];
		_splitpath_s(pstrRoot, strDrive, _MAX_DRIVE, strDir, _MAX_DIR, NULL, 0, NULL, 0);
		_makepath_s(pstrRoot, _MAX_PATH, strDrive, strDir, NULL, NULL);
		m_strRoot = pstrRoot;
	}
	//m_strRoot.ReleaseBuffer(-1);
	

	return TRUE;
}

int CFileFinder::MatchesMask(DWORD dwMask) const
{
	TA_Assert(m_hContext != NULL);
	//ASSERT_VALID(this);

	if (m_pFoundInfo != NULL)
		return (!!(((LPWIN32_FIND_DATA) m_pFoundInfo)->dwFileAttributes & dwMask));
	else
		return FALSE;
}

int CFileFinder::IsDots() const
{
	TA_Assert(m_hContext != NULL);
	//ASSERT_VALID(this);

	// return TRUE if the file name is "." or ".." and
	// the file is a directory

	int bResult = FALSE;
	if (m_pFoundInfo != NULL && IsDirectory())
	{
		LPWIN32_FIND_DATA pFindData = (LPWIN32_FIND_DATA) m_pFoundInfo;
		if (pFindData->cFileName[0] == '.')
		{
			if (pFindData->cFileName[1] == '\0' ||
				(pFindData->cFileName[1] == '.' &&
				 pFindData->cFileName[2] == '\0'))
			{
				bResult = TRUE;
			}
		}
	}

	return bResult;
}

int CFileFinder::IsDirectory() const
{
	return MatchesMask(FILE_ATTRIBUTE_DIRECTORY);
}

int CFileFinder::FindNextFile()
{
	TA_Assert(m_hContext != NULL);

	if (m_hContext == NULL)
		return FALSE;
	if (m_pFoundInfo == NULL)
		m_pFoundInfo = new WIN32_FIND_DATA;

	//ASSERT_VALID(this);

	void* pTemp = m_pFoundInfo;
	m_pFoundInfo = m_pNextInfo;
	m_pNextInfo = pTemp;

	return ::FindNextFile(m_hContext, (LPWIN32_FIND_DATA) m_pNextInfo);
}

std::string CFileFinder::GetFileURL() const
{
	TA_Assert(m_hContext != NULL);
	//ASSERT_VALID(this);

	std::string strResult("file://");
	strResult += GetFilePath();
	return strResult;
}

std::string CFileFinder::GetRoot() const
{
	TA_Assert(m_hContext != NULL);
	//ASSERT_VALID(this);

	return m_strRoot;
}

std::string CFileFinder::GetFilePath() const
{
	TA_Assert(m_hContext != NULL);
	//ASSERT_VALID(this);

	std::string strResult = m_strRoot;
	char * pszResult;
	unsigned char * pchLast;
	pszResult = (char *) strResult.c_str();
	pchLast = _mbsdec(reinterpret_cast<unsigned char *>(pszResult), reinterpret_cast<unsigned char *>(pszResult+strResult.length()) );
	if ((*pchLast != ('\\')) && (*pchLast != ('/')))
		strResult += m_chDirSeparator; //TODO test
	strResult += GetFileName();
	return strResult;
}

std::string CFileFinder::GetFileTitle() const
{
	TA_Assert(m_hContext != NULL);
	//ASSERT_VALID(this);

	std::string strFullName = GetFileName();
	char * strResult;
	_splitpath(strFullName.c_str(), NULL, NULL, strResult, NULL);
	//strResult.ReleaseBuffer();
	return strResult;
}

std::string CFileFinder::GetFileName() const
{
	TA_Assert(m_hContext != NULL);
	//ASSERT_VALID(this);

	std::string ret;

	if (m_pFoundInfo != NULL)
		ret = ((LPWIN32_FIND_DATA) m_pFoundInfo)->cFileName;
	return ret;
}

ULONGLONG CFileFinder::GetLength() const
{
	TA_Assert(m_hContext != NULL);
	//ASSERT_VALID(this);

   ULARGE_INTEGER nFileSize;

	if (m_pFoundInfo != NULL)
   {
	  nFileSize.LowPart = ((LPWIN32_FIND_DATA) m_pFoundInfo)->nFileSizeLow;
	  nFileSize.HighPart = ((LPWIN32_FIND_DATA) m_pFoundInfo)->nFileSizeHigh;
   }
   else
   {
	  nFileSize.QuadPart = 0;
   }

   return nFileSize.QuadPart;
}



