#pragma once
#include "TVMForeService.h"

#define  _MAX_ROW  15     //定义最大行数
#define  _MAX_COL   2     //定义最大列数

/**
@brief    数据导出
*/
class CDataOutputSvc: public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CDataOutputSvc();
	~CDataOutputSvc();
	//定义错误类型
	typedef enum _long_error_code
	{
		ERROR_CODE_NORMAL = 1,
		ERROR_CODE_CREATE_FILE_FAILED,
		ERROR_CODE_OTHER_ERROR,
	}ERROR_CODE;
	//画面
	typedef enum _dialog_group {
		DIALOG_GROUP_DATAOUTPUT_SELECT = 1,           
	} DIALOG_GROUP;

protected:
	bool AuthUSB();										// 认证USB设备
	long PrepareUploadLogFile(CString& fileName);		// 创建日志上传文件
	long ExportParameter(DATA_TYPE_CODE);				// 向外部媒体导出数据
	CString GetFileKeyName(int ifileNum);				// 获得存储文件设备ID中文件名key名
	CString m_DeviceID;									// 设备编码
	bool IsUSBAuthed();									// USB是否认证通过？
private:
	DATA_TYPE_CODE  m_DataType;
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);	// 处理 [Enter] 键按下消息
	LRESULT OnKeyboardF3(WPARAM , LPARAM );							// 处理 [F3/Ctrl] 键按下消息
	void OnStart();
	bool m_bIsAuthPass;									// U盘认证是否通过？
};