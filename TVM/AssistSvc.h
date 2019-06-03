#pragma once
#include "tvmforeservice.h"
class CAssistSvc :
	public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CAssistSvc(void);
	~CAssistSvc(void);
	typedef enum _dialog_group {
		DIALOG_GROUP_BASE	= 1,
		DIALOG_GROUP_ONE    = 2,
	} DIALOG_GROUP;

	typedef struct _tagDataModel
	{
		
	}DataModel;

	void NotifyDataModelChanged();

private:
	DataModel m_DataModel;
};

