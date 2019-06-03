#pragma once

#include "ServiceHelper.h"
#include "UPSCommand.h"

/**
@brief  UPS������
*/

class CUPSHelper :
	public CServiceHelper
{
public:

	CUPSHelper(CService& service);
	~CUPSHelper();

	// ��ӡ����� 
	long UPSOpen();															// ����UPS
	long UPSRead(_tagUPSBatteryInfo &info);									// ��ȡUPS
	long UPSClose();                                                        // �ر�UPS
	DWORD GetUPSBatteryCapacity();
	DWORD GetUPSLeftStandbyTime();
};