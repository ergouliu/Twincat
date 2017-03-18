///////////////////////////////////////////////////////////////////////////////
// TcAsyncFileFindDrvDriver.h

#ifndef __TCASYNCFILEFINDDRVDRIVER_H__
#define __TCASYNCFILEFINDDRVDRIVER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TcBase.h"

#define TCASYNCFILEFINDDRVDRV_NAME        "TCASYNCFILEFINDDRV"
#define TCASYNCFILEFINDDRVDRV_Major       1
#define TCASYNCFILEFINDDRVDRV_Minor       0

#define DEVICE_CLASS CTcAsyncFileFindDrvDriver

#include "ObjDriver.h"

class CTcAsyncFileFindDrvDriver : public CObjDriver
{
public:
	virtual IOSTATUS	OnLoad();
	virtual VOID		OnUnLoad();

	//////////////////////////////////////////////////////
	// VxD-Services exported by this driver
	static unsigned long	_cdecl TCASYNCFILEFINDDRVDRV_GetVersion();
	//////////////////////////////////////////////////////
	
};

Begin_VxD_Service_Table(TCASYNCFILEFINDDRVDRV)
	VxD_Service( TCASYNCFILEFINDDRVDRV_GetVersion )
End_VxD_Service_Table


#endif // ifndef __TCASYNCFILEFINDDRVDRIVER_H__