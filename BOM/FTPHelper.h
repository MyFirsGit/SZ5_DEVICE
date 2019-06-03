#pragma once
#include <vector>

/**
@brief  FTP控制管理类
*/

#define  theFTP_HELPER CFTPHelper::GetInstance()

class CFTPHelper
{
public:

	
	vector<CString> m_vecDownloadFiles;	// 下载文件
	CString m_localPath;				// 下载路径

	vector<CString> m_vecUploadFiles;	// 上传文件
	CString m_remotePath;				// 上传路径

	void DownloadParam(vector<CString>);			// 下载参数文件	
	void DownloadProgm(CString,CString);			        // 下载程序文件	
	long UploadFiles(vector<CString>,CString);				// 上传文件

	void DownloadFiles(vector<pair<CString,CString>>,bool deleteOldFolder=true);

	bool RepairException();
	
	// 请勿直接调用以下4个函数
	bool FTPConnect();
	bool FTPClose();
	bool FTPDownloadFile(CString,CString);
	bool FTPUploadFile(CString,CString);


	vector<CString> GetUploadFiles();

    static CFTPHelper&  GetInstance(){
        return theInstance;
    }

private:

    // 构造,析构函数
    CFTPHelper(void);
    ~CFTPHelper();
    CFTPHelper(const CFTPHelper&);
    CFTPHelper& operator=(const CFTPHelper&);

    static  CFTPHelper theInstance;   // 该类的唯一接口
};
