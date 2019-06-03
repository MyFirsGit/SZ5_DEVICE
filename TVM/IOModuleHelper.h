#pragma once
#include "stdafx.h"
#include "ServiceHelper.h"
#include "DIOMessages.h"
#include "IOdef.h"

/**
@brief  IO������
*/

class CIOModuleHelper
{
public:

	CIOModuleHelper();                              // ���캯��
	~CIOModuleHelper();                                              // ��������

	void OpenIO();                                                   // ��IOͨ��
	void CloseIO();                                                  // �ر�IOͨ��
	void InitializeIO();											// ��ʼ��IO
	void GetIOStatus(IO_RSP_STATUS_INFO&);	                      	// ��ȡIO״̬
	void OpenCHIndicator();											// ����Ͷ�ҿ�ָʾ��
	void CloseCHIndicator();										// �ر�Ͷ�ҿ�ָʾ��
	void OpenBHIndicator();											// ����ֽ��Ͷ�ҿ�ָʾ��
	void CloseBHIndicator();										// �ر�ֽ��Ͷ�ҿ�ָʾ��

	void OpenReturnCupIndicator();						            // ����ȡƱ����ָʾ��
	void CloseReturnCupIndicator();			             			// Ϩ��ȡƱ����ָʾ��
	void OpenReturnLight();									        // ����ȡƱ����������
	void CloseReturnLight();									    // Ϩ��ȡƱ����������
	void OpenAlarm();												// �򿪷�������
	void CloseAlarm();												// �رշ�������
	void OpenBKCardIndicator();										// ������ֵ��ָʾ��
	void CloseBKCardIndicator();									// �رմ�ֵ��ָʾ��
	void OpenOperatorLight();									    // ��������������
	void CloseOperatorLight();									    // Ϩ�����������

	void ReturnRechargeCard();										// ��ֵ������
	// �������
	void Initialize();												// �򿪲���ʼ��IO
	void RepareIO();												// �޸�IO�쳣
	void OpenAll();
	void CloseAll();


	//���õĽӿ�
	void OpenCoinShutter();											// ��Ͷ�ҿ�
	void CloseCoinShutter();										// �ر�Ͷ�ҿ�

	void OpenHelpButtonLight();										// ����Ԯ��ť��
	void CloseHelpButtonLight();									// �ر���Ԯ��ť��
	void OpenReturnCupIndicatorFlash();								// ����ȡƱ����ָʾ������˸��ʽ

	void AbortCheckCardStandby(){}
	void CheckCardStandby(CService*,int){};
	void CloseChargeIndicator(){};
	void OpenChargeIndicator(){}
};
