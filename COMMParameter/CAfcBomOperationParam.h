#pragma once
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcBomOperationParam的唯一接口
#define theAFC_BOM_OPERATION CAfcBomOperationParam::GetInstance()        

/**
 @brief   BOM运营参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAfcBomOperationParam :public CAfcParamMaster
{
public: 

	// BOM票卡读写参数
	 typedef struct _tpu_param {
		WORD maxWriteErrorCnt;	//车票连续出错最大张数	2	BIN	发票时车票连续读写错误的最大次数。默认为3张，0表示参数无效。达到该次数后，BOM停止出票。
		WORD unwriteWaitTime;	//未写入完全的卡等待时间	2	BIN	写卡不完整后，提示重新刷卡的最大等待时间。单位：秒。默认值为5秒
		WORD unwriteRetryTimes;	//未写入完全的卡重试次数	2	BIN	当BOM写卡不完整后，重新写卡的次数。
        _tpu_param(){
            maxWriteErrorCnt = 0;
            unwriteWaitTime = 0;
			unwriteRetryTimes = 0;
        }
    }TPU_PARAM;	

	// BOM维修登录相关参数
    typedef struct _maintain_param{ 
		WORD pwdInputTime;			//密码输入时间	2	BIN	单位秒，默认为60秒。密码输入操作的时限，超过该时间未输入密码，自动进入开机界面。
		WORD loginRetryTimes;		//登录尝试次数	2	BIN	操作员登录设备次数限制，默认为3次。
		WORD autoLogoffTime;		//无操作自动登出时间	2	BIN	单位秒，默认为120秒，0表示该值无效。
		WORD doorUnlockAlarmTime;	//维修门未关闭报警时间	2	BIN	单位秒，默认为60秒。无则不用
        _maintain_param(){
            pwdInputTime = 0;
            loginRetryTimes   = 0;
            autoLogoffTime = 0;
            doorUnlockAlarmTime = 0;
        }   
    } MAINTAIN_PARAM;

	// BOM票箱参数
    typedef struct _magazine_param{ 
		WORD willEmptyValue;				//票箱将空张数		2	BIN	票箱将空张数阀值
		WORD haveEmptyValue;				//票箱已空张数		2	BIN	票箱已空张数阀值
		WORD willFullValue;					//票箱将满张数		2	BIN	票箱将满张数阀值
		WORD haveFullValue;					//票箱已满张数		2	BIN	票箱已满张数阀值
		WORD willInvalidFullValue;			//废票箱将满张数	2	BIN	废票箱将满张数阀值
		WORD haveInvalidFullValue;			//废票箱已满张数	2	BIN	废票箱已满张数阀值
        _magazine_param(){
			willEmptyValue = 0;				 
			haveEmptyValue = 0;					 
			willFullValue = 0;				 
			haveFullValue = 0;					 
			willInvalidFullValue = 0;		 
			haveInvalidFullValue = 0;		 
        }   
    } MAGAZINE_PARAM;

	// BOM最近可查最小交易限制
	 typedef struct _txn_lookup_param{ 
		BYTE minCount;				//可查询交易数量下限	1	BIN	可查最小交易数量。默认30条。
		BYTE minTime;				//可查询交易时间下限	1	BIN	单位：分钟。默认10分钟
        _txn_lookup_param(){
			minCount = 0;			 
			minTime = 0;		 
        }   
    } TXN_LOOKUP_PARAM;

	// 共有接口
    void GetTpuParam(TPU_PARAM&);						// 获得BOM票卡读写参数
	void GetMaintainParam(MAINTAIN_PARAM&);				// 获得BOM维修登录相关参数
	void GetMagazineParam(MAGAZINE_PARAM&);				// 获得BOM票箱参数
	void GetTxnLookupParam(TXN_LOOKUP_PARAM&);			// 获得BOM最近可查最小交易限制
	WORD GetAutoLogoffTime();							// 获得自动登出等待时间参数
	
    static CAfcBomOperationParam&  GetInstance()
    {
        return theInstance;
    }

private:

	SECTION_INFO m_section[4];	//分段信息

    // 私有成员变量
    TPU_PARAM			m_tpu_param;			// BOM票卡读写参数
	MAINTAIN_PARAM		m_maintain_param;		// BOM维修登录相关参数
	MAGAZINE_PARAM		m_magazine_param;		// BOM票箱参数
	TXN_LOOKUP_PARAM	m_txn_lookup_param;		// BOM最近可查最小交易限制

    // 私有成员函数
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAfcBomOperationParam(void);
    ~CAfcBomOperationParam();
	CAfcBomOperationParam(const CAfcBomOperationParam&);
	CAfcBomOperationParam& operator=(const CAfcBomOperationParam&);

    static  CAfcBomOperationParam theInstance;
};
