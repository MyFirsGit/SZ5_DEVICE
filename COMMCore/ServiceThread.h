#pragma once
#include "UtilThread.h"
#include "service.h"
#include "CoreExp.h"

/**
@brief	Service��ʹ�õ��߳���                                                                   
*/
class CORE_API CServiceThread :
    public CUtilThread
{
    DECLARE_DYNCREATE(CServiceThread)
    DECLARE_MESSAGE_MAP()
	
	typedef struct _ThreadSendMessageStruct
	{
		UINT message;
		WPARAM wParam;
		LPARAM lParam;
		LRESULT lResult;
		HANDLE handle;
		_ThreadSendMessageStruct()
		{
			message = 0;
			wParam =0;
			lParam=0;
			lResult = SP_ERR_INTERNAL_ERROR;
			handle = INVALID_HANDLE_VALUE;
		}
	} THREADSENDMESSAGESTRUCT,*LPTHREADSENDMESSAGESTRUCT;

public:
    CServiceThread(void);                                           // ���캯��
    ~CServiceThread(void);                                          // ��������
	void SetService(CService* pService);
    bool IsBusy();                                                  // �Ƿ�æµ
	int GetPendMsgCount();											// ��ȡδ������Ϣ��
	BOOL PostThreadServiceMsg(UINT msg,WPARAM wParam,LPARAM lParam);
	LRESULT SendThreadServiceMsg(UINT msg,WPARAM wParam,LPARAM lParam);
private:
	void OnPostServiceMsg(WPARAM wParam, LPARAM lParam);	//������Ϣ
	void OnSendServiceMsg(WPARAM wParam,LPARAM lParam);
    void DispachCmdMsg(UINT message, WPARAM wParam, LPARAM lParam); // ����Service������Ӧ����
	
    CService* m_pService;                                           // ������Service
    int m_pendMsgCount;                                             // δ�������Ϣ��
};
