#pragma once
#include <wtypes.h>
#include <vector>
#include "ParameterDef.h"
#include "CAfcTVMOperationParam.h"
#include "CAfcAccessLevelParam.h"
#include "CAfcDeviceCommonParam.h"
#include "CAfcStaffPwdParam.h"
#include "CAfcDeviceTimeParam.h"
#include "CAfcOperatorCodeTableParam.h"
#include "CAccBusinessParam.h"
#include "CAccServiceFeeParam.h"
#include "CAccLineInfoParam.h"
#include "CAccTicketParam.h"
#include "CAccNotServiceStation.h"
#include "cacctollstationparam.h"
#include "caccsingleblacklistparam.h"
#include "caccsectionblacklistparam.h"
#include "caccbasepriceparam.h"
#include "caccpricelevelparam.h"
#include "caccpriceadjuest.h"
#include "cacctimetypeparam.h"
#include "cacctimedetailparam.h"
#include "cacctimefloatparam.h"
#include "CAccDiscountParam.h"
#include "caccwritelistparam.h"
#include "caccothercitycard.h"
#include "caccovertimeadjuestparam.h"
#include "CAccDeviceControlParam.h"
#include "CAccIssueCompanyParam.h"
#include "CAccSectionParam.h"
// CParamHelper��Ψһ�ӿ�
#define thePARAM_HELPER CParamHelper::GetInstance()              

#define CD_NO_VERSION 0xFFFF
/**
@brief  ����������
*/

class CParamHelper
{
public:

	// �������ͱ���
	typedef	enum _param_type
	{
		ACC_VER  = 1,//ACC
		ECT_VER	 = 2,//ECT
		AFC_VER  = 3,//AFC
		UNKNOW   = 255 //δ֪
	} PARAM_TYPE;

	// �汾�л���¼
	typedef struct _version_change_info {
		WORD paramId;		             // ����ID
		PARAM_VERSION_TYPE_CODE versionType;		// �汾����
		DWORD oldVersion;		// �ɰ汾��
		_DATE oldStartDate;		// �ɰ汾��Ч����
		DWORD newVersion;		// �°汾��
		_DATE newStartDate;		// �°汾��Ч����
		_version_change_info(){
			paramId = PARAM_UNKNOW;
			versionType = VER_TYPE_UNKNOW;
			oldVersion = 0;
			newVersion = 0;		
		}
	}VERSION_CHANGE_INFO;
	
	typedef std::vector<VERSION_CHANGE_INFO> VEC_VERSION_INFO;		// �汾�л���Ϣ
	typedef std::vector<WORD>	VEC_PARAM_ID;									  // ����ID����
	bool	InitParam();
	// �����ӿں���	
	bool   InitAllParam();														// ��ʼ�����в���
	bool   InitAccParam(bool flag);										// ��ʼ��Acc����
//	bool   InitAccParamNoVersionChange(bool flag);			// ��ʼ��ACC����(�����汾�л�)
//	bool   InitEctParam();													// ��ʼ��Ect����
	bool   InitAfcParam(bool readflag);								// ��ʼ��Afc����

// 	void GetCurrentCDVesion(CD_VERSION&,WORD&);						     // ����CD�汾��Ϣ���õ�ǰCD�汾�����ص�ǰ�汾
// 	void SetCurrentCDVesion(CD_VERSION);								 // ���õ�ǰCD�汾�������ڴ�

	VEC_VERSION_INFO CheckAndChangeToFutVerAcc();				         // ���������л���δ���汾��ACC������
	//VEC_VERSION_INFO ChangeFutVerToCurVer(PARAM_TYPE);			     // ��ȫ��δ���汾�л�����ǰ�汾
	void ChangeFutVerToCurVer(PARAM_TYPE paraType,CString sPrefix);	     // ��ָ��δ���汾�л�����ǰ�汾
	void DeleteTheParamFile(CString sParamPrefix,BYTE bVerType);		 // ɾ��ָ�������ļ�
	bool DeleteAllFileFromTheDir(CString sDirPath);					     // ɾ��ָ��Ŀ¼�������ļ�

	void UpdateLocalVersion(VEC_VERSION_INFO&, bool bUpdate=true);	// ���µ�ǰ�汾

	void RestoreCurVersion(PARAM_TYPE,VEC_PARAM_ID,bool allFlag = true); // ��ԭ��ǰ��ʽ�汾

	void GetCurAccParamVersion(VERSION_ACC&);	// ��ȡACC��ǰ�����汾
	void GetFutAccParamVersion(VERSION_ACC&);	// ��ȡACCδ�������汾

	//void GetCurEctParamVersion(VERSION_ECT&);	// ��ȡECT��ǰ�����汾
	//void GetFutEctParamVersion(VERSION_ECT&);	// ��ȡECTδ�������汾

	void GetCurAfcParamVersion(VERSION_AFC&);	// ��ȡAFC��ǰ�����汾
	void GetFutAfcParamVersion(VERSION_AFC&);	// ��ȡAFCδ�������汾

	

	PARAM_VERSION_TYPE_CODE GetParamVersionType(CString);			// ��ȡ�����汾����
	CString GetParamVerFromParamName(CString sParamName);			// ��ȡ�����汾

	CString GetTheParamPathName(WORD paraID,CString ParamPath);		// ��ȡָ��Ŀ¼�µĲ����ļ���

	CString GetTheParamBackUpPathName(WORD paraID,CString ParamPath);		// ��ȡָ��Ŀ¼�µĲ����ļ���

	void RemoveParamTransHead(CString sParamPath);			// ɾ��ָ���ļ��������в����Ĵ���Э��ͷ

	CString GetProgBatchNoFromProgName(CString sProgName);	// ��ȡ�������κ�
	CString GetProgVerFromProgName(CString sProgName);		// ��ȡ����汾��
	PARAM_VERSION_TYPE_CODE GetProgVersionType(CString);			// ��ȡ�����汾����
	CString GetFileNameFromFullPath(CString sPathAndName);
	static CParamHelper&  GetInstance(){
		return theInstance;
	}


	CString GetCurXorSumVersion();
	CString GetFutXorSumVersion();
	std::map<PARAMETER_ID,CParameterMaster*> GetVersionProvider(){return mapVersionProvider;};
	CString* GetParameterNames(){return parameterNames;};
private:

	void GetVesionChangeInfo(VEC_VERSION_INFO&);      // ��ȡACC�汾�л���Ϣ

	CString GetTheParamPathNameImp(WORD paraID,CString ParamPath,CString basePath);		// ��ȡָ��Ŀ¼�µĲ����ļ���

	CString GetCurParamName(CString,PARAM_TYPE,CString&,bool isBackup = false);			// ��ȡָ����ǰ����·�����ļ���
	CString GetFutParamName(CString,PARAM_TYPE,CString&,bool isBackup = false);			// ��ȡָ��δ������·�����ļ���
	PARAM_TYPE GetParamType(CString);								// ��ò�������

	void BackUpLocalCurVersion(PARAM_TYPE,CString,CString);			// ���ݵ�ǰ����
	void GetVersionChangeInfo(CString,CString,VERSION_CHANGE_INFO&);// ��ȡ�汾�л���Ϣ
	bool CompareVersion(DWORD ver1, DWORD ver2);					// �����汾�Ƚ�
	
	bool CheckIsNeedChangeParam(CString);							// �������Ƿ���Ҫ�л���δ���汾
	CString ConvertFutFileNameToCurrentVersion(CString);            // �ѽ����汾�ļ����޸�Ϊ��ǰ�汾

	// ����,��������
	CParamHelper(void);
	~CParamHelper();
	CParamHelper(const CParamHelper&);
	CParamHelper& operator=(const CParamHelper&);

	static  CParamHelper theInstance;   // �����Ψһ�ӿ�

	CString m_strCurVersion;
	CString m_strFurVersion;
	std::map<PARAMETER_ID,CParameterMaster*> mapVersionProvider;//AFC��ACC������������Ӧ�ó���
	CString parameterNames[32];
};
