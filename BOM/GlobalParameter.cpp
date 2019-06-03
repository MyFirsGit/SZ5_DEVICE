#include "stdafx.h"
#include "GlobalParameter.h"
#include "getIni.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const    EXCEPTION_FILE_NAME    = _T("EXCEPTION.CSV");// �쳣��Ϣ�ļ���

CGlobalParameter  CGlobalParameter::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ���캯��
                                                                 
 @param      (i)void

 @retval      ��

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CGlobalParameter::CGlobalParameter(void)
{
	m_uHour = 0;
	m_uMinute = 0;
	m_uSecond = 0;
	m_recoverTimes = 0;
	m_vfareRecoverTimes = 0;

	//TCHAR szAppPath[_MAX_PATH];
	//ComGetAppPath(szAppPath,_MAX_PATH);
	//CString szBOMINIFName = CString(szAppPath) + BOM_INI_FILE_NAME;
	//CGetIni GetIni(szBOMINIFName);
	//
	//theBOM_INFO.Get
    //m_maxRecoverTimes = (unsigned long)GetIni.GetDataInt("APPLICATION", "MaxRecoverTimes", 0);

    m_bNeedRestart = false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)void

@retval      ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CGlobalParameter::~CGlobalParameter(void)
{
    Release();
	delete m_expfile; 
	m_expfile = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ͷ���Դ

@param      ��

@retval      ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CGlobalParameter::Release()
{
    POSITION pos = NULL;
    LPEXCEPTION_DETAIL  exDetail = NULL;
    CString sKey=_T("");
    pos = m_mapException.GetStartPosition();

    while (pos != NULL) {
        m_mapException.GetNextAssoc(pos, sKey, exDetail);
        delete exDetail;
    }

    m_mapException.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ӫ�˿�ʼʱ��

@param      (i)unsigned short Hour����Сʱ
@param      (i)unsigned short Minute������

@retval      bool 
              true:�ɹ��� false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CGlobalParameter::SetBusiDateSwitchTime(unsigned char Hour, unsigned char Minute,unsigned char Second)
{
	if (Hour > 23 || Minute > 59 || Second > 59) {
		return false;
	}
	m_uHour = Hour;
	m_uMinute = Minute;
	m_uSecond = Second;
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�Ӫ�˿�ʼʱ��

@param      (i)unsigned short& Hour����Сʱ
@param      (i)unsigned short& Minute������
@param      (i)unsigned short& Second����

@retval      bool 
               true:�ɹ��� false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CGlobalParameter::GetBusiDateSwitchTime(unsigned char& Hour, unsigned char& Minute, unsigned char& Second)
{
	if (m_uHour > 23 || m_uMinute > 59 || m_uSecond > 59) {
		return false;
	}

	Hour = m_uHour;
	Minute = m_uMinute;
	Second = m_uSecond;
	return true;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief       �ļ��쳣�ָ�������1

@param       ��

@retval      ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CGlobalParameter::IncRecoverTimes()
{
    m_recoverTimes++;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����Ʊ�۱�ǰ�ָ�����(�ۼ�)

@param       (i) int vfareRecoverTimes �������

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CGlobalParameter::UpdateVfareRecoverTimes(int vfareRecoverTimes)
{
	m_vfareRecoverTimes = m_vfareRecoverTimes + vfareRecoverTimes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �õ�Ʊ�۱�ǰ�ָ�����

@param       ��

@retval      unsigned long  Ʊ�۱�ǰ�ָ�����

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
unsigned long CGlobalParameter::GetCurVfareRecoverTimes()
{
	return m_vfareRecoverTimes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �õ��ļ��������ָ�����

@param       ��

@retval      unsigned long  �ļ��������ָ�����

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
unsigned long CGlobalParameter::GetFileMaxRecoverTimes()
{
	return m_maxRecoverTimes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ��ļ��쳣�ָ�����

@param      (i)void

@retval      CGlobalParameter& ��CGlobalParameter�����
               
@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CGlobalParameter& CGlobalParameter::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ񳬹����ָ�����

@param      (i)void

@retval      bool:
               true:������������false:δ����������

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CGlobalParameter::OverMaxRecoverTimes()
{
	if (m_recoverTimes > m_maxRecoverTimes) {
		return true;
	}
	else{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ񳬹�Ʊ�۱����ָ�����

@param      (i)void

@retval      bool:		\n
				true:������������false:δ����������

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CGlobalParameter::OverMaxVfareRecoverTimes()
{
	bool ret = false;
	if (m_vfareRecoverTimes > m_maxRecoverTimes) {
		ret = true;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����쳣��Ϣ��ֵ

@param      (i)EXCEPTION_MAP& exMap�����쳣��Ϣ����

@retval      unsigned int: 0:�ɹ�����0:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
unsigned int CGlobalParameter::GetExceptionDetail()
{
	unsigned int iRet = 0;
	CString strline = _T(""); 
	CString strKey = _T(""); 
	CString strID = _T("");
	CString strDetailID = _T("");
	CString strError = _T("");
	CString strDetail = _T("");
	CStringArray *strExpArray = NULL;

	CString sAppPath;
	CString filename;
	// ��ʼ���쳣�ļ�
	GetAppPath(sAppPath);
	filename = sAppPath + EXCEPTION_FILE_NAME;	
	CXFile *expfile = new CXFile(filename);
	// ���ж�ȡ�ļ�
	expfile->ReadString(strline);  
	while(!feof(expfile->m_pStream))   
	{   
		expfile->ReadString(strline); 	
		if(strline.GetLength() == 0){
			continue;
		}
		strExpArray = SplitCString(strline,_T(","));
		
		int cnt = strExpArray->GetSize();
		if(cnt != 6){
			iRet = 1;
			return iRet;
		}
		else{
			LPEXCEPTION_DETAIL  pedData = new EXCEPTION_DETAIL;

			// ������
			pedData->lID = _ttol(strExpArray->GetAt(0));
			strKey = strExpArray->GetAt(0);
			// ������ϸ���
			pedData->lDetail_ID = _ttol(strExpArray->GetAt(1));
			strKey += strExpArray->GetAt(1);
			// MTC���
			pedData->lMTC = _ttol(strExpArray->GetAt(2));
			// EC���
			pedData->lEc = _ttoi(strExpArray->GetAt(3));
			// ��������
			strError = strExpArray->GetAt(4);
			// ������ϸ����
			strDetail = strExpArray->GetAt(5);

			pedData->sErrLen = strError.GetLength() + 1;
			pedData->sDealLen = strDetail.GetLength() + 1;
			pedData->sErr = new char[pedData->sErrLen];
			pedData->sDeal = new char[pedData->sDealLen];
			memset(pedData->sErr, 0, pedData->sErrLen);
			memset(pedData->sDeal, 0, pedData->sDealLen);
			memcpy(pedData->sErr, strError, strError.GetLength());
			memcpy(pedData->sDeal, strDetail, strDetail.GetLength());
			strID.Format(_T("%d"), pedData->lID);
			strDetailID.Format(_T("%d"), pedData->lDetail_ID);
			strKey = strID + strDetailID;
			m_mapException.SetAt(strKey, pedData);	
		}			
	}   
	if(strExpArray != NULL){
		delete strExpArray;
		strExpArray = NULL;
	}
	return iRet;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ��쳣��Ϣ��ֵ

@param      (i)EXCEPTION_DETAIL& exMsg �쳣��Ϣ�ṹ
@param      (i)long lId                �쳣��ϢID
@param      (i)lDetailID               �쳣��ϢDetailID

@retval      unsigned int:
              0:û�ҵ�����0:�ҵ�

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
unsigned int CGlobalParameter::GetExceptionByID(EXCEPTION_DETAIL& exMsg, long lId, long lDetailID)
{
    unsigned int iRet = 0;
	CString sKey = 0;
	CString buff1, buff2;
	LPEXCEPTION_DETAIL exMsgOne = NULL;
	
	if (lId < 10000) {
		lId = 1000;
	}
	buff1.Format(_T("%d"), lId);
	buff2.Format(_T("%d"), lDetailID);
	sKey = buff1 + buff2;
	m_mapException.Lookup(sKey, exMsgOne);
	if (NULL != exMsgOne) {
		exMsg = *exMsgOne;
		iRet = 1;
	}else{
		iRet = 0;
	}
	return iRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���õ�ǰϵͳ�Ƿ���Ҫ������������

@param      (i)bool bNeedRestart = true   �Ƿ���Ҫ�������� \n
               true: ��Ҫ ; false ����Ҫ

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CGlobalParameter::SetbNeedRestart(bool bNeedRestart)
{
    m_bNeedRestart = bNeedRestart;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��õ�ǰϵͳ�Ƿ���Ҫ��������

@param       void

@retval      bool  \n
             true: ��Ҫ ; false ����Ҫ

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CGlobalParameter::GetbNeedRestart()
{
    return m_bNeedRestart;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ڻ��ļ��쳣�ָ�����

@param      none

@retval     void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CGlobalParameter::InitRecoverTimes()
{
	m_recoverTimes = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ڻ��ļ��쳣�ָ�����

@param      none

@retval     void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CGlobalParameter::InitVfareRecoverTimes()
{
	m_vfareRecoverTimes = 0;
}

