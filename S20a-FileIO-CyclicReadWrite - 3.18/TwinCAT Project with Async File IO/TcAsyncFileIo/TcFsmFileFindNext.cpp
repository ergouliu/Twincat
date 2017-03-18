///////////////////////////////////////////////////////////////////////////////
#include "TcPch.h"
#pragma hdrstop

#include "TcFsmFileFindNext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DEFINE_THIS_FILE()

///////////////////////////////////////////////////////////////////////////////
// CTcFsmFileFindNext
///////////////////////////////////////////////////////////////////////////////
CTcFsmFileFindNext::CTcFsmFileFindNext()
{
	m_spSrv = NULL;
	m_spFileAccessAsync = NULL;
	m_pFileFindData = NULL;
}

HRESULT CTcFsmFileFindNext::Init
(
	ITComObjectServer* ipSrv,
	ITcFileAccessAsync* ipFileAccessAsync,
	TcFileFindData* pFileFindData
)
{
	HRESULT hr = S_OK;
	hr = TTcFsmFile::Init(ipSrv, ipFileAccessAsync, FileFindNextError, FileFindNextFinished);
	if (SUCCEEDED(hr))
	{
		m_pFileFindData = pFileFindData;
		SetState(FileFindNextInit, S_OK);
	}
	return hr;
}

HRESULT CTcFsmFileFindNext::Eval(TcFileFindHandle hFileFind)
{
	HRESULT hr = S_OK;
	switch (GetState())
	{
	case FileFindNextError:
		break;
	case FileFindNextInit:
		hr = m_spFileAccessAsync->FileFindNext(hFileFind, m_pFileFindData); 
		if (FAILED_AND_NOT_PENDING(hr))
		{
			SetState(FileFindNextError, hr);
		}
		else
		{
			SetState(FileFindNextCheck);
		}
		break;
	case FileFindNextCheck:
		hr = m_spFileAccessAsync->Check();
		if (FAILED_AND_NOT_PENDING(hr))
		{
			SetState(FileFindNextError, hr);
		}
		else if (SUCCEEDED(hr))
		{
			SetState(FileFindNextFinished);
		}
		break;

	case FileFindNextFinished:
		break;
	}
	return hr;
}
