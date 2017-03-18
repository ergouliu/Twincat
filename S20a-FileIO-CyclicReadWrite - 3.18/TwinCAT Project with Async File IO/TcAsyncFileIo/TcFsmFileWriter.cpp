///////////////////////////////////////////////////////////////////////////////
// TcFileAsyncTestModule.cpp
#include "TcPch.h"
#pragma hdrstop

#include "TcFsmFileWriter.h"
#include "TcTimeConversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DEFINE_THIS_FILE()

///////////////////////////////////////////////////////////////////////////////
// CTcFsmFileWriter
///////////////////////////////////////////////////////////////////////////////

CTcFsmFileWriter::CTcFsmFileWriter(TcTraceLevel& TraceLevelMax)
: m_Trace(TraceLevelMax, m_spSrv)
{
	m_szFilename = NULL;
	m_hFileWriter = 0;
	m_pFileWriterBuffer = NULL;
	m_nFileWriterBuffer = 0; 
	m_nBytesWrittenTotal = 0;
}

HRESULT CTcFsmFileWriter::Init
(
	ITComObjectServer* ipSrv, 
	ITcFileAccessAsync* ipFileAccessAsync, 
	PCCH szFilename, 
	PBYTE pBuffer, 
	ULONG nBuffer
)
{
	HRESULT hr = S_OK;
	hr = TTcFsmFile::Init(ipSrv, ipFileAccessAsync, FileWriterError, FileWriterFinished); 
	if (SUCCEEDED(hr))
	{
		m_szFilename = szFilename; 
		m_hFileWriter = 0; 

		m_pFileWriterBuffer = pBuffer;
		m_nFileWriterBuffer = nBuffer;

		SetState(FileWriterOpen, S_OK);
	}
	return hr;
}

HRESULT CTcFsmFileWriter::Eval()
{
	HRESULT hr = S_OK;

	switch (GetState())
	{
	case FileWriterError:
		hr = GetResult();
		break;
	case FileWriterOpen:
		if (!m_fsmFileOpen.IsActive())
		{
			hr = m_fsmFileOpen.Init(m_spSrv, m_spFileAccessAsync, m_szFilename, static_cast<TcFileAccessMode>(amBinary | amAppend));
			m_Trace.Log(tlVerbose, FLEAVEA "Opening file %s", m_szFilename);
			if (FAILED(hr))
			{
				SetState(FileWriterError, hr);
			}
		}
		
		if (m_fsmFileOpen.IsActive())
		{
			hr = m_fsmFileOpen.Eval();
			if (FAILED_AND_NOT_PENDING(hr))
			{
				m_Trace.Log(tlVerbose, FLEAVEA "Failed: Opening file %s", m_szFilename);
				SetState(FileWriterError, hr);
			}
			else if (SUCCEEDED(hr))
			{
				m_hFileWriter = m_fsmFileOpen.GetFileHandle();
				m_Trace.Log(tlVerbose, FLEAVEA "Done: Opened file %s", m_szFilename);
				SetState(FileWriterWrite);
			}
		}
		break;
	case FileWriterWrite:
		if (!m_fsmFileWrite.IsActive())
		{
			hr = m_fsmFileWrite.Init(m_spSrv, m_spFileAccessAsync, m_hFileWriter, m_pFileWriterBuffer, m_nFileWriterBuffer);
			m_Trace.Log(tlVerbose, FLEAVEA "Writing file");
			if (FAILED(hr))
			{
				m_Trace.Log(tlVerbose, FLEAVEA "Failed: Writing file");
				SetState(FileWriterErrorCleanup, hr); 
			}
		}

		if (m_fsmFileWrite.IsActive())
		{
			hr = m_fsmFileWrite.Eval();
			if (FAILED_AND_NOT_PENDING(hr))
			{
				SetState(FileWriterErrorCleanup, hr); 
				m_Trace.Log(tlVerbose, FLEAVEA "Failed: Writing file");
			}
			else if (SUCCEEDED(hr))
			{
				m_nBytesWrittenTotal+=m_fsmFileWrite.GetFileWriteActually();
				m_Trace.Log(tlVerbose, FLEAVEA "Done: Writing file");
				SetState(FileWriterClose); 
			}
		}
		break;
	case FileWriterClose:
		if (m_hFileWriter == 0)
		{
			SetState(FileWriterFinished); 
			m_Trace.Log(tlVerbose, FLEAVEA "Done: No close required");
		}
		else
		{
			if (!m_fsmFileClose.IsActive())
			{
				hr = m_fsmFileClose.Init(m_spSrv, m_spFileAccessAsync, m_hFileWriter);
				m_Trace.Log(tlVerbose, FLEAVEA "Closing file");
				if (FAILED(hr))
				{
					m_Trace.Log(tlVerbose, FLEAVEA "Failed: Closing");
					SetState(FileWriterError, hr);
				}
			}

			if (m_fsmFileClose.IsActive())
			{
				hr = m_fsmFileClose.Eval();
				if (FAILED_AND_NOT_PENDING(hr))
				{
					m_Trace.Log(tlVerbose, FLEAVEA "Failed: Closing");
					SetState(FileWriterError, hr);
				}
				else if (SUCCEEDED(hr))
				{
					m_Trace.Log(tlVerbose, FLEAVEA "Done: Closing");
					m_hFileWriter = 0;
					SetState(FileWriterFinished);
				}
			}
		}
		break;
	case FileWriterErrorCleanup:
		if (!m_fsmFileClose.IsActive())
		{
			hr = m_fsmFileClose.Init(m_spSrv, m_spFileAccessAsync, m_hFileWriter);
			m_Trace.Log(tlVerbose, FLEAVEA "Closing file (error cleanup)");
			if (FAILED(hr))
			{
				SetState(FileWriterError, hr);
				m_Trace.Log(tlVerbose, FLEAVEA "Failed: Closing file (error cleanup)");
			}
		}
		
		if (m_fsmFileClose.IsActive())
		{
			hr = m_fsmFileClose.Eval();
			if (FAILED_AND_NOT_PENDING(hr))
			{
				m_Trace.Log(tlVerbose, FLEAVEA "Failed: Closing file (error cleanup)");
				SetState(FileWriterError, hr);
			}
			else if (SUCCEEDED(hr))
			{
				m_Trace.Log(tlVerbose, FLEAVEA "Done: Closing file (error cleanup)");
				m_hFileWriter = 0;
				SetState(FileWriterError);
			}
		}
		break;
	case FileWriterFinished:
		hr = GetResult();
		break; 
	}

	return hr;
}
