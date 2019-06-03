#pragma  once
#include <afx.h>
#include <vector>
#include "afxtempl.h"
#include "Xfile.h"
#include "CommonExp.h"
 
using namespace std;

#define theExceptionInfo  CExceptionInfo::GetInstance()

/**
 @brief      ȫ�ֲ�����
*/
class CExceptionInfo
{
private:
	CExceptionInfo(void);
	~CExceptionInfo(void);

public:
	typedef struct exception_detail {
		unsigned long lID ;
		unsigned long lDetail_ID;
		unsigned long lMTC;
		unsigned short lEc;
		CString         sErr;
		CString         sDeal;
		CString			eErr;
		CString			eDeal;
		CString			pErr;
		CString         pDeal;
		exception_detail(){
           lID = 0;
           lDetail_ID = 0;
           lMTC = 0;
           lEc = 0;
		   sErr = _T("");
		   sDeal = _T("");
		   eErr = _T("");
		   eDeal = _T("");
		   pErr = _T("");
		   pDeal = _T("");
		}
		exception_detail(const exception_detail& that){
			lID = that.lID;
			lDetail_ID = that.lDetail_ID;
			lMTC = that.lMTC;
			lEc = that.lEc;
			sErr = that.sErr;
			sDeal = that.sDeal;
			eErr = that.eErr;
			eDeal = that.eDeal;
			pErr = that.pErr;
			pDeal = that.pDeal;
		}

		~exception_detail(){
			
		}
		exception_detail& exception_detail::operator=(const exception_detail& exData){
			lID = exData.lID;
			lDetail_ID = exData.lDetail_ID;
			lMTC = exData.lMTC;
			lEc = exData.lEc;
			sErr = exData.sErr;
			sDeal = exData.sDeal;
			eErr = exData.eErr;
			eDeal = exData.eDeal;
			pErr = exData.pErr;
			pDeal = exData.pDeal;
			return *this;
		}
	} EXCEPTION_DETAIL, *LPEXCEPTION_DETAIL;

	typedef CMap<CString, LPCTSTR, LPEXCEPTION_DETAIL, LPEXCEPTION_DETAIL> EXCEPTION_MAP;
	    

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

    static CExceptionInfo& GetInstance();

    void SetbNeedRestart(bool bNeedRestart = true);                             // ���õ�ǰϵͳ�Ƿ���Ҫ������������
    bool GetbNeedRestart();                                                     // ��õ�ǰϵͳ�Ƿ���Ҫ��������
	CString GetDeviceComponentCode(AFC_DEVICE_TYPE deviceType,DEVICE_SUBTYPE deviceSubType,long moudleID); // ��ȡMTCǰ���ֽڱ���


protected:
private:
	unsigned long    m_recoverTimes;											// XFile��ǰ�ָ�����
	unsigned long	 m_vfareRecoverTimes;										// Ʊ�۱�ǰ�ָ�����
	unsigned long    m_maxRecoverTimes;											// �ļ����ָ�����
    bool             m_bNeedRestart;                                            // �Ƿ���Ҫ��������
	CDXFile			 *m_expfile;												// �쳣��Ϣ�ļ�
	EXCEPTION_MAP    m_mapException;
	map<int,CString>     m_deviceComponentCode;
	static CExceptionInfo  theInstance;
};