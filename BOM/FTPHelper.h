#pragma once
#include <vector>

/**
@brief  FTP���ƹ�����
*/

#define  theFTP_HELPER CFTPHelper::GetInstance()

class CFTPHelper
{
public:

	
	vector<CString> m_vecDownloadFiles;	// �����ļ�
	CString m_localPath;				// ����·��

	vector<CString> m_vecUploadFiles;	// �ϴ��ļ�
	CString m_remotePath;				// �ϴ�·��

	void DownloadParam(vector<CString>);			// ���ز����ļ�	
	void DownloadProgm(CString,CString);			        // ���س����ļ�	
	long UploadFiles(vector<CString>,CString);				// �ϴ��ļ�

	void DownloadFiles(vector<pair<CString,CString>>,bool deleteOldFolder=true);

	bool RepairException();
	
	// ����ֱ�ӵ�������4������
	bool FTPConnect();
	bool FTPClose();
	bool FTPDownloadFile(CString,CString);
	bool FTPUploadFile(CString,CString);


	vector<CString> GetUploadFiles();

    static CFTPHelper&  GetInstance(){
        return theInstance;
    }

private:

    // ����,��������
    CFTPHelper(void);
    ~CFTPHelper();
    CFTPHelper(const CFTPHelper&);
    CFTPHelper& operator=(const CFTPHelper&);

    static  CFTPHelper theInstance;   // �����Ψһ�ӿ�
};
