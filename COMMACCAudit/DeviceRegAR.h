#pragma once
#include "ACCAuditDef.h"
#include "CACCAuditException.h"
#include "Util.h"
#include "ACCAuditExp.h"


typedef map<unsigned short,int> DEVICEREG_AR;


// �豸�Ĵ���Ψһ�ӿ�
#define theDeviceRegAR DeviceRegAR::GetInstance()  
// 
class ACCAUDIT_DECLSPEC DeviceRegAR
{
public:

	static DeviceRegAR &GetInstance()
	{
		return theInstance;
	}

	void Initialize(CString strMainPath,CString strBackupPath);

	// TH �豸�Ĵ���
	void AddThResetCount(int nAddCount = 1);			// ����ʹ�ô���
	void AddThBoxAUpDownCount(int nAddCount = 1);		// Ʊ��A��������
	void AddThBoxBUpDownCount(int nAddCount = 1);		// Ʊ��B��������
	void AddThScratchWheelACount(int nAddCount = 1);	// ��ƱA��Ʊ����
	void AddThScratchWheelBCount(int nAddCount = 1);	// ��ƱB��Ʊ����

	//void AddThTransMotorCount(int nAddCount = 1);		// ������ʹ�ô���
	void AddThSendOutMotorCount(int nAddCount = 1);		// ��Ʊ���ʹ�ô���
	void AddThLinkElecFlipCount(int nAddCount = 1);		// ���˵������������

	// CH �豸�Ĵ���
	void AddChResetCount(int nAddCount = 1);			// �������ô���
	void AddChSortTrayRunCount(int nAddCount = 1);		// �ּ���ת������
	void AddChTransMotorCount(int nAddCount = 1);		// ������ת������
	void AddChCycleChangeCount(int nAddCount = 1);		// ѭ���������������
	void AddChSupplayBoxAChangeCount(int nAddCount = 1);// ������A�������
	void AddChSupplayBoxBChangeCount(int nAddCount = 1);// ������B�������

	// BHChange �豸�Ĵ���
	void AddBhcResetCount(int nAddCount = 1);			// ����ʹ�ô���
	void AddBhcBoxAChangeCount(int nAddCount = 1);		// ֽ��������A��������
	void AddBhcBoxBChangeCount(int nAddCount = 1);		// ֽ��������B�߳�����

	// BH �豸�Ĵ���
	void AddBhResetCount(int nAddCount = 1);			// ����ʹ�ô���
	void AddBhRecongnitionCount(int nAddCount = 1);		// ֽ��ʶ�����
	void AddBhMotorPressCount(int nAddCount = 1);		// Ѻ�����ѹ�����

	// ����
	void AddAR(DEVICEREG_AR& accAR);
	DEVICEREG_AR& GetDeviceRegAr();

	void ClearDeviceAR();								// ���AR����
	
private:
	CString m_dataPath;
	CString m_BackupPath;

	DEVICEREG_AR m_deviceRegAr;

	static DeviceRegAR theInstance;

	void LoadACCARFromFile(DEVICEREG_AR&);
	void SaveACCARToFile(DEVICEREG_AR&);
	void SerializeACCAR(DEVICEREG_AR&,VARIABLE_DATA&);
	void DeserializeACCAR(DEVICEREG_AR&,VARIABLE_DATA&);
	void Initialize();

	DeviceRegAR(void);
	~DeviceRegAR(void);
};
