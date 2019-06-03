#pragma once
#include "TVMForeService.h"

/**
@brief ά��ҵ��Service
*/

#define  MAX_ROW  15

class CReceipPrintSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CReceipPrintSvc();					// ���캯��
	virtual ~CReceipPrintSvc();			// ��������

	/*
		���涨��
	*/
	typedef enum _dialog_group {
		DIALOG_GROUP_MAIN_MENU = 1,		// ���˵�ҳ��
		DIALOG_GROUP_FILE_LIST = 2,		// �ɴ�ӡ�б�ҳ��
	} DIALOG_GROUP;

	/*
		�˵�ѡ����
	*/
	typedef enum _menu_number_{
		MENU_NUM_NONE,
		MENU_NUM_BALANCE,
		MENU_NUM_BH_RECV,
		MENU_NUM_BH_CHANGE,
		MENU_NUM_CH_RECEIPT,
		MENU_NUM_TH_RECEIPT,
		MENU_NUM_EXCEPTION_RECEIPT,
	}MENU_NUMBER;

	/*
		����ģ�Ͷ���
	*/
	typedef struct _tagModel
	{
		vector<CString> vecBalanceFileList;
		vector<CString> vecBHReciveFileList;
		vector<CString> vecBHChangeFileList;
		vector<CString> vecCHOperFileList;
		vector<CString> vecTHOperFileList;
		vector<CString> vecPassengerFileList;

		MENU_NUMBER		curSelectMenu;

		_tagModel(){
			vecBalanceFileList.clear();
			vecBHReciveFileList.clear();
			vecBHChangeFileList.clear();
			vecCHOperFileList.clear();
			vecTHOperFileList.clear();
			vecPassengerFileList.clear();
			curSelectMenu = MENU_NUM_NONE;
		}

	}Model;


	inline Model& GetDataModel(){
		return m_DataModel;
	}
	
	void NotifyDataModelChanged();

	void GetSelectedFileList(vector<CString>* &pVec);
protected:
	void OnStart();								// Service��������

private:
	Model m_DataModel;
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);
	// ����Ƚ�
	//static inline bool Upgreater(const CString& strDate1,const CString& strDate2){
	//	return (strDate1.Compare(strDate2) > 0);
	//};

	//int nPage;

	void InitAllPrintedFile();						// ��ʼ�������Ѵ�ӡ��־

	void InitBalanceFileList(vector<CString>&);		// ��ʼ��ҵ�����д�ӡ�����б�

	void InitPassangerFileList(vector<CString>&);	// ��ʼ�˿�ҵ���쳣��ӡ����

	void InitStaffPrintedFileList();				// ��ʼ��վԱҵ���ӡ�����б�
	
	long TryPrintSelectedFile(const CString&);		// ���Դ�ӡѡ�е��ļ�

	void SerchAllFilesFormPath(const CString&,vector<CString>&);	// ��ȡ�ļ��������е��ļ�

	void DoPrintSelectFile(int nSelectFileID);		// ��ӡѡ�е��ļ�
};