#include "stdafx.h"
#include "IOMonitorSvc.h"


//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
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
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CIOMonitorSvc::~CIOMonitorSvc()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ض�������

@param      ��

@retval     bool �Ƿ����쳣����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CIOMonitorSvc::Monitor()
{
	try
	{
		//// ��Ԯҵ��ά���Ŵ򿪡��رճ˿�ҵ���ά��ҵ��ת�� -- �ſ�ע�ͼ���
		//PostAssistanceButtonActionMSG();	// ������Ԯ��ť������Ϣ
		//PostDoorActionMSG();				// ����ά���Ŷ�����Ϣ
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
@brief      ������Ԯ��ť������Ϣ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIOMonitorSvc::PostAssistanceButtonActionMSG()
{
	//IO_RSP_STATUS_INFO iorspstatusinfo;				// IO״̬

	//// ��ȡIO״̬
	//IO_HELPER->GetIOStatus(iorspstatusinfo);

	//
	//// �ж���Ԯ��ť�Ƿ���
	//
	//// ��Ԯ��ť����
	//if(TRUE == iorspstatusinfo.isAssittentButtonPressed && UPORDOWN == 1)
	//{
	//	// �����Զ�����Ԯ��ť������Ϣ
	//	ResponseAssistBtn(SM_ASSITANCE_BUTTON_DOWN);
	//	//theSERVICE_MGR.GetCurService()->PostMessage(SM_ASSITANCE_BUTTON_DOWN,NULL,NULL);
	//	UPORDOWN ^= 1;
	//}
	//// ��Ԯ��ť����
	//else if(FALSE == iorspstatusinfo.isAssittentButtonPressed && UPORDOWN == 0)
	//{
	//	// �����Զ�����Ԯ��ť������Ϣ
	//	ResponseAssistBtn(SM_ASSITANCE_BUTTON_UP);
	//	//theSERVICE_MGR.GetCurService()->PostMessage(SM_ASSITANCE_BUTTON_UP,NULL,NULL);
	//	UPORDOWN ^= 1;
	//}	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ά���Ŷ�����Ϣ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIOMonitorSvc::PostDoorActionMSG()
{
	//IO_RSP_STATUS_INFO iorspstatusinfo;				// IO״̬

	//// ��ȡIO״̬
	//IO_HELPER->GetIOStatus(iorspstatusinfo);

	//// �ж�ά�����Ƿ��

	//// ά���Ŵ�
	//if(TRUE == iorspstatusinfo.isFrontDoorOpen && OPENORCLOSE == 1)
	//{
	//	// �����Զ���ά���Ŵ���Ϣ
	//	theSERVICE_MGR.GetCurService()->PostMessage(SM_DOOR_OPENED,NULL,NULL);
	//	OPENORCLOSE ^= 1;

	//	// ά��ҵ��
	//	theSERVICE_MGR.SetState(WAIT_MAINTENANCE);
	//	theMENU_MGR.SetMenuKind(CMenuMgr::MENU_MAINTAIN);
	//	theTVM_STATUS_MGR.SetWorkMode(0xffff);
	//}
	//// ά���Źر�
	//else if(FALSE == iorspstatusinfo.isFrontDoorOpen && OPENORCLOSE == 0)
	//{
	//	// �����Զ���ά���Źر���Ϣ
	//	theSERVICE_MGR.GetCurService()->PostMessage(SM_DOOR_CLOSED,NULL,NULL);
	//	OPENORCLOSE ^= 1;

	//	// �˿�ҵ��
	//	theSERVICE_MGR.SetState(READY);
	//	theMENU_MGR.SetMenuKind(CMenuMgr::MENU_COMMON);
	//	theSERVICE_MGR.StartService(ISSUE_SVC);
	//	theTVM_STATUS_MGR.SetWorkMode(0x061f);
	//}	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��Ӧ��Ԯ��ť

@param      ��

@retval     ��

@exception  ��
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
			theSERVICE_MGR.SetState(OUT_OF);    //����״̬
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
			theSERVICE_MGR.SetState(READY);    //����״̬
			theSERVICE_MGR.StartService(ISSUE_SVC);
			//����Ĭ��ҵ��
			//����״̬ IN_SERVICE
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
