#include "stdafx.h"
#include "CHCommand.h"
#include "CHException.h"
#include "CHHardwareException.h"
//#include "bh_proto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
/**
 @brief		构造函数

 @param		WORD num_OneY : 1元个数
			WORD num_HalfY : 5角个数

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCHChange::CCHChange(WORD num_OneY,WORD num_HalfY)
{
	m_num_OneY = num_OneY;
	m_num_HalfY = num_HalfY;

	memset(&changeSet, 0, sizeof(changeSet));
	changeSet.wMHopper1Num = m_num_OneY;
	changeSet.wMHopper2Num = m_num_HalfY;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCHChange::~CCHChange()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		执行命令

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCHChange::ExecuteCommand()
{
	long lRet = 1;
	lRet = ChangeFunction();
	this->OnComplete(lRet);
	return lRet;
}


long CCHChange::GetResponse(tCHAChangeNum* rsp)
{
	memcpy(rsp, &changeSet, sizeof(tCHAChangeNum));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		找零

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCHChange::ChangeFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("CHANGE"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	sLogString.Format(_T("iChangeEx: nChangeCoin1 = %d, nChangeCoin2 = %d"), m_num_OneY, m_num_HalfY);
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CH_ERROR;

	tCHADevReturn DevStatus = {0};
	errCode = CHA_ChangeCoin(&changeSet,&DevStatus);
	if (errCode != E_NO_CH_ERROR){
		sLogString.Format(_T("iChangeEx: change failed! errCode = %d, interface return change1 = %d, change2 = %d"), errCode, changeSet.wMHopper2Num, changeSet.wMHopper1Num);
		theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

		//if(changeSet.wMHopper1Num==m_num_OneY && changeSet.wMHopper2Num==m_num_HalfY){//暂时发现，找零失败，未找出硬币时，接口返回的找零数目等于请求的找零数目，因此加上这样的判断。
		//	changeSet.wMHopper1Num = 0;
		//	changeSet.wMHopper2Num = 0;
		//}
	}
#if 0
	tModuleStatus sModuleStatus;
	tDevReturn sDevReturn[8];
	tChange chagneSet = {0};
	chagneSet.byMHopper1Num = m_num_HalfY;
	chagneSet.byMHopper2Num = m_num_OneY;
	/*
	注意事项
		1、针对存在找零暂存的硬币模块，调用找零接口后必须调用该接口才能完成找零操作。
		2、如果找零成功，可以控制换向器转向左侧；如果找零失败，可以控制换向器转向右侧。
	*/
	errCode = gOCAS002Dev.iChangeEx(&chagneSet, &sModuleStatus, sDevReturn);

	int nNo = 1;	// 换向器编号 1-1号 2-2号 
	int nDir = 1;   // 运动方向 1 = 左边(倒到出口方向) 2 = 右边（倒入钱箱方向） 3 = 中间 
	if (errCode != E_NO_CH_ERROR){
		errCode = sDevReturn[0].iLogicCode;	
		sLogString.Format(_T("iChangeEx: change failed! errCode = %d, interface return change1 = %d, change2 = %d"), errCode, chagneSet.byMHopper2Num, chagneSet.byMHopper1Num);
		theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
		gOCAS002Dev.iRouterControlEx(nNo, nDir, &sModuleStatus, sDevReturn);

		if(chagneSet.byMHopper2Num==m_num_OneY && chagneSet.byMHopper1Num==m_num_HalfY){//暂时发现，找零失败，未找出硬币时，接口返回的找零数目等于请求的找零数目，因此加上这样的判断。
			chagneSet.byMHopper1Num = 0;
			chagneSet.byMHopper2Num = 0;
		}
	}
	else{
		gOCAS002Dev.iRouterControlEx(nNo, nDir, &sModuleStatus, sDevReturn);
	}
#endif

	//m_rsp.wOneYuanCount = changeSet.wMHopper1Num;
	//m_rsp.wHalfYuanCount = changeSet.wMHopper2Num;

	sLogString.Format(_T("iChangeEx: hopperChange1 = %d, hopperChange2 = %d"), changeSet.wMHopper1Num, changeSet.wMHopper2Num);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	sLogString.Format(_T("iChangeEx: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}
