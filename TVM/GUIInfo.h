#pragma once
#include "GetIni.h"
#include "UIDefinition.h"
#include "Util.h"

#define theGUI_INFO CGUIInfo::GetInstance()

#define LANGUAGE_TYPE_COUNT  3 // ��������

// ����������������
typedef enum{
	LANGUAGE_TYPE_INDEX_CN = 0,
	LANGUAGE_TYPE_INDEX_EN  = 1,
	LANGUAGE_TYPE_INDEX_PN = 2
}LANGUAGE_TYPE_INDEX;

/**
@brief GUI�����ļ�������
*/

class  CGUIInfo
{
public:

	static CGUIInfo&  GetInstance();
	int Initialize();                                                   // ���ڻ�

	// ����Ա������������
	DWORD GetTitleFontSize();		    // ȡ�ñ��������С
 	DWORD GetBtnFontSize();				// ȡ�ð�ť�����С
 	DWORD GetNormalFontSize();		    // ������Ϣ�����������С
	DWORD GetGuideFontSize();			// ����Ա���������������С
	DWORD GetIniFontSize();				// ��ʼ������������
	
	CString GetTitleFontName();		    // ȡ�ñ�������
	CString GetBtnFontName();		    // ȡ�ð�ť����
	CString GetNormalFontName();		// ������Ϣ��������������
	CString GetGuideFontName();	// ����Ա������������������
	CString GetIniFontName();		    // ��ʼ���������������
	LOGFONT& GetDefaultFont();          // ȡ����ͨ�ı�����

	COLORREF GetTitleFontColor();		// ȡ����������ɫ
	COLORREF GetBtnFontColor();			// ȡ��ť������ɫ
	COLORREF GetNormalFontColor();		// ȡ������Ϣ��������������ɫ
	COLORREF GetOperatorGuideFontColor();//ȡ����Ա����������������ɫ
	COLORREF GetIniFontColor();			// ȡ��ʼ������������ɫ
	COLORREF GetInitRsltSucceedFontColor();// ȡ��ʼ�����������ɫ
	COLORREF GetInitRsltFailedFontColor();
	COLORREF GetInitRsltUnknowFontColor();
	
	// �˿ͽ�����������
	DWORD GetRecStationFontSize(/*LANGUAGETYPE_T language_mode*/);			// ȡ�ó˿ͽ�����ͨ��վ���������С
	DWORD GetRecTitleFontSize(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);		    // ȡ�ó˿ͽ�����������С
	DWORD GetRecDateFontSize(/*LANGUAGETYPE_T language_mode*/);				// ȡ�ó˿ͽ������������С
	DWORD GetRecBaseInfoFontSize(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);		    // ȡ�ó˿ͽ��������Ϣ�����С
	DWORD GetRecGuideFontSize(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);			// ȡ�ó˿ͽ�����ͨҵ�������������С
	DWORD GetRecMessageBoxFontSize(/*LANGUAGETYPE_T language_mode*/);	    // ȡ�ó˿ͽ�����Ϣ�����������С
	DWORD GetRecMessageBoxTitleFontSize(/*LANGUAGETYPE_T language_mode*/);	// ȡ�ó˿ͽ�����Ϣ��������������С
	DWORD GetFunctBtnFontSize();										// ȡ�˿ͽ��湦�ܰ�ť�����С
	DWORD GetMenueBtnFontSize();										// ȡ�˿ͽ���˵���ť�����С
	DWORD GetFuncBtnGroupTitleFontSize();								// ȡ�˿ͽ��湦�ܰ�ť����������С
	DWORD GetLogoFontSizeCN();		                                    // LOGO���������С
	DWORD GetLogoFontSizeEN();		                                    // LOGOӢ�������С
	DWORD GetPassengerFontCodeType();												// ȡ���ַ�����Ϣ
	DWORD GetOperatorFontCodeType();												// ȡ���ַ�����Ϣ
	DWORD GetVersionFontSize();										    // ��ȡ�˿ͽ���汾��Ϣ���������С
	DWORD GetSvcStateFontSize();										// �˿ͽ������״̬�����С
	DWORD GetServiceStateFontSize();									// �˿ͽ������״̬�������С����ͣ������������ȴ��֣�

	CString GetRecStationFontName(/*LANGUAGETYPE_T language_mode*/);		// ȡ�ó˿ͽ�����ͨ��վ��������
	CString GetRecTitleFontName(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);		    // ȡ�ó˿ͽ����������
	CString GetRecDateFontName(/*LANGUAGETYPE_T language_mode*/);		    // ȡ�ó˿ͽ�����������
	CString GetRecBaseInfoFontName(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);	    // ȡ�ó˿ͽ��������Ϣ����
	CString GetRecGuideFontName(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);		    // ȡ�ó˿ͽ�����ͨҵ������������
	CString GetRecMessageBoxFontName(/*LANGUAGETYPE_T language_mode*/);		// ȡ�ó˿ͽ�����Ϣ����������
	CString GetRecMessageBoxTitleFontName(/*LANGUAGETYPE_T language_mode*/);// ȡ�ó˿ͽ�����Ϣ�������������
	//Added
	CString GetFunctBtnFontName();										// ȡ�˿ͽ��湦�ܰ�ť����
	CString GetMenueBtnFontName();										// ȡ�˿ͽ���˵���ť����
	CString GetFuncBtnGroupTitleFontName();								// ȡ�˿ͽ��湦�ܰ�ť���������
	CString GetSvcStateFontName();										// ȡ�˿ͽ������״̬����
	CString GetServiceStateFontName();									// ȡ�˿ͽ������״̬������������ͣ����ȣ���������
	
	CString GetLogoFontNameCN();		                                // LOGO������������
	CString GetLogoFontNameEN();		                                // LOGOӢ����������

	COLORREF GetRecStationFontColor();									// ȡ�˿ͽ�����ͨ��վ����������ɫ
	COLORREF GetRecTitleFontColor(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);		// ȡ�˿ͽ������������ɫ
	COLORREF GetRecDateFontColor(/*LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED*/);			// ȡ�˿ͽ�������������ɫ
	COLORREF GetRecBaseInfoFontColor(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);		// ȡ�˿ͽ��������Ϣ������ɫ
	COLORREF GetRecGuideFontColor(LANGUAGETYPE_T language_mode = LANGUAGE_ID_UNSPECIFIED);		// ȡ�ó˿ͽ�����ͨҵ��������������ɫ
	COLORREF GetRecMsgBoxFontColor(/*LANGUAGETYPE_T language_mode*/);		// ȡ�ó˿ͽ�����Ϣ������������ɫ
	COLORREF GetRecMsgBoxTitleFontColor(/*LANGUAGETYPE_T language_mode*/);	// ȡ�ó˿ͽ�����Ϣ���������������ɫ
	COLORREF GetLogoFontColorCN(LANGUAGETYPE_T language_mode);			// LOGO����������ɫ
	COLORREF GetLogoFontColorEN(LANGUAGETYPE_T language_mode);			// LOGOӢ��������ɫ

	COLORREF GetTitleBakColor();		                                // ȡ�ñ��ⱳ��ɫ
	COLORREF GetBaseBakColor();			                                // ȡ�û������򱳾�ɫ
	COLORREF GetStatusBakColor();		                                // ȡ��״̬������ɫ
	COLORREF GetFunBakColor();				                            // ȡ�ù���������ɫ
	COLORREF GetFuncBtnFontColor();
	COLORREF GetMenuBtnFontColor();
	COLORREF GetFuncBtnGroupTitleFontColor();
	COLORREF GetNormalSvcStateFontColor();								// ��������״̬������ɫ
	COLORREF GetUnNormalSvcStateFontColor();							// ����������״̬������ɫ
	COLORREF GetGuideAreaColor();										// ����Ա�����򱳾�ɫ
	COLORREF GetBaseInfoAreaColor();									// ����Ա������Ϣ���򱳾�ɫ
	COLORREF GetStateAreaColor();										// ����Ա״̬���򱳾�ɫ
	COLORREF GetFuncAreaColor();										// ����Ա�������򱳾�ɫ
	COLORREF GetServiceStateFontColor();								// �˿ͽ������״̬������ɫ 

	double GetReceptionSolutionWidthRatio();		                    // ��ȡ�˿ͻ���ֱ��ʿ�
	double GetReceptionSolutionHeightRatio();		                    // ��ȡ�˿ͻ���ֱ��ʸ�
	LOGFONT& GetBtnFont();                                              // ȡ��ť����
	LOGFONT& GetRecMenuBtnFont();										// ȡ�˵�����ť����
	double GetOperatorImageResolutionWidth();							// ȡ��ά�������ȷֱ���
	double GetOperatorImageResolutionHeight();							// ȡ��ά������߶ȷֱ���
	CString GetHistoryGridTxtFontName();								// ȡ�˿ͽ����ѯ-��ʷ��¼�����������
	DWORD GetHistoryGridTxtFontSize();									// ȡ�˿ͽ����ѯ-��ʷ��¼��������С
	COLORREF GetHistoryGridTxtFontColor();								// ȡ�˿ͽ����ѯ-��ʷ��¼���������ɫ

private:
	

	LOGFONT m_DefaultFont;
	LOGFONT m_BtnFont;
	LOGFONT m_TitleFont;
	LOGFONT	m_MenueBtnFont;				// �˿ͽ���˵���ť����

	COLORREF m_TitleBakColor;		    // ���ⱳ��ɫ
	COLORREF m_BaseBakColor;			// �������򱳾�ɫ
	COLORREF m_StatusBakColor;		    // ״̬������ɫ
	COLORREF m_FunBakColor;				// ����������ɫ

	COLORREF m_OpBaseAreaColor;			// ����Ա������Ϣ���򱳾�ɫ
	COLORREF m_OpGuidAreaColor;			// ����Ա�����򱳾�ɫ
	COLORREF m_OpStatAreaColor;			// ����Ա״̬���򱳾�ɫ
	COLORREF m_OpStateFuncAreaColor;	// ����Ա���������򱳾�ɫ
	// ����Ա����
	DWORD m_OperatorTitleFontSize[LANGUAGE_TYPE_COUNT];						// ����Ա������������С
	CStringArray m_OperatorTitleFontName;									// ����Ա���������������
	DWORD m_OperatorBtnFontSize[LANGUAGE_TYPE_COUNT];						// ����Ա���水ť�����С���ݲ�ʹ�ã�
	CStringArray m_OperatorBtnFontName;										// ����Ա���水ť�������ƣ��ݲ�ʹ�ã�
	DWORD m_OperatorNormalFontSize[LANGUAGE_TYPE_COUNT];					// ����Ա���������Ϣ�����������С 
	CStringArray m_OperatorNormalFontName;									// ����Ա���������Ϣ�������������� 
	DWORD m_OperatorGuideFontSize[LANGUAGE_TYPE_COUNT];						// ����Ա��������Ϣ�������С
	CStringArray m_OperatorGuideFontName;									// ����Ա��������Ϣ����������
	DWORD m_OperatorIniFontSize[LANGUAGE_TYPE_COUNT];						// ����Ա��ʼ�����������С  
	CStringArray m_OperatorIniFontName;										// ����Ա��ʼ��������������  
	CUIntArray m_OperatorCharSet;												// ����Ա�����ַ���
	double m_OperatorSolutionWidthRatio;									// ����Ա����ֱ���-��	
	double m_OperatorSolutionHeightRatio;									// ����Ա����ֱ���-��	

	// �˿ͽ���
	DWORD m_RecStationFontSize[LANGUAGE_TYPE_COUNT];						// �˿ͽ���״̬����վ���������С
	CStringArray m_RecStationFontName;										// �˿ͽ���״̬����վ������������
	DWORD m_RecTitleFontSize[LANGUAGE_TYPE_COUNT];							// �˿ͽ��������Ϣ�����������С
	CStringArray m_RecTitleFontName;										// �˿ͽ��������Ϣ��������������
	DWORD m_RecBaseInfoFontSize[LANGUAGE_TYPE_COUNT];						// �˿ͽ��������Ϣ�����������С
	CStringArray m_RecBaseInfoFontName;										// �˿ͽ��������Ϣ��������������
	DWORD m_RecDateFontSize[LANGUAGE_TYPE_COUNT];							// �˿ͽ���״̬������ʱ�������С
	CStringArray m_RecDateFontName;											// �˿ͽ���״̬������ʱ����������
	DWORD m_RecSvcStaeFontSize[LANGUAGE_TYPE_COUNT];						// �˿ͽ���״̬������״̬�����С
	CStringArray m_RecSvcStateFontName;										// �˿ͽ���״̬������״̬��������
	DWORD m_RecGuideInfoFontSize[LANGUAGE_TYPE_COUNT];						// �˿ͽ�������Ϣ�������С
	CStringArray m_RecGuideInfoFontName;									// �˿ͽ�������Ϣ����������
	DWORD m_RecMessageBoxFontSize[LANGUAGE_TYPE_COUNT];						// �˿ͽ�����Ϣ��ʾ�����������С
	CStringArray m_RecMessageBoxFontName;									// �˿ͽ�����Ϣ��ʾ��������������
	DWORD m_RecMessageBoxTitleFontSize[LANGUAGE_TYPE_COUNT];				// �˿ͽ�����Ϣ��ʾ����������С
	CStringArray m_RecMessageBoxTitleFontName;								// �˿ͽ�����Ϣ��ʾ�������������
	//Added
	DWORD m_FuncBtnFontSize[LANGUAGE_TYPE_COUNT];							// �˿ͽ��湦�ܰ�ť�����С
	CStringArray m_FuncBtnFontName;											// �˿ͽ��湦�ܰ�ť��������
	DWORD m_MenueBtnFontSize[LANGUAGE_TYPE_COUNT];							// �˿ͽ���˵���ť�����С
	CStringArray m_MenueBtnFontName;										// �˿ͽ���˵���ť��������
	DWORD m_FuncBtnGroupTitleFontSize[LANGUAGE_TYPE_COUNT];					// �˿ͽ��湦�ܰ�ť��������������С
	CStringArray m_FuncBtnGroupTitleFontName;								// �˿ͽ��湦�ܰ�ť�����������������
	DWORD m_ServiceStateTxtSize[LANGUAGE_TYPE_COUNT];						// �˿ͽ������״̬�����С
	CStringArray m_ServiceStateTxtName;										// �˿ͽ������״̬��������

	DWORD m_RecLogoFontSize[LANGUAGE_TYPE_COUNT];							// �˿ͽ���LOGO�����С
	CStringArray m_RecLogoFontName;											// �˿ͽ���LOGO��������
	DWORD m_RecVersionInfoFontSize;											// �˿ͽ���汾��Ϣ�����С
	double m_ReceptionSolutionWidthRatio;									// �˿ͽ���ֱ���-��	
	double m_ReceptionSolutionHeightRatio;									// �˿ͽ���ֱ���-��	
	double m_OperatorImageResolutionWidth;									// ��ά������ֱ��� - ���
	double m_OperatorImageResolutionHeight;									// ��ά������ֱ��� -	 �߶�
	
	COLORREF m_TitleFontColor[LANGUAGE_TYPE_COUNT];							// ����Ա�������������ɫ
	COLORREF m_BtnFontColor[LANGUAGE_TYPE_COUNT];							// ����Ա���水ť������ɫ(�ݲ�ʹ��)
	COLORREF m_NormalFontColor[LANGUAGE_TYPE_COUNT];						// ����Ա���������Ϣ��������������ɫ
	COLORREF m_GuideFontColor[LANGUAGE_TYPE_COUNT];							// ����Ա������������ɫ
	COLORREF m_InitFontColor[LANGUAGE_TYPE_COUNT];							// ����Ա�����ʼ��������ɫ

	COLORREF m_InitSucceedFontColor;										// ��ʼ������ɹ�������ɫ
	COLORREF m_InitFailedFontColor;											// ��ʼ�����ʧ��������ɫ
	COLORREF m_InitUnknownFontColor;										// ��ʼ�����δ֪������ɫ
	
	COLORREF m_RectStationFontColor[LANGUAGE_TYPE_COUNT];					// �˿ͽ��泵վ������ɫ
	COLORREF m_RectNormalSvcStateFontColor;									// ��������״̬������ɫ
	COLORREF m_RectUnNormalSvcStateFontColor;								// ����������������ɫ
	COLORREF m_RectTitleFontColor[LANGUAGE_TYPE_COUNT];						// �˿ͽ������������ɫ
	COLORREF m_RectDateFontColor[LANGUAGE_TYPE_COUNT];						// �˿ͽ�������������ɫ
	COLORREF m_RectBaseInfoFontColor[LANGUAGE_TYPE_COUNT];					// �˿ͽ��������Ϣ������ɫ
	COLORREF m_RectGuideFontColor[LANGUAGE_TYPE_COUNT];						// �˿ͽ�������Ϣ������ɫ
	COLORREF m_RectMsgBoxFontColor[LANGUAGE_TYPE_COUNT];					// �Ի�����Ϣ������ɫ
	COLORREF m_RectMsgBoxTitleFontColor[LANGUAGE_TYPE_COUNT];				// �Ի������������ɫ
	COLORREF m_FuncBtnFontColor[LANGUAGE_TYPE_COUNT];						// ���ܰ�ť������ɫ
	COLORREF m_MenuBtnFontColor[LANGUAGE_TYPE_COUNT];						// �˵���ť������ɫ
	COLORREF m_FuncBtnGroupTitleFontColor[LANGUAGE_TYPE_COUNT];				// ���ܰ�ť�������������ɫ
	COLORREF m_LogoFontColor[LANGUAGE_TYPE_COUNT];							// LOGO������ɫ
	COLORREF m_ServiceStateTxtColor[LANGUAGE_TYPE_COUNT];					// ����״̬������ɫ

	CStringArray m_HistoryGridFontName;										// ��ʷ��¼��������
	DWORD m_HistoryGridFontSize[LANGUAGE_TYPE_COUNT];						// ��ʷ��¼�����С
	COLORREF m_HistoryGridFontColor[LANGUAGE_TYPE_COUNT];					// ��ʷ��¼������ɫ

	static CGUIInfo theInstance;	
    CGetIni* m_GUIIni;														// INI�ļ�����

private:
	// ���ع��캯��
	CGUIInfo();
	~CGUIInfo();
	CGUIInfo(const CGUIInfo&);
	CGUIInfo& operator=(const CGUIInfo&);

	LANGUAGE_TYPE_INDEX GetPassengerLanguageIndex();						// ��ȡ�˿ͽ���������������
	LANGUAGE_TYPE_INDEX GetOperatorLanguageIndex();							// ��ȡվԱ����������������
};
