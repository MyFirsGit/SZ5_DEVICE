#pragma once

#define  theSNTP_HELPER CSNTPHelper::GetInstance()

class CSNTPHelper
{
public:
	static CSNTPHelper&  GetInstance(){
		return theInstance;
	}
	
	void ForceTimeSynchronize();
	void TimeSynchronize(bool isInit = false);

private:

	// 构造,析构函数
	CSNTPHelper(void);
	~CSNTPHelper();
	CSNTPHelper(const CSNTPHelper&);
	CSNTPHelper& operator=(const CSNTPHelper&);

	static  CSNTPHelper theInstance;   // 该类的唯一接口
};
