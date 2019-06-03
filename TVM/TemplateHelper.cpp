#include "stdafx.h"
#include "TemplateHelper.h"
#import "C:\\windows\\system32\\msscript.ocx" no_namespace

CTemplateHelper CTemplateHelper::theInstance;

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTemplateHelper::CTemplateHelper(void)
{	


}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTemplateHelper::~CTemplateHelper(void)
{	

}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool  CTemplateHelper::LoadBillTemplate(CString templateName,CStringArray& contents)
{
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);
	CString filePath =  CString(szAppPath) +  _T("\\Template\\")+templateName;
	CXFile templateFile(filePath);
	CString strLine;
	while(!feof(templateFile.m_pStream))   
	{
		templateFile.ReadString(strLine);
		contents.Add(strLine);
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief     获取template目录下特定文件的内容 

@param CString fileName 文件名

@retval     CString 文件内容

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CTemplateHelper::GetFileContent(CString fileName)
{
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);
	CString filePath =  CString(szAppPath) +  _T("\\Template\\")+fileName;
	CXFile templateFile(filePath);
	CString strLine;
	CString content;
	while(templateFile.ReadString(strLine))   
	{
		content+=strLine+_T("\n");
	}
	return content;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    执行解析脚本  

@param		CString strParseScript  解析转换的javascript
@param		CString receiptFormatDefinition  需要被解析转换的凭条格式化样式
@param	(o)	CString &result 被解析后的可用执行的输出凭条样式的结果。

@retval		long 0 解析成功；非0 不成功	   

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CTemplateHelper::ExecuteParseScript(CString strParseScript, CString receiptFormatDefinition, CString &result)
{
	HRESULT hr = CoInitialize(NULL); 
	IScriptControlPtr pScriptControl(__uuidof(ScriptControl)); 
	pScriptControl->Language = _T("JavaScript"); 
	pScriptControl->AllowUI = TRUE; 
	

	// 参数准备
	LPSAFEARRAY psa; 
	SAFEARRAYBOUND rgsabound[]  = { 1, 0 }; // 3 elements, 0-based 
	psa = SafeArrayCreate(VT_VARIANT, 1, rgsabound); 
	VARIANT vFlavors[1]; 
	VariantInit(&vFlavors[0]); 
	V_VT(&vFlavors[0]) = VT_BSTR; 
	_bstr_t bstr = (LPTSTR)(LPCTSTR)receiptFormatDefinition;
	V_BSTR(&vFlavors[0]) = SysAllocString(bstr);
	long lZero = 0;
	hr = SafeArrayPutElement(psa, &lZero,&vFlavors[0]);
	SysFreeString(vFlavors[0].bstrVal);

	long errorNumber = -1;
	try{
		//添加代码并传递参数执行
		pScriptControl->AddCode((LPTSTR)(LPCTSTR)strParseScript);
		_variant_t outpar = pScriptControl->Run("parse", &psa); 

		errorNumber = pScriptControl->GetError()->GetNumber();
		if(errorNumber==0)
		{
			result = (TCHAR *)(_bstr_t)outpar;
		}
	}
	catch(...)
	{

	}
	SafeArrayDestroy(psa); 
	pScriptControl->Release();
	pScriptControl.Detach();
	CoUninitialize(); 

	return errorNumber;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     执行格式化凭条脚本

@param		CString strRunScript 格式化脚本
@param	(o)	CString &result 格式化好后的凭条内容

@retval     long 0执行成功，非0 不成功

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long  CTemplateHelper::ExecuteRunScript(CString strRunScript,CString &result)
{
	
	HRESULT hr = CoInitialize(NULL); 
	IScriptControlPtr pScriptControl(__uuidof(ScriptControl)); 

	SAFEARRAYBOUND rgsabound[]  = { 0, 0 }; // 3 elements, 0-based 
	LPSAFEARRAY psa= SafeArrayCreate(VT_VARIANT, 1, rgsabound); 
	pScriptControl->Language = _T("JavaScript"); 
	pScriptControl->AllowUI = TRUE; 

	pScriptControl->AddCode((LPTSTR)(LPCTSTR)strRunScript);
	
	_variant_t outpar = pScriptControl->Run("run", &psa); 
	
	long errorNumber = pScriptControl->GetError()->GetNumber();
	if(errorNumber==0)
	{
		result = (TCHAR *)(_bstr_t)outpar;
	}

	SafeArrayDestroy(psa); 
	pScriptControl->Release();
	pScriptControl.Detach();
	CoUninitialize(); 
	
	return errorNumber;

}

//////////////////////////////////////////////////////////////////////////
/*
@brief   格式化凭条   

@param	(i)	CString receiptModelDefinition		凭条的数据定义内容
@param	(i)	CString receiptFormatDefinitionFileName		凭条格式定义文件名
@param	(o)	CStringArray& contents		生成的凭条内容


@retval     bool 成功或失败

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTemplateHelper::FormatReceipt(CString receiptModelDefinition, CString receiptFormatDefinitionFileName,CStringArray& contents)
{
	CString strParseScript = GetFileContent(_T("Parse.js"));
	CString strReceiptFormatScript;
	bool parseSuccess = ExecuteParseScript(strParseScript,GetFileContent(receiptFormatDefinitionFileName),strReceiptFormatScript) == 0;
	if(!parseSuccess)
	{
		return false;
	}
	CString strRunScript = _T("\
							  function run(){ \n \
							  var Data={}; \n \
							  [RECEIPT_MODEL_DEFINITION] ;\n\
							  [RECEIPT_FORMAT_DEFINITION] ;\n\
							  return template;\
							  }\
							  ");
	strRunScript.Replace(_T("[RECEIPT_MODEL_DEFINITION]"),receiptModelDefinition);
	strRunScript.Replace(_T("[RECEIPT_FORMAT_DEFINITION]"),strReceiptFormatScript);

	CString strReceipt;
	bool formatSuccess = ExecuteRunScript(strRunScript,strReceipt)==0;
	if(!formatSuccess)
	{
		return false;
	}
	GetSplitStrArray(strReceipt,_T("\n"),contents);
	return true;
};