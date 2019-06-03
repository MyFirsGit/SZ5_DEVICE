#pragma once

#define theFunction_INFO CFunctionInfo::GetInstance()       // ��������Ψһ�ӿ�

/**
@brief FunctionInfo.INI����Ϣ����
*/
class CFunctionInfo
{
public:

    static CFunctionInfo&  GetInstance()
    {
        return theInstance;
    }

    int Initialize();                              // ���ڻ�

	// === TVM�������� ===
	int  GetACCAuthorizeManner();	                    // ACC��֤��ʽ��0������֤��1���ڲ�ģʽ��2���ⲿģʽ��  
	int  GetECTAuthorizeManner();                      // ECT��֤��ʽ��0������֤��1���ڲ�ģʽ��2���ⲿģʽ��
	bool IsTpuParamterZipFlag();                        // TPU����ѹ����־��0����ѹ����1����Ҫѹ����
	bool IsUseFounderStaffID();							// �Ƿ�����Ĭ��Ա��
	int GetFounderStaffID();								// ��ȡĬ��Ա��ID
	bool IsFounderStaffID(int userID);					// �Ƿ���Ĭ��Ա��
	CString GetFounderStaffPassword();				// ��ȡĬ��Ա������
	bool IsFounderOperation();                          // �Ƿ����ø߼����ܣ�0�������ã�1�����ã�
	bool GetUSBAuthFlag();							   // USB��֤��־��0������֤��1����Ҫ��֤��
	bool IsStaffDefaultService();						   // ����Ա��¼��Ĭ��ҵ��Service ID��
	bool IsDeleteParameterHead();						// �Ƿ�ɾ������Э��ͷ��β��
	bool IsShowInitProcess();							// �Ƿ���ʾ��ʼ��ϸ�ڹ���
	bool IsEquipmentAutomaticRunningOn();				// �豸�Զ�����״̬�Ƿ�����
	bool IsShowNearlyFullEmptyParaValue();				// �Ƿ���ʾ�豸�ռ����ղ���ֵ
	int  GetPrintLastReceiptCount();					// ȡ����Ĵ�ӡʧ�ܵ�����
	bool IsShowPaymentMethod();							// �Ƿ���ʾ֧����ʽ����
	bool IsNeedPassengerClickOkButton();				// �Ƿ���Ҫ�û����ȷ�ϰ�ť
	bool IsAutoDegradeWhenCashDoorOpen();				// �Ƿ����ֽ���״̬��
	int GetLoadMaxTransactionHistory();					// ���ɼ��ؽ�����ʷ��¼����
	bool IsBanknoteChangeBoxIDRead();					// ֽ������ģ��RFID�Ƿ�ʹ�ã�
	//=== �������� ===
	UINT GetLimitedDaysOfUnSendData();
	UINT GetLimitedDaysOfSendData();

	void SetLimitedDaysOfSendData(int nDays);

	// === ��·���ã�0:�����ã�1�����ã� ===
	bool    IsLineEnabled(DWORD lineCode);           // �ж�ָ����·��ť�Ƿ����
    
	// === ��վ���ã�0:�����ã�1�����ã� ===
	bool    IsStationEnabled(WORD stationCode);    // �ж�ָ����վ��ť�Ƿ�ɼ�

	// === �˵���ť����===
	bool IsMenuEnabled(WORD menuID);				// ��ť�Ƿ����
	bool IsMenuVisibled(WORD menuID);				// ��ť�Ƿ�ɼ�

	//=== ����Ʊ������(0:�����ۣ�1������) ===
	bool IsTicketIssueEnabled(WORD TicketTypeId);														// �жϳ�Ʊ�Ƿ�ɷ���
	bool IsPayTypeEnable(WORD ServiceID,CardType_t TicketType,int PayTypeId);					// �жϸ��ʽ�Ƿ����

	//=== ��Ʊ���� ===
	vector<int> GetTicketCounts();																					// ��ȡ��Ʊ����������ť��Ϣ

	//=== ��Ʊ������Ʊ��===
	vector<int> GetTicketAmount();


	int GetTicketMaxAmount() const;
	void SetTicketMaxAmount(int )const ;

	//=====RFID����=========
	bool IsCardNormalBoxRFIDWriteable();
	bool IsCardWasteBoxRFIDWriteable();
	bool IsCoinChangeBoxRFIDWriteable();
	bool IsCoinRecycleBoxRFIDWriteable();
	bool IsBanknoteChangeBoxRFIDWriteable();
	bool IsBanknoteRecycleBoxRFIDWriteable();
	bool IsTokenNormalBoxRFIDWriteable();
	bool IsTokenWasteBoxRFIDWriteable();

	bool IsAutoReadRfidWithoutManual();					// �Ƿ��Զ���ȡRFID

	//=====�����趨=========

	CString IsCollectOverFile();                         // ���ն�������������·��                             
	int IsCollectOverBuzzerTime();					 // ���ն�����������������ʱ������λ���룩  
	int IsCollectOverBuzzerInterval();				 // ���ն�����������������ʱ��������λ���룩
	CString IsExceptionOccurFile();				     // �쳣����������·��                             
	int IsExceptionOccurBuzzerTime();				 // �쳣��������������ʱ������λ���룩  
	int IsExceptionOccurBuzzerInterval();			 // �쳣��������������ʱ��������λ���룩
	CString IsOperationMissFile();                    // �쳣����������·��                             
	int IsOperationMissBuzzerTime();				 // �쳣��������������ʱ������λ���룩  
	int IsOperationMissBuzzerInterval();			 // �쳣��������������ʱ��������λ���룩
	CString IsDemotionOccurFile();                  // �Զ���������������·��                             
	int IsDemotionOccurBuzzerTime();			 // �Զ�������������������ʱ������λ���룩  
	int IsDemotionOccurBuzzerInterval();		 // �Զ�������������������ʱ��������λ���룩

	//=====��ȡLED��ʾ����=========
	CString getModeLanguageCN(WORD wdModeCode);
	CString getModeLanguageEN(WORD wdModeCode);
	CString getModeLanguagePN(WORD wdModeCode);

	CString getModeLanguage(WORD);
	CString getModeLanguage(WORD ,int languageId);
	CString getLedWorkMode();

	void getLedContentFormat(CStringList& strList);

	//=====��ȡ��ƱƱ��ѡ��=======
	int GetAvailableMagazineBox();					// ��ȡ��ʹ�õ���ƱƱ��

	//=====�˿�ҵ������Ĭ��ҵ���쳣�������=======
	int GetTimesRepareException();					// �˿�ҵ������Ĭ��ҵ���쳣�������

	//=====��ȡ������־�������� (�����������:������־��������:������־����Ӣ��:������־��������)=========
	CString GetOperationLogLevelContent(WORD Key,int index);
	//=====��ȡ״̬��ʾ������״̬=============
	CString getCNWorkMode();
	CString getENWorkMode();

	// [TVM_WEB_API]
	//CString GetWebApiMobileHost();					 // ��ȡ WEB_API WenHost
	CString GetWebApiHost();                         // ��ȡ WEB_API Host
	int     GetWebApiPort();                         // ��ȡ WEB_API Port
	//CString GetWebApiPwd();							 // ��ȡ WEB_API ����
	CString GetWebApiLoginUrl();					 // ��ȡ WEB_API  ��¼��ַ
	CString GetWebApiLockUrl();						 // ��ȡ WEB_API  ��ѯ��ַ
	/*CString GetWebApiLockUrl();						 // ��ȡ WEB_API  ������ַ
	CString GetWebApiConfirmUrl();					 // ��ȡ WEB_API  �ύ��ַ
	CString GetWebApiGetOrderUrl();					 // ��ȡ WEB_API  ȡƱ���ȡ�����ŵ�ַ
	CString GetWebApiTVMStateUrl();					 // ��ȡ WEB_API  ��ѯTVM״̬��ַ
	CString GetWebApiUpdateStateUrl();				 // ��ȡ WEB_API  �޸�TVM״̬*/
private:
    static CFunctionInfo theInstance;
    CGetIni* m_functionIni;

	map<DWORD,bool> mapLineEnable;
	map<WORD,bool> mapStationEnable;
	map<WORD,CString> m_mapPayType;
	vector<int> m_vTicketCounts;
	vector<int> m_vTicketAmount;
	int         m_accAuthorizeManner;
	int         m_ectAuthorizeManner;

	// [TVM_WEB_API]
	//CString m_strWebMobileHost;					// �ƶ���Host��ַ
	CString m_WebApiHost;                         // WEB_API Host
	int     m_WebApiPort;                       // WEB_API Port
	//CString m_WebApiPwd;						 // WEB_API ����
	CString m_WebApiLoginUrl;					 // WEB_API  ��¼��ַ
	//CString m_WebApiQueryUrl;					 // WEB_API  ��ѯ��ַ
	CString m_WebApiLockUrl;					 // WEB_API  ������ַ
	//CString m_WebApiConfirmUrl;					 // WEB_API  �ύ��ַ
	//bool	m_bWebApiUpdated;					 // WEB ��ַ�Ƿ����
	//CString m_WebApiGetOrderUrl;					 // WEB_API  ȡƱ���ȡ�����ŵ�ַ
	//CString m_WebApiTVMStateUrl;					 // WEB_API  ��ѯTVM״̬��ַ
	//CString m_WebApiUpdateStateUrl;					 // WEB_API  �޸�TVM״̬��ַ
    // ���ع��캯��
    CFunctionInfo();
    ~CFunctionInfo();
    CFunctionInfo(const CFunctionInfo&);
    CFunctionInfo& operator=(const CFunctionInfo&);
};