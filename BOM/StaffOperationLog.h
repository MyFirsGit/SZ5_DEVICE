#pragma once;

class CXButton;
class CStaffOperationLog : public CLog
{

public:
	~CStaffOperationLog();

private:
	// 构造,析构函数
	CStaffOperationLog(void);
	
	CStaffOperationLog(const CStaffOperationLog&);
	CStaffOperationLog& operator=(const CStaffOperationLog&);
	
	void OnButtonClicking(CXButton*,bool*);

	static  CStaffOperationLog theInstance;   // 该类的唯一接口
};