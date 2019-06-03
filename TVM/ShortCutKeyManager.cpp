#include "stdafx.h"
#include "ShortCutKeyManager.h"
#include "XButton.h"
#include <algorithm>
#include "MessageIDDefinition.h"

//////////////////////////////////////////////////////////////////////////
//
// 快捷键管理类唯一实例
//////////////////////////////////////////////////////////////////////////
CShortCutKeyManager CShortCutKeyManager::instance;

//////////////////////////////////////////////////////////////////////////
/*
@brief      快捷键管理类构造函数

@param     无

@retval     无

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
CShortCutKeyManager::CShortCutKeyManager()
{

}
//////////////////////////////////////////////////////////////////////////
/*
@brief      快捷键管理类析构函数
					将保存的快捷键及按钮对应关系删掉。

@param     无

@retval     无

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
CShortCutKeyManager::~CShortCutKeyManager()
{
	for(map<UINT,vector<CXButton*>>::iterator iterItem = container.begin();iterItem!=container.end();iterItem++)
	{
		vector<CXButton*> vecButton = iterItem->second;
		vecButton.clear();
	}
	container.clear();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得快捷键管理类唯一对象

@param     无

@retval     CShortCutKeyManager& 快捷键对象

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
CShortCutKeyManager& CShortCutKeyManager::GetInstance()
{
	return instance;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      注册一个快捷键，将一个快捷键同一个按钮关联起来。

@param	(i)UINT key					快捷键
@param	(i)CXButton * button 按钮

@retval     无

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
void CShortCutKeyManager::RegisterShortCutKey(UINT key, CXButton * button)
{
	UnRegisterShortCutKey(button);
	container[key].push_back(button);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取消一个已经注册的快捷键按钮。


@param	(i)CXButton * button 按钮

@retval     无

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
void CShortCutKeyManager::UnRegisterShortCutKey(CXButton * button)
{
	for(map<UINT,vector<CXButton*>>::iterator iterItem = container.begin();iterItem!=container.end();iterItem++)
	{
		vector<CXButton*> vecButton = iterItem->second;
		vector<CXButton*>::const_iterator place = find(vecButton.begin(),vecButton.end(),button);
		if(place!=vecButton.end())
		{
			vecButton.erase(place);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      引发一个快捷键事件，相当于按下对应的按钮。

@param	(i)UINT key 快捷键。

@retval     无

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
bool CShortCutKeyManager::FireShortCutKey(UINT key)
{
	map<UINT,vector<CXButton*>>::const_iterator place = container.find(key);
	if(place==container.end())
	{
		return false;
	}
	vector<CXButton*> vecButtons = container[key];
	for(vector<CXButton*>::iterator iterButton = vecButtons.begin();iterButton!=vecButtons.end();iterButton++)
	{
		CXButton* button = (*iterButton);
		if(IsWindow(button->m_hWnd) && IsWindowVisible(button->m_hWnd) && IsWindowEnabled(button->m_hWnd) )
		{
			button->SetFocus();
			return true;
		}
	}
	return false;
}