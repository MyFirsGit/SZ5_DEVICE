#pragma once

#define theFunction_INFO CFunctionInfo::GetInstance()       // ��������Ψһ�ӿ�

/**
@brief FunctionInfo.INI����Ϣ����
*/
class CFunctionInfo
{
public:

    static CFunctionInfo&  GetInstance()
    {
        return theInstance;
    }

    int Initialize();                              // ���ڻ�

    // ������ȡ�����ݵķ���

    // [LINE]
	bool    IsLineEnabled(DWORD lineCode);           // �ж�ָ����·��ť�Ƿ����
    
	// [STATION]
	bool    IsStationEnabled(WORD stationCode);    // �ж�ָ����վ��ť�Ƿ�ɼ�

	// [MENU]
	bool IsMenuEnabled(WORD menuID,CString menuText);
    
	bool IsMenuVisibled(WORD menuID,CString menuText);

	bool IsTicketIssueEnabled(WORD TicketTypeId);	// �жϳ�Ʊ�Ƿ�ɷ���
private:
    static CFunctionInfo theInstance;
    CGetIni* m_functionIni;

	map<DWORD,bool> mapLineEnable;
	map<WORD,bool> mapStationEnable;
    // ���ع��캯��
    CFunctionInfo();
    ~CFunctionInfo();
    CFunctionInfo(const CFunctionInfo&);
    CFunctionInfo& operator=(const CFunctionInfo&);
};