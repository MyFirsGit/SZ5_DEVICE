#include "stdafx.h"
#include "ParamMsg.h"
#include "ParamHelper.h"
#include "maintenanceinfo.h"

CParamMsg CParamMsg::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      CParamMsg���캯��
@param      ��
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
CParamMsg::CParamMsg()
{
	SetAccParameterVersion();
	SetAfcParameterVersion();
	SetECTParameterVersion();
	m_wDelayMode = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CParamMsg��������
@param      ��
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
CParamMsg::~CParamMsg()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ��ʵ��
@param      ��
@retval     CParamMsg&
*/
//////////////////////////////////////////////////////////////////////////
CParamMsg& CParamMsg::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡACC�����汾������ǰ��ͽ����棩
@param      ��
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CParamMsg::SetAccParameterVersion()
{
 //   // ACC������
	//m_cur_version_acc.lBlackListParam = theACC_BLACKLIST.GetCurMasterVer();
	//m_future_version_acc.lBlackListParam = theACC_BLACKLIST.GetFutMasterVer();
 //   // ACCҵ��������
	//m_cur_version_acc.lBusinessListParam = theACC_BUSINESS.GetCurMasterVer();
	//m_future_version_acc.lBusinessListParam = theACC_BUSINESS.GetFutMasterVer();
	//// ACC�����汾����
	//m_cur_version_acc.lLocationParam = theACC_LOCATION.GetCurMasterVer();
	//m_future_version_acc.lLocationParam = theACC_LOCATION.GetFutMasterVer();
 //   // ACC��Ʒ����
	//m_cur_version_acc.lProductParam = theACC_PRODUCT.GetCurMasterVer();
	//m_future_version_acc.lProductParam = theACC_PRODUCT.GetFutMasterVer();
 //   // ACCϵͳ������
	//m_cur_version_acc.lSystemParam = theACC_SYSTEM.GetCurMasterVer();
	//m_future_version_acc.lSystemParam = theACC_SYSTEM.GetFutMasterVer();
 //   // ACC�汾������
	//m_cur_version_acc.lVersionInfoParam = theACC_VERSION.GetCurMasterVer();
	//m_future_version_acc.lVersionInfoParam = theACC_VERSION.GetFutMasterVer();
 //   // ��������
	//m_cur_version_acc.lCalenderParam = 0;
	//m_future_version_acc.lCalenderParam = 0;
	//// ���ʲ���
	//m_cur_version_acc.lFareTableParam = 0;
	//m_future_version_acc.lFareTableParam = 0;
 //   // ���۲���
	//m_cur_version_acc.lSellTableParam = 0;
	//m_future_version_acc.lSellTableParam = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡһ��ͨ�����汾
@param      ��
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CParamMsg::SetECTParameterVersion()
{
	//// һ��ͨ��Ƭ���Զ��������M3��
	//m_version_ect.lCardPropertyParam = theECT_CARD_PROPERTY.GetCurMasterVer();
	//// һ��ͨ������������(��ֵ������)
	//m_version_ect.lChargeBlacklistParam = theECT_BLACKLIST_CHARGE.GetCurMasterVer();
	//// һ��ͨ��ֵ�ն�ͨѶ����
	//m_version_ect.lConnectionParam = theECT_CONNECTION.GetCurMasterVer();
	//// һ��ͨ�ƴο���ֵҵ�����
	//m_version_ect.lMultirideChargeParam = theECT_MULTIRIDE_CHARGE.GetCurMasterVer();
	//// һ��ͨ���ڿ���ֵҵ�����
	//m_version_ect.lPassChargeParam = theECT_PASS_CHARGE.GetCurMasterVer();
	//// һ��ͨ��ֵ����ֵҵ�����
	//m_version_ect.lPurseChargeParam = theECT_PURSE_CHARGE.GetCurMasterVer();
	//// һ��ͨ�˿�ҵ�����
	//m_version_ect.lRefundParam = theECT_REFUND.GetCurMasterVer();
	//// һ��ͨ���ѿ��ÿ����Ͳ�����
	//m_version_ect.lShopAbleCardParam = theECT_ENABLE_CARD.GetCurMasterVer();
	//// һ��ͨ������������(��ֵ������)
	//m_version_ect.lShopBlacklistParam = theECT_BLACKLIST_SHOP.GetCurMasterVer();
	//// һ��ͨ�����ն��޶����
	//m_version_ect.lShopLimitParam = theECT_LIMIT.GetCurMasterVer();
	//// ��ҵ�����˲���
	//m_version_ect.lTransferParam = theECT_TRANSFER.GetCurMasterVer();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡAFCϵͳ�����汾
@param      ��
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CParamMsg::SetAfcParameterVersion()
{
	//// һ��ͨҵ�񲹳����
	//m_version_afc.lAddParam = theAFC_ECTADD.GetCurMasterVer();
	//// �豸Ȩ�޲�����
	//m_version_afc.lAccessLevelParam = theAFC_ACCESS_LEVEL.GetCurMasterVer();
	//// BOM��Ӫ������
	//m_version_afc.lTVMOperationParam = theAFC_TVM_OPERATION.GetCurMasterVer();
	//// �豸����������
	//m_version_afc.lDeviceCommonParam = theAFC_COMMON.GetCurMasterVer();
	//// ����Ա������
	//m_version_afc.lStaffPwdParam = theAFC_STAFF.GetCurMasterVer();
	//// �豸����ʱ�����
	//m_version_afc.lDeviceRunTimeParam = 0;
}

void CParamMsg::Set24HourMode(BYTE bMode)
{
	m_b24HourMode = bMode;
}

BYTE CParamMsg::Get24HourMode()
{
	return m_b24HourMode;
}

void CParamMsg::SetOperationMode(BYTE bMode)
{
	m_bOperationMode = bMode;
}

BYTE CParamMsg::GetOperationMode()
{
	return m_bOperationMode;
}

void CParamMsg::SetDelayMode(BYTE wMode)
{
	m_wDelayMode = wMode;
}

BYTE CParamMsg::GetDelayMode()
{
	return m_wDelayMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      д��parameter message

@param      (i)WORD wMsgCode			            messege code
@param 	    (i)const VARIABLE_DATA& pParamtMsg		writing data

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CParamMsg::SaveMsg(LPBYTE lpData)
{
	theDEBUG_LOG->WriteData(_T("CParamMsg"), _T("SaveMsg"), __LINE__, _T("<-"));
	try {
		LPBYTE lpMsg = lpData;
		char* pData = (char*)lpMsg;

		// ��ȡ��Ϣ�ܳ���
		ULONG pDataSize = 0;
		memcpy(&pDataSize, lpMsg+2, 4);

		// ȡ��1002�����汾��
		lpMsg += 8;
		CString csVersion = _T("");
		int iVersion = 0;
		memcpy(&iVersion, lpMsg, 4);
		csVersion.Format(_T("%d"),iVersion);

		// ɾ���ɰ汾�����ļ�
		CString sOldFileName = thePARAM_HELPER.GetTheParamPathName(AFC_MODEHISTORY_ID/*0x1002*/,PARAM_AFC_CURRENT_DIR);
		DeleteFile(sOldFileName);

		CString sOldBackUpFileName = thePARAM_HELPER.GetTheParamBackUpPathName(AFC_MODEHISTORY_ID/*0x1002*/,PARAM_AFC_CURRENT_DIR);
		DeleteFile(sOldBackUpFileName);

		// д1002�����ļ�
		CString stationCode(_T(""));
		stationCode.Format(_T("0000%.4x"),theMAINTENANCE_INFO.GetStationCodeWithLine());
		//CString csFileName = theTVM_INFO.GetDataPath() + PARAM_AFC_CURRENT_DIR + AFC_PARAM_ID_1090/*AFC_PARAM_ID_1002*/  + _T("00000000.") + csVersion +_T(".00");
		//CString csFileNameBackup = theTVM_INFO.GetBackupPath()+ PARAM_AFC_CURRENT_DIR + AFC_PARAM_ID_1090/*AFC_PARAM_ID_1002*/ + _T("00000000.") + csVersion +_T(".00");
		//auto_ptr<CDXFile> FileMgr(new CDXFile(csFileName,csFileNameBackup));
		//FileMgr->WriteBinFile(pData, pDataSize);

		// ȡ��������Ŀ¼
		TCHAR szRunPath[MAX_PATH];
		ComGetAppPath(szRunPath, MAX_PATH);
		CString csFileName = CString(szRunPath) + PM_UPDATE_DIR + AFC_PARAM_ID_1002  + _T("00000000.") + csVersion +_T(".00");
		CFile cFile;
		CFileException cfException;
		if(!cFile.Open( csFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary,&cfException)){
			TCHAR   strCause[ERROR_MSG_MAX_LEN]; 
			CString strException; 
			cfException.GetErrorMessage(strCause, sizeof(strCause), NULL); 
			strException = "�쳣��"; 
			strException += strCause; 
			throw CTxnDataException(CTxnDataException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
		}
		try{
			cFile.Write(pData,pDataSize);
			cFile.Flush();
			cFile.Close();
		}
		catch(CSysException &)
		{
			cFile.Close();
			throw;
		}
		catch(...){
			cFile.Close();
			throw CTxnDataException(CTxnDataException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__); 
		}


		theDEBUG_LOG->WriteData(_T("CParamMsg"), _T("SaveMsg"), __LINE__, _T("->"));
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
}