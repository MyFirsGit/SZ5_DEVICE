#pragma once
#include <vector>
#include "Util.h"

using namespace std;

/**
@brief  日志文件操作类
*/

class CLogHelper : public CServiceHelper
{
public:
	CLogHelper(CService& service);                                                 // 构造函数
	~CLogHelper();                                                                 // 析构函数
	static long PrepareUploadLogFile(CString& fileName,vector<_DATE> logDates);
	static bool UploadLogFile(_DATE dateOfLog);
};