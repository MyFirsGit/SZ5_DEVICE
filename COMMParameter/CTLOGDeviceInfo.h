#pragma once
#include "CParameterCommandExp.h"
#include "dxfile.h"

#define theDeviceInfo    CCTLOGDeviceInfo::GetInstance()             // CCTLOGDeviceInfo��Ψһ�ӿ�

/**
@brief   �豸��Ϣ
*/
class PARAMETERCOMMAND_DECLSPEC CCTLOGDeviceInfo
{
public:	
	// �豸��Ϣ
	typedef struct _device_info {	         //     TVM           TOM          ES For Card          ES For Token
		BYTE ACC_RW1_ISAM[6];                // ��ֵ��д��      ��д��           ��д��              ��д��1
		BYTE ACC_RW1_PSAM[6];                // ��ֵ��д��      ��д��           ��д��              ��д��1
		BYTE ECT_RW1_ISAM[6];                // ��ֵ��д��      ��д��           ��д����ĸ����      ��д��1��ĸ����
		BYTE ECT_RW1_PSAM[6];                // ��ֵ��д��      ��д��           ��д�������俨��    ��д��1�����俨��

		BYTE ACC_RW2_ISAM[6];                // Token��д��                                          ��д��2
		BYTE ACC_RW2_PSAM[6];                // Token��д��                                          ��д��2
		BYTE ECT_RW2_ISAM[6];                // Token��д��                                          ��д��2��ĸ����
		BYTE ECT_RW2_PSAM[6];                // Token��д��                                          ��д��2�����俨��

		BYTE ACC_RW3_ISAM[6];                // Card��д��                                           ��д��3
		BYTE ACC_RW3_PSAM[6];                // Card��д��                                           ��д��3
		BYTE ECT_RW3_ISAM[6];                // Card��д��                                           ��д��3��ĸ����
		BYTE ECT_RW3_PSAM[6];                // Card��д��                                           ��д��3�����俨��

		BYTE RW_DEVICE_ID[6];               // �ⲿ��д���豸ID
		BYTE TW_DEVICE_ID[6];               // �ڲ���д���豸ID
		BYTE PRT_DEVICE_ID[6];              // ��ӡ���豸ID
		BYTE TH_DEVICE_ID[6];               // ��Ʊ���豸ID

		_device_info() {
			memset(ACC_RW1_ISAM,0,6);
			memset(ACC_RW1_PSAM,0,6);
			memset(ECT_RW1_ISAM,0,6);
			memset(ECT_RW1_PSAM,0,6);

			memset(ACC_RW2_ISAM,0,6);
			memset(ACC_RW2_PSAM,0,6);
			memset(ECT_RW2_ISAM,0,6);
			memset(ECT_RW2_PSAM,0,6);

			memset(ACC_RW3_ISAM,0,6);
			memset(ACC_RW3_PSAM,0,6);
			memset(ECT_RW3_ISAM,0,6);
			memset(ECT_RW3_PSAM,0,6);

			memset(RW_DEVICE_ID, 0,6);
			memset(TW_DEVICE_ID, 0,6);
			memset(PRT_DEVICE_ID,0,6);
			memset(TH_DEVICE_ID, 0,6);
		}
	} DEVICE_INFO;

	BOOL Internalize(CString dataPath, CString backupPath);     // ���ڻ�

	void GetDeviceInfo(DEVICE_INFO&);                           // ��ȡ�豸��Ϣ
	//RW 1
	void SetACC_RW1_ISAM(LPBYTE);            // ����RW1��ACC ISAM����
	void SetACC_RW1_PSAM(LPBYTE);            // ����RW1��ACC PSAM����
	void SetETC_RW1_ISAM(LPBYTE);            // ����RW1��ACC ISAM����
	void SetETC_RW1_PSAM(LPBYTE);            // ����RW1��ACC PSAM����
	//RW 2
	void SetACC_RW2_ISAM(LPBYTE);            // ����RW2��ACC ISAM����
	void SetACC_RW2_PSAM(LPBYTE);            // ����RW2��ACC PSAM����
	void SetETC_RW2_ISAM(LPBYTE);            // ����RW2��ACC ISAM����
	void SetETC_RW2_PSAM(LPBYTE);            // ����RW2��ACC PSAM����
	//RW 3
	void SetACC_RW3_ISAM(LPBYTE);            // ����RW3��ACC ISAM����
	void SetACC_RW3_PSAM(LPBYTE);            // ����RW3��ACC PSAM����
	void SetETC_RW3_ISAM(LPBYTE);            // ����RW3��ACC ISAM����
	void SetETC_RW3_PSAM(LPBYTE);            // ����RW3��ACC PSAM����

	void SetRWId(LPBYTE);					// �����ⲿ��д���豸ID
	void SetTWId(LPBYTE);					// �����ڲ���д���豸ID
	void SetPRTId(LPBYTE);					// ���ô�ӡ���豸ID
	void SetTHId(LPBYTE);					// ���ó�Ʊ���豸ID

	static CCTLOGDeviceInfo&  GetInstance()
	{
		return theInstance;
	}

private:
	enum {
		DEVICE_INFO_LEN = 96,	            ///< �ļ�����
	};

	// �ֶ�����λ��
	enum {
		ACC_RW1_ISAM_POS = 0,                 // ��ֵ��д��
		ACC_RW1_PSAM_POS = 6,                 // ��ֵ��д��
		ECT_RW1_ISAM_POS = 12,                // ��ֵ��д��
		ECT_RW1_PSAM_POS = 18,                // ��ֵ��д��

		ACC_RW2_ISAM_POS = 24,                // Token��д��
		ACC_RW2_PSAM_POS = 30,                // Token��д��
		ECT_RW2_ISAM_POS = 36,                // Token��д��
		ECT_RW2_PSAM_POS = 42,                // Token��д��

		ACC_RW3_ISAM_POS = 48,                // Card��д��
		ACC_RW3_PSAM_POS = 54,                // Card��д��
		ECT_RW3_ISAM_POS = 60,                // Card��д��
		ECT_RW3_PSAM_POS = 66,                // Card��д��

		RW_DEVICE_ID_POS  = 72,               // �ⲿ��д���豸ID,
		TW_DEVICE_ID_POS  = 78,               // �ڲ���д���豸ID,
		PRT_DEVICE_ID_POS = 84,               // ��ӡ���豸ID,
		TH_DEVICE_ID_POS  = 90,               // ��Ʊ���豸ID,
	};

	DEVICE_INFO    m_deviceInfo;                    // �豸��Ϣ
	CDXFile         *m_file;                         // �ļ���ȡ������
	char           m_fileData[DEVICE_INFO_LEN];     // ��ǰ�ļ�����

	static  CCTLOGDeviceInfo theInstance;
	CCTLOGDeviceInfo(void);
	~CCTLOGDeviceInfo();
	CCTLOGDeviceInfo(const CCTLOGDeviceInfo&);
	CCTLOGDeviceInfo& operator=(const CCTLOGDeviceInfo&);
};
