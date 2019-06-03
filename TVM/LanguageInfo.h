#pragma once

#define theLANGUAGE_INFO CLanguageInfo::GetInstance()

#define _tl(x) theLANGUAGE_INFO.GetLanguageInfo(x)

#define _opl(x) theLANGUAGE_INFO.GetLanguageInfo((x),-1,2)

#define _cn(x) theLANGUAGE_INFO.GetLanguageInfo((x),LANGUAGE_ID_CHINESE)

#define _en(x) theLANGUAGE_INFO.GetLanguageInfo((x),LANGUAGE_ID_ENGLISH)

#define _po(x) theLANGUAGE_INFO.GetLanguageInfo((x),LANGUAGE_ID_PORTUGUESE)


class CLanguageInfo
{
public:
	static CLanguageInfo& GetInstance();

	CString GetLanguageInfo(int, int Model=-1,int positionType=1);				// 获取语言  positionType=1 乘客界面，2站员界面
private:
	typedef struct _language_data{
		CString msgCode;
		CString strTraditionalChinese;
		CString strEnglish;
		CString strPortuguese;
	}LANGUAGE_DATA,*LPLANGUAGE_DATA;

	CLanguageInfo();
	~CLanguageInfo();

	bool Initialize();									// 初始化

	CString GetTraditionalChinese(LANGUAGE_MSG);		// 获取繁体中文
	CString GetEnglish(LANGUAGE_MSG);					// 获取英文
	CString GetPortuguese(LANGUAGE_MSG);				// 获取葡萄牙文

	static CLanguageInfo theInstance;
	CMap<CString,LPCTSTR,LPLANGUAGE_DATA,LPLANGUAGE_DATA> m_mapLanguageInfo;
};