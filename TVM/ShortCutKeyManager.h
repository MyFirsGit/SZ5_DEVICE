#pragma once
#include <map>
#include <vector>

#define  theShortCutKey_MGR  CShortCutKeyManager::GetInstance()

using namespace std;

class CXButton;
//////////////////////////////////////////////////////////////////////////
class CShortCutKeyManager
{
private:
	map<UINT,vector<CXButton*>> container;
	static CShortCutKeyManager instance;
	CShortCutKeyManager();
public:
	~CShortCutKeyManager();
	void RegisterShortCutKey(UINT,CXButton*);
	void UnRegisterShortCutKey(CXButton*);
	bool FireShortCutKey(UINT);
	static CShortCutKeyManager& GetInstance();
};