#pragma once
#include "CParameterCommandExp.h"
#include "Xfile.h"
#include "cparametermaster.h"
/**
@brief   ECT参数管理父类
*/
class PARAMETERCOMMAND_DECLSPEC CEctParamMaster : public CParameterMaster
{
public:
	bool Internalize(bool readFlag = true,CString dataPath=_T(""),CString backupPath=_T(""));         			// 初期化

	bool ReadMasterData(VARIABLE_DATA&, bool);                   // 读取参数信息  
	bool UpdateMasterFile(const char *, ULONG, bool);           // 更新参数信息

	DWORD GetCurMasterVer();                                    // 得到当前版本信息
	DWORD GetFutMasterVer();                                    // 得到未来版本信息
	_DATE GetCurStartDate();                                    // 得到当前生效日期信息
	_DATE GetFutStartDate();                                    // 得到未来生效日期信息
	PARAM_VERSION_TYPE_CODE GetCurVerType();					// 取得当前参数版本类型
	PARAM_VERSION_TYPE_CODE GetFutVerType();					// 取得未来参数版本类型

	bool CheckDataInfo(const VARIABLE_DATA&);					// 检查电文内容正确性

protected:
	// ECT参数包头结构体
	typedef struct _ect_package_head {
		BYTE    formatVersion;			// 包格式版本号
		BYTE    dataOrigin;				// 数据来源方
		DWORD   packageLength;			// 数据包长度
		WORD	dataTypeCode;			// 数据类型代码	
		DWORD   versionCode;			// 版本号
		_DATE_TIME  createDateTime;		// 版本创建时间
		_DATE   startDate;				// 生效日期
		WORD    sectionCount;			// 数据分段总数
		BYTE    reverse[3];				// 预留字段

		void internalize(){ 
			formatVersion       = 0;
			dataOrigin          = 0;
			packageLength       = 0;
			dataTypeCode  = 0;
			versionCode     = 0;
			sectionCount  = 0;            
			memset(reverse,0,3);
		}
		// 结构体初期化
		_ect_package_head(){
			internalize();
		}

	}ECT_PACKAGE_HEAD;

	typedef struct _section_info {
		DWORD startPosition;				// 分段起始偏移量
		DWORD recordCount;					// 分段结构体记录数
		_section_info(){
			startPosition = 0;
			recordCount = 0; 
		}
	}SECTION_INFO;

	enum{		
		START_POSITION_LENS = 8,		   // 单个偏移量记录体长度
		PACKAGE_HEAD_LENS   = 28           // 参数文件头长度
	};	
	ECT_PACKAGE_HEAD m_packageHead;					// 电文包头

	// 构造，析构函数
	CEctParamMaster(void);
	~CEctParamMaster();
	CEctParamMaster(const CEctParamMaster&);
	CEctParamMaster& operator=(const CEctParamMaster&);

	void GetParamFileNameSub(CString);				// 获取参数文件名
private:
	// 成员私有变量
	DWORD   m_curVersion;                           // current版本
	DWORD   m_futVersion;                           // future版本
	_DATE   m_curStartDate;							// current版本生效日期
	_DATE	m_futStartDate;							// future版本生效日期
	PARAM_VERSION_TYPE_CODE m_curVersionType;		// current版本类型
	PARAM_VERSION_TYPE_CODE m_futVersionType;		// future版本类型
	CXFile  *m_curFile;                             // 指向current版本文件
	CXFile  *m_futFile;                             // 指向future版本文件 
	CString m_dataPath;								// 主文件路径
	CString m_backupPath;							// 备份文件路径
	bool    m_flag;                                 // 文件选择标志 true:当前版本文件   false:未来版本文件
	CString m_curfileName;							// 当前参数文件名
	CString m_futfileName;							// 未来参数文件名

	// 成员私有函数    
	bool ReadFile();                                 // 读取文件
	void ParseHead(LPBYTE);							 // 读出当前文件的包头部分

	virtual void ParseSectionInfo(LPBYTE)= 0;		 // 解析分段信息
	virtual bool CheckBodyInfo(LPBYTE) = 0;			 // 检查电文内容
	virtual void ParseBody(LPBYTE) = 0;              // 解析电文内容
	virtual void GetParamFileName() = 0;			 // 获取参数文件名

	void SetCurVerType(CString);					// 设置当前参数版本类型
	void SetFutVerType(CString);					// 设置未来参数版本类型

};
