#pragma  once
#include <afx.h>
#include <vector>
#include "afxtempl.h"
#include "Xfile.h"
#include "CommonExp.h"
 
using namespace std;

#define theExceptionInfo  CExceptionInfo::GetInstance()

/**
 @brief      全局参数类
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
	    

	void IncRecoverTimes();                                                     //文件异常恢复次数加1
	void UpdateVfareRecoverTimes(int);											//设置票价表当前恢复次数(累加)
	unsigned long GetCurVfareRecoverTimes();                                    //得到票价表当前恢复次数
	unsigned long GetFileMaxRecoverTimes();                                     //得到文件最大允许恢复次数	
	bool OverMaxRecoverTimes();                                                 //判断是否超过最大恢复次数
	bool OverMaxVfareRecoverTimes();                                            //判断是否超过票价表最大恢复次数
	unsigned int GetExceptionByID(EXCEPTION_DETAIL& exMsg, long lId, long lDetailID );  // 获得指定异常详细信息
	unsigned int GetExceptionDetail();											// 获取全部异常信息
    void Release();                                                             //释放资源

	void InitRecoverTimes();                                                    //初期化文件异常恢复次数
	void InitVfareRecoverTimes();                                               //初期化票价表文件异常恢复次数

    static CExceptionInfo& GetInstance();

    void SetbNeedRestart(bool bNeedRestart = true);                             // 设置当前系统是否需要设置重新启动
    bool GetbNeedRestart();                                                     // 获得当前系统是否需要重新启动
	CString GetDeviceComponentCode(AFC_DEVICE_TYPE deviceType,DEVICE_SUBTYPE deviceSubType,long moudleID); // 获取MTC前两字节编码


protected:
private:
	unsigned long    m_recoverTimes;											// XFile当前恢复次数
	unsigned long	 m_vfareRecoverTimes;										// 票价表当前恢复次数
	unsigned long    m_maxRecoverTimes;											// 文件最大恢复次数
    bool             m_bNeedRestart;                                            // 是否需要重新启动
	CDXFile			 *m_expfile;												// 异常信息文件
	EXCEPTION_MAP    m_mapException;
	map<int,CString>     m_deviceComponentCode;
	static CExceptionInfo  theInstance;
};