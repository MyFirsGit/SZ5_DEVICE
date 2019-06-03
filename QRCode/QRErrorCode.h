
typedef enum _rqcode_error_code
{
	QRCODE_OK = 0,						// 执行成功
	QRCODE_FILENAME_ERROR = 101,		// 文件名错误
	QRCODE_CREATE_FILE_ERROR = 102,		// 创建文件失败
	QRCODE_BUFFER_LENGTH_LESS,			// 接受数据的缓冲区长度太小
	QRCODE_VERSION_ERROR,				// 版本超出范围
	QRCODE_OUT_OF_IMAGE,				// 对齐方式查找超过了图像
	QRCODE_INVALID_NUMBER,				// 无效的数据检测模式
	QRCODE_VERSION_RECOVERY,			// 版本信息纠错失败
	QRCODE_FORMATE_RECOVERY,			// 格式信息纠错失败
	QRCODE_OUT_OF_SAMPLING,				// 超出图像采样网格
	QRCODE_NOT_SUPPORT_CHARACTERSET,	// 不支持的字符集
									
};