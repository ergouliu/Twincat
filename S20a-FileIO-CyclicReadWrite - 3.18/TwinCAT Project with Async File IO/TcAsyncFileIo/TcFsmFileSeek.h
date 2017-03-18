#pragma once

#include "TcFileAccessInterfaces.h"
#include "TcFsmFile.h"

enum eFileSeekState
{
	FileSeekError,
	FileSeekInit,
	FileSeekCheck,
	FileSeekFinished
};

class CTcFsmFileSeek : public TTcFsmFile<eFileSeekState>
{
public: 
	CTcFsmFileSeek();
	HRESULT Init
	(
		ITComObjectServer* ipSrv, 
		ITcFileAccessAsync* ipFileAccessAsync, 
		TcFileHandle hFileSeek, 
		UINT nFileSeekOffset
	);
	HRESULT Eval();

private: 
	TcFileHandle m_hFileSeek;
	UINT         m_nFileSeekOffset; 
};

