#include "stdafx.h"
#include <afxmt.h>
#include "Sync.h"

IMPLEMENT_SYNC_OBJECT(SYNCManager)



CSyncObjectEx::CSyncObjectEx(CString name){
	this->syncObjectName = name;
	this->refernceCount = 0;
	this->p_SyncObject = new CCriticalSection();
}

CSyncObjectEx::~CSyncObjectEx(){
	if(p_SyncObject!=NULL){
		delete p_SyncObject;
		p_SyncObject = NULL;
	}
}

CSyncObjectEx* CSyncObjectEx::Create(CString name){
	return new CSyncObjectEx(name);
}


std::list<CSyncObjectEx*> CSyncObjectManager::listSyncObjects;

CCriticalSection* CSyncObjectManager::AquireCriticalSectionByName(CString name){
	SYNC_SYNCManager();
	CSyncObjectEx* p_SyncObjectEx = NULL;
	std::list<CSyncObjectEx*>::iterator iter;
	for(iter = listSyncObjects.begin();iter!=listSyncObjects.end();iter++){
		if(name.CompareNoCase((*iter)->syncObjectName) == 0){
			p_SyncObjectEx = *iter;
			break;
		}
	}

	//如果没有找到，则添加
	if(p_SyncObjectEx==NULL){
		p_SyncObjectEx = CSyncObjectEx::Create(name); 
		listSyncObjects.push_back(p_SyncObjectEx);
	}

	ASSERT(p_SyncObjectEx!=NULL);
	p_SyncObjectEx->refernceCount++;
	return p_SyncObjectEx->p_SyncObject;

}
void CSyncObjectManager::ReleaseCriticalSectionByName(CString name){
	
	//ASSERT(name!=NULL);
	SYNC_SYNCManager();
	CSyncObjectEx* p_SyncObjectEx = NULL;
	std::list<CSyncObjectEx*>::iterator iter;
	for(iter = listSyncObjects.begin();iter!=listSyncObjects.end();iter++){
		if(name.CompareNoCase((*iter)->syncObjectName) == 0){
			p_SyncObjectEx = *iter;
			break;
		}
	}
	if(p_SyncObjectEx!=NULL){
		p_SyncObjectEx->refernceCount--;
		if(p_SyncObjectEx->refernceCount <=0){
			delete p_SyncObjectEx;
			listSyncObjects.remove(p_SyncObjectEx);
		}
	}
}

CSyncObjectManager::CSyncObjectManager(CString syncName){
	this->syncName = syncName;
	p_CurrentCriticalSection = AquireCriticalSectionByName(syncName);
	p_CurrentCriticalSection->Lock();
}

CSyncObjectManager::~CSyncObjectManager(){
	if(p_CurrentCriticalSection!=NULL){
		p_CurrentCriticalSection->Unlock();
		p_CurrentCriticalSection = NULL;
	}
	ReleaseCriticalSectionByName(syncName);
}
