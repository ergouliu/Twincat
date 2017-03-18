///////////////////////////////////////////////////////////////////////////////
// TcFileAsyncTestModule.cpp
#include "TcPch.h"
#pragma hdrstop

#include "TcFsmFileRead.h"
#include "TcTimeConversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DEFINE_THIS_FILE()

///////////////////////////////////////////////////////////////////////////////
// CTcFsmFileRead
///////////////////////////////////////////////////////////////////////////////

CTcFsmFileRead::CTcFsmFileRead()
{
	m_hFileRead = 0;
	m_pFileReadBuffer = NULL;
	m_nFileReadBuffer = 0;
	m_nFileReadActually = 0;
}

HRESULT CTcFsmFileRead::Init
(
	ITComObjectServer* ipSrv, 
	ITcFileAccessAsync* ipFileAccessAsync,
	TcFileHandle hFileRead, 
	PBYTE pFileReadBuffer, 
	ULONG nFileReadBuffer
)
{
	HRESULT hr = S_OK;
	hr = TTcFsmFile::Init(ipSrv, ipFileAccessAsync, FileReadError, FileReadFinished); 
	m_hFileRead = hFileRead;
	m_pFileReadBuffer = pFileReadBuffer;
	m_nFileReadBuffer = nFileReadBuffer;
	m_nFileReadActually = 0;
	SetState(FileReadInit, S_OK);
	return hr;
}

HRESULT CTcFsmFileRead::Eval()
{
	HRESULT hr = S_OK;
	switch(GetState())
	{
	case FileReadError:
		break;
	case FileReadInit:
		hr = 
			m_spFileAccessAsync->FileRead
			(
				m_hFileRead, 
				m_pFileReadBuffer, 
				m_nFileReadBuffer, 
				&m_nFileReadActually
			);
		if (FAILED_AND_NOT_PENDING(hr))
		{
			SetState(FileReadError, hr);
		}
		else
		{
			SetState(FileReadCheck);
		}
		break;
	case FileReadCheck:
		hr = m_spFileAccessAsync->Check();
		if (FAILED_AND_NOT_PENDING(hr))
		{
			SetState(FileReadError, hr);
		}
		else if (SUCCEEDED(hr))
		{
			SetState(FileReadFinished);
		}
		break;
	case FileReadFinished:
		break;
	}
	return hr;
}
