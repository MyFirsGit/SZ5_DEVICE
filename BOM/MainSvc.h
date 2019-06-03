#pragma once
#include "MenuDlg.h"
#include "BOMForeService.h"
#include "audithelper.h"

class CMainSvc :
    public CBOMForeService
{

	DECLARE_MESSAGE_MAP()

public:
	typedef enum _tagTimers
	{
		TIMER_NTP = 1,			//NTP��ʱ��ʱ��
		TIMER_DEFAULT_SERVICE=2,	//Ĭ��ҵ��ʱ��
		TIMER_AUTOMATIC_LOGOFF=3, //�Զ��ǳ���ʱ��
		TIMER_BUSIDAY_SWITH=4//��Ӫ���л���ʱ��
	}TIMERS;

    CMainSvc(void);
    ~CMainSvc(void);

    virtual void OnModeChange(UINT mode);                                   // ģʽ�仯��Ӧ����

    void CloseOperation(void);                                              // ҵ�����
    void Shutdown(void);                                                    // �ػ�
	void RestartBOM();														// ��������
	void ExitBOM();
	void RepairInitException();
	void StartDefaultService();

	delegate<void (SERVICE_ID,bool*)> StartingForeService;

protected:
	void OnStart();
private:
    typedef enum _dialog_group {
        DIALOG_GROUP_BASE = 1,
    } DIALOG_GROUP;
    void ReleaseController();                                               // �ͷ�Controller
	LRESULT OnForceLogout(WPARAM wParam,LPARAM lParam);
	LRESULT OnNetStatusChange(WPARAM wParam,LPARAM lParam);
	LRESULT OnShutDown(WPARAM wParam,LPARAM lParam);
	LRESULT OnCloseOperation(WPARAM wParam,LPARAM lParam);
	LRESULT OnStartNtpTimer(WPARAM wParam,LPARAM lParam);
	LRESULT OnStartSwithBusinessDayTimer(WPARAM wParam,LPARAM lParam);
	LRESULT OnNewWorkMode(WPARAM wParam,LPARAM lParam);
	void  OnButtonClicking(CXButton*,bool*);
	void OnEditChanged(CGEdit*,CString);
	void OnTimer(UINT nEventID);
	_DATE currentBusiDate;
};
