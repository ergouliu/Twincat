#pragma once

#include "TcFileAccessInterfaces.h"
#include "TcFsmFile.h"
#include "TcFsmFileFindFirst.h"
#include "TcFsmFileFindNext.h"
#include "TcFsmFileFindClose.h"

enum eFileFindState
{
	FileFindError,
	FileFindFirst,
	FileFindNext,
	FileFindClose,
	FileFindFinished
};

class CTcFsmFileFind : public TTcFsmFile<eFileFindState>
{
public:
	CTcFsmFileFind(TcTraceLevel& TraceLevelMax);
	HRESULT Init
	(
		ITComObjectServer* ipSrv,
		ITcFileAccessAsync* ipFileAccessAsync,
		PCCH szPath
	);
	HRESULT Eval();

protected:
	// Tracing
	CTcTrace m_Trace;

private:
	PCCH m_szPath;
	TcFileFindData   m_fileFindData;
	TcFileFindHandle m_hFileFind;

	CTcFsmFileFindFirst m_fileFindFirst;
	CTcFsmFileFindNext m_fileFindNext;
	CTcFsmFileFindClose m_fileFindClose;
};