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
@brief	各项业务服务基类
*/

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      UINT serviceID            Service ID
@param      UINT parentServiceID      上级Service ID
@param      bool isBackground               是否是后台Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CService::CService(UINT serviceID, UINT parentServiceID, bool isBackground)
{
    // 初始化成员变量
    m_serviceID = serviceID;                            // Service ID
    m_parentServiceID = parentServiceID;                // 上级Service ID
    m_isBackground = isBackground;                      // 是否是后台Service

    m_pParent = NULL;                                   // 上级Service
	
	pBag = new CBag;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CService::~CService()
{
   delete pBag;
   pBag = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取一个容器

@param      无

@retval     CBag* 返回一个容器

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBag* CService::GetBag()
{
	return pBag;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      启动服务

@param      无

@retval     无

@exception  无
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
@brief      停止服务

@param      无

@retval     无

@exception  无
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
@brief      获取Service ID

@param      无

@retval     UINT      Service ID

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CService::GetServiceID()
{
    return m_serviceID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取上级Service ID

@param      无

@retval     UINT      上级Service ID

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CService::GetParentServiceID()
{
    return m_parentServiceID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取上级Service

@param      无

@retval     CService*   上级Service

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CService* CService::GetParent()
{
    return m_pParent;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取下级Service

@param      无

@retval     ServiceChildren& 下级Service

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CService::ServiceChildren& CService::GetChildren()
{
    return m_children;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否是后台Service

@param      无

@retval     bool    true:是后台Service;  false:不是后台Service

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CService::IsBackground()
{
    return m_isBackground;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否是前台Service

@param      无

@retval     bool    true:是前台Service;  false:不是前台Service

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CService::IsForeground()
{
    return !m_isBackground;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      添加子Service

@param      int index

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CService::AddChild(CService* pChild)
{
    try {
        pChild->m_pParent = this;       // 设置父Service
        m_children.push_back(pChild);         // 添加到数组中
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
@brief      删除全部子Service

@param      无

@retval     无

@exception  无
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
@brief      删除子Service

@param      CService* pChild

@retval     无

@exception  无
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

        if(i < m_children.size()) {     // 找到
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
@brief      停止子Service

@param      int index

@retval     无

@exception  无
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
@brief      删除子Service

@param      int index

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CService::DeleteChild(int index)
{
    try {
		UINT serviceID = m_children[index]->GetServiceID();
        m_children[index]->DeleteChildren();    // 删除子Service的全部子Service
        delete m_children[index];               // 销毁
       m_children.erase(m_children.begin() + index);             // 从数组中删除
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
@brief      在本Service中根据Service ID查找Service

@param      (i)UINT serviceID	        Service编号

@retval     CService*     NULL：未找到，其他：Serivere

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
@brief      启动定时器

@param      (i)UINT timerID 
@param      (i)int seconds  等待时间(秒)

@retval     bool true:成功 false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CService::StartTimer(UINT timerID, int seconds)
{
	bool bRet = false;
	// 启动计时器失败重试三次
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
@brief      停止定时器

@param      (i)UINT timerID 

@retval     无

@exception  无
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
		// 分解Commmand ID, 错误编码和命令对象
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
		LRESULT result = -1;    // 处理标志

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
				result = 0;      // 被处理过
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
					result = -1; // 未被处理
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



