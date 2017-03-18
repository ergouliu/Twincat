///////////////////////////////////////////////////////////////////////////////
// TcAsyncBufferReadingDrvCtrl.h

#ifndef __TCASYNCBUFFERREADINGDRVCTRL_H__
#define __TCASYNCBUFFERREADINGDRVCTRL_H__

#include <atlbase.h>
#include <atlcom.h>

#define TCASYNCBUFFERREADINGDRVDRV_NAME "TCASYNCBUFFERREADINGDRV"

#include "resource.h"       // main symbols
#include "TcAsyncBufferReadingDrvW32.h"
#include "TcBase.h"
#include "TcAsyncBufferReadingDrvClassFactory.h"
#include "TcOCFCtrlImpl.h"

class CTcAsyncBufferReadingDrvCtrl 
	: public CComObjectRootEx<CComMultiThreadModel>
	, public CComCoClass<CTcAsyncBufferReadingDrvCtrl, &CLSID_TcAsyncBufferReadingDrvCtrl>
	, public ITcAsyncBufferReadingDrvCtrl
	, public ITcOCFCtrlImpl<CTcAsyncBufferReadingDrvCtrl, CTcAsyncBufferReadingDrvClassFactory>
{
public:
	CTcAsyncBufferReadingDrvCtrl();
	virtual ~CTcAsyncBufferReadingDrvCtrl();

DECLARE_REGISTRY_RESOURCEID(IDR_TCASYNCBUFFERREADINGDRVCTRL)
DECLARE_NOT_AGGREGATABLE(CTcAsyncBufferReadingDrvCtrl)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTcAsyncBufferReadingDrvCtrl)
	COM_INTERFACE_ENTRY(ITcAsyncBufferReadingDrvCtrl)
	COM_INTERFACE_ENTRY(ITcCtrl)
	COM_INTERFACE_ENTRY(ITcCtrl2)
END_COM_MAP()

};

#endif // #ifndef __TCASYNCBUFFERREADINGDRVCTRL_H__
