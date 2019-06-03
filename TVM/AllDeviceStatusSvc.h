#pragma once
#include "TVMForeService.h"
#pragma warning(disable : 4482)

/**
@brief Ӳ���Լ������豸״̬
*/
class CAllDeviceStatusSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:

	enum INIT_RESULT {
		UNKNOWN = 0,			// δ֪״̬
		SUCCESSFUL,				// �ɹ�
		FAIL,					// ʧ��
	};

	// ���ڻ���ʾ��Ϣ�ṹ
	typedef struct _INITIAL_INFO {
		CString title;          // ����
		INIT_RESULT result;     // ���
		CString errorInfo;		// ��ϸ������Ϣ
		CString dealInfo;		// ��������Ϣ
		_INITIAL_INFO()
		{
			title = "";
			result = UNKNOWN;
			errorInfo = "";
			dealInfo = "";
		}
	} INITIAL_INFO;

	// ���ڻ���ʾ�к�
	enum ENUM_ROW {
		ROW_IO_MODULE = 0,
		//ROW_CHARGE_RW,
		ROW_CARD_RW,
		//ROW_CARD_MODULE,
		ROW_TOKEN_MODULE,
		ROW_COIN_MODULE,
		ROW_BANKNOTE_MODULE,
		//ROW_BANKNOTE_CHANGE_MODULE,
		ROW_PRINT,
		ROW_MAX,
		ROW_INVALID,
	};

	INITIAL_INFO m_InitialInfo[ROW_MAX];					// ���ڻ���ʾ��Ϣ
	int m_order[ROW_MAX];									// �Ƿ��ʼ�����Գ�ʼ��������������

	CAllDeviceStatusSvc(void);								// ���캯��
	~CAllDeviceStatusSvc(void);								// ��������
	virtual void OnStart();									// Service��������
	void SetInitRessult(int index, INIT_RESULT result);		// ������ʾ���ڻ����

private:	
	typedef enum _dialog_group {
		DIALOG_GROUP_BASE = 1,
	} DIALOG_GROUP;
	void InitDataModel();
	// ��ʼ��Ӳ��ģ��
	bool InitializeStatusDisplay();						// ��ʼ������
	bool InitializeChargeRW();						    // ��ʼ����ֵ��д��
	bool InitializeCardRW();							// ��ʼ��Card��д��
	bool InitializeCardModule();						// ��ʼ��Cardģ��
	bool InitializeTokenModule();						// ��ʼ��tokenģ��
	bool InitializeCoinModule();						// ��ʼ��Ӳ��ģ��
	bool InitializeBanknoteModule();				    // ��ʼ��ֽ�ҽ���ģ��
	bool InitializeBanknoteChangeModule();				// ��ʼ��ֽ������ģ��
	bool InitializePrinter();							// ���ڻ���ӡ��(Printer)
	bool InitializeIO();								// ��ʼ��IOģ��
private:	
	void DoException(CSysException& e,int index);
	LRESULT OnKeyboardF3(WPARAM , LPARAM );             //����[F3/Ctrl]��������Ϣ�����ˣ�
	bool m_bIsFinished;
	//void OnExceptionNotify(CSysException&,bool);
};
