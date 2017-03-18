#pragma once

#include "TcFileAccessInterfaces.h"
#include "TcFsmFile.h"

enum eFileReadState
{
	FileReadError,
	FileReadInit,
	FileReadCheck,
	FileReadFinished
};

class CTcFsmFileRead : public TTcFsmFile<eFileReadState>
{
public: 
	CTcFsmFileRead();
	HRESULT Init
	(
		ITComObjectServer* ipSrv, 
		ITcFileAccessAsync* ipFileAccessAsync, 
		TcFileHandle hFileRead, 
		PBYTE pFileReadBuffer, 
		ULONG nFileReadBuffer
	);
	HRESULT Eval();

	// Output 
	UINT GetFileReadActually() const { return m_nFileReadActually; }

private: 
	TcFileHandle m_hFileRead;
	PBYTE        m_pFileReadBuffer;
	UINT         m_nFileReadBuffer; 
	UINT         m_nFileReadActually;
};

