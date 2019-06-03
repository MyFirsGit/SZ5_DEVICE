#pragma once
#include "Service.h"
#include <map>

using namespace std;

/**
@brief      定时器管理
*/
class CTimerMgr
{
public:
	CTimerMgr();                                                    // 构造函数
	~CTimerMgr();                                                   // 析构函数
    static CTimerMgr& GetInstance();                                // 获取该类的对象
    bool StartTimer(CService& service, UINT nTimerID, UINT nElapse);// 启动定时器
    bool StopTimer(CService& service, UINT nTimerID);               // 停止定时器
    bool StopTimer(CService& service);                              // 停止Service中所有定时器
    static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);   // 定时器回调函数

private:
    UINT_PTR FindTimer(CService& service, UINT nTimerID);           // 在m_timerMap中根据service和nIDEvent查找
	void OnServiceClosed(CService* pService);
	bool HasServiceTimer(CService& service);						// 判断是否为指定的Service的Timer

    // 定时器信息结构
    typedef struct _TIMER_INFO {
        CService* service;      // 所属的Service
        UINT nTimerID;          // Service内的定时器编号
            _TIMER_INFO()
            {
                service = NULL;
                nTimerID = 0;
            }
    } TIMER_INFO;

    typedef map<UINT_PTR, TIMER_INFO> TIMER_MAP;

    static TIMER_MAP m_timerMap;                                    // Windows定时器编号与定时器信息的映射
    static CTimerMgr theInstance;                                   // 该类的唯一接口
};
