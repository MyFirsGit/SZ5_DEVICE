#pragma once
#include "TVMForeService.h"

/**
@brief 维护业务Service
*/

#define  MAX_ROW  15

class CReceipPrintSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CReceipPrintSvc();					// 构造函数
	virtual ~CReceipPrintSvc();			// 析构函数

	/*
		界面定义
	*/
	typedef enum _dialog_group {
		DIALOG_GROUP_MAIN_MENU = 1,		// 主菜单页面
		DIALOG_GROUP_FILE_LIST = 2,		// 可打印列表页面
	} DIALOG_GROUP;

	/*
		菜单选择定义
	*/
	typedef enum _menu_number_{
		MENU_NUM_NONE,
		MENU_NUM_BALANCE,
		MENU_NUM_BH_RECV,
		MENU_NUM_BH_CHANGE,
		MENU_NUM_CH_RECEIPT,
		MENU_NUM_TH_RECEIPT,
		MENU_NUM_EXCEPTION_RECEIPT,
	}MENU_NUMBER;

	/*
		数据模型定义
	*/
	typedef struct _tagModel
	{
		vector<CString> vecBalanceFileList;
		vector<CString> vecBHReciveFileList;
		vector<CString> vecBHChangeFileList;
		vector<CString> vecCHOperFileList;
		vector<CString> vecTHOperFileList;
		vector<CString> vecPassengerFileList;

		MENU_NUMBER		curSelectMenu;

		_tagModel(){
			vecBalanceFileList.clear();
			vecBHReciveFileList.clear();
			vecBHChangeFileList.clear();
			vecCHOperFileList.clear();
			vecTHOperFileList.clear();
			vecPassengerFileList.clear();
			curSelectMenu = MENU_NUM_NONE;
		}

	}Model;


	inline Model& GetDataModel(){
		return m_DataModel;
	}
	
	void NotifyDataModelChanged();

	void GetSelectedFileList(vector<CString>* &pVec);
protected:
	void OnStart();								// Service启动后处理

private:
	Model m_DataModel;
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);
	// 排序比较
	//static inline bool Upgreater(const CString& strDate1,const CString& strDate2){
	//	return (strDate1.Compare(strDate2) > 0);
	//};

	//int nPage;

	void InitAllPrintedFile();						// 初始化所有已打印日志

	void InitBalanceFileList(vector<CString>&);		// 初始化业结所有打印数据列表

	void InitPassangerFileList(vector<CString>&);	// 初始乘客业务异常打印数据

	void InitStaffPrintedFileList();				// 初始化站员业务打印数据列表
	
	long TryPrintSelectedFile(const CString&);		// 尝试打印选中的文件

	void SerchAllFilesFormPath(const CString&,vector<CString>&);	// 获取文件夹下所有的文件

	void DoPrintSelectFile(int nSelectFileID);		// 打印选中的文件
};