#pragma once


#define theTEMPLATE_HELPER CTemplateHelper::GetInstance()

class CTemplateHelper
{
public:
	static CTemplateHelper&  GetInstance(){
		return theInstance;
	}
	
	bool  LoadBillTemplate(CString templateName,CStringArray& contents);

private:

	// ����,��������
	CTemplateHelper(void);
	~CTemplateHelper();
	CTemplateHelper(const CTemplateHelper&);
	CTemplateHelper& operator=(const CTemplateHelper&);

	static  CTemplateHelper theInstance;   // �����Ψһ�ӿ�
};