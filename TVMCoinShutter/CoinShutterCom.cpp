#include "stdafx.h"
#include "CoinShutterCom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

HANDLE m_hCoinShutter = INVALID_HANDLE_VALUE;		// Ӳ��Ͷ�ҿ�ģ����
bool m_bConnected = false;
OEDUShutterDev m_ShutterDev;