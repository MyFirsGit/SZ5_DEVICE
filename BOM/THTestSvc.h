#pragma once
#include "bomforeservice.h"

// �����������
typedef enum single_test_type{		
	TRANSFER_TEST = 1,						// ������Ʊ����
	VALIDATE_SEND  = 11,
	//RECYCLE_SEND  = 12,
	RESET_TEST = 2,							// ���ò���
	TH_RESET = 21,
	MOVE_BOX_TEST = 3,						// ����Ʊ�����
	MOVE_BOXA_TEST = 31,
	MOVE_BOXB_TEST = 32,	
	INVALID_HANDLE_TEST = 4,                 //��Ʊ�������
	INVALID_TEST = 41,	
	SENSOR_TEST = 5,						// ����������
	SENSOR_SW_TEST = 51,
	RW_RFID_TEST = 6,						// RFID��д
	R_RFIDNUM_TEST = 61,
	W_RFIDNUM_TEST = 62,
}SINGLE_TEST_TYPE;	

/**
@brief    ����ҵ�� - ��Ʊ������
*/
class CTHTestSvc : public CBOMForeService
{
public:
	CTHTestSvc();
	~CTHTestSvc();

	typedef struct _tagModel{
		CString strBoxA;			// AƱ����
		CString strBoxB;			// BƱ����
		CString strBoxC;			// BƱ����
		//SC_RES_READ_RFID readBoxAInfo;
		//SC_RES_READ_RFID readBoxBInfo;
		//SC_RES_READ_RFID readBoxCInfo;
		bool isBoxAExit;
		bool isBoxBExit;
		bool isBoxCExit;
		_tagModel(){
			strBoxA = _T("");
			strBoxB = _T("");
			strBoxC = _T("");
			isBoxAExit = false;
			isBoxBExit = false;
			isBoxCExit = false;
		}
	}Model;

	Model& GetDataModel();
private:
	typedef enum _dialog_group {
		DIALOG_GROUP_TH_TEST		= 1,                       
	} DIALOG_GROUP;
	Model m_DataModel;
};