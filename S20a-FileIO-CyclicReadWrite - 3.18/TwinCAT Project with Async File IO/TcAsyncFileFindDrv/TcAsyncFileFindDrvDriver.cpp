///////////////////////////////////////////////////////////////////////////////
// TcAsyncFileFindDrvDriver.cpp
#include "TcPch.h"
#pragma hdrstop

#include "TcAsyncFileFindDrvDriver.h"
#include "TcAsyncFileFindDrvClassFactory.h"

DECLARE_GENERIC_DEVICE(TCASYNCFILEFINDDRVDRV)

IOSTATUS CTcAsyncFileFindDrvDriver::OnLoad( )
{
	TRACE(_T("CObjClassFactory::OnLoad()\n") );
	m_pObjClassFactory = new CTcAsyncFileFindDrvClassFactory();

	return IOSTATUS_SUCCESS;
}

VOID CTcAsyncFileFindDrvDriver::OnUnLoad( )
{
	delete m_pObjClassFactory;
}

unsigned long _cdecl CTcAsyncFileFindDrvDriver::TCASYNCFILEFINDDRVDRV_GetVersion( )
{
	return( (TCASYNCFILEFINDDRVDRV_Major << 8) | TCASYNCFILEFINDDRVDRV_Minor );
}

