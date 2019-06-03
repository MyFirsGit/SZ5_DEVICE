#include "StdAfx.h"
#include "FunctionInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFunctionInfo CFunctionInfo::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CFunctionInfo::CFunctionInfo()
{
    try {
        m_functionIni = NULL;
        // 初期化
        Initialize();
    }
    catch (...) {
        // 忽略异常信息
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CFunctionInfo::~CFunctionInfo()
{
    delete m_functionIni;
    m_functionIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化

@param       无 

@retval      int 0 成功，1 失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::Initialize() 
{
    try {
        // 若已初始化，则直接返回
        if (m_functionIni != NULL) {
            return 0;
        }

        TCHAR szAppPath[_MAX_PATH];

        if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
			return 1;
        }

        CString fileName = CString(szAppPath) + FUNCTION_INI_FILE_NAME;

        // check Functioninfo.INI file exist
        if (FALSE == ComCheckFileExist(fileName)) {
			return 1;
        }

        // 文件属性改为可写
        SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

        m_functionIni = new CGetIni(fileName);

		return 0;
   }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
		throw CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
    }
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       判断指定线路按钮是否可用

@param       BYTE lineCode 线路编码

@retval      bool true:可用 ; false:不可用

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsLineEnabled(DWORD lineCode) 
{
	if(mapLineEnable.find(lineCode) == mapLineEnable.end())
	{
		CString slineCode(_T(""));
		slineCode.Format(_T("%d"),lineCode);
		DWORD lineStatus = m_functionIni->GetDataInt(_T("LINE_ENABLE"), slineCode, 1);
		mapLineEnable[lineCode] =  (lineStatus != 0);
	}
	return mapLineEnable[lineCode];
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       判断指定车站按钮是否可用

@param       WORD stationCode 车站编码

@retval      bool true:在最前面 ; false:不在最前面

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsStationEnabled(WORD stationCode)
{
	if(mapStationEnable.find(stationCode) == mapStationEnable.end())
	{
		CString sStationCode(_T(""));
		sStationCode.Format(_T("%d"),sStationCode);
		DWORD stationStatus = m_functionIni->GetDataInt(_T("STATION_ENABLE"), sStationCode, 1);
		mapStationEnable[stationCode] = (stationStatus != 0);
	}
	return mapStationEnable[stationCode];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断菜单按钮是否可用

@param       (i)WORD menuID			菜单ID
@param       (i)CString menuText	按钮文字

@retval      bool true:可见 ; false:不可见

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsMenuEnabled(WORD menuID,CString menuText)
{
	CString key(_T(""));
	key.Format(_T("%d-%s"),menuID,menuText);
	DWORD menuStatus = m_functionIni->GetDataInt(_T("MENU_ENABLE"), key, 1);
	return (menuStatus != 0);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsMenuVisibled(WORD menuID,CString menuText)
{
	CString key(_T(""));
	menuText.Replace(_T("\n"),_T(""));
	key.Format(_T("%d-%s"),menuID,menuText);
	DWORD menuStatus = m_functionIni->GetDataInt(_T("MENU_VISIBLE"), key, 1);
	return (menuStatus != 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断票种是否可发售

@param       (i)WORD TicketTypeId	票种ID
@param       (i)CString menuText	按钮文字

@retval      bool true:可 ; false:不可

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
bool CFunctionInfo::IsTicketIssueEnabled(WORD TicketTypeId)
{
	CString key(_T(""));
	key.Format(_T("%d"),TicketTypeId);
	DWORD menuStatus = m_functionIni->GetDataInt(_T("ISSUE_ENABLE"), key, 0);
	return (menuStatus != 0);
}
