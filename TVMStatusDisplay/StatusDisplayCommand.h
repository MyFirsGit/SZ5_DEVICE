#pragma once
#include "StatusDisplayExp.h"
#include "Command.h"
#include "StatusDisplayDef.h"
#include "StatusDisplayException.h"
#include "SerialPort.h"
#include "LEDApi.h"

/**
 @brief  ����ģʽ����
*/
class STATUS_DISPLAY_API CStatusDisplayCommand : public CCommand
{
public:
	CStatusDisplayCommand(void);   
    ~CStatusDisplayCommand(void);
	long ExecuteCommand(void){	return 0;}

public:
	long Open(int nPort, int nBardRet);
	long Close();
	long Init(char** strExtenModeTextAray, int nCount);	// ������չģʽ����
	long GetStatus();
	long ShowExtenModeText(int extenModeIndex);			// ��ʾ��չģʽ����
	long ShowFixedModeText(int fixedModeIndex);			// ��ʾ�̶�ģʽ���ԣ�LED������д���ģ�
	long DownloadTextToLed(int SerialNo,CString szCn,CString szEn,int color);			// ֱ����ʾ�ı����ݣ���Ӣ��˫����ʾ

private:
	CSerialPort m_serialPort;
	// д��չģʽ
	BOOL WriteXMode(BYTE nItem, BYTE* BCT, BYTE len);
	// ��ʾģʽ����
	BOOL SetWorkMode(BYTE nWM);
};









