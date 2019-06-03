#include "stdafx.h"
#include "CardRWMonitorSvc.h"
#include "CAfcStaffPwdParam.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCardRWMonitorSvc::CCardRWMonitorSvc()
:CMonitorSvc(MONITOR_CARD_RW__SVC,ROOT_SVC,theTVM_INFO.GetRWMonitorInterval(TVM_CARD_RW))
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
CCardRWMonitorSvc::~CCardRWMonitorSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ض�������

@param      ��

@retval     bool �Ƿ����쳣����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CCardRWMonitorSvc::Monitor()
{
	RW_RSP_STATUS status;
	try{
		//CARDRW_HELPER->GetStatus(status);

		// ���TVMΪ��ʼ���л�ȴ��л������״̬
		if(theSERVICE_MGR.GetState() == INIT || theSERVICE_MGR.GetState() == READY || theSERVICE_MGR.GetState() == REST){
			//Do fallow
			theAPP_SESSION.SetCardOpenMainDoorOperatorID(0);
			RW_READ_CARD_RESPONSE cardRspInfo;
			long resultCode = CARDRW_HELPER->TryReadCard(MONITOR_CARD_RW__SVC,cardRspInfo,false);
			if(resultCode != RW_RSP_OK) {
				if(cardRspInfo.DetailErrorCode == RW_ERROR_CARD_NO_CARD){
	
				}
				else if (resultCode == RW_RSP_ACC_CARD_BLACKLIST || resultCode == RW_RSP_BLACK_CARD) {

				}
				// Ѱ�������ʧ��
				else if((resultCode ==  RW_RSP_READERROR) || (resultCode == RW_RSP_FINDERROR) || (resultCode == RW_RSP_FIND_CARD_FAILED) || (resultCode == RW_RSP_READ_CARD_FAILED)){		

				}
			}
			else{
				// ����Ʊ��
				CACCCard accCard;
				if(!accCard.GetCardInfo(cardRspInfo.CardInfo)) {
					//OnError(_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT), GUIDE_CHARGE_TPU_ERROR_CANNOT);
					return false;
				}	

				// ��֤Ա�������Ƿ��ڲ����д���
				CString cardLogicalNumber = ComConvToHexStr(accCard.applicationData.CardLogicalNumber, 8);
				typedef CAfcStaffPwdParam::STAFF STAFF_t;
				vector<STAFF_t> vecStaff = theAFC_STAFF.GetStaffParam();
				STAFF_t currentStaff;
				bool hasFound = false;
				for (int i = 0; i < vecStaff.size(); ++i){
					if (vecStaff.at(i).staffID == cardLogicalNumber){
						currentStaff = vecStaff.at(i);
						hasFound = true;
						break;
					}
				}
				if (!hasFound){
					return false;
				}
				else{
					theAPP_SESSION.SetCardOpenMainDoorOperatorID(currentStaff.staffID);
				}

				try{
					//IO_RSP_STATUS_INFO rsp;
					//IO_HELPER->GetIOStatus(rsp);
					//if(rsp.isBUSensorReady){
					//	IO_HELPER->OpenBUElectromagnet();
					//}

					//if(rsp.isTUSensorReady){
					//	IO_HELPER->OpenTUElectromagnet();
					//}
				}
				catch(...){

				}
			}
		}
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		return false;
	}
	return true;
}