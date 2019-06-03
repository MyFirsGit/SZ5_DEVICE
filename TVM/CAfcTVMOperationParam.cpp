#include "StdAfx.h"
#include "CAfcTVMOperationParam.h"
#include "ParameterDef.h"
#include "CParamException.h"
//#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INVALID_VALUE		0xFF

CAfcTVMOperationParam CAfcTVMOperationParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAfcTVMOperationParam::CAfcTVMOperationParam(void):CAfcParamMaster()
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
CAfcTVMOperationParam::~CAfcTVMOperationParam(void)
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
void CAfcTVMOperationParam::GetParamFileName()
{
	GetParamFileNameSub(AFC_PARAM_ID_1011);		// ��ȡ�����ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ�������

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::ParseBody(LPBYTE lpData)
{
	try{
		//m_tpu_param.vecPaymentID.clear();
		// ƫ��������	
		lpData += m_section[0].startPosition;
		// �ֶ�1��TVMƱ����д����
		for(int iloop=0;iloop<m_section[0].recordCount;iloop++)
		{
			// ��Ʊ���������������		
			m_tpu_param.maxWriteErrorCnt = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// δд����ȫ�Ŀ��ȴ�ʱ��
			m_tpu_param.unwriteWaitTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// δд����ȫ�Ŀ����Դ���
			m_tpu_param.unwriteRetryTimes = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// Ԥ��
			lpData += 4;
		}
		// �ֶ�2��TVMά�޵�¼��ز���
		for(int iloop=0;iloop<m_section[1].recordCount ;iloop++)
		{
			//��������ʱ��
			m_maintain_param.pwdInputTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//��¼���Դ���
			m_maintain_param.loginRetryTimes = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//�޲����Զ��ǳ�ʱ��
			m_maintain_param.autoLogoffTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 2;
			//ά����δ�رձ���ʱ��
			m_maintain_param.doorUnlockAlarmTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// Ԥ��
			lpData += 4;
		}
		// �ֶ�3��TVMǮ�����
		for(int iloop=0;iloop<m_section[2].recordCount ;iloop++)
		{
			//********************��ƱTicket***********************//
			//��Ʊ���յ�ֽ�����1 1	BIN
			m_billbox_param.acceptSalesTicketBillValue1 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//��Ʊ���յ�ֽ�����2 1	BIN
			m_billbox_param.acceptSalesTicketBillValue2 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//��Ʊ���յ�ֽ�����3 1	BIN
			m_billbox_param.acceptSalesTicketBillValue3 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//��Ʊ���յ�ֽ�����4 1	BIN
			m_billbox_param.acceptSalesTicketBillValue4 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));
			lpData += 1;
			//��Ʊ���յ�ֽ�����5 1	BIN
			m_billbox_param.acceptSalesTicketBillValue5 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//��Ʊ���յ�ֽ�����6 1	BIN
			m_billbox_param.acceptSalesTicketBillValue6 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//��Ʊ���յ�ֽ�����7 1	BIN
			m_billbox_param.acceptSalesTicketBillValue7 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//��Ʊ���յ�ֽ�����8 1	BIN
			m_billbox_param.acceptSalesTicketBillValue8 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//��Ʊ���յ�ֽ�����9 1	BIN
			m_billbox_param.acceptSalesTicketBillValue9 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//��Ʊ���յ�ֽ�����10 1 BIN
			m_billbox_param.acceptSalesTicketBillValue10= *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//��Ʊ���յ����ֽ������ 2	BIN

			m_billbox_param.acceptSalesTicketMaxBillCnt= mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//��Ʊ���յ����ֽ���ܶ� 2	BIN
			m_billbox_param.acceptSalesTicketMaxBillAmt= mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;

			// ��Ʊ���յ�Ӳ����� 1 ��λ����
			m_billbox_param.acceptSalesCoinValue1 = *lpData;
			lpData++;
			// ��Ʊ���յ�Ӳ����� 2 ��λ��Ԫ
			m_billbox_param.acceptSalesCoinValue2 = *lpData;
			lpData++;
			//Ԥ��
			lpData += 2;
			//********************��ֵ***********************//
			//��ֵ���յ�ֽ�����1 1	BIN
			m_billbox_param.acceptChrgrBillValue1 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 1;
			//��ֵ���յ�ֽ�����2 1	BIN
			m_billbox_param.acceptChrgrBillValue2 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//��ֵ���յ�ֽ�����3 1	BIN
			m_billbox_param.acceptChrgrBillValue3 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//��ֵ���յ�ֽ�����4 1	BIN
			m_billbox_param.acceptChrgrBillValue4 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 1;
			//��ֵ���յ�ֽ�����5 1	BIN
			m_billbox_param.acceptChrgrBillValue5 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//��ֵ���յ�ֽ�����6 1	BIN
			m_billbox_param.acceptChrgrBillValue6 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//��ֵ���յ�ֽ�����7 1	BIN
			m_billbox_param.acceptChrgrBillValue7 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//��ֵ���յ�ֽ�����8 1	BIN
			m_billbox_param.acceptChrgrBillValue8 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//��ֵ���յ�ֽ�����9 1	BIN
			m_billbox_param.acceptChrgrBillValue9 = *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//��ֵ���յ�ֽ�����10 1	BIN
			m_billbox_param.acceptChrgrBillValue10= *lpData;//mstos(ComMakeWORD(*lpData,*(lpData + 1)));			
			lpData += 1;
			//��ֵ���յ����ֽ������ 2	BIN
			m_billbox_param.acceptChrgrMaxBillCnt= mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//��ֵ���յ����ֽ���ܶ� 2	BIN
			m_billbox_param.acceptChrgrMaxBillAmt= mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			// Ԥ��
			lpData += 4;
			//******************************************************************//
			//��Ӳ������ʱ�Ƿ�������Ʊ 1	BIN
			m_billbox_param.salesNoCoin =*lpData;	
			lpData += 1;
			//����Ӳ�ҵ��������       1    BIN
			m_billbox_param.acceptCoinMaxCnt =*lpData;
			lpData += 1;
			//Ӳ��������������       1    BIN
			m_billbox_param.chargeCoinMaxCnt =*lpData;
			lpData += 1;
			//��Ʊ�Ƿ�����ֽ������     1    BIN
			m_billbox_param.chargeBill =*lpData;
			lpData += 1;
			//ֽ���������ֽ������     1    BIN
			m_billbox_param.chargeBillMaxCnt =*lpData;
			lpData += 1;
			//ֽ���������ֽ���ܽ��   1    BIN
			m_billbox_param.chargeBillMaxAmt =*lpData;
			lpData += 1;
			// Ԥ��						9 BIN
			lpData += 9;
			// ��Ԯ��ť���ñ�ʶ			1 BIN
			m_billbox_param.IsEnableAssistanceBtn = *lpData;
			lpData += 1;
			//******************************************************************//
			//����ȡ��ʱ��	2	BIN
			m_billbox_param.cancelDealTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//�Ƿ�������ʾ��������	1	BIN
			m_billbox_param.showWaitScreen = *lpData;		
			lpData += 1;
			//���������л�ʱ��	2	BIN
			m_billbox_param.changeWaitScreenTime = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//ֽ��Ǯ�佫������	2	BIN
			m_billbox_param.willFullBillCnt     = mstos (ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//ֽ��Ǯ����������	2	BIN
			m_billbox_param.alreadyFullBillCnt  = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//Ӳ�һ�����A��������	2	BIN  
			m_billbox_param.coinCollectionWillFullCoinCnt     = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//Ӳ�һ�����A��������	2	BIN
			m_billbox_param.coinCollectionAlreadyFullCoinCnt  = mstos(ComMakeWORD(*lpData,*(lpData + 1)));	
			lpData += 2;
			//Ӳ��������A��������	2	BIN
			m_billbox_param.coinChangeWillEmptyCoinCnt = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//TVM���ٳ�Ʊ����	2	BIN(Ԥ��)
			m_billbox_param.minTicketCnt        = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//TVMֽ�������佫������	2	BIN(Ԥ��)
			m_billbox_param.willEmptyBillBoxCnt = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			//Ԥ���ֶ�
			lpData += 3;
			//***************************************************************************//
		}
		// �ֶ�4��TVMƱ�����
		for(int iloop=0;iloop<m_section[3].recordCount ;iloop++)
		{
			// CardƱ�佫��������ֵ
			m_magazine_param.TicketNearEmptyValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// CardƱ���ѿ�������ֵ
			m_magazine_param.TicketEmptyValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// CardƱ�佫��������ֵ
			m_magazine_param.TicketNearFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// CardƱ������������ֵ
			m_magazine_param.TicketFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));				
			lpData += 2;
			// Card��Ʊ�佫��������ֵ
			m_magazine_param.TicketInvalidNearFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			// Card��Ʊ������������ֵ
			m_magazine_param.TicketInvalidFullValue = mstos(ComMakeWORD(*lpData,*(lpData + 1)));		
			lpData += 2;
			// Ԥ��
			lpData += 4;
		}
		// �ֶ�5��TVM����ɲ���С��������
		for(int iloop=0;iloop<m_section[4].recordCount ;iloop++)
		{
			//�ɲ�ѯ������������ 
			m_txn_lookup_param.minCount = *lpData;			
			lpData ++;
			//�ɲ�ѯ����ʱ������
			m_txn_lookup_param.minTime = *lpData;			
			lpData ++;

			// ��־��������
			m_txn_lookup_param.logSaveDay = *lpData;
			lpData++;

			// �ѻ��������
			m_txn_lookup_param.maxOffLineDay = *lpData;
			lpData++;

			// ���ϴ����ױ�������
			m_txn_lookup_param.expiredTransKeepDay = *lpData;
			lpData++;
		}

		m_vecDiskWaringSet.clear();
		// �ֶ�6�����̿ռ䱨������
		for(int iloop = 0;iloop < m_section[5].recordCount;iloop++){
			// �����������
			WORD wDiskCount = *lpData;
			lpData++;

			// ������������ֵ
			for(int nCount = 0;nCount < wDiskCount;nCount++){
				DISK_WARN_SETTING diskWarnSet;
				// �������ID
				diskWarnSet.diskType = (DISK_TYPE)*lpData;
				lpData++;

				// ���̽���������ֵ���ٷֱȣ�
				diskWarnSet.wDiskNearFullPersent = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
				lpData += 2;

				// ���̽���������ֵ����λ��MB��
				diskWarnSet.wDiskNearFullValue	 = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
				lpData += 2;

				// �����������Ϸ�ֵ���ٷֱȣ�
				diskWarnSet.wDiskFullPersent	 = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
				lpData += 2;

				// �����������Ϸ�ֵ����λ��MB��
				diskWarnSet.wDiskFullValue		 = mstos(ComMakeWORD(*lpData,*(lpData + 1)));
				lpData += 2;

				m_vecDiskWaringSet.push_back(diskWarnSet);
			}
		}
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		//throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
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
void CAfcTVMOperationParam::ParseSectionInfo(LPBYTE lpData)
{
	try{
		if(m_packageHead.sectionCount != 0){			
			for(int i =0;i<m_packageHead.sectionCount && i < 6/* MaxCount */;i++){
				m_section[i].startPosition = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData+=4;
				m_section[i].recordCount = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData+=4;
			}
		}
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		//throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
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
bool CAfcTVMOperationParam::CheckBodyInfo(LPBYTE lpData)
{   
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���TVMƱ����д����

@param      (o)TPU_PARAM& temParam   TVMƱ����д����

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetTpuParam(TPU_PARAM& temParam)
{
	temParam = m_tpu_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���TVMά�޵�¼��ز���

@param      (o)MAINTAIN_PARAM& temParam   TVMά�޵�¼��ز���

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetMaintainParam(MAINTAIN_PARAM& temParam)
{
	temParam = m_maintain_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���TVMǮ�����

@param      (o)MAGAZINE_PARAM& operationTm   TVMƱ�����

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetBillboxParam(BILLBOX_PARAM& temParam)			
{
	temParam = m_billbox_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���TVMƱ�����

@param      (o)MAGAZINE_PARAM& operationTm   TVMƱ�����

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetMagazineParam(MAGAZINE_PARAM& temParam)			
{
	temParam = m_magazine_param;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Զ��ǳ��ȴ�ʱ�����

@param      none

@retval     �޲����ǳ���λ��

@exception  int   �Զ��ǳ��ȴ�ʱ�����
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetAutoLogoffTime()
{
	return m_maintain_param.autoLogoffTime;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �����������ʱ�����

@param      none

@retval     ��������ʱ��			��λ��

@exception  int   �Զ��ǳ��ȴ�ʱ�����
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetPwdInputTime()
{
	return m_maintain_param.pwdInputTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������¼���Դ���

@param      none

@retval     WORD ����

@exception  int   �Զ��ǳ��ȴ�ʱ�����
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetPwdInputMaxTimes(){
	return m_maintain_param.loginRetryTimes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ά����δ�رձ���ʱ��

@param      none

@retval     ��λ��

@exception  int   �Զ��ǳ��ȴ�ʱ�����
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetDoorUnlockAlarmTime()
{
	return m_maintain_param.doorUnlockAlarmTime;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �������֧����ʽID(�ѷ�)

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
const vector<BYTE>& CAfcTVMOperationParam::GetAllPaymentID()
{
	vector<BYTE> vecPaymentID;	// ֧����ʽID
	return vecPaymentID;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���VM���۶����ޣ��ѷϣ�

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD CAfcTVMOperationParam::GetMaxSellValue()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��úϲ�֧�����ñ�־���ѷϣ�

@param      none

@retval     1��true  0��false

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BYTE CAfcTVMOperationParam::GetCombinePayFlag()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��Cardʱ�ɽ��յ�ֽ����ֵ����ԪΪ��λ��

@param      vector<WORD>& vecFace ��ֵ�б�

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetAcceptBanknoteFaceOfIssueCard(vector<WORD>& vecFace)
{
	vecFace.clear();
	if (m_billbox_param.acceptSalesTicketBillValue1 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue1);
	if (m_billbox_param.acceptSalesTicketBillValue2 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue2);
	if (m_billbox_param.acceptSalesTicketBillValue3 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue3);
	if (m_billbox_param.acceptSalesTicketBillValue4 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue4);
	if (m_billbox_param.acceptSalesTicketBillValue5 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue5);
	if (m_billbox_param.acceptSalesTicketBillValue6 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue6);
	if (m_billbox_param.acceptSalesTicketBillValue7 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue7);
	if (m_billbox_param.acceptSalesTicketBillValue8 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue8);
	if (m_billbox_param.acceptSalesTicketBillValue9 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue9);
	if (m_billbox_param.acceptSalesTicketBillValue10 != INVALID_VALUE)
		vecFace.push_back(m_billbox_param.acceptSalesTicketBillValue10);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��Cardʱ�ɽ��յ�ֽ���������

@param      none

@retval     WORD

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetAcceptBanknoteMaxPieceOfIssueCard()
{
	return m_billbox_param.acceptSalesTicketMaxBillCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�۵���Ʊʱ�ɽ��յ�ֽ������ܶ�(��λΪԪ)

@param      none

@retval     WORD

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetAcceptBanknoteMaxAmoutOfIssueCard()
{
	return m_billbox_param.acceptSalesTicketMaxBillAmt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������е�Ӳ����ֵ����תΪ�Է�Ϊ��λ����ֵ

@param      BYTE code ��ֵ����

@retval     �Է�Ϊ��λ����ֵ

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::ConvertCoinFace(BYTE code)
{
	switch (code)
	{
	case 5://���
		return 50;
	case 10://һԪ
		return 100;
	default:
		return 0;
		/*case 3:
		return 200;
		case 4:
		return 500;*/
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ�֧��ָ����ֵ��Ӳ�ң���AVOSΪ��λ��

@param      (i)WORD face ��ֵ

@retval     bool

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcTVMOperationParam::SupportCoinFaceOfIssue(WORD face)
{
	vector<WORD> vecFace;
	GetAcceptCoinFaceOfIssue(vecFace);
	for (vector<WORD>::iterator iter = vecFace.begin(); iter != vecFace.end(); ++iter)
	{
		if (face == *iter) return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��Ʊʱ�ɽ��յ�Ӳ����ֵ���Է�Ϊ��λ��

@param      vector<WORD>& vecFace ��ֵ�б�

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetAcceptCoinFaceOfIssue(vector<WORD>& vecFace)
{
	vecFace.clear();

	WORD face = 0;
	face = ConvertCoinFace(m_billbox_param.acceptSalesCoinValue1);
	if (face != 0)
		vecFace.push_back(face);
	face = ConvertCoinFace(m_billbox_param.acceptSalesCoinValue2);
	if (face != 0)
		vecFace.push_back(face);
	face = ConvertCoinFace(m_billbox_param.acceptSalesCoinValue3);
	if (face != 0)
		vecFace.push_back(face);
	face = ConvertCoinFace(m_billbox_param.acceptSalesCoinValue4);
	if (face != 0)
		vecFace.push_back(face);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��Ʊʱ�ɽ��յ�Ӳ�����ö��

@param      none

@retval     WORD

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetAcceptCoinMaxPieceOfIssue()
{
	return m_billbox_param.acceptCoinMaxCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��Ʊʱ�������Ӳ�����ö��

@param      none

@retval     WORD

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetChageCoinMaxPieceOfIssue()
{
	return m_billbox_param.chargeCoinMaxCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetAcceptBanknotesFaceOfRecharge(vector<WORD>& vecFace){
	vecFace.clear();
	if (m_billbox_param.acceptChrgrBillValue1 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue1);
	}
	if (m_billbox_param.acceptChrgrBillValue2 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue2);
	}
	if (m_billbox_param.acceptChrgrBillValue3 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue3);
	}
	if(m_billbox_param.acceptChrgrBillValue4 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue4);
	}
	if(m_billbox_param.acceptChrgrBillValue5 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue5);
	}
	if(m_billbox_param.acceptChrgrBillValue6 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue6);
	}
	if (m_billbox_param.acceptChrgrBillValue7 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue7);
	}
	if (m_billbox_param.acceptChrgrBillValue8 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue8);
	}
	if (m_billbox_param.acceptChrgrBillValue9 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue9);
	}
	if(m_billbox_param.acceptChrgrBillValue10 != INVALID_VALUE){
		vecFace.push_back(m_billbox_param.acceptChrgrBillValue10);
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ֵʱ����ֽ���������

@param      none

@retval     WORD ����ֽ������

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetAcceptBanknotesMaxPieceOfRecharge(){
	return m_billbox_param.acceptChrgrMaxBillCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ֵʱ����ֽ������ܶ�(��λΪԪ)

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetAcceptBanknotesMaxAmountOfRecharge(){
	return m_billbox_param.acceptChrgrMaxBillAmt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ֵ����Ӳ�����

@param      (o)vector<WORD>& vecFace

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CAfcTVMOperationParam::GetAcceptCoinFaceOfRecharge(vector<WORD>& vecFace){
	vecFace.clear();
	WORD face = 0;
	face = ConvertCoinFace(m_billbox_param.acceptChrgrCoinValue1);
	if (face != 0)
		vecFace.push_back(face);
	face = ConvertCoinFace(m_billbox_param.acceptChrgrCoinValue2);
	if (face != 0)
		vecFace.push_back(face);
	face = ConvertCoinFace(m_billbox_param.acceptChrgrCoinValue3);
	if (face != 0)
		vecFace.push_back(face);
	face = ConvertCoinFace(m_billbox_param.acceptChrgrCoinValue4);
	if (face != 0)
		vecFace.push_back(face);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ֵ����Ӳ�ҵ����ö��

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetAcceptCoinMaxPieceOfRecharge(){
	return m_billbox_param.acceptCoinMaxCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��ȡ��ֵʱ��������ö�� 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetChageCoinMaxPieceOfRecharge(){
	return m_billbox_param.chargeCoinMaxCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		�Ƿ���ʾ��ӡ��ť  

@param      none

@retval     bool:  true��ʾ  false ����ʾ 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcTVMOperationParam::SupportPrintSwitch(){
	return true;//m_function_param.PrintSwitch;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ȡ������A ����ö��  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetChangeCoinBoxNearEmptyCount(){
	return m_billbox_param.coinChangeWillEmptyCoinCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ȡ������A ����ö��

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetRecycleBoxNearFullCount(){
	return m_billbox_param.coinCollectionWillFullCoinCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ȡ������A ����ö��  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetRecycleBoxFullCount(){
	return m_billbox_param.coinCollectionAlreadyFullCoinCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		ֽ���佫��ֵ  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetBanknotesNearFullCount(){
	return m_billbox_param.willFullBillCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		ֽ��������ֵ  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetBanknotesFullConut(){
	return m_billbox_param.alreadyFullBillCnt;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ȡTicket��������  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetTicketNearEmptyCount(){
	return m_magazine_param.TicketNearEmptyValue;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ȡTicket�ѿ�����  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetTicketEmptyCount(){
	return m_magazine_param.TicketEmptyValue;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ȡTicket��Ʊ�佫������  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetTicketInvalidNearFullCount(){
	return m_magazine_param.TicketInvalidNearFullValue;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ȡTicket��Ʊ����������  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetTicketInvalidFullCount(){
	return m_magazine_param.TicketInvalidFullValue;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ȡ���׳�ʱʱ�� 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetCancelDealTime(){
	return m_billbox_param.cancelDealTime > 0 ? m_billbox_param.cancelDealTime : 60;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ȡ�ȴ��е���Ϣ��״̬ʱ��  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetReadyToRestTime(){
	return m_billbox_param.changeWaitScreenTime;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��Ӳ�������Ƿ�������Ʊ

@param      ��

@retval     bool true��������false��������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcTVMOperationParam::SupportSaleWithoutCoinChange(){
	return m_billbox_param.salesNoCoin;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��Ʊ�Ƿ�����ֽ������

@param      ��

@retval     bool true���� false������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcTVMOperationParam::SupportBHChangeWithSale(){
	return m_billbox_param.chargeBill;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ����������ܽ���λ��Ԫ��

@param      ��

@retval     WORD ����ֽ������������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetBHChangeMaxAmount(){
	return m_billbox_param.chargeBillMaxAmt;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ƿ�������Ԯ��ť

@param      ��

@retval     bool true��������Ԯ��ť
				 false:������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcTVMOperationParam::IsEnableAssistanceBtn()
{
	if (0 == m_billbox_param.IsEnableAssistanceBtn)
		return false;
	else
		return true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ�������������

@param      ��

@retval     WORD������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetBHChangeMaxPiece(){
	return m_billbox_param.chargeBillMaxCnt;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡֽ�������佫������

@param      ��

@retval     WORD ������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetBanknoteChangeNearEmptyCount(){
	return m_billbox_param.willEmptyBillBoxCnt;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ȴ��е��������Ƿ�ʹ�ã�

@param      ��

@retval     bool true��ʹ��  false����ʹ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcTVMOperationParam::IsReadyToRestUsed(){
	return m_billbox_param.showWaitScreen;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡϵͳ�̽����ٷֱ���

@param      ��

@retval     WORD (�����ٷֱ�%)

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetSystemDiskNearFullPersent(){
	WORD wDiskNearFullPersent = 90;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_SYSTEM){
			wDiskNearFullPersent = iter->wDiskNearFullPersent;
			break;
		}
	}
	return wDiskNearFullPersent;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡϵͳ�̽���ֵ��MB��

@param      ��

@retval     DWORD wDiskNearFullValue;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetSystemDiskNearFullValue(){
	WORD wDiskNearFullValue = 200;	// default 200MB

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_SYSTEM){
			wDiskNearFullValue = iter->wDiskNearFullValue;
			break;
		}
	}
	return wDiskNearFullValue;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡϵͳ�������ٷֱȣ�%��

@param      ��

@retval     WORD wDiskFullPersent;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetSystemDiskFullPersent(){
	WORD wDiskFullPersent = 95;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_SYSTEM){
			wDiskFullPersent = iter->wDiskFullPersent;
			break;
		}
	}
	return wDiskFullPersent;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡϵͳ����������ֵ��MB��

@param      ��

@retval     WORD wDiskFullValue;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetSystemDiskFullValue(){
	WORD wDiskFullValue = 200;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_SYSTEM){
			wDiskFullValue = iter->wDiskFullValue;
			break;
		}
	}
	return wDiskFullValue;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ�����̽����ٷֱȣ�%��

@param      ��

@retval     WORD wDiskNearFullPersent;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetDataDiskNearFullPersent(){
	WORD wDiskNearFullPersent = 90;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_DATA){
			wDiskNearFullPersent = iter->wDiskNearFullPersent;
			break;
		}
	}
	return wDiskNearFullPersent;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ�����̽���ֵ��MB��

@param      ��

@retval     WORD wDiskNearFullValue;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetDataDiskNearFullValue(){
	WORD wDiskNearFullValue = 200;	// default 200MB

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_DATA){
			wDiskNearFullValue = iter->wDiskNearFullValue;
			break;
		}
	}
	return wDiskNearFullValue;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ�����������ٷֱȣ�%��

@param      ��

@retval     WORD wDiskFullPersent;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetDataDiskFullPersent(){
	WORD wDiskFullPersent = 95;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_DATA){
			wDiskFullPersent = iter->wDiskFullPersent;
			break;
		}
	}
	return wDiskFullPersent;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ����������ֵ��MB��

@param      ��

@retval     WORD wDiskFullValue;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetDataDiskFullValue(){
	WORD wDiskFullValue = 200;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_DATA){
			wDiskFullValue = iter->wDiskFullValue;
			break;
		}
	}
	return wDiskFullValue;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��־�̽����ٷֱȣ�%��

@param      ��

@retval     WORD wDiskNearFullPersent;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetLogDiskNearFullPersent(){
	WORD wDiskNearFullPersent = 90;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_LOG){
			wDiskNearFullPersent = iter->wDiskNearFullPersent;
			break;
		}
	}
	return wDiskNearFullPersent;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��־�̽���ֵ��MB��

@param      ��

@retval     WORD wDiskNearFullValue;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetLogDiskNearFullValue(){
	WORD wDiskNearFullValue = 200;	// default 200MB

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_LOG){
			wDiskNearFullValue = iter->wDiskNearFullValue;
			break;
		}
	}
	return wDiskNearFullValue;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��־����������ֵ�ٷֱȣ�%��

@param      ��

@retval     WORD wDiskFullPersent��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetLogDiskFullPersent(){
	WORD wDiskFullPersent = 95;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_LOG){
			wDiskFullPersent = iter->wDiskFullPersent;
			break;
		}
	}
	return wDiskFullPersent;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��־����������ֵ��MB��

@param      ��

@retval     WORD wDiskFullValue;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetLogDiskFullValue(){
	WORD wDiskFullValue = 200;	// default 90%

	for(auto iter = m_vecDiskWaringSet.begin();iter != m_vecDiskWaringSet.end();iter++){
		if(iter->diskType == DISK_TYPE_LOG){
			wDiskFullValue = iter->wDiskFullValue;
			break;
		}
	}
	return wDiskFullValue;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��־��������

@param      ��

@retval     WORD ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetMaxLogSaveDays(){
	return m_txn_lookup_param.logSaveDay;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ѻ���Ӫ�������

@param      ��

@retval     WORD ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetOfflineWorkDays(){
	return m_txn_lookup_param.maxOffLineDay;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ϴ����ױ�������

@param      ��

@retval     WORD ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CAfcTVMOperationParam::GetSendedMsgSaveDays(){
	return m_txn_lookup_param.expiredTransKeepDay;
}