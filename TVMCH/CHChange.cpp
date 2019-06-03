#include "stdafx.h"
#include "CHCommand.h"
#include "CHException.h"
#include "CHHardwareException.h"
//#include "bh_proto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
/**
 @brief		���캯��

 @param		WORD num_OneY : 1Ԫ����
			WORD num_HalfY : 5�Ǹ���

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CCHChange::CCHChange(WORD num_OneY,WORD num_HalfY)
{
	m_num_OneY = num_OneY;
	m_num_HalfY = num_HalfY;

	memset(&changeSet, 0, sizeof(changeSet));
	changeSet.wMHopper1Num = m_num_OneY;
	changeSet.wMHopper2Num = m_num_HalfY;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CCHChange::~CCHChange()
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
long CCHChange::ExecuteCommand()
{
	long lRet = 1;
	lRet = ChangeFunction();
	this->OnComplete(lRet);
	return lRet;
}


long CCHChange::GetResponse(tCHAChangeNum* rsp)
{
	memcpy(rsp, &changeSet, sizeof(tCHAChangeNum));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		����

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CCHChange::ChangeFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("CHANGE"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	sLogString.Format(_T("iChangeEx: nChangeCoin1 = %d, nChangeCoin2 = %d"), m_num_OneY, m_num_HalfY);
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CH_ERROR;

	tCHADevReturn DevStatus = {0};
	errCode = CHA_ChangeCoin(&changeSet,&DevStatus);
	if (errCode != E_NO_CH_ERROR){
		sLogString.Format(_T("iChangeEx: change failed! errCode = %d, interface return change1 = %d, change2 = %d"), errCode, changeSet.wMHopper2Num, changeSet.wMHopper1Num);
		theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

		//if(changeSet.wMHopper1Num==m_num_OneY && changeSet.wMHopper2Num==m_num_HalfY){//��ʱ���֣�����ʧ�ܣ�δ�ҳ�Ӳ��ʱ���ӿڷ��ص�������Ŀ���������������Ŀ����˼����������жϡ�
		//	changeSet.wMHopper1Num = 0;
		//	changeSet.wMHopper2Num = 0;
		//}
	}
#if 0
	tModuleStatus sModuleStatus;
	tDevReturn sDevReturn[8];
	tChange chagneSet = {0};
	chagneSet.byMHopper1Num = m_num_HalfY;
	chagneSet.byMHopper2Num = m_num_OneY;
	/*
	ע������
		1����Դ��������ݴ��Ӳ��ģ�飬��������ӿں������øýӿڲ���������������
		2���������ɹ������Կ��ƻ�����ת����ࣻ�������ʧ�ܣ����Կ��ƻ�����ת���Ҳࡣ
	*/
	errCode = gOCAS002Dev.iChangeEx(&chagneSet, &sModuleStatus, sDevReturn);

	int nNo = 1;	// ��������� 1-1�� 2-2�� 
	int nDir = 1;   // �˶����� 1 = ���(�������ڷ���) 2 = �ұߣ�����Ǯ�䷽�� 3 = �м� 
	if (errCode != E_NO_CH_ERROR){
		errCode = sDevReturn[0].iLogicCode;	
		sLogString.Format(_T("iChangeEx: change failed! errCode = %d, interface return change1 = %d, change2 = %d"), errCode, chagneSet.byMHopper2Num, chagneSet.byMHopper1Num);
		theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
		gOCAS002Dev.iRouterControlEx(nNo, nDir, &sModuleStatus, sDevReturn);

		if(chagneSet.byMHopper2Num==m_num_OneY && chagneSet.byMHopper1Num==m_num_HalfY){//��ʱ���֣�����ʧ�ܣ�δ�ҳ�Ӳ��ʱ���ӿڷ��ص�������Ŀ���������������Ŀ����˼����������жϡ�
			chagneSet.byMHopper1Num = 0;
			chagneSet.byMHopper2Num = 0;
		}
	}
	else{
		gOCAS002Dev.iRouterControlEx(nNo, nDir, &sModuleStatus, sDevReturn);
	}
#endif

	//m_rsp.wOneYuanCount = changeSet.wMHopper1Num;
	//m_rsp.wHalfYuanCount = changeSet.wMHopper2Num;

	sLogString.Format(_T("iChangeEx: hopperChange1 = %d, hopperChange2 = %d"), changeSet.wMHopper1Num, changeSet.wMHopper2Num);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	sLogString.Format(_T("iChangeEx: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}
