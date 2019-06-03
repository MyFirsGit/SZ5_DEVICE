#pragma once
#ifndef _SCCOMMAND_H_
#define _SCCOMMAND_H_
#include "SCAPI.h"
#include "SCExp.h"
#include "Command.h"
#include <list>
using namespace std;
static const int MAX_TCP_TRANS_SIZE = 8148;
//static const int MAX_TCP_PDU_MSG_SIZE = MAX_TCP_TRANS_SIZE - CMD_HEADER_DATA_LEN - 2/*CRC*/ - 2/* STATR TAG + END TAG */;

class CSCResponseCommand;

class SC_API CSCCommand : public CCommand
{
public:

	CSCCommand();
	~CSCCommand();
	
	long IsValidCommand();

	void SetHeader(CMD_HEADER *pHeader);      // 设置电文头部
	CMD_HEADER& GetHeader();    // 获取电文头部
	void SetContent(LPVARIABLE_DATA pBody);
	void SetContent(LPBYTE body,UINT length);
	template<UINT length>
	void SetContent(BYTE (&body)[length] )
	{
		if(m_pContent!=NULL)
		{
			delete m_pContent;
			m_pContent = NULL;
		}
		m_pContent = new VARIABLE_DATA;
		m_pContent->lpData = new BYTE[length];
		memcpy(m_pContent->lpData,body,length);
		m_pContent->nLen =length;
	};

	LPVARIABLE_DATA GetContent();

	virtual void  SerializeBody(VARIABLE_DATA& buffer);
	
	virtual void DeserializeBody(LPBYTE bodyData,UINT length);

	BOOL IsRemote();


	BOOL IsReceived();

	BOOL IsNeedReply();
	void SetIsNeedReply(BOOL needReply);

	CSCResponseCommand* GetReplyCommand();
	void SetReplyCommand(CSCResponseCommand* command);

protected:
	long ExecuteCommand();
	LPVARIABLE_DATA m_pContent;

	CMD_HEADER m_Header;
	WORD m_ReplyMsgCode;
	BOOL m_IsRemote;
private:

	void SetIsReceived(BOOL acked);
	void SetIsRemote(BOOL isRemote);
	
	BOOL m_IsReceived;
	BOOL m_NeedReply;

	CSCResponseCommand* m_ReplyCommand;
	CSCCommand* m_InterruptedCommand;

	friend class CSCClient; 

};

#endif