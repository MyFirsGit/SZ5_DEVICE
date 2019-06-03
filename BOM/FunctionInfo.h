#pragma once

#define theFunction_INFO CFunctionInfo::GetInstance()       // 功能配置唯一接口

/**
@brief FunctionInfo.INI的信息管理，
*/
class CFunctionInfo
{
public:

    static CFunctionInfo&  GetInstance()
    {
        return theInstance;
    }

    int Initialize();                              // 初期化

    // 以下是取得数据的方法

    // [LINE]
	bool    IsLineEnabled(DWORD lineCode);           // 判断指定线路按钮是否可用
    
	// [STATION]
	bool    IsStationEnabled(WORD stationCode);    // 判断指定车站按钮是否可见

	// [MENU]
	bool IsMenuEnabled(WORD menuID,CString menuText);
    
	bool IsMenuVisibled(WORD menuID,CString menuText);

	bool IsTicketIssueEnabled(WORD TicketTypeId);	// 判断车票是否可发售
private:
    static CFunctionInfo theInstance;
    CGetIni* m_functionIni;

	map<DWORD,bool> mapLineEnable;
	map<WORD,bool> mapStationEnable;
    // 隐藏构造函数
    CFunctionInfo();
    ~CFunctionInfo();
    CFunctionInfo(const CFunctionInfo&);
    CFunctionInfo& operator=(const CFunctionInfo&);
};