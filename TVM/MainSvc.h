#pragma once
#include "MenuDlg.h"
#include "TVMForeService.h"
#include "audithelper.h"
#include "TVMRWHelper.h"
//#include "BanknoteModuleHelper.h"
#include "BHChangeHelper.h"
#include "BRHModuleHelper.h"
#include "CoinModuleHelper.h"
#include "PrinterHelper.h"
#include "TokenHelper.h"
#include "THHelper.h"
#include "IOModuleHelper.h"
#include "AudioHelper.h"
#include "StatusDisplayModuleHelper.h"
#include "DataHelper.h"
//#include "BHRfidHelper.h"
#include "GuiParaMgr.h"
#include "WebServerHelper.h"

class CMainSvc :
    public CTVMForeService
{

	DECLARE_MESSAGE_MAP()

public:
	typedef enum _tagTimers
	{
		TIMER_NTP				= 1,	// NTP��ʱ��ʱ��
		TIMER_DEFAULT_SERVICE	= 2,	// Ĭ��ҵ��ʱ��
		TIMER_AUTOMATIC_LOGOFF	= 3,    // �Զ��ǳ���ʱ��
		TIMER_BUSIDAY_SWITH		= 4,    // ��Ӫ���л���ʱ��
		TIMER_IO_MONITOR		= 5,	// IO��ؼ�ʱ��
		TIMER_RETURN_INDICATOR	= 6,	// ����ָʾ�Ƽ�ʱ�� 
		TIMER_RETURN_LIGHT		= 7,	// �����ȼ�ʱ��
		TIMER_READY_TO_REST		= 8,	// �ȴ��е������м�ʱ��
		TIMER_OUT_OF_TIME_LOGIN = 9,	// �ȴ���½��ʱ����ά���ſ���
		TIMER_REPEAT_EVENT_AUDIO= 10,	// �����ظ����ż�ʱ��
		TIMER_BLIND_OPERATION_OUT_OF_TIME = 11,//ä�˹�Ʊ������ʱ��ʱ��
		TIMER_WEB_LOCAL_PROCESS	= 12,	// ��ά�붩�����ش���ʱ��
		TIMER_PARAMETER_UPDATE	= 13,	// SC���ز������ض�ʱ��
	}TIMERS;

    CMainSvc(void);
    ~CMainSvc(void);

	void StartOperation();										// ��Ӫ��ʼ
    void CloseOperation(void);                                  // ��Ӫ����

	void ResumeService();										// ��ʼ����
	void PauseService();										// ��ͣ����

    void Shutdown(void);                                        // �ػ�
	void RestartTVM();											// ��������
	void ExitTVM();												// �˳�TVM����
	void RepairInitException();									// �޸���ʼ���쳣
	void StartDefaultService();									// ����Ĭ�Ϸ���
	void SetDegradedMode(bool bOnOrOff);						// ���ý������У���/��

	// IO Module
	void PostAssistanceButtonActionMSG();						// ������Ԯ��ť������Ϣ
	void PostDoorActionMSG();									// ����ά���Ŷ�����Ϣ
	void FeelSomeCloseDevice();									// ���봫������Ӧ����
	void WhenShockActive();										// �𶯴�������Ӧ
	void ReSetHelpStatus(){m_bIsCallHelpRunning = false;UPORDOWN ^= 1;};
	// ҵ���л�
	bool IsServiceBusyOnPassger();								// �жϵ�ǰservice״̬�Ƿ��г˿Ͳ���

	// ä�˹�Ʊ
	void StartManagedBlind(BLIND_OPERATION_STATUS status);		// �й���Ʊ�ͳ�ֵʣ��Ĺ���
	bool OnClickButtonWithoutPinPad(bool isStop = false);		// ����Ʊ������MainSvc����ʱ����˽���İ�ť�Ĵ���
	delegate<void (SERVICE_ID,bool*)> StartingForeService;
	delegate<void()> busidaySwitch;
	delegate<void()> hideAllCompoment;							// �йܷ���Ĭ��ҵ��ʱ��ť������
	bool ResponseAssistBtn(UINT uMsg);                                       // ��Ӧ��Ԯ��ť

protected:
	void OnStart();
	// IO Module Rsp
	void CheckSetModuleStatus();											 // �ж������趨״̬
private:
    typedef enum _dialog_group {
        DIALOG_GROUP_BASE = 1,
		DIALOG_GROUP_NAVI = 2,
    } DIALOG_GROUP;
    void ReleaseController();                                               // �ͷ�Controller
	LRESULT OnForceLogout(WPARAM wParam,LPARAM lParam);
	LRESULT OnNetStatusChange(WPARAM wParam,LPARAM lParam);
	LRESULT OnShutDown(WPARAM wParam,LPARAM lParam);
	LRESULT OnCloseOperation(WPARAM wParam,LPARAM lParam);
	LRESULT OnDeviceControl(WPARAM wParam,LPARAM lParam);
	LRESULT OnStartNtpTimer(WPARAM wParam,LPARAM lParam);
	LRESULT OnStartSwithBusinessDayTimer(WPARAM wParam,LPARAM lParam);
	LRESULT OnNewWorkMode(WPARAM wParam,LPARAM lParam);
	LRESULT OnNewOperationMode(WPARAM wParam,LPARAM lParam);				// ��Ӧ��Ӫģʽ����
	LRESULT OnStartIOMonitor(WPARAM wParam,LPARAM lParam);					// ��ӦIO�����Ϣ
	LRESULT OnStartReturnIndicator(WPARAM wParam,LPARAM lParam);			// ��Ӧ����ָʾ�Ƽ����Ϣ
	LRESULT OnStartReturnLight(WPARAM wParam,LPARAM lParam);				// ��Ӧ���������Ƽ����Ϣ
	LRESULT OnStartReadyToRest(WPARAM wParam,LPARAM lParam);				// ��Ӧ�ȴ��е������м����Ϣ
	LRESULT OnFrontDoorClose(WPARAM wParam,LPARAM lParam);					// ��Ӧ���Ų�����Ϣ
	LRESULT OnFrontDoorOpenWithoutLogin(WPARAM wParam,LPARAM lParam);		// �����ſ�δ��½��ʱ��
	LRESULT OnFrontDoorOpenTimeOutOperation(WPARAM wParam,LPARAM lParam);	// ��������Ա������ʱ��ʱ��
	LRESULT OnPinPadMonitorReturn(WPARAM wParam,LPARAM lParam);				// ������̼�ط���
	LRESULT OnSwitchBusinessDayAndCloseOperation(WPARAM wParam,LPARAM lParam);// ִ����Ӫ���л����ս�
	LRESULT OnServiceTimerControl(WPARAM wParam,LPARAM lParam);				// ������ƶ�ʱ��
	LRESULT OnWebIdleProcess(WPARAM wParam,LPARAM lParam);					// ��ά�붩�����ش���
	LRESULT OnRechargeServerMonitor(WPARAM wParam,LPARAM lParam);			// ��ֵ���������
	LRESULT OnParameterUpdateProcess(WPARAM wParam,LPARAM lParam);			// SC���ز�������

	void	OnButtonClicking(CXButton*,bool*);
	void	OnEditChanged(CGEdit*,CString);
	void	OnTimer(UINT nEventID);
	void	GetPaymentInfoFromFuncIni(WORD& device_pay_status);				// ��Function.ini�л�ȡ֧����ʽ
	void    StartOutOfService();											// ������ͣ����

	void    JudgeIsOperatorLogin();											// �ж�վԱ�Ƿ��½
	bool    IsAutoDownGradeNeedWarn();										// �ж��Ƿ����Զ�������ʾ
	void	PlayEventAudio(DWORD eventID,bool bStopRepeat = false);			// ���������ӿ�
	_DATE m_CurrentBusiDate;												// ��ǰ��Ӫ��
	int UPORDOWN;															// ��ť��ǰ״̬
	int OPENORCLOSE;														// ά���ŵ�ǰ״̬
	int SOMEONECLOSE;														// ���˿���TVM
	int WHENSHOCKACTIVE;													// �𶯴�������Ӧ
	IO_RSP_STATUS_INFO m_IOStatusRsp;										// IO��ǰ״̬
	bool m_bIsAramOpen;														// �������Ƿ���Ӧ��
	bool m_bIsBeginRunning;													// �жϵ�ǰ�Ƿ�ʼ��Ӫ
	bool m_bIsEndRunning;													// �жϵ�ǰ�Ƿ������Ӫ
	bool m_bIsStartingDefaultServic;										// �Ƿ���������Ĭ��ҵ��
	bool m_bIsCallHelpRunning;												// ��Ԯ�Ƿ����ڽ���?

	bool m_bIsMenualRepare;													// �Ƿ��ֶ�����쳣
	/*************************��λ�����ж�*******************/
	// ��λ���������״̬
	bool CheckSvcStatus();													// ��Ӧ��λ�·�״̬
	bool m_bIsSvcMsgComeSystemBusy;											// ��λ�·������λæµ��
	DEVICE_CONTRL_CODE m_device_contrl_code;								// ��λ�·��������
	// ģʽ����
	OperationalMode_t m_operational_mode;									// ��λ�·���ģʽ���ƴ���
	bool m_bIsOperaArryBusy;												// ��λ�·�ģʽ���ƴ���ʱ���豸æµ

	bool m_bIsServerMsgArrial;												// ��λ����Ϣ�·�����ֹͣ����ʱ
	bool m_bBeforeRunCommandArrial;											// ��Ӫ��ʼǰ��λ�·���Ϣ
	bool m_bEndRuningCommandArrial;											// ��Ӫ��������λ�·���Ϣ

	// ��Ӫ�л�
	void OnSwitchBussinessDay();											// ��Ӧ��Ӫ�л�
	bool m_bIsSwitchBussinessDay;											// �л���Ӫ��

	// ��Ӫ����
	bool m_bIsStartOperation;												// �Ƿ��Ѿ���ʼ��Ӫ��������Ӫʱ������λ�·���Ӫ��ʼ��Ҫ��¼�������ļ���
	bool m_bIsReadyStartOperation;											// ��Ӫʱ�䵽�˻�������Ӫʱ���п����Ƿ���Ӫ��ʼ�ˣ�
	bool m_bIsStartOperationWhenOpen;										// �����߹�ʱ��û�У�

	bool m_bPuseCheckOperation;												// �Ƿ���ͣ��Ӫ���л����

private:
	int m_timerInterval;
private:
	CTVMRWHelper* m_pRWHelper;
	CTVMRWHelper* m_pRechargeHelper;
	CPrinterHelper* m_pPrinterHelper;
	CTHHelper* m_pCardModuleHelper;
	CTOKENHelper* m_pTokenHelper;
	CIOModuleHelper* m_pIOHelper;
	//CBanknoteModuleHelper* m_pBanknoteModuleHelper;
	CBRHModuleHelper* m_pBanknoteModuleHelper;
	//CBHChangeHelper *m_pBHChangeHelper;
	CBRHModuleHelper* m_pBHChangeHelper;
	CCoinModuleHelper* m_pCoinModuleHelper;
	//CStatusDisplayModuleHelper* m_pStatusDisplayModuleHelper;
	CDataHelper* m_pDataHelper;
	CAudioHelper* m_pAudioHelper;
	void CheckIniValue(WORD& wService_Status,WORD& wPayment_Status,WORD& wChange_Status);// ��������ļ����úϷ���
public:
	CTVMRWHelper* GetRWHelper();

	CPrinterHelper*	GetPrinterHelper();
	CTHHelper* GetCardModuleHelper();
	CIOModuleHelper* GetIOModuleHelper();
	//CBanknoteModuleHelper* GetBanknoteModuleHelper();
	CBRHModuleHelper* GetBanknoteModuleHelper();
	//CBHChangeHelper* GetBHChangeHelper(); 
	CBRHModuleHelper* GetBHChangeHelper(); 
	CCoinModuleHelper* GetCoinModuleHelper();
	//CStatusDisplayModuleHelper* GetStatusDisplayModuleHelper();
	CDataHelper* GetDataHelper();
	CAudioHelper* GetAudioHelper();

	// ��־����
public:
	void DeleteLog();														// ɾ�����й�����־
	void DeleteLog(int days);												// ɾ��ָ������ǰ��FTP�ϴ�����־
	void DeleteTpuLog(int days);											// ɾ��ָ������ǰ����־�ļ�
	void DeletePrintHistory(int days);										// ɾ����ӡ��ʷ��¼
private:
	void GetLimitTpuLogDate(int days,vector<CString>& logdate);				// ��ȡָ��������Tpulog��־�ļ�����
	void GetLimitLogDate(int days,vector<int>& logdate);					// ��ȡָ��������log��־�ļ�����
	void GetLimitPrintLogDate(int days,CString strPath,vector<CString>& vecNeedDel,bool bIsDir = false);// ȡ��ָ��Ŀ¼��ָ������Ӧ��ɾ������֮����ļ�
};

extern CTVMRWHelper* RECHARGERW_HELPER;
extern CTVMRWHelper* CARDRW_HELPER;

extern CPrinterHelper* PRINTER_HELPER;
extern CTHHelper* CARD_HELPER;
extern CTOKENHelper* TOKEN_HELPER;
extern CIOModuleHelper* IO_HELPER;
//extern CBanknoteModuleHelper* BANKNOTE_HELPER;
extern CBRHModuleHelper* BANKNOTE_HELPER;
//extern CBHChangeHelper * BH_CHANGE_HELPER;
extern CBRHModuleHelper * BH_CHANGE_HELPER;
extern CCoinModuleHelper* COIN_HELPER;
extern CDataHelper* DATA_HELPER;
extern CAudioHelper* AUDIO_HELPER;
//extern CBHRfidHelper* BH_RFID_HELPER;