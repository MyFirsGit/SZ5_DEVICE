// OEDUShutterDev.h: interface for the OEDUShutterDev class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OEDUSHUTTERDEV_H__6CF9B598_BD33_48F1_82DA_519C0343DF2D__INCLUDED_)
#define AFX_OEDUSHUTTERDEV_H__6CF9B598_BD33_48F1_82DA_519C0343DF2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ODevBase.h"

typedef struct
{
	BOOL bOpenSensor;	// 开门传感器，TRUE-门开，FALSE-门关
	BOOL bCloseSensor;	// 关门传感器，TRUE-门开，FALSE-门关
	BOOL bBackSensor;	// 回位传感器，TRUE-回位传感器开，FALSE-回位传感器关
} tShutterStatus;

class AFX_EXT_CLASS OEDUShutterDev : public ODevBase  
{
public:
	OEDUShutterDev();
	virtual ~OEDUShutterDev();

public:
	int iInit(tDevReturn *p_psStatus );
	int iGetStatus(tDevReturn *p_psStatus );
	int	iOpen(tDevReturn *p_psStatus );
	int iClose(tDevReturn *p_psStatus );
	int	iGetDevVersion(char *p_pcVersion);
	int iSetCommPara(tDevReturn* p_psStatus);
	int	iGetVersion(char* p_pcVersion);
	int iGetDevStatus(tShutterStatus* p_psShutterStatus,tDevReturn* p_psStatus);
	
private:
	
	int	iSendAndRev(char *p_pcSendData, UINT p_wSendLen,char *p_pcRevData,UINT& p_wRevLen, tDevReturn *p_psStatus, int p_iTimeLimit = 5000, int p_iTryNum = 2);
	char	m_acDevName[32];		// 设备逻辑标识名称

};

#endif // !defined(AFX_OEDUSHUTTERDEV_H__6CF9B598_BD33_48F1_82DA_519C0343DF2D__INCLUDED_)
