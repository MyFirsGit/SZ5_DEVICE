#pragma once;

class CXButton;
class CStaffOperationLog : public CLog
{

public:
	~CStaffOperationLog();

private:
	// ����,��������
	CStaffOperationLog(void);
	
	CStaffOperationLog(const CStaffOperationLog&);
	CStaffOperationLog& operator=(const CStaffOperationLog&);
	
	void OnButtonClicking(CXButton*,bool*);

	static  CStaffOperationLog theInstance;   // �����Ψһ�ӿ�
};