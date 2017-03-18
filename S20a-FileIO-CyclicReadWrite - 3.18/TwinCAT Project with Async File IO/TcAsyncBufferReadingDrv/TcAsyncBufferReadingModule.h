///////////////////////////////////////////////////////////////////////////////
// TcAsyncBufferReadingModule.h
#pragma once

#include "TcIoInterfaces.h"
#include "TcRtInterfaces.h"
#include "TcAsyncBufferReadingDrvInterfaces.h"
#include "TcFileAccessInterfaces.h"
#include "TcFsmFileReader.h"

class CTcAsyncBufferReadingModule 
	: public ITComObject
	, public ITcADI
///<AutoGeneratedContent id="InheritanceList">
	, public ITcCyclic
///</AutoGeneratedContent>
{
public:
	DECLARE_IUNKNOWN()
	DECLARE_IPERSIST(CID_TcAsyncBufferReadingDrvCTcAsyncBufferReadingModule)
	DECLARE_ITCOMOBJECT_LOCKOP()
	DECLARE_OBJDATAAREA_MAP()
	DECLARE_ITCADI()

	CTcAsyncBufferReadingModule();

	virtual	~CTcAsyncBufferReadingModule();


///<AutoGeneratedContent id="InterfaceMembers">
	// ITcCyclic
	virtual HRESULT TCOMAPI CycleUpdate(ITcTask* ipTask, ITcUnknown* ipCaller, ULONG_PTR context);

///</AutoGeneratedContent>

protected:
	DECLARE_ITCOMOBJECT_SETSTATE();

	HRESULT AddModuleToCaller();
	VOID RemoveModuleFromCaller();

	HRESULT InitReadBuffer();
	VOID DeinitReadBuffer();
	
	// Tracing
	CTcTrace m_Trace;

///<AutoGeneratedContent id="Members">
	TcAsyncBufferReadingModuleParameter m_Parameter;
	TcTraceLevel m_TraceLevelMax;
	TcAsyncBufferReadingModuleInputs m_Inputs;
	TcAsyncBufferReadingModuleOutputs m_Outputs;
	TcAsyncBufferReadingModuleData m_Data;
	ITcCyclicCallerInfoPtr m_spCyclicCaller;
///</AutoGeneratedContent>

	// TODO: Custom variable
	UINT m_counter;
	BOOL m_bExecute;

	ITcFileAccessAsyncPtr m_spFileAccessAsync;
	CTcFsmFileReader m_fsmFileReader;

	PBYTE m_pReadBuffer; 
	ULONG m_nReadBuffer;
	unsigned char buf[20];
};