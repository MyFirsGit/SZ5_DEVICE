#pragma once
#include <afx.h>
#include <vector>
#include <atlstr.h>
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcTVMOperationParam��Ψһ�ӿ�
#define theAFC_TVM_OPERATION CAfcTVMOperationParam::GetInstance()        

/**
 @brief   TVM��Ӫ������
*/

class CAfcTVMOperationParam :public CAfcParamMaster
{
public: 

	// TVMƱ����д����
	typedef struct _tpu_param {
		WORD maxWriteErrorCnt;		// ��Ʊ���������������	2	BIN	��Ԥ������Ʊʱ��Ʊ������д�������������Ĭ��Ϊ3�ţ�0��ʾ������Ч���ﵽ�ô�����BOMֹͣ��Ʊ��
		WORD unwriteWaitTime;		// δд����ȫ�Ŀ��ȴ�ʱ��	2	BIN	��Ԥ����д������������ʾ����ˢ�������ȴ�ʱ�䡣��λ���롣Ĭ��ֵΪ5��
		WORD unwriteRetryTimes;		// δд����ȫ�Ŀ����Դ���	2	BIN	��Ԥ������BOMд��������������д���Ĵ�����
		_tpu_param(){
			maxWriteErrorCnt = 0;
			unwriteWaitTime = 0;
			unwriteRetryTimes = 0;
		}
	}TPU_PARAM;	

	// TVMά�޵�¼��ز���
	typedef struct _maintain_param{ 
		WORD pwdInputTime;			//��������ʱ��			2	BIN	��Ԥ������λ�룬Ĭ��Ϊ60�롣�������������ʱ�ޣ�������ʱ��δ�������룬�Զ����뿪�����档
		WORD loginRetryTimes;		//��¼���Դ���			2	BIN	��Ԥ��������Ա��¼�豸�������ƣ�Ĭ��Ϊ3�Ρ�
		WORD autoLogoffTime;		//�޲����Զ��ǳ�ʱ��	2	BIN	��Ԥ������λ�룬Ĭ��Ϊ120�룬0��ʾ��ֵ��Ч��
		WORD doorUnlockAlarmTime;	//ά����δ�رձ���ʱ��	2	BIN	��Ԥ������λ�룬Ĭ��Ϊ60�롣������

		_maintain_param(){
			pwdInputTime = 0;
			loginRetryTimes   = 0;
			autoLogoffTime = 0;
			doorUnlockAlarmTime = 0;
		}   
	} MAINTAIN_PARAM;

	typedef enum invalid_value {
		BANKNOTE_INVALID_VALUE = 255,
		COIN_INVALID_VALUE = 0
	} INVALUE_VALUE;

	typedef struct _billbox_param{ 
		BYTE acceptSalesTicketBillValue1;			//Ticket��Ʊ���յ�ֽ�����1	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptSalesTicketBillValue2;			//Ticket��Ʊ���յ�ֽ�����2	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptSalesTicketBillValue3;			//Ticket��Ʊ���յ�ֽ�����3	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptSalesTicketBillValue4;			//Ticket��Ʊ���յ�ֽ�����4	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptSalesTicketBillValue5;			//Ticket��Ʊ���յ�ֽ�����5	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptSalesTicketBillValue6;			//Ticket��Ʊ���յ�ֽ�����6	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptSalesTicketBillValue7;			//Ticket��Ʊ���յ�ֽ�����7	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptSalesTicketBillValue8;			//Ticket��Ʊ���յ�ֽ�����8	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptSalesTicketBillValue9;			//Ticket��Ʊ���յ�ֽ�����9	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptSalesTicketBillValue10;		    //Ticket��Ʊ���յ�ֽ�����10	1	BIN	��λ��Ԫ����ЧʱΪ255
		WORD acceptSalesTicketMaxBillCnt;			//Ticket��Ʊ���յ����ֽ������	2	BIN	����������ʱת����INTEL��
		WORD acceptSalesTicketMaxBillAmt;			//Ticket��Ʊ���յ����ֽ���ܶ�	2	BIN��	��λ��Ԫ������ʱת����INTEL��

		BYTE acceptChrgrBillValue1;			//��ֵ���յ�ֽ�����1	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptChrgrBillValue2;			//��ֵ���յ�ֽ�����2	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptChrgrBillValue3;			//��ֵ���յ�ֽ�����3	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptChrgrBillValue4;			//��ֵ���յ�ֽ�����4	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptChrgrBillValue5;			//��ֵ���յ�ֽ�����5	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptChrgrBillValue6;			//��ֵ���յ�ֽ�����6	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptChrgrBillValue7;			//��ֵ���յ�ֽ�����7	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptChrgrBillValue8;			//��ֵ���յ�ֽ�����8	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptChrgrBillValue9;			//��ֵ���յ�ֽ�����9	1	BIN	��λ��Ԫ����ЧʱΪ255
		BYTE acceptChrgrBillValue10;		//��ֵ���յ�ֽ�����10	1	BIN	��λ��Ԫ����ЧʱΪ255
		WORD acceptChrgrMaxBillCnt;			//��ֵ���յ����ֽ������	2	BIN	����������ʱת����INTEL��
		WORD acceptChrgrMaxBillAmt;			//��ֵ���յ����ֽ���ܶ�	2	BIN��	��λ��Ԫ������ʱת����INTEL��

		BYTE salesNoCoin;			        //��Ӳ������ʱ�Ƿ�������Ʊ	1	BIN	0x00: ������0x01������
		BYTE acceptCoinMaxCnt;			    //����Ӳ�ҵ��������	1	BIN	
		BYTE chargeCoinMaxCnt;			    //Ӳ��������������	1	BIN	
		BYTE chargeBill;			        //��Ʊ�Ƿ�����ֽ������	1	BIN	0x00: ������0x01������
		BYTE chargeBillMaxCnt;   			//ֽ���������ֽ������	1	BIN	
		BYTE chargeBillMaxAmt;	    		//ֽ���������ֽ���ܽ��	1	BIN	��λ��Ԫ
		BYTE IsEnableAssistanceBtn;			//��Ԯ��ť���ñ�ʶ		1	HEX 0�������ã�1������
		WORD cancelDealTime;		     	//����ȡ��ʱ��	2	BIN	��λ���롣����ʱת����INTEL��
		BYTE showWaitScreen;			    //�Ƿ�������ʾ��������	1	BIN	
		WORD changeWaitScreenTime;			//���������л�ʱ��	2	BIN	��λ���롣����ʱת����INTEL��
		WORD willFullBillCnt;		     	//ֽ��Ǯ�佫������	2	BIN	����ʱת����INTEL��
		WORD alreadyFullBillCnt;			//ֽ��Ǯ����������	2	BIN	����ʱת����INTEL��
		WORD coinCollectionWillFullCoinCnt;	//Ӳ�һ�����A��������	2	BIN	����ʱת����INTEL��
		WORD coinCollectionAlreadyFullCoinCnt;//Ӳ�һ�����A��������	2	BIN	����ʱת����INTEL��
		WORD coinChangeWillEmptyCoinCnt;	//Ӳ��������A��������	2	BIN	����ʱת����INTEL��
		WORD minTicketCnt;			        //TVM���ٳ�Ʊ����	2	BIN	���ڸ�����ʱ��TVM��������Ʊ������ʱת����INTEL��
		WORD willEmptyBillBoxCnt;			//ֽ�������佫������	2	BIN	����ʱת����INTEL�� 

		BYTE acceptSalesCoinValue1;			//��Ʊ���յ�Ӳ�����1	1	BIN	0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP
		BYTE acceptSalesCoinValue2;			//��Ʊ���յ�Ӳ�����2	1	BIN	0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP
		BYTE acceptSalesCoinValue3;			//��Ʊ���յ�Ӳ�����3	1	BIN	0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP
		BYTE acceptSalesCoinValue4;			//��Ʊ���յ�Ӳ�����4	1	BIN	0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP

		BYTE acceptChrgrCoinValue1;			//��ֵ���յ�Ӳ�����1	1	BIN	0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP
		BYTE acceptChrgrCoinValue2;			//��ֵ���յ�Ӳ�����2	1	BIN	0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP
		BYTE acceptChrgrCoinValue3;			//��ֵ���յ�Ӳ�����3	1	BIN	0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP
		BYTE acceptChrgrCoinValue4;			//��ֵ���յ�Ӳ�����4	1	BIN	0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP
		WORD BankeCardPayL;                 //���п���������޶�	2	Bin	Ĭ��1000

		_billbox_param()
		{
			acceptSalesTicketBillValue1 = 0;
			acceptSalesTicketBillValue2 = 0;
			acceptSalesTicketBillValue3 = 0;
			acceptSalesTicketBillValue4 = 0;
			acceptSalesTicketBillValue5 = 0;
			acceptSalesTicketBillValue6 = 0;
			acceptSalesTicketBillValue7 = 0;
			acceptSalesTicketBillValue8 = 0;
			acceptSalesTicketBillValue9 = 0;
			acceptSalesTicketBillValue10 = 0;
			acceptSalesTicketMaxBillCnt = 0;
			acceptSalesTicketMaxBillAmt = 0;

			acceptChrgrBillValue1 = 0;
			acceptChrgrBillValue2 = 0;
			acceptChrgrBillValue3 = 0;
			acceptChrgrBillValue4 = 0;
			acceptChrgrBillValue5 = 0;
			acceptChrgrBillValue6 = 0;
			acceptChrgrBillValue7 = 0;
			acceptChrgrBillValue8 = 0;
			acceptChrgrBillValue9 = 0;
			acceptChrgrBillValue10 = 0;
			acceptChrgrMaxBillCnt = 0;
			acceptChrgrMaxBillAmt = 0;

			salesNoCoin = 0;
			acceptCoinMaxCnt = 0;
			chargeCoinMaxCnt = 0;
			chargeBill = 0;
			chargeBillMaxAmt = 0;

			cancelDealTime = 0;
			showWaitScreen = 0;
			changeWaitScreenTime = 0;
			willFullBillCnt = 0;
			alreadyFullBillCnt = 0;
			coinCollectionWillFullCoinCnt=0;
			coinCollectionAlreadyFullCoinCnt=0;
			coinChangeWillEmptyCoinCnt=0;

			minTicketCnt = 0;
			willEmptyBillBoxCnt = 0;

			acceptSalesCoinValue1 = 0;
			acceptSalesCoinValue2 = 0;
			acceptSalesCoinValue3 = 0;
			acceptSalesCoinValue4 = 0;
		}
	}BILLBOX_PARAM;   

	// TVMƱ�����
	typedef struct _magazine_param{ 
		WORD TicketNearEmptyValue;		    //TicketƱ�佫������		2	BIN	��Ԥ����Ʊ�佫��������ֵ
		WORD TicketEmptyValue;				//TicketƱ���ѿ�����		2	BIN	��Ԥ����Ʊ���ѿ�������ֵ
		WORD TicketNearFullValue;			//TicketƱ�佫������		2	BIN	��Ԥ����Ʊ�佫��������ֵ
		WORD TicketFullValue;				//TicketƱ����������		2	BIN	��Ԥ����Ʊ������������ֵ
		WORD TicketInvalidNearFullValue;	//��Ʊ�佫������			2	BIN	��Ԥ������Ʊ�佫��������ֵ
		WORD TicketInvalidFullValue;		//��Ʊ����������			2	BIN	��Ԥ������Ʊ������������ֵ
		_magazine_param(){
			TicketNearEmptyValue = 0;		    //TicketƱ�佫������		2	BIN	��Ԥ����Ʊ�佫��������ֵ
			TicketEmptyValue = 0;				//TicketƱ���ѿ�����		2	BIN	��Ԥ����Ʊ���ѿ�������ֵ
			TicketNearFullValue = 0;			//TicketƱ�佫������		2	BIN	��Ԥ����Ʊ�佫��������ֵ
			TicketFullValue = 0;				//TicketƱ����������		2	BIN	��Ԥ����Ʊ������������ֵ
			TicketInvalidNearFullValue = 0;		//��Ʊ�佫������	        2	BIN	��Ԥ������Ʊ�佫��������ֵ
			TicketInvalidFullValue = 0;			//��Ʊ����������	        2	BIN	��Ԥ������Ʊ������������ֵ

		}   
	} MAGAZINE_PARAM;

	// TVM����ɲ���С��������
	typedef struct _txn_lookup_param{ 
		BYTE minCount;				//�ɲ�ѯ������������	1	HEX	��Ԥ�����ɲ���С����������Ĭ��30����
		BYTE minTime;				//�ɲ�ѯ����ʱ������	1	HEX	��Ԥ������λ�����ӡ�Ĭ��10����
		BYTE logSaveDay;			//��־��������			1	BIN	��λ���졣Ĭ�ϣ�10�졣
		BYTE maxOffLineDay;			//�����������			1	BIN ��λ���졣Ĭ�ϣ�7�졣
		BYTE expiredTransKeepDay;	//���ϴ����ױ�������	1	BIN ��λ���졣Ĭ�ϣ�10�졣	
		_txn_lookup_param(){
			minCount = 0;
			minTime = 0;
			logSaveDay = 0;
			maxOffLineDay = 0;
			expiredTransKeepDay = 0;
		}
	} TXN_LOOKUP_PARAM;

	// TVM �������ò��� 
	typedef struct _function_param{ 
		BYTE MaxOfflineBusinessTime;	  //������������߷���ʱ��	1	BIN	��λ���죬Ĭ������
		BYTE PrintSwitch;				  //��ӡ��ť����	1	BIN	�Ƿ�����˿Ͷ���ʾ��ӡ��ť
		BYTE QueryFunctionSwitch;		  //��ѯ���ܿ���	1	BIN	�Ƿ������ѯ����
		WORD VersionPic;                  //��������ͼƬ�汾	    2	BIN
		CString FilePathPic;              //��������ͼƬ·��	40	ASCII	
		_function_param(){
			MaxOfflineBusinessTime = 0;			 
			PrintSwitch = 0;
			QueryFunctionSwitch = 0;			 
			VersionPic = 0;
			FilePathPic = "";
		}   
	} FUNCTION_PARAM;

	// ���̿ռ䱨������
	typedef enum _disk_type_{
		DISK_TYPE_SYSTEM = 1,							// ϵͳ��
		DISK_TYPE_DATA,									// ������
		DISK_TYPE_LOG,									// ��־��
	}DISK_TYPE;

	typedef struct _disk_warn_setting_{
		DISK_TYPE diskType;								// BIN	1	�������ID	1��ϵͳ�̣�2�������̣�3����־�̡�
		WORD	  wDiskNearFullPersent;					// BIN	2	���̽���������ֵ���ٷֱȣ�
		WORD	  wDiskNearFullValue;					// BIN	2	���̽���������ֵ����������λ��MB
		WORD	  wDiskFullPersent;						// BIN	2	�����������Ϸ�ֵ���ٷֱȣ�
		WORD	  wDiskFullValue;						// BIN	2	�����������Ϸ�ֵ����������λ��MB

		_disk_warn_setting_(){
			diskType = DISK_TYPE_SYSTEM;
			wDiskNearFullPersent	= 0;
			wDiskNearFullValue		= 0;
			wDiskFullPersent		= 0;
			wDiskFullValue			= 0;
		}
	}DISK_WARN_SETTING;

	// ���нӿ�
    void GetTpuParam(TPU_PARAM&);						// ���TVMƱ����д����
	void GetMaintainParam(MAINTAIN_PARAM&);				// ���TVMά�޵�¼��ز���
	void GetMagazineParam(MAGAZINE_PARAM&);				// ���TVMƱ�����
	void GetBillboxParam(BILLBOX_PARAM&);				// ���TVMǮ�����
	void GetTxnLookupParam(TXN_LOOKUP_PARAM&);			// ���TVM����ɲ���С��������
	WORD GetAutoLogoffTime();							// ����Զ��ǳ��ȴ�ʱ�����
	WORD GetPwdInputTime();                             // �����������ʱ�����
	WORD GetPwdInputMaxTimes();							// ��ȡ������볢�Դ���
	WORD GetDoorUnlockAlarmTime();                      // ά����δ�رձ���ʱ��
	const vector<BYTE>& GetAllPaymentID();				// ���ѷϷ�����ԣ�
	DWORD GetMaxSellValue();							//���ѷϷ�����ԣ�
	BYTE GetCombinePayFlag();							// ���ѷϷ�����ԣ���úϲ�֧�����ñ�־ 1��true  0��false  ����ʱ��������˫��Ʒ���ϲ�֧��

	bool SupportSaleWithoutCoinChange();				// û��Ӳ������ʱ�Ƿ�������Ʊ
	bool SupportBHChangeWithSale();						// ��Ʊʱ�Ƿ�����ֽ������
	WORD GetBHChangeMaxPiece();							// ֽ�������������
	WORD GetBHChangeMaxAmount();						// ֽ�����������
	bool IsEnableAssistanceBtn();						// ��Ԯ��ť�Ƿ�����
	// ��־��������
	WORD GetMaxLogSaveDays();							// ȡ��־������������λ���죩
	// �ѻ���Ӫ�������
	WORD GetOfflineWorkDays();							// ��ȡ�ѻ���Ӫ�����������λ���죩
	// ���ϴ����ױ�������
	WORD GetSendedMsgSaveDays();						// ��ȡ���ϴ����ױ�����������λ���죩

	// ��ʾ�������棿
	bool IsReadyToRestUsed();										// �����е��ȴ����Ƿ�ʹ��

	// ֽ�ҽӿڿ��Ժϳ�һ�֣�ͬʱֻ��ʹ��һ��
	void GetAcceptBanknoteFaceOfIssueCard(vector<WORD>& vecFace);	// ��ȡ��Cardʱ�ɽ��յ�ֽ����ֵ����MOPΪ��λ��
	WORD GetAcceptBanknoteMaxPieceOfIssueCard();					// ��ȡ��Cardʱ�ɽ��յ�ֽ���������
	WORD GetAcceptBanknoteMaxAmoutOfIssueCard();					// ��ȡ��Cardʱ�ɽ��յ�ֽ������ܶ�
	void GetAcceptCoinFaceOfIssue(vector<WORD>& vecFace);			// ��ȡ��Ʊʱ�ɽ��յ�Ӳ����ֵ����AVOSΪ��λ��
	WORD GetAcceptCoinMaxPieceOfIssue();							// ��ȡ��Ʊʱ�ɽ��յ�Ӳ�����ö��
	WORD GetChageCoinMaxPieceOfIssue();								// ��ȡ��Ʊʱ�������Ӳ�����ö��

	// ��ֵ
	void GetAcceptBanknotesFaceOfRecharge(vector<WORD>& vecFace);	// ��ȡ��ֵʱ�ɽ��յ�ֽ�ҵ���ֵ
	WORD GetAcceptBanknotesMaxPieceOfRecharge();					// ��ȡ��ֵʱ�ɽ��յ�ֽ�ҵ��������
	WORD GetAcceptBanknotesMaxAmountOfRecharge();					// ��ȡ��ֵʱ�ɽ��յ�ֽ������ܶ�
	void GetAcceptCoinFaceOfRecharge(vector<WORD>& vecFace);		// ��ȡ��ֵʱ�ɽ��յ�Ӳ�ҵ���ֵ
	WORD GetAcceptCoinMaxPieceOfRecharge();							// ��ȡ��ֵʱ�ɽ��յ�Ӳ�ҵ��������
	WORD GetChageCoinMaxPieceOfRecharge();							// ��ȡ��ֵʱ�������Ӳ�ҵ����ö��

	WORD ConvertCoinFace(BYTE code);								// �������е�Ӳ����ֵ����תΪ�Է�Ϊ��λ����ֵ
	bool SupportCoinFaceOfIssue(WORD face);							// �Ƿ�֧��ָ����ֵ��Ӳ�ң���AVOSΪ��λ��

	// ��������
	bool SupportPrintSwitch();										// �Ƿ���ʾ��ӡ��ť

	// Ӳ��ö��״̬
	WORD GetChangeCoinBoxNearEmptyCount();								// ��ȡ������A ����ö��
	WORD GetRecycleBoxNearFullCount();									// ��ȡ������A ����ö��
	WORD GetRecycleBoxFullCount();										// ��ȡ������A ����ö��

	// ֽ������״̬
	WORD GetBanknotesNearFullCount();										// ��ȡֽ���佫������
	WORD GetBanknotesFullConut();											// ��ȡֽ������������
	WORD GetBanknoteChangeNearEmptyCount();									// ֽ�������佫������

	// Ʊ������״̬
	// Ticket ��
	WORD GetTicketNearEmptyCount();											// ��ȡTicket��������
	WORD GetTicketEmptyCount();												// ��ȡTicket�ѿ�����
	// Ticket ��Ʊ��
	WORD GetTicketInvalidNearFullCount();									// ��ȡTicket��Ʊ�佫������
	WORD GetTicketInvalidFullCount();										// ��ȡTicket��Ʊ����������

	WORD GetCancelDealTime();												// ��ȡȡ������ʱ��
	WORD GetReadyToRestTime();												// ��ȡ�ȴ��е�������ʱ��

	// ���̱�������
	WORD GetSystemDiskNearFullPersent();									// ��ȡϵͳ�̽�����ֵ�ٷֱ�
	WORD GetSystemDiskNearFullValue();										// ��ȡϵͳ�̽�����ֵ��MB��
	WORD GetSystemDiskFullPersent();										// ��ȡϵͳ����������ֵ�ٷֱ�
	WORD GetSystemDiskFullValue();											// ��ȡϵͳ����������ֵ��MB��

	WORD GetDataDiskNearFullPersent();										// ��ȡ�����̽�����ֵ�ٷֱ�
	WORD GetDataDiskNearFullValue();										// ��ȡ�����̽�����ֵ��MB��
	WORD GetDataDiskFullPersent();											// ��ȡ��������������ֵ�ٷֱ�
	WORD GetDataDiskFullValue();											// ��ȡ��������������ֵ��MB��

	WORD GetLogDiskNearFullPersent();										// ��ȡ��־�̽�����ֵ�ٷֱ�
	WORD GetLogDiskNearFullValue();											// ��ȡ��־�̽�����ֵ��MB��
	WORD GetLogDiskFullPersent();											// ��ȡ��־����������ֵ�ٷֱ�
	WORD GetLogDiskFullValue();												// ��ȡ��־����������ֵ��MB��

    static CAfcTVMOperationParam&  GetInstance()
    {
        return theInstance;
    }

private:

	SECTION_INFO m_section[6];					// �ֶ���Ϣ

    // ˽�г�Ա����
    TPU_PARAM				m_tpu_param;			// TVMƱ����д����
	MAINTAIN_PARAM			m_maintain_param;		// TVMά�޵�¼��ز���
	BILLBOX_PARAM			m_billbox_param;        // TVMǮ�����
	MAGAZINE_PARAM			m_magazine_param;		// TVMƱ�����
	TXN_LOOKUP_PARAM		m_txn_lookup_param;		// TVM����ɲ���С��������
	FUNCTION_PARAM			m_function_param;       // TVM �������ò���	
	vector<DISK_WARN_SETTING>	m_vecDiskWaringSet; // TVM���̿ռ䱨������

    // ˽�г�Ա����
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAfcTVMOperationParam(void);
    ~CAfcTVMOperationParam();
	CAfcTVMOperationParam(const CAfcTVMOperationParam&);
	CAfcTVMOperationParam& operator=(const CAfcTVMOperationParam&);

    static  CAfcTVMOperationParam theInstance;
};
