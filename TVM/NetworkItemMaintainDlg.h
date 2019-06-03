#pragma once
#include "OperationDlg.h"
#include "NetworkTestSvc.h"

#define _MAX_ROW 15                 //定义基本显示区最多显示行数
#define _MAX_COL 2                  //定义基本显示区最多显示列数

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //成功显示
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //失败显示
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //测试完成

#define  UNKNOW      _T("")

class CNetworkItemMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CNetworkItemMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:

	CNetworkItemMaintainDlg(CService* pService);     //标准构造函数
	~CNetworkItemMaintainDlg();

	enum {IDD = IDD_20551_NETWORK_ITEM_DLG};         //对话框资源ID

	typedef struct _Menu_Status_
	{
		CString strMenuID;							//菜单编号

		CString  strMenu_1_Status;                  //菜单1执行结果
		CString  strMenu_2_Status;					//菜单2执行结果
		CString  strMenu_3_Status;                  //菜单3执行结果,返回上级目录显示

		_Menu_Status_()
		{
			strMenuID        = _T("");
			strMenu_1_Status = _T("");	
			strMenu_2_Status = _T("");	
			strMenu_3_Status = _T("");
		}

	}Menu_Info;							   //记录当前菜单目录深度,信息选项

public:
	//To services接口
	void PingScConn();                     //SC连接测试
	void FtpUploading();                   //FTP上传测试
	void FtpDownloading();                 //FTP下载测试

	void RebackToMenu();                   //返回上一级界面
	Menu_Info& GetStructData();            //使用引用，共享struct变量

public:
	CString CreateFTPTestFile();           //创建FTP测试文件

private:
	LABEL_GROUP m_lablegroup[_MAX_ROW];    //Main 显示内容
	Menu_Info m_Menu_Info; 

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

private:	
	void SetLableGroup(const LABEL_GROUP* pLableGroup,const UINT uLableGroupNum);
	void UpdateAreaLable(int nRow,int nCol,CString strValue,COLORREF itemColor); //更新某单元格数据
	void CutCStringToPingStyle(CString& strBeCut,vector<CString>& vStrPing);     //显示标准Ping格式
	void reflushBaseArea();                                                      //强制更新
	BOOL UpdateLableGroup(int nCol);											 //设置显示的列数
	void ClearLableData();                                                       //清除static显示内容

	friend class CNetworkTestSvc;
};