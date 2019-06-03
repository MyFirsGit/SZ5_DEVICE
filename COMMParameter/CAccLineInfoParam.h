#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccLineInfoParam��Ψһ�ӿ�
#define theACC_LOCATION CAccLineInfoParam::GetInstance()        
#define theACC_LINE theACC_LOCATION
/**
@brief   ACC��·��Ϣ������
*/

class PARAMETERCOMMAND_DECLSPEC CAccLineInfoParam :public CAccParamMaster
{
public:
	
	// ��·��Ϣ��¼��
	typedef struct _line_Info {
		WORD   lineNum;							//��·����		
		CString line_Chinese_Name;				//��·��������
		CString line_Eng_Name;				    //��·Ӣ������
		DWORD   Active_flag;                    //���ñ�־		

		_line_Info()
		{
			lineNum = 0;
			line_Chinese_Name = "";
			line_Eng_Name = "";
			Active_flag = 1;
		};
	}LINE_INFO;	

	// ��վ��Ϣ��¼��
	typedef struct _station_Info {
		WORD stationNum;						//��վ����		
		CString station_Chinese_Name;	        //��վ��������
		CString station_Eng_Name;				//��վӢ������
		WORD fareStationNum;					//�Ʒ�վ�����		
		BYTE Active_flag;                       //���ñ�־
		BYTE isExchangeStation;					//�Ƿ񻻳�վ

		_station_Info()
		{

			stationNum = 0;
			station_Chinese_Name = "";
			station_Eng_Name = "";
			fareStationNum = 0;
			Active_flag = 1;
			isExchangeStation = 0;
		};
	}STATION_INFO;	


	
	// ����վ��Ϣ��¼��
	typedef struct _exchange_Station_Info {
		WORD  stationNum;						//��վ����		
		WORD  exchangeStationNum;				//���˳�վ����
		_exchange_Station_Info()
		{
			stationNum = 0;
			exchangeStationNum = 0;
		}
	}EXCHANGE_STATION_INFO;	

	// ���нӿ�
	const vector<LINE_INFO>&					GetLinInfo();					// ��·��Ϣ
	const vector<STATION_INFO>&					GetStationInfo();				// ���ȫ·����վ��Ϣ
	const vector<STATION_INFO>&					GetStationInfo(DWORD inLineNum);// ���ָ����·��վ��Ϣ
	const vector<EXCHANGE_STATION_INFO>&	    GetExchangeInfo();				// ����վ��Ϣ
	bool  GetStationInfobyCode(WORD inStationNum,STATION_INFO& stationinfo);	// ���ָ����վ��Ϣ
	CString GetLocationName(WORD inStationNum, LANGUAGETYPE_T);						// ���ָ��λ�ñ���ĳ�վ����

	static CAccLineInfoParam&  GetInstance()
	{
		return theInstance;
	}

protected:

	map<int,LINE_INFO>             m_map_lineInfoList_param;    		// ��·��Ϣ����
	vector<LINE_INFO>              m_vec_lineInfoList_param;    		// ��·��Ϣ����
	vector<STATION_INFO>	    m_vec_stationInfoList_param;			// ��վ��Ϣ����	
	vector<STATION_INFO>		m_vec_stationInfoListOfLine_param;		// ָ����·��վ��Ϣ
	vector<EXCHANGE_STATION_INFO> m_vec_exchangeInfoList_param;			// ����վ��Ϣ����

	void					SetLinInfo();					// ��·��Ϣ

private:

	SECTION_INFO m_section[3];                  	//�ֶ���Ϣ

	// ˽�г�Ա����
	

	// ˽�г�Ա����
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAccLineInfoParam(void);
	~CAccLineInfoParam();
	CAccLineInfoParam(const CAccLineInfoParam&);
	CAccLineInfoParam& operator=(const CAccLineInfoParam&);

	static  CAccLineInfoParam theInstance;
};


