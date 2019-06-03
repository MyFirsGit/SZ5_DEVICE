#include "StdAfx.h"
#include "ServiceMgr.h"
#include "InnerException.h"
#include "exceptionmgr.h"
#include "CursorMgr.h"
#include "messageiddefinition.h"
#include "foreservice.h"
#include "mainsvc.h"
#include "initsvc.h"
#include "servicedefinition.h"
#include "loginsvc.h"
#include "bomforeservice.h"
#include "moneyboxtestsvc.h"
#include "networktestsvc.h"
#include "printertestsvc.h"
#include "rwtestsvc.h"
#include "systemsetsvc.h"
#include "devicemodeswitchsvc.h"
#include "communicationsetsvc.h"
#include "stationsetsvc.h"
#include "timesetsvc.h"
#include "UDSNSetSvc.h"
#include "THModeSetSvc.h"
#include "UPSInfoSvc.h"
#include "THTestSvc.h"
#include "DataInputSvc.h"
#include "DataOutputSvc.h"
#include "removeexceptionsvc.h"
#include "chargesvc.h"
#include "adjustsvc.h"
#include "analyzesvc.h"
#include "refundsvc.h"
#include "RefundApplySvc.h"
#include "CounteractSvc.h"
#include "tvmfailurerefundsvc.h"
#include "otherssvc.h"
#include "nobusinessincomesvc.h"
#include "refreshsvc.h"
#include "serverclientsvc.h"
#include "rootsvc.h"
#include "IssueSvc.h"

#include "BusinessTranslateSvc.h"
#include "EventDataSvc.h"
#include "TranscationTranslateSvc.h"
#include "csalestaticsvc.h"
#include "ExchangeSvc.h"
#include "signcardreportlosssvc.h"
#include "signcardapplysvc.h"
#include "CardSuspendAndResumeSvc.h"
#include "upsmonitorsvc.h"
#include "thmonitorsvc.h"
#include "twmonitorsvc.h"
#include "rwmonitorsvc.h"
#include "ticketboxexchangesvc.h"
#include "defersvc.h"
#include "LPTMonitorSvc.h"
#include "rfidsetsvc.h"
#include "MessageBox.h"
#include "confirmalivesvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CServiceMgr CServiceMgr::theInstance;   // ʵ����CServiceMgr


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CServiceMgr::CServiceMgr()
{
    m_pRootService = NULL;
    m_pCurService = NULL;
    m_serviceState = INIT;
	m_ServiceBusy = false;
	m_IsRunning = false;

	CMessageBox::OnShowing.AddHandler(this,&CServiceMgr::OnShowingMessageBox);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CServiceMgr::~CServiceMgr()
{
    StopService(m_pRootService);
	CMessageBox::OnShowing.RemoveHandler(this,&CServiceMgr::OnShowingMessageBox);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡCServiceMgr���ʵ��

@param      ��

@retval     ��

@exception  CServiceMgr&    CServiceMgr���ʵ��
*/
//////////////////////////////////////////////////////////////////////////
CServiceMgr& CServiceMgr::GetInstance()
{
    return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Service

@param      (i)SERVICE_ID serviceID	    Service���
@param      (i)bool post                true: ͨ���ʵ���Ϣ����, false:ֱ������

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::StartService(SERVICE_ID serviceID, bool post)
{
    try {
        if (post) 
		{
            AfxGetMainWnd()->PostMessage(WM_START_SERVICE, serviceID, NULL);
        } 
		else if (CMessageBox::Exist()) 
		{
            CMessageBox::End(IDABORT);                 // ���������Ϣ����������
            AfxGetMainWnd()->PostMessage(WM_START_SERVICE, serviceID, NULL);
        }
        else 
		{
            CService* pService = CreateService(serviceID);
            StartService(pService);
        }
    }
    catch (CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Service

@param      (i)CService* pService   ��������Service����

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::StartService(CService* pService)
{
    try {
        if (pService == NULL) {
            throw CInnerException(ROOT_SVC, CInnerException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("����Serviceʱ��Service����ΪNULL"));
        }
        // �����ϼ�Service
        CService* pParentService = SearchService((SERVICE_ID)pService->GetParentServiceID());

        // �����趨��ǰService
        if (pService->GetServiceID() == ROOT_SVC || pService->IsForeground()) 
		{
            m_pCurService = pService;
        }

        if (pParentService != NULL)  // �ϼ�Service����
		{ 
            if (pParentService->GetServiceID() != ROOT_SVC) 
			{
				pParentService->StopChildren();
                pParentService->DeleteChildren();             // ɾ��ͬ����Service
            }
			StartingService.Invoke(pService);			//�����¼�
            pParentService->AddChild(pService);    // ���뵽�ϼ�Service
			pService->Start();
			StartedService.Invoke(pService);				//�����¼�
        }
        else  // �ϼ�Service�����ڣ���Root Service�� 
		{    
            m_pRootService = pService;
        }
    }
    catch (CSysException&) 
	{
        throw;
    }
    catch (...) {
        throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����������ǰService

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::RestartCurService()
{
    if (GetCurService() != NULL) 
	{
        StartService((SERVICE_ID)GetCurService()->GetServiceID());
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ǰService

@param      ��

@retval     CService*   ��ǰService

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CService* CServiceMgr::GetCurService()
{
    return m_pCurService;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣService

@param      (i)SERVICE_ID serviceID	    Service���
@param      (i)bool post                true: ͨ���ʵ���Ϣ�ر�, false:ֱ�ӹر�

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::StopService(SERVICE_ID serviceID, bool post)
{
    try {
        if (post) 
		{
            AfxGetMainWnd()->PostMessage(WM_STOP_SERVICE, serviceID, NULL);
        }
        else 
		{
            CService* pService = SearchService(serviceID);
            StopService(pService);
        }
    }
    catch (CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣService

@param      (i)CService* pService

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::StopService(CService* pService)
{
    try {
        if (pService != NULL) {
            CService* pParentService = pService->GetParent();

            // �����趨��ǰService
            if (pService->IsForeground())
			{
                m_pCurService = pParentService;
            }
			StopingService.Invoke(pService);
			pService->Stop();
			StopedService.Invoke(pService);
            if (pParentService != NULL) 
			{
                // �Ӹ�Service��ɾ����Service
                pParentService->DeleteChild(pService);
            }
            else 
			{
                // ɾ��ROOT_SVC, ��Service��Ϊ��
                pService->DeleteChildren();
                delete pService;
                m_pRootService = NULL;
            }
        }
    }
    catch (CSysException&) 
	{
        throw;
    }
    catch (...) 
	{
        throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Service ID��ȡService����

@param      (i)SERVICE_ID serviceID	        Service���

@retval     CService*   NULL��δ�ҵ���������Service����

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CService* CServiceMgr::GetService(SERVICE_ID serviceID)
{
    try 
	{
        return SearchService(serviceID);
    }
    catch (CSysException&) 
	{
        throw;
    }
    catch (...) 
	{
        throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ServiceTree�и���Service ID����Service

@param      (i)SERVICE_ID serviceID	        Service���

@retval     SeriverNode     NULL��δ�ҵ���������Serivere

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CService* CServiceMgr::SearchService(SERVICE_ID serviceID)
{
    try 
	{
        CService* pService = NULL;
        if (m_pRootService != NULL) 
		{
            pService = m_pRootService->SearchService(serviceID);
        }
        return pService;
    }
    catch (CSysException&) 
	{
        throw;
    }
    catch (...) 
	{
        throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Service

@param      (i)SERVICE_ID       Service���

@retval     CService*   fail:NULL

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CService* CServiceMgr::CreateService(SERVICE_ID serviceID)
{
    try 
	{
        CService* pService = NULL;

        switch(serviceID) 
		{
        case ROOT_SVC:          // ��
            pService = new CRootSvc;
            break;
        case MAIN_SVC:          // ����
            pService = new CMainSvc;
            break;
		case CONFIRM_ALIVE_SVC:
			pService = new CConfirmAliveSvc;
			break;
		case SC_SVC:
			pService = new CServerClientSvc;
			break;
		case BUSINESS_INTERVAL_SVC:
			pService = new CBusinessTranslateSvc;
			break;
		case EVENT_MSG_SVC:
			pService = new CEventDataSvc;
			break;
		case TRANSMISSION_INTERVAL_SVC:
			pService = new CTransSvc;
			break;
		case MONITOR_RW_SVC:
			pService = new CRWMonitorSvc;
			break;
		case MONITOR_TW_SVC:
			pService = new CTWMonitorSvc;
			break;
		case MONITOR_TH_SVC:
			pService = new CTHMonitorSvc;
			break;
		case MONITOR_UPS_SVC:
			pService = new CUPSMonitorSvc;
			break;
		case MONITOR_LPT_SVC:
			pService = new CLPTMonitorSvc;
			break;
        case INIT_SVC:          // ��ʼ��
            pService = new CInitSvc;
            break;
        case LOGIN_SVC:         // ��¼
            pService = new CLoginSvc;
            break;
		case ISSUE_SVC:			//��Ʊ
			pService = new CIssueSvc;
			break;
		case CHARGE_SVC:
			pService = new CChargeSvc;
			break;
		case ADJUST_SVC:
			pService = new CAdjustSvc;
			break;
		case REFUND_BASE_SVC:
			pService = new CRefundSvc;
			break;
		case REFUND_APPLY_SVC:
			pService = new CRefundApplySvc;
			break;
		//case REFUND_QUERY_SVC:
		//	pService = new CRefundQuerySvc;
		//	break;
		case ANALYZE_SVC:
			pService = new CAnalyzeSvc;
			break;
		case COUNTERACT_SVC:
			pService = new CCounteractSvc;
			break;
		case TVMFAILURE_REFUND_SVC:
			pService = new CTVMFailureRefundSvc;
			break;
		case NOBUSINESS_INCOME_SVC:
			pService = new CNoBusinessIncomeSvc;
			break;
		case REFRESH_SVC:
			pService = new CRefreshSvc;
			break;
		case DEFER_SVC:
			pService = new CDeferSvc;
			break;
		case SIGNCARD_APPLY_SVC:
			pService = new CSignCardApplySvc;
			break;
		case SIGNCARD_REPORT_LOSS_SVC:
			pService = new CSignCardReportLossSvc;
			break;
		case CARD_SUSPEND_AND_RESUME_SVC:
			pService = new CCardSuspendAndResumeSvc;
			break;
		case TICKETBOX_EXCHANGE_SVC:
			pService = new CTicketBoxExchangeSvc;
			break;
		case EXCHANGE_SVC:
			pService = new CExchangeSvc;
			break;
		case STATION_SET_SVC:
			pService = new CStationSetSvc;
			break;
		case COMMUNICATION_SET_SVC:
			pService = new CCommunicationSetSvc;
			break;
		case MODE_SWITCH_SVC:
			pService = new CDeviceModeSwitchSvc;
			break;
		case SYSTEM_TIME_SET_SVC:
			pService = new CTimeSetSvc;
			break;
		case RFID_MODE_SET_SVC:
			pService = new CRFIDModeSetSvc;
			break;
		case UDSN_SET_SVC:
			pService = new CUDSNSetSvc;
			break;
		case TH_MODE_SET_SVC:
			pService = new CTHModeSetSvc;
			break;
		case UPS_INFO_SVC:
			pService = new CUPSInfoSvc;
			break;
		case TH_TEST_SVC:
			pService = new CTHTestSvc;
			break;
		case DATA_INPUT_SVC:
			pService = new CDataInputSvc;
			break;
		case DATA_OUTPUT_SVC:
			pService = new CDataOutputSvc;
			break;
		case OUTRW_TEST_SVC:
			pService = new CRWTestSvc;
			break;
		case PRINTER_TEST_SVC:
			pService = new CPrinterTestSvc;
			break;
		case MONEYBOX_TEST_SVC:
			pService = new CMoneyBoxTestSvc;
			break;
		case NETWORK_TEST_SVC:
			pService = new CNetworkTestSvc;
			break;
		case REMOVE_EXCEPTION_SVC:          // �쳣���
		    pService = new CRemoveExceptionSvc;
		    break;
		case SALE_STATIC_SVC:          
			pService = new CSaleStaticSvc;
			break;
        default:
            break;
        }
        return pService;
    }
    catch (CSysException&) 
	{
        throw;
    }
    catch (...) 
	{
        throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ֹ��Service, ����Ĭ��Service

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::NotifyFinish()
{
    // ����ǰ����ҵ��������Ĭ��ҵ��������ҵ����ֹͣ��ҵ��
    if (GetCurService() != NULL && GetCurService()->GetParentServiceID() == MAIN_SVC) 
	{
		GetService(MAIN_SVC)->StartTimer(CMainSvc::TIMER_DEFAULT_SERVICE,0);
    }
    else  if (GetCurService() != NULL) 
	{
        StopService((SERVICE_ID)GetCurService()->GetServiceID(), true);
    }
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ֪ͨǰ̨Serviceģʽ�����ı�

@param      (i)UINT mode                ģʽ���ࣨSP_MODE_SYSTEM������
@param      (i)CForeService* pService   ��֪ͨ��Service��NULL: MAIN_SVC��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CServiceMgr::NotifyModeChange(UINT mode, CBOMForeService* pService)
//{
//    // pServiceΪ��ʱ��ȡMAIN_SVC
//    if (pService == NULL) 
//	{
//        pService = (CBOMForeService*)theSERVICE_MGR.GetService(MAIN_SVC);
//        if (pService == NULL) 
//		{
//            return;
//        }
//    }
//    // ����Service��OnModeChange
//    pService->OnModeChange(mode);
//    // �ݹ������Service��OnModeChange
//    CService::ServiceChildren children = pService->GetChildren();
//    for (size_t i = 0; i < children.size(); i++) 
//	{
//        ((CBOMForeService*)children[i])->OnModeChange(mode);
//    }
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ѵ�ǰ�����Ϊæµ

@param      (i)bool bBusy \n
true :��Ϊæµ ; false: ��Ϊ��æµ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::SetForeServiceBusy(bool bBusy)
{
	if(bBusy == m_ServiceBusy)
	{
		return;
	}
	m_ServiceBusy = bBusy;
	if(GetCurService()->GetServiceID()!=MAIN_SVC)
	{
		GetService<CMainSvc>(MAIN_SVC)->OnBusy(bBusy);
	}
	if (GetCurService()->IsForeground()) 
	{
		CBOMForeService* pService = (CBOMForeService*)GetCurService();
		if (pService != NULL) 
		{
			pService->OnBusy(bBusy);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡҵ��״̬

@param      ��

@retval     SERVICE_STATE, ҵ��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SERVICE_STATE CServiceMgr::GetState()
{
    return m_serviceState;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ҵ��״̬

@param      (i)SERVICE_STATE state, ҵ��״̬

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::SetState(SERVICE_STATE state)
{
    if (state == ABNORMAL) 
	{
        throw CInnerException(GetCurService()->GetServiceID(), CInnerException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("��������Ϊ�쳣״̬"));
    }
    if (m_serviceState == MAINTENANCE && state != OUT_OF) 
	{
        throw CInnerException(GetCurService()->GetServiceID(), CInnerException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__, _T("��ά��״ֻ̬��ת����ֹ״̬"));
    }
    try 
	{
		if (state == m_serviceState) 
		{
			if(state == RECEPT)
			{
				int interval = theBOM_INFO.GetWaitOperationInterval();
				GetService(MAIN_SVC)->StartTimer(CMainSvc::TIMER_DEFAULT_SERVICE,interval);
			}
			return;
		}
		if(state == ISSUEING)
		{
			SetForeServiceBusy(true);
		}
		else
		{
			SetForeServiceBusy(false);
		}
		if(state == FINISH || state == RECEPT)
		{
			int interval = state == FINISH ? theBOM_INFO.GetReturnDefaultServiceInterval():theBOM_INFO.GetWaitOperationInterval();
			GetService(MAIN_SVC)->StartTimer(CMainSvc::TIMER_DEFAULT_SERVICE,interval);
		}
		else
		{
			GetService(MAIN_SVC)->StopTimer(CMainSvc::TIMER_DEFAULT_SERVICE);
		}
        if (state == READY)  // ��ҵ��״̬ == �ȴ���
		{    
			GetService(MAIN_SVC)->StartTimer(CMainSvc::TIMER_AUTOMATIC_LOGOFF,theAFC_BOM_OPERATION.GetAutoLogoffTime() == 0 ? 2*60 : theAFC_BOM_OPERATION.GetAutoLogoffTime());
        }
		else
		{
			GetService(MAIN_SVC)->StopTimer(CMainSvc::TIMER_AUTOMATIC_LOGOFF);
		}
		SERVICE_STATE oldState = m_serviceState;
        m_serviceState = state;
		StateChanged.Invoke(oldState,m_serviceState);
    }
    catch (CSysException&) 
	{
        throw;
    }
    catch (...) 
	{
        throw CInnerException(GetCurService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ����ʾ

@param      (i)SERVICE_STATE state, ҵ��״̬

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::OnShowingMessageBox()
{
	//if(m_serviceState!=ISSUEING && m_serviceState!=MAINTENANCE)
	//{
	//	SetState(ISSUEING);
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      BOM�Ƿ�������״̬��ֻ����ǿ�Ƶǳ�

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CServiceMgr::IsRunning()
{
	return m_IsRunning;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����BOM����״̬

@param      (i)bool isRunning ����״̬

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CServiceMgr::SetRunningStatus(bool isRunning)
{
	m_IsRunning = isRunning;
}