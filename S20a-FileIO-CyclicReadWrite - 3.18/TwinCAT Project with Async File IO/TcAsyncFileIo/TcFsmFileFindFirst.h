#pragma once

#include "TcFileAccessInterfaces.h"
#include "TcFsmFile.h"

enum eFileFindFirstState
{
	FileFindFirstError,
	FileFindFirstInit,
	FileFindFirstCheck,
	FileFindFirstFinished
};

class CTcFsmFileFindFirst : public TTcFsmFile<eFileFindFirstState>
{
public:
	CTcFsmFileFindFirst();
	HRESULT Init
	(
		ITComObjectServer* ipSrv,
		ITcFileAccessAsync* ipFileAccessAsync,
		PCCH szPath,
		TcFileFindData* pFileFindData,
		TcFileFindHandle* phFileFind
	);
	HRESULT Eval();

	PCCH GetPath();

private:
	PCCH m_szPath;
	TcFileFindData* m_pFileFindData;
	TcFileFindHandle* m_phFileFind;
};