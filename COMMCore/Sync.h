#pragma once

#include "stdafx.h"
#include <list>
#include <afxmt.h>
#include "CoreExp.h"

#define DECLARE_SYNC_OBJECT(objectName)\
class CORE_API  C##objectName##SysObject\
{\
private:\
	static CCriticalSection s_##objectName##SysObject;\
public:\
	C##objectName##SysObject();\
	~C##objectName##SysObject();\
};

#define IMPLEMENT_SYNC_OBJECT(objectName)\
CCriticalSection C##objectName##SysObject::s_##objectName##SysObject;\
C##objectName##SysObject::C##objectName##SysObject(){\
	s_##objectName##SysObject.Lock();\
}\
C##objectName##SysObject::~C##objectName##SysObject(){\
	s_##objectName##SysObject.Unlock();\
}

DECLARE_SYNC_OBJECT(SYNCManager)

#define USING_SYNC(objectName) C##objectName##SysObject m_##objectName##SysObject;

#define SYNC_SYNCManager() USING_SYNC(SYNCManager)

class CORE_API CSyncObjectEx{
private:
	CString syncObjectName;
	int refernceCount;
	CCriticalSection* p_SyncObject;
	CSyncObjectEx(CString name);
	

public:
	static CSyncObjectEx* Create(CString name);
	virtual ~CSyncObjectEx();
	friend class CSyncObjectManager;
};


class CORE_API CSyncObjectManager{

private:
	static std::list<CSyncObjectEx*> listSyncObjects;
	static CCriticalSection* AquireCriticalSectionByName(CString name);
	static void ReleaseCriticalSectionByName(CString name);

	CString syncName;
	CCriticalSection* p_CurrentCriticalSection;
	
public:
	CSyncObjectManager(CString syncName);
	~CSyncObjectManager();

};

#define SYNC(SYNC_NAME, SYNC_KEY) CSyncObjectManager m_##SYNC_NAME##SyncObject(SYNC_KEY);

