#include "stdafx.h"
#include "TokenCyclerRfidCommand.h"


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
CTCRfidInit::CTCRfidInit()
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
CTCRfidInit::~CTCRfidInit()
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
long CTCRfidInit::ExecuteCommand()
{
	long lRet = 0;
	lRet = InitFunction();
	this->OnComplete(lRet);
	return lRet;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		�˺������� Request ���� �������Ч��Χ���Ƿ��п��Ĵ���

 @param		mode:�������� 
			mode=0:�������߷�Χ�� IDLE ״̬�Ŀ� HALT ״̬�ĳ��� 
			mode=1: �������߷�Χ�ڵ����п� 
			tagtype:���ص�Ŀ������

 @retval	unsigned char ����ֵ 0 ��ʾ�ɹ� ���򷵻ش����� 

 @exception	��

*/
//////////////////////////////////////////////////////////////////////////
long CTCRfidInit::InitFunction()
{
	CString sLogString(_T(""));
	SYNC(CS,_T("INIT"));

	sLogString.Format(_T("CTCRfidInit: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (m_bTokenCyclerRfidConnected == false){
		return CTokenCyclerRfidHardwareException::CS_ERR_NOT_INIT;
	}

	long errCode = E_NO_CS_ERROR;
	// 1. Request
	sLogString.Format(_T("zlg500B_request: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	unsigned char mode = 1;//0:�������߷�Χ�� IDLE ״̬�Ŀ� HALT ״̬�ĳ���;1: �������߷�Χ�ڵ����п�
	unsigned short tagType = 0;//���ص�Ŀ������
	//�������Ч��Χ���Ƿ��п��Ĵ���
	errCode = zlg500B_request(mode, tagType);
	if (E_NO_CS_ERROR != errCode){
		goto INIT_END;
	}

	// 2. Anticoll
	sLogString.Format(_T("zlg500B_anticoll: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	unsigned char bcnt = 0;// ΪԤѡ���������λ�ĸ��� ͨ�� Bcnt=0
	unsigned long cardSN = 0;//���ؿ������
	BYTE rfidData[8] = {0};
	//�˺�����ʼ����ͻ���� 
	errCode = zlg500B_anticoll(0, cardSN);
	if (E_NO_CS_ERROR != errCode){
		goto INIT_END;
	}

	// 3. Select
	sLogString.Format(_T("zlg500B_select: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	unsigned char cardSize = 0;// ���ؿ�������
	//�˺���ѡ��ĳһ����ŵĿ�
	errCode = zlg500B_select(cardSN, cardSize);
	if (E_NO_CS_ERROR != errCode){
		goto INIT_END;
	}

	// 4. Authen
	sLogString.Format(_T("zlg500B_authentication: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	unsigned char pwMode = 0;// ��Կ����,��Կ A:0x00,��Կ B:0x04
	unsigned char secnr = 1;// Ҫ��֤�Ŀ�������
	//�� RC500 �е��ܳ��뿨�е��ܳ׽�����֤ �ڽ��жԿ�Ƭ�Ķ�д����֮ǰ ����ɹ�ִ�д�ָ�� 
	//��ϵͳ��ʼ��ʱ �����豸����ÿ��������������ص��������� 
	errCode = zlg500B_authentication(pwMode, secnr);
	if (E_NO_CS_ERROR != errCode){
		goto INIT_END;
	}

INIT_END:
	sLogString.Format(_T("CTCRfidInit: errCode = %d"), errCode);
	theTokenCyclerRfid_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
