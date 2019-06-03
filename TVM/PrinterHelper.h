#pragma once
#include "ServiceHelper.h"
#include "PRT.h"

/**
@brief  ��ӡ��������
*/

class CPrinterHelper
{
public:

	CPrinterHelper();
	~CPrinterHelper();

public:
	bool IsPrinterHasException();				// �жϴ�ӡ���Ƿ����쳣
	long PrinterConnect();						// ���Ӵ�ӡ��
	long PrinterClose();						// �رմ�ӡ��
	long CheckStatus();							// ����ӡ��״̬���ɼ��ȱֽ����ֽ���е�λ���쳣��ѹֽ��λ���쳣��RAM�洢���쳣��
	long GetPrinterID(BYTE*);					// ��ȡ��ӡ��Ӳ�����кţ�Ԥ����
	bool Repare();								//�޸���ӡ���쳣

	long PrintReceiptByBmp (CStringArray* sArray, bool bIsSaveFile,bool bIsPrintLogo = true);  // ��ƾ���ı��ϳ�ͼƬ����ӡ
	long PrintReceiptByText(CStringArray* sArray,  bool bIsSaveFile,bool bIsPrintLogo = true);   // ���ı���ʽ��ӡƾ������֧���������ģ�
	long PrintLastUnprintReceipt();				// ��ӡ���ӡ�����ϲ�����δ��ӡ��ƾ��

	BOOL isNoPaper();	//wu zhi
	BOOL isLessPaper();	//shao zhi
	BOOL isRamError();//ram shi fou zhengchang		//֣�ݲ�֧��
	BOOL isKnifePosError();//qie zhi dao wei zhi shi fou zheng que		//֣�ݲ�֧��
	BOOL isBlackSplitPosNotUnderKnife();//hei tiao wei zhi shi fou zai qie dao xia fang ֣�ݲ�֧��
	BOOL isStickPosError();//ya zhi bang wei zhi shi fou da kai		//֣�ݲ�֧��




	/*
	*  Software use
	*/
	RECEIPT_PRINT_TYPE GetPrintTypeFromFileName(const CString&);											// ͨ����ӡ�ļ������ƽ�����������
public:
	void SaveToPrintHistoryFile(CStringArray* sArray);                                                       // ��ӡ�ı���δ��ӡĿ¼
private:
	long GetPrinterStatus();
	void PrinterToFile(CStringArray* sArray);                                                               // ��ӡ�ı��������ļ�

	CString SaveToUnPrintDir(CStringArray* sArray);															// �����ӡ�ı�Ϊδ��ӡ״̬
	void MovePrintFileToHistory(CString strFilePathName);													// �ƶ���ӡ��¼����ӡ��ʷ
	CString m_strNeedPrintPath;																				// ��Ҫ��ӡ���ļ�·��

	CString GetNeedPrintFileName();																			// ��ȡ��Ҫ���ɵ��ļ�������

	BOOL PrintLastUnprintFromPath(CString strFilePath);														// ��ӡָ��Ŀ¼�µ�δ��ӡ���ļ�

	
};
