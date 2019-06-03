#include "StdAfx.h"
#include "CAccBasePriceParam.h"
#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAccBasePriceParam CAccBasePriceParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAccBasePriceParam::CAccBasePriceParam():CAccParamMaster()
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
CAccBasePriceParam::~CAccBasePriceParam(void)
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
void CAccBasePriceParam::GetParamFileName()
{
	GetParamFileNameSub(ACC_PARAM_ID_0601);		// ��ȡ�����ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ�������

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccBasePriceParam::ParseBody(LPBYTE lpData)
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

		//// �ֶ�4 ��������
		//if (m_section[3].recordCount != 0) { 			 
		//	// ѭ������
		//	lpData += m_section[3].startPosition;
		//	DWORD fareValueList_len = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		//	lpData +=4;	
		//						
  //          for (unsigned int i=0; i<fareValueList_len; i++) {  
		//		DWORD tempRecord;
  //              // ��ʼ��ID���
  //              lpData += 2;
  //              // ���˿�ID���
		//		tempRecord =  mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
  //              lpData += 4;
		//		m_fare.insert(map<int ,int >::value_type(tempRecord,tempRecord));
		//						
		//	}
		
      //  }
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
void CAccBasePriceParam::ParseSectionInfo(LPBYTE lpData)
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
bool CAccBasePriceParam::CheckBodyInfo(LPBYTE lpData)
{   
    return true;
}

