///////////////////////////////////////////////////////////////////////////////
// TcFileAsyncTestModule.cpp
#include "TcPch.h"
#pragma hdrstop

#include "TcFsmFileGetStatus.h"
#include "TcTimeConversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DEFINE_THIS_FILE()

///////////////////////////////////////////////////////////////////////////////
// CTcFsmFileGetStatus
///////////////////////////////////////////////////////////////////////////////

CTcFsmFileGetStatus::CTcFsmFileGetStatus()
{
	m_spSrv = NULL;
	m_spFileAccessAsync = NULL;
	m_szFileGetStatusName = NULL;
	memset(&m_FileStatus, 0, sizeof(m_FileStatus));
}

HRESULT CTcFsmFileGetStatus::Init
(
	ITComObjectServer* ipSrv, 
	ITcFileAccessAsync* ipFileAccessAsync,
	PCCH szFileName
)
{
	HRESULT hr = S_OK;
	hr = TTcFsmFile::Init(ipSrv, ipFileAccessAsync, FileGetStatusError, FileGetStatusFinished); 
	memset(&m_FileStatus, 0, sizeof(m_FileStatus)); 
	m_szFileGetStatusName = szFileName;
	SetState(FileGetStatusInit, S_OK);
	return hr;
}

HRESULT CTcFsmFileGetStatus::Eval()
{
	HRESULT hr = S_OK;
	switch(GetState())
	{
	case FileGetStatusError:
		break;
	case FileGetStatusInit:
		hr = m_spFileAccessAsync->FileGetStatus(m_szFileGetStatusName, &m_FileStatus); 
		if (FAILED_AND_NOT_PENDING(hr))
		{
			SetState(FileGetStatusError, hr);
		}
		else
		{
			SetState(FileGetStatusCheck);
		}
		break;
	case FileGetStatusCheck:
		hr = m_spFileAccessAsync->Check(); 
		if (FAILED_AND_NOT_PENDING(hr))
		{
			SetState(FileGetStatusError, hr);
		}
		else if (SUCCEEDED(hr))
		{
			SYSTEMTIME stCreateTime;
			SYSTEMTIME stModifiedTime;
			SYSTEMTIME stReadTime;

			TcFileTimeToSystemTime(m_FileStatus.ulCreateTime, &stCreateTime); 
			TcFileTimeToSystemTime(m_FileStatus.ulModifiedTime, &stModifiedTime); 
			TcFileTimeToSystemTime(m_FileStatus.ulReadTime, &stReadTime); 

			m_spSrv->TcReportLog(ADSLOG_MSGTYPE_LOG | ADSLOG_MSGTYPE_STRING, FNAMEA "CreateTime=" SYSTEMTIME_FMTSTRA, SYSTEMTIME_PARAMS(stCreateTime)); 
			m_spSrv->TcReportLog(ADSLOG_MSGTYPE_LOG | ADSLOG_MSGTYPE_STRING, FNAMEA "ModifiedTime=" SYSTEMTIME_FMTSTRA, SYSTEMTIME_PARAMS(stModifiedTime)); 
			m_spSrv->TcReportLog(ADSLOG_MSGTYPE_LOG | ADSLOG_MSGTYPE_STRING, FNAMEA "ReadTime=" SYSTEMTIME_FMTSTRA, SYSTEMTIME_PARAMS(stReadTime)); 

			SetState(FileGetStatusFinished, hr); 
		}
		break;
	case FileGetStatusFinished:
		break;
	}
	return hr;
}
