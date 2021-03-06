///////////////////////////////////////////////////////////////////////////////
// TcAsyncBufferReadingModule.cpp
#include "TcPch.h"
#pragma hdrstop

#include "TcAsyncBufferReadingModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DEFINE_THIS_FILE()

///////////////////////////////////////////////////////////////////////////////
// CTcAsyncBufferReadingModule
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Collection of interfaces implemented by module CTcAsyncBufferReadingModule
BEGIN_INTERFACE_MAP(CTcAsyncBufferReadingModule)
	INTERFACE_ENTRY_ITCOMOBJECT()
	INTERFACE_ENTRY(IID_ITcCyclic, ITcCyclic)
	INTERFACE_ENTRY(IID_ITcADI, ITcADI)
///<AutoGeneratedContent id="InterfaceMap">
	INTERFACE_ENTRY(IID_ITcCyclic, ITcCyclic)
///</AutoGeneratedContent>
END_INTERFACE_MAP()

IMPLEMENT_ITCOMOBJECT(CTcAsyncBufferReadingModule)
IMPLEMENT_ITCOMOBJECT_SETSTATE_LOCKOP2(CTcAsyncBufferReadingModule)
IMPLEMENT_ITCADI(CTcAsyncBufferReadingModule)

///////////////////////////////////////////////////////////////////////////////
// Set parameters of CTcAsyncBufferReadingModule 
BEGIN_SETOBJPARA_MAP(CTcAsyncBufferReadingModule)
	SETOBJPARA_DATAAREA_MAP()
///<AutoGeneratedContent id="SetObjectParameterMap">
	SETOBJPARA_VALUE(PID_TcAsyncBufferReadingModuleParameter, m_Parameter)
	SETOBJPARA_VALUE(PID_TcTraceLevel, m_TraceLevelMax)
	SETOBJPARA_ITFPTR(PID_Ctx_TaskOid, m_spCyclicCaller)
///</AutoGeneratedContent>
END_SETOBJPARA_MAP()

///////////////////////////////////////////////////////////////////////////////
// Get parameters of CTcAsyncBufferReadingModule 
BEGIN_GETOBJPARA_MAP(CTcAsyncBufferReadingModule)
	GETOBJPARA_DATAAREA_MAP()
///<AutoGeneratedContent id="GetObjectParameterMap">
	GETOBJPARA_VALUE(PID_TcAsyncBufferReadingModuleParameter, m_Parameter)
	GETOBJPARA_VALUE(PID_TcTraceLevel, m_TraceLevelMax)
	GETOBJPARA_ITFPTR(PID_Ctx_TaskOid, m_spCyclicCaller)
///</AutoGeneratedContent>
END_GETOBJPARA_MAP()

///////////////////////////////////////////////////////////////////////////////
// Get data area members of CTcAsyncBufferReadingModule
BEGIN_OBJDATAAREA_MAP(CTcAsyncBufferReadingModule)
///<AutoGeneratedContent id="ObjectDataAreaMap">
	OBJDATAAREA_VALUE(0, m_Inputs)
	OBJDATAAREA_VALUE(1, m_Outputs)
	OBJDATAAREA_VALUE(3, m_Data)
///</AutoGeneratedContent>
END_OBJDATAAREA_MAP()


///////////////////////////////////////////////////////////////////////////////
CTcAsyncBufferReadingModule::CTcAsyncBufferReadingModule()
	: m_counter(0)
	, m_Trace(m_TraceLevelMax, m_spSrv)
	, m_fsmFileReader(m_TraceLevelMax)
{
	memset(&m_Parameter, 0, sizeof(m_Parameter)); 
	memset(&m_Inputs, 0, sizeof(m_Inputs)); 
	memset(&m_Outputs, 0, sizeof(m_Outputs)); 
}

///////////////////////////////////////////////////////////////////////////////
CTcAsyncBufferReadingModule::~CTcAsyncBufferReadingModule() 
{
}


///////////////////////////////////////////////////////////////////////////////
// State Transitions 
///////////////////////////////////////////////////////////////////////////////
IMPLEMENT_ITCOMOBJECT_SETOBJSTATE_IP_PI(CTcAsyncBufferReadingModule)

///////////////////////////////////////////////////////////////////////////////
// State transition from PREOP to SAFEOP
//
// Initialize input parameters 
// Allocate memory
HRESULT CTcAsyncBufferReadingModule::SetObjStatePS(PTComInitDataHdr pInitData)
{
	HRESULT hr = S_OK;
	IMPLEMENT_ITCOMOBJECT_EVALUATE_INITDATA(pInitData);

	hr = FAILED(hr) ? hr : InitReadBuffer();
	hr = FAILED(hr) ? hr : m_spSrv->TcCreateInstance(CID_TcFileAccessAsync, m_spFileAccessAsync.GetIID(), (PPVOID)(&m_spFileAccessAsync)); 
	if (SUCCEEDED(hr))
	{
		ITComObjectPtr spFileAccessObj = m_spFileAccessAsync;
		if (spFileAccessObj == NULL)
		{
			hr = ADS_E_NOINTERFACE;
		}
		else
		{
			OTCID oid = m_spCyclicCaller.GetOID();
			hr = FAILED(hr) ? hr : spFileAccessObj->TcSetObjPara(PID_TcFileAccessAsyncAdsProvider, sizeof(oid), &oid); 
			if (m_Parameter.nSegmentSize == 0)
			{
				hr = ADS_E_INVALIDPARM; 
			}
			hr = FAILED(hr) ? hr : spFileAccessObj->TcSetObjPara(PID_TcFileAccessAsyncSegmentSize, sizeof(m_Parameter.nSegmentSize), &m_Parameter.nSegmentSize); 
			hr = FAILED(hr) ? hr : spFileAccessObj->TcSetObjPara(PID_TcFileAccessAsyncTimeoutMs, sizeof(m_Parameter.nTimeout), &m_Parameter.nTimeout); 
			hr = FAILED(hr) ? hr : spFileAccessObj->TcSetObjState(TCOM_STATE_SAFEOP, m_spSrv, NULL); 
		}
	}

	// Cleanup if transition failed at some stage
	if (FAILED(hr))
	{
		DeinitReadBuffer();
		m_spFileAccessAsync = NULL;
	}
	return hr;
}

///////////////////////////////////////////////////////////////////////////////
// State transition from SAFEOP to OP
//
// Register with other TwinCAT objects
HRESULT CTcAsyncBufferReadingModule::SetObjStateSO()
{
	HRESULT hr = S_OK;

	// TODO: Add any additional initialization
	if (SUCCEEDED(hr))
	{
		ITComObjectPtr spFileAccessObj = m_spFileAccessAsync;
		hr = spFileAccessObj->TcSetObjState(TCOM_STATE_OP, m_spSrv, NULL); 
	}

	// If following call is successful the CycleUpdate method will be called, 
	// eventually even before method has been left.
	hr = FAILED(hr) ? hr : AddModuleToCaller(); 

	// Cleanup if transition failed at some stage
	if ( FAILED(hr) )
	{
		RemoveModuleFromCaller(); 
	}
	return hr;
}

///////////////////////////////////////////////////////////////////////////////
// State transition from OP to SAFEOP
HRESULT CTcAsyncBufferReadingModule::SetObjStateOS()
{
	HRESULT hr = S_OK;

	RemoveModuleFromCaller(); 

	return hr;
}

///////////////////////////////////////////////////////////////////////////////
// State transition from SAFEOP to PREOP
HRESULT CTcAsyncBufferReadingModule::SetObjStateSP()
{
	HRESULT hr = S_OK;
	DeinitReadBuffer();
	m_spFileAccessAsync = NULL;
	return hr;
}

///////////////////////////////////////////////////////////
// ITcCyclic
///<AutoGeneratedContent id="ImplementationOf_ITcCyclic">
HRESULT CTcAsyncBufferReadingModule::CycleUpdate(ITcTask* ipTask, ITcUnknown* ipCaller, ULONG_PTR context)
{
	HRESULT hr = S_OK;

	// rising edge on input bExecute
	if (m_fsmFileReader.IsActive())
	{
		hr = ADS_E_BUSY;
	}
	else
	{
		hr = m_fsmFileReader.Init(m_spSrv, m_spFileAccessAsync, "%TC_BOOTPRJPATH%ReadTest.txt", m_Inputs.nOffset, m_pReadBuffer, m_nReadBuffer); 
	}

	if (m_fsmFileReader.IsActive())
	{
		m_Outputs.bBusy = TRUE;
		hr = m_fsmFileReader.Eval();
		if (FAILED_AND_NOT_PENDING(hr))
		{
			m_Outputs.bError = TRUE;
			m_Outputs.iErrorId = hr;
		}
		else if (SUCCEEDED(hr))
		{
			m_Outputs.nBytesRead = m_fsmFileReader.GetBytesReadTotal();
			ULONG nCopySize = min(sizeof(m_Data.Values), m_nReadBuffer); 
			memcpy(buf, m_pReadBuffer, 20);

			m_Trace.Log(tlVerbose, FLEAVEA "Read bytes: nCopySize=0x%08x", nCopySize);
		}
	}
	else
	{
		m_Outputs.bBusy = FALSE;
	}
	return hr;
}
///</AutoGeneratedContent>


///////////////////////////////////////////////////////////////////////////////
HRESULT CTcAsyncBufferReadingModule::InitReadBuffer()
{
	HRESULT hr = S_OK;
	if (m_Parameter.nReadBufferSize == 0)
	{
		// default size
		m_nReadBuffer = 16*1024;
	}
	else
	{
		m_nReadBuffer = m_Parameter.nReadBufferSize;
	}
	m_pReadBuffer = new BYTE[m_nReadBuffer];

	if (m_pReadBuffer == NULL)
	{
		hr = ADS_E_NOMEMORY;
		SUCCEEDED_DBG(hr);
	}
	return hr;
}

///////////////////////////////////////////////////////////////////////////////
VOID CTcAsyncBufferReadingModule::DeinitReadBuffer()
{
	safe_delete_a(m_pReadBuffer); 
	m_nReadBuffer = 0;
}

///////////////////////////////////////////////////////////////////////////////
HRESULT CTcAsyncBufferReadingModule::AddModuleToCaller()
{
	HRESULT hr = S_OK;
	if ( m_spCyclicCaller.HasOID() )
	{
		if ( SUCCEEDED_DBG(hr = m_spSrv->TcQuerySmartObjectInterface(m_spCyclicCaller)) )
		{
			if ( FAILED(hr = m_spCyclicCaller->AddModule(m_spCyclicCaller, THIS_CAST(ITcCyclic))) )
			{
				m_spCyclicCaller = NULL;
			}
		}
	}
	else
	{
		hr = ADS_E_INVALIDOBJID; 
		SUCCEEDED_DBGT(hr, "Invalid OID specified for caller task");
	}
	return hr;
}

///////////////////////////////////////////////////////////////////////////////
VOID CTcAsyncBufferReadingModule::RemoveModuleFromCaller()
{
	if ( m_spCyclicCaller )
	{
		m_spCyclicCaller->RemoveModule(m_spCyclicCaller);
	}
	m_spCyclicCaller	= NULL;
}

