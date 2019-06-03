#include "stdafx.h"
#include "ControlManager.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CControlManager CControlManager::instance;


//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��Ψһʵ����ں���

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CControlManager& CControlManager::GetInstance()
{
	return instance;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CControlManager::CControlManager()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CControlManager::~CControlManager()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �õ�����ע��ؼ�

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
std::list<CWnd*> CControlManager::GetControls()
{
	return m_controls;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ע��һ���ؼ�

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CControlManager::RegisterControl(CWnd* control)
{
	std::list<CWnd*>::iterator place = find(m_controls.begin(),m_controls.end(),control);
	if(place==m_controls.end())
	{
		m_controls.push_back(control);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ע��һ���ؼ�

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CControlManager::UnRegisterControl(CWnd* control)
{
	m_controls.remove(control);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ȡ����һ�����û�ý���Ŀؼ� 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CWnd* CControlManager::GetNextCanFocusControl(CWnd* current)
{
	std::list<CWnd*>::iterator place = find(m_controls.begin(),m_controls.end(),current);
	std::list<CWnd*>::iterator start=m_controls.begin();
	if(place != m_controls.end())
	{
		start = ++place;
	}
	for (std::list<CWnd*>::iterator iterControl = start;iterControl!=m_controls.end();iterControl++)
	{
		CWnd* control = *iterControl;
		if(control!=NULL && ::IsWindow(control->m_hWnd) && ::IsWindowVisible(control->m_hWnd) && ::IsWindowEnabled(control->m_hWnd))
		{
			return control;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �õ���һ�����Ի�ý���Ŀؼ�

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CWnd* CControlManager::GetPreCanFocusControl(CWnd* current)
{
	std::list<CWnd*>::reverse_iterator place = find(m_controls.rbegin(),m_controls.rend(),current);
	std::list<CWnd*>::reverse_iterator start=m_controls.rbegin();
	if(place != m_controls.rend())
	{
		start = ++place;
	}
	for (std::list<CWnd*>::reverse_iterator iterControl = start;iterControl!=m_controls.rend();iterControl++)
	{
		CWnd* control = *iterControl;
		if(control!=NULL && ::IsWindow(control->m_hWnd) && ::IsWindowVisible(control->m_hWnd) && ::IsWindowEnabled(control->m_hWnd))
		{
			return control;
		}
	}
	return NULL;
}
