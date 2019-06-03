#pragma once

#include "Xfile.h"
#include "afxtempl.h"
#include "afxmt.h"
#include "coreexp.h"

/**
@brief ��־trace
*/

#define CLOG_FILE_EXT_CHAR							_T(".")
#define CLOG_FILE_KIND								_T("log")

class CLogThread;

class CORE_API CLog
{
public:
    CLog(CString fileNamePrefix);  // ���캯��
    virtual ~CLog(void);                                     // ��������

    static void Initialize(CString path);                              // ����Ŀ¼

protected:
    CString GetCurrentDateFileName();                                          // ȡ�ļ���
    //void OpenFile();                                                // ���ļ�
    //void CloseFile();                                               // �ر��ļ�
    void WriteData(CString& data);                                   // д��־
	void CloseLog();												 // �ر���־
private:

	static CLogThread* s_LogThread;
    static CString s_path;                                          // TraceĿ¼
    CString m_fileNamePrefix;                                       // �ļ���ǰ׺�����������ַ�"_"��
    CString m_fileName;                                             // �ļ�ȫ��
    CXFile* m_file;                                                 // �ļ�����
	void WriteDataImp(CString& data);
friend class CLogThread;
};
