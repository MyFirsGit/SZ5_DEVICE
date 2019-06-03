#include "stdafx.h"
#include "SCMessageParseHelper.h"
#include "TxnDataEnum.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief  该类的唯一实例    
*/
//////////////////////////////////////////////////////////////////////////
CSCMessageParseHelper CSCMessageParseHelper::instance;

//////////////////////////////////////////////////////////////////////////
/*
@brief   构造函数   

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
@brief    析构函数  

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
@brief   解析参数和程序更新的反馈数据   

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
@brief      解析上传指定包命令

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
@brief    解析上传调试数据命令  

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
@brief  解析运营模式控制命令    

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
@brief     解析设备控制命令 

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
@brief    解析24小时模式控制  

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
@brief     解析延长运营命令 

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
@brief   解析发售限制命令   

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
@brief    解析账户解锁命令  

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
@brief      解析登录登出请求

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
@brief  解析密码修改反馈    

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
@brief     解析个性化查询反馈 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::ParsePersonalizationReply(LPBYTE lpContent,vector<Personalization>& result)
{
	// 2	BIN		个人化信息数量
	WORD personCount = 0;
	memcpy(&personCount,lpContent,2);
	lpContent+=2;
	// 个人化信息
	for (int i=0;i<personCount;i++)
	{
		Personalization personalization;
		// 5	ASCII	持卡人的头衔，如先生、女士、教授等
		memcpy(personalization.cardHolderTitle,lpContent,5);
		lpContent+=5;
		// 61	ASCII	持卡人姓名
		memcpy(personalization.cardHolderName,lpContent,61);
		lpContent+=61;
		// 1	BIN		持卡人选定的写入票卡的的语言（备用）
		memcpy(&personalization.languagePreference,lpContent,2);
		lpContent+=2;
		// 4	BCD		Date of birth of cardholder
		personalization.dateOfBirth = BCD2Date((char*)lpContent);
		lpContent+=4;
		// 21	ASCII	持卡人日间电话
		memcpy(personalization.cardholderPhoneDay,lpContent,21);
		lpContent+=21;
		// 21	ASCII	持卡人家庭电话
		memcpy(personalization.cardholderPhoneHome,lpContent,21);
		lpContent+=21;
		// 129	ASCII	持卡人电子邮件
		memcpy(personalization.cardholderEmail,lpContent,129);
		lpContent+=129;
		// 21	ASCII	持卡人密码提示
		memcpy(personalization.cardholderClue,lpContent,21);
		lpContent+=21;
		// 16	ASCII	持卡人密码
		memcpy(personalization.cardholderPassword,lpContent,16);
		lpContent+=16;
		// 30	ASCII	关于持卡人的员工ID
		memcpy(personalization.staffID,lpContent,30);
		lpContent+=30;
		// 1	BIN		持卡人性别 1: Male. 男, 2: Female. 女, 255: No value set. 无设定值。
		personalization.sex = *lpContent;
		lpContent++;
		// 1	BIN		标明卡面是否已印制照片：1=已印制，0=未印制。
		personalization.photePrinted = *lpContent;
		lpContent++;
		// 129	ASCII	持卡人地址详细资料1
		memcpy(personalization.address1,lpContent,129);
		lpContent+=129;
		// 129	ASCII	持卡人地址详细资料2
		memcpy(personalization.address2,lpContent,129);
		lpContent+=129;
		// 19	ASCII	持卡人提供的个人身份。
		memcpy(personalization.cardholderNumber,lpContent,19);
		lpContent+=19;
		// 1	BIN		定义持卡人编号中使用的个人身份类型
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
@brief     解析历史信息查询反馈 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSCMessageParseHelper::ParseHistoryInfoQuerryReply(LPBYTE lpContent,vector<HistoryProductInfo>& result)
{
	DWORD cardIssuerId = 0;	        // 发卡方ID
	DWORD cardSerialNumber = 0;	    // 逻辑卡号
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