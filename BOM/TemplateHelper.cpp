#include "stdafx.h"
#include "TemplateHelper.h"


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