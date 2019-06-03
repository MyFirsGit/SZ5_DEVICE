#include "StdAfx.h"
#include "TimerMgr.h"
#include "SysException.h"
#include "InnerException.h"
#include "CoreAPI.h"
#include "Sync.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 实例化CTimerMgr的唯一对象
CTimerMgr CTimerMgr::theInstance;
CTimerMgr::TIMER_MAP CTimerMgr::m_timerMap;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTimerMgr::CTimerMgr()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTimerMgr::~CTimerMgr()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取该类的对象

@param      无

@retval     CTimerMgr&

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTimerMgr& CTimerMgr::GetInstance()
{
    return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      当Service结束时，强制关闭该服务中的所有TIMER

@param      无

@retval     CTimerMgr&

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTimerMgr::OnServiceClosed(CService* pService)
{
	this->StopTimer(*pService);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      启动定时器

@param      (i)CService& service    定时器所属的Service
@param      (i)UINT nTimerID        定时器编号
@param      (i)UINT nElapse         间隔(秒)

@retval     bool    true:成功;  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTimerMgr::StartTimer(CService& service, UINT nTimerID, UINT nElapse)
{
    try {
		SYNC(TIMERMGR,_T("TIMERMGR"));
        // 若定时器已启动，则先删除
        StopTimer(service, nTimerID);

        // 启动Windows定时器
        UINT_PTR idEvent = ::SetTimer(NULL, NULL, nElapse * 1000, TimerProc);

        if (idEvent == 0) {
            return false;
        }
		if(!HasServiceTimer(service))
		{
			//添加回调处理函数，确保服务结束时关闭TIMER
			service.Stopped.AddHandler(this,&CTimerMgr::OnServiceClosed);
		}
        // 加入到Map中
        TIMER_INFO timerInfo;
        timerInfo.service = &service;
        timerInfo.nTimerID = nTimerID;
        
        m_timerMap[idEvent] = timerInfo;

        return true;
    }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(service.GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      停止定时器

@param      (i)CService& service    定时器所属的Service
@param      (i)UINT nTimerID        定时器编号

@retval     bool    true:成功;  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTimerMgr::StopTimer(CService& service, UINT nTimerID)
{
    try {
		SYNC(TIMERMGR,_T("TIMERMGR"));
        UINT_PTR idEvent = FindTimer(service, nTimerID);

        if (idEvent > 0) {
            ::KillTimer(NULL, idEvent);
            m_timerMap.erase(idEvent);
        }
		if(!HasServiceTimer(service))
		{
			service.Stopped.RemoveHandler(this,&CTimerMgr::OnServiceClosed);
		}
        return true;
    }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(service.GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      停止定时器

@param      (i)CService& service    定时器所属的Service

@retval     bool    true:成功;  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTimerMgr::StopTimer(CService& service)
{
    try {
		SYNC(TIMERMGR,_T("TIMERMGR"));
        while (true) {
            TIMER_MAP::iterator pos = m_timerMap.begin();

            if (pos == m_timerMap.end()) {
                break;
            }

            bool found = false;

            while (pos != m_timerMap.end()) {
                UINT_PTR key = pos->first;
                TIMER_INFO value = pos->second;

                if (value.service->GetServiceID() == service.GetServiceID()) {
                    ::KillTimer(NULL, key);
                    m_timerMap.erase(key);
                    found = true;
                    break;
                }

                pos++;
            }

            if (! found) {
                break;
            }
        }
		service.Stopped.RemoveHandler(this,&CTimerMgr::OnServiceClosed);
        return true;
    }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(service.GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      定时器回调函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
VOID CALLBACK CTimerMgr::TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    try {
        TIMER_MAP::iterator pos = m_timerMap.find(idEvent);

        if (pos != m_timerMap.end()) {
            TIMER_INFO timerInfo = pos->second;
            timerInfo.service->PostMessage(ST_SERVICE_TIMER, timerInfo.nTimerID, NULL);
        }
    }
    catch (CSysException&) {
       throw;
    }
    catch (...) {
        throw CInnerException(0, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   在m_timerMap中根据service判断有没有TIMER   

@param (i)CService& service    所属的Service

@retval    bool 存在或不存在 

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTimerMgr::HasServiceTimer(CService& service)
{
	try {
		SYNC(TIMERMGR,_T("TIMERMGR"));
		for (TIMER_MAP::iterator pos = m_timerMap.begin(); pos != m_timerMap.end(); pos++) {
			TIMER_INFO& value = pos->second;

			if (value.service->GetServiceID() == service.GetServiceID()) {
				return true;
			}
		}

		return false;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(service.GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      在m_timerMap中根据service和nIDEvent查找

@param      (i)CService& service    所属的Service
@param      (i)UINT nTimerID        Service内的定时器编号

@retval     UINT_PTR   Windows定时器编号   >0:存在;  0:不存在

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT_PTR CTimerMgr::FindTimer(CService& service, UINT nTimerID)
{
    try {
		SYNC(TIMERMGR,_T("TIMERMGR"));
        UINT_PTR result = 0;

        for (TIMER_MAP::iterator pos = m_timerMap.begin(); pos != m_timerMap.end(); pos++) {
            if (pos->second.service->GetServiceID() == service.GetServiceID() && pos->second.nTimerID == nTimerID) {
                result = pos->first;
                break;
            }
        }

        return result;
    }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        throw CInnerException(service.GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}
