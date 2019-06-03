#pragma once


#define theTEMPLATE_HELPER CTemplateHelper::GetInstance()

class CTemplateHelper
{
public:
	static CTemplateHelper&  GetInstance(){
		return theInstance;
	}
	
	bool  LoadBillTemplate(CString templateName,CStringArray& contents);
	


	bool FormatReceipt(CString receiptModelDefinition, CString receiptFormatDefinitionFileName,CStringArray& contents);
	

private:
	long ExecuteParseScript(CString strParseScript,CString receiptFormatDefinition,CString &result);
	long ExecuteRunScript(CString strRunScript,CString &result);
	CString GetFileContent(CString fileName);
	// 构造,析构函数
	CTemplateHelper(void);
	~CTemplateHelper();
	CTemplateHelper(const CTemplateHelper&);
	CTemplateHelper& operator=(const CTemplateHelper&);
	
	static  CTemplateHelper theInstance;   // 该类的唯一接口


};