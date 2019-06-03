#pragma once
#include "CParameterCommandExp.h"
#include "Xfile.h"
#include "cparametermaster.h"
/**
@brief   ECT����������
*/
class PARAMETERCOMMAND_DECLSPEC CEctParamMaster : public CParameterMaster
{
public:
	bool Internalize(bool readFlag = true,CString dataPath=_T(""),CString backupPath=_T(""));         			// ���ڻ�

	bool ReadMasterData(VARIABLE_DATA&, bool);                   // ��ȡ������Ϣ  
	bool UpdateMasterFile(const char *, ULONG, bool);           // ���²�����Ϣ

	DWORD GetCurMasterVer();                                    // �õ���ǰ�汾��Ϣ
	DWORD GetFutMasterVer();                                    // �õ�δ���汾��Ϣ
	_DATE GetCurStartDate();                                    // �õ���ǰ��Ч������Ϣ
	_DATE GetFutStartDate();                                    // �õ�δ����Ч������Ϣ
	PARAM_VERSION_TYPE_CODE GetCurVerType();					// ȡ�õ�ǰ�����汾����
	PARAM_VERSION_TYPE_CODE GetFutVerType();					// ȡ��δ�������汾����

	bool CheckDataInfo(const VARIABLE_DATA&);					// ������������ȷ��

protected:
	// ECT������ͷ�ṹ��
	typedef struct _ect_package_head {
		BYTE    formatVersion;			// ����ʽ�汾��
		BYTE    dataOrigin;				// ������Դ��
		DWORD   packageLength;			// ���ݰ�����
		WORD	dataTypeCode;			// �������ʹ���	
		DWORD   versionCode;			// �汾��
		_DATE_TIME  createDateTime;		// �汾����ʱ��
		_DATE   startDate;				// ��Ч����
		WORD    sectionCount;			// ���ݷֶ�����
		BYTE    reverse[3];				// Ԥ���ֶ�

		void internalize(){ 
			formatVersion       = 0;
			dataOrigin          = 0;
			packageLength       = 0;
			dataTypeCode  = 0;
			versionCode     = 0;
			sectionCount  = 0;            
			memset(reverse,0,3);
		}
		// �ṹ����ڻ�
		_ect_package_head(){
			internalize();
		}

	}ECT_PACKAGE_HEAD;

	typedef struct _section_info {
		DWORD startPosition;				// �ֶ���ʼƫ����
		DWORD recordCount;					// �ֶνṹ���¼��
		_section_info(){
			startPosition = 0;
			recordCount = 0; 
		}
	}SECTION_INFO;

	enum{		
		START_POSITION_LENS = 8,		   // ����ƫ������¼�峤��
		PACKAGE_HEAD_LENS   = 28           // �����ļ�ͷ����
	};	
	ECT_PACKAGE_HEAD m_packageHead;					// ���İ�ͷ

	// ���죬��������
	CEctParamMaster(void);
	~CEctParamMaster();
	CEctParamMaster(const CEctParamMaster&);
	CEctParamMaster& operator=(const CEctParamMaster&);

	void GetParamFileNameSub(CString);				// ��ȡ�����ļ���
private:
	// ��Ա˽�б���
	DWORD   m_curVersion;                           // current�汾
	DWORD   m_futVersion;                           // future�汾
	_DATE   m_curStartDate;							// current�汾��Ч����
	_DATE	m_futStartDate;							// future�汾��Ч����
	PARAM_VERSION_TYPE_CODE m_curVersionType;		// current�汾����
	PARAM_VERSION_TYPE_CODE m_futVersionType;		// future�汾����
	CXFile  *m_curFile;                             // ָ��current�汾�ļ�
	CXFile  *m_futFile;                             // ָ��future�汾�ļ� 
	CString m_dataPath;								// ���ļ�·��
	CString m_backupPath;							// �����ļ�·��
	bool    m_flag;                                 // �ļ�ѡ���־ true:��ǰ�汾�ļ�   false:δ���汾�ļ�
	CString m_curfileName;							// ��ǰ�����ļ���
	CString m_futfileName;							// δ�������ļ���

	// ��Ա˽�к���    
	bool ReadFile();                                 // ��ȡ�ļ�
	void ParseHead(LPBYTE);							 // ������ǰ�ļ��İ�ͷ����

	virtual void ParseSectionInfo(LPBYTE)= 0;		 // �����ֶ���Ϣ
	virtual bool CheckBodyInfo(LPBYTE) = 0;			 // ����������
	virtual void ParseBody(LPBYTE) = 0;              // ������������
	virtual void GetParamFileName() = 0;			 // ��ȡ�����ļ���

	void SetCurVerType(CString);					// ���õ�ǰ�����汾����
	void SetFutVerType(CString);					// ����δ�������汾����

};
