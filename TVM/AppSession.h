#include "childview.h"
#include "ChildViewRec.h"
#include "servicedefinition.h"
#include "SvcDef.h"
#pragma once


#define theAPP_SESSION  CAppSession::GetInstance()

/**
  @brief ��TVMȫ��ҵ�������Ϣ���й������
*/
class CAppSession
{
public:
    static CAppSession& GetInstance();                                  // ��ȡ����Ķ���

	// ����״̬
    void SetSCConnected(bool);                                         // ��������״̬
    bool IsSCConnected() const;                                        // �ж��Ƿ�����
    void SetHasNewParam(bool);                                          // �����Ƿ����²���������λ���أ�
    bool HasNewParam() const;                                           // �ж��Ƿ����²���������λ���أ�

	void SetPhysicalConnStatus(bool);									// ������������״̬
	bool GetPhysicalConnStatus();										// ��ȡ��������״̬

	//void SetStartOperationFlag(bool);									// ���ÿ�ʼ��Ӫ��־
	//bool GetStartOperationFlag();										// ��ȡ��ʼ��Ӫ��־���ϴ��豸״̬�ã�

	void SetCloseOperationFlag(bool);									// ������Ӫ������־
	bool GetCloseOperationFlag() const;									// ��ȡ��Ӫ������־

    // ���ڻ�ʧ��
    bool IsInitCheckError() const;                                      // �жϳ��ڻ�ʧ��
    void SetInitCheckError(bool);                                       // ���ó��ڻ�ʧ��
//
//	bool IsInitDetailError(INIT_DETAIL_ERROR);							// �Ƿ����ĳ�����ڻ�����
	void SetInitDetailErrorFlag(INIT_DETAIL_ERROR);						// ����ĳ�����ڻ�����
//	void InitDetailErrorInfo();											// ���ڻ�������Ϣ
//	void RemoveInitDetailErrorFlag(INIT_DETAIL_ERROR);					// �Ƴ�ĳ�����ڻ�����
	DWORD GetInitDetailErrorFlag();										// ��ȡ���ڻ������־
	DWORD IsInitErrorOver();											// �Ƿ������Ҫ��ֹ�ĳ��ڻ�����
//
	void SetInitDetailErrorMsg(CString);								// ���ó��ڻ���ϸ�������ԣ�Ԥ����
	CString GetInitDetailErrorMsg();									// ��ȡ���ڻ���ϸ�������ԣ�Ԥ����
//
	// TH�˿��Ƿ�����
	bool IsTHConnected() const;                                         // �ж�TH�˿��Ƿ�����
	void SetIsTHConnected(bool);                                        // ����TH�˿��Ƿ�����

	// TOKEN�˿��Ƿ�����
	bool IsTOKENModuleConnected() const;                                // �ж�TOKEN�˿��Ƿ�����
	void SetIsTOKENModuleConnected(bool);                               // ����TOKEN�˿��Ƿ�����

	// CH�˿��Ƿ�����
	bool IsCHConnected() const;                                         // �ж�CH�˿��Ƿ�����
	void SetIsCHConnected(bool);                                        // ����CH�˿��Ƿ�����

	bool IsIOConnected() const;											// �ж�IO�˿��Ƿ�����
	void SetIsIOConnected(bool);										// ����IO�˿��Ƿ�����

	bool IsBankCardConnected() const;									// �ж����п��˿��Ƿ�����
	void SetIsBankCardConnected(bool);									// �������п��˿�״̬

	bool IsWarningStart() const;										// �ж��𶯴������Ƿ񱨾�
	void SetIsWarningStart(bool);										// �����𶯴���������״̬

    // �����һ������
    bool IsFirstRun() const;                                            // �ж��Ƿ�����һ������
    void SetIsFirstRun(bool);                                           // �����Ƿ�����һ������

	// ˢ������ά����
	int GetCardOpenMainDoorOperatorID() const;                      // ��ȡˢ������ά���ŵ�Ա��ID
	void SetCardOpenMainDoorOperatorID(int operatorID);                  // ����ˢ������ά���ŵ�Ա��ID

//    // ����δ������
//    bool HasDeliverData() const;                                        // �ж��Ƿ���δ������
//    void SetHasDeliverData(bool);                                       // �����Ƿ���δ������
//    // �汾��Ϣ����
//    void SaveVersion(const VARIABLE_DATA& binData);                      // ���������Ϣ
//    void GetVersion(VARIABLE_DATA& binData);                             // ��ȡ������Ϣ
    void SetHasProgUpdate(bool);                                        // �����Ƿ��г������
    bool HasProgUpdate();                                               // �ж��Ƿ��г������

//    void SetIsDownload(bool);                                           // �����Ƿ�����FTP��
//    bool GetIsDownload() const;                                         // ��ȡ�Ƿ�����FTP��
//
//    void SetIsUpdateParam(bool);                                        // �����Ƿ��������ز�����
//    bool GetIsUpdateParam() const;                                      // ��ȡ�Ƿ��������ز�����
//
//    void SetIsWorkingHourChange(bool);                                  // �����Ƿ���Ӫ���л�
//    bool GetIsWorkingHourChange() const;                                // ��ȡ�Ƿ���Ӫ���л�

	// SAM��Ϣ
    void SetSAMInfo(SAM_TYPE ,const LPBYTE);							// ����SAM��Ϣ
    SAM_ID_INFO GetSAMInfo();											// ��ȡSAM��Ϣ

	// ������������
	CLOSE_START_FLAG GetCloseStartFlag() const;                         // ��ȡ������������
	void SetCloseStartFlag(const CLOSE_START_FLAG);                     // ���ù����������� 

	// վԱ��Ϣ
	bool IsSuperUser();
	USER_INFO GetUserInfo() const;                                      // ��ȡվԱ��Ϣ
	void SetUserInfo(const USER_INFO&);                                 // ����վԱ��Ϣ
	bool HasUserLogin();			 									// �Ƿ���վԱ��¼��
	void SetUserLoginStatus(bool bIsLogin);

    // ��վ��Ϣ
    CString GetStationName(LANGUAGETYPE_T ) const;                      // ��ȡ��վ��վ����
    void SetStationName(const CString, const CString, const CString);   // ���õ�վ��վ����

    void ShowReceptionGuide(DWORD msgID );                              // ��ʾ�˿ͻ�������Ϣ
	void ShowReceptionGuide(CString recpMsgCn, CString recpMsgEn ,CString guidePoMsg=_T(""));  // ��ʾ�˿ͻ�������Ϣ

    void ShowAllGuide(DWORD officeMsgID,DWORD msgID);                   // ��ʾվԱ�ͳ˿ͻ�������Ϣ
	void ShowAllGuide(CString officeMsg, CString recpMsgCn, CString recpMsgEn, CString guidePoMsg=_T(""));                   // ��ʾվԱ�ͳ˿ͻ�������Ϣ

    CChildView* GetChildView();                                         // ��ȡChildView
    CChildViewRec* GetChildViewRec();                                   // ��ȡ�˿��Ӵ�ChildViewRec
    CString GetServiceName(SERVICE_ID serviceID,LANGUAGETYPE_T languageType = LANGUAGE_ID_UNSPECIFIED);                     // ��ȡҵ�����ƣ����ģ�
    void ParseGuideMsg(CString originalGuideMsg, CString& guideMsg, COLORREF& color);                                               // ��������Ϣ

//    bool HasARToSend();                                                 // �Ƿ���AR������Ҫ����
//    void SetARToSend(bool need);                                        // �����Ƿ���AR������Ҫ����
//	  void SetRePrintStaffID(CString);									// ����ǰ�δ�ӡԱ��ID
//	  void SetRePrintLoginTime(_DATE_TIME);								// ����ǰ�δ�ӡ��¼ʱ��
//	  void SetRePrintGroupID(CString);									// ����ǰ�δ�ӡ��ID
//    CString GetRePrintStaffID();										// ȡ��ǰ�δ�ӡԱ��ID
//	  _DATE_TIME GetRePrintLoginTime();									// ȡ��ǰ�δ�ӡ��¼ʱ��
//	  CString GetRePrintGroupID();										// ȡ��ǰ�δ�ӡ��ID
	delegate<void(LANGUAGETYPE_T&)> ClientLanguageModeChanged;
	delegate<void(LANGUAGETYPE_T&)> OperatorLanguageModeChanged;
	static LANGUAGETYPE_T GetCurrentClientLanguageType();
	void SetCurrentClientLanguageType(LANGUAGETYPE_T languageType = LANGUAGE_ID_CHINESE);
	static LANGUAGETYPE_T GetCurrentOperatorLanguageType();
	void SetCurrentOperatorLanguageType(LANGUAGETYPE_T languageType = LANGUAGE_ID_CHINESE);

	static LANGUAGETYPE_T GetCurrentBlindPlayVoiceLanguageType();
	// ////////////////////////////////////////////////////////////////////////////////////////////////
	// ά����ܲ���

	// ����Ա������Ϣ
	void ShowOfficeGuide(DWORD id);					// ����վԱ������Ϣ������������ʾ���ԣ�
	void ShowOfficeGuide(CString msg);				// ����վԱ������Ϣ������������ʾ���ԣ�
	void SetGuideWhenDelete(CString msg);			// ����ɾ��Ϊ��ʱ��������Ϣ
	CString GetGuideWhenDelete();					// ��ȡɾ��Ϊ��ʱ��������Ϣ

	// ����Աά������
	void SetKeyboard(DWORD keys);					// ����ά������ָ��������ö��KEYBOARD���԰�λ��
	void DiableKeyboard(DWORD keys);				// ����ά������ָ������
	void SetMaxInputLength(int length);				// �����������λ����������ǰ׺��
	void SetPrefixToGuide(CString prefix);			// �����������Ե�ǰ׺
	CString GetPrefixGuide();						// ��ȡ��������ǰ׺
	void AppendOneNumToGuide(int number);			// ׷�ӵ������ֵ���������
	void AppendOneDotToGuide();						// ׷�ӵ���Բ�㡰.������������
	bool DeleteOneCharFromGuide();					// ɾ�������ַ������ֻ�Բ�㣩
	void DeleteAllInput();							// ɾ��������������
	int		GetInputToInt();						// ��ȡ��������������
	CString GetInputToString();						// ��ȡ��������������
	bool	IsEmptyPrefix();						// �ж�ǰ׺�Ƿ�Ϊ��

	void ReturnMaintainMenu();						// ����ά��ҵ��˵���1���˵���2���˵���
	void SetMaintainMenu(SERVICE_ID serviceID);
	SERVICE_ID GetMaintainMenu();
	SERVICE_ID ResetReturnSeviceID();				// ���÷���ά��ҵ��˵���ID
	SERVICE_ID GetReturnSeviceID(){return m_return_svcid;};
	bool IsReturnMaintainMenu(bool bSetStatus = false);	// �Ƿ񷵻�ά��ҵ��

	// ����Ա�˵�������
	void SetMenuLevel(SERVICE_ID serviceID);		// ���ò˵���������
	void AppendMenuLevel(CString level);			// ׷�Ӳ˵���������

	void ShowStatusWaringMsg(LANGUAGE_MSG strMsg,bool bIsShow = true);// ��ʾ�澯��Ϣ��վԱ

	bool IsBlindManOperation();						// �ж��Ƿ���ä�˹�Ʊ
	void SetBlindManStatus(bool);					// �����Ƿ���ä�˹�Ʊ��

	bool IsManualCloseOperation();						// �ж��Ƿ����˹�ҵ�������
	void SetManualCloseOperation(bool isManual = true);	// �����Ƿ��˹�ҵ�����

	void ShowMessageToPassenger(LANGUAGE_MSG );			// ��ʾ��Ϣ���˿ͽ������Ͻ�	
	LANGUAGE_MSG GetMessageShowPassenger();				// ȡ��Ҫ��ʾ���˿ͽ������Ͻǵ���Ϣ

	void SetReceiptPrintType(const RECEIPT_PRINT_TYPE& );	// ����ƾ֤��ӡ����
	RECEIPT_PRINT_TYPE GetReceiptPrintType();				// ��ȡƾ֤��ӡ����

	bool IsCoinNeedSyncMasterFile();						// Ӳ��ģ���Ƿ�Ӧ��ͬ������
	void SetCoinNeedSyncMasterFile(bool bNeed = false);		// ����Ӳ��ģ�Ƿ�Ӧ��ͬ������

	void SetRechargeServerEnabled(bool bEnabled);			// ��ֵ������������ 
	bool IsRechargeServerEnabled();							// ��ֵ�������Ƿ���� 

private:

    bool m_isSCConnected;                                              // �Ƿ�����
	bool m_isPhysicalConnStatus;										// ��������ʱ��
	bool m_isStartOperation;											// �Ƿ�������Ӫ��ʼʱ��
    bool m_hasNewParam;                                                 // �Ƿ����²���������λ���أ�
    bool m_hasProgUpdate;                                               // �Ƿ��г������
    bool m_isFirstRun;                                                  // �Ƿ��һ�����г���
      bool m_InitCheckError;                                              // ���ڼ��ʧ��
      DWORD m_initDetailErrorFlag;										// ���ڻ���ϸ�����־
	CString m_initDetailErrorMsg;										// ���ڻ���ϸ��������
//    bool m_isDownload;                                                  // �Ƿ�����FTP��
//    bool m_needSendAR;                                                  // �Ƿ���AR������Ҫ����
	bool m_isTHConnected;                                               // TH�˿��Ƿ�����
	bool m_isTOKENConnected;                                            // TOKEN�˿��Ƿ�����
	bool m_isCHConnected;                                               // CH�˿��Ƿ�����
	bool m_isIOConnected;												// IO�˿��Ƿ�����
	bool m_isBankCardConnected;											// ���п��Ƿ�����
	bool m_isWarningStart;												// �Ƿ񴥷��𶯴�����������
	bool m_isDisableOrBlindOperation;									// �Ƿ���ä�˹�Ʊ��
	bool m_isManualCloseOperation;										// �Ƿ����˹���ҵ�������
	bool m_isCloseOperationFlag;										// ҵ�������־
//    bool m_isUpdateParameter;                                           // ACC�����Ƿ����ڸ���
//    bool m_isWorkingHourChange;                                         // �Ƿ���Ӫ���л�
	CLOSE_START_FLAG  m_closeStartFlag;                                 // ���մ����־
	USER_INFO         m_userInfo;                                       // վԱ��Ϣ
	bool			  m_bIsUserLogin;									// �Ƿ���վԱ��¼��
    CString           m_stationNameCN;                                  // ��վ���ƣ����ģ�
    CString           m_stationNameEN;                                  // ��վ���ƣ�Ӣ�ģ�
	CString           m_stationNamePN;                                  // ��վ���ƣ��������ģ�
    SAM_ID_INFO   m_SAMInfo;                                   // SAMid�Ƿ�ı�
	bool m_bIsReturnMainMenu;											// �Ƿ񷵻����˵���

	bool m_bRechargeServerEnabled;										// ��ֵ�������Ƿ����
//
//	CString m_printStaffID;												// ǰ��Ա��ID
//	_DATE_TIME m_printLoginTime;										// ǰ��Ա����¼ʱ��
//	CString m_printGroupID;												// ǰ�ΰ�ID

   static LANGUAGETYPE_T m_currentRecpLanguage;                                // ��ǰ�˿ͽ����Z��
   static LANGUAGETYPE_T m_currentOperatorLanguage;                            // ��ǰ�����T�����Z��
   static LANGUAGETYPE_T m_currentBlindPlayVoiceLanguage;					   // ��ǰä�˹�Ʊ����
   int m_guide_max;							// ����������������������磺ֻ������2λ����
   CString m_guide_prefix;					// ������ǰ׺���֣����磺��Line Code: ��
   CString m_guide_input;					// ���������벿�֣����磺��0x08��
   CString m_guide_delete;					// ɾ��Ϊ��ʱ������
   SERVICE_ID m_return_svcid;				// ���ص�ά��ҵ��˵�֮ǰ��ҵ��ID
   SERVICE_ID m_maintance_svcid;			// ���ε�½��֤������ѡҵ��ID
   LANGUAGE_MSG m_EmShowMessage;			// ��ʾ���˿͵���Ϣ

   RECEIPT_PRINT_TYPE m_receipt_print_type; // ƾ֤��ӡ������
   bool m_bNeedSyncMasterFile;				// �Ƿ���Ҫͬ������

public:
   UINT GetBNRRecyclerNearEmpty(int recyclerNo);
   void SetBNRRecyclerNearEmpty(int recyclerNo, UINT nearEmptyNum);
private:
   int m_openMainDoorOperatorID;
   UINT m_uBNRRecyclerNearEmpty_A;			// ֽ��ģ��ѭ����A������������ѭ���ĵ�ˮλ��ֵ������ֽ��ģ��ҵ�����Զ�����ʱ�����������
   UINT m_uBNRRecyclerNearEmpty_B;			// ֽ��ģ��ѭ����A������
   UINT m_uBNRRecyclerNearEmpty_C;			// ֽ��ģ��ѭ����A������
   UINT m_uBNRRecyclerNearEmpty_D;			// ֽ��ģ��ѭ����A������
private:
	static CAppSession theInstance;                                     // �����Ψһ�ӿ�
    CAppSession();
    ~CAppSession(){};
    CAppSession(const CAppSession&);
    CAppSession& operator=(const CAppSession&);
};