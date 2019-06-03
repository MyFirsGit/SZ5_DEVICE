#include "StdAfx.h"
#include "CPUCardInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���캯��
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CCPUCard::CCPUCard(void)
//{
//	Initialize();
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��������
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CCPUCard::~CCPUCard(void)
//{
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ʼ��
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CCPUCard::Initialize()
//{
//	// Ʊ��������Ϣ�ṹ��
//	applicationData.PublisherCode = 0;					// ����������
//	applicationData.CityCode = 0;						// ���д���
//	applicationData.IndustryCode = 0;					// ��ҵ����
//	applicationData.CardPhysicalType = 0;				// Ʊ����������
//	memset(applicationData.CardPhysicalNumber,0,8);		// Ʊ��������
//	applicationData.CardLogicalNumber = 0;				// Ʊ���߼����
//	applicationData.CardAppMainType = 0;				// ��Ӧ��������
//	applicationData.CardAppSubType = 0;					// ��Ӧ��������
//	applicationData.TestCard = 0;						// ���Կ���־
//	applicationData.CardEnabled = 0;					// ���ñ�־
//	applicationData.TicketStatus = 0;
//	applicationData.CardLocked = 0;
//	applicationData.CardActived = 0;
//	memset(applicationData.DeviceID,0,6);				// �����豸��Ϣ
//	applicationData.AppVersion = 0;						// Ӧ�ð汾��
//	applicationData.ExchangeCard = 0;					// ��ͨ����ʶ
//	applicationData.Deposit = 0;						// Ѻ��
//	applicationData.Cost = 0;							// �ɱ�
//	applicationData.Balance = 0;						// Ʊ�����
//
//	// �ֿ�����Ϣ�ṹ��
//	personalData.Sex = 0;								// �ֿ����Ա�
//	personalData.CredentialsType = 0;					// �ֿ���֤������
//
//	// �ó���Ϣ�ṹ��
//	journeyData.JourneyStatus = 0;						// �ó�״̬
//	journeyData.EnterStation = 0;						// ��վվ��
//	memset(journeyData.EnterDeviceID,0,6);				// ��վ�豸 
//	journeyData.ExitStation = 0;						// ��վվ��
//	memset(journeyData.ExitDeviceID,0,6);				// ��վ�豸
//
//	// ������Ϣ�ṹ��
//	saleData.SaleStation = 0;
//	saleData.StarSection = 0;
//	saleData.EndSection = 0;
//	saleData.SalePrice = 0;
//
//	// ��ʷ��¼
//	historyData.clear();
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��ULƱ����Ϣ
//
//@param      (i)BYTE* cardInfo ���������ص�Ʊ����Ϣ
//
//@retval     bool true:�ɹ� false:ʧ��
//
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
//bool CCPUCard::GetCardInfo(WORD len,BYTE* cardInfo)
//{
//	bool ret = true;
//	try {
//		LPBYTE pCardinfo = cardInfo;
//
//		if(len == 360){
//			// ����������
//			applicationData.PublisherCode = BCD2int((char*)pCardinfo,1);			
//			pCardinfo += 2;
//
//			// ���д���
//			applicationData.CityCode = MAKEWORD(*pCardinfo, *(pCardinfo + 1));
//			pCardinfo += 2;
//
//			// ��ҵ����
//			applicationData.IndustryCode = BCD2int((char*)pCardinfo,1);
//			pCardinfo += 2;
//
//			// Ԥ��
//			pCardinfo += 2;
//
//			// Ʊ���߼����
//			pCardinfo += 4;
//			applicationData.CardLogicalNumber = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//			pCardinfo += 4;
//
//			// ��Ӧ��������
//			pCardinfo++;
//			applicationData.CardAppMainType = *pCardinfo;
//			pCardinfo++;
//
//			// ��������
//			applicationData.PublishDate.wYear = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//			applicationData.PublishDate.biMonth = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//			applicationData.PublishDate.biDay = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//
//			// �����豸��Ϣ
//			memcpy(applicationData.DeviceID,(char*)pCardinfo,6);
//			pCardinfo += 6;
//
//			// Ӧ�ð汾��
//			applicationData.AppVersion = MAKEWORD(*pCardinfo, *(pCardinfo + 1));
//			pCardinfo += 2;
//
//			// �����̴���
//			pCardinfo += 2;
//
//			// ���д���
//			pCardinfo += 2;
//
//			// ��ҵ����
//			pCardinfo += 2;
//
//			// Ԥ��
//			pCardinfo += 2;
//
//			// Ӧ�����ͱ�ʶ�����ñ�־��
//			applicationData.CardEnabled = *pCardinfo;
//			pCardinfo++;
//
//			// Ӧ�ð汾
//			pCardinfo++;
//
//			// ��ͨ����ʶ
//			applicationData.ExchangeCard = MAKEWORD(*pCardinfo, *(pCardinfo + 1));
//			pCardinfo += 2;
//
//			// Ӧ�����к�
//			pCardinfo += 8;
//
//			// Ӧ����������
//			applicationData.EnabledDate.wYear = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//			applicationData.EnabledDate.biMonth = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//			applicationData.EnabledDate.biDay = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//
//			// Ӧ����Ч����
//			applicationData.ValidDate.wYear = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//			applicationData.ValidDate.biMonth = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//			applicationData.ValidDate.biDay = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//
//			// Ԥ��
//			pCardinfo += 2;
//
//			// �ֿ������ͱ�ʶ
//			personalData.CardHoldType = *pCardinfo;
//			pCardinfo++;
//
//			// �ֿ���ְ����ʶ
//			personalData.StaffStatus = *pCardinfo;
//			pCardinfo++;
//
//			// �ֿ�������
//			char name[21] = {0};
//			memcpy(name, pCardinfo, 20);
//			personalData.Name = name;
//			pCardinfo += 20;
//
//			// �ֿ���֤������
//			char num[33] = {0};
//			memcpy(num, pCardinfo, 32);
//			personalData.CredentialsNumber = num;
//			pCardinfo += 32;
//
//			// �ֿ���֤������
//			personalData.CredentialsType = *pCardinfo;
//			pCardinfo++;
//
//			// Ѻ����֣�
//			applicationData.Deposit = MAKEWORD(*pCardinfo, *(pCardinfo + 1));
//			pCardinfo += 2;
//
//			// �������ѱ�ʶ
//			pCardinfo++;
//
//			// ��¼����
//			pCardinfo++;
//
//			// ��������������־λ
//			pCardinfo++;
//
//			// ���ؿ�
//			if(applicationData.CityCode == 0x1161 || applicationData.CityCode == 0x1100){
//					pCardinfo += 16;
//			}
//			// ��ؿ�
//			else{
//				// ����վ��־
//				journeyData.JourneyStatus = *pCardinfo;
//				pCardinfo++;
//
//				// �ѽ�վ
//				if(journeyData.JourneyStatus != UNENTER_STATION){
//					// ����վ�ն�
//					pCardinfo += 4;
//
//					// ����վ�ص�
//					BYTE pEnterStation[2] = {0,0};
//					WORD enterStation = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//					int2BCD(enterStation,(char*)pEnterStation,2);
//					journeyData.EnterStation = ComMakeWORD(*pEnterStation,*(pEnterStation + 1));
//					pCardinfo += 4;
//
//					// ����վʱ��
//					journeyData.EnterTime.wYear = BCD2int((char*)pCardinfo,2);
//					pCardinfo += 2;
//					journeyData.EnterTime.biMonth = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.EnterTime.biDay = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.EnterTime.biHour = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.EnterTime.biMinute = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.EnterTime.biSecond = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//				}
//				// �ѳ�վ
//				else {
//					// ����վ�ն�
//					pCardinfo += 4;
//
//					// ����վ�ص�
//					BYTE pExitStation[2] = {0,0};
//					WORD exitStation = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//					int2BCD(exitStation,(char*)pExitStation,2);
//					journeyData.ExitStation = ComMakeWORD(*pExitStation,*(pExitStation + 1));
//					pCardinfo += 4;
//
//					// ����վʱ��
//					journeyData.ExitTime.wYear = BCD2int((char*)pCardinfo,2);
//					pCardinfo += 2;
//					journeyData.ExitTime.biMonth = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.ExitTime.biDay = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.ExitTime.biHour = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.ExitTime.biMinute = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//					journeyData.ExitTime.biSecond = BCD2int((char*)pCardinfo,1);
//					pCardinfo++;
//				}
//				//else{
//				//	pCardinfo += 15;
//				//}
//			}
//
//			// Ԥ��
//			pCardinfo += 29;
//
//			// ��״̬��־
//			applicationData.TicketStatus = *pCardinfo;
//			pCardinfo++;
//
//			// �������ͱ�ʶ
//			pCardinfo++;
//
//			// ���ʹ���ն˵���ҵ���
//			pCardinfo += 2;
//
//			// ���ʹ���ն˵���·���
//			pCardinfo += 2;
//
//			// ������ͨӦ�����ʹ�õص�
//			pCardinfo += 4;
//
//			// ������ͨӦ�����ʹ��ʱ��
//			pCardinfo += 7;
//
//			// ͸֧���
//			applicationData.dwOverdraftValue = ComMakeLong(0,*(pCardinfo + 2), *(pCardinfo + 1),*pCardinfo);
//			pCardinfo += 3;
//
//			// ����
//			pCardinfo += 4;
//
//			// Ԥ��
//			pCardinfo += 42;
//
//			// ���ؿ�
//			if((applicationData.CityCode == 0x1161) 
//				|| (applicationData.CityCode == 0x1100)){
//					// ����վ��־
//					journeyData.JourneyStatus = *pCardinfo;
//					pCardinfo++;
//					// �ѽ�վ
//					if(journeyData.JourneyStatus != UNENTER_STATION){
//						// ����վ�ն�
//						pCardinfo += 4;
//						
//						BYTE emptyEnterInfo[8] = {0};
//						if(memcmp(emptyEnterInfo,pCardinfo,sizeof(emptyEnterInfo)) == 0)
//						{
//							journeyData.EnterStation = 0;
//							journeyData.EnterTime = _DATE_TIME();
//							pCardinfo+=8;
//						}
//						else
//						{
//							// ����վ�ص�
//							BYTE pEnterStation[2] = {0,0};
//							WORD enterStation = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//							int2BCD(enterStation,(char*)pEnterStation,2);
//							journeyData.EnterStation = ComMakeWORD(*pEnterStation,*(pEnterStation + 1));
//							pCardinfo += 4;
//
//							// ��ʼʱ��(6λ�ꡢ4λ�¡�5λ�ա�5λʱ��6λ�֡�6λ��)
//							DWORD tmpTime = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//							BYTE year = (BYTE)((tmpTime >> 26) & 0x3f);
//							BYTE month = (BYTE)((tmpTime >> 22) & 0x0f);
//							BYTE day = (BYTE)((tmpTime >> 17) & 0x1f);
//							BYTE hour = (BYTE)((tmpTime >> 12) & 0x1f);
//							BYTE minute = (BYTE)((tmpTime >> 6) & 0x3f);
//							BYTE second = (BYTE)(tmpTime & 0x3f);
//							journeyData.EnterTime.wYear = 2000 + year;
//							journeyData.EnterTime.biMonth = month;
//							journeyData.EnterTime.biDay = day;
//							journeyData.EnterTime.biHour = hour;
//							journeyData.EnterTime.biMinute = minute;
//							journeyData.EnterTime.biSecond = second;
//							pCardinfo += 4;
//						}
//						
//
//					}
//					// �ѳ�վ
//					else{
//						// ����վ�ն�
//						pCardinfo += 4;
//						BYTE emptyEnterInfo[8] = {0};
//						if(memcmp(emptyEnterInfo,pCardinfo,sizeof(emptyEnterInfo)) == 0)
//						{
//							journeyData.ExitStation = 0;
//							journeyData.ExitTime = _DATE_TIME();
//							pCardinfo+=8;
//						}
//						else
//						{
//							// ����վ�ص�
//							BYTE pExitStation[2] = {0,0};
//							WORD exitStation = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//							int2BCD(exitStation,(char*)pExitStation,2);
//							journeyData.ExitStation = ComMakeWORD(*pExitStation,*(pExitStation + 1));
//							pCardinfo += 4;
//
//							// ��ʼʱ��(6λ�ꡢ4λ�¡�5λ�ա�5λʱ��6λ�֡�6λ��)
//							DWORD tmpTime = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//							BYTE year = (BYTE)((tmpTime >> 26) & 0x3f);
//							BYTE month = (BYTE)((tmpTime >> 22) & 0x0f);
//							BYTE day = (BYTE)((tmpTime >> 17) & 0x1f);
//							BYTE hour = (BYTE)((tmpTime >> 12) & 0x1f);
//							BYTE minute = (BYTE)((tmpTime >> 6) & 0x3f);
//							BYTE second = (BYTE)(tmpTime & 0x3f);
//							journeyData.ExitTime.wYear = 2000 + year;
//							journeyData.ExitTime.biMonth = month;
//							journeyData.ExitTime.biDay = day;
//							journeyData.ExitTime.biHour = hour;
//							journeyData.ExitTime.biMinute = minute;
//							journeyData.ExitTime.biSecond = second;
//							pCardinfo += 4;
//						}
//					}
//					//else{
//					//	pCardinfo += 12;
//					//}
//			}
//			// ��ؿ�
//			else{
//				pCardinfo += 13;
//			}
//
//			// �ճ�������
//			pCardinfo += 2;
//
//			// �ճ�����������
//			pCardinfo += 4;
//
//			// ���������ۼ�
//			pCardinfo += 2;
//
//			// ���ظ��ϼ�¼MAC
//			pCardinfo += 4;
//
//			// Ԥ��
//			pCardinfo += 36;
//
//			// �������ѱ�ʶ
//			pCardinfo++;
//
//			// ��¼����
//			pCardinfo++;
//
//			// Ӧ��������־λ
//			pCardinfo++;
//
//			// ���³���ʶ
//			pCardinfo++;
//
//			// �����˴�
//			pCardinfo++;
//
//			// �ϳ�վ���
//			pCardinfo++;
//
//			// �ϳ�ʱ��
//			pCardinfo += 4;
//
//			// ��˾��
//			pCardinfo++;
//
//			// ·����·��
//			pCardinfo += 4;
//
//			// ������
//			pCardinfo += 3;
//
//			// ˾������
//			pCardinfo += 4;
//
//			// ȫ��Ʊ��
//			pCardinfo += 4;
//
//			// Ԥ��
//			pCardinfo += 38;
//
//			// Ǯ�����
//			if(applicationData.dwOverdraftValue > 0){		// ��͸֧
//				applicationData.Balance = 0 - applicationData.dwOverdraftValue;
//			}
//			else{
//				applicationData.Balance = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//			}		
//			pCardinfo += 4;
//		}
//		//ADF2��
//		else if(len == 304){
//			// ����������
//			applicationData.PublisherCode = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//
//			// ���д���
//			applicationData.CityCode = MAKEWORD(*pCardinfo, *(pCardinfo + 1));
//			pCardinfo += 2;
//
//			// ��ҵ����
//			applicationData.IndustryCode = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//
//			// Ԥ��
//			pCardinfo += 2;
//
//			// Ʊ���߼����
//			pCardinfo += 4;
//			applicationData.CardLogicalNumber = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//			pCardinfo += 4;
//
//			// �����ͣ���ʹ�ã�
//			pCardinfo += 2;
//
//			// ��������
//			applicationData.PublishDate.wYear = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//			applicationData.PublishDate.biMonth = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//			applicationData.PublishDate.biDay = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//
//			// �����豸��Ϣ
//			memcpy(applicationData.DeviceID,(char*)pCardinfo,6);
//			pCardinfo += 6;
//
//			// Ӧ�ð汾��
//			applicationData.AppVersion = MAKEWORD(*pCardinfo, *(pCardinfo + 1));
//			pCardinfo += 2;
//
//			// �����̴���
//			pCardinfo += 8;
//
//			// Ӧ�����ͱ�ʶ
//			applicationData.CardEnabled = *pCardinfo;
//			pCardinfo++;
//
//			// �����ͣ�ʹ�ã�
//			applicationData.CardAppMainType = *pCardinfo;
//			pCardinfo++;
//
//			// ���ұ�ʶ
//			pCardinfo++;
//
//			// ���貿��ʶ
//			pCardinfo++;
//
//			// ���д���
//			pCardinfo += 2;
//
//			// ��ҵ����
//			pCardinfo += 2;
//
//			// ��Ӧ�ú�
//			pCardinfo += 4;
//
//			// �������� 
//			pCardinfo += 4;
//
//			// ��ֹ����
//			pCardinfo += 4;
//
//			// ���汾��
//			pCardinfo++;
//
//			// Ӧ�ð汾��
//			pCardinfo++;
//
//			// Ʊ��������
//			pCardinfo++;
//
//			// Ԥ��
//			pCardinfo++;
//
//			// ��Ч��ʼ����
//			applicationData.EnabledDate.wYear = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//			applicationData.EnabledDate.biMonth = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//			applicationData.EnabledDate.biDay = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//
//			// ��Ч��������
//			applicationData.ValidDate.wYear = BCD2int((char*)pCardinfo,2);
//			pCardinfo += 2;
//			applicationData.ValidDate.biMonth = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//			applicationData.ValidDate.biDay = BCD2int((char*)pCardinfo,1);
//			pCardinfo++;
//
//			// Ѻ����֣�
//			applicationData.Deposit = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//			pCardinfo += 4;
//
//			// ��������
//			pCardinfo++;
//
//			// ���ʽ
//			pCardinfo++;
//
//			// ����״̬
//			pCardinfo++;
//
//			// ��ʹ��վ��1
//			saleData.SaleStation = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//			pCardinfo += 4;
//
//			// ��ʹ��վ��2
//			pCardinfo += 4;
//
//			// ��ʹ������
//			pCardinfo += 4;
//
//			// ��Ʊ�ն˻����
//			pCardinfo += 4;
//
//			// ��Ʊ����Ա����
//			pCardinfo += 4;
//
//			// ��Ʊʱ��
//			pCardinfo += 4;
//
//			// ��Ʊ���
//			pCardinfo += 4;
//
//			// ���ý��ױ�ʶ
//			pCardinfo++;
//
//			// ���ó�ֵ����
//			pCardinfo++;
//
//			// Ԥ��
//			pCardinfo += 3;
//
//			// Ӧ�ð汾
//			pCardinfo++;
//
//			// ����λְ����ʶ
//			personalData.StaffStatus = *pCardinfo;
//			pCardinfo++;
//
//			// �ֿ�������
//			char name[21] = {0};
//			memcpy(name, pCardinfo, 20);
//			personalData.Name = name;
//			pCardinfo += 20;
//
//			// �ֿ���֤������
//			char num[33] = {0};
//			memcpy(num, pCardinfo, 32);
//			personalData.CredentialsNumber = num;
//			pCardinfo += 32;
//
//			// �ֿ���֤������
//			personalData.CredentialsType = *pCardinfo;
//			pCardinfo++;
//
//			// �ֿ����Ա�
//			personalData.Sex = *pCardinfo;
//			pCardinfo++;
//
//			// ����
//			pCardinfo += 24;
//
//			// ?
//			pCardinfo++;
//
//			// ��״̬��־
//			applicationData.TicketStatus = *pCardinfo;
//			pCardinfo++;
//
//			// �������ͱ�ʶ
//			pCardinfo++;
//
//			// ���ʹ��ʱ��
//			pCardinfo += 7;
//
//			// ���ʹ���ն˵���ҵ���
//			pCardinfo += 2;
//
//			// ���ʹ���ն˵���·���
//			pCardinfo += 2;
//
//			// ������ͨӦ�����ʹ�õص�
//			pCardinfo += 4;
//
//			// ������ͨӦ�����ʹ��ʱ��
//			pCardinfo += 7;
//
//			// ����
//			pCardinfo += 4;
//
//			// ����
//			pCardinfo++;
//
//			// ��ֵ�������
//			pCardinfo += 2;
//
//			// ��ֵ�����
//			pCardinfo += 4;
//
//			// ��ֵ���׽��
//			pCardinfo += 4;
//
//			// �������ͱ�ʶ
//			pCardinfo++;
//
//			// �ն˻����
//			pCardinfo += 6;
//
//			// ��������ʱ��
//			pCardinfo += 7;
//
//			// �ۼƳ�ֵ��
//			pCardinfo += 4;
//
//			// ��ֵ����Ա����
//			pCardinfo += 4;
//
//			// �ϴγ�ֵ����
//			pCardinfo += 2;
//
//			// ���γ�ֵ����
//			pCardinfo += 2;
//
//			// ��ε�������
//			pCardinfo += 4;
//
//			// Ԥ��
//			pCardinfo += 8;
//
//			// ?
//			pCardinfo++;
//
//			// ����վ��־
//			journeyData.JourneyStatus = *pCardinfo;
//			pCardinfo++;
//			
//			// �ѽ�վ
//			if(journeyData.JourneyStatus != UNENTER_STATION){
//				// ����վ�ն�
//				pCardinfo += 4;
//				BYTE emptyEnterInfo[8] = {0};
//				if(memcmp(emptyEnterInfo,pCardinfo,sizeof(emptyEnterInfo)) == 0)
//				{
//					journeyData.EnterStation = 0;
//					journeyData.EnterTime = _DATE_TIME();
//					pCardinfo+=8;
//				}
//				else
//				{
//					// ����վ�ص�
//					BYTE pEnterStation[2] = {0,0};
//					WORD enterStation = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//					int2BCD(enterStation,(char*)pEnterStation,2);
//					journeyData.EnterStation = ComMakeWORD(*pEnterStation,*(pEnterStation + 1));
//					pCardinfo += 4;
//
//					// ��ʼʱ��(6λ�ꡢ4λ�¡�5λ�ա�5λʱ��6λ�֡�6λ��)
//					DWORD tmpTime = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//					BYTE year = (BYTE)((tmpTime >> 26) & 0x3f);
//					BYTE month = (BYTE)((tmpTime >> 22) & 0x0f);
//					BYTE day = (BYTE)((tmpTime >> 17) & 0x1f);
//					BYTE hour = (BYTE)((tmpTime >> 12) & 0x1f);
//					BYTE minute = (BYTE)((tmpTime >> 6) & 0x3f);
//					BYTE second = (BYTE)(tmpTime & 0x3f);
//					journeyData.EnterTime.wYear = 2000 + year;
//					journeyData.EnterTime.biMonth = month;
//					journeyData.EnterTime.biDay = day;
//					journeyData.EnterTime.biHour = hour;
//					journeyData.EnterTime.biMinute = minute;
//					journeyData.EnterTime.biSecond = second;
//					pCardinfo += 4;
//				}
//			}
//			// �ѳ�վ
//			else{
//				// ����վ�ն�
//				pCardinfo += 4;
//				BYTE emptyEnterInfo[8] = {0};
//				if(memcmp(emptyEnterInfo,pCardinfo,sizeof(emptyEnterInfo)) == 0)
//				{
//					journeyData.ExitStation = 0;
//					journeyData.ExitTime = _DATE_TIME();
//					pCardinfo+=8;
//				}
//				else
//				{
//					// ����վ�ص�
//					BYTE pExitStation[2] = {0,0};
//					WORD exitStation = ComMakeLong(*pCardinfo,*(pCardinfo + 1),*(pCardinfo + 2),*(pCardinfo+3));
//					int2BCD(exitStation,(char*)pExitStation,2);
//					journeyData.ExitStation = ComMakeWORD(*pExitStation,*(pExitStation + 1));
//					pCardinfo += 4;
//
//					// ��ʼʱ��(6λ�ꡢ4λ�¡�5λ�ա�5λʱ��6λ�֡�6λ��)
//					DWORD tmpTime = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//					BYTE year = (BYTE)((tmpTime >> 26) & 0x3f);
//					BYTE month = (BYTE)((tmpTime >> 22) & 0x0f);
//					BYTE day = (BYTE)((tmpTime >> 17) & 0x1f);
//					BYTE hour = (BYTE)((tmpTime >> 12) & 0x1f);
//					BYTE minute = (BYTE)((tmpTime >> 6) & 0x3f);
//					BYTE second = (BYTE)(tmpTime & 0x3f);
//					journeyData.ExitTime.wYear = 2000 + year;
//					journeyData.ExitTime.biMonth = month;
//					journeyData.ExitTime.biDay = day;
//					journeyData.ExitTime.biHour = hour;
//					journeyData.ExitTime.biMinute = minute;
//					journeyData.ExitTime.biSecond = second;
//					pCardinfo += 4;
//				}
//			}
//			//else{
//			//	pCardinfo += 12;
//			//}
//
//			// �ճ�������
//			pCardinfo += 2;
//
//			// �ճ�����������
//			pCardinfo += 4;
//
//			// ���������ۼ�
//			pCardinfo += 2;
//
//			// �³����ۼƴ���
//			pCardinfo += 2;
//
//			// Ԥ��
//			pCardinfo += 8;
//
//			// Ǯ�����
//			applicationData.Balance = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
//			pCardinfo += 4;
//		}
//	}
//	// �쳣����
//	catch(CSysException&) {
//		throw;
//	}
//	catch (...) {
//		throw CRWException(CRWException::OTHER_ERR, _T(__FILE__), __LINE__);
//	}
//	return ret;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��ECT��ʷ���׼�¼��Ϣ
//
//@param      (i)BYTE* cardInfo ���������ص�Ʊ����Ϣ
//
//@retval     bool true:�ɹ� false:ʧ��
//
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
//bool CCPUCard::GetHistoryInfo(int count,BYTE* historyInfo)
//{
//	bool ret = true;
//	try {
//		LPBYTE pHistoryinfo = historyInfo;
//		STR_CARD_HISTORY_DATA history;
//		historyData.clear();
//
//		for(int i=0; i<count; i++)
//		{
//			// ����Ǯ�����ѡ���ֵ�������
//			history.TransNumber = MAKEWORD(*pHistoryinfo, *(pHistoryinfo + 1));
//			pHistoryinfo += 2;
//
//			// Ԥ��
//			pHistoryinfo += 3;
//
//			// ���׽��
//			history.TransValue = ComMakeLong(*pHistoryinfo, *(pHistoryinfo + 1),*(pHistoryinfo + 2),*(pHistoryinfo + 3));
//			pHistoryinfo += 4;
//
//			// �������ͱ�ʶ
//			history.TransType = *pHistoryinfo;
//			pHistoryinfo++;
//
//			// �ն˻����
//			pHistoryinfo++;
//			history.TransStation = MAKEWORD(*(pHistoryinfo + 1),*pHistoryinfo);
//			pHistoryinfo += 5;
//
//			// ����������ʱ��
//			history.TransTime.wYear = BCD2int((char*)pHistoryinfo,2);	
//			pHistoryinfo += 2;
//			history.TransTime.biMonth = BCD2int((char*)pHistoryinfo,1);	
//			pHistoryinfo++;
//			history.TransTime.biDay = BCD2int((char*)pHistoryinfo,1);	
//			pHistoryinfo++;
//			history.TransTime.biHour = BCD2int((char*)pHistoryinfo,1);	
//			pHistoryinfo++;
//			history.TransTime.biMinute = BCD2int((char*)pHistoryinfo,1);	
//			pHistoryinfo++;
//			history.TransTime.biSecond = BCD2int((char*)pHistoryinfo,1);
//			pHistoryinfo++;
//
//			if(history.TransType != 0){
//				historyData.push_back(history);
//			}			
//		}
//	}
//	// �쳣����
//	catch(CSysException&) {
//		throw;
//	}
//	catch (...) {
//		throw CRWException(CRWException::OTHER_ERR, _T(__FILE__), __LINE__);
//	}
//	return ret;
//}