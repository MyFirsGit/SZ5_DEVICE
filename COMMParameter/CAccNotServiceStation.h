#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccNotServiceStation��Ψһ�ӿ�
#define theACC_NOTSERVSTATION CAccNotServiceStation::GetInstance()        

/**
@brief   ͣ�˳�վ�������
*/

class PARAMETERCOMMAND_DECLSPEC CAccNotServiceStation :public CAccParamMaster
{
public: 

	//ͣ�˳�վ���¼��
	typedef struct _no_Serv_Station_Code {
		WORD	noServStationCode;				// ͣ�˳�վ����
		_no_Serv_Station_Code(){				
			noServStationCode =0;					    		   	
		}
	}NO_SERV_STATION_CODE;



	// ���нӿ�
	const vector<NO_SERV_STATION_CODE>&  GetNotServiceStationListParam();										                // ��ȡ����ͣ�˳�վ����
	bool  IsNotServiceStationParam(WORD StationCode);																            // �ж��Ƿ���ͣ�˳�վ
	static CAccNotServiceStation&  GetInstance()
	{
		return theInstance;
	}

protected:

	vector<NO_SERV_STATION_CODE>  m_vec_NotServiceStationList_param;			// ���г�Ʊ����
private:

	SECTION_INFO m_section[1];	//�ֶ���Ϣ

	// ˽�г�Ա����


	// ˽�г�Ա����
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAccNotServiceStation(void);
	~CAccNotServiceStation();
	CAccNotServiceStation(const CAccNotServiceStation&);
	CAccNotServiceStation& operator=(const CAccNotServiceStation&);

	static  CAccNotServiceStation theInstance;
};

