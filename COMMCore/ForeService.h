#pragma once
#include "DialogFlow.h"
#include "Service.h"
#include "CoreExp.h"

//#include "BR.h"


/**
@brief	ǰ̨�������
*/

class CORE_API CForeService :
    public CService
{
		DECLARE_MESSAGE_MAP()
public:
    CForeService(UINT serviceID, UINT parentServiceID,CDialogFlow* dialogFlow = NULL);      // ���캯��
    ~CForeService();                                                                // ��������

	virtual BOOL PostMessage(UINT message,WPARAM wParam, LPARAM lParam);
	virtual LRESULT SendMessage(UINT message,WPARAM wParam, LPARAM lParam);
  
    bool static IsWaitingCommand();                                                 // �Ƿ��ڵȴ����������
    void static SetWaitingCommand(bool isWaiting);                                  // ���õȴ����������
	void DoDialogFlow(const TCHAR* pButtonMsg);                                      // ��������ת��ť��Ϣ
	CDialogFlow* GetDialogFlow();
	virtual bool StartTimer(UINT timerID, int seconds);         // ������ʱ��
	virtual void StopTimer(UINT timerID);                       // ֹͣ��ʱ��
protected:
	virtual void OnStart();                                                         // Service��������
	virtual bool OnStop();                                                          // Serviceֹͣǰ����
    CDialogFlow* m_pDialogFlow;                                                     // �Ի�����ת����
private:
    bool static m_isWaitingCommand;                                                 // �Ƿ��ڵȴ����������
	LRESULT OnStartTimer(WPARAM,LPARAM);
	LRESULT OnStopTimer(WPARAM,LPARAM);
};
