///////////////////////////////////////////////////////////////////////////////
// TcAsyncFileFindDrvServices.h

#pragma once

#include "TcServices.h"

const ULONG DrvID_TcAsyncFileFindDrv = 0x3F000000;
#define SRVNAME_TCASYNCFILEFINDDRV "TcAsyncFileFindDrv"

///<AutoGeneratedContent id="ClassIDs">
const CTCID CID_TcAsyncFileFindDrvCTcAsyncFileFindModule = {0xb779de1f,0x606f,0x4baa,{0x82,0xa4,0xb7,0x14,0x3c,0x42,0xbd,0x13}};
///</AutoGeneratedContent>

///<AutoGeneratedContent id="ParameterIDs">
const PTCID PID_TcAsyncFileFindModuleParameter = 0x00000001;
const PTCID PID_TcAsyncFileFindModuleFilePath = 0x00000002;
///</AutoGeneratedContent>

///<AutoGeneratedContent id="DataTypes">
typedef struct _TcAsyncFileFindModuleParameter
{
	ULONG SegmentSize;
	ULONG Timeout;
} TcAsyncFileFindModuleParameter, *PTcAsyncFileFindModuleParameter;

#if !defined(_TC_TYPE_18071995_0000_0000_0000_000100000103_INCLUDED_)
#define _TC_TYPE_18071995_0000_0000_0000_000100000103_INCLUDED_
typedef CHAR STRING259[260];
#endif // !defined(_TC_TYPE_18071995_0000_0000_0000_000100000103_INCLUDED_)

typedef struct _TcAsyncFileFindModuleInputs
{
	bool bExecute;
} TcAsyncFileFindModuleInputs, *PTcAsyncFileFindModuleInputs;

typedef struct _TcAsyncFileFindModuleOutputs
{
	bool bBusy;
} TcAsyncFileFindModuleOutputs, *PTcAsyncFileFindModuleOutputs;

///</AutoGeneratedContent>



///<AutoGeneratedContent id="DataAreaIDs">
#define ADI_TcAsyncFileFindModuleInputs 1
#define ADI_TcAsyncFileFindModuleOutputs 2
///</AutoGeneratedContent>

///<AutoGeneratedContent id="InterfaceIDs">
///</AutoGeneratedContent>
