#pragma once
#include "Service.h"
#include <map>

using namespace std;

/**
@brief      ��ʱ������
*/
class CTimerMgr
{
public:
	CTimerMgr();                                                    // ���캯��
	~CTimerMgr();                                                   // ��������
    static CTimerMgr& GetInstance();                                // ��ȡ����Ķ���
    bool StartTimer(CService& service, UINT nTimerID, UINT nElapse);// ������ʱ��
    bool StopTimer(CService& service, UINT nTimerID);               // ֹͣ��ʱ��
    bool StopTimer(CService& service);                              // ֹͣService�����ж�ʱ��
    static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);   // ��ʱ���ص�����

private:
    UINT_PTR FindTimer(CService& service, UINT nTimerID);           // ��m_timerMap�и���service��nIDEvent����
	void OnServiceClosed(CService* pService);
	bool HasServiceTimer(CService& service);						// �ж��Ƿ�Ϊָ����Service��Timer

    // ��ʱ����Ϣ�ṹ
    typedef struct _TIMER_INFO {
        CService* service;      // ������Service
        UINT nTimerID;          // Service�ڵĶ�ʱ�����
            _TIMER_INFO()
            {
                service = NULL;
                nTimerID = 0;
            }
    } TIMER_INFO;

    typedef map<UINT_PTR, TIMER_INFO> TIMER_MAP;

    static TIMER_MAP m_timerMap;                                    // Windows��ʱ������붨ʱ����Ϣ��ӳ��
    static CTimerMgr theInstance;                                   // �����Ψһ�ӿ�
};
