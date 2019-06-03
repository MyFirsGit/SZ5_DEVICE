#pragma once
#include "scapi.h"
#include <list>
#include <vector>
#include "ParamHelper.h"
#include "scmessagecontentdefine.h"
#include "TxnDataEnum.h"
using namespace std;

#define  theSCMessageParaseHelper CSCMessageParseHelper::GetInstance()
class CSCMessageParseHelper
{
public:

	static CSCMessageParseHelper& GetInstance()
	{
		return instance;
	}
	~CSCMessageParseHelper();
/************************************************************************/
/* ��������                                                 */
/************************************************************************/
	bool ParseUploadSpecifyBlockNOData(LPBYTE lpContent,DATA_TYPE_CODE& dataType, std::vector<CString>& blockNOs);
	bool ParseUploadDebugData(LPBYTE lpContent,_DATE& date,CString& ftpDirectory);
	bool ParseOperationData(LPBYTE lpContent,WORD& mode);
	bool ParseDeviceControl(LPBYTE lpContent,WORD& mode);
	bool Parse24HourMode(LPBYTE lpContent,BYTE& modeCode);
	bool ParseDelayOperationData(LPBYTE lpContent,WORD& delayTime);
	bool ParseIssueRestrict(LPBYTE lpContent,BYTE& restrictType,int& stationCode);
	bool ParseParameterAndSoftwareUpdate(LPBYTE lpContent,std::vector<ParameterAndSoftwareUpdate>& updates);

	/************************************************************************/
	/* ֪ͨ����                                                 */
	/************************************************************************/
	bool ParseUnlockStaff(LPBYTE lpContent,int& staffID);
/************************************************************************/
/*   ��������                                                */
/************************************************************************/
	bool ParseLoginOrOutReply(LPBYTE lpContent,BYTE& result);
	bool ParseChangePasswordReply(LPBYTE lpContent,BYTE& result);
private:
	CSCMessageParseHelper();
	static CSCMessageParseHelper instance;
};