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
 @brief		构造函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHGetFaceValue::CBHGetFaceValue(BYTE* face_id)
{
	this->m_face_id = face_id;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHGetFaceValue::~CBHGetFaceValue()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		执行命令

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
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
 @brief		取得执行结果

 @param		(o)BH_FACEVALUE* rsp  面值结构体

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
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
 @brief		向纸币接收模块请求面值

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHGetFaceValue::GetFaceValueFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}

	// 拼接命令
	BYTE facecmd[5] = {'I', 'N', 'D'};
	memcpy(facecmd+3, m_face_id, 2);

	// 发送命令
	BYTE recvbuf[RES_MAX] = { 0 };
	DWORD recvlen = 0;
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand(sizeof(facecmd),facecmd, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	// 验证并解析返回数据
	if (recvlen > 0){
		m_face_value.Deserialize(recvbuf);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	return 0;
}
