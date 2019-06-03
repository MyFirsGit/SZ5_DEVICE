#ifdef BHLIB_EXPORTS
#define DLLCLASS __declspec(dllexport)
#else
#define DLLCLASS __declspec(dllimport)
#endif

//BH��־��ӡ�ඨ��

class DLLCLASS CBHLog
{
public:
	CBHLog();
	virtual ~CBHLog();

    void Initialize(const char* tracePath);
	void OpenLog();
	void printf(const char *fmt, ... );
	void print(const char *text);
	void CloseLog();
    void ClearLog();
    void MakeFileName(char* fileName);
    void MakeFullName(char* strFullName);
  
private:
	HANDLE			 m_hFile;		// ��־�ļ����
	CRITICAL_SECTION m_hLock;		// lock for thread-safe access
	char			 m_strFileName[MAX_PATH];   // ��־�ļ���
    char			 m_strTracePath[MAX_PATH];  // ��־���Ŀ¼
    DWORD			 m_dwLineCount;
};

