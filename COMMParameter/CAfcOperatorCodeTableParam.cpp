#include "StdAfx.h"
#include "CAfcOperatorCodeTableParam.h"
//#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAfcOperatorCodeTableParam CAfcOperatorCodeTableParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAfcOperatorCodeTableParam::CAfcOperatorCodeTableParam(void):CAfcParamMaster()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAfcOperatorCodeTableParam::~CAfcOperatorCodeTableParam(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�����ļ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAfcOperatorCodeTableParam::GetParamFileName()
{
	//GetParamFileNameSub(AFC_PARAM_ID_1097);		// ��ȡ�����ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ�������

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcOperatorCodeTableParam::ParseBody(LPBYTE lpData)
{
    try{

    }
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ֶ���Ϣ

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcOperatorCodeTableParam::ParseSectionInfo(LPBYTE lpData)
{
	try{
		//if(m_packageHead.sectionCount != 0){			
		//	for(int i =0;i<m_packageHead.sectionCount;i++){
		//		m_section[i].startPosition = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		//		lpData+=4;
		//		m_section[i].recordCount = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		//		lpData+=4;
		//	}
		//}
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����Ľṹ��������������ȷ��

@param      (i)LPBYTE           ������Ϣ

@retval     bool   true:��ȷ  false������ȷ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcOperatorCodeTableParam::CheckBodyInfo(LPBYTE lpData)
{   
    return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����Ӫʱ�����

@param      (o)OPRATION_TIME& operationTm   ��Ӫʱ�����

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CAfcOperatorCodeTableParam::GetOperationTimeParam(OPRATION_TIME& operationTm)
//{
//	operationTm = m_operation_param;
//}