#pragma once
#include "utilobject.h"
#include "CoreExp.h"
#include <list>
#include <afxmt.h>

class CORE_API CUtilList :
	public CUtilObject
{
public:
	CUtilList(void);
	~CUtilList(void);

	void AddTail(CUtilObject* pItem);
	void AddHead(CUtilObject* pItem);
	BOOL IsEmpty();


	CUtilObject* RemoveHead();
	void Remove(CUtilObject* pItem);

	CUtilObject* GetHead();
	int GetCount();
	void First();
	BOOL Next(CUtilObject** ppItem);
	void RemoveAll();

private:
	std::list<CUtilObject*> m_list;
	std::list<CUtilObject*>::iterator m_listiter;
    CMutex m_mutex;
};
