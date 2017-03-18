///////////////////////////////////////////////////////////////////////////////
#include "TcPch.h"
#pragma hdrstop

#include "TcFsmFileFind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DEFINE_THIS_FILE()

///////////////////////////////////////////////////////////////////////////////
// CTcFsmFileFind
///////////////////////////////////////////////////////////////////////////////

CTcFsmFileFind::CTcFsmFileFind(TcTraceLevel& TraceLevelMax)
: m_Trace(TraceLevelMax, m_spSrv)
{
	m_spSrv = NULL;
	m_spFileAccessAsync = NULL;

}

HRESULT CTcFsmFileFind::Init
(
	ITComObjectServer* ipSrv,
	ITcFileAccessAsync* ipFileAccessAsync,
	PCCH szPath
)
{
	HRESULT hr = S_OK;
	hr = TTcFsmFile::Init(ipSrv, ipFileAccessAsync, FileFindError, FileFindFinished);
	if (SUCCEEDED(hr))
	{
		
		m_fileFindClose.Init(ipSrv, ipFileAccessAsync);
		m_szPath = szPath;
		memset(&m_fileFindData, 0, sizeof(m_fileFindData));
		m_hFileFind = 0;
		SetState(FileFindFirst, S_OK);
	}
	return hr;
}

HRESULT CTcFsmFileFind::Eval()
{
	HRESULT hr = S_OK;
	switch (GetState())
	{
	case FileFindError:
		break;
	case FileFindFirst:
		if (!m_fileFindFirst.IsActive())
		{
			hr = m_fileFindFirst.Init(m_spSrv, m_spFileAccessAsync, m_szPath, &m_fileFindData, &m_hFileFind);
			m_Trace.Log(tlVerbose, FNAMEA "First file init for %s, hr=0x%08x (%s)", m_szPath, hr, AdsGetHResultText(hr));
			if (FAILED(hr))
			{
				SetState(FileFindError, hr);
			}
		}

		if (m_fileFindFirst.IsActive())
		{
			hr = m_fileFindFirst.Eval();
			if (FAILED_AND_NOT_PENDING(hr))
			{
				m_Trace.Log(tlError, FNAMEA "Failed: First file init for %s, hr=0x%08x (%s)", m_szPath, hr, AdsGetHResultText(hr));
				SetState(FileFindError, hr);
			}
			else if (SUCCEEDED(hr))
			{
				m_Trace.Log(tlInfo, FNAMEA "First file in %s: %s", m_szPath, m_fileFindData.cFileName);
				SetState(FileFindNext);
			}
		}
		break;

	case FileFindNext:
		if (!m_fileFindNext.IsActive())
		{
			hr = m_fileFindNext.Init(m_spSrv, m_spFileAccessAsync, &m_fileFindData);
			m_Trace.Log(tlVerbose, FNAMEA "Next file init for %s, hr=0x%08x (%s)", m_szPath, hr, AdsGetHResultText(hr));
			if (FAILED(hr))
			{
				SetState(FileFindError, hr);
			}
		}

		if (m_fileFindNext.IsActive())
		{
			hr = m_fileFindNext.Eval(m_hFileFind); 
			if (FAILED_AND_NOT_PENDING(hr))
			{
				if (hr == ADS_E_NOTFOUND)
				{
					m_Trace.Log(tlInfo, FNAMEA "Next file not found for %s (0x%08x), hr=0x%08x (%s)",
						m_szPath, m_hFileFind, hr, AdsGetHResultText(hr));
					SetState(FileFindFinished);
				}
				else
				{
					m_Trace.Log(tlError, FNAMEA "Failed: Next file init for %s (0x%08x), hr=0x%08x (%s)",
						m_szPath, m_hFileFind, hr, AdsGetHResultText(hr));
					SetState(FileFindError, hr);
				}
			}
			else if (SUCCEEDED(hr))
			{
				m_Trace.Log(tlInfo, FNAMEA "Next file in %s: %s", m_szPath, m_fileFindData.cFileName);
				SetState(FileFindNext);
			}
		}
		break;

	case FileFindFinished:
		break;
	}
	return hr;
}
