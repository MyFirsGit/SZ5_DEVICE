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
  @brief	各项业务服务基类                                                                   
*/
class CORE_API CService
{
	//DECLARE_DYNAMIC(CService)
    DECLARE_MESSAGE_MAP()

public:
	typedef vector<CService*> ServiceChildren;
	static const AFX_MSGMAP_ENTRY _messageEntries[];    // 消息函数关联结构
	static const AFX_MSGMAP messageMap;                 // 消息映射MAP
    CService(UINT serviceID, UINT parentServiceID, bool isBackGround);
	virtual ~CService();
	delegate<void (CService*)> Starting;
	delegate<void (CService*)> Started;
	void Start();													//启动服务
	delegate<void (CService*)> Stopping;
	delegate<void (CService*)> Stopped;
	void Stop();													//停止服务
	virtual BOOL PostMessage(UINT message,WPARAM wParam,LPARAM lParam)=0;
	virtual LRESULT SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam )=0;
    UINT GetServiceID();                          // 获取Service ID
    UINT GetParentServiceID();                    // 获取上级Service ID
    CService* GetParent();                              // 获取上级Service
    ServiceChildren& GetChildren();                     // 获取下级Service
    bool IsBackground();                                // 是否是后台Service
    bool IsForeground();                                // 是否是前台Service
    void AddChild(CService* pChild);                    // 添加子Service
    void DeleteChild(CService* pChild);                 // 删除子Service
    void DeleteChildren();                              // 删除全部子Service
	void StopChildren();									//停止全部子Service
    CService* SearchService(UINT serviceID);      // 在本Service中根据Service ID查找Service
	bool StartTimer(UINT timerID, int seconds);         // 启动定时器
	void StopTimer(UINT timerID);                       // 停止定时器
	LRESULT DispachCmdMsg(UINT message, WPARAM wParam, LPARAM lParam);
	CBag* GetBag();
protected:
	virtual void OnStart() {};                          // Service启动后处理
	virtual bool OnStop() {return true;};               // Service停止前处理
private:
    void DeleteChild(int index);                        // 删除子Service
    UINT m_serviceID;                             // Service ID
    UINT m_parentServiceID;                       // 上级Service ID
    CService* m_pParent;                                // 上级Service
    ServiceChildren m_children;                         // 下级Service
    bool m_isBackground;                                // 是否是后台Service
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

// 以下部分用于Service的MessageMap
// 命令形式
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

// 通用命令
#define ON_SERVICE(id, memberFxn) \
{ WM_COMMAND, CN_COMMAND, (WORD)id, (WORD)id, serviceSig_General, \
	(AFX_PMSG) (static_cast<void (AFX_MSG_CALL CService::*)(UINT, void*)> (memberFxn)) }, 

// Service定时器
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
