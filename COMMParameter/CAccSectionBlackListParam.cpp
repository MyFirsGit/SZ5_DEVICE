#include "StdAfx.h"
#include "CAccSectionBlackListParam.h"
#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAccSectionBlackListParam CAccSectionBlackListParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAccSectionBlackListParam::CAccSectionBlackListParam():CAccParamMaster()
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
CAccSectionBlackListParam::~CAccSectionBlackListParam(void)
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
void CAccSectionBlackListParam::GetParamFileName()
{
	GetParamFileNameSub(ACC_PARAM_ID_0702);		// ��ȡ�����ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ�������

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccSectionBlackListParam::ParseBody(LPBYTE lpData)
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
void CAccSectionBlackListParam::ParseSectionInfo(LPBYTE lpData)
{
	try{
		/*if(m_packageHead.sectionCount != 0){			
			for(int i =0;i<m_packageHead.sectionCount;i++){
				m_section[i].startPosition = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData+=4;
				m_section[i].recordCount = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData+=4;
			}
		}*/
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
bool CAccSectionBlackListParam::CheckBodyInfo(LPBYTE lpData)
{   
    return true;
}
