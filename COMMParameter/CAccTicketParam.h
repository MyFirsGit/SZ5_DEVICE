#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"
#include "CommonDefine.h"

// CAccTicketParam��Ψһ�ӿ�
#define theACC_PRODUCT CAccTicketParam::GetInstance()        
#define theACC_TICKET CAccTicketParam::GetInstance()
/**
@brief   Ʊ�����Բ�����
*/

class PARAMETERCOMMAND_DECLSPEC CAccTicketParam :public CAccParamMaster
{
public: 

	// �����ṩ�̲�����¼��
	typedef struct _tickets_Type {
		BYTE ticketTypebaseCode;				//��Ʊ�����ʹ���
		BYTE ticketTypeCode;				//��Ʊ�����ʹ���
		DWORD MaxBalance;                       //������/������
		BYTE chargeable;                        //�Ƿ�ɳ�ֵ
		DWORD maxChargeSize;                    //��ֵ����
		BYTE payMathodOfAdjuest;                //����ʱ�շѷ�ʽ
		BYTE refoundable;						//�Ƿ���˿�
		BYTE sendBackablecLimitCount;			//�˿�ʱʹ�ô�������
		BYTE hasLimitOfOneday;                  //�Ƿ������ճ�������
		BYTE maxMountOfOneDay;					//�ճ�����������
		BYTE prechargeable;						//�Ƿ�����Ԥ��ֵ
		WORD vilidateData;				    	//�̶���ֹ��Ч����
		WORD extendTime;                        //���ӳ�ʱ�� 
		LONG deposit;							//Ʊ������Ѻ��
		BYTE needActive;                        //ʹ��ǰ�Ƿ���Ҫ����
		BYTE checkBlackList;                    //�Ƿ��������
		BYTE checkBalance;                      //�Ƿ������/���
		BYTE checkValidate;                     //�Ƿ�����Ч��
		BYTE checkSequence;                     //�Ƿ����������
		BYTE checkOvertime;                     //�Ƿ��鳬ʱ
		BYTE checkOverJourney;                  //�Ƿ��鳬��
		BYTE checkLimitLine;                    //�Ƿ�������·
		BYTE checkOnlyThis;                     //�Ƿ�ֻ����վ����
		BYTE checkStickTime;                    //�Ƿ�����վ��ͣ��ʱ��
		WORD maxStickTime;                      //վ��ͣ��ʱ������
		BYTE chargeDevice;                      //�ɳ�ֵ���豸
		BYTE UseableDevice;                     //��ʹ�õ��豸
		BYTE checkInFlag;                       //��վ�������Ʊ��
		BYTE favourable;				        //�Ƿ�����β���Ż�
		BYTE recorverFlag;				        //���ձ�ʶ
		BYTE chargeType;						//�Ʒѷ�ʽ 01�����Ʒѣ�02�������ʱ�03������
		BYTE overdraftFlag;				     	//͸֧��ʶ
		BYTE checkMinBalanceFlag;		        //��ֵƱ��С�����
		BYTE checkMaxBalanceFlag;		        //��������
		BYTE checkEnterTime[3];		            //��վʱ����
		DWORD refoundFee;						//��Ʊ������
		DWORD exchangeFee;						//��Ʊ������
		DWORD amercementOfloseCard;				//������Ʊ��Ʊ�����
		DWORD amercementOfBrokenCard;			//������Ʊ��Ϊ�𻵷����
		DWORD overTimeFee;						//��ʱ�����

		CString ticketTypeName_cn;
		CString ticketTypeName_en;
		DWORD activeFlag;



		_tickets_Type(){
			ticketTypeName_cn ="";
			ticketTypeName_en ="";
			 ticketTypebaseCode=0;				//��Ʊ�����ʹ���
			 ticketTypeCode=0;				//��Ʊ�����ʹ���
			 MaxBalance=0;                       //������/������
			 chargeable=0;                        //�Ƿ�ɳ�ֵ
			 maxChargeSize=0;                    //��ֵ����
			 payMathodOfAdjuest=0;                //����ʱ�շѷ�ʽ
			 refoundable=0;						//�Ƿ���˿�
			 sendBackablecLimitCount=0;			//�˿�ʱʹ�ô�������
			 hasLimitOfOneday=0;                  //�Ƿ������ճ�������
			 maxMountOfOneDay=0;					//�ճ�����������
			 prechargeable=0;						//�Ƿ�����Ԥ��ֵ
			 vilidateData=0;				    	//�̶���ֹ��Ч����
			 extendTime=0;                        //���ӳ�ʱ�� 
			 deposit=0;							//Ʊ������Ѻ��
			 needActive=0;                        //ʹ��ǰ�Ƿ���Ҫ����
			 checkBlackList=0;                    //�Ƿ��������
			 checkBalance=0;                      //�Ƿ������/���
			 checkValidate=0;                     //�Ƿ�����Ч��
			 checkSequence=0;                     //�Ƿ����������
			 checkOvertime=0;                     //�Ƿ��鳬ʱ
			 checkOverJourney=0;                  //�Ƿ��鳬��
			 checkLimitLine=0;                    //�Ƿ�������·
			 checkOnlyThis=0;                     //�Ƿ�ֻ����վ����
			 checkStickTime=0;                    //�Ƿ�����վ��ͣ��ʱ��
			 maxStickTime=0;                      //վ��ͣ��ʱ������
			 chargeDevice=0;                      //�ɳ�ֵ���豸
			 UseableDevice=0;                     //��ʹ�õ��豸
			 checkInFlag=0;                       //��վ�������Ʊ��
			 favourable=0;				        //�Ƿ�����β���Ż�
			 recorverFlag=0;				        //���ձ�ʶ
			 chargeType=0;						//�Ʒѷ�ʽ
			 overdraftFlag=0;				     	//͸֧��ʶ
			 checkMinBalanceFlag=0;		        //��ֵƱ��С�����
			 checkMaxBalanceFlag=0;		        //��������
			 memset(checkEnterTime,0,3);		            //��վʱ����
			 refoundFee=0;						//��Ʊ������
			 exchangeFee=0;						//��Ʊ������
			 amercementOfloseCard=0;				//������Ʊ��Ʊ�����
			 amercementOfBrokenCard=0;			//������Ʊ��Ϊ�𻵷����
			 overTimeFee=0;						//��ʱ�����
			 activeFlag=1;
 	
		}
	}TICKET_TYPE;

	

	// ���нӿ�
	const vector<TICKET_TYPE>&  GetTicketTypeListParam();										            // ��ȡ���г�Ʊ���Ͳ���
	const int  GetTicketInfo(WORD ticketTypeCode,TICKET_TYPE& ticket_type_Param);									// ��ȡָ�����͵ĳ�Ʊ����
	const int  GetTicketValidate(WORD ticketTypeCode,_DATE_TIME& validate);										// ��ȡָ�����͵ĳ�Ʊ��Ч����
	const int  GetTicketDeposit(WORD ticketTypeCode,LONG& deposit);												// ��ȡָ�����͵ĳ�ƱѺ����Ϣ
	CString  GetProductName(WORD ticketTypeCode,LANGUAGETYPE_T language);		// ��ȡָ����Ʊ���͵��������Ƽ�Ӣ������
	BYTE  GetTicketCategry(WORD ticketTypeCode);								// ��ȡָ�����͵ĳ�Ʊ�ļƷ�����
	const int  GetTicketCategry(WORD ticketTypeCode,ProductCategory_t& ticket_categry);								// ��ȡָ�����͵ĳ�Ʊ�ļƷ�����
	const int  GetTicketTypeName(WORD ticketTypeCode,CString& ticketTypeName_cn,CString& ticketTypeName_en);		// ��ȡָ����Ʊ���͵��������Ƽ�Ӣ������

	const BYTE GetTicketMainType(BYTE ticketSubTypeCode);														// ���ݳ�Ʊ�����ͻ�ȡ��Ʊ��������
	bool IsNotCollectedTicket(BYTE ticketSubTypeCode);
	static CAccTicketParam&  GetInstance()
	{
		return theInstance;
	}

protected:

	vector<TICKET_TYPE>  m_vec_ticketsTypeList_param;			// ���г�Ʊ����
	vector<TICKET_TYPE>  m_vec_ticketTypeList_param;			// ָ����Ʊ����
	
private:

	SECTION_INFO m_section[1];	//�ֶ���Ϣ

	// ˽�г�Ա����
	

	// ˽�г�Ա����
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAccTicketParam(void);
	~CAccTicketParam();
	CAccTicketParam(const CAccTicketParam&);
	CAccTicketParam& operator=(const CAccTicketParam&);

	static  CAccTicketParam theInstance;
};


typedef CAccTicketParam::TICKET_TYPE		ACC_PRODUCT_t;