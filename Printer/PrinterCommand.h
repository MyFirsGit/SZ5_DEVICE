#pragma once
#include "PrinterExp.h"
#include "Command.h"

/**
 @brief  串口打印机命令--打开串口
*/
class PRINTER_API CPrinterConnect : public CCommand
{
public:
	CPrinterConnect(int nPort=1,int nBaud=9600 , int nSize = 8);   
    ~CPrinterConnect(void);
public:
	long ExecuteCommand(void);  //执行命令
private:
	int m_nPort;                       //串口端口号
    int m_nBaud;                       //波特率
	int m_nSize;                       //字长
	int ConnectFunction();             //打开串口
	int CheckBaudValid(int nBaud) ;   //检查波特率是否合法

};

/**
 @brief  串口打印机命令--打印内容
*/
class PRINTER_API CPrinterPrint : public CCommand
{
public:
	CPrinterPrint(CStringArray* sArray , bool bOpenBox = false);
	~CPrinterPrint(void);
public:
	long ExecuteCommand(void);           //执行命令
private:
	CStringArray* m_sArrPrinter;         //打印内容
	bool          m_bOpenBox;            //是否开钱箱
	int PrintFunction();                 //打印
};

/**
@brief   串口打印机命令--读取状态
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
@brief   串口打印机命令--读取打印机信息
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
 @brief   串口打印机命令--关闭串口
*/
class PRINTER_API CPrinterClose : public CCommand
{
public:
	CPrinterClose(void);
	~CPrinterClose(void);

public:
   long ExecuteCommand(void);          //执行命令

private:
   int CloseFunction();//关闭串口
};

/**
 @brief    串口打印机命令--打开钱箱
*/
class  PRINTER_API CPrinterOpenBox : public CCommand
{
public:
	CPrinterOpenBox(void);        
	~CPrinterOpenBox(void);

public:
	long ExecuteCommand(void);   //执行命令

private:
	int OpenBoxFunction();       //打开钱箱
};










