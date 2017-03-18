#pragma once

#include "TcFileAccessInterfaces.h"
#include "TcFsmFile.h"

enum eFileFindNextState
{
	FileFindNextError,
	FileFindNextInit,
	FileFindNextCheck,
	FileFindNextFinished
};

class CTcFsmFileFindNext : public TTcFsmFile<eFileFindNextState>
{
public:
	CTcFsmFileFindNext();
	HRESULT Init
	(
		ITComObjectServer* ipSrv,
		ITcFileAccessAsync* ipFileAccessAsync,
		TcFileFindData* pFileFindData
	);

	HRESULT Eval(TcFileFindHandle hFileFind);

private:
	TcFileFindData* m_pFileFindData;
};