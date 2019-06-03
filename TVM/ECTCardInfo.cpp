#include "StdAfx.h"
#include "ECTCardInfo.h"
#include "CommonDefine.h"

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
CECTCard::CECTCard(void)
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
CECTCard::~CECTCard(void)
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
void CECTCard::Initialize()
{
	//// ��Ƭ������Ϣ�ṹ��
	//memset(cardData.TicketPhyID,0,7);				// ������	    7HEX	��Ʊ������
	//memset(cardData.TicketLogicalID,0,10);			// �߼�����	    10HEX	��Ʊ�߼����
	//cardData.CityCode = 0;							// ���д���		2BCD	
	//cardData.IndustryCode = 0;						// ��ҵ����		2BCD			
	//cardData.TicketFamily = 0;						// ��Ʊ�����	1HEX	
	//cardData.TicketType = 0;						// ��Ӧ������
	//cardData.TicketStatus = 0;						// ���ñ�־
	//cardData.Deposit = 0;							// Ѻ��			2BCD    ��λ:��        

	//// Ǯ����Ϣ�ṹ��
	//purseData.Balance = 0;							// ���/���
	//purseData.TransactionTimes = 0;					// Ǯ���ۼƽ��״���

	//// ��ֵ��Ϣ�ṹ��
	//chargeData.ChargeTotalValue = 0;                // �ۼƳ�ֵ�� ��λ:��
	//chargeData.MaxChargeValue = 0;					// ����������,CPUƱ�����ֶ�Ϊ0		
	//chargeData.ChargeDeviceID = 0;					// ��ֵ�豸���(��·����վ���豸���͡��豸��Ÿ�1�ֽ�) 

	//// �����ó����ݵĽṹ��
	//journeyData.JourneyStatus = 0;					// �ó�״̬		41:��ʾ��վ   81:��ʾ��վ	
	//journeyData.EnterStation = 0;					// ��վվ��				��·��1HEX��+��վ��1HEX��
	//journeyData.EnterDeviceID = 0;					// ��վ�豸             �豸���ͣ�1HEX��+ վ�ڱ�ţ�1HEX��
	//journeyData.EnterTransValue = 0;				// ��վ���׽��	2HEX	��λ����		
	//journeyData.ExitStation = 0;					// ��վվ��				��·��1HEX��+��վ��1HEX��
	//journeyData.ExitDeviceID = 0;					// ��վ�豸				�豸���ͣ�1HEX��+ վ�ڱ�ţ�1HEX��
	//journeyData.ExitTransValue = 0;					// ��վ���׽��	2HEX	��λ����
	//journeyData.BackupFlag = 0;						// ���ݱ�ʶ	1	HEX	55����ʾ�����վ�� 66����ʾ�����վ��	71����ʾ���������£�72����ʾ��ʱ����

	//// �Ż���Ϣ�ṹ��     
	//discountData.IndustryCode = 0;					// ��ҵ��ʶ
	//discountData.TicketType = 0;					// ��Ӧ������

	//// ��ʷ��¼
	//historyData.clear();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ��ECTƱ����Ϣ

@param      (i)BYTE* cardInfo ���������ص�Ʊ����Ϣ

@retval     bool true:�ɹ� false:ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CECTCard::GetCardInfo(BYTE* cardInfo)
{
	bool ret = true;
	try {
	//	LPBYTE pCardinfo = cardInfo;
	//	// ��Ƭ������Ϣ�ṹ��
	//	// ������
	//	memcpy(cardData.TicketPhyID,(char*)pCardinfo,7); 
	//	pCardinfo += 7;

	//	// ������ˮ��
	//	memcpy(cardData.TicketLogicalID,(char*)pCardinfo,10);
	//	pCardinfo += 10;

	//	// ���д���
	//	cardData.CityCode = BCD2int((char*)pCardinfo,2);
	//	pCardinfo += 2;
	//	
	//	// ��ҵ����
	//	cardData.IndustryCode = BCD2int((char*)pCardinfo,2);
	//	pCardinfo += 2;
	//			
	//	// ��Ʊ�����
	//	cardData.TicketFamily = *pCardinfo;  
	//	pCardinfo ++;

	//	// ��Ʊ��Ʒ����
	//	cardData.TicketType = *pCardinfo;
	//	pCardinfo += 1;

	//	// ����״̬
	//	cardData.TicketStatus = *pCardinfo;				
	//	pCardinfo ++;

	//	// Ѻ��
	//	cardData.Deposit = MAKEWORD(*pCardinfo,*(pCardinfo+1));				
	//	pCardinfo += 2;
	//	
	//	// ��������
	//	cardData.InitDate.wYear = BCD2int((char*)pCardinfo,2);	
	//	pCardinfo += 2;
	//	cardData.InitDate.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	cardData.InitDate.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;

	//	// ��Ч����
	//	cardData.ValiditDate.wYear = BCD2int((char*)pCardinfo,2);	
	//	pCardinfo += 2;
	//	cardData.ValiditDate.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	cardData.ValiditDate.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;

	//	// ��������
	//	cardData.StartDate.wYear = BCD2int((char*)pCardinfo,2);	
	//	pCardinfo += 2;
	//	cardData.StartDate.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	cardData.StartDate.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	
	//	// Ԥ��
	//	pCardinfo += 6;

	//	// Ԥ��
	//	pCardinfo += 16;
	//	
	//	// Ǯ����Ϣ
	//	// �������
	//	purseData.Balance = MAKELONG(MAKEWORD(*pCardinfo, *(pCardinfo + 1)),MAKEWORD(*(pCardinfo + 2), *(pCardinfo + 3)));           	
	//	pCardinfo += 4;

	//	// Ǯ���ۼƽ��״���
	//	purseData.TransactionTimes = MAKELONG(MAKEWORD(*pCardinfo, *(pCardinfo + 1)),MAKEWORD(*(pCardinfo + 2), *(pCardinfo + 3)));           	
	//	pCardinfo += 4;
	//	
	//	//��ֵʱ��
	//	purseData.chargeDate.wYear = BCD2int((char*)pCardinfo,2);	
	//	pCardinfo += 2;
	//	purseData.chargeDate.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	purseData.chargeDate.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	//��ֵԭ��
	//	purseData.chargeAmout =MAKELONG(MAKEWORD(*pCardinfo, *(pCardinfo + 1)),MAKEWORD(*(pCardinfo + 2), *(pCardinfo + 3)));           	
	//	pCardinfo += 4;
	//	//���γ�ֵ
	//	purseData.currentChargeAmout = MAKEWORD(*pCardinfo, *(pCardinfo + 1));
	//	pCardinfo+=2;
	//	// Ԥ��
	//	pCardinfo += 2;

	//	// ��ֵ��Ϣ
	//	// �ۼƳ�ֵ�� ��λ:��
	//	chargeData.ChargeTotalValue  =MAKELONG(MAKEWORD(*pCardinfo, *(pCardinfo + 1)),MAKEWORD(*(pCardinfo + 2), *(pCardinfo + 3)));           	            
	//	pCardinfo += 4;

	//	// ����������,CPUƱ�����ֶ�Ϊ0
	//	chargeData.MaxChargeValue  =MAKELONG(MAKEWORD(*pCardinfo, *(pCardinfo + 1)),MAKEWORD(*(pCardinfo + 2), *(pCardinfo + 3)));           	
	//	pCardinfo += 4;

	//	// ��ֵ�豸���(��·����վ���豸���͡��豸��Ÿ�1�ֽ�) 
	//	chargeData.ChargeDeviceID  = BCD2int((char*)pCardinfo,4);	
	//	pCardinfo += 4;

	//	// Ԥ��
	//	pCardinfo += 8;

	//	// �ó�����
	//	// �ó�״̬		41:��ʾ��վ   81:��ʾ��վ
	//	journeyData.JourneyStatus = *pCardinfo;		
	//	pCardinfo ++;

	//	// ��վ����ʱ��	5BCD	YYMMDDHHMM   ��Ϊ0
	//	journeyData.EnterTime.wYear = 2000 +  BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.EnterTime.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.EnterTime.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.EnterTime.biHour = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.EnterTime.biMinute = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.EnterTime.biSecond = 0;
	//	
	//	// ��վվ��				��·��1HEX��+��վ��1HEX��
	//	journeyData.EnterStation = MAKEWORD(*pCardinfo, *(pCardinfo + 1));		
	//	pCardinfo += 2;

	//	// ��վ�豸             �豸���ͣ�1HEX��+ վ�ڱ�ţ�1HEX��
	//	journeyData.EnterDeviceID = MAKEWORD(*pCardinfo, *(pCardinfo + 1));	
	//	pCardinfo += 2;

	//	// ��վ���׽��	2HEX	��λ����
	//	journeyData.EnterTransValue = MAKEWORD(*pCardinfo, *(pCardinfo + 1));		
	//	pCardinfo += 2;

	//	// Ԥ��
	//	pCardinfo ++;

	//	// ��վ����ʱ��	5BCD	YYMMDDHHMM   ��Ϊ0
	//	journeyData.ExitTime.wYear = 2000 +  BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.ExitTime.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.ExitTime.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.ExitTime.biHour = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.ExitTime.biMinute = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	journeyData.ExitTime.biSecond = 0;
	//	
	//	// ��վվ��				��·��1HEX��+��վ��1HEX��
	//	journeyData.ExitStation = MAKEWORD(*pCardinfo, *(pCardinfo + 1));	
	//	pCardinfo += 2;

	//	// ��վ�豸				�豸���ͣ�1HEX��+ վ�ڱ�ţ�1HEX��
	//	journeyData.ExitDeviceID = MAKEWORD(*pCardinfo, *(pCardinfo + 1));		
	//	pCardinfo += 2;

	//	// ��վ���׽��	2HEX	��λ����
	//	journeyData.ExitTransValue = MAKEWORD(*pCardinfo, *(pCardinfo + 1));		
	//	pCardinfo += 2;

	//	// ���ݱ�ʶ	1	HEX	55����ʾ�����վ�� 66����ʾ�����վ��
	//	journeyData.BackupFlag =  *pCardinfo;						
	//	pCardinfo ++;

	//	// Ԥ��
	//	pCardinfo += 8;

	//	// �Ż���Ϣ����
	//	// ��ҵ��ʶ
	//	discountData.IndustryCode = MAKEWORD(*pCardinfo, *(pCardinfo + 1));	
	//	pCardinfo += 2;

	//	// ��Ӧ������
	//	discountData.TicketType = MAKEWORD(*pCardinfo, *(pCardinfo + 1));	
	//	pCardinfo += 2;

	//	// ��������	    4BCD	YYYYMMDD
	//	discountData.StartDate.wYear = BCD2int((char*)pCardinfo,2);	
	//	pCardinfo += 2;
	//	discountData.StartDate.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	discountData.StartDate.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;

	//	// ��������		4BCD	YYYYMMDD
	//	discountData.EndDate.wYear = BCD2int((char*)pCardinfo,2);	
	//	pCardinfo += 2;
	//	discountData.EndDate.biMonth = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;
	//	discountData.EndDate.biDay = BCD2int((char*)pCardinfo,1);	
	//	pCardinfo ++;

	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
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
bool CECTCard::GetHistoryInfo(BYTE TicketPhyType,int count,BYTE* historyInfo)
{
	bool ret = true;
	try {
	//	LPBYTE pCardinfo = historyInfo;
	//	STR_CARD_HISTORY_DATA history;
	//	historyData.clear();
	//	 
	//	//һ��ͨM1��
	//	if (TicketPhyType == MIFARE1K){
	//		for(int i=0; i<count; i++){
	//			// ����ʱ�� YYMMDDHHMM   ��Ϊ0
	//			history.TransTime.wYear = 0;	
	//			pCardinfo ++;
	//			history.TransTime.biMonth = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biDay = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biHour = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biMinute = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biSecond = 0;
	//			pCardinfo += 4;			

	//			// ���׽��?????????????????????????????????????????????????????????????
	//			history.TransValue  = ComMakeLong(0, *(pCardinfo + 2), *(pCardinfo + 1), *pCardinfo);             
	//			pCardinfo += 3;
	//			
	//			// ��������
	//			BYTE type= *pCardinfo;
	//			switch (type){
	//			case LOACLPAK_CONSUMER:
	//				history.TransType = "����";
	//				break;
	//			case UPDATE_TRAC:
	//				history.TransType = "��Ʊ";
	//				break;
	//			case REFOUND_TRAC:
	//				history.TransType = "�˿�";
	//				break;
	//			case LUCK_TRAC:
	//				history.TransType = "����";
	//				break;
	//			case ENTRE_TRAC:
	//				history.TransType = "��վ";
	//				break;
	//			case OUT_TRAC:
	//				history.TransType = "��վ";
	//				break;
	//			case CHARGE_TRAC:
	//				history.TransType = "��ֵ";
	//				break;
	//			default :
	//				history.TransType ="--"; 
	//				break;
	//			}				
	//			pCardinfo ++;
	//			// ����վ��	
	//			history.TransStation = BCD2int((char*)pCardinfo,2);		
	//			pCardinfo += 4;
	//			historyData.push_back(history);
	//		}
	//	} 
	//	//һ��ͨCPU��
	//	else if (TicketPhyType == CPU_TICKET){
	//		for(int i=0; i<count; i++){	
	//			pCardinfo += 5;
	//			// ��ֵ���
	//			history.TransValue  = ComMakeLong(*pCardinfo, *(pCardinfo + 1), *(pCardinfo + 2), *(pCardinfo + 3));             
	//			pCardinfo += 4;
	//			
	//			// ��������
	//			int type= *pCardinfo;
	//			switch (type){
	//			case MONTHPAK_STROE:
	//				history.TransType = "��ƱǮ�� Ȧ��";
	//				break;
	//			case PUBPAK_STROE:
	//				history.TransType = "����Ǯ�� Ȧ��";
	//				break;
	//			case MONTHPAK_CONSUMER:
	//				history.TransType = "��ƱǮ�� ����";
	//				break;
	//			case PUBPAK_CONSUMER:
	//				history.TransType = "����Ǯ�� ����";
	//				break;
	//			case COM_CONSUMER:
	//				history.TransType = "��������";
	//				break;
	//			default :
	//				history.TransType ="--"; 
	//				break;
	//			}
	//			// ����վ�� ��·��1HEX��+��վ��1HEX��
	//			history.TransStation = ComMakeWORD(*pCardinfo, *(pCardinfo + 1));		
	//			pCardinfo += 6;
	//		
	//			// ����ʱ��	  	YYMMDDHHMM   ��Ϊ0
	//			history.TransTime.wYear = BCD2int((char*)pCardinfo,2);	
	//			pCardinfo ++;
	//			history.TransTime.biMonth = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biDay = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biHour = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biMinute = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;
	//			history.TransTime.biSecond = BCD2int((char*)pCardinfo,1);	
	//			pCardinfo ++;

	//			historyData.push_back(history);
	//		}
	//	}
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return ret;
}