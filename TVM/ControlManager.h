#pragma once
#include <list>

#define  theControl_MGR CControlManager::GetInstance()

class CControlManager
{
private:
	static CControlManager instance;
	CControlManager();
	std::list<CWnd*> m_controls;
public:
	virtual ~CControlManager();
	static CControlManager& GetInstance();
	CWnd* GetNextCanFocusControl(CWnd*);
	CWnd* GetPreCanFocusControl(CWnd*);
	std::list<CWnd*> GetControls();
	void RegisterControl(CWnd*);
	void UnRegisterControl(CWnd*);

};