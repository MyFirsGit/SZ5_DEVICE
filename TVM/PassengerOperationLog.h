#pragma once;

class CXButton;
class CPassengerOperationLog : public CLog
{

public:
	~CPassengerOperationLog();
	 static void ReleasePassengerOperationLog();
private:
	// 构造,析构函数
	CPassengerOperationLog(void);
	
	CPassengerOperationLog(const CPassengerOperationLog&);
	CPassengerOperationLog& operator=(const CPassengerOperationLog&);
	
	void OnButtonClicking(CXButton*,bool*);

	static  CPassengerOperationLog theInstance;   // 该类的唯一接口
};