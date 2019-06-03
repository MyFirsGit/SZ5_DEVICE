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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CFunctionInfo::CFunctionInfo()
{
    try {
        m_functionIni = NULL;
        // ���ڻ�
        Initialize();
    }
    catch (...) {
        // �����쳣��Ϣ
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CFunctionInfo::~CFunctionInfo()
{
    delete m_functionIni;
    m_functionIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ�

@param       �� 

@retval      int 0 �ɹ���1 ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionInfo::Initialize() 
{
    try {
        // ���ѳ�ʼ������ֱ�ӷ���
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

        // �ļ����Ը�Ϊ��д
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
@brief       �ж�ָ����·��ť�Ƿ����

@param       BYTE lineCode ��·����

@retval      bool true:���� ; false:������

@exception   ��
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
@brief       �ж�ָ����վ��ť�Ƿ����

@param       WORD stationCode ��վ����

@retval      bool true:����ǰ�� ; false:������ǰ��

@exception   ��
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
@brief       �жϲ˵���ť�Ƿ����

@param       (i)WORD menuID			�˵�ID
@param       (i)CString menuText	��ť����

@retval      bool true:�ɼ� ; false:���ɼ�

@exception   ��
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
@brief       �ж�Ʊ���Ƿ�ɷ���

@param       (i)WORD TicketTypeId	Ʊ��ID
@param       (i)CString menuText	��ť����

@retval      bool true:�� ; false:����

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
