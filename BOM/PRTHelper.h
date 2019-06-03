#pragma once

#include "ServiceHelper.h"
#include "PrinterCommand.h"

/**
@brief  ��ӡ��������
*/

class PRTHelper :
	public CServiceHelper
{
public:

	PRTHelper(CService& service);
	~PRTHelper();

	bool IsPrinterHasException();												 // �жϴ�ӡ���Ƿ����쳣

	// ��ӡ�����
	void CheckDeviceExchange();													  // ����ӡ���Ƿ����
	long PrinterConnect(int nPort, int nBaud);                                    // ���Ӵ�ӡ��
	void PrinterPrint(CStringArray* sArray, bool bOpenBox = false);               // ��ӡ����
	void PrinterOpenBox();                                                        // ��Ǯ��
	void PrinterClose();                                                          // �رմ�ӡ��
	void CheckStatus();															  // ����ӡ��״̬
	long GetPrinterID(BYTE*);													  // ��ȡ��ӡ��ID
	bool Repare();																  // �޸���ӡ���쳣
	// 20170315������ҵ�����󣬳�ֵ����ӡˮ�����������ļ���δ��ӡĿ¼�£�������ҵ�������ӵ��ݴ�ӡ��ť����ӡ���һ�ʳ�ֵ���ݡ�
	void SaveToChargePrintFile(CStringArray* sArray, bool bOpenBox);				// ��ӡ���ݱ��浽��ֵ����Ŀ¼��,����Ǯ��
	void PrintLastChargeReceipt();													// ��ӡ��ֵ����Ŀ¼�±�������һ�ʳ�ֵ����
	CString m_lastChargePrintFile;													// ���һ�ʳ�ֵ�����ļ�����
private:
	void PrinterToFile(CStringArray* sArray,bool bOpenBox = false);				  // ��ӡ���ݵ��ļ�
	void PrintUnPrintFile();
	void SaveToUnPrintFile(CStringArray* sArray,bool bOpenBox = false);
	//void PrintLastPrintFile();
	//void SaveToLastPrintFile(CStringArray* sArray);
	//CString lastPrintFileFullPath;
};
