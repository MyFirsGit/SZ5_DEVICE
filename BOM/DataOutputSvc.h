#pragma once
#include "BOMForeService.h"

/**
@brief    数据导出
*/
class CDataOutputSvc: public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	typedef struct _file_struct{
		int iParamID;									// 参数ID
		CString sParamPath;								// 参数目录/文件名
		bool isSucceed;									// 导入是否成功
		_file_struct(){
			iParamID = 0;
			isSucceed = false;
		}
	}FILE_STRUCT;

	CDataOutputSvc();
	~CDataOutputSvc();

	bool ExportParameter(DATA_TYPE_CODE);				// 向外部媒体导出数据
	CString GetFileKeyName(int ifileNum);				// 获得存储文件设备ID中文件名key名
	bool AuthUSB();										// 认证USB设备
	long PrepareUploadLogFile(CString& fileName);		// 创建日志上传文件

	CString m_DeviceID;									// 设备编码

private:
	typedef enum _dialog_group {
		DIALOG_GROUP_DATAOUTPUT_SELECT = 1,           
	} DIALOG_GROUP;
};