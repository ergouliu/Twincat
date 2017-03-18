///////////////////////////////////////////////////////////////////////////////
// TcAsyncFileFindDrvCtrl.h

#ifndef __TCASYNCFILEFINDDRVCTRL_H__
#define __TCASYNCFILEFINDDRVCTRL_H__

#include <atlbase.h>
#include <atlcom.h>

#define TCASYNCFILEFINDDRVDRV_NAME "TCASYNCFILEFINDDRV"

#include "resource.h"       // main symbols
#include "TcAsyncFileFindDrvW32.h"
#include "TcBase.h"
#include "TcAsyncFileFindDrvClassFactory.h"
#include "TcOCFCtrlImpl.h"

class CTcAsyncFileFindDrvCtrl 
	: public CComObjectRootEx<CComMultiThreadModel>
	, public CComCoClass<CTcAsyncFileFindDrvCtrl, &CLSID_TcAsyncFileFindDrvCtrl>
	, public ITcAsyncFileFindDrvCtrl
	, public ITcOCFCtrlImpl<CTcAsyncFileFindDrvCtrl, CTcAsyncFileFindDrvClassFactory>
{
public:
	CTcAsyncFileFindDrvCtrl();
	virtual ~CTcAsyncFileFindDrvCtrl();

DECLARE_REGISTRY_RESOURCEID(IDR_TCASYNCFILEFINDDRVCTRL)
DECLARE_NOT_AGGREGATABLE(CTcAsyncFileFindDrvCtrl)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTcAsyncFileFindDrvCtrl)
	COM_INTERFACE_ENTRY(ITcAsyncFileFindDrvCtrl)
	COM_INTERFACE_ENTRY(ITcCtrl)
	COM_INTERFACE_ENTRY(ITcCtrl2)
END_COM_MAP()

};

#endif // #ifndef __TCASYNCFILEFINDDRVCTRL_H__
