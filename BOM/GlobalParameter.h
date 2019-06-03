#pragma  once
#include <afx.h>
#include <vector>
#include "afxtempl.h"
#include "Xfile.h"
#include "CommonExp.h"
 
using namespace std;

#define theGlobalParameter  CGlobalParameter::GetInstance()

/**
 @brief      全局参数类
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
	    
    bool SetBusiDateSwitchTime(unsigned char Hour = 0, unsigned char Minute = 0,unsigned char Second = 0);    //设置营运日切换时间
	bool GetBusiDateSwitchTime(unsigned char& Hour, unsigned char& Minute,unsigned char& Second);			  //得到营运日切换时间

	

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

    static CGlobalParameter& GetInstance();

    void SetbNeedRestart(bool bNeedRestart = true);                             // 设置当前系统是否需要设置重新启动
    bool GetbNeedRestart();                                                     // 获得当前系统是否需要重新启动

protected:
private:
    unsigned char    m_uHour;
	unsigned char    m_uMinute;
	unsigned char    m_uSecond;
	unsigned long    m_recoverTimes;											// XFile当前恢复次数
	unsigned long	 m_vfareRecoverTimes;										// 票价表当前恢复次数
	unsigned long    m_maxRecoverTimes;											// 文件最大恢复次数
    bool             m_bNeedRestart;                                            // 是否需要重新启动
	CXFile			 *m_expfile;												// 异常信息文件
	EXCEPTION_MAP    m_mapException;
	static CGlobalParameter  theInstance;
};