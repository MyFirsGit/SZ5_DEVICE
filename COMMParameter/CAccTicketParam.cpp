#include "StdAfx.h"
#include "CAccTicketParam.h"
#include "LogException.h"
#include "CommonDefine.h"
#include "ticketname.h"
#include "BomInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAccTicketParam CAccTicketParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAccTicketParam::CAccTicketParam():CAccParamMaster()
{
	m_vec_ticketsTypeList_param.clear();	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�����ļ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAccTicketParam::GetParamFileName()
{
	GetParamFileNameSub(ACC_PARAM_ID_0401);		// ��ȡ�����ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAccTicketParam::~CAccTicketParam(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ�������

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccTicketParam::ParseBody(LPBYTE lpData)
{
	try{
		// ��1�Σ���Ʊ��Ϣ       
		// ƫ��������
		m_vec_ticketsTypeList_param.clear();
		m_vec_ticketTypeList_param.clear();
		lpData += m_section[0].startPosition;
		if (m_section[0].recordCount != 0) { 			
			WORD fareTypeInfoList_len;
			memcpy(&fareTypeInfoList_len,lpData,2);
			lpData += 2;      
			for(int i = 0;i<fareTypeInfoList_len;i++){
				TICKET_TYPE tmpFareTypeInfo;
				//��Ʊ�����ʹ���
				tmpFareTypeInfo.ticketTypebaseCode = BCD2int((char*)lpData,1);	
				lpData ++;
				//��Ʊ�����ʹ���
				tmpFareTypeInfo.ticketTypeCode = BCD2int((char*)lpData,1);
				tmpFareTypeInfo.ticketTypeName_cn = theTICKET_NAME.GetTicketNameCn(tmpFareTypeInfo.ticketTypeCode);
				tmpFareTypeInfo.ticketTypeName_en = theTICKET_NAME.GetTicketNameEn(tmpFareTypeInfo.ticketTypeCode);
				lpData ++;
				//��Ʊ���/������
				memcpy(&tmpFareTypeInfo.MaxBalance,lpData,4);
				lpData +=4;
				//�Ƿ������ֵ
				tmpFareTypeInfo.chargeable = BCD2int((char*)lpData,1);
				lpData ++;
				//��ֵ����
				memcpy(&tmpFareTypeInfo.maxChargeSize,lpData,4);
				lpData +=4;	
				//����ʱ�շѷ�ʽ
				tmpFareTypeInfo.payMathodOfAdjuest = BCD2int((char*)lpData,1);
				lpData ++;
				//�Ƿ������˿�
				tmpFareTypeInfo.refoundable = BCD2int((char*)lpData,1);
				lpData ++;
				//�˿�ʱʹ�ô�������
				tmpFareTypeInfo.sendBackablecLimitCount = *lpData;
				lpData ++;		
				//�Ƿ������ճ�������
				tmpFareTypeInfo.hasLimitOfOneday = BCD2int((char*)lpData,1);
				lpData ++; 
				//�ճ�����������
				tmpFareTypeInfo.maxMountOfOneDay = *lpData;//BCD2int((char*)lpData,2);
				lpData ++;  
				//�Ƿ�����Ԥ��ֵ
				tmpFareTypeInfo.prechargeable = BCD2int((char*)lpData,1);
				lpData ++;
				//��Ч��	
				memcpy(&tmpFareTypeInfo.vilidateData,lpData,2);
				lpData += 2; 
				//���ӳ�ʱ��	
				memcpy(&tmpFareTypeInfo.extendTime,lpData,2);
				lpData += 2; 
				//Ѻ��
				memcpy(&tmpFareTypeInfo.deposit,lpData,4);
				lpData +=4;		
				//ʹ��ǰ�Ƿ��輤��
				tmpFareTypeInfo.needActive = BCD2int((char*)lpData,1);
				lpData ++;	
				//�Ƿ��������
				tmpFareTypeInfo.checkBlackList = BCD2int((char*)lpData,1);
				lpData ++;	
				//�Ƿ������/���
				tmpFareTypeInfo.checkBalance = BCD2int((char*)lpData,1);
				lpData ++;	
				//�Ƿ�����Ч��
				tmpFareTypeInfo.checkValidate = BCD2int((char*)lpData,1);
				lpData ++;	
				//�Ƿ����������
				tmpFareTypeInfo.checkSequence = BCD2int((char*)lpData,1);
				lpData ++;		
				//�Ƿ��鳬ʱ
				tmpFareTypeInfo.checkOvertime = BCD2int((char*)lpData,1);
				lpData ++;		
				//�Ƿ��鳬��
				tmpFareTypeInfo.checkOverJourney = BCD2int((char*)lpData,1);		
				lpData ++;		
				//�Ƿ�������·
				tmpFareTypeInfo.checkLimitLine = BCD2int((char*)lpData,1);
				lpData ++;	
				//�Ƿ�ֻ����վ����
				tmpFareTypeInfo.checkOnlyThis = BCD2int((char*)lpData,1);	
				lpData ++;	
				//�Ƿ�����վ��ͣ��ʱ��
				tmpFareTypeInfo.checkStickTime = BCD2int((char*)lpData,1);	
				lpData ++;	
				//վ��ͣ��ʱ������
				memcpy(&tmpFareTypeInfo.maxStickTime,lpData,2);	
				lpData +=2;	
				//�ɳ�ֵ���豸
				memcpy(&tmpFareTypeInfo.chargeDevice,lpData,1);	
				lpData ++;
				//��ʹ�õ��豸
				memcpy(&tmpFareTypeInfo.UseableDevice,lpData,1);	
				lpData ++;
				//��վ�������Ʊ��
				tmpFareTypeInfo.checkInFlag = BCD2int((char*)lpData,1);	
				lpData ++;	
				//β���Ż�
				tmpFareTypeInfo.favourable =  BCD2int((char*)lpData,1);
				lpData ++;		
				//���ձ�ʶ
				tmpFareTypeInfo.recorverFlag = BCD2int((char*)lpData,1);
				lpData ++;	
				//�Ʒѷ�ʽ1BCD�Ʒ�����
				tmpFareTypeInfo.chargeType =  BCD2int((char*)lpData,1);
				lpData += 1;
				//͸֧��ʶ
				tmpFareTypeInfo.overdraftFlag =  BCD2int((char*)lpData,1);
				lpData += 1;
				//��ֵƱ��С�����
				tmpFareTypeInfo.checkMinBalanceFlag =  BCD2int((char*)lpData,1);
				lpData += 1;
				//��ֵƱ��������
				tmpFareTypeInfo.checkMaxBalanceFlag =  BCD2int((char*)lpData,1);
				lpData += 1;
				//��վʱ����
				memcpy(tmpFareTypeInfo.checkEnterTime, lpData,3);
				lpData += 3;
				//��Ʊ������
				memcpy(&tmpFareTypeInfo.refoundFee,lpData,4);
				lpData += 4;
				//��Ʊ������
				memcpy(&tmpFareTypeInfo.exchangeFee,lpData,4);
				lpData += 4;
				//������Ʊ��Ʊ�����
				memcpy(&tmpFareTypeInfo.amercementOfloseCard,lpData,4);
				lpData += 4;
				//������Ʊ��Ϊ�𻵷����
				memcpy(&tmpFareTypeInfo.amercementOfBrokenCard,lpData,4);
				lpData += 4;	
				//��ʱ�����
				memcpy(&tmpFareTypeInfo.overTimeFee,lpData,4);
				lpData += 4;
				m_vec_ticketsTypeList_param.push_back(tmpFareTypeInfo);
			}
		}
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
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
void CAccTicketParam::ParseSectionInfo(LPBYTE lpData)
{
	try{
		if(m_packageHead.sectionCount != 0){			
			for(int i =0;i<m_packageHead.sectionCount;i++){
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
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
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
bool CAccTicketParam::CheckBodyInfo(LPBYTE lpData)
{   
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ʊ����

@param      none

@retval     vector<FARE_TYPE_LIST>   ��Ʊ������������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const vector<CAccTicketParam::TICKET_TYPE>&  CAccTicketParam::GetTicketTypeListParam()
{
	return m_vec_ticketsTypeList_param;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡָ����Ʊ����

@param      (i)WORD ticketTypeCode   ��Ʊ���ͱ���(���ֽ���Ч)
@param  	(o)TICKET_TYPE_LIST& ticket_type_Param      ��Ʊ������Ϣ  

@retval     int  0 �ɹ� 		 1 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const int  CAccTicketParam::GetTicketInfo(WORD ticketTypeCode,CAccTicketParam::TICKET_TYPE& ticket_type_Param)
{
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;

	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == LOBYTE(ticketTypeCode)){
			ticket_type_Param = *it;
			return 0;
		}
	}
	return 1;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡָ�����ͳ�Ʊ����Ч����

@param      (i)WORD ticketTypeCode			��Ʊ�����ͱ���(���ֽ���Ч)
@param  	(o)_DATE_TIME& validateDate     ��Ʊ��Ч��  

@retval     int    -1   ʧ��	
					0	��Զ��Ч
					1	һֱ��Ч
					2	�ۺ�/��ֵ��һ��������Ч
					3	�״�ʹ�ú�һ��������Ч(�����״�ʹ��)
					4	�̶���ʼ��Ч��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const int  CAccTicketParam::GetTicketValidate(WORD ticketTypeCode,_DATE_TIME& validateDate)
{
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;

	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == LOBYTE(ticketTypeCode)){
			_DATE date;
			date.wYear = validateDate.wYear;
			date.biMonth = validateDate.biMonth;
			date.biDay = validateDate.biDay;
			_DATE day = GetDayAfter(date,it->vilidateData);	
			validateDate.wYear = day.wYear;
			validateDate.biMonth = day.biMonth;
			validateDate.biDay = day.biDay;
			validateDate.biHour = 23;
			validateDate.biMinute = 59;
			validateDate.biSecond = 59;			
		}
	}
	return -1;	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡָ�����ͳ�Ʊ��Ѻ����Ϣ

@param      (i)DWORD ticketTypeCode   ��Ʊ���ͱ���(���ֽ���Ч)
@param  	(o)DWORD* deposit        ��ƱѺ��

@retval     int  0 �ɹ�         1 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const int  CAccTicketParam::GetTicketDeposit(WORD ticketTypeCode,LONG& deposit)
{
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;

	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == LOBYTE(ticketTypeCode)){
			deposit = it->deposit;
			return 0;
		}
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡָ�����ͳ�Ʊ���������Ƽ�Ӣ������

@param      (i)WORD ticketTypeCode             ��Ʊ���ͱ���(���ֽ���Ч)
@param      (o)CString& ticketTypeName_cn       ��Ʊ����������
@param      (o)CString& ticketTypeName_en		��Ʊ����Ӣ����

@retval     int  0 �ɹ�         1 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString  CAccTicketParam::GetProductName(WORD ticketTypeCode,LANGUAGETYPE_T language)
{
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;

	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == LOBYTE(ticketTypeCode)){
			if (language == CHINESE)
			{
				return it->ticketTypeName_cn;
			}
			else
				return it->ticketTypeName_en;
		}
	}
	return _T("");
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ݳ�Ʊ�����ͻ�ȡ��Ʊ��������

@param      (i)BYTE ticketSubTypeCode     ��Ʊ�����ͱ���

@retval     BYTE  ��Ʊ�����ͱ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const BYTE  CAccTicketParam::GetTicketMainType(BYTE ticketSubTypeCode)
{
	BYTE byTicketMainType = 0;
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;
	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == ticketSubTypeCode){
			byTicketMainType = it->ticketTypebaseCode;
			break;
		}
	}
	return byTicketMainType;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ��ǲ�����Ʊ��

@param      (i)BYTE ticketSubTypeCode  Ʊ��������

@retval     true:��Ҫ���� false:����Ҫ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAccTicketParam::IsNotCollectedTicket(BYTE ticketSubTypeCode)
{
	bool iscollect = false;
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;
	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == ticketSubTypeCode){
			// ������Ʊ��
			if(it->recorverFlag == 0x01){
				iscollect = true;
				break;
			}			
		}
	}
	return iscollect;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡָ�����͵ĳ�Ʊ�ļƷ�����

@param      (i)WORD ticketTypeCode              ��Ʊ���ͱ���(���ֽ���Ч)
@param      (o)TicketType_t& ticket_categry     ��Ʊ����

@retval     int  0 �ɹ�         1 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BYTE CAccTicketParam::GetTicketCategry(WORD ticketTypeCode)
{
	ProductCategory_t ticket_categry;
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;
	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == LOBYTE(ticketTypeCode)){
			switch(it->chargeType){			
			case 01:
				ticket_categry = PERIOD;
				break;
			case 02:
				ticket_categry = PURSE;
				break;
			case 03:
				ticket_categry = MULTIRIDE;
				break;
			default:
				return -1;
				break;
			}
		}
	}
	return ticket_categry;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡָ�����͵ĳ�Ʊ�ļƷ�����

@param      (i)WORD ticketTypeCode              ��Ʊ���ͱ���(���ֽ���Ч)
@param      (o)TicketType_t& ticket_categry     ��Ʊ����

@retval     int  0 �ɹ�         1 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const int CAccTicketParam::GetTicketCategry(WORD ticketTypeCode,ProductCategory_t& ticket_categry)
{
	if(theBOM_INFO.IsUseConfigTicketCategory())
	{
		int ticketCategory = theBOM_INFO.GetConfigTicketCategory(ticketTypeCode);
		if(ticketCategory>0)
		{
			ticket_categry = (ProductCategory_t)ticketCategory;
			return 0;
		}
	}

	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;
	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == LOBYTE(ticketTypeCode)){
			switch(it->chargeType){			
			case 01:
				ticket_categry = PERIOD;
				break;
			case 02:
				ticket_categry = PURSE;
				break;
			case 03:
				ticket_categry = MULTIRIDE;
				break;
			default:
				return -1;
				break;
			}
			return 0;
		}
	}
	return -1;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡָ�����ͳ�Ʊ���������Ƽ�Ӣ������

@param      (i)WORD ticketTypeCode             ��Ʊ���ͱ���(���ֽ���Ч)
@param      (o)CString& ticketTypeName_cn       ��Ʊ����������
@param      (o)CString& ticketTypeName_en		��Ʊ����Ӣ����

@retval     int  0 �ɹ�         1 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const int  CAccTicketParam::GetTicketTypeName(WORD ticketTypeCode,CString& ticketTypeName_cn,CString& ticketTypeName_en)
{
	vector<CAccTicketParam::TICKET_TYPE>::iterator  it;

	for (it=m_vec_ticketsTypeList_param.begin(); it!=m_vec_ticketsTypeList_param.end(); it++){
		if(it->ticketTypeCode == LOBYTE(ticketTypeCode)){
			ticketTypeName_cn = it->ticketTypeName_cn;
			ticketTypeName_en = it->ticketTypeName_en;
			return 0;
		}
	}
	return 1;
}
