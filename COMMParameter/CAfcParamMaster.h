#pragma once
#include "Xfile.h"
#include "ParameterDef.h"
#include "CParameterCommandExp.h"
#include "cparametermaster.h"
#include "dxfile.h"

/**
 @brief   AFC参数管理父类
*/
class  PARAMETERCOMMAND_DECLSPEC CAfcParamMaster: public CParameterMaster
{
public:
    // AFC参数包头结构体
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

    // 构造,析构函数
    CAfcParamMaster(void);
    virtual ~CAfcParamMaster(void);

    bool Internalize(bool readFlag,CString dataPath,CString backupPath);

    bool ReadMasterData(VARIABLE_DATA&,bool);                     // 读取Master文件数据
    bool UpdateMasterFile(const char*, ULONG);					  // 更新文件信息
	bool readVersionDataInfo(CString dataPath,CString backupPath);//仅仅读取版本日期信息
	DWORD GetCurMasterVer();                                    // 得到当前版本信息
    DWORD GetFutMasterVer();                                    // 得到未来版本信息
	_DATE GetCurStartDate();                                    // 得到当前生效日期信息
	_DATE GetFutStartDate();                                    // 得到未来生效日期信息
	PARAM_VERSION_TYPE_CODE GetCurVerType();					// 取得当前参数版本类型
	PARAM_VERSION_TYPE_CODE GetFutVerType();					// 取得未来参数版本类型
    bool CheckDataInfo(const VARIABLE_DATA&);				// 检查电文内容正确性
	void GetParamFileNameSub(const TCHAR*);			// 获取参数文件名

protected:
	enum{
		START_POSITION_LENS = 8,		   // 单个偏移量记录体长度
        PACKAGE_HEAD_LENS   = 28           // 参数文件头长度
    };
	typedef struct _section_info {
        DWORD startPosition;				// 分段起始偏移量
        DWORD recordCount;					// 分段结构体记录数
        _section_info(){
            startPosition = 0;
            recordCount = 0; 
        }
    }SECTION_INFO;

	ECT_PACKAGE_HEAD m_packageHead;			// 电文包头

private:
	virtual void ParseSectionInfo(LPBYTE)= 0;		 // 解析分段信息
    virtual void ParseBody(LPBYTE) = 0;	
    virtual bool CheckBodyInfo(LPBYTE) = 0;
	virtual void GetParamFileName() = 0;			 // 获取参数文件名
	
    void ParseHead(LPBYTE);            // 电文包头解析	
	void SetCurVerType(CString);					// 设置当前参数版本类型
	void SetFutVerType(CString);					// 设置未来参数版本类型

    static CString m_dataPath;         // 主文件路径
    static CString m_backupPath;       // 备份文件路径

	DWORD   m_curVersion;                           // current版本
    DWORD   m_futVersion;                           // future版本
	_DATE   m_curStartDate;							// current版本生效日期
	_DATE	m_futStartDate;							// future版本生效日期
	PARAM_VERSION_TYPE_CODE m_curVersionType;		// current版本类型
	PARAM_VERSION_TYPE_CODE m_futVersionType;		// future版本类型
    CDXFile  *m_curFile;                             // 指向current版本文件
    CDXFile  *m_futFile;                             // 指向future版本文件 
	bool    m_flag;                                 // 文件选择标志 true:当前版本文件   false:未来版本文件
	CString m_curfileName;							// 当前参数文件名
	CString m_futfileName;							// 未来参数文件名
};
