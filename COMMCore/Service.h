#pragma once
#include <afxtempl.h>
#include <vector>
#include "Command.h"
#include "CoreExp.h"
#include "Bag.h"
#include "delegate.h"

class CCommand;
class CSysException;
using namespace std;

/**
  @brief	����ҵ��������                                                                   
*/
class CORE_API CService
{
	//DECLARE_DYNAMIC(CService)
    DECLARE_MESSAGE_MAP()

public:
	typedef vector<CService*> ServiceChildren;
	static const AFX_MSGMAP_ENTRY _messageEntries[];    // ��Ϣ���������ṹ
	static const AFX_MSGMAP messageMap;                 // ��Ϣӳ��MAP
    CService(UINT serviceID, UINT parentServiceID, bool isBackGround);
	virtual ~CService();
	delegate<void (CService*)> Starting;
	delegate<void (CService*)> Started;
	void Start();													//��������
	delegate<void (CService*)> Stopping;
	delegate<void (CService*)> Stopped;
	void Stop();													//ֹͣ����
	virtual BOOL PostMessage(UINT message,WPARAM wParam,LPARAM lParam)=0;
	virtual LRESULT SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam )=0;
    UINT GetServiceID();                          // ��ȡService ID
    UINT GetParentServiceID();                    // ��ȡ�ϼ�Service ID
    CService* GetParent();                              // ��ȡ�ϼ�Service
    ServiceChildren& GetChildren();                     // ��ȡ�¼�Service
    bool IsBackground();                                // �Ƿ��Ǻ�̨Service
    bool IsForeground();                                // �Ƿ���ǰ̨Service
    void AddChild(CService* pChild);                    // �����Service
    void DeleteChild(CService* pChild);                 // ɾ����Service
    void DeleteChildren();                              // ɾ��ȫ����Service
	void StopChildren();									//ֹͣȫ����Service
    CService* SearchService(UINT serviceID);      // �ڱ�Service�и���Service ID����Service
	bool StartTimer(UINT timerID, int seconds);         // ������ʱ��
	void StopTimer(UINT timerID);                       // ֹͣ��ʱ��
	LRESULT DispachCmdMsg(UINT message, WPARAM wParam, LPARAM lParam);
	CBag* GetBag();
protected:
	virtual void OnStart() {};                          // Service��������
	virtual bool OnStop() {return true;};               // Serviceֹͣǰ����
private:
    void DeleteChild(int index);                        // ɾ����Service
    UINT m_serviceID;                             // Service ID
    UINT m_parentServiceID;                       // �ϼ�Service ID
    CService* m_pParent;                                // �ϼ�Service
    ServiceChildren m_children;                         // �¼�Service
    bool m_isBackground;                                // �Ƿ��Ǻ�̨Service
	CBag* pBag;

};

struct ServiceMsg
{
	CService* pService;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
	ServiceMsg()
	{
		pService = NULL;
		message = 0;
		wParam = 0;
		lParam = 0;
	}
};

// ���²�������Service��MessageMap
// ������ʽ
enum serviceSig
{
	serviceSig_end = 0,               // [marks end of message map]
	serviceSig_General, 				// void (int, void*)
	serviceSig_v_ui,                  // void (UINT)
	serviceSig_l_wl,					// LRESULT (WPARAM,LPARAM)
	serviceSig_v_PRequestItem,		// void (UINT, CRequestItem*)
	serviceSig_v_PException		// void (UINT,CSysException*)
};

typedef void (AFX_MSG_CALL CService::*SERVICE_PMSG)(void);

union ServiceMessageMapFunctions
{
	SERVICE_PMSG pfn;   // generic member function pointer
	void (AFX_MSG_CALL CService::*pfn_General)(UINT, void*);
	void (AFX_MSG_CALL CService::*pfn_v_ui)(UINT);
	LRESULT (AFX_MSG_CALL CService::*pfn_l_wl)(WPARAM,LPARAM);
	void (AFX_MSG_CALL CService::*pfn_Service_PRequestItem)(UINT, CCommand*);
	void (AFX_MSG_CALL CService::*pfn_Service_PException)(CSysException*,CCommand*);
};

// ͨ������
#define ON_SERVICE(id, memberFxn) \
{ WM_COMMAND, CN_COMMAND, (WORD)id, (WORD)id, serviceSig_General, \
	(AFX_PMSG) (static_cast<void (AFX_MSG_CALL CService::*)(UINT, void*)> (memberFxn)) }, 

// Service��ʱ��
#define ON_SERVICE_TIMER() \
{ WM_COMMAND, ST_SERVICE_TIMER, (WORD)0, (WORD)0, serviceSig_v_ui, \
	(AFX_PMSG) (static_cast<void (AFX_MSG_CALL CService::*)(UINT)> (OnTimer)) }, 

#define  ON_COMMAND_COMPLETE(CommandID,memberFxn)\
{WM_COMMAND,CN_COMMAND,(WORD)CommandID,(WORD)CommandID,serviceSig_v_PRequestItem,\
	(AFX_PMSG)(static_cast<void(AFX_MSG_CALL CService::*)(UINT,CCommand*)>(memberFxn))},

#define ON_SERVICE_MESSAGE(message,memberFxn)\
{WM_COMMAND,message,(WORD)0,(WORD)0,serviceSig_l_wl,\
	(AFX_PMSG)(static_cast<LRESULT(AFX_MSG_CALL CService::*)(WPARAM,LPARAM)>(memberFxn))},

#define  ON_COMMAND_EXCEPTION(memberFxn)\
{WM_COMMAND,SP_EXCEPTION,(WORD)0,(WORD)0,serviceSig_v_PException,\
	(AFX_PMSG)(static_cast<void(AFX_MSG_CALL CService::*)(CSysException*,CCommand*)>(memberFxn))},
