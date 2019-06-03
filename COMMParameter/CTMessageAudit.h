#pragma once
#include "CParameterCommandExp.h"
#include "Xfile.h"
#include <afxmt.h>
#include "dxfile.h"


#define thePEKG_AUDIT    CTMessageAudit::GetInstance()       // CTMessageAudit的唯一接口

/**
@brief 数据包审计Master
*/
class PARAMETERCOMMAND_DECLSPEC  CTMessageAudit
{
	
	public:

		// 包数据结构体
		typedef struct _Pekg_info{
			BYTE sPekgId[14];   
			WORD iRecordCnt;
			_Pekg_info()
			{
				memset(sPekgId,0x00,14);
				iRecordCnt = 0;
			}   
		} PEKG_INFO;

		bool GetPekgAuditMessage(VARIABLE_DATA& binfileData);		// 获取包传输审计电文
		bool Internalize(CString dataPath, CString backupPath);     // 初期化
		void AddWriteFile(PEKG_INFO);								// 更新包审计文件
		void SetStartDateTime(_DATE_TIME);							// 设置审计开始时间
		void SetEndDateTime(_DATE_TIME);							// 设置审计结束时间
		void ClearAuditData();										// 清空审计文件数据
		bool HasStartDateTime();									// 是否设置开始时间
		bool HasEndDateTime();										// 是否设置开始时间

		static CTMessageAudit &GetInstance()
		{
			return theInstance;
		}

	private:

		enum {
			MIN_FILE_LENGTH = 18,						// 数据包审计文件最小长度
		};

		static  CTMessageAudit theInstance;    // 该类的唯一接口

		// 私有成员变量
		CCriticalSection m_criticalSection;                             // 关键区控制
		CDXFile  *m_pekgAuditFile;        // 包审计文件
		vector<PEKG_INFO>  m_pekgList;   // 数据包信息
		_DATE_TIME   m_StartDateTime;    // 审计开始时间
		_DATE_TIME   m_EndDateTime;      // 审计结束时间
		DWORD m_PekgCnt;				 // 数据包数量

		CTMessageAudit(void);
		~CTMessageAudit();
		CTMessageAudit(const CTMessageAudit&);
		CTMessageAudit& operator=(const CTMessageAudit&);
	};

