#include "stdafx.h"
#include "ShortCutKeyManager.h"
#include "XButton.h"
#include <algorithm>
#include "MessageIDDefinition.h"

//////////////////////////////////////////////////////////////////////////
//
// ��ݼ�������Ψһʵ��
//////////////////////////////////////////////////////////////////////////
CShortCutKeyManager CShortCutKeyManager::instance;

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ݼ������๹�캯��

@param     ��

@retval     ��

@exception  �� 
*/
//////////////////////////////////////////////////////////////////////////
CShortCutKeyManager::CShortCutKeyManager()
{

}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ݼ���������������
					������Ŀ�ݼ�����ť��Ӧ��ϵɾ����

@param     ��

@retval     ��

@exception  �� 
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
@brief      ȡ�ÿ�ݼ�������Ψһ����

@param     ��

@retval     CShortCutKeyManager& ��ݼ�����

@exception  �� 
*/
//////////////////////////////////////////////////////////////////////////
CShortCutKeyManager& CShortCutKeyManager::GetInstance()
{
	return instance;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ע��һ����ݼ�����һ����ݼ�ͬһ����ť����������

@param	(i)UINT key					��ݼ�
@param	(i)CXButton * button ��ť

@retval     ��

@exception  �� 
*/
//////////////////////////////////////////////////////////////////////////
void CShortCutKeyManager::RegisterShortCutKey(UINT key, CXButton * button)
{
	UnRegisterShortCutKey(button);
	container[key].push_back(button);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��һ���Ѿ�ע��Ŀ�ݼ���ť��


@param	(i)CXButton * button ��ť

@retval     ��

@exception  �� 
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
@brief      ����һ����ݼ��¼����൱�ڰ��¶�Ӧ�İ�ť��

@param	(i)UINT key ��ݼ���

@retval     ��

@exception  �� 
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