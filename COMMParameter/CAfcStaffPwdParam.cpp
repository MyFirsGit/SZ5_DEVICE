#include "StdAfx.h"
#include "CAfcStaffPwdParam.h"
//#include "mlcEncrypt.h"
//#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAfcStaffPwdParam CAfcStaffPwdParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAfcStaffPwdParam::CAfcStaffPwdParam(void):CAfcParamMaster()
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
CAfcStaffPwdParam::~CAfcStaffPwdParam(void)
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
void CAfcStaffPwdParam::GetParamFileName()
{
	GetParamFileNameSub(AFC_PARAM_ID_1006);		// 获取参数文件名
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出当前文件的内容

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcStaffPwdParam::ParseBody(LPBYTE lpData)
{
    try{
		// 偏移量设置	
		lpData += m_section.startPosition;
       // 操作员数量
		DWORD staffCnt = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		lpData +=4;	
		// vector空间分配
		m_vec_staff_param.clear();
        m_vec_staff_param.reserve(staffCnt);			

        for (unsigned int i=0; i<staffCnt; i++) {  
			STAFF tempRecord;
            // 操作员 ID 
            tempRecord.staffID = BCD2int((char*)lpData,3);
            lpData += 3;
            // 操作员密码
			char CiperPassWord[8];
			char CiperPassWordOne[8];
			char* lpPassword = CiperPassWord;
			memset(CiperPassWord, 0, 6);
			memset(CiperPassWordOne, 0, 6);
			BCD2PSTR((char*)(lpData), 3, lpPassword);

			// 密码解密
			decrypt((const BYTE*)lpPassword, 6, CiperPassWordOne);
			USES_CONVERSION;
			tempRecord.staffPWD = A2T(CiperPassWordOne);
			lpData += 3;
			// 操作员密码有效截止日期
			tempRecord.pwdEndDate = BCD2Date((char*)lpData);
			lpData += 4;
            m_vec_staff_param.push_back(tempRecord);
		}	   
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
void CAfcStaffPwdParam::ParseSectionInfo(LPBYTE lpData)
{
	try{
		if(m_packageHead.sectionCount != 0){			
			m_section.startPosition = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
			lpData+=4;
			m_section.recordCount = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
			lpData+=4;
		}
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
bool CAfcStaffPwdParam::CheckBodyInfo(LPBYTE lpData)
{   
    return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得操作员参数

@param      none

@retval     vector<STAFF>  操作员参数记录体

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
vector<CAfcStaffPwdParam::STAFF> CAfcStaffPwdParam::GetStaffParam() 	
{
	return  m_vec_staff_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      操作员密码解密算法

@param      none

@retval     vector<STAFF>  操作员参数记录体

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAfcStaffPwdParam::decrypt(const BYTE* ciphertext, int len, char * plaintext)
{
	int i;
	for ( i = 0; i < len; i++ ) {
		if( ciphertext[ i ]  < ( i + 0x30 ) ) {
			plaintext[ i ] = 10 + ciphertext[ i ] - i;
		} else {
			plaintext[ i ] = ciphertext[ i ] - i;
		}
	}	
	plaintext[ len ] = '\0';
}