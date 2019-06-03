#include "childview.h"
#include "ChildViewRec.h"
#include "servicedefinition.h"
#include "SvcDef.h"
#pragma once


#define theAPP_SESSION  CAppSession::GetInstance()

/**
  @brief 对BOM全体业务相关信息进行管理的类
*/
class CAppSession
{
public:
    static CAppSession& GetInstance();                                  // 获取该类的对象

//    // 在线状态
    void SetSCConnected(bool);                                         // 设置在线状态
    bool IsSCConnected() const;                                        // 判断是否在线
   void SetHasNewParam(bool);                                          // 设置是否有新参数（从上位下载）
   bool HasNewParam() const;                                           // 判断是否有新参数（从上位下载）
//    void SetIsCDVersionChange(bool);                                    // 设置是否有CD版本更换
//    bool IsCDVersionChange() const;                                     // 判断是否有CD版本更换
//
//    // 初期化失败
    bool IsInitCheckError() const;                                      // 判断初期化失败
    void SetInitCheckError(bool);                                       // 设置初期化失败
//
//	bool IsInitDetailError(INIT_DETAIL_ERROR);							// 是否存在某个初期化错误
	void SetInitDetailErrorFlag(INIT_DETAIL_ERROR);						// 设置某个初期化错误
//	void InitDetailErrorInfo();											// 初期化错误信息
//	void RemoveInitDetailErrorFlag(INIT_DETAIL_ERROR);					// 移除某个初期化错误
	DWORD GetInitDetailErrorFlag();										// 获取初期化错误标志
	DWORD IsInitErrorOver();											// 是否存在需要中止的初期化错误
//
	void SetInitDetailErrorMsg(CString);								// 设置初期化详细错误文言
	CString GetInitDetailErrorMsg();									// 获取初期化详细错误文言
//
	// TH端口是否连接
	bool IsTHConnected() const;                                         // 判断RW端口是否连接
	void SetIsTHConnected(bool);                                        // 设置RW端口是否连接
//
//    // 程序第一次启动
//    bool IsFirstRun() const;                                            // 判断是否程序第一次启动
//    void SetIsFirstRun(bool);                                           // 设置是否程序第一次启动
//

//
//    // 有无未送数据
//    bool HasDeliverData() const;                                        // 判断是否有未送数据
//    void SetHasDeliverData(bool);                                       // 设置是否有未送数据
//
//    // 版本信息电文
//    void SaveVersion(const VARIABLE_DATA& binData);                      // 保存电文信息
//    void GetVersion(VARIABLE_DATA& binData);                             // 获取电文信息
    void SetHasProgUpdate(bool);                                        // 设置是否有程序更新
    bool HasProgUpdate();                                               // 判断是否有程序更新
//
//    void SetIsDownload(bool);                                           // 设置是否正在FTP中
//    bool GetIsDownload() const;                                         // 获取是否正在FTP中
//
//    void SetIsUpdateParam(bool);                                        // 设置是否正在下载参数中
//    bool GetIsUpdateParam() const;                                      // 获取是否正在下载参数中
//
//    void SetIsWorkingHourChange(bool);                                  // 设置是否运营日切换
//    bool GetIsWorkingHourChange() const;                                // 获取是否运营日切换
//
//
	// SAM信息
    void SetSAMInfo(SAM_TYPE ,const LPBYTE);							// 设置SAM信息
    SAM_ID_INFO GetSAMInfo();											// 获取SAM信息

	// 工作处理类型
	CLOSE_START_FLAG GetCloseStartFlag() const;                         // 获取工作处理类型
	void SetCloseStartFlag(const CLOSE_START_FLAG);                     // 设置工作处理类型 

	// 站员信息
	bool IsSuperUser();
	USER_INFO GetUserInfo() const;                                      // 获取站员信息
	void SetUserInfo(const USER_INFO&);                                 // 设置站员信息

    // 车站信息
    CString GetStationNameCN() const;                                   // 获取当站车站名称（中文）
    CString GetStationNameEN() const;                                   // 获取当站车站名称（英文）
    void SetStationName(const CString, const CString);                  // 设置当站车站名称

    void ShowOfficeGuide(CString sMsg);                                 // 显示站员画面向导标题
    void ShowReceptionGuide(CString sMsg, CString sEnglishMsg);         // 显示乘客画面向导信息
    void ShowAllGuide(CString sOfficeMsg, CString sReceptionMsg, 
        CString sReceptionEnglishMsg);                                  // 显示站员和乘客画面向导信息
    CChildView* GetChildView();                                         // 获取ChildView
    CChildViewRec* GetChildViewRec();                                   // 获取顾客视窗ChildViewRec
    CString GetServiceNameCH(SERVICE_ID serviceID);                     // 获取业务名称（中文）
    CString GetServiceNameEN(SERVICE_ID serviceID);                     // 获取业务名称（英文）
   // void ShowReceptionServiceName(CString nameCH, CString nameEN);      // 显示乘客画面车站名称
    void ParseGuideMsg(CString originalGuideMsg, CString& guideMsg, 
        COLORREF& color);                                               // 解析向导信息
//
//    bool HasARToSend();                                                 // 是否有AR数据需要发送
//    void SetARToSend(bool need);                                        // 设置是否有AR数据需要发送
//

//
//	void SetRePrintStaffID(CString);									// 设置前次打印员工ID
//	void SetRePrintLoginTime(_DATE_TIME);								// 设置前次打印登录时间
//	void SetRePrintGroupID(CString);									// 设置前次打印班ID
//
//	CString GetRePrintStaffID();										// 取得前次打印员工ID
//	_DATE_TIME GetRePrintLoginTime();									// 取得前次打印登录时间
//	CString GetRePrintGroupID();										// 取得前次打印班ID
//
private:

    bool m_isSCConnected;                                              // 是否在线
    bool m_hasNewParam;                                                 // 是否有新参数（从上位下载）
    bool m_hasProgUpdate;                                               // 是否有程序更新
//    bool m_isFirstRun;                                                  // 是否第一次运行程序
      bool m_InitCheckError;                                              // 初期检查失败
      DWORD m_initDetailErrorFlag;										// 初期化详细错误标志
	CString m_initDetailErrorMsg;										// 初期化详细错误文言
//    bool m_isDownload;                                                  // 是否正在FTP中
//    bool m_needSendAR;                                                  // 是否有AR数据需要发送
	bool m_isTHConnected;                                               // RW端口是否连接
//    bool m_isUpdateParameter;                                           // ACC参数是否正在更新
//    bool m_isWorkingHourChange;                                         // 是否运营日切换
	CLOSE_START_FLAG  m_closeStartFlag;                                 // 日终处理标志
	USER_INFO         m_userInfo;                                       // 站员信息
    CString           m_stationNameCN;                                  // 车站名称（中文）
    CString           m_stationNameEN;                                  // 车站名称（英文）
    SAM_ID_INFO   m_SAMInfo;                                   // SAMid是否改变

//
//	CString m_printStaffID;												// 前次员工ID
//	_DATE_TIME m_printLoginTime;										// 前次员工登录时间
//	CString m_printGroupID;												// 前次班ID

    // Hidden
private:
	static CAppSession theInstance;                                     // 该类的唯一接口
    CAppSession();
    ~CAppSession(){};
    CAppSession(const CAppSession&);
    CAppSession& operator=(const CAppSession&);
};