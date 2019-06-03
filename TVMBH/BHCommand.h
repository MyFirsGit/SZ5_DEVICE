#pragma once
#include "BHExp.h"
#include "Command.h"
#include "BHdef.h"
#include "BHLib.h"

/**
 @brief  BH�����
*/
class BH_API CBHCommand : public CCommand
{
public:
	CBHCommand(void);   
    ~CBHCommand(void);
public:
	BOOL         IsTimeUp(IN DWORD dwTimer,IN DWORD	dwBaseTime);
	BHTRS_RESULT SendOneCommand(DWORD nSendLength,BYTE* pSendBuffer, BYTE* pRecvBuff,DWORD& nRecvLength,DWORD dwTimer = 10000);
	BHTRS_RESULT SendOneCommand_NoRrcv(DWORD nSendLength,BYTE* pSendBuffer);
};
/**
 @brief	ԭ��������򿪴���
*/
class BH_API CBHConnect : public CBHCommand
{
public:
	CBHConnect(int nPort, int nBaud, int nSize);
	~CBHConnect();
public:
	long ExecuteCommand();
private:
	int m_nPort;						// �˿ں�
	int m_nBaud;						// ������
	int m_nSize;						// �ֳ�
	BHTRS_CONNECT_PARAM m_connectParam;	// ���Ӳ���
private:
	long ConnectFunction();
};

/**
 @brief	ԭ��������رմ���
*/
class BH_API CBHClose : public CBHCommand
{
public:
	CBHClose();
	~CBHClose();
public:
	long ExecuteCommand();
private:
	long CloseFunction();
};

/**
 @brief	ԭ���������λ��TF��
*/
class BH_API CBHReset : public CBHCommand
{
public:
	CBHReset();
	~CBHReset();
	DWORD done_phase; // ��ɽ׶�
public:
	long ExecuteCommand();
private:
	long ResetFunction();
};

/**
 @brief	ԭ���������ȡ״̬��ST��
*/
class BH_API CBHGetSts : public CBHCommand
{
public:
	CBHGetSts();
	~CBHGetSts();
public:
	long ExecuteCommand();
	long GetResponse(BH_STS*);
private:
	long GetStsFunction();
	BH_STS m_bhSts;
};

/**
 @brief	ԭ���������ȡ��ֵ��INDxx��
*/
class BH_API CBHGetFaceValue : public CBHCommand
{
public:
	CBHGetFaceValue(BYTE* face_id);
	~CBHGetFaceValue();
public:
	long ExecuteCommand();
	long GetResponse(BH_FACEVALUE*);
private:
	long GetFaceValueFunction();
	BH_FACEVALUE m_face_value;			// ʵ����ֵ��С
	BYTE* m_face_id;					// ֽ����ֵ���
};

/**
 @brief	ԭ�����������ʶ��ģʽ����ʼ����ֽ�ң�V1��
*/
class BH_API CBHStartValidation : public CBHCommand
{
public:
	CBHStartValidation();
	~CBHStartValidation();
public:
	long ExecuteCommand();
private:
	long StartValidationFunction();
};

/**
 @brief	ԭ��������������ģʽ��ֹͣ����ֽ�ң�V0��
*/
class BH_API CBHStopValidation : public CBHCommand
{
public:
	CBHStopValidation();
	~CBHStopValidation();
public:
	long ExecuteCommand();
private:
	long StopValidationFunction();
};

/**
 @brief	���������˻ػ�������ֽ�ң�������->���ڣ���B0+ST���Ƿ�ȴ��˳��ڳ�Ʊ��ȡ�ߣ�����ʱȷ����
*/
class BH_API CBHReturnOne : public CBHCommand
{
public:
	CBHReturnOne(bool isCheckShutter);
	~CBHReturnOne();
	DWORD done_phase; // ��ɽ׶�
public:
	long ExecuteCommand();
private:
	long ReturnOneFunction();
	bool m_isCheckShutter;
};

/**
 @brief	��������ѹǮ�䣨������+�ݴ���->Ǯ�䣩��B1+ST,����Ǯ�佫�������жϣ�
*/
class BH_API CBHCollect : public CBHCommand
{
public:
	CBHCollect();
	~CBHCollect();
	DWORD done_phase; // ��ɽ׶�
public:
	long ExecuteCommand();
private:
	long CollectFunction();
};

/**
 @brief	���������˻�ȫ��ֽ�ң�������+�ݴ���->���ڣ���B2+ST���Ƿ����˳����г�Ʊ�ڹ���ʱȷ�ϣ�
*/
class BH_API CBHReturnAll : public CBHCommand
{
public:
	CBHReturnAll(bool isCheckShutter);
	~CBHReturnAll();
	DWORD done_phase; // ��ɽ׶�
public:
	long ExecuteCommand();
private:
	long ReturnAllFunction();
	bool m_isCheckShutter;
};

/**
 @brief	��������ѹ�ݴ棨������->�ݴ�������B3+ST��
*/
class BH_API CBHHold : public CBHCommand
{
public:
	CBHHold();
	~CBHHold();
	DWORD done_phase; // ��ɽ׶�
public:
	long ExecuteCommand();
private:
	long HoldFunction();
};

/**
 @brief ԭ������������Ʊ��������->�ݴ�������BD��
*/
class BH_API CBHTransHold : public CBHCommand
{
public:
	CBHTransHold();
	~CBHTransHold();
public:
	long ExecuteCommand();
private:
	long TransHoldFunction();
};

/**
 @brief	ԭ������������Ʊ��������->Ǯ�䣩��BE��
*/
class BH_API CBHTransCollect : public CBHCommand
{
public:
	CBHTransCollect();
	~CBHTransCollect();
public:
	long ExecuteCommand();
private:
	long TransCollectFunction();
};

/**
 @brief	ԭ������������Ʊ��������->���ڣ���BF��
*/
class BH_API CBHTransReturn : public CBHCommand
{
public:
	CBHTransReturn();
	~CBHTransReturn();
public:
	long ExecuteCommand();
private:
	long TransReturnFunction();	
};

/**
 @brief	ԭ���������ֹ��ѯ������ȫ�ֱ�����
*/
class BH_API CBHAbortAction : public CBHCommand
{
public:
	CBHAbortAction();
	~CBHAbortAction();
public:
	long ExecuteCommand();
};

BH_API void  BH_EnableInsert();
BH_API void  BH_DisableInsert();

/**
 @brief	���������ȴ�Ͷ��ʶ��V1+ST��
*/
class BH_API CBHOpenAndValidate : public CBHCommand
{
public:
	CBHOpenAndValidate();
	CBHOpenAndValidate(BH_FACE_ACEPTED_TABLE& bhTable);
	~CBHOpenAndValidate();
public:
	long ExecuteCommand();	
	long GetResponse(BH_STATUS*);
public:
	DWORD done_phase;				// ִ�н��
private:
	long OpenAndValidateFunction();
	long GetFace();
	bool IsAcceptFaceValue(BankNoteAndCoinType_t& thisFaceValue);
	BH_STATUS m_bhStatus;
	BH_FACE_ACEPTED_TABLE m_bhTable;
};

/**
 @brief	����������մ���ϵͳ��BD/BE/BF+ST��
*/
class BH_API CBHClearTrans : public CBHCommand
{
public:
	CBHClearTrans(BH_CLEAN_DEST dest = CLEAN_TO_BOX);
	~CBHClearTrans();
public:
	long ExecuteCommand();
public:
	DWORD done_phase;				// ִ�н��
private:
	long ClearTransFunction();
	BH_CLEAN_DEST m_clean_dest;
};

/**
 @brief ����������ȡ�汾��RS0+RS1+RS2+RS3+RS5��
*/
class BH_API CBHGetVersion : public CBHCommand
{
public:
	CBHGetVersion();
	~CBHGetVersion();
public:
	long ExecuteCommand();
	long GetResponse(BH_VERSION*);
private:
	long GetVersionFunction();
	BH_VERSION m_bhVer;
};

/**
 @brief	����������ȡǮ��ID��D4,2+R+D4,3+R��
*/
class BH_API CBHGetBoxID : public CBHCommand
{
public:
	CBHGetBoxID();
	~CBHGetBoxID();
public:
	long ExecuteCommand();
	long GetResponse(BH_BOX_ID*);
private:
	long GetBoxIDFunction();
	BH_BOX_ID m_box_id;
};

/**
 @brief	ԭ�������ʱ�����ã�MAW*��
*/
class BH_API CBHSetTime : public CBHCommand
{
public:
	CBHSetTime(_DATE_TIME current);
	~CBHSetTime();
public:
	long ExecuteCommand();
	long GetResponse(_DATE_TIME*);
private:
	long SetTimeFunction();
	_DATE_TIME m_current_datetime;
	_DATE_TIME m_bh_datetime;
};
