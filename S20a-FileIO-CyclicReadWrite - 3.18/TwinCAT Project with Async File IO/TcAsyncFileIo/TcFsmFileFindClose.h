#pragma once

#include "TcFileAccessInterfaces.h"
#include "TcFsmFile.h"

enum eFileFindCloseState
{
	FileFindCloseError,
	FileFindCloseInit,
	FileFindCloseCheck,
	FileFindCloseFinished
};

class CTcFsmFileFindClose : public TTcFsmFile<eFileFindCloseState>
{
public:
	CTcFsmFileFindClose();
	HRESULT Init
	(
		ITComObjectServer* ipSrv,
		ITcFileAccessAsync* ipFileAccessAsync
	);
	HRESULT Eval(TcFileFindHandle hFileFind);

private:
};