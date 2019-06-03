#pragma once
#include "TxnDataException.h"

// CSerialNumberMgr的唯一接口
#define theSELNO_MGR CSerialNumberMgr::GetInstance()              
 
/**
@brief  数据包时间戳管理类
*/

class CSerialNumberMgr
{
public:
    // 获得打包序列号
	WORD GetSelNo(_DATE_TIME);				 
	
    static CSerialNumberMgr&  GetInstance(){
        return theInstance;
    }

private:	
	
    // 成员变量定义
	//_DATE_TIME	m_pekgDateTime;										// 时间戳
	WORD		m_pekgSelNo;										// 打包序列号
   
	CCriticalSection m_criticalSection;                             // 关键区控制
		
    // 构造,析构函数
    CSerialNumberMgr(void);
    ~CSerialNumberMgr();
    CSerialNumberMgr(const CSerialNumberMgr&);
    CSerialNumberMgr& operator=(const CSerialNumberMgr&);

    static  CSerialNumberMgr theInstance;   // 该类的唯一接口
};
