#include "StdAfx.h"
#include "BHException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
 @brief		ֽ�ҽ���ģ���쳣��
*/

//////////////////////////////////////////////////////////////////////////
/**
 @brief		���캯��

 @param		(i)DETAIL_ERROR_CODE lDetailID	ģ������ϸ����
 @param		(i)CString sFileName			�쳣����ʱ�ļ���
 @param		(i)long lLineNo					�쳣����ʱ�к�
 @param		(i)CString sMessage				������Ϣ

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHException::CBHException(long lDetailID, CString sFileName, long lLineNo, CString sMessage)
	:CSysException(MODULE_ID, lDetailID, sFileName, lLineNo, sMessage)
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHException::~CBHException()
{
}
