#pragma once
#include "ACCAuditExp.h"
#include <map>
#include "Util.h"

using namespace std;

#define theACCAUDIT_MGR CACCAuditManager::GetInstance()

typedef pair<unsigned short,int> ACCAR_ITEM;
typedef map<unsigned short,int> ACCAR;

class ACCAUDIT_DECLSPEC CACCAuditManager
{
public:
	static CACCAuditManager& GetInstance();
	void Initialize(CString dataPath=_T(""),CString backupPath=_T(""));
	void AddAR(ACCAR& accAR);
	ACCAR& GetCurrentAR();
private:

	CACCAuditManager();
	~CACCAuditManager();
	void LoadACCARFromFile(ACCAR&);
	void SaveACCARToFile(ACCAR&);
	void SerializeACCAR(ACCAR&,VARIABLE_DATA&);
	void DeserializeACCAR(ACCAR&,VARIABLE_DATA&);
	void InitARItems();
	static CACCAuditManager instance;
	ACCAR m_ACCAR;
	CString m_dataPath;
	CString m_BackupPath;
};