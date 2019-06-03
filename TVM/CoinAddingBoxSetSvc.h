#pragma once
#include "TVMForeService.h"

#define MAX_COINADDMENU_NUM 2		//补充箱币种设置界面菜单的最大编号

/**
@brief    补充箱币种设置
*/
class CCoinAddingBoxSetSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CCoinAddingBoxSetSvc();
	~CCoinAddingBoxSetSvc();
	
	typedef enum _dialog_group{
		DIALOG_GROUP_COINADDINGBOX_SET = 1,
	}DIALOG_GROUP;

	typedef struct _tagModel
	{
		int hopone ;
		int hoptwo ;
		CString hoponecurrency;
		CString hoptwocurrency;
		int MenuNumber;			//菜单号
		BOOL MenuChange;		//菜单变化标志
		BOOL CodeChange;		//代码变化标志
		BOOL CodeContent;		//输入内容是否非法标志
		_tagModel()
		{
			hopone = -1;
			hoptwo = -1;
			hoponecurrency = _T("");
			hoptwocurrency = _T("");
			MenuNumber = 0;
			MenuChange = FALSE;
			CodeChange = FALSE;
			CodeContent = TRUE;

		}
	}Model;

	typedef struct _CurrIndex
	{
		CString numberone;
		CString numbertwo;
		_CurrIndex()
		{
			numberone = _T("");
			numbertwo = _T("");
		}
	}CurrIndex;

	Model& GetDataModel();
	void ReadTvmIni();															//读取TVM.INI文件
	CString ReadCurrency(int hopetype);											//识别币种
	void CheckMenuNumber();
	void InputGetGuide();														//对GetGuide分别赋值
	void DoCheckInput();														//对输入数据是否合理做出相应项的实际判断
	CString CurrGet(int currmenunumber);										//对菜单号转换后保存
	virtual void NotifyDataModelChanged();
private:
	int EnterTime;
	CString GetGuide;															//向导栏提示
	
	Model m_DataModel;
	CurrIndex CurrIndexNumber;
	
	LRESULT OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Clear] 键按下消息（删除）
	//LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM /*lParam*/);					// 处理 [0~9] 键按下消息
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消息
};