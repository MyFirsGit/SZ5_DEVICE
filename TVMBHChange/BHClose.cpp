#include "stdafx.h"
#include "BHChangeCommand.h"
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
CBHChangeClose::CBHChangeClose()
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
CBHChangeClose::~CBHChangeClose()
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
long CBHChangeClose::ExecuteCommand()
{
	long lRet = 1;
	lRet = CloseFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		�رմ���

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeClose::CloseFunction()
{
	DWORD errCode = BHTRS_RES_OK;

	// ���򿪣�����Ҫ�ر�
	if (m_hBH != NULL){
		errCode = BHTrs_Close(m_hBH);
		m_hBH = NULL;
		if (errCode != BHTRS_RES_OK){
			return CBHChangeException::ERROR_BH_CLOSE_FAILED;
		}
	}

	return 0;
}
