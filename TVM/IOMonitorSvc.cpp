#include "stdafx.h"
#include "IOMonitorSvc.h"


//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CIOMonitorSvc::CIOMonitorSvc()
:CMonitorSvc(MONITOR_IO_SVC,ROOT_SVC,theTVM_INFO.GetIOMonitorInterval())
{
	UPORDOWN = 1;
	OPENORCLOSE = 1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CIOMonitorSvc::~CIOMonitorSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      监控动作函数

@param      无

@retval     bool 是否有异常发生

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CIOMonitorSvc::Monitor()
{
	try
	{
		//// 招援业务、维护门打开、关闭乘客业务和维护业务转换 -- 放开注释即可
		//PostAssistanceButtonActionMSG();	// 发送招援按钮动作消息
		//PostDoorActionMSG();				// 发送维护门动作消息
		IO_HELPER->GetIOStatus(ioStatus);
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发送招援按钮动作消息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIOMonitorSvc::PostAssistanceButtonActionMSG()
{
	//IO_RSP_STATUS_INFO iorspstatusinfo;				// IO状态

	//// 获取IO状态
	//IO_HELPER->GetIOStatus(iorspstatusinfo);

	//
	//// 判断招援按钮是否按下
	//
	//// 招援按钮按下
	//if(TRUE == iorspstatusinfo.isAssittentButtonPressed && UPORDOWN == 1)
	//{
	//	// 发送自定义招援按钮按下消息
	//	ResponseAssistBtn(SM_ASSITANCE_BUTTON_DOWN);
	//	//theSERVICE_MGR.GetCurService()->PostMessage(SM_ASSITANCE_BUTTON_DOWN,NULL,NULL);
	//	UPORDOWN ^= 1;
	//}
	//// 招援按钮弹起
	//else if(FALSE == iorspstatusinfo.isAssittentButtonPressed && UPORDOWN == 0)
	//{
	//	// 发送自定义招援按钮弹起消息
	//	ResponseAssistBtn(SM_ASSITANCE_BUTTON_UP);
	//	//theSERVICE_MGR.GetCurService()->PostMessage(SM_ASSITANCE_BUTTON_UP,NULL,NULL);
	//	UPORDOWN ^= 1;
	//}	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发送维护门动作消息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIOMonitorSvc::PostDoorActionMSG()
{
	//IO_RSP_STATUS_INFO iorspstatusinfo;				// IO状态

	//// 获取IO状态
	//IO_HELPER->GetIOStatus(iorspstatusinfo);

	//// 判断维护门是否打开

	//// 维护门打开
	//if(TRUE == iorspstatusinfo.isFrontDoorOpen && OPENORCLOSE == 1)
	//{
	//	// 发送自定义维护门打开消息
	//	theSERVICE_MGR.GetCurService()->PostMessage(SM_DOOR_OPENED,NULL,NULL);
	//	OPENORCLOSE ^= 1;

	//	// 维护业务
	//	theSERVICE_MGR.SetState(WAIT_MAINTENANCE);
	//	theMENU_MGR.SetMenuKind(CMenuMgr::MENU_MAINTAIN);
	//	theTVM_STATUS_MGR.SetWorkMode(0xffff);
	//}
	//// 维护门关闭
	//else if(FALSE == iorspstatusinfo.isFrontDoorOpen && OPENORCLOSE == 0)
	//{
	//	// 发送自定义维护门关闭消息
	//	theSERVICE_MGR.GetCurService()->PostMessage(SM_DOOR_CLOSED,NULL,NULL);
	//	OPENORCLOSE ^= 1;

	//	// 乘客业务
	//	theSERVICE_MGR.SetState(READY);
	//	theMENU_MGR.SetMenuKind(CMenuMgr::MENU_COMMON);
	//	theSERVICE_MGR.StartService(ISSUE_SVC);
	//	theTVM_STATUS_MGR.SetWorkMode(0x061f);
	//}	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      响应招援按钮

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIOMonitorSvc::ResponseAssistBtn(UINT uMsg)
{
	SERVICE_STATE state = theSERVICE_MGR.GetState();

	switch(uMsg)
	{
	case SM_ASSITANCE_BUTTON_DOWN:
		switch(state)
		{
		case OUT_OF:
		case READY:
		case RECEPT:
		case ABNORMAL:
		case REST:
		case FINISH:
			theSERVICE_MGR.StartService(CALL_HELP_SVC);
			theSERVICE_MGR.SetState(OUT_OF);    //设置状态
			break;
		default:
			break;
		}
			
		break;
	case SM_ASSITANCE_BUTTON_UP:
		switch(state)
		{
		case OUT_OF:
		case READY:
		case RECEPT:
		case ABNORMAL:
		case REST:
		case FINISH:
			theSERVICE_MGR.SetState(READY);    //设置状态
			theSERVICE_MGR.StartService(ISSUE_SVC);
			//启动默认业务
			//设置状态 IN_SERVICE
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
