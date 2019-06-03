#pragma once
#include "BOMForeService.h"

/**
  @brief 分析业务Service                                                                  
*/

class CAnalyzeSvc : public CBOMForeService
{
    DECLARE_MESSAGE_MAP()

public:
    // 画面分组
    typedef enum _dialog_group {
        DIALOG_GROUP_WAIT	= 1,
        DIALOG_GROUP_BASE,
		DIALOG_GROUP_ERROR,
    } DIALOG_GROUP;
	typedef struct _tagDataModel
	{
		RW_READ_CARD_RESPONSE rwReadResponse;
		CULCard* ulCard;
		CCPUCard* cpuCard;
		_tagDataModel()
		{
			ulCard = NULL;
			cpuCard = NULL;
		}
	}DataModel;

    CAnalyzeSvc();
	virtual ~CAnalyzeSvc();
	DataModel& GetDataModel();
protected:
	void OnReadCard(UINT,CRWReadCard*);
	
private:
    long BlockCard(UINT resultCode,RW_BUSSINESS_RESPONSE response);
	void OnError(CString strMessage);
	DataModel m_DataModel;
};


