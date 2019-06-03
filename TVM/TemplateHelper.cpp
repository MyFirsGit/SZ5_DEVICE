#include "stdafx.h"
#include "TemplateHelper.h"
#import "C:\\windows\\system32\\msscript.ocx" no_namespace

CTemplateHelper CTemplateHelper::theInstance;

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTemplateHelper::CTemplateHelper(void)
{	


}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
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
@brief     ��ȡtemplateĿ¼���ض��ļ������� 

@param CString fileName �ļ���

@retval     CString �ļ�����

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
@brief    ִ�н����ű�  

@param		CString strParseScript  ����ת����javascript
@param		CString receiptFormatDefinition  ��Ҫ������ת����ƾ����ʽ����ʽ
@param	(o)	CString &result ��������Ŀ���ִ�е����ƾ����ʽ�Ľ����

@retval		long 0 �����ɹ�����0 ���ɹ�	   

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CTemplateHelper::ExecuteParseScript(CString strParseScript, CString receiptFormatDefinition, CString &result)
{
	HRESULT hr = CoInitialize(NULL); 
	IScriptControlPtr pScriptControl(__uuidof(ScriptControl)); 
	pScriptControl->Language = _T("JavaScript"); 
	pScriptControl->AllowUI = TRUE; 
	

	// ����׼��
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
		//��Ӵ��벢���ݲ���ִ��
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
@brief     ִ�и�ʽ��ƾ���ű�

@param		CString strRunScript ��ʽ���ű�
@param	(o)	CString &result ��ʽ���ú��ƾ������

@retval     long 0ִ�гɹ�����0 ���ɹ�

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
@brief   ��ʽ��ƾ��   

@param	(i)	CString receiptModelDefinition		ƾ�������ݶ�������
@param	(i)	CString receiptFormatDefinitionFileName		ƾ����ʽ�����ļ���
@param	(o)	CStringArray& contents		���ɵ�ƾ������


@retval     bool �ɹ���ʧ��

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