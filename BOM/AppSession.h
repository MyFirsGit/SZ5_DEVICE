#include "childview.h"
#include "ChildViewRec.h"
#include "servicedefinition.h"
#include "SvcDef.h"
#pragma once


#define theAPP_SESSION  CAppSession::GetInstance()

/**
  @brief ��BOMȫ��ҵ�������Ϣ���й������
*/
class CAppSession
{
public:
    static CAppSession& GetInstance();                                  // ��ȡ����Ķ���

//    // ����״̬
    void SetSCConnected(bool);                                         // ��������״̬
    bool IsSCConnected() const;                                        // �ж��Ƿ�����
   void SetHasNewParam(bool);                                          // �����Ƿ����²���������λ���أ�
   bool HasNewParam() const;                                           // �ж��Ƿ����²���������λ���أ�
//    void SetIsCDVersionChange(bool);                                    // �����Ƿ���CD�汾����
//    bool IsCDVersionChange() const;                                     // �ж��Ƿ���CD�汾����
//
//    // ���ڻ�ʧ��
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
	void SetInitDetailErrorMsg(CString);								// ���ó��ڻ���ϸ��������
	CString GetInitDetailErrorMsg();									// ��ȡ���ڻ���ϸ��������
//
	// TH�˿��Ƿ�����
	bool IsTHConnected() const;                                         // �ж�RW�˿��Ƿ�����
	void SetIsTHConnected(bool);                                        // ����RW�˿��Ƿ�����
//
//    // �����һ������
//    bool IsFirstRun() const;                                            // �ж��Ƿ�����һ������
//    void SetIsFirstRun(bool);                                           // �����Ƿ�����һ������
//

//
//    // ����δ������
//    bool HasDeliverData() const;                                        // �ж��Ƿ���δ������
//    void SetHasDeliverData(bool);                                       // �����Ƿ���δ������
//
//    // �汾��Ϣ����
//    void SaveVersion(const VARIABLE_DATA& binData);                      // ���������Ϣ
//    void GetVersion(VARIABLE_DATA& binData);                             // ��ȡ������Ϣ
    void SetHasProgUpdate(bool);                                        // �����Ƿ��г������
    bool HasProgUpdate();                                               // �ж��Ƿ��г������
//
//    void SetIsDownload(bool);                                           // �����Ƿ�����FTP��
//    bool GetIsDownload() const;                                         // ��ȡ�Ƿ�����FTP��
//
//    void SetIsUpdateParam(bool);                                        // �����Ƿ��������ز�����
//    bool GetIsUpdateParam() const;                                      // ��ȡ�Ƿ��������ز�����
//
//    void SetIsWorkingHourChange(bool);                                  // �����Ƿ���Ӫ���л�
//    bool GetIsWorkingHourChange() const;                                // ��ȡ�Ƿ���Ӫ���л�
//
//
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

    // ��վ��Ϣ
    CString GetStationNameCN() const;                                   // ��ȡ��վ��վ���ƣ����ģ�
    CString GetStationNameEN() const;                                   // ��ȡ��վ��վ���ƣ�Ӣ�ģ�
    void SetStationName(const CString, const CString);                  // ���õ�վ��վ����

    void ShowOfficeGuide(CString sMsg);                                 // ��ʾվԱ�����򵼱���
    void ShowReceptionGuide(CString sMsg, CString sEnglishMsg);         // ��ʾ�˿ͻ�������Ϣ
    void ShowAllGuide(CString sOfficeMsg, CString sReceptionMsg, 
        CString sReceptionEnglishMsg);                                  // ��ʾվԱ�ͳ˿ͻ�������Ϣ
    CChildView* GetChildView();                                         // ��ȡChildView
    CChildViewRec* GetChildViewRec();                                   // ��ȡ�˿��Ӵ�ChildViewRec
    CString GetServiceNameCH(SERVICE_ID serviceID);                     // ��ȡҵ�����ƣ����ģ�
    CString GetServiceNameEN(SERVICE_ID serviceID);                     // ��ȡҵ�����ƣ�Ӣ�ģ�
   // void ShowReceptionServiceName(CString nameCH, CString nameEN);      // ��ʾ�˿ͻ��泵վ����
    void ParseGuideMsg(CString originalGuideMsg, CString& guideMsg, 
        COLORREF& color);                                               // ��������Ϣ
//
//    bool HasARToSend();                                                 // �Ƿ���AR������Ҫ����
//    void SetARToSend(bool need);                                        // �����Ƿ���AR������Ҫ����
//

//
//	void SetRePrintStaffID(CString);									// ����ǰ�δ�ӡԱ��ID
//	void SetRePrintLoginTime(_DATE_TIME);								// ����ǰ�δ�ӡ��¼ʱ��
//	void SetRePrintGroupID(CString);									// ����ǰ�δ�ӡ��ID
//
//	CString GetRePrintStaffID();										// ȡ��ǰ�δ�ӡԱ��ID
//	_DATE_TIME GetRePrintLoginTime();									// ȡ��ǰ�δ�ӡ��¼ʱ��
//	CString GetRePrintGroupID();										// ȡ��ǰ�δ�ӡ��ID
//
private:

    bool m_isSCConnected;                                              // �Ƿ�����
    bool m_hasNewParam;                                                 // �Ƿ����²���������λ���أ�
    bool m_hasProgUpdate;                                               // �Ƿ��г������
//    bool m_isFirstRun;                                                  // �Ƿ��һ�����г���
      bool m_InitCheckError;                                              // ���ڼ��ʧ��
      DWORD m_initDetailErrorFlag;										// ���ڻ���ϸ�����־
	CString m_initDetailErrorMsg;										// ���ڻ���ϸ��������
//    bool m_isDownload;                                                  // �Ƿ�����FTP��
//    bool m_needSendAR;                                                  // �Ƿ���AR������Ҫ����
	bool m_isTHConnected;                                               // RW�˿��Ƿ�����
//    bool m_isUpdateParameter;                                           // ACC�����Ƿ����ڸ���
//    bool m_isWorkingHourChange;                                         // �Ƿ���Ӫ���л�
	CLOSE_START_FLAG  m_closeStartFlag;                                 // ���մ����־
	USER_INFO         m_userInfo;                                       // վԱ��Ϣ
    CString           m_stationNameCN;                                  // ��վ���ƣ����ģ�
    CString           m_stationNameEN;                                  // ��վ���ƣ�Ӣ�ģ�
    SAM_ID_INFO   m_SAMInfo;                                   // SAMid�Ƿ�ı�

//
//	CString m_printStaffID;												// ǰ��Ա��ID
//	_DATE_TIME m_printLoginTime;										// ǰ��Ա����¼ʱ��
//	CString m_printGroupID;												// ǰ�ΰ�ID

    // Hidden
private:
	static CAppSession theInstance;                                     // �����Ψһ�ӿ�
    CAppSession();
    ~CAppSession(){};
    CAppSession(const CAppSession&);
    CAppSession& operator=(const CAppSession&);
};