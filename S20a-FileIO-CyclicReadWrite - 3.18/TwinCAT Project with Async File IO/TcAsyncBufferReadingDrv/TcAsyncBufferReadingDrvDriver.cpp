///////////////////////////////////////////////////////////////////////////////
// TcAsyncBufferReadingDrvDriver.cpp
#include "TcPch.h"
#pragma hdrstop

#include "TcAsyncBufferReadingDrvDriver.h"
#include "TcAsyncBufferReadingDrvClassFactory.h"

DECLARE_GENERIC_DEVICE(TCASYNCBUFFERREADINGDRVDRV)

IOSTATUS CTcAsyncBufferReadingDrvDriver::OnLoad( )
{
	TRACE(_T("CObjClassFactory::OnLoad()\n") );
	m_pObjClassFactory = new CTcAsyncBufferReadingDrvClassFactory();

	return IOSTATUS_SUCCESS;
}

VOID CTcAsyncBufferReadingDrvDriver::OnUnLoad( )
{
	delete m_pObjClassFactory;
}

unsigned long _cdecl CTcAsyncBufferReadingDrvDriver::TCASYNCBUFFERREADINGDRVDRV_GetVersion( )
{
	return( (TCASYNCBUFFERREADINGDRVDRV_Major << 8) | TCASYNCBUFFERREADINGDRVDRV_Minor );
}

