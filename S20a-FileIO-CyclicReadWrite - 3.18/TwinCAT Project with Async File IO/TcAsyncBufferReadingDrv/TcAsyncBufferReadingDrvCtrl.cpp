// TcAsyncBufferReadingDrvCtrl.cpp : Implementation of CTcTcAsyncBufferReadingDrvCtrl
#include "TcPch.h"
#pragma hdrstop

#include "TcAsyncBufferReadingDrvW32.h"
#include "TcAsyncBufferReadingDrvCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTcAsyncBufferReadingDrvCtrl

CTcAsyncBufferReadingDrvCtrl::CTcAsyncBufferReadingDrvCtrl() 
	: ITcOCFCtrlImpl<CTcAsyncBufferReadingDrvCtrl, CTcAsyncBufferReadingDrvClassFactory>() 
{
}

CTcAsyncBufferReadingDrvCtrl::~CTcAsyncBufferReadingDrvCtrl()
{
}

