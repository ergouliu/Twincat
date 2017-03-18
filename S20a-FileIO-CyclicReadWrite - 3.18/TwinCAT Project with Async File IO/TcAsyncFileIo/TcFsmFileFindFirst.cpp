///////////////////////////////////////////////////////////////////////////////
#include "TcPch.h"
#pragma hdrstop

#include "TcFsmFileFindFirst.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DEFINE_THIS_FILE()

///////////////////////////////////////////////////////////////////////////////
// CTcFsmFileFindFirst
///////////////////////////////////////////////////////////////////////////////
CTcFsmFileFindFirst::CTcFsmFileFindFirst()
{
	m_spSrv = NULL;
	m_spFileAccessAsync = NULL;
	m_pFileFindData = NULL;
	m_phFileFind = NULL;
}

HRESULT CTcFsmFileFindFirst::Init
(
	ITComObjectServer* ipSrv,
	ITcFileAccessAsync* ipFileAccessAsync,
	PCCH szPath,
	TcFileFindData* pFileFindData,
	TcFileFindHandle* phFileFind
)
{
	HRESULT hr = S_OK;
	hr = TTcFsmFile::Init(ipSrv, ipFileAccessAsync, FileFindFirstError, FileFindFirstFinished);
	if (SUCCEEDED(hr))
	{
		m_szPath = szPath;
		m_pFileFindData = pFileFindData;
		m_phFileFind = phFileFind;
		SetState(FileFindFirstInit, S_OK);
	}
	return hr;
}

HRESULT CTcFsmFileFindFirst::Eval()
{
	HRESULT hr = S_OK;
	switch (GetState())
	{
	case FileFindFirstError:
		break;
	case FileFindFirstInit:
		hr = m_spFileAccessAsync->FileFindFirst(m_szPath, m_pFileFindData, m_phFileFind); 
		if (FAILED_AND_NOT_PENDING(hr))
		{
			SetState(FileFindFirstError, hr);
		}
		else
		{
			SetState(FileFindFirstCheck);
		}
		break;
	case FileFindFirstCheck:
		hr = m_spFileAccessAsync->Check();
		if (FAILED_AND_NOT_PENDING(hr))
		{
			SetState(FileFindFirstError, hr);
		}
		else if (SUCCEEDED(hr))
		{
			if (m_phFileFind != NULL)
			{
				*m_phFileFind = m_pFileFindData->hFile;
			}
			SetState(FileFindFirstFinished);
		}
		break;

	case FileFindFirstFinished:
		break;
	}
	return hr;
}
