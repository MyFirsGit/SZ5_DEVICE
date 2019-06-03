#pragma once
#include "afx.h"
#include "CoreExp.h"
#include "Bag.h"

/**
@brief	�쳣������
*/
class CORE_API CSysException
{

public:
    CSysException(long moduleID, long detailID, CString fileName, long lineNo, CString message =  CString(_T("")));
    virtual ~CSysException();
    CSysException(const CSysException& that);

    long GetModuleID();                         // ��ȡģ�����
    long GetDetailID();                         // ��ȡģ������ϸ����
    CString GetFileName();                      // ��ȡ�쳣����ʱ�ļ���
    long GetLineNo();                           // ��ȡ�쳣����ʱ�к�
    CString GetMessage();                       // ��ȡ���Ӵ�����Ϣ
    COleDateTime GetOccurTime();                // ��ȡ�쳣����ʱ��
	CBag& GetBag(); 
private:
    long m_moduleID;                            // ģ�����
    long m_detailID;                            // ģ������ϸ����
    CString m_fileName;                         // �쳣����ʱ�ļ���
    long m_lineNo;                              // �쳣����ʱ�к�
    CString m_message;                          // ���Ӵ�����Ϣ
    COleDateTime m_occurTime;                   // �����쳣ʱ��
	CBag m_Bag;
};
