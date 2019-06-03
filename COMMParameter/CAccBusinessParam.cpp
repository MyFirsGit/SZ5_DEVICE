#include "StdAfx.h"
#include "CAccBusinessParam.h"
#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAccBusinessParam CAccBusinessParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAccBusinessParam::CAccBusinessParam():CAccParamMaster()
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
CAccBusinessParam::~CAccBusinessParam(void)
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
void CAccBusinessParam::GetParamFileName()
{
	GetParamFileNameSub(ACC_PARAM_ID_0101);		// ��ȡ�����ļ���
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ�������

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccBusinessParam::ParseBody(LPBYTE lpData)
{
	try{
		// ƫ��������	
		//lpData += m_section[0].startPosition;
		// �ֶ�1
		//if (m_section[0].recordCount != 0) { 
		//// ������������������
		//DWORD singleBlacklistCnt = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		//lpData +=4;	
		//// vector�ռ����
		//m_vec_single_blacklist_param.clear();
		//         m_vec_single_blacklist_param.reserve(singleBlacklistCnt);			

		//         for (unsigned int i=0; i<singleBlacklistCnt; i++) {  
		//	TK_SINGLE_BLACKLIST tempRecord;
		//             // ��ID��
		//             tempRecord.cardID = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		//             lpData += 4;
		//             // ����������
		//	tempRecord.cardID = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
		//             lpData += 2;
		//	// �������
		//             tempRecord.cardID = *lpData;
		//             lpData ++;	
		//	// ״̬����
		//             tempRecord.cardID = *lpData;
		//             lpData ++;
		//             m_vec_single_blacklist_param.push_back(tempRecord);
		//}	   
		//}
		// �ֶ�2
		//if (m_section[1].recordCount != 0) { 			 
		//// ��Χ��������������
		//WORD groupBlacklistCnt = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
		//lpData +=2;	
		//// vector�ռ����
		//m_vec_blacklist_group_param.clear();
		//         m_vec_blacklist_group_param.reserve(groupBlacklistCnt);			

		//         for (unsigned int i=0; i<groupBlacklistCnt; i++) {  
		//	TK_BLACKLIST_GROUP tempRecord;
		//             // ��ʼ��ID���
		//             tempRecord.dStartId = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		//             lpData += 4;
		//             // ���˿�ID���
		//	tempRecord.dEndId =  mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		//             lpData += 4;
		//	// �������
		//             tempRecord.cardActionCode = *lpData;
		//             lpData ++;	
		//	// ״̬����
		//             tempRecord.cardStatusCode = *lpData;
		//             lpData ++;
		//             m_vec_blacklist_group_param.push_back(tempRecord);
		//}	   
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
@brief      �����ֶ���Ϣ

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccBusinessParam::ParseSectionInfo(LPBYTE lpData)
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
bool CAccBusinessParam::CheckBodyInfo(LPBYTE lpData)
{   
	return true;
}

