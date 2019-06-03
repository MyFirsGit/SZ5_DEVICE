#pragma once
#include "StatusDisplayExp.h"
#include "Command.h"
#include "StatusDisplayDef.h"
#include "StatusDisplayException.h"
#include "SerialPort.h"
#include "LEDApi.h"

/**
 @brief  命令模式父类
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
	long Init(char** strExtenModeTextAray, int nCount);	// 设置扩展模式文言
	long GetStatus();
	long ShowExtenModeText(int extenModeIndex);			// 显示扩展模式文言
	long ShowFixedModeText(int fixedModeIndex);			// 显示固定模式文言（LED板子里写死的）
	long DownloadTextToLed(int SerialNo,CString szCn,CString szEn,int color);			// 直接显示文本内容，中英文双行显示

private:
	CSerialPort m_serialPort;
	// 写扩展模式
	BOOL WriteXMode(BYTE nItem, BYTE* BCT, BYTE len);
	// 显示模式文言
	BOOL SetWorkMode(BYTE nWM);
};









