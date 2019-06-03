#pragma once
#include "PrinterExp.h"
#include "Command.h"

/**
 @brief  ���ڴ�ӡ������--�򿪴���
*/
class PRINTER_API CPrinterConnect : public CCommand
{
public:
	CPrinterConnect(int nPort=1,int nBaud=9600 , int nSize = 8);   
    ~CPrinterConnect(void);
public:
	long ExecuteCommand(void);  //ִ������
private:
	int m_nPort;                       //���ڶ˿ں�
    int m_nBaud;                       //������
	int m_nSize;                       //�ֳ�
	int ConnectFunction();             //�򿪴���
	int CheckBaudValid(int nBaud) ;   //��鲨�����Ƿ�Ϸ�

};

/**
 @brief  ���ڴ�ӡ������--��ӡ����
*/
class PRINTER_API CPrinterPrint : public CCommand
{
public:
	CPrinterPrint(CStringArray* sArray , bool bOpenBox = false);
	~CPrinterPrint(void);
public:
	long ExecuteCommand(void);           //ִ������
private:
	CStringArray* m_sArrPrinter;         //��ӡ����
	bool          m_bOpenBox;            //�Ƿ�Ǯ��
	int PrintFunction();                 //��ӡ
};

/**
@brief   ���ڴ�ӡ������--��ȡ״̬
*/
class PRINTER_API CPrinterGetStatus:public CCommand
{
public:
	CPrinterGetStatus();
	~CPrinterGetStatus();
protected:
	long ExecuteCommand();
private:
	long GetStatus();
};


/**
@brief   ���ڴ�ӡ������--��ȡ��ӡ����Ϣ
*/
class PRINTER_API CPrinterGetInfo:public CCommand
{
public:
	CPrinterGetInfo();
	~CPrinterGetInfo();
	void GetRWResponse(BYTE*);
protected:	
	long ExecuteCommand();
private:
	long GetPrinterId();
	BYTE m_printerId[6];
};


/**
 @brief   ���ڴ�ӡ������--�رմ���
*/
class PRINTER_API CPrinterClose : public CCommand
{
public:
	CPrinterClose(void);
	~CPrinterClose(void);

public:
   long ExecuteCommand(void);          //ִ������

private:
   int CloseFunction();//�رմ���
};

/**
 @brief    ���ڴ�ӡ������--��Ǯ��
*/
class  PRINTER_API CPrinterOpenBox : public CCommand
{
public:
	CPrinterOpenBox(void);        
	~CPrinterOpenBox(void);

public:
	long ExecuteCommand(void);   //ִ������

private:
	int OpenBoxFunction();       //��Ǯ��
};










