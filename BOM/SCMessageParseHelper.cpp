#include "stdafx.h"
#include "SCMessageParseHelper.h"
#include "TxnDataEnum.h"

CSCMessageParseHelper CSCMessageParseHelper::instance;

CSCMessageParseHelper::CSCMessageParseHelper()
{

}

CSCMessageParseHelper::~CSCMessageParseHelper()
{

}

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
		//CString csDataMsgNo = _T("");
		//csDataMsgNo = changeBuff2String((char*)lpContent,14);
		CHAR pDataMsgNo[28+2]={0x00};
		PSTR2BCD((char*)lpContent,14,pDataMsgNo);	
		lpContent+=14;
		USES_CONVERSION;
		CString csDataMsgNo = A2T(pDataMsgNo);
		blockNOs.push_back(csDataMsgNo);
	}
	return true;
}


bool CSCMessageParseHelper::ParseUploadDebugData(LPBYTE lpContent,_DATE& date,CString& ftpDirectory)
{
	date = BCD2Date((char*)lpContent);
	lpContent+=4;
	char cPath[65]={0};
	memcpy(cPath,lpContent,64);
	USES_CONVERSION;
	//ftpDirectory.SetString(cPath);
	ftpDirectory = A2T(cPath);
	return true;
}


bool CSCMessageParseHelper::ParseOperationData(LPBYTE lpContent,WORD& mode)
{
	memcpy(&mode,lpContent,2);
	return true;
}


bool CSCMessageParseHelper::ParseDeviceControl(LPBYTE lpContent,WORD& mode)
{
	memcpy(&mode,lpContent,2);
	return true;
}

bool CSCMessageParseHelper::Parse24HourMode(LPBYTE lpContent,BYTE& modeCode)
{
	memcpy(&modeCode,lpContent,1);
	return true;
}

bool CSCMessageParseHelper::ParseDelayOperationData(LPBYTE lpContent,WORD& delayTime)
{
	memcpy(&delayTime,lpContent,2);
	return true;
}

bool CSCMessageParseHelper::ParseIssueRestrict(LPBYTE lpContent,BYTE& restrictType,int& stationCode)
{
	memcpy(&restrictType,lpContent,1);
	lpContent+=1;
	stationCode = ComMakeWORD(*lpContent,*(lpContent+1));
	//stationCode = BCD2int((char*)lpContent,2);
	return true;
}

bool CSCMessageParseHelper::ParseUnlockStaff(LPBYTE lpContent,int& staffID)
{
	staffID = BCD2int((char*)lpContent,3);
	return true;
}

bool CSCMessageParseHelper::ParseLoginOrOutReply(LPBYTE lpContent,BYTE& result)
{
	result = *(lpContent+5);
	return true;
}

bool CSCMessageParseHelper::ParseChangePasswordReply(LPBYTE lpContent,BYTE& result)
{
	result = *(lpContent+7);
	return true;
}