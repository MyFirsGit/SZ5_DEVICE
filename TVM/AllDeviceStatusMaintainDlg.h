#pragma once
#include "OperationDlg.h"

#include "afxwin.h"
#include "AllDeviceStatusSvc.h"
/**
@brief Ӳ���Լ������豸״̬
*/

class CAllDeviceStatusMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CAllDeviceStatusMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CAllDeviceStatusMaintainDlg(CService* pService);      // ���캯��
	~CAllDeviceStatusMaintainDlg();                       // ��������

	enum { IDD = IDD_ALL_DEVICE_STATUS };       // �Ի���ID

	void ShowInitRessult(int index);            // ˢ�³��ڻ����
	void InitTitle();
private:
	void OnDraw(CDC* pDC);   // ������ƺ���
	CString GetResultText(CAllDeviceStatusSvc::INIT_RESULT result);   // ��ȡ���ڻ��������
	COLORREF GetResultColor(CAllDeviceStatusSvc::INIT_RESULT result); // ��ȡ���ڻ������ɫ
};