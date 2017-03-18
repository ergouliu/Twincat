///////////////////////////////////////////////////////////////////////////////
// TcAsyncFileIoDriver.cpp
#include "TcPch.h"
#pragma hdrstop

#include "TcAsyncFileIoDriver.h"
#include "TcAsyncFileIoClassFactory.h"

DECLARE_GENERIC_DEVICE(TCASYNCFILEIODRV)

IOSTATUS CTcAsyncFileIoDriver::OnLoad( )
{
	TRACE(_T("CObjClassFactory::OnLoad()\n") );
	m_pObjClassFactory = new CTcAsyncFileIoClassFactory();

	return IOSTATUS_SUCCESS;
}

VOID CTcAsyncFileIoDriver::OnUnLoad( )
{
	delete m_pObjClassFactory;
}

unsigned long _cdecl CTcAsyncFileIoDriver::TCASYNCFILEIODRV_GetVersion( )
{
	return( (TCASYNCFILEIODRV_Major << 8) | TCASYNCFILEIODRV_Minor );
}

