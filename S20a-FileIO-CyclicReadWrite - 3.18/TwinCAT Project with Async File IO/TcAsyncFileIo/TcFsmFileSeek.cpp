///////////////////////////////////////////////////////////////////////////////
// TcFileAsyncTestModule.cpp
#include "TcPch.h"
#pragma hdrstop

#include "TcFsmFileSeek.h"
#include "TcTimeConversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DEFINE_THIS_FILE()

///////////////////////////////////////////////////////////////////////////////
// CTcFsmFileSeek
///////////////////////////////////////////////////////////////////////////////

CTcFsmFileSeek::CTcFsmFileSeek()
{
	m_hFileSeek = 0;
	m_nFileSeekOffset = 0;
}

HRESULT CTcFsmFileSeek::Init
(
	ITComObjectServer* ipSrv, 
	ITcFileAccessAsync* ipFileAccessAsync,
	TcFileHandle hFileSeek, 
	UINT nFileSeekOffset
)
{
	HRESULT hr = S_OK;
	hr = TTcFsmFile::Init(ipSrv, ipFileAccessAsync, FileSeekError, FileSeekFinished); 
	m_hFileSeek = hFileSeek;
	m_nFileSeekOffset = nFileSeekOffset;
	SetState(FileSeekInit, S_OK);
	return hr;
}

HRESULT CTcFsmFileSeek::Eval()
{
	HRESULT hr = S_OK;
	switch(GetState())
	{
	case FileSeekError:
		break;
	case FileSeekInit:
		hr = 
			m_spFileAccessAsync->FileSeek
			(
				m_hFileSeek, 
				m_nFileSeekOffset
			);
		if (FAILED_AND_NOT_PENDING(hr))
		{
			SetState(FileSeekError, hr);
		}
		else
		{
			SetState(FileSeekCheck);
		}
		break;
	case FileSeekCheck:
		hr = m_spFileAccessAsync->Check();
		if (FAILED_AND_NOT_PENDING(hr))
		{
			SetState(FileSeekError, hr);
		}
		else if (SUCCEEDED(hr))
		{
			SetState(FileSeekFinished);
		}
		break;
	case FileSeekFinished:
		break;
	}
	return hr;
}
