///////////////////////////////////////////////////////////////////////////////
// TcAsyncBufferReadingDrvDriver.h

#ifndef __TCASYNCBUFFERREADINGDRVDRIVER_H__
#define __TCASYNCBUFFERREADINGDRVDRIVER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TcBase.h"

#define TCASYNCBUFFERREADINGDRVDRV_NAME        "TCASYNCBUFFERREADINGDRV"
#define TCASYNCBUFFERREADINGDRVDRV_Major       1
#define TCASYNCBUFFERREADINGDRVDRV_Minor       0

#define DEVICE_CLASS CTcAsyncBufferReadingDrvDriver

#include "ObjDriver.h"

class CTcAsyncBufferReadingDrvDriver : public CObjDriver
{
public:
	virtual IOSTATUS	OnLoad();
	virtual VOID		OnUnLoad();

	//////////////////////////////////////////////////////
	// VxD-Services exported by this driver
	static unsigned long	_cdecl TCASYNCBUFFERREADINGDRVDRV_GetVersion();
	//////////////////////////////////////////////////////
	
};

Begin_VxD_Service_Table(TCASYNCBUFFERREADINGDRVDRV)
	VxD_Service( TCASYNCBUFFERREADINGDRVDRV_GetVersion )
End_VxD_Service_Table


#endif // ifndef __TCASYNCBUFFERREADINGDRVDRIVER_H__