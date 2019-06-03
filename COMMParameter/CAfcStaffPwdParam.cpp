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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAfcStaffPwdParam::CAfcStaffPwdParam(void):CAfcParamMaster()
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
CAfcStaffPwdParam::~CAfcStaffPwdParam(void)
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
void CAfcStaffPwdParam::GetParamFileName()
{
	GetParamFileNameSub(AFC_PARAM_ID_1006);		// ��ȡ�����ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ�������

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcStaffPwdParam::ParseBody(LPBYTE lpData)
{
    try{
		// ƫ��������	
		lpData += m_section.startPosition;
       // ����Ա����
		DWORD staffCnt = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		lpData +=4;	
		// vector�ռ����
		m_vec_staff_param.clear();
        m_vec_staff_param.reserve(staffCnt);			

        for (unsigned int i=0; i<staffCnt; i++) {  
			STAFF tempRecord;
            // ����Ա ID 
            tempRecord.staffID = BCD2int((char*)lpData,3);
            lpData += 3;
            // ����Ա����
			char CiperPassWord[8];
			char CiperPassWordOne[8];
			char* lpPassword = CiperPassWord;
			memset(CiperPassWord, 0, 6);
			memset(CiperPassWordOne, 0, 6);
			BCD2PSTR((char*)(lpData), 3, lpPassword);

			// �������
			decrypt((const BYTE*)lpPassword, 6, CiperPassWordOne);
			USES_CONVERSION;
			tempRecord.staffPWD = A2T(CiperPassWordOne);
			lpData += 3;
			// ����Ա������Ч��ֹ����
			tempRecord.pwdEndDate = BCD2Date((char*)lpData);
			lpData += 4;
            m_vec_staff_param.push_back(tempRecord);
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
bool CAfcStaffPwdParam::CheckBodyInfo(LPBYTE lpData)
{   
    return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ò���Ա����

@param      none

@retval     vector<STAFF>  ����Ա������¼��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
vector<CAfcStaffPwdParam::STAFF> CAfcStaffPwdParam::GetStaffParam() 	
{
	return  m_vec_staff_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ա��������㷨

@param      none

@retval     vector<STAFF>  ����Ա������¼��

@exception  ��
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