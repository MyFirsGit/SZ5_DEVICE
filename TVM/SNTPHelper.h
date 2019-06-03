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

	// ����,��������
	CSNTPHelper(void);
	~CSNTPHelper();
	CSNTPHelper(const CSNTPHelper&);
	CSNTPHelper& operator=(const CSNTPHelper&);

	static  CSNTPHelper theInstance;   // �����Ψһ�ӿ�
};
