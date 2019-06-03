#pragma once
#include "TxnDataException.h"

// CSerialNumberMgr��Ψһ�ӿ�
#define theSELNO_MGR CSerialNumberMgr::GetInstance()              
 
/**
@brief  ���ݰ�ʱ���������
*/

class CSerialNumberMgr
{
public:
    // ��ô�����к�
	WORD GetSelNo(_DATE_TIME);				 
	
    static CSerialNumberMgr&  GetInstance(){
        return theInstance;
    }

private:	
	
    // ��Ա��������
	//_DATE_TIME	m_pekgDateTime;										// ʱ���
	WORD		m_pekgSelNo;										// ������к�
   
	CCriticalSection m_criticalSection;                             // �ؼ�������
		
    // ����,��������
    CSerialNumberMgr(void);
    ~CSerialNumberMgr();
    CSerialNumberMgr(const CSerialNumberMgr&);
    CSerialNumberMgr& operator=(const CSerialNumberMgr&);

    static  CSerialNumberMgr theInstance;   // �����Ψһ�ӿ�
};
