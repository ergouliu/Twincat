///////////////////////////////////////////////////////////////////////////////
#include "TcPch.h"
#pragma hdrstop

#include "TcFsmFileFindClose.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DEFINE_THIS_FILE()

///////////////////////////////////////////////////////////////////////////////
// CTcFsmFileFindClose
///////////////////////////////////////////////////////////////////////////////
CTcFsmFileFindClose::CTcFsmFileFindClose()
{
	m_spSrv = NULL;
	m_spFileAccessAsync = NULL;

}

HRESULT CTcFsmFileFindClose::Init
(
	ITComObjectServer* ipSrv,
	ITcFileAccessAsync* ipFileAccessAsync
)
{
	HRESULT hr = S_OK;
	hr = TTcFsmFile::Init(ipSrv, ipFileAccessAsync, FileFindCloseError, FileFindCloseFinished);
	if (SUCCEEDED(hr))
	{
		SetState(FileFindCloseInit, S_OK);
	}
	return hr;
}

HRESULT CTcFsmFileFindClose::Eval(TcFileFindHandle hFileFind)
{
	HRESULT hr = S_OK;
	switch (GetState())
	{
	case FileFindCloseError:
		break;
	case FileFindCloseInit:
		hr = m_spFileAccessAsync->FileFindClose(hFileFind);
		if (FAILED_AND_NOT_PENDING(hr))
		{
			SetState(FileFindCloseError, hr);
		}
		else
		{
			SetState(FileFindCloseCheck);
		}
		break;
	case FileFindCloseCheck:
		hr = m_spFileAccessAsync->Check();
		if (FAILED_AND_NOT_PENDING(hr))
		{
			SetState(FileFindCloseError, hr);
		}
		else if (SUCCEEDED(hr))
		{
			SetState(FileFindCloseFinished);
		}
		break;

	case FileFindCloseFinished:
		break;
	}
	return hr;
}
