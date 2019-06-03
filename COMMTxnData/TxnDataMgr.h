#pragma once
#include "TXNDATAExp.h"
#include "InnerException.h"
#include "TxnDataException.h"
#include "TxnDataStruct.h"
#include "TxnDataEnum.h"
#include "TxnDataControl.h"
#include "SerialNumberMgr.h"
#include "DebugLog.h"
#include "delegate.h"

#define theACC_TXN_MGR CTxnDataMgr::GetAccTxnDataMgr() 
#define theAFC_TXN_MGR CTxnDataMgr::GetAfcTxnDataMgr() 
#define theBUS_TXN_MGR CTxnDataMgr::GetBusTxnDataMgr()      
#define theECT_TXN_MGR CTxnDataMgr::GetEctTxnDataMgr()
#define theEVN_TXN_MGR CTxnDataMgr::GetEvnTxnDataMgr()      

/**
@brief      交易数据管理父类
*/
class TXNDATA_API CTxnDataMgr
{
public:  

protected:
	CTxnDataMgr();                                                    // 构造函数
	CTxnDataMgr(CString className,TXN_FILE_NAMES txnFileNames,DATA_TYPE_CODE txnCode,DATA_PEKG_KIND pekgKind);                                                    // 构造函数
	virtual ~CTxnDataMgr();                                           // 析构函数
//	CFileLogTrace *DEBUG_LOG;

public:
	// 公共接口函数
	void Internalize(CString strDataPath,CString strBackupPath);
	virtual int		     InitTxnData(bool IsValidateUnSendPackage  = false,int iSendedDataMaxDay = 10,int iUnSendDataMaxDay = 7); // 初始化交易数据	
	delegate<void(WORD,LPBYTE)> TxnDataSaved;					 // 保存交易数据后的回调函数 （保存交易审核数据）
	virtual void		 SaveTxnData(WORD,LPBYTE);				 // 保存交易数据
	virtual unsigned long HasUnsendData();						 // 是否存在未送数据
	virtual bool		 GetDataPekgById(PEKG_DATA&);			 // 获取指定数据包
	virtual bool		 GetDataPekg(PEKG_DATA&);				 // 获取队列中的数据包
	virtual void		 SetPekgSended(CString);				 // 设置数据包为已送
	virtual void		 MakePekg(bool isInit = false);							 // 组包
	void ProtectUnsendPackage();
	static CTxnDataMgr& GetAccTxnDataMgr()
	{
		return accTxnDataMgr;
	};
	static CTxnDataMgr& GetEctTxnDataMgr()
	{
		return ectTxnDataMgr;
	};
	static CTxnDataMgr& GetAfcTxnDataMgr()
	{
		return afcTxnDataMgr;
	};
	static CTxnDataMgr& GetBusTxnDataMgr()
	{
		return busTxnDataMgr;
	};
	static CTxnDataMgr& GetEvnTxnDataMgr()
	{
		return evnTxnDataMgr;
	};
private:
	virtual CString UpdateCurrentPekgID();						// 更新当前数据包ID

	virtual bool HasTooMoreUnSendData();								// 判断未送数据是否过多
	void DeleteTooMoreSendedData(vector<CString>,unsigned int);
	void EncryptUnsendPackage(_DATE_TIME& currentTime,VARIABLE_DATA& data);
	
	void ValidateUnsendPackage();
	// 成员变量定义
	queue<CString>	 m_queUnSendPekgName;						// 未送数据包文件名
	TXN_PEKG_DATA	 m_currentPekg;								// 当前数据包
	
	CString  m_dataPath;												// BOM主数据目录
	CString  m_BackupPath;											// BOM备份数据目录
	int m_iUnsendMaxDay;												// 未送数据最大保存天数
	int m_iSendedDataMaxDay;										// 已送数据最大保存天数
	CString className;
	TXN_FILE_NAMES txnFileNames;
	DATA_TYPE_CODE txnCode;
	DATA_PEKG_KIND txnKind;

	static CTxnDataMgr accTxnDataMgr;
	static CTxnDataMgr afcTxnDataMgr;
	static CTxnDataMgr busTxnDataMgr;
	static CTxnDataMgr ectTxnDataMgr;
	static CTxnDataMgr evnTxnDataMgr;

};







