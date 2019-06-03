#pragma once
#include "BackService.h"
#include "ParamHelper.h"
#include "scapi.h"
#include "SCSvcDef.h"
#include <list>
#include "SCMessageContentDefine.h"
using namespace std;

/**
@brief ���ڼ���Service
*/
class CBusinessTranslateSvc :
	public CBackService
{
	DECLARE_MESSAGE_MAP()

public:
	CBusinessTranslateSvc(void);
	~CBusinessTranslateSvc(void);

public:
	void StartBusinessIntervalTimer();
	void EndBusinessIntervalTimer();
	void InsertMsgUpdateParameterAndProgram(CParamHelper::VEC_VERSION_INFO& Vec_VersionInfo);		// 6.4.2  �����ͳ������
	void InsertMsgDownloadParameterAndProgram(vector<ParameterAndSoftwareUpdate>&);  				// 6.4.3  �����ͳ�������ҵ������
	void InsertMsgImportantStatusChange(SERVICE_STATUS status);  									// 6.4.4  �豸��Ҫ״̬�ı�	
	//void InsertMsgIsamCheckIn(ISAM_AUTH isam_auth, BYTE result);									// 6.4.5  ISAMǩ��/ǩ��
	void InsertMsgOperationLog(OPERATOR_LOG operLog);												// 6.4.6  ������־(���Ƽ�ʹ��)
	void InsertMsgOperationLog(OPERATION_CODE operCode,bool operResult = true,CString strOperComment = _T("")); // 6.4.6	������־
	void InsertMsgDeviceErrorData(DEVICE_ERRORDATA& ErrorData);										// 6.4.7  �豸��������
	void InsertMsgDeviceRegisterData(AR_REASON arReson);											// 6.4.8  �豸�Ĵ�������
	void InsertMsgMagazineExchange(MAGAZINE_CHANGE& magazine_change);								// 6.4.10 Ʊ�����
	void InsertMsgBanknoteBoxExchange(CASHBOX_CHANGE& cashbox_change);								// 4.2.16 Ǯ�����
	//void InsertMsgDeviceComponentInfo();															// 6.4.11 �豸��������
	void InsertMsgBussnessFailedData(BUSINESS_FAILED_DATA inBusiness_data);							// TVM���ϵ�ҵ������

	long SendBusinessData();
protected:
	void OnStart();
	bool OnStop();
private:
	enum ENUM_TIMER {
		TIMER_REGISTER = 1,         // ҵ�����ݼĴ�����ʱ��
		TIMER_PACKAGE = 2			//���ʱ�䶨ʱ��
	};

	void OnTimer(UINT nEventID);
	LRESULT OnStartTimer(WPARAM,LPARAM);
	LRESULT OnEndTimer(WPARAM,LPARAM);
	
};
