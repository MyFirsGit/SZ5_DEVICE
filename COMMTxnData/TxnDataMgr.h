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
@brief      �������ݹ�����
*/
class TXNDATA_API CTxnDataMgr
{
public:  

protected:
	CTxnDataMgr();                                                    // ���캯��
	CTxnDataMgr(CString className,TXN_FILE_NAMES txnFileNames,DATA_TYPE_CODE txnCode,DATA_PEKG_KIND pekgKind);                                                    // ���캯��
	virtual ~CTxnDataMgr();                                           // ��������
//	CFileLogTrace *DEBUG_LOG;

public:
	// �����ӿں���
	void Internalize(CString strDataPath,CString strBackupPath);
	virtual int		     InitTxnData(bool IsValidateUnSendPackage  = false,int iSendedDataMaxDay = 10,int iUnSendDataMaxDay = 7); // ��ʼ����������	
	delegate<void(WORD,LPBYTE)> TxnDataSaved;					 // ���潻�����ݺ�Ļص����� �����潻��������ݣ�
	virtual void		 SaveTxnData(WORD,LPBYTE);				 // ���潻������
	virtual unsigned long HasUnsendData();						 // �Ƿ����δ������
	virtual bool		 GetDataPekgById(PEKG_DATA&);			 // ��ȡָ�����ݰ�
	virtual bool		 GetDataPekg(PEKG_DATA&);				 // ��ȡ�����е����ݰ�
	virtual void		 SetPekgSended(CString);				 // �������ݰ�Ϊ����
	virtual void		 MakePekg(bool isInit = false);							 // ���
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
	virtual CString UpdateCurrentPekgID();						// ���µ�ǰ���ݰ�ID

	virtual bool HasTooMoreUnSendData();								// �ж�δ�������Ƿ����
	void DeleteTooMoreSendedData(vector<CString>,unsigned int);
	void EncryptUnsendPackage(_DATE_TIME& currentTime,VARIABLE_DATA& data);
	
	void ValidateUnsendPackage();
	// ��Ա��������
	queue<CString>	 m_queUnSendPekgName;						// δ�����ݰ��ļ���
	TXN_PEKG_DATA	 m_currentPekg;								// ��ǰ���ݰ�
	
	CString  m_dataPath;												// BOM������Ŀ¼
	CString  m_BackupPath;											// BOM��������Ŀ¼
	int m_iUnsendMaxDay;												// δ��������󱣴�����
	int m_iSendedDataMaxDay;										// ����������󱣴�����
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







