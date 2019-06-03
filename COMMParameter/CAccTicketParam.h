#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"
#include "CommonDefine.h"

// CAccTicketParam的唯一接口
#define theACC_PRODUCT CAccTicketParam::GetInstance()        
#define theACC_TICKET CAccTicketParam::GetInstance()
/**
@brief   票卡属性参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccTicketParam :public CAccParamMaster
{
public: 

	// 服务提供商参数记录体
	typedef struct _tickets_Type {
		BYTE ticketTypebaseCode;				//车票主类型代码
		BYTE ticketTypeCode;				//车票子类型代码
		DWORD MaxBalance;                       //最大余额/次上限
		BYTE chargeable;                        //是否可充值
		DWORD maxChargeSize;                    //充值上限
		BYTE payMathodOfAdjuest;                //更新时收费方式
		BYTE refoundable;						//是否可退款
		BYTE sendBackablecLimitCount;			//退款时使用次数限制
		BYTE hasLimitOfOneday;                  //是否限制日乘坐次数
		BYTE maxMountOfOneDay;					//日乘坐次数限制
		BYTE prechargeable;						//是否允许预赋值
		WORD vilidateData;				    	//固定截止有效日期
		WORD extendTime;                        //可延长时间 
		LONG deposit;							//票卡售卖押金
		BYTE needActive;                        //使用前是否需要激活
		BYTE checkBlackList;                    //是否检查黑名单
		BYTE checkBalance;                      //是否检查余额/余次
		BYTE checkValidate;                     //是否检查有效期
		BYTE checkSequence;                     //是否检查进出次序
		BYTE checkOvertime;                     //是否检查超时
		BYTE checkOverJourney;                  //是否检查超乘
		BYTE checkLimitLine;                    //是否限制线路
		BYTE checkOnlyThis;                     //是否只允许本站进出
		BYTE checkStickTime;                    //是否限制站内停留时间
		WORD maxStickTime;                      //站内停留时间上限
		BYTE chargeDevice;                      //可充值的设备
		BYTE UseableDevice;                     //可使用的设备
		BYTE checkInFlag;                       //本站进出控制标记
		BYTE favourable;				        //是否允许尾程优惠
		BYTE recorverFlag;				        //回收标识
		BYTE chargeType;						//计费方式 01：不计费，02：按费率表，03：按次
		BYTE overdraftFlag;				     	//透支标识
		BYTE checkMinBalanceFlag;		        //储值票最小余额检查
		BYTE checkMaxBalanceFlag;		        //最大余额检查
		BYTE checkEnterTime[3];		            //进站时间检查
		DWORD refoundFee;						//退票手续费
		DWORD exchangeFee;						//换票手续费
		DWORD amercementOfloseCard;				//回收类票丢票罚款额
		DWORD amercementOfBrokenCard;			//回收类票人为损坏罚款额
		DWORD overTimeFee;						//超时罚款额

		CString ticketTypeName_cn;
		CString ticketTypeName_en;
		DWORD activeFlag;



		_tickets_Type(){
			ticketTypeName_cn ="";
			ticketTypeName_en ="";
			 ticketTypebaseCode=0;				//车票主类型代码
			 ticketTypeCode=0;				//车票子类型代码
			 MaxBalance=0;                       //最大余额/次上限
			 chargeable=0;                        //是否可充值
			 maxChargeSize=0;                    //充值上限
			 payMathodOfAdjuest=0;                //更新时收费方式
			 refoundable=0;						//是否可退款
			 sendBackablecLimitCount=0;			//退款时使用次数限制
			 hasLimitOfOneday=0;                  //是否限制日乘坐次数
			 maxMountOfOneDay=0;					//日乘坐次数限制
			 prechargeable=0;						//是否允许预赋值
			 vilidateData=0;				    	//固定截止有效日期
			 extendTime=0;                        //可延长时间 
			 deposit=0;							//票卡售卖押金
			 needActive=0;                        //使用前是否需要激活
			 checkBlackList=0;                    //是否检查黑名单
			 checkBalance=0;                      //是否检查余额/余次
			 checkValidate=0;                     //是否检查有效期
			 checkSequence=0;                     //是否检查进出次序
			 checkOvertime=0;                     //是否检查超时
			 checkOverJourney=0;                  //是否检查超乘
			 checkLimitLine=0;                    //是否限制线路
			 checkOnlyThis=0;                     //是否只允许本站进出
			 checkStickTime=0;                    //是否限制站内停留时间
			 maxStickTime=0;                      //站内停留时间上限
			 chargeDevice=0;                      //可充值的设备
			 UseableDevice=0;                     //可使用的设备
			 checkInFlag=0;                       //本站进出控制标记
			 favourable=0;				        //是否允许尾程优惠
			 recorverFlag=0;				        //回收标识
			 chargeType=0;						//计费方式
			 overdraftFlag=0;				     	//透支标识
			 checkMinBalanceFlag=0;		        //储值票最小余额检查
			 checkMaxBalanceFlag=0;		        //最大余额检查
			 memset(checkEnterTime,0,3);		            //进站时间检查
			 refoundFee=0;						//退票手续费
			 exchangeFee=0;						//换票手续费
			 amercementOfloseCard=0;				//回收类票丢票罚款额
			 amercementOfBrokenCard=0;			//回收类票人为损坏罚款额
			 overTimeFee=0;						//超时罚款额
			 activeFlag=1;
 	
		}
	}TICKET_TYPE;

	

	// 共有接口
	const vector<TICKET_TYPE>&  GetTicketTypeListParam();										            // 获取所有车票类型参数
	const int  GetTicketInfo(WORD ticketTypeCode,TICKET_TYPE& ticket_type_Param);									// 获取指定类型的车票参数
	const int  GetTicketValidate(WORD ticketTypeCode,_DATE_TIME& validate);										// 获取指定类型的车票有效日期
	const int  GetTicketDeposit(WORD ticketTypeCode,LONG& deposit);												// 获取指定类型的车票押金信息
	CString  GetProductName(WORD ticketTypeCode,LANGUAGETYPE_T language);		// 获取指定车票类型的中文名称及英文名称
	BYTE  GetTicketCategry(WORD ticketTypeCode);								// 获取指定类型的车票的计费类型
	const int  GetTicketCategry(WORD ticketTypeCode,ProductCategory_t& ticket_categry);								// 获取指定类型的车票的计费类型
	const int  GetTicketTypeName(WORD ticketTypeCode,CString& ticketTypeName_cn,CString& ticketTypeName_en);		// 获取指定车票类型的中文名称及英文名称

	const BYTE GetTicketMainType(BYTE ticketSubTypeCode);														// 根据车票子类型获取车票的主类型
	bool IsNotCollectedTicket(BYTE ticketSubTypeCode);
	static CAccTicketParam&  GetInstance()
	{
		return theInstance;
	}

protected:

	vector<TICKET_TYPE>  m_vec_ticketsTypeList_param;			// 所有车票参数
	vector<TICKET_TYPE>  m_vec_ticketTypeList_param;			// 指定车票参数
	
private:

	SECTION_INFO m_section[1];	//分段信息

	// 私有成员变量
	

	// 私有成员函数
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAccTicketParam(void);
	~CAccTicketParam();
	CAccTicketParam(const CAccTicketParam&);
	CAccTicketParam& operator=(const CAccTicketParam&);

	static  CAccTicketParam theInstance;
};


typedef CAccTicketParam::TICKET_TYPE		ACC_PRODUCT_t;