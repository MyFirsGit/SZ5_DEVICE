#include "StdAfx.h"
#include "CAfcDeviceCommonParam.h"
//#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAfcDeviceCommonParam CAfcDeviceCommonParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAfcDeviceCommonParam::CAfcDeviceCommonParam(void):CAfcParamMaster()
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
CAfcDeviceCommonParam::~CAfcDeviceCommonParam(void)
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
void CAfcDeviceCommonParam::GetParamFileName()
{
	GetParamFileNameSub(AFC_PARAM_ID_1005);		// ��ȡ�����ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ�������

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcDeviceCommonParam::ParseBody(LPBYTE lpData)
{
    try{
		// ƫ��������	
		lpData += m_section[0].startPosition;
        // �ֶ�1����·�趨����
		if (m_section[0].recordCount != 0) { 
			// Ԥ���ֶ�
			lpData +=16;	
        }
		// �ֶ�2����Ӫʱ�����
		if (m_section[1].recordCount != 0) { 	
			// 24Сʱ��ӪʱƱ���ӳ�ʹ��ʱ��
			m_operation_param.delayTime24H = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));			
			lpData += 4;
			// SLE���Ӽ��ʱ����
			m_operation_param.connectCheckInterval = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// Ԥ��
			lpData += 2;
        }
		// �ֶ�3��ͨѶ���� 
		if (m_section[2].recordCount != 0) { 	
			// �������ݴ��ͼ��		2	BIN	��λ���ӣ�Ĭ��Ϊ1����
			m_communication_param.txnInterval = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// ���ʱ����			2	BIN	��λ���ӣ�Ĭ��ֵΪ15����
			m_communication_param.auditInterval = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// ҵ�����ݴ��ͼ��		2	BIN	��λ���ӣ�Ĭ��ֵΪ15����
			m_communication_param.businessInterval = mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 2;
			// ǮƱ�����ݲ������	2	BIN	��λ���ӣ�Ĭ��ֵΪ30����
			m_communication_param.magazineInterval = mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 2;
			// ʱ��ͬ��ʱ����		2	BIN	��λ���ӣ�����������ʱ��ͬ�������Ĭ��15����
			m_communication_param.ntpInterval = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// ͬ��ʱ�䱨����ֵ		2	BIN	��λ���롣Ĭ��60�롣
			m_communication_param.ntpAlarmValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// ͬ��ʱ����ϲ�ֵ		2	BIN	��λ���롣Ĭ��300�롣
			m_communication_param.ntpMalfunctionValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			// Ԥ��
			lpData += 2;
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
void CAfcDeviceCommonParam::ParseSectionInfo(LPBYTE lpData)
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
bool CAfcDeviceCommonParam::CheckBodyInfo(LPBYTE lpData)
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
void CAfcDeviceCommonParam::GetOperationTimeParam(OPRATION_TIME& operationTm)
{
	operationTm = m_operation_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ͨѶ����

@param      (o)COMMUNICATION& communicationParam ͨѶ����

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAfcDeviceCommonParam::GetCommunication(COMMUNICATION& communicationParam)
{
	communicationParam = m_communication_param;
}