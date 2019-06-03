#pragma once
#include "CParameterCommandExp.h"
#include "dxfile.h"


#define thePWD_ERR    CPwdInputError::GetInstance()       ///< CPwdInputError的唯一接口

class PARAMETERCOMMAND_DECLSPEC   CPwdInputError
{
public:

	bool Internalize(CString, CString);                             // 初期化
	void AddPWDErrorStuffID(ULONG);                                 // 添加员工信息
	bool IsStuffIDExist(ULONG);                                     // 判断员工信息是否存在
	void ClearStuffInfo();                                          // 清除所有员工信息
	bool ReadFile();                                                // 读取文件

	void ClearTheStaffNoInfo(ULONG);                                // 账户解锁
	static CPwdInputError&  GetInstance()
	{
		return theInstance;
	}

private:

	static  CPwdInputError theInstance;        // 该类的唯一接口

	enum {
		FILE_BASE_SIZE = 5,					        // 最大履历记录数 4Byte(operatorID) + 1byte(error_Count)
		FILE_DATE_SIZE = 4,					        // 日期记录长度
	};

	typedef struct _staff_err_info
	{
		ULONG    stuffID;             // 员工ID
		BYTE     errCnt;              // 错误数
		_staff_err_info(){
			stuffID = 0;
			errCnt = 0;
		}
	} STAFF_ERR_INFO;

	vector<STAFF_ERR_INFO>   m_stuffErrList;        // 发生错误的员工信息

	CDXFile* m_StuffIDFile;                          // 密码输入错误的员工ID文件
	CString m_dataPath;                             // 数据主目录
	CString m_backupPath;                           // 备份主目录
	_DATE   m_curDate;                              // 错误发生日期

	void SetFileDate();								// 写入
	void WriteFile();                               // 写文件

	CPwdInputError(void);
	~CPwdInputError();
	CPwdInputError(const CPwdInputError&);
	CPwdInputError& operator=(const CPwdInputError&);
};
