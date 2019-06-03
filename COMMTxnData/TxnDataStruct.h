#pragma once

#include <vector>
#include <TxnDataEnum.h>

using namespace std;

typedef struct _pekg_data{
	CString strPekgID;	// ���ݰ�ID
    LPBYTE lpData;      // ���ݰ�����
    ULONG  nLen;		// ���ݰ�����
    _pekg_data(){
		strPekgID = "";
        lpData = NULL;
        nLen = 0;
    }
    ~_pekg_data(){
        if (lpData != NULL) {
            delete []lpData;
            lpData = NULL;
        }
    }
	_pekg_data& _pekg_data::operator=(const _pekg_data& PekgData){
		strPekgID = PekgData.strPekgID;
		nLen = PekgData.nLen;
		lpData = NULL;
		if (PekgData.nLen > 0 && PekgData.lpData != NULL) {
			lpData = new BYTE[nLen];
			memset(lpData, 0, nLen);
			memcpy(lpData, PekgData.lpData, nLen);
		}
		return *this;
	}
}PEKG_DATA;

// �������ݽṹ�嶨��
typedef struct _txn_message_data {
	DWORD staff_ID;				// ����ԱID	3	BCD
	_DATE dtHappen_Date;		// ��������	4	BCD
	WORD  message_Len;			// ��Ϣ����	2	BIN	
	LPBYTE pbMessage_Content;	// ��������
	_txn_message_data(){
		staff_ID = 0;
		dtHappen_Date.wYear = 0;
        dtHappen_Date.biMonth = 0;
        dtHappen_Date.biDay = 0;
		message_Len = 0;		
		pbMessage_Content = NULL;
	}
	~_txn_message_data(){
		staff_ID = 0;
		dtHappen_Date.wYear = 0;
        dtHappen_Date.biMonth = 0;
        dtHappen_Date.biDay = 0;
		message_Len = 0;		
		if (pbMessage_Content != NULL) {
			delete [] pbMessage_Content;
			pbMessage_Content = NULL;
		}
	}
	_txn_message_data(const _txn_message_data& MessageData){
		staff_ID		= MessageData.staff_ID;
		dtHappen_Date	= MessageData.dtHappen_Date;
		message_Len     = MessageData.message_Len;
		pbMessage_Content = NULL;
		if (MessageData.message_Len > 0 && MessageData.pbMessage_Content != NULL) {
			pbMessage_Content = new BYTE[message_Len];
			memset(pbMessage_Content, 0, message_Len);
			memcpy(pbMessage_Content, MessageData.pbMessage_Content, message_Len);
		}
	}
	_txn_message_data& _txn_message_data::operator=(const _txn_message_data& MessageData){
		staff_ID		= MessageData.staff_ID;
		dtHappen_Date	= MessageData.dtHappen_Date;
		message_Len		= MessageData.message_Len;
		pbMessage_Content		= NULL;
		if (MessageData.message_Len > 0 && MessageData.pbMessage_Content != NULL) {
			pbMessage_Content = new BYTE[message_Len];
			memset(pbMessage_Content, 0, message_Len);
			memcpy(pbMessage_Content, MessageData.pbMessage_Content, message_Len);
		}
		return *this;
	}
	void _txn_message_data::clear(){
		staff_ID = 0;
		dtHappen_Date.wYear = 0;
        dtHappen_Date.biMonth = 0;
        dtHappen_Date.biDay = 0;
		message_Len = 0;	
		if (pbMessage_Content != NULL) {
			delete [] pbMessage_Content;
			pbMessage_Content = NULL;
		}
	}
}TXN_MESSAGE_DATA; // ACC ,ECT ,AFC �������ݽṹ��

// ҵ������ & �������� ����ʽ����
typedef struct _txn_pekg_data {
	BYTE	pekgID[14];					// ���ݰ�ID		14	1BIN+1BIN+2BCD+1BIN+7BCD+2BIN	���ݰ�����+�豸ID+ʱ���+������кţ�INTEL��
	WORD	pekg_Len;					// ���ݰ�����	4	BIN	 �Ӱ���ʽ�汾�ſ�ʼ����������
	WORD	record_cnt;					// ���м�¼��	2	BIN	���ݼ�¼�����������Чֵ��Χ1~~10000 ��INTEL��
	vector<BYTE>  vecPekg_Content;		// ��Ϣ����		
	_txn_pekg_data(){		
		memset(pekgID,0,14);
		pekg_Len = 0;	
		record_cnt = 0;
		vecPekg_Content.clear();
	}
	~_txn_pekg_data(){		
		memset(pekgID,0,14);
		pekg_Len = 0;	
		record_cnt = 0;
		vecPekg_Content.clear();
	}
	_txn_pekg_data(const _txn_pekg_data& PekgData){
		memcpy(pekgID,PekgData.pekgID,14);
		pekg_Len	= PekgData.pekg_Len;
		record_cnt  = PekgData.record_cnt;
		vecPekg_Content = PekgData.vecPekg_Content;
	}
	_txn_pekg_data& _txn_pekg_data::operator=(const _txn_pekg_data& PekgData){
		memcpy(pekgID,PekgData.pekgID,14);
		pekg_Len	= PekgData.pekg_Len;
		record_cnt  = PekgData.record_cnt;
		vecPekg_Content = PekgData.vecPekg_Content;
		return *this;
	}
	void _txn_pekg_data::clear(){
		memset(pekgID,0,14);
		pekg_Len = PEKG_ID_LEN;	
		record_cnt = 0;
		vecPekg_Content.clear();
	}
}TXN_PEKG_DATA;

typedef struct _txn_file_names{
	CString txnFolderPath;
	CString sendedFolderPath;
	CString currentFileName;
	CString unsendedFolderPath;
	_txn_file_names(){
		sendedFolderPath = "";
		currentFileName = "";
		unsendedFolderPath = "";
	}
	_txn_file_names(CString txnFolderPath,CString sendedFolderPath,CString currentFileName,CString unsendedFolderPath){
		this->txnFolderPath = txnFolderPath;
		this->sendedFolderPath = sendedFolderPath;
		this->currentFileName = currentFileName;
		this->unsendedFolderPath = unsendedFolderPath;
	}
} TXN_FILE_NAMES;
