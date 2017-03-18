///////////////////////////////////////////////////////////////////////////////
// TcAsyncBufferWritingDrvClassFactory.cpp
#include "TcPch.h"
#pragma hdrstop

#include "TcAsyncBufferWritingDrvClassFactory.h"
#include "TcAsyncBufferWritingDrvServices.h"
#include "TcAsyncBufferWritingModule.h"

BEGIN_CLASS_MAP(CTcAsyncBufferWritingDrvClassFactory)
	CLASS_ENTRY(CID_TcAsyncBufferWritingDrvCTcAsyncBufferWritingModule, SRVNAME_TCASYNCBUFFERWRITINGDRV "!CTcAsyncBufferWritingModule", CTcAsyncBufferWritingModule)
///<AutoGeneratedContent id="ClassMap">
	CLASS_ENTRY(CID_TcAsyncBufferWritingDrvCTcAsyncBufferWritingModule, SRVNAME_TCASYNCBUFFERWRITINGDRV "!CTcAsyncBufferWritingModule", CTcAsyncBufferWritingModule)
///</AutoGeneratedContent>
END_CLASS_MAP()

CTcAsyncBufferWritingDrvClassFactory::CTcAsyncBufferWritingDrvClassFactory() : CObjClassFactory()
{
	TcDbgUnitSetImageName(TCDBG_UNIT_IMAGE_NAME(SRVNAME_TCASYNCBUFFERWRITINGDRV)); 
}

