#pragma once

#include "TcFileAccessInterfaces.h"
#include "TcFsmFile.h"
#include "TcFsmFileOpen.h"
#include "TcFsmFileSeek.h"
#include "TcFsmFileRead.h"
#include "TcFsmFileClose.h"


enum eStateFileReader
{
	FileReaderError,
	FileReaderOpen,
	FileReaderSeek,
	FileReaderRead,
	FileReaderClose,
	FileReaderErrorCleanup,
	FileReaderFinished
};

class CTcFsmFileReader : public TTcFsmFile<eStateFileReader>
{
public: 
	CTcFsmFileReader(TcTraceLevel& TraceLevelMax);
	HRESULT Init
	(
		ITComObjectServer* ipSrv, 
		ITcFileAccessAsync* ipFileAccessAsync, 
		PCCH szFilename, 
		ULONG nOffset,
		PBYTE pBuffer, 
		ULONG nBuffer
	);
	HRESULT Eval();

	ULONG GetBytesReadTotal() { return m_nBytesReadTotal; }

protected:
	// Tracing
	CTcTrace m_Trace;

private: 
	PCCH         m_szFilename;
	TcFileHandle m_hFileReader;
	PBYTE        m_pFileReaderBuffer;
	ULONG        m_nFileReaderBuffer; 
	ULONG        m_nBytesReadTotal;
	ULONG        m_nOffset;

	CTcFsmFileOpen  m_fsmFileOpen;
	CTcFsmFileSeek  m_fsmFileSeek;
	CTcFsmFileRead  m_fsmFileRead;
	CTcFsmFileClose m_fsmFileClose;
};