#include "StdAfx.h"
#include "CAccSectionParam.h"
#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAccSectionParam CAccSectionParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAccSectionParam::CAccSectionParam():CAccParamMaster()
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAccSectionParam::~CAccSectionParam(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取参数文件名

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAccSectionParam::GetParamFileName()
{
	GetParamFileNameSub(ACC_PARAM_ID_0303);		// 获取参数文件名
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出当前文件的内容

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccSectionParam::ParseBody(LPBYTE lpData)
{
    try{
		// 偏移量设置	
		//lpData += m_section[0].startPosition;
        // 分段1
		//if (m_section[0].recordCount != 0) { 
			//// 单独黑名单卡总数量
			//DWORD singleBlacklistCnt = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
			//lpData +=4;	
			//// vector空间分配
			//m_vec_single_blacklist_param.clear();
   //         m_vec_single_blacklist_param.reserve(singleBlacklistCnt);			

   //         for (unsigned int i=0; i<singleBlacklistCnt; i++) {  
			//	TK_SINGLE_BLACKLIST tempRecord;
   //             // 卡ID号
   //             tempRecord.cardID = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
   //             lpData += 4;
   //             // 卡生命周期
			//	tempRecord.cardID = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
   //             lpData += 2;
			//	// 卡活动编码
   //             tempRecord.cardID = *lpData;
   //             lpData ++;	
			//	// 状态编码
   //             tempRecord.cardID = *lpData;
   //             lpData ++;
   //             m_vec_single_blacklist_param.push_back(tempRecord);
			//}	   
        //}

		//// 分段4 基本费率
		//if (m_section[3].recordCount != 0) { 			 
		//	// 循环次数
		//	lpData += m_section[3].startPosition;
		//	DWORD fareValueList_len = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		//	lpData +=4;	
		//						
  //          for (unsigned int i=0; i<fareValueList_len; i++) {  
		//		DWORD tempRecord;
  //              // 开始卡ID编号
  //              lpData += 2;
  //              // 终了卡ID编号
		//		tempRecord =  mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
  //              lpData += 4;
		//		m_fare.insert(map<int ,int >::value_type(tempRecord,tempRecord));
		//						
		//	}
		
      //  }
    }
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出分段信息

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccSectionParam::ParseSectionInfo(LPBYTE lpData)
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
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查电文结构调整部分内容正确性

@param      (i)LPBYTE           电文信息

@retval     bool   true:正确  false：不正确

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAccSectionParam::CheckBodyInfo(LPBYTE lpData)
{   
    return true;
}

