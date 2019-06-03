#include "StdAfx.h"
#include "ULCardInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CULCard::CULCard(void)
{
    Initialize();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CULCard::~CULCard(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CULCard::Initialize()
{
	// Ʊ��������Ϣ�ṹ��
	applicationData.PublisherCode = 0;					// ����������
	applicationData.CardPhysicalType = 0;				// Ʊ����������
	memset(applicationData.CardPhysicalNumber,0,8);		// Ʊ��������
	applicationData.CardLogicalNumber = 0;
	applicationData.CardAppMainType = 0;				// ��Ӧ��������
	applicationData.CardAppSubType = 0;					// ��Ӧ��������
	applicationData.TestCard = 0;						// ����Ʊ��ʶ
	applicationData.BatchNumber = 0;					// Ʊ�����κ�
	applicationData.AppVersion = 0;						// Ӧ�ð汾��
	applicationData.TicketStatus = 0;					// ��Ʊ״̬
	applicationData.CardActived = 0;
	applicationData.SaleStation = 0;					// ���۳�վ
	applicationData.Deposit = 0;						// Ѻ��
	applicationData.Cost = 0;							// �ɱ�
	applicationData.SalePrice = 0;						// ��Ʊ���
	applicationData.Balance = 0;						// Ʊ�����

	// �ó���Ϣ�ṹ��
	journeyData.JourneyStatus = 0;						// �ó�״̬
	journeyData.EnterStation = 0;						// ��վվ��
	memset(journeyData.EnterDeviceID,0,6);				// ��վ�豸 
	journeyData.ExitStation = 0;						// ��վվ��
	memset(journeyData.ExitDeviceID,0,6);				// ��վ�豸

	// ��ʷ��¼
	historyData.clear();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ��ULƱ����Ϣ

@param      (i)BYTE* cardInfo ���������ص�Ʊ����Ϣ

@retval     bool true:�ɹ� false:ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CULCard::GetCardInfo(BYTE* cardInfo)
{
	bool ret = true;
	try {
		LPBYTE pCardinfo = cardInfo;

		// UID
		pCardinfo += 8;

		// ������ˮ��
		applicationData.CardLogicalNumber = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
		pCardinfo += 4;

		// �����̴���
		applicationData.PublisherCode = *pCardinfo;
		pCardinfo++;

		// ��Ʊ������
		pCardinfo++;

		// ��Ӧ�ñ�ʶ
		applicationData.TestCard = *pCardinfo;
		pCardinfo++;

		// ��Կ�汾
		pCardinfo++;

		// ��Ӧ�ð汾
		applicationData.AppVersion = *pCardinfo;
		pCardinfo++;

		// ��������
		applicationData.BatchNumber = ComMakeWORD(*(pCardinfo + 1),*pCardinfo);
		pCardinfo += 2;

		// ������
		applicationData.CardAppMainType = *pCardinfo; 
		pCardinfo++;

		// ��������
		applicationData.PublishDate.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		applicationData.PublishDate.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.PublishDate.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;

		// �������ӱ�ʶ
		pCardinfo++;

		// ������֤��MAC1
		pCardinfo += 4;

		// Ʊ���ࣨ��Ӧ�����ͣ�
		applicationData.CardAppMainType = applicationData.CardAppMainType - (*pCardinfo);
		pCardinfo++;
		
		// ����վ��1����·+վ�㣩
		applicationData.SaleStation = ComMakeWORD(*(pCardinfo+1),*pCardinfo);
		pCardinfo += 2;

		// ����վ��2����·+վ�㣩
		pCardinfo += 2;

		// Ǯ����λ           	
		pCardinfo++;

		// ��Ч��ʼʱ��
		applicationData.ValidStartTime.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		applicationData.ValidStartTime.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidStartTime.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidStartTime.biHour = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidStartTime.biMinute = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidStartTime.biSecond = BCD2int((char*)pCardinfo,1);
		pCardinfo++;

		// ��Ч����ʱ��
		applicationData.ValidEndTime.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		applicationData.ValidEndTime.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidEndTime.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidEndTime.biHour = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidEndTime.biMinute = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.ValidEndTime.biSecond = BCD2int((char*)pCardinfo,1);
		pCardinfo++;

		// ��վƱ��־     	
		pCardinfo++;

		// �Ƿ���Ҫ����
		applicationData.CardActived = *pCardinfo;
		pCardinfo++;

		// ��Ʊ���
		applicationData.SalePrice = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
		pCardinfo += 4;

		// ״̬��־
		applicationData.TicketStatus = *pCardinfo;             	
		pCardinfo++;

		// ��բ��־
		journeyData.JourneyStatus = *pCardinfo;
		pCardinfo++;

		// Ǯ��
		memcpy(&applicationData.Balance,pCardinfo,4);
		pCardinfo += 4;

		// ���������
		pCardinfo += 2;

		// ���ݱ�־
		pCardinfo++;

		// ��վʱ��
		journeyData.EnterTime.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		journeyData.EnterTime.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.EnterTime.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.EnterTime.biHour = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.EnterTime.biMinute = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.EnterTime.biSecond = BCD2int((char*)pCardinfo,1);
		pCardinfo++;

		// ��վվ��
		journeyData.EnterStation = ComMakeWORD(*(pCardinfo+1),*pCardinfo);
		pCardinfo += 2;

		// ������֤��MAC2
		pCardinfo += 3;
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CRWException(CRWException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ��ECT��ʷ���׼�¼��Ϣ

@param      (i)BYTE* cardInfo ���������ص�Ʊ����Ϣ

@retval     bool true:�ɹ� false:ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CULCard::GetHistoryInfo(int count,BYTE* historyInfo)
{
	bool ret = true;
	try {
		LPBYTE pHistoryinfo = historyInfo;
		STR_CARD_HISTORY_DATA history;
		historyData.clear();
	
		for(int i=0; i<count; i++)
		{
			// ����Ǯ�����ѡ���ֵ�������
			history.TransNumber = MAKEWORD(*pHistoryinfo, *(pHistoryinfo + 1));
			pHistoryinfo += 2;

			// Ԥ��
			pHistoryinfo += 3;

			// ���׽��
			history.TransValue = ComMakeLong(*pHistoryinfo, *(pHistoryinfo + 1),*(pHistoryinfo + 2),*(pHistoryinfo + 3));
			pHistoryinfo += 4;

			// �������ͱ�ʶ
			history.TransType = *pHistoryinfo;
			pHistoryinfo++;

			// �ն˻����
			pHistoryinfo++;
			history.TransStation = MAKEWORD(*(pHistoryinfo + 1),*pHistoryinfo);
			pHistoryinfo += 5;

			// ����������ʱ��
			history.TransTime.wYear = BCD2int((char*)pHistoryinfo,2);	
			pHistoryinfo += 2;
			history.TransTime.biMonth = BCD2int((char*)pHistoryinfo,1);	
			pHistoryinfo++;
			history.TransTime.biDay = BCD2int((char*)pHistoryinfo,1);	
			pHistoryinfo++;
			history.TransTime.biHour = BCD2int((char*)pHistoryinfo,1);	
			pHistoryinfo++;
			history.TransTime.biMinute = BCD2int((char*)pHistoryinfo,1);	
			pHistoryinfo++;
			history.TransTime.biSecond = BCD2int((char*)pHistoryinfo,1);
			pHistoryinfo++;

			historyData.push_back(history);
		}
	}
	
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CRWException(CRWException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return ret;
}