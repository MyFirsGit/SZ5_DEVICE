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
@brief      取得唯一实例入口函数

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
@brief      构造函数

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
@brief      析构函数

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
@brief      得到所有注册控件

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
@brief     注册一个控件

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
@brief      注销一个控件

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
@brief     取得下一个可用获得焦点的控件 

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
@brief      得到上一个可以获得焦点的控件

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
