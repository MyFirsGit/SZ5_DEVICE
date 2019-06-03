#include "StdAfx.h"
#include "CAfcBomOperationParam.h"
//#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAfcBomOperationParam CAfcBomOperationParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAfcBomOperationParam::CAfcBomOperationParam(void):CAfcParamMaster()
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
CAfcBomOperationParam::~CAfcBomOperationParam(void)
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
void CAfcBomOperationParam::GetParamFileName()
{
	//GetParamFileNameSub(AFC_PARAM_ID_1021);		// ��ȡ�����ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ�������

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcBomOperationParam::ParseBody(LPBYTE lpData)
{
    try{
		// ƫ��������	
		lpData += m_section[0].startPosition;
        // �ֶ�1��BOMƱ����д����
		if (m_section[0].recordCount != 0) { 
			// ��Ʊ���������������		
			m_tpu_param.maxWriteErrorCnt = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// δд����ȫ�Ŀ��ȴ�ʱ��
            m_tpu_param.unwriteWaitTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// δд����ȫ�Ŀ����Դ���
			m_tpu_param.unwriteRetryTimes = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// Ԥ��
			lpData += 4;
        }
		// �ֶ�2��BOMά�޵�¼��ز���
		if (m_section[1].recordCount != 0) { 
			//��������ʱ��
			m_maintain_param.pwdInputTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//��¼���Դ���
			m_maintain_param.loginRetryTimes = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//�޲����Զ��ǳ�ʱ��
			m_maintain_param.autoLogoffTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 2;
			//ά����δ�رձ���ʱ��
			m_maintain_param.doorUnlockAlarmTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// Ԥ��
			lpData += 4;
        }
		// �ֶ�3��BOMƱ�����
		if (m_section[2].recordCount != 0) { 
			// Ʊ�佫��������ֵ
			m_magazine_param.willEmptyValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// Ʊ���ѿ�������ֵ
			m_magazine_param.haveEmptyValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// Ʊ�佫��������ֵ
			m_magazine_param.willFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// Ʊ������������ֵ
			m_magazine_param.haveFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// ��Ʊ�佫��������ֵ
			m_magazine_param.willInvalidFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			// ��Ʊ������������ֵ
			m_magazine_param.haveInvalidFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			// Ԥ��
			lpData += 4;
		}
		// �ֶ�4��BOM����ɲ���С��������
		if (m_section[3].recordCount != 0) { 
			//�ɲ�ѯ������������ 
			m_txn_lookup_param.minCount = *lpData;			
			lpData ++;
			//�ɲ�ѯ����ʱ������
			m_txn_lookup_param.minTime = *lpData;			
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
void CAfcBomOperationParam::ParseSectionInfo(LPBYTE lpData)
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
bool CAfcBomOperationParam::CheckBodyInfo(LPBYTE lpData)
{   
    return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���BOMƱ����д����

@param      (o)TPU_PARAM& temParam   BOMƱ����д����

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAfcBomOperationParam::GetTpuParam(TPU_PARAM& temParam)
{
	temParam = m_tpu_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���BOMά�޵�¼��ز���

@param      (o)MAINTAIN_PARAM& temParam   BOMά�޵�¼��ز���

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAfcBomOperationParam::GetMaintainParam(MAINTAIN_PARAM& temParam)
{
	temParam = m_maintain_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���BOMƱ�����

@param      (o)MAGAZINE_PARAM& operationTm   BOMƱ�����

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAfcBomOperationParam::GetMagazineParam(MAGAZINE_PARAM& temParam)			
{
	temParam = m_magazine_param;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���BOM����ɲ���С��������

@param      (o)TXN_LOOKUP_PARAM& temParam   BOM����ɲ���С��������

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAfcBomOperationParam::GetTxnLookupParam(TXN_LOOKUP_PARAM& temParam)
{
	temParam = m_txn_lookup_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Զ��ǳ��ȴ�ʱ�����

@param      none

@retval     none

@exception  int   �Զ��ǳ��ȴ�ʱ�����
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcBomOperationParam::GetAutoLogoffTime()
{
	return m_maintain_param.autoLogoffTime;
}