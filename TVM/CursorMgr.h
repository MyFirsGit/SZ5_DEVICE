#pragma once

#define theCURSOR_MGR CCursorMgr::GetInstance()

/**
/@brief ������
*/

class CCursorMgr
{
public:
    static CCursorMgr& GetInstance();           // ��ȡ����Ķ���
    void BeginWaitCursor();                     // ��ʾ�ȴ����
    void EndWaitCursor();                       // �����ȴ����
    void RestoreWaitCursor();                   // �ָ��ȴ����

private:
    CCursorMgr(void);                           // ���캯��
    ~CCursorMgr(void);                          // ��������

private:
    static CCursorMgr theInstance;              // �����Ψһ�ӿ�
    int m_displayCount;                         // �����ʾ����
    bool m_isWaitCursor;                        // �Ƿ��ǵȴ����
};
