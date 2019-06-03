#include "StdAfx.h"
#include "ACCCardInfo.h"

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
CACCCard::CACCCard(void)
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
CACCCard::~CACCCard(void)
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
void CACCCard::Initialize()
{
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
bool CACCCard::GetCardInfo(BYTE* cardInfo)
{
	bool ret = true;
	try {
		ACCCARD_PRODUCT_INFO productInfo; 
		LPBYTE pCardinfo = cardInfo;

		// 1 ���д���				2	BCD	����CJ/T166����صĹ涨Ҫ�󣬳������������ǰ4λȷ��
		applicationData.sCityCode = ComMakeWORD(*pCardinfo,*(pCardinfo + 1)); 
		pCardinfo += 2;
		// 2 ��ҵ����				2	BCD	���貿��׼
		applicationData.sTradeCode = ComMakeWORD(*pCardinfo,*(pCardinfo + 1));
		pCardinfo += 2;
		// 3 �����̴���			2	BCD	ACC����Ʊ��ʱд��0002
		applicationData.CardIssuerCode = ComMakeWORD(*pCardinfo,*(pCardinfo + 1));
		pCardinfo += 2;
		// 8 ������ˮ��			8	BIN	��Ƭ����ʱд������к�
		memcpy(applicationData.CardLogicalNumber, pCardinfo, 8);
		pCardinfo += 8;
		// 5 ��Ӧ�ñ�ʶ			1	BIN	��ʼ��ʱд�� 0 �� ��ʽ����1 �� ���Կ�
		applicationData.testFlag = (ACC_CARD_TEST_STATUS)*pCardinfo;
		pCardinfo += 1;
		// 6 ���汾				1	BIN
		pCardinfo += 1;
		// 7 ������				1	BIN	��ʶ��Ʊ����������, ��Χ��00-99
		productInfo.productType = *pCardinfo;  
		pCardinfo += 1;
		// 8 ��Կ�汾				1	BIN
		pCardinfo += 1;
		// 9 ��������				4	BCD	YYYYMMDD
		applicationData.initialDate.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		applicationData.initialDate.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		applicationData.initialDate.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++; 
		// 10 ��������			2	BIN	3λ��������, ��Χ��000��999
		applicationData.sInitialNum = ComMakeWORD(*pCardinfo,*(pCardinfo + 1));  
		pCardinfo += 2;     
		// 12 ��Ч��ʼʱ��		7	BCD	YYYYMMDDHHMISS
		productInfo.ValidStartTime.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		productInfo.ValidStartTime.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidStartTime.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidStartTime.biHour = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidStartTime.biMinute = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidStartTime.biSecond = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		// 13 ��Ч����ʱ��		7	BCD	YYYYMMDDHHMISS
		productInfo.ValidEndTime.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		productInfo.ValidEndTime.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidEndTime.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidEndTime.biHour = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidEndTime.biMinute = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		productInfo.ValidEndTime.biSecond = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		// 14 ��������			1	BIN	����08֣��ACC ������Ӫ�����׼-08ϵͳ�������.doc��
		productInfo.productSubType = *pCardinfo; 
		pCardinfo += 1;
		// 15 Ѻ��				4	BIN	�ۿ�ʱ��ȡ��Ѻ�𣬵�λ����                      
		applicationData.Deposit = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo);
		pCardinfo += 4;   
		// 16 ��Ʊ�ն�			4	BIN	��ʶ����Ʊ�����豸���
		pCardinfo += 4;
		// 17 ��Ʊ���			4	BIN	��Ѻ���⣬��Ʊʵ�ʷ�������λ����
		pCardinfo += 4;
		// 18 ��ʹ��վ��1			2	BIN	��ʾ����ʹ�õ�վ��
		productInfo.dwStartStation = ComMakeWORD(*(pCardinfo+1),*(pCardinfo)); 
		pCardinfo += 2;	
		// 19 ��ʹ��վ��2			2	BIN	��ʾ����ʹ�õ�վ��
		productInfo.dwEndStation = ComMakeWORD(*(pCardinfo+1),*(pCardinfo)); 
		pCardinfo += 2;
		// 20 ��ʹ�����α��		2	BIN	��ʹ��վ�㼯�ϵı�ţ���Ҫ��������Ʊ
		productInfo.wArea = ComMakeWORD(*(pCardinfo+1),*(pCardinfo)); 
		pCardinfo += 2;
		// 22 Ǯ�����			4	BIN              
		productInfo.Balance = ComMakeLong(*(pCardinfo+3),*(pCardinfo + 2),*(pCardinfo + 1),*pCardinfo); 
		pCardinfo += 4;   
		// 23 ��״̬				1	BIN	0�C δ�ۣ������ۿ��� 3 �C ���ۣ�����ʹ���еĿ���7 �C ���գ��ѻ��տ���9 �C ����������             
		applicationData.TicketStatus = *pCardinfo;
		pCardinfo += 1;
		// 24 ���������			2	BIN	���ۼƽ��״�����ÿ��д����1����ʶ�����׵�������
		pCardinfo += 2;
		// 25 ����״̬			1	BIN	 0��δ���1���Ѽ���       
		productInfo.CardActived = (ACC_CARD_ACTIVE_STATUS)*pCardinfo;   
		pCardinfo += 1;  
		// 26 �������Ч��ʼʱ��	6	BCD	��Ҫ�����Ʊ������������Ч��ʼ����, YYYYMMDDHHMI
		//productInfo.ValidStartTime.wYear = BCD2int((char*)pCardinfo,2);
		//pCardinfo += 2;
		//productInfo.ValidStartTime.biMonth = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		//productInfo.ValidStartTime.biDay = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		//productInfo.ValidStartTime.biHour = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		//productInfo.ValidStartTime.biMinute = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		pCardinfo+=6;
		// 27 �������Ч����ʱ��	6	BCD	YYYYMMDDHHMI
		//productInfo.ValidEndTime.wYear = BCD2int((char*)pCardinfo,2);
		//pCardinfo += 2;
		//productInfo.ValidEndTime.biMonth = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		//productInfo.ValidEndTime.biDay = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		//productInfo.ValidEndTime.biHour = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		//productInfo.ValidEndTime.biMinute = BCD2int((char*)pCardinfo,1);
		//pCardinfo++;
		pCardinfo+=6;
		// 28 �ֿ�������			20	ASC	�ֿ�����������GB2312-80���ֱ��뷽ʽ�����ֽ�Ϊһ�����֣��8������
		memcpy(userInfo.name,pCardinfo,20);
		pCardinfo += 20;
		// 29 �ֿ���֤������		32	ASC	�����֤��ѧ��֤��֤������
		memcpy(userInfo.certificateCardNumber,pCardinfo,32);
		pCardinfo += 32;
		// 30 �ֿ���֤������		1	BCD	0�����֤ 1������֤ 2������ 3��ѧ��֤
		userInfo.certificateCardType = *pCardinfo;
		pCardinfo += 1;
		// 31 �ֿ����Ա�			1	BIN	0����     1��Ů
		pCardinfo += 1;
		// 32 ������λ����		2	BIN	��ʶ�ֿ������ڵ�λ�ı���
		userInfo.companyID = ComMakeWORD(*pCardinfo,*(pCardinfo + 1));
		pCardinfo += 2;
		// 33 �ֿ���ְ����ʶ		1	BCD	�Ƿ�Ϊ����λְ��
		pCardinfo += 1;
		// 34 ����վ��־			1	BIN	0��δ��վ 1���ѽ�վ
		journeyData.byTravelStatus = (ACC_CARD_JOURNEY_STATUS)*pCardinfo;
		pCardinfo += 1;
		// 35 ����վ�ն�			4	BIN	Ʊ������վ���豸���
		pCardinfo += 4;
		//    ����վվ��			2	BIN	Ʊ������վ��վ��
		journeyData.dwLastTransStation = ComMakeWORD(*(pCardinfo+1),*(pCardinfo)); 
		pCardinfo += 2;
		// 36 ����վʱ��			7	BCD	Ʊ������վ�Ľ���ʱ��, YYYYMMDDHHMISS               
		journeyData.lastTransTime.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		journeyData.lastTransTime.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.lastTransTime.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.lastTransTime.biHour = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.lastTransTime.biMinute = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		journeyData.lastTransTime.biSecond = BCD2int((char*)pCardinfo,1);
		pCardinfo++; 
		// 37 �ճ�������			2	BIN	ÿ������Ĵ���
		pCardinfo += 2;
		// 38 ���������ۼ�		4	BIN	���������ۼƴ���
		pCardinfo += 4;
		// 39 �³����ۼƴ���		2	BIN	ÿ�³��������ۼ�
		pCardinfo += 2;
		// 40 ��ֵ�ն˺�			4	BIN	���һ�γ�ֵ���豸���
		//	rechargeData.transStation = ComMakeLong(*pCardinfo,*(pCardinfo+1),*(pCardinfo+2),*(pCardinfo+3));
		pCardinfo += 4;
		// 41 ��ֵ��������		1	BIN	���һ�γ�ֵ��������
		rechargeData.TransType = *pCardinfo;
		pCardinfo += 1;
		// 42 ��ֵ����ʱ��		7	BCD	���һ�γ�ֵ���׵�����ʱ��
		rechargeData.TransTime.wYear = BCD2int((char*)pCardinfo,2);
		pCardinfo += 2;
		rechargeData.TransTime.biMonth = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		rechargeData.TransTime.biDay = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		rechargeData.TransTime.biHour = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		rechargeData.TransTime.biMinute = BCD2int((char*)pCardinfo,1);
		pCardinfo++;
		rechargeData.TransTime.biSecond = BCD2int((char*)pCardinfo,1);
		pCardinfo++; 
		// 43 ��ֵ���׽��		4	BIN	���һ�γ�ֵ����λ����
		rechargeData.TransValue = ComMakeLong(*(pCardinfo+3),*(pCardinfo+2),*(pCardinfo+1),*pCardinfo);
		pCardinfo += 4;
		// 44 ��ֵ�������		2	BIN	���һ�γ�ֵ�Ľ������
		pCardinfo += 2;
		// 45 ��ֵ�����			4	BIN	���һ�γ�ֵ�������λ����
		rechargeData.remaingValue = ComMakeLong(*(pCardinfo+3),*(pCardinfo+2),*(pCardinfo+1),*pCardinfo);
		pCardinfo += 4;
		// 46 �ۼƳ�ֵ���		4	BIN	�ۻ���ֵ�Ľ���λ����
		pCardinfo += 4;
		// 47 �ϴγ�ֵ����		2	BIN	��Դ�Ʊ�ϴγ�εĵ��ۣ���λ����
		pCardinfo += 2;
		// 48 ���γ�ֵ����		2	BIN	��Դ�Ʊ���γ�εĵ��ۣ���λ����
		pCardinfo += 2;
		// 49 ��ε�������		7	BCD	��Դ�Ʊ�ϴγ�εĵ�������
		pCardinfo += 7;
		// 50 ��ֵ����Ա		3	BCD	���һ�γ�ֵ�Ĳ���Ա����
		rechargeData.TransOperator = BCD2int((char*)pCardinfo, 3);
		pCardinfo += 3;
		// 51 ���˿�Ǯ�����	4	BIN	���ֶν������˿�Ǯ�������Ч��������0
		productInfo.nRemainRides = ComMakeLong(*(pCardinfo+3),*(pCardinfo+2),*(pCardinfo+1),*pCardinfo);
		pCardinfo += 4;

		// ������ֵ�ֶ�
		applicationData.byProductsNum = 1;	//��Ʒ����
		productInfo.productCategory = (ProductCategory_t)theACC_PRODUCT.GetTicketCategry(ComMakeWORD(productInfo.productType,productInfo.productSubType));	// ��Ʒ����
		applicationData.byLastUsedProductCategory = productInfo.productCategory;	// ���ʹ�õĲ�Ʒ����
		applicationData.byLastUsedProductSubType = productInfo.productSubType;		// ���ʹ�õĲ�Ʒ����
		applicationData.byLastUsedProductType = productInfo.productType;			// ���ʹ�õĲ�Ʒ������
		v_ProductInfo.push_back(productInfo);

		// δʹ���ֶ�
		//productInfo.productIssuerID = 1;				// ��Ʒ������ID
		//productInfo.sSamID = 1;							// �ϴζԲ�Ʒ����/��ֵ��SAM ID
		//applicationData.sInitialNum = 1;				// ���ط��м���
		//applicationData.byPassengerType = 1;			// �˿�����
		//applicationData.byTransNum= 1;					// �ѻ��˴���
		//// �����ó̿�ʼʱ��                 
		//journeyData.travelStartTime.wYear = 2013;
		//journeyData.travelStartTime.biMonth = 1;
		//journeyData.travelStartTime.biDay = 1;
		//journeyData.travelStartTime.biHour = 0;
		//journeyData.travelStartTime.biMinute = 0;
		//journeyData.travelStartTime.biSecond = 0;
		//// �����ó̵���ʼվ��  
		//journeyData.dwTravelStartStation = 1;
		//// �Ѹ�����
		//journeyData.nPaid = 1;
		//// ���Ի���ʾ
		//applicationData.byPersonalizeStatus = 0;
		//if (applicationData.byPersonalizeStatus){
		//	// ����
		//	userInfo.birthday.wYear = 2000;
		//	userInfo.birthday.biMonth = 1;
		//	userInfo.birthday.biDay = 1;
		//	// �˿ͼ���
		//	userInfo.certificateCardType = CLASSIFICATION_LEVEL_NORMOL;
		//}
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
@brief      ȡ��ACC��ʷ���׼�¼��Ϣ

@param      (i)BYTE* cardInfo ���������ص�Ʊ����Ϣ

@retval     bool true:�ɹ� false:ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CACCCard::GetHistoryInfo(int count,BYTE* historyInfo)
{
	bool ret = true;
	try {
		LPBYTE pHistoryinfo = historyInfo;
		CARD_HISTORY_DATA history;
		historyData.clear();

		for(int i=0; i<count; i++)
		{
			CARD_HISTORY_DATA history;
			// 1 �������						2	BIN
			pHistoryinfo += 2;
			// 2 ���׽��						4	BIN	��λ����
			history.TransValue = ComMakeLong(*(pHistoryinfo+3),*(pHistoryinfo+2),*(pHistoryinfo+1),*pHistoryinfo);
			pHistoryinfo += 4;
			// 3 ��������						1	BIN
			history.TransType = *pHistoryinfo;
			pHistoryinfo += 1;
			// 4 �ն˻����					6	BIN
			//WORD high_long = ComMakeWORD(*pHistoryinfo,*(pHistoryinfo+1));
			//DWORD low_long = ComMakeLong(*(pHistoryinfo+2),*(pHistoryinfo+3),*(pHistoryinfo+4),*(pHistoryinfo+5));
			//WORD high_long = ComMakeWORD(*pHistoryinfo,*(pHistoryinfo+1),*(pHistoryinfo+2),*(pHistoryinfo+3));
			//DWORD low_long = ComMakeLong(*(pHistoryinfo+4),*(pHistoryinfo+5));
			memcpy(history.transStation,pHistoryinfo,6);
			pHistoryinfo += 6;
			// 5 ����ʱ��						7	BCD	
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

			// δʹ���ֶ�
			//// �����ṩ��  
			//history.serviceProviderId = *pHistoryinfo;
			//pHistoryinfo++;
			//// ��Ʒ����  0:�� 1:Ǯ�� 2:�������� 3:�ƴ����� 4-7:Ԥ��  
			//history.productType = (*pHistoryinfo)>>5;
			//// ������ 
			//BYTE productIssuerId = *pHistoryinfo;
			//history.productIssuerId = productIssuerId%32;
			//pHistoryinfo ++;                                     
			//// ���׷�ʽ  
			//WORD paymentMethod= MAKEWORD(*pHistoryinfo, *(pHistoryinfo + 1));
			//paymentMethod = paymentMethod%61440;
			//paymentMethod = paymentMethod>>7;
			//history.paymentMethod = paymentMethod;
			//pHistoryinfo++;
			//// ��Ʒ����
			//BYTE productType = *pHistoryinfo;
			//productType = productType%128;
			//productType = productType>>1;
			//history.productType = productType;                                
			//// ���׺���                                            
			//DWORD remaingValue = ComMakeLong(*pHistoryinfo, *(pHistoryinfo + 1),*(pHistoryinfo + 2),0);
			//remaingValue = remaingValue&0x3FFFFFF;
			//remaingValue = remaingValue>>3;
			//history.remaingValue = remaingValue;
			//pHistoryinfo += 3;
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

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡָ���Ŀ��ڲ�Ʒ��Ϣ

@param      (i)WORD productType ��Ʒ����
@param      (i)WORD productSubType ��Ʒ������
@param      (o)ACCCARD_PRODUCT_INFO* productInfo ��Ʒ��Ϣ

@retval     bool	true:�ɹ�	false:ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CACCCard::GetProductInfo(WORD productType, WORD productSubType, ACCCARD_PRODUCT_INFO& productInfo)
{
	for (vector<ACCCARD_PRODUCT_INFO>::iterator iter = v_ProductInfo.begin(); iter != v_ProductInfo.end(); ++iter)
	{
		if (iter->productType == productType && iter->productSubType==productSubType)
		{
			productInfo = *iter;
			return true;
		}
	}

	return false;
}