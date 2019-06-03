#pragma once 
#include "BHChangeCommand.h"
// ֽ������ģ��Ӳ���ӿ�
class CBHChangeHelper
{
	enum { 
		REJECT	= 3,  // REJECT �ܾ���������0,
		RETRY	= 3,  // RETRY ������������RETRY 
		Lenmax	= 175, // Lenmax ��󳤶�  175mm 
		Lenmin	= 110, // ��������Lenmin ��С����  110mm
		Thick	= 10  // Thick  ��� 10 ���� 0.10����
	}; 

public:
	CBHChangeHelper();
	~CBHChangeHelper();
public:
	long initialize();													// ��ʼ��
	long GetStatus(DEVICE_STATUS_INFO& device_info);					// ȡֽ������ģ��״̬
	long SendOutBill(int nCountBoxA,int nCountBoxB);					// ֽ���ͳ������͸��˿ͣ���ͬ�����ã�
	void SendOutBill(CService* pService,int nCountBoxA,int nCountBoxB);	// ֽ���ͳ������͸��˿ͣ����첽���ã�

	long Reset();														// ����ֽ������ģ��
	long Repare();														// �޸�ֽ������ģ��

	long UnInitialize();												// ����ʼ��
	
protected:
	// Ԥ����Ӳ���Լ죨��ʱ������,δʵ�֣�
	//long CountBill(int nCountBoxA,int nCountBoxB);					// �������ֽ������
	//long TransportationBill();										// ������������ֽ�Ұ��ͳ�ֽ������ģ��
	//long RetrievalBill();												// ������������ֽ�Ұ��͵�ֽ������ģ��󷽣������䣩
	//long Abort();														// ��ֹ����(��ʱ����)
private:
	void OpenBHChangeComPort();											// ����ֽ������ģ��
	void CloseBHChangeComPort();										// �ر�ֽ������ģ��
	bool IsHardWareException(long lErrorCode);							// �Ƿ�Ϊֽ������Ӳ���쳣
};
