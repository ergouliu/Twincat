#pragma once

#include "TcFileAccessInterfaces.h"
#include "TcFsmFile.h"

enum eFileGetStatusState
{
	FileGetStatusError,
	FileGetStatusInit,
	FileGetStatusCheck,
	FileGetStatusFinished
};

class CTcFsmFileGetStatus : public TTcFsmFile<eFileGetStatusState>
{
public: 
	CTcFsmFileGetStatus();
	HRESULT Init(ITComObjectServer* ipSrv, ITcFileAccessAsync* ipFileAccessAsync, PCCH szFileName);
	HRESULT Eval();

	// Output 
	const TcFileStatus& GetFileStatus() const { return m_FileStatus; }

private: 
	PCCH         m_szFileGetStatusName;
	TcFileStatus m_FileStatus;
};

