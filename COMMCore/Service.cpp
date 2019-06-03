#include "stdafx.h"
#include "Service.h"
#include "InnerException.h"
#include "DebugLog.h"
#include "CoreAPI.h"
#include "TimerMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief	����ҵ��������
*/

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      UINT serviceID            Service ID
@param      UINT parentServiceID      �ϼ�Service ID
@param      bool isBackground               �Ƿ��Ǻ�̨Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CService::CService(UINT serviceID, UINT parentServiceID, bool isBackground)
{
    // ��ʼ����Ա����
    m_serviceID = serviceID;                            // Service ID
    m_parentServiceID = parentServiceID;                // �ϼ�Service ID
    m_isBackground = isBackground;                      // �Ƿ��Ǻ�̨Service

    m_pParent = NULL;                                   // �ϼ�Service
	
	pBag = new CBag;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CService::~CService()
{
   delete pBag;
   pBag = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡһ������

@param      ��

@retval     CBag* ����һ������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBag* CService::GetBag()
{
	return pBag;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CService::Start()
{
	try {
		Starting.Invoke(this);
		OnStart();
		Started.Invoke(this);
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CService::Stop()
{
	try {
		Stopping.Invoke(this);
		/*while (m_children.size() > 0) {
			m_children[0]->Stop();
		}*/
		OnStop();
		Stopped.Invoke(this);
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡService ID

@param      ��

@retval     UINT      Service ID

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CService::GetServiceID()
{
    return m_serviceID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�ϼ�Service ID

@param      ��

@retval     UINT      �ϼ�Service ID

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CService::GetParentServiceID()
{
    return m_parentServiceID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�ϼ�Service

@param      ��

@retval     CService*   �ϼ�Service

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CService* CService::GetParent()
{
    return m_pParent;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�¼�Service

@param      ��

@retval     ServiceChildren& �¼�Service

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CService::ServiceChildren& CService::GetChildren()
{
    return m_children;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ��Ǻ�̨Service

@param      ��

@retval     bool    true:�Ǻ�̨Service;  false:���Ǻ�̨Service

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CService::IsBackground()
{
    return m_isBackground;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ���ǰ̨Service

@param      ��

@retval     bool    true:��ǰ̨Service;  false:����ǰ̨Service

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CService::IsForeground()
{
    return !m_isBackground;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����Service

@param      int index

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CService::AddChild(CService* pChild)
{
    try {
        pChild->m_pParent = this;       // ���ø�Service
        m_children.push_back(pChild);         // ��ӵ�������
    }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ɾ��ȫ����Service

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CService::DeleteChildren()
{
    try {
        while (m_children.size() > 0) {
            DeleteChild(0);
        }
    }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ɾ����Service

@param      CService* pChild

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CService::DeleteChild(CService* pChild)
{
    try {
        unsigned int i = 0;

        for(; i < m_children.size(); i++) {
            if (m_children[i]->GetServiceID() == pChild->GetServiceID()){
                break;
            }
        }

        if(i < m_children.size()) {     // �ҵ�
            DeleteChild(i);
        }
    }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ��Service

@param      int index

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CService::StopChildren()
{
	for(unsigned int i=0; i < m_children.size(); i++) {
		m_children[i]->StopChildren();
		m_children[i]->Stop();
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ɾ����Service

@param      int index

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CService::DeleteChild(int index)
{
    try {
		UINT serviceID = m_children[index]->GetServiceID();
        m_children[index]->DeleteChildren();    // ɾ����Service��ȫ����Service
        delete m_children[index];               // ����
       m_children.erase(m_children.begin() + index);             // ��������ɾ��
    }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ڱ�Service�и���Service ID����Service

@param      (i)UINT serviceID	        Service���

@retval     CService*     NULL��δ�ҵ���������Serivere

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CService* CService::SearchService(UINT serviceID)
{
    try {
        CService* pService = NULL;

        if (GetServiceID() == serviceID) {
            pService = this;
        }
        else 
		{
            // find in children
			std::vector<CService*>::iterator itSvc=m_children.begin();
			for(;itSvc!=m_children.end();itSvc++)
			{
				pService = (*itSvc)->SearchService(serviceID);
				if(NULL != pService )
				{
					break;
				}
			}
            //for(unsigned int i = 0; i < m_children.size(); i++) {
              //  pService = m_children[i]->SearchService(serviceID);
			//
              //  if (pService != NULL) {
                //    break;
                //}
            //}
        }

        return pService;
    }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ʱ��

@param      (i)UINT timerID 
@param      (i)int seconds  �ȴ�ʱ��(��)

@retval     bool true:�ɹ� false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CService::StartTimer(UINT timerID, int seconds)
{
	bool bRet = false;
	// ������ʱ��ʧ����������
	for(unsigned int retryTimes = 1; retryTimes <= 3; retryTimes++){
		if(CTimerMgr::GetInstance().StartTimer(*this, timerID, seconds)){
			bRet = true;
			break;
		}
		else{
			theDEBUG_LOG->WriteData(_T("CService"), _T("StartTimer"), __LINE__, _T("timerID = 0x%x seconds = 0x%d failure"),timerID,seconds);
			Sleep(100);
		}
	}
	return bRet;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ��ʱ��

@param      (i)UINT timerID 

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CService::StopTimer(UINT timerID)
{
	CTimerMgr::GetInstance().StopTimer(*this, timerID);
}

/////////////////////////////////////////////////////////////////////////////
// Root of message maps

const AFX_MSGMAP CService::messageMap =
{
	NULL, 
	&CService::_messageEntries[0]
};

const AFX_MSGMAP* CService::GetMessageMap() const
{
	return &CService::messageMap;
}

#ifdef _AFXDLL
const AFX_MSGMAP* CService::GetThisMessageMap()
{
	return &CService::messageMap;
}
#endif

const AFX_MSGMAP_ENTRY CService::_messageEntries[] =
{
	{ 0, 0, AfxSig_end, 0 }     // nothing here
};

/////////////////////////////////////////////////////////////////////////////
// Root of dispatch maps

const AFX_MSGMAP_ENTRY* AFXAPI
AfxFindMessageEntry(const AFX_MSGMAP_ENTRY* lpEntry, 
					UINT nMsg, UINT nCode, UINT nID);


LRESULT CService::DispachCmdMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
	try {
		UINT nMsg = WM_COMMAND;
		UINT nCode = CN_COMMAND;
		UINT uiCommandID = 0;
		// �ֽ�Commmand ID, ���������������
		CCommand* pExtra = NULL;
		if(message == SP_REQUEST_COMPLETE || message == SP_REQUEST_ONPROGRESS)
		{
			pExtra = (CCommand*)lParam;
			uiCommandID = pExtra->GetCommandID();
		}
		else
		{
			nCode = message;
		}
		LRESULT result = -1;    // �����־

		// determine the message number and code (packed into firstParam)
		const AFX_MSGMAP* pMessageMap;
		const AFX_MSGMAP_ENTRY* lpEntry;
		

		union ServiceMessageMapFunctions mmf;

		// look through message map to see if it applies to us
#ifdef _AFXDLL
		for (pMessageMap = GetMessageMap(); pMessageMap->pfnGetBaseMap != NULL;
			pMessageMap = (*pMessageMap->pfnGetBaseMap)())
#else
		for (pMessageMap = GetMessageMap(); pMessageMap != NULL;
			pMessageMap = pMessageMap->pBaseMap)
#endif
		{
			// Note: catches BEGIN_MESSAGE_MAP(CMyClass, CMyClass)!
#ifdef _AFXDLL
			ASSERT(pMessageMap != (*pMessageMap->pfnGetBaseMap)());
#else
			ASSERT(pMessageMap != pMessageMap->pBaseMap);
#endif

			lpEntry = AfxFindMessageEntry(pMessageMap->lpEntries, nMsg, nCode,uiCommandID);

			if (lpEntry != NULL) {  // found it
				result = 0;      // �������
				mmf.pfn = (SERVICE_PMSG)lpEntry->pfn;
				switch (lpEntry->nSig) {
				case serviceSig_General:
					(this->*mmf.pfn_General)(wParam, (void*)(lParam));
					break;
				case serviceSig_v_ui:
					(this->*mmf.pfn_v_ui)(wParam);
					break;
				case serviceSig_l_wl:
					result =(this->*mmf.pfn_l_wl)(wParam,lParam);
					break;
				case serviceSig_v_PRequestItem:
					(this->*mmf.pfn_Service_PRequestItem)(wParam,(CCommand*)(lParam));
					break;
				case serviceSig_v_PException:
					(this->*mmf.pfn_Service_PException)((CSysException*)(wParam),(CCommand*)(lParam));
				default:
					ASSERT(FALSE);
					result = -1; // δ������
					break;
				}
			}
		}
		if(pExtra!=NULL)
		{
			if(pExtra->GetAutoDeleted())
			{
				delete pExtra;
				pExtra = NULL;
			}
		}
		return result;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}



