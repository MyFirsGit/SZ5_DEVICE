#pragma once
#include "GetIni.h"
#include "UIDefinition.h"
#include "Util.h"

#define theGUI_INFO CGUIInfo::GetInstance()

#define LANGUAGE_TYPE_COUNT  3 // 语种数量

// 语言种类数组索引
typedef enum{
	LANGUAGE_TYPE_INDEX_CN = 0,
	LANGUAGE_TYPE_INDEX_EN  = 1,
	LANGUAGE_TYPE_INDEX_PN = 2
}LANGUAGE_TYPE_INDEX;

/**
@brief GUI配置文件控制类
*/

class  CGUIInfo
{
public:

	static CGUIInfo&  GetInstance();
	int Initialize();                                                   // 初期化

	// 操作员界面字体设置
	DWORD GetTitleFontSize();		    // 取得标题字体大小
 	DWORD GetBtnFontSize();				// 取得按钮字体大小
 	DWORD GetNormalFontSize();		    // 基本信息区正文字体大小
	DWORD GetGuideFontSize();			// 操作员界面向导文言字体大小
	DWORD GetIniFontSize();				// 初始化界面的字体大
	
	CString GetTitleFontName();		    // 取得标题字体
	CString GetBtnFontName();		    // 取得按钮字体
	CString GetNormalFontName();		// 基本信息区正文字体名称
	CString GetGuideFontName();	// 操作员界面向导文言字体名称
	CString GetIniFontName();		    // 初始化界面的字体名称
	LOGFONT& GetDefaultFont();          // 取得普通文本字体

	COLORREF GetTitleFontColor();		// 取标题字体颜色
	COLORREF GetBtnFontColor();			// 取按钮字体颜色
	COLORREF GetNormalFontColor();		// 取基本信息区域正文字体颜色
	COLORREF GetOperatorGuideFontColor();//取操作员界面向导文言字体颜色
	COLORREF GetIniFontColor();			// 取初始化界面字体颜色
	COLORREF GetInitRsltSucceedFontColor();// 取初始化结果字体颜色
	COLORREF GetInitRsltFailedFontColor();
	COLORREF GetInitRsltUnknowFontColor();
	
	// 乘客界面字体设置
	DWORD GetRecStationFontSize(/*LANGUAGETYPE_T language_mode*/);			// 取得乘客界面普通车站名称字体大小
	DWORD GetRecTitleFontSize(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);		    // 取得乘客界面标题字体大小
	DWORD GetRecDateFontSize(/*LANGUAGETYPE_T language_mode*/);				// 取得乘客界面日期字体大小
	DWORD GetRecBaseInfoFontSize(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);		    // 取得乘客界面基本信息字体大小
	DWORD GetRecGuideFontSize(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);			// 取得乘客界面普通业务向导文言字体大小
	DWORD GetRecMessageBoxFontSize(/*LANGUAGETYPE_T language_mode*/);	    // 取得乘客界面消息框文言字体大小
	DWORD GetRecMessageBoxTitleFontSize(/*LANGUAGETYPE_T language_mode*/);	// 取得乘客界面消息框标题文言字体大小
	DWORD GetFunctBtnFontSize();										// 取乘客界面功能按钮字体大小
	DWORD GetMenueBtnFontSize();										// 取乘客界面菜单按钮字体大小
	DWORD GetFuncBtnGroupTitleFontSize();								// 取乘客界面功能按钮组标题字体大小
	DWORD GetLogoFontSizeCN();		                                    // LOGO中文字体大小
	DWORD GetLogoFontSizeEN();		                                    // LOGO英文字体大小
	DWORD GetPassengerFontCodeType();												// 取得字符集信息
	DWORD GetOperatorFontCodeType();												// 取得字符集信息
	DWORD GetVersionFontSize();										    // 获取乘客界面版本信息文言字体大小
	DWORD GetSvcStateFontSize();										// 乘客界面服务状态字体大小
	DWORD GetServiceStateFontSize();									// 乘客界面服务状态的字体大小（暂停服务，正常服务等大字）

	CString GetRecStationFontName(/*LANGUAGETYPE_T language_mode*/);		// 取得乘客界面普通车站名称字体
	CString GetRecTitleFontName(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);		    // 取得乘客界面标题字体
	CString GetRecDateFontName(/*LANGUAGETYPE_T language_mode*/);		    // 取得乘客界面日期字体
	CString GetRecBaseInfoFontName(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);	    // 取得乘客界面基本信息字体
	CString GetRecGuideFontName(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);		    // 取得乘客界面普通业务向导文言字体
	CString GetRecMessageBoxFontName(/*LANGUAGETYPE_T language_mode*/);		// 取得乘客界面消息框文言字体
	CString GetRecMessageBoxTitleFontName(/*LANGUAGETYPE_T language_mode*/);// 取得乘客界面消息框标题文言字体
	//Added
	CString GetFunctBtnFontName();										// 取乘客界面功能按钮字体
	CString GetMenueBtnFontName();										// 取乘客界面菜单按钮字体
	CString GetFuncBtnGroupTitleFontName();								// 取乘客界面功能按钮组标题字体
	CString GetSvcStateFontName();										// 取乘客界面服务状态字体
	CString GetServiceStateFontName();									// 取乘客界面服务状态（正常服务，暂停服务等）字体名称
	
	CString GetLogoFontNameCN();		                                // LOGO中文字体名称
	CString GetLogoFontNameEN();		                                // LOGO英文字体名称

	COLORREF GetRecStationFontColor();									// 取乘客界面普通车站名称字体颜色
	COLORREF GetRecTitleFontColor(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);		// 取乘客界面标题字体颜色
	COLORREF GetRecDateFontColor(/*LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED*/);			// 取乘客界面日期字体颜色
	COLORREF GetRecBaseInfoFontColor(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);		// 取乘客界面基本信息字体颜色
	COLORREF GetRecGuideFontColor(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);		// 取得乘客界面普通业务向导文言字体颜色
	COLORREF GetRecMsgBoxFontColor(/*LANGUAGETYPE_T language_mode*/);		// 取得乘客界面消息框文言字体颜色
	COLORREF GetRecMsgBoxTitleFontColor(/*LANGUAGETYPE_T language_mode*/);	// 取得乘客界面消息框标题文言字体颜色
	COLORREF GetLogoFontColorCN(LANGUAGETYPE_T language_mode);			// LOGO中文字体颜色
	COLORREF GetLogoFontColorEN(LANGUAGETYPE_T language_mode);			// LOGO英文字体颜色

	COLORREF GetTitleBakColor();		                                // 取得标题背景色
	COLORREF GetBaseBakColor();			                                // 取得基本区域背景色
	COLORREF GetStatusBakColor();		                                // 取得状态区背景色
	COLORREF GetFunBakColor();				                            // 取得功能区背景色
	COLORREF GetFuncBtnFontColor();
	COLORREF GetMenuBtnFontColor();
	COLORREF GetFuncBtnGroupTitleFontColor();
	COLORREF GetNormalSvcStateFontColor();								// 正常服务状态字体颜色
	COLORREF GetUnNormalSvcStateFontColor();							// 非正常服务状态字体颜色
	COLORREF GetGuideAreaColor();										// 操作员向导区域背景色
	COLORREF GetBaseInfoAreaColor();									// 操作员基本信息区域背景色
	COLORREF GetStateAreaColor();										// 操作员状态区域背景色
	COLORREF GetFuncAreaColor();										// 操作员导航区域背景色
	COLORREF GetServiceStateFontColor();								// 乘客界面服务状态字体颜色 

	double GetReceptionSolutionWidthRatio();		                    // 获取乘客画面分辨率宽
	double GetReceptionSolutionHeightRatio();		                    // 获取乘客画面分辨率高
	LOGFONT& GetBtnFont();                                              // 取按钮字体
	LOGFONT& GetRecMenuBtnFont();										// 取菜单栏按钮字体
	double GetOperatorImageResolutionWidth();							// 取后维护界面宽度分辨绿
	double GetOperatorImageResolutionHeight();							// 取后维护界面高度分辨率
	CString GetHistoryGridTxtFontName();								// 取乘客界面查询-历史记录表格字体名称
	DWORD GetHistoryGridTxtFontSize();									// 取乘客界面查询-历史记录表格字体大小
	COLORREF GetHistoryGridTxtFontColor();								// 取乘客界面查询-历史记录表格字体颜色

private:
	

	LOGFONT m_DefaultFont;
	LOGFONT m_BtnFont;
	LOGFONT m_TitleFont;
	LOGFONT	m_MenueBtnFont;				// 乘客界面菜单按钮字体

	COLORREF m_TitleBakColor;		    // 标题背景色
	COLORREF m_BaseBakColor;			// 基本区域背景色
	COLORREF m_StatusBakColor;		    // 状态区背景色
	COLORREF m_FunBakColor;				// 功能区背景色

	COLORREF m_OpBaseAreaColor;			// 操作员基本信息区域背景色
	COLORREF m_OpGuidAreaColor;			// 操作员向导区域背景色
	COLORREF m_OpStatAreaColor;			// 操作员状态区域背景色
	COLORREF m_OpStateFuncAreaColor;	// 操作员导航栏区域背景色
	// 操作员界面
	DWORD m_OperatorTitleFontSize[LANGUAGE_TYPE_COUNT];						// 操作员界面标题字体大小
	CStringArray m_OperatorTitleFontName;									// 操作员界面标题字体名称
	DWORD m_OperatorBtnFontSize[LANGUAGE_TYPE_COUNT];						// 操作员界面按钮字体大小（暂不使用）
	CStringArray m_OperatorBtnFontName;										// 操作员界面按钮字体名称（暂不使用）
	DWORD m_OperatorNormalFontSize[LANGUAGE_TYPE_COUNT];					// 操作员界面基本信息区正文字体大小 
	CStringArray m_OperatorNormalFontName;									// 操作员界面基本信息区正文字体名称 
	DWORD m_OperatorGuideFontSize[LANGUAGE_TYPE_COUNT];						// 操作员界面向导信息区字体大小
	CStringArray m_OperatorGuideFontName;									// 操作员界面向导信息区字体名称
	DWORD m_OperatorIniFontSize[LANGUAGE_TYPE_COUNT];						// 操作员初始化界面字体大小  
	CStringArray m_OperatorIniFontName;										// 操作员初始化界面字体名称  
	CUIntArray m_OperatorCharSet;												// 操作员界面字符集
	double m_OperatorSolutionWidthRatio;									// 操作员界面分辨率-宽	
	double m_OperatorSolutionHeightRatio;									// 操作员界面分辨率-高	

	// 乘客界面
	DWORD m_RecStationFontSize[LANGUAGE_TYPE_COUNT];						// 乘客界面状态栏车站名称字体大小
	CStringArray m_RecStationFontName;										// 乘客界面状态栏车站名称字体名称
	DWORD m_RecTitleFontSize[LANGUAGE_TYPE_COUNT];							// 乘客界面基本信息区标题字体大小
	CStringArray m_RecTitleFontName;										// 乘客界面基本信息区标题字体名称
	DWORD m_RecBaseInfoFontSize[LANGUAGE_TYPE_COUNT];						// 乘客界面基本信息区正文字体大小
	CStringArray m_RecBaseInfoFontName;										// 乘客界面基本信息区正文字体名称
	DWORD m_RecDateFontSize[LANGUAGE_TYPE_COUNT];							// 乘客界面状态栏日期时间字体大小
	CStringArray m_RecDateFontName;											// 乘客界面状态栏日期时间字体名称
	DWORD m_RecSvcStaeFontSize[LANGUAGE_TYPE_COUNT];						// 乘客界面状态栏服务状态字体大小
	CStringArray m_RecSvcStateFontName;										// 乘客界面状态栏服务状态字体名称
	DWORD m_RecGuideInfoFontSize[LANGUAGE_TYPE_COUNT];						// 乘客界面向导信息区字体大小
	CStringArray m_RecGuideInfoFontName;									// 乘客界面向导信息区字体名称
	DWORD m_RecMessageBoxFontSize[LANGUAGE_TYPE_COUNT];						// 乘客界面消息提示框正文字体大小
	CStringArray m_RecMessageBoxFontName;									// 乘客界面消息提示框正文字体名称
	DWORD m_RecMessageBoxTitleFontSize[LANGUAGE_TYPE_COUNT];				// 乘客界面消息提示框标题字体大小
	CStringArray m_RecMessageBoxTitleFontName;								// 乘客界面消息提示框标题字体名称
	//Added
	DWORD m_FuncBtnFontSize[LANGUAGE_TYPE_COUNT];							// 乘客界面功能按钮字体大小
	CStringArray m_FuncBtnFontName;											// 乘客界面功能按钮字体名称
	DWORD m_MenueBtnFontSize[LANGUAGE_TYPE_COUNT];							// 乘客界面菜单按钮字体大小
	CStringArray m_MenueBtnFontName;										// 乘客界面菜单按钮字体名称
	DWORD m_FuncBtnGroupTitleFontSize[LANGUAGE_TYPE_COUNT];					// 乘客界面功能按钮组区域标题字体大小
	CStringArray m_FuncBtnGroupTitleFontName;								// 乘客界面功能按钮组区域标题字体名称
	DWORD m_ServiceStateTxtSize[LANGUAGE_TYPE_COUNT];						// 乘客界面服务状态字体大小
	CStringArray m_ServiceStateTxtName;										// 乘客界面服务状态字体名称

	DWORD m_RecLogoFontSize[LANGUAGE_TYPE_COUNT];							// 乘客界面LOGO字体大小
	CStringArray m_RecLogoFontName;											// 乘客界面LOGO字体名称
	DWORD m_RecVersionInfoFontSize;											// 乘客界面版本信息字体大小
	double m_ReceptionSolutionWidthRatio;									// 乘客界面分辨率-宽	
	double m_ReceptionSolutionHeightRatio;									// 乘客界面分辨率-高	
	double m_OperatorImageResolutionWidth;									// 后维护界面分辨率 - 宽度
	double m_OperatorImageResolutionHeight;									// 后维护界面分辨率 -	 高度
	
	COLORREF m_TitleFontColor[LANGUAGE_TYPE_COUNT];							// 操作员界面标题字体颜色
	COLORREF m_BtnFontColor[LANGUAGE_TYPE_COUNT];							// 操作员界面按钮字体颜色(暂不使用)
	COLORREF m_NormalFontColor[LANGUAGE_TYPE_COUNT];						// 操作员界面基本信息区域正文字体颜色
	COLORREF m_GuideFontColor[LANGUAGE_TYPE_COUNT];							// 操作员界面向导字体颜色
	COLORREF m_InitFontColor[LANGUAGE_TYPE_COUNT];							// 操作员界面初始化字体颜色

	COLORREF m_InitSucceedFontColor;										// 初始化结果成功字体颜色
	COLORREF m_InitFailedFontColor;											// 初始化结果失败字体颜色
	COLORREF m_InitUnknownFontColor;										// 初始化结果未知字体颜色
	
	COLORREF m_RectStationFontColor[LANGUAGE_TYPE_COUNT];					// 乘客界面车站字体颜色
	COLORREF m_RectNormalSvcStateFontColor;									// 正常服务状态字体颜色
	COLORREF m_RectUnNormalSvcStateFontColor;								// 非正常服务字体颜色
	COLORREF m_RectTitleFontColor[LANGUAGE_TYPE_COUNT];						// 乘客界面标题字体颜色
	COLORREF m_RectDateFontColor[LANGUAGE_TYPE_COUNT];						// 乘客界面日期字体颜色
	COLORREF m_RectBaseInfoFontColor[LANGUAGE_TYPE_COUNT];					// 乘客界面基本信息字体颜色
	COLORREF m_RectGuideFontColor[LANGUAGE_TYPE_COUNT];						// 乘客界面向导信息字体颜色
	COLORREF m_RectMsgBoxFontColor[LANGUAGE_TYPE_COUNT];					// 对话框消息字体颜色
	COLORREF m_RectMsgBoxTitleFontColor[LANGUAGE_TYPE_COUNT];				// 对话框标题字体颜色
	COLORREF m_FuncBtnFontColor[LANGUAGE_TYPE_COUNT];						// 功能按钮字体颜色
	COLORREF m_MenuBtnFontColor[LANGUAGE_TYPE_COUNT];						// 菜单按钮字体颜色
	COLORREF m_FuncBtnGroupTitleFontColor[LANGUAGE_TYPE_COUNT];				// 功能按钮区域标题字体颜色
	COLORREF m_LogoFontColor[LANGUAGE_TYPE_COUNT];							// LOGO字体颜色
	COLORREF m_ServiceStateTxtColor[LANGUAGE_TYPE_COUNT];					// 服务状态字体颜色

	CStringArray m_HistoryGridFontName;										// 历史记录字体名称
	DWORD m_HistoryGridFontSize[LANGUAGE_TYPE_COUNT];						// 历史记录字体大小
	COLORREF m_HistoryGridFontColor[LANGUAGE_TYPE_COUNT];					// 历史记录字体颜色

	static CGUIInfo theInstance;	
    CGetIni* m_GUIIni;														// INI文件对象

private:
	// 隐藏构造函数
	CGUIInfo();
	~CGUIInfo();
	CGUIInfo(const CGUIInfo&);
	CGUIInfo& operator=(const CGUIInfo&);

	LANGUAGE_TYPE_INDEX GetPassengerLanguageIndex();						// 获取乘客界面语言种类索引
	LANGUAGE_TYPE_INDEX GetOperatorLanguageIndex();							// 获取站员界面语言种类索引
};
