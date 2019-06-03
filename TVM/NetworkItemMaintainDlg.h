#pragma once
#include "OperationDlg.h"
#include "NetworkTestSvc.h"

#define _MAX_ROW 15                 //���������ʾ�������ʾ����
#define _MAX_COL 2                  //���������ʾ�������ʾ����

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //�ɹ���ʾ
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //ʧ����ʾ
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //�������

#define  UNKNOW      _T("")

class CNetworkItemMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CNetworkItemMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:

	CNetworkItemMaintainDlg(CService* pService);     //��׼���캯��
	~CNetworkItemMaintainDlg();

	enum {IDD = IDD_20551_NETWORK_ITEM_DLG};         //�Ի�����ԴID

	typedef struct _Menu_Status_
	{
		CString strMenuID;							//�˵����

		CString  strMenu_1_Status;                  //�˵�1ִ�н��
		CString  strMenu_2_Status;					//�˵�2ִ�н��
		CString  strMenu_3_Status;                  //�˵�3ִ�н��,�����ϼ�Ŀ¼��ʾ

		_Menu_Status_()
		{
			strMenuID        = _T("");
			strMenu_1_Status = _T("");	
			strMenu_2_Status = _T("");	
			strMenu_3_Status = _T("");
		}

	}Menu_Info;							   //��¼��ǰ�˵�Ŀ¼���,��Ϣѡ��

public:
	//To services�ӿ�
	void PingScConn();                     //SC���Ӳ���
	void FtpUploading();                   //FTP�ϴ�����
	void FtpDownloading();                 //FTP���ز���

	void RebackToMenu();                   //������һ������
	Menu_Info& GetStructData();            //ʹ�����ã�����struct����

public:
	CString CreateFTPTestFile();           //����FTP�����ļ�

private:
	LABEL_GROUP m_lablegroup[_MAX_ROW];    //Main ��ʾ����
	Menu_Info m_Menu_Info; 

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

private:	
	void SetLableGroup(const LABEL_GROUP* pLableGroup,const UINT uLableGroupNum);
	void UpdateAreaLable(int nRow,int nCol,CString strValue,COLORREF itemColor); //����ĳ��Ԫ������
	void CutCStringToPingStyle(CString& strBeCut,vector<CString>& vStrPing);     //��ʾ��׼Ping��ʽ
	void reflushBaseArea();                                                      //ǿ�Ƹ���
	BOOL UpdateLableGroup(int nCol);											 //������ʾ������
	void ClearLableData();                                                       //���static��ʾ����

	friend class CNetworkTestSvc;
};