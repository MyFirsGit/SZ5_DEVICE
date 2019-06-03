#include "StdAfx.h"
#include "StatusDisplayModuleHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// LED显示文言
#define SHOW_MSG_COUNT	13
static int nLedShowMsgIndex[SHOW_MSG_COUNT] = {
	IN_SERVICE,
	OUTOF_SERVICE,
	STOP_SERVICE,
	WORK_MODE_BANKNOTE_ONLY,
	WORK_MODE_COIN_ONLY,
	WORK_MODE_NO_CHANGE,
	WORK_MODE_SVT_ONLY,
	WORK_MODE_NO_ISSUE,
	WORK_MODE_ANALY_ONLY,
	WORK_MODE_NO_SVT,
	WORK_MODE_NO_COIN_ACCEPT,
	WORK_MODE_NO_BANKNOTE_ACCEPT,
	WORK_MODE_NORMALLY
};
// 语言对应Ini配置文件
#define CN_ID				1
#define EN_ID				2

CStatusDisplayModuleHelper CStatusDisplayModuleHelper::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStatusDisplayModuleHelper::CStatusDisplayModuleHelper()
{
	m_wCurrentStatus = 0x0000;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStatusDisplayModuleHelper::~CStatusDisplayModuleHelper(void)
{
	Close();
}

CStatusDisplayModuleHelper& CStatusDisplayModuleHelper::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			打开串口

@param      (i)无

@retval     long \n
0:成功	非0:失败

@exception
*/
//////////////////////////////////////////////////////////////////////////
long CStatusDisplayModuleHelper::Connect()
{
	long lret=1;
	try{
		lret = m_StatusDisplay.Open(theTVM_SETTING.GetDisplayComPort(), theTVM_SETTING.GetDisplayBaudRate());
	}
	catch(CSysException &e){

	}
	catch(...){

	}
	return lret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			关闭串口

@param      (i)无

@retval     long \n
0:成功	非0:失败

@exception
*/
//////////////////////////////////////////////////////////////////////////
long CStatusDisplayModuleHelper::Close()
{
	long lret=1;	
	lret = m_StatusDisplay.Close();
	return lret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			打开串口

@param      (i)无

@retval     long \n
0:成功	非0:失败

@exception
*/
//////////////////////////////////////////////////////////////////////////
long CStatusDisplayModuleHelper::Init()
{
	long lret=1;
	try{
		USES_CONVERSION;
		for (int i=0;i<SHOW_MSG_COUNT;i++){
				CString szCn = theFunction_INFO.getModeLanguage(nLedShowMsgIndex[i],CN_ID);	
				CString szEn = theFunction_INFO.getModeLanguage(nLedShowMsgIndex[i],EN_ID);
				int nSerialNo = nLedShowMsgIndex[i];
				if (nSerialNo == IN_SERVICE)
				{
					nSerialNo = 1;//LED硬件不支持0
				}
				int nColor = 0;//0 ~ 6 分别对应 红、绿、黄、蓝、紫、青、白等颜色。
				if (nLedShowMsgIndex[i] == 	IN_SERVICE || nLedShowMsgIndex[i] == WORK_MODE_NORMALLY)
				{
					nColor = 0x11;
				}
				else if (nLedShowMsgIndex[i] == OUTOF_SERVICE || nLedShowMsgIndex[i] == STOP_SERVICE)
				{
					nColor = 0;
				}
				else 
				{
					nColor = 0x33;
				}
				lret = m_StatusDisplay.DownloadTextToLed(nLedShowMsgIndex[i],szCn,szEn,nColor);
			}
	}
	catch(CSysException &e){

	}
	catch(...){

	}
	return lret;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief			尝试修复条屏

@param      (i)无

@retval     long \n
0:成功	非0:失败

@exception
*/
//////////////////////////////////////////////////////////////////////////
bool CStatusDisplayModuleHelper::Repare()
{
	bool bret=false;
	if(!Close() && !Connect())
	{
		bret=true;
	}
	return bret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			根据工作模式设置显示指定内

@param      (i)无

@retval     long \n
0:成功	非0:失败

@exception
*/
//////////////////////////////////////////////////////////////////////////
long CStatusDisplayModuleHelper::showTVMStatus(WORD bStatusCode)
{
	ShowCurWorkMode();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			显示工作模式对应的内容

@param      (i)无

@retval     long \n
0:成功	非0:失败

@exception
*/
//////////////////////////////////////////////////////////////////////////
long  CStatusDisplayModuleHelper::showWorkMode()
{
	ShowCurWorkMode();
	return 0;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief			显示找零模式对应的内容

@param      (i)无

@retval     long \n
0:成功	非0:失败

@exception
*/
//////////////////////////////////////////////////////////////////////////
long CStatusDisplayModuleHelper::showChargeMode()
{
	ShowCurWorkMode();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			显示支付模式对应的内容

@param      (i)无

@retval     long \n
0:成功	非0:失败

@exception
*/
//////////////////////////////////////////////////////////////////////////
long CStatusDisplayModuleHelper::showPaymentMode()
{
	ShowCurWorkMode();
	return 0;
}


long CStatusDisplayModuleHelper::ShowModeFunction(int modeValue)
{
	long lret = 1;
	try{
		USES_CONVERSION;
		for (int i=0;i<SHOW_MSG_COUNT;i++){
			if(nLedShowMsgIndex[i]==modeValue){
				CString strRunMode = theFunction_INFO.getModeLanguage(nLedShowMsgIndex[i]);	
				CStringA strLog;
				strLog.Format("(%d)-%s", modeValue, T2A(strRunMode));
				lret = m_StatusDisplay.ShowExtenModeText(i);
				break;
			}
		}
		if(lret!=0) {
			CStringA strLog;
			strLog.Format("(%d)-%s", modeValue, "未匹配到文言");
		}
	}
	catch(CSysException& e){

	}
	catch(...){

	}
	return lret;
}

void CStatusDisplayModuleHelper::ShowCurWorkMode()
{
	try{
		CString szCn = _T(""),szEn = _T("");
		int nColor = 0;//1 ~ 6 分别对应 红、绿、黄、蓝、紫、青、白等颜色。
		// 停止服务
		if(theTVM_STATUS_MGR.GetServiceStatus() == STOP_SERVICE){
			m_StatusDisplay.ShowExtenModeText(STOP_SERVICE);
		}
		// 暂停服务
		else if(theTVM_STATUS_MGR.GetServiceStatus() == OUTOF_SERVICE){
			m_StatusDisplay.ShowExtenModeText(OUTOF_SERVICE);
		}
		// 正常服务
		else if(theTVM_STATUS_MGR.GetServiceStatus() == IN_SERVICE){
			WORK_MODE curMode = theTVM_STATUS_MGR.GetDegreeWorkMode();
			m_StatusDisplay.ShowExtenModeText(curMode);
		}
	}
	catch(CSysException&e){
	}
	catch(...){
	}
}