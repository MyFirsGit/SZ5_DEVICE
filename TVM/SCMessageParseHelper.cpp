#include "stdafx.h"
#include "SCMessageParseHelper.h"
#include "TxnDataEnum.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief  �����Ψһʵ��    
*/
//////////////////////////////////////////////////////////////////////////
CSCMessageParseHelper CSCMessageParseHelper::instance;

//////////////////////////////////////////////////////////////////////////
/*
@brief   ���캯��   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CSCMessageParseHelper::CSCMessageParseHelper()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief    ��������  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CSCMessageParseHelper::~CSCMessageParseHelper()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ���������ͳ�����µķ�������   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::ParseParameterAndSoftwareUpdate(LPBYTE lpContent, std::vector<ParameterAndSoftwareUpdate> &updates)
{
	WORD usDownloadNum = 0;
	memcpy(&usDownloadNum,lpContent,sizeof(WORD));
	lpContent+=sizeof(WORD);
	for (int i=0;i<usDownloadNum;i++)
	{
		ParameterAndSoftwareUpdate  parameterAndSoftwareUpdate(lpContent);
		lpContent+=84;
		updates.push_back(parameterAndSoftwareUpdate);
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      �����ϴ�ָ��������

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::ParseUploadSpecifyBlockNOData(LPBYTE lpContent,DATA_TYPE_CODE& dataType,std::vector<CString>& blockNOs)
{
	
	switch(*lpContent)
	{
	case 0x01:
		dataType = ACC_TRANS;
		break;
	case 0x02:
		dataType = ECT_TRANS;
		break;
	case 0x03:
		dataType = AFC_TRANS;
		break;
	case 0x04:
		dataType = BUS_DATA;
		break;
	case 0x05:
		dataType = EVEN_DATA;
		break;
	case 0x09:
		dataType = LOG_DATA;
		break;
	default:
		dataType = UNKNOW_DATA;
		break;
	}
	lpContent ++;
	DWORD dwNumber = 0;
	memcpy(&dwNumber,lpContent,sizeof(DWORD));
	lpContent+=sizeof(DWORD);

	for (int i=0;i<dwNumber;i++)
	{
		CHAR pDataMsgNo[28+2]={0x00};
		BCD2PSTR((char*)lpContent,14,pDataMsgNo);
		lpContent+=14;
		USES_CONVERSION;
		CString csDataMsgNo = A2T(pDataMsgNo);
		blockNOs.push_back(csDataMsgNo);
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief    �����ϴ�������������  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::ParseUploadDebugData(LPBYTE lpContent,_DATE& date,CString& ftpDirectory)
{
	date = BCD2Date((char*)lpContent);
	lpContent+=4;
	char cPath[65]={0};
	memcpy(cPath,lpContent,64);
	USES_CONVERSION;
	ftpDirectory = A2T(cPath);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  ������Ӫģʽ��������    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::ParseOperationData(LPBYTE lpContent,WORD& mode)
{
	memcpy(&mode,lpContent,2);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     �����豸�������� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::ParseDeviceControl(LPBYTE lpContent,WORD& mode)
{
	//memcpy(&mode,lpContent,2);
	mode = MAKEWORD(*(lpContent+1),*lpContent);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    ����24Сʱģʽ����  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::Parse24HourMode(LPBYTE lpContent,BYTE& modeCode)
{
	memcpy(&modeCode,lpContent,1);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     �����ӳ���Ӫ���� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::ParseDelayOperationData(LPBYTE lpContent,WORD& delayTime)
{
	memcpy(&delayTime,lpContent,2);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ����������������   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::ParseIssueRestrict(LPBYTE lpContent,BYTE& restrictType,int& stationCode)
{
	memcpy(&restrictType,lpContent,1);
	lpContent+=1;
	//stationCode = ComMakeWORD(*lpContent,*(lpContent+1));
	stationCode = BCD2int((char*)lpContent,2);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    �����˻���������  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::ParseUnlockStaff(LPBYTE lpContent,int& staffID)
{
	staffID = BCD2int((char*)lpContent,3);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������¼�ǳ�����

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::ParseLoginOrOutReply(LPBYTE lpContent,BYTE& result)
{
	result = *(lpContent+5);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  ���������޸ķ���    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::ParseChangePasswordReply(LPBYTE lpContent,BYTE& result)
{
	result = *(lpContent+7);
	return true;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief     �������Ի���ѯ���� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::ParsePersonalizationReply(LPBYTE lpContent,vector<Personalization>& result)
{
	// 2	BIN		���˻���Ϣ����
	WORD personCount = 0;
	memcpy(&personCount,lpContent,2);
	lpContent+=2;
	// ���˻���Ϣ
	for (int i=0;i<personCount;i++)
	{
		Personalization personalization;
		// 5	ASCII	�ֿ��˵�ͷ�Σ���������Ůʿ�����ڵ�
		memcpy(personalization.cardHolderTitle,lpContent,5);
		lpContent+=5;
		// 61	ASCII	�ֿ�������
		memcpy(personalization.cardHolderName,lpContent,61);
		lpContent+=61;
		// 1	BIN		�ֿ���ѡ����д��Ʊ���ĵ����ԣ����ã�
		memcpy(&personalization.languagePreference,lpContent,2);
		lpContent+=2;
		// 4	BCD		Date of birth of cardholder
		personalization.dateOfBirth = BCD2Date((char*)lpContent);
		lpContent+=4;
		// 21	ASCII	�ֿ����ռ�绰
		memcpy(personalization.cardholderPhoneDay,lpContent,21);
		lpContent+=21;
		// 21	ASCII	�ֿ��˼�ͥ�绰
		memcpy(personalization.cardholderPhoneHome,lpContent,21);
		lpContent+=21;
		// 129	ASCII	�ֿ��˵����ʼ�
		memcpy(personalization.cardholderEmail,lpContent,129);
		lpContent+=129;
		// 21	ASCII	�ֿ���������ʾ
		memcpy(personalization.cardholderClue,lpContent,21);
		lpContent+=21;
		// 16	ASCII	�ֿ�������
		memcpy(personalization.cardholderPassword,lpContent,16);
		lpContent+=16;
		// 30	ASCII	���ڳֿ��˵�Ա��ID
		memcpy(personalization.staffID,lpContent,30);
		lpContent+=30;
		// 1	BIN		�ֿ����Ա� 1: Male. ��, 2: Female. Ů, 255: No value set. ���趨ֵ��
		personalization.sex = *lpContent;
		lpContent++;
		// 1	BIN		���������Ƿ���ӡ����Ƭ��1=��ӡ�ƣ�0=δӡ�ơ�
		personalization.photePrinted = *lpContent;
		lpContent++;
		// 129	ASCII	�ֿ��˵�ַ��ϸ����1
		memcpy(personalization.address1,lpContent,129);
		lpContent+=129;
		// 129	ASCII	�ֿ��˵�ַ��ϸ����2
		memcpy(personalization.address2,lpContent,129);
		lpContent+=129;
		// 19	ASCII	�ֿ����ṩ�ĸ�����ݡ�
		memcpy(personalization.cardholderNumber,lpContent,19);
		lpContent+=19;
		// 1	BIN		����ֿ��˱����ʹ�õĸ����������
		personalization.cardholderNumberType = *lpContent;
		lpContent++;
		// 4	BIN		Defines the card logic serial number for the passenger.
		personalization.cardSerialNumber = ComMakeLong(*lpContent,*(lpContent+1),*(lpContent+2),*(lpContent+3));
		lpContent+=4;
		
		result.push_back(personalization);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ������ʷ��Ϣ��ѯ���� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::ParseHistoryInfoQuerryReply(LPBYTE lpContent,vector<HistoryProductInfo>& result)
{
	DWORD cardIssuerId = 0;	        // ������ID
	DWORD cardSerialNumber = 0;	    // �߼�����
	memcpy(&cardIssuerId,lpContent,sizeof(DWORD));
	lpContent+=sizeof(DWORD);
	memcpy(&cardSerialNumber,lpContent,sizeof(DWORD));
	lpContent+=sizeof(DWORD);

	DWORD productCount = ComMakeLong(*lpContent,*(lpContent+1),*(lpContent+2),*(lpContent+3));
	lpContent+=sizeof(DWORD);

	for (int i=0;i<productCount;i++)
	{
		HistoryProductInfo historyProductInfo;
		historyProductInfo.cardIssuerId = cardIssuerId;
		historyProductInfo.cardSerialNumber = cardSerialNumber;
		memcpy(&historyProductInfo.ProductType,lpContent,sizeof(historyProductInfo.ProductType));
		lpContent+=sizeof(historyProductInfo.ProductType);
		memcpy(&historyProductInfo.ProductVariantType,lpContent,sizeof(historyProductInfo.ProductVariantType));
		lpContent+=sizeof(historyProductInfo.ProductVariantType);

		DWORD TransactionCount =   ComMakeLong(*lpContent,*(lpContent+1),*(lpContent+2),*(lpContent+3));
		lpContent+=sizeof(DWORD);

		for (int i=0;i<TransactionCount;i++)
		{
			HistoryTranceInfo historyTranceInfo;
			memcpy(&historyTranceInfo.TransactionStation,lpContent,sizeof(historyTranceInfo.TransactionStation));
			lpContent+=sizeof(historyTranceInfo.TransactionStation);
			memcpy(&historyTranceInfo.TransactionDevice,lpContent,sizeof(historyTranceInfo.TransactionDevice));
			lpContent+=sizeof(historyTranceInfo.TransactionDevice);
			memcpy(&historyTranceInfo.TransactionType,lpContent,sizeof(historyTranceInfo.TransactionType));
			lpContent+=sizeof(historyTranceInfo.TransactionType);
			historyTranceInfo.TransactionValue = ComMakeLong(*lpContent,*(lpContent+1),*(lpContent+2),*(lpContent+3));
			lpContent+=sizeof(historyTranceInfo.TransactionValue);
			historyTranceInfo.RemainingValue = ComMakeLong(*lpContent,*(lpContent+1),*(lpContent+2),*(lpContent+3));
			lpContent+=sizeof(historyTranceInfo.RemainingValue);
			memcpy(&historyTranceInfo.PaymentMethod,lpContent,sizeof(historyTranceInfo.PaymentMethod));
			lpContent+=sizeof(historyTranceInfo.PaymentMethod);
			historyTranceInfo.PaymentValue = ComMakeLong(*lpContent,*(lpContent+1),*(lpContent+2),*(lpContent+3));
			lpContent+=sizeof(historyTranceInfo.PaymentValue);
			historyProductInfo.TransactionRecord.push_back(historyTranceInfo);
		}
		result.push_back(historyProductInfo);
	}
	return true;
}