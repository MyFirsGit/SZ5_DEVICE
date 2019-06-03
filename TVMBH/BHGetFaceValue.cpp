#include "stdafx.h"
#include "BHCommand.h"
#include "BHException.h"
//#include "bh_proto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief		���캯��

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHGetFaceValue::CBHGetFaceValue(BYTE* face_id)
{
	this->m_face_id = face_id;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHGetFaceValue::~CBHGetFaceValue()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ִ������

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHGetFaceValue::ExecuteCommand(void)
{
	long lRet = 1;
	lRet = GetFaceValueFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ȡ��ִ�н��

 @param		(o)BH_FACEVALUE* rsp  ��ֵ�ṹ��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHGetFaceValue::GetResponse(BH_FACEVALUE* rsp)
{
	try {
		*rsp = m_face_value;
	}
	catch (...){
		return CBHException::OTHER_ERR;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ֽ�ҽ���ģ��������ֵ

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHGetFaceValue::GetFaceValueFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}

	// ƴ������
	BYTE facecmd[5] = {'I', 'N', 'D'};
	memcpy(facecmd+3, m_face_id, 2);

	// ��������
	BYTE recvbuf[RES_MAX] = { 0 };
	DWORD recvlen = 0;
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand(sizeof(facecmd),facecmd, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	// ��֤��������������
	if (recvlen > 0){
		m_face_value.Deserialize(recvbuf);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	return 0;
}
