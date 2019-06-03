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

// ʵ����CTimerMgr��Ψһ����
CTimerMgr CTimerMgr::theInstance;
CTimerMgr::TIMER_MAP CTimerMgr::m_timerMap;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTimerMgr::CTimerMgr()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTimerMgr::~CTimerMgr()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ����Ķ���

@param      ��

@retval     CTimerMgr&

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTimerMgr& CTimerMgr::GetInstance()
{
    return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Service����ʱ��ǿ�ƹرո÷����е�����TIMER

@param      ��

@retval     CTimerMgr&

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTimerMgr::OnServiceClosed(CService* pService)
{
	this->StopTimer(*pService);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ʱ��

@param      (i)CService& service    ��ʱ��������Service
@param      (i)UINT nTimerID        ��ʱ�����
@param      (i)UINT nElapse         ���(��)

@retval     bool    true:�ɹ�;  false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTimerMgr::StartTimer(CService& service, UINT nTimerID, UINT nElapse)
{
    try {
		SYNC(TIMERMGR,_T("TIMERMGR"));
        // ����ʱ��������������ɾ��
        StopTimer(service, nTimerID);

        // ����Windows��ʱ��
        UINT_PTR idEvent = ::SetTimer(NULL, NULL, nElapse * 1000, TimerProc);

        if (idEvent == 0) {
            return false;
        }
		if(!HasServiceTimer(service))
		{
			//��ӻص���������ȷ���������ʱ�ر�TIMER
			service.Stopped.AddHandler(this,&CTimerMgr::OnServiceClosed);
		}
        // ���뵽Map��
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
@brief      ֹͣ��ʱ��

@param      (i)CService& service    ��ʱ��������Service
@param      (i)UINT nTimerID        ��ʱ�����

@retval     bool    true:�ɹ�;  false:ʧ��

@exception  ��
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
@brief      ֹͣ��ʱ��

@param      (i)CService& service    ��ʱ��������Service

@retval     bool    true:�ɹ�;  false:ʧ��

@exception  ��
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
@brief      ��ʱ���ص�����

@param      ��

@retval     ��

@exception  ��
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
@brief   ��m_timerMap�и���service�ж���û��TIMER   

@param (i)CService& service    ������Service

@retval    bool ���ڻ򲻴��� 

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
@brief      ��m_timerMap�и���service��nIDEvent����

@param      (i)CService& service    ������Service
@param      (i)UINT nTimerID        Service�ڵĶ�ʱ�����

@retval     UINT_PTR   Windows��ʱ�����   >0:����;  0:������

@exception  ��
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
