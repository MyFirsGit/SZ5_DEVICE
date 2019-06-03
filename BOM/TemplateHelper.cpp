#include "stdafx.h"
#include "TemplateHelper.h"


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
	CStdioFile file;
	CFileException cfeError;
	
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);
	CString filePath =  CString(szAppPath) +  _T("\\Template\\")+templateName;
	if(!file.Open(filePath,CFile::modeRead|CFile::typeText,&cfeError))
	{
		return false;
	}
	CString line;
	while(file.ReadString(line))
	{
		contents.Add(line);
	}
	file.Close();
	return true;
}