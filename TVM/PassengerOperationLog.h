#pragma once;

class CXButton;
class CPassengerOperationLog : public CLog
{

public:
	~CPassengerOperationLog();
	 static void ReleasePassengerOperationLog();
private:
	// ����,��������
	CPassengerOperationLog(void);
	
	CPassengerOperationLog(const CPassengerOperationLog&);
	CPassengerOperationLog& operator=(const CPassengerOperationLog&);
	
	void OnButtonClicking(CXButton*,bool*);

	static  CPassengerOperationLog theInstance;   // �����Ψһ�ӿ�
};