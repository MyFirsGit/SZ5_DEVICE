#pragma  once
#include <afx.h>
#include <vector>
#include "afxtempl.h"
#include "Xfile.h"
#include "CommonExp.h"
 
using namespace std;

#define theGlobalParameter  CGlobalParameter::GetInstance()

/**
 @brief      ȫ�ֲ�����
*/
class CGlobalParameter
{
private:
	CGlobalParameter(void);
	~CGlobalParameter(void);

public:
	typedef struct exception_detail {
		unsigned long lID ;
		unsigned long lDetail_ID;
		unsigned long lMTC;
		unsigned short lEc;
		char*         sErr;
		char*         sDeal;
		unsigned long sErrLen;
		unsigned long sDealLen;
		exception_detail(){
           lID = 0;
           lDetail_ID = 0;
           lMTC = 0;
           lEc = 0;
           sErr = NULL;
           sDeal = NULL;
		   sErrLen= 0;
           sDealLen = 0;
		}
		exception_detail(const exception_detail& that){
			lID = that.lID;
			lDetail_ID = that.lDetail_ID;
			lMTC = that.lMTC;
			lEc = that.lEc;
			if (that.sErrLen > 0 && that.sErr != NULL ) {
				sErr = new char[that.sErrLen];
				memcpy(sErr, that.sErr, that.sErrLen);
			}
			if (that.sDealLen > 0 && that.sDeal != NULL) {
				sDeal = new char[that.sDealLen];
				memcpy(sDeal, that.sDeal, that.sDealLen);
			}
			sErrLen= that.sErrLen;
			sDealLen = that.sDealLen;
		}

		~exception_detail(){
			if (NULL != sErr) {
				delete [] sErr;
				sErr = NULL;
			}
			if (NULL != sDeal) {
				delete [] sDeal;
				sDeal= NULL;
			}
		}
		exception_detail& exception_detail::operator=(const exception_detail& exData){
			lID = exData.lID;
			lDetail_ID = exData.lDetail_ID;
			lMTC = exData.lMTC;
			lEc = exData.lEc;
			sErrLen= exData.sErrLen;
			sDealLen = exData.sDealLen;
			if (exData.sErrLen > 0 && exData.sErr != NULL) {
				sErr = new char[exData.sErrLen];
				memset(sErr, 0, exData.sErrLen);
				memcpy(sErr, exData.sErr, exData.sErrLen);
			}
			if (exData.sDealLen > 0 && exData.sDeal != NULL) {
				sDeal = new char[exData.sDealLen];
				memset(sDeal, 0, exData.sDealLen);
				memcpy(sDeal, exData.sDeal, exData.sDealLen);
			}
			return *this;
		}
	} EXCEPTION_DETAIL, *LPEXCEPTION_DETAIL;

	typedef CMap<CString, LPCTSTR, LPEXCEPTION_DETAIL, LPEXCEPTION_DETAIL> EXCEPTION_MAP;
	    
    bool SetBusiDateSwitchTime(unsigned char Hour = 0, unsigned char Minute = 0,unsigned char Second = 0);    //����Ӫ�����л�ʱ��
	bool GetBusiDateSwitchTime(unsigned char& Hour, unsigned char& Minute,unsigned char& Second);			  //�õ�Ӫ�����л�ʱ��

	

	void IncRecoverTimes();                                                     //�ļ��쳣�ָ�������1
	void UpdateVfareRecoverTimes(int);											//����Ʊ�۱�ǰ�ָ�����(�ۼ�)
	unsigned long GetCurVfareRecoverTimes();                                    //�õ�Ʊ�۱�ǰ�ָ�����
	unsigned long GetFileMaxRecoverTimes();                                     //�õ��ļ��������ָ�����	
	bool OverMaxRecoverTimes();                                                 //�ж��Ƿ񳬹����ָ�����
	bool OverMaxVfareRecoverTimes();                                            //�ж��Ƿ񳬹�Ʊ�۱����ָ�����
	unsigned int GetExceptionByID(EXCEPTION_DETAIL& exMsg, long lId, long lDetailID );  // ���ָ���쳣��ϸ��Ϣ
	unsigned int GetExceptionDetail();											// ��ȡȫ���쳣��Ϣ
    void Release();                                                             //�ͷ���Դ

	void InitRecoverTimes();                                                    //���ڻ��ļ��쳣�ָ�����
	void InitVfareRecoverTimes();                                               //���ڻ�Ʊ�۱��ļ��쳣�ָ�����

    static CGlobalParameter& GetInstance();

    void SetbNeedRestart(bool bNeedRestart = true);                             // ���õ�ǰϵͳ�Ƿ���Ҫ������������
    bool GetbNeedRestart();                                                     // ��õ�ǰϵͳ�Ƿ���Ҫ��������

protected:
private:
    unsigned char    m_uHour;
	unsigned char    m_uMinute;
	unsigned char    m_uSecond;
	unsigned long    m_recoverTimes;											// XFile��ǰ�ָ�����
	unsigned long	 m_vfareRecoverTimes;										// Ʊ�۱�ǰ�ָ�����
	unsigned long    m_maxRecoverTimes;											// �ļ����ָ�����
    bool             m_bNeedRestart;                                            // �Ƿ���Ҫ��������
	CXFile			 *m_expfile;												// �쳣��Ϣ�ļ�
	EXCEPTION_MAP    m_mapException;
	static CGlobalParameter  theInstance;
};