#include "StdAfx.h"
#include "CAfcDeviceTimeParam.h"
//#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAfcDeviceTimeParam CAfcDeviceTimeParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAfcDeviceTimeParam::CAfcDeviceTimeParam(void):CAfcParamMaster()
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
CAfcDeviceTimeParam::~CAfcDeviceTimeParam(void)
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
void CAfcDeviceTimeParam::GetParamFileName()
{
	GetParamFileNameSub(AFC_PARAM_ID_1089);		// ��ȡ�����ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ�������

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcDeviceTimeParam::ParseBody(LPBYTE lpData)
{
    try{
		// ƫ��������	
		lpData += m_section[0].startPosition;
        // �ֶ�1���豸��Ӫʱ�����
		if (m_section[0].recordCount != 0) { 
			//��ʼ��Ӫʱ��
			m_operation_param.oprationBeginTime.bHour = BCD2int((char*)lpData,1);
			lpData ++;
			m_operation_param.oprationBeginTime.bMin = BCD2int((char*)lpData,1);
			lpData ++;
			//������Ӫʱ��
			m_operation_param.oprationEndTime.bHour = BCD2int((char*)lpData,1);
			lpData ++;
			m_operation_param.oprationEndTime.bMin = BCD2int((char*)lpData,1);
			lpData ++;
			//��Ӫ���л�ʱ��
			m_operation_param.oprationDateChangeTime.bHour = BCD2int((char*)lpData,1);
			lpData ++;
			m_operation_param.oprationDateChangeTime.bMin = BCD2int((char*)lpData,1);
			lpData ++;
        }
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
void CAfcDeviceTimeParam::ParseSectionInfo(LPBYTE lpData)
{
	try{
		if(m_packageHead.sectionCount != 0){			
			for(int i =0;i<m_packageHead.sectionCount;i++){
				m_section[i].startPosition = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData+=4;
				m_section[i].recordCount = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData+=4;
			}
		}
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
bool CAfcDeviceTimeParam::CheckBodyInfo(LPBYTE lpData)
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
void CAfcDeviceTimeParam::GetOperationTimeParam(OPRATION_TIME& operationTm)
{
	operationTm = m_operation_param;
}