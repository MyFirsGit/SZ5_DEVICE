#pragma once
#include <vector>
#include "Util.h"

using namespace std;

/**
@brief  ��־�ļ�������
*/

class CLogHelper : public CServiceHelper
{
public:
	CLogHelper(CService& service);                                                 // ���캯��
	~CLogHelper();                                                                 // ��������
	static long PrepareUploadLogFile(CString& fileName,vector<_DATE> logDates);
	static bool UploadLogFile(_DATE dateOfLog);
};