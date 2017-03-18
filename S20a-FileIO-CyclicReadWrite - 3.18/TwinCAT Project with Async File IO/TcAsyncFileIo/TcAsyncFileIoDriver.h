///////////////////////////////////////////////////////////////////////////////
// TcAsyncFileIoDriver.h

#ifndef __TCASYNCFILEIODRIVER_H__
#define __TCASYNCFILEIODRIVER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TcBase.h"

#define TCASYNCFILEIODRV_NAME        "TCASYNCFILEIO"
#define TCASYNCFILEIODRV_Major       1
#define TCASYNCFILEIODRV_Minor       0

#define DEVICE_CLASS CTcAsyncFileIoDriver

#include "ObjDriver.h"

class CTcAsyncFileIoDriver : public CObjDriver
{
public:
	virtual IOSTATUS	OnLoad();
	virtual VOID		OnUnLoad();

	//////////////////////////////////////////////////////
	// VxD-Services exported by this driver
	static unsigned long	_cdecl TCASYNCFILEIODRV_GetVersion();
	//////////////////////////////////////////////////////
	
};

Begin_VxD_Service_Table(TCASYNCFILEIODRV)
	VxD_Service( TCASYNCFILEIODRV_GetVersion )
End_VxD_Service_Table


#endif // ifndef __TCASYNCFILEIODRIVER_H__