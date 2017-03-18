///////////////////////////////////////////////////////////////////////////////
// TcFileAsyncTestModule.cpp
#include "TcPch.h"
#pragma hdrstop

#include "TcFsmFileReader.h"
#include "TcTimeConversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DEFINE_THIS_FILE()

///////////////////////////////////////////////////////////////////////////////
// CTcFsmFileReader
///////////////////////////////////////////////////////////////////////////////

CTcFsmFileReader::CTcFsmFileReader(TcTraceLevel& TraceLevelMax)
: m_Trace(TraceLevelMax, m_spSrv)
{
	m_szFilename = NULL;
	m_hFileReader = 0;
	m_pFileReaderBuffer = NULL;
	m_nFileReaderBuffer = 0; 
	m_nBytesReadTotal = 0;
}

HRESULT CTcFsmFileReader::Init
(
	ITComObjectServer* ipSrv, 
	ITcFileAccessAsync* ipFileAccessAsync, 
	PCCH szFilename, 
	ULONG nOffset,
	PBYTE pBuffer, 
	ULONG nBuffer
)
{
	HRESULT hr = S_OK;
	hr = TTcFsmFile::Init(ipSrv, ipFileAccessAsync, FileReaderError, FileReaderFinished); 
	if (SUCCEEDED(hr))
	{
		m_szFilename = szFilename; 
		m_hFileReader = 0; 

		m_pFileReaderBuffer = pBuffer;
		m_nFileReaderBuffer = nBuffer;
		m_nOffset = nOffset;

		SetState(FileReaderOpen, S_OK);
	}
	return hr;
}

HRESULT CTcFsmFileReader::Eval()
{
	HRESULT hr = S_OK;

	switch (GetState())
	{
	case FileReaderError:
		break;
	case FileReaderOpen:
		if (!m_fsmFileOpen.IsActive())
		{
			hr = m_fsmFileOpen.Init(m_spSrv, m_spFileAccessAsync, m_szFilename, amReadBinary);
			m_Trace.Log(tlVerbose, FLEAVEA "Opening file %s", m_szFilename);
			if (FAILED(hr))
			{
				SetState(FileReaderError, hr);
			}
		}
		
		if (m_fsmFileOpen.IsActive())
		{
			hr = m_fsmFileOpen.Eval();
			if (FAILED_AND_NOT_PENDING(hr))
			{
				m_Trace.Log(tlVerbose, FLEAVEA "Failed: Opening file %s", m_szFilename);
				SetState(FileReaderError, hr);
			}
			else if (SUCCEEDED(hr))
			{
				m_hFileReader = m_fsmFileOpen.GetFileHandle();
				m_Trace.Log(tlVerbose, FLEAVEA "Done: Opened file %s", m_szFilename);
  				SetState(FileReaderSeek);
			}
		}
		break;
	case FileReaderSeek:
		if (!m_fsmFileSeek.IsActive())
		{
			hr = m_fsmFileSeek.Init(m_spSrv, m_spFileAccessAsync, m_hFileReader, m_nOffset);
			if (FAILED(hr))
			{
				m_Trace.Log(tlVerbose, FLEAVEA "Seeking");
				SetState(FileReaderError, hr);
			}
		}
		
		if (m_fsmFileSeek.IsActive())
		{
			hr = m_fsmFileSeek.Eval();
			if (FAILED_AND_NOT_PENDING(hr))
			{
				m_Trace.Log(tlVerbose, FLEAVEA "Failed: Seeked");
				SetState(FileReaderError, hr);
			}
			else if (SUCCEEDED(hr))
			{
				m_Trace.Log(tlVerbose, FLEAVEA "Done: Seeked");
				SetState(FileReaderRead);
			}
		}
		break;
	case FileReaderRead:
		if (!m_fsmFileRead.IsActive())
		{
			hr = m_fsmFileRead.Init(m_spSrv, m_spFileAccessAsync, m_hFileReader, m_pFileReaderBuffer, m_nFileReaderBuffer);
			m_Trace.Log(tlVerbose, FLEAVEA "Reading");
			if (FAILED(hr))
			{
				m_Trace.Log(tlVerbose, FLEAVEA "Failed: Reading");
				SetState(FileReaderErrorCleanup, hr); 
			}
		}
		
		if (m_fsmFileRead.IsActive())
		{
			hr = m_fsmFileRead.Eval();
			if (FAILED_AND_NOT_PENDING(hr))
			{
				m_Trace.Log(tlVerbose, FLEAVEA "Failed: Reading");
				SetState(FileReaderErrorCleanup, hr); 
			}
			else if (SUCCEEDED(hr))
			{
				m_nBytesReadTotal+=m_fsmFileRead.GetFileReadActually();
				m_Trace.Log(tlVerbose, FLEAVEA "Done: Reading");
				SetState(FileReaderClose); 
			}
		}
		break;
	case FileReaderClose:
		if (m_hFileReader == 0)
		{
			SetState(FileReaderFinished); 
			m_Trace.Log(tlVerbose, FLEAVEA "Done: No close required");
		}
		else
		{
			if (!m_fsmFileClose.IsActive())
			{
				hr = m_fsmFileClose.Init(m_spSrv, m_spFileAccessAsync, m_hFileReader);
				m_Trace.Log(tlVerbose, FLEAVEA "Closing file");
				if (FAILED(hr))
				{
					m_Trace.Log(tlVerbose, FLEAVEA "Failed: Closing");
					SetState(FileReaderError, hr);
				}
			}
			
			if (m_fsmFileClose.IsActive())
			{
				hr = m_fsmFileClose.Eval();
				if (FAILED_AND_NOT_PENDING(hr))
				{
					m_Trace.Log(tlVerbose, FLEAVEA "Failed: Closing");
					SetState(FileReaderError, hr);
				}
				else if (SUCCEEDED(hr))
				{
					m_hFileReader = 0;
					SetState(FileReaderFinished);
					m_Trace.Log(tlVerbose, FLEAVEA "Done: Closed file");
				}
			}
		}
		break;
	case FileReaderErrorCleanup:
		if (!m_fsmFileClose.IsActive())
		{
			hr = m_fsmFileClose.Init(m_spSrv, m_spFileAccessAsync, m_hFileReader);
			m_Trace.Log(tlVerbose, FLEAVEA "Closing file (error cleanup)");
			if (FAILED(hr))
			{
				SetState(FileReaderError, hr);
				m_Trace.Log(tlVerbose, FLEAVEA "Failed: Closing file (error cleanup)");
			}
		}
		
		if (m_fsmFileClose.IsActive())
		{
			hr = m_fsmFileClose.Eval();
			if (FAILED_AND_NOT_PENDING(hr))
			{
				m_Trace.Log(tlVerbose, FLEAVEA "Failed: Closing file (error cleanup)");
				SetState(FileReaderError, hr);
			}
			else if (SUCCEEDED(hr))
			{
				m_Trace.Log(tlVerbose, FLEAVEA "Done: Closing file (error cleanup)");
				m_hFileReader = 0;
				SetState(FileReaderError);
			}
		}
		break;
	case FileReaderFinished:
		break; 
	}

	return hr;
}
