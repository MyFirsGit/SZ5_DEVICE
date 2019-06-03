#pragma once
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcBomOperationParam��Ψһ�ӿ�
#define theAFC_BOM_OPERATION CAfcBomOperationParam::GetInstance()        

/**
 @brief   BOM��Ӫ������
*/

class PARAMETERCOMMAND_DECLSPEC CAfcBomOperationParam :public CAfcParamMaster
{
public: 

	// BOMƱ����д����
	 typedef struct _tpu_param {
		WORD maxWriteErrorCnt;	//��Ʊ���������������	2	BIN	��Ʊʱ��Ʊ������д�������������Ĭ��Ϊ3�ţ�0��ʾ������Ч���ﵽ�ô�����BOMֹͣ��Ʊ��
		WORD unwriteWaitTime;	//δд����ȫ�Ŀ��ȴ�ʱ��	2	BIN	д������������ʾ����ˢ�������ȴ�ʱ�䡣��λ���롣Ĭ��ֵΪ5��
		WORD unwriteRetryTimes;	//δд����ȫ�Ŀ����Դ���	2	BIN	��BOMд��������������д���Ĵ�����
        _tpu_param(){
            maxWriteErrorCnt = 0;
            unwriteWaitTime = 0;
			unwriteRetryTimes = 0;
        }
    }TPU_PARAM;	

	// BOMά�޵�¼��ز���
    typedef struct _maintain_param{ 
		WORD pwdInputTime;			//��������ʱ��	2	BIN	��λ�룬Ĭ��Ϊ60�롣�������������ʱ�ޣ�������ʱ��δ�������룬�Զ����뿪�����档
		WORD loginRetryTimes;		//��¼���Դ���	2	BIN	����Ա��¼�豸�������ƣ�Ĭ��Ϊ3�Ρ�
		WORD autoLogoffTime;		//�޲����Զ��ǳ�ʱ��	2	BIN	��λ�룬Ĭ��Ϊ120�룬0��ʾ��ֵ��Ч��
		WORD doorUnlockAlarmTime;	//ά����δ�رձ���ʱ��	2	BIN	��λ�룬Ĭ��Ϊ60�롣������
        _maintain_param(){
            pwdInputTime = 0;
            loginRetryTimes   = 0;
            autoLogoffTime = 0;
            doorUnlockAlarmTime = 0;
        }   
    } MAINTAIN_PARAM;

	// BOMƱ�����
    typedef struct _magazine_param{ 
		WORD willEmptyValue;				//Ʊ�佫������		2	BIN	Ʊ�佫��������ֵ
		WORD haveEmptyValue;				//Ʊ���ѿ�����		2	BIN	Ʊ���ѿ�������ֵ
		WORD willFullValue;					//Ʊ�佫������		2	BIN	Ʊ�佫��������ֵ
		WORD haveFullValue;					//Ʊ����������		2	BIN	Ʊ������������ֵ
		WORD willInvalidFullValue;			//��Ʊ�佫������	2	BIN	��Ʊ�佫��������ֵ
		WORD haveInvalidFullValue;			//��Ʊ����������	2	BIN	��Ʊ������������ֵ
        _magazine_param(){
			willEmptyValue = 0;				 
			haveEmptyValue = 0;					 
			willFullValue = 0;				 
			haveFullValue = 0;					 
			willInvalidFullValue = 0;		 
			haveInvalidFullValue = 0;		 
        }   
    } MAGAZINE_PARAM;

	// BOM����ɲ���С��������
	 typedef struct _txn_lookup_param{ 
		BYTE minCount;				//�ɲ�ѯ������������	1	BIN	�ɲ���С����������Ĭ��30����
		BYTE minTime;				//�ɲ�ѯ����ʱ������	1	BIN	��λ�����ӡ�Ĭ��10����
        _txn_lookup_param(){
			minCount = 0;			 
			minTime = 0;		 
        }   
    } TXN_LOOKUP_PARAM;

	// ���нӿ�
    void GetTpuParam(TPU_PARAM&);						// ���BOMƱ����д����
	void GetMaintainParam(MAINTAIN_PARAM&);				// ���BOMά�޵�¼��ز���
	void GetMagazineParam(MAGAZINE_PARAM&);				// ���BOMƱ�����
	void GetTxnLookupParam(TXN_LOOKUP_PARAM&);			// ���BOM����ɲ���С��������
	WORD GetAutoLogoffTime();							// ����Զ��ǳ��ȴ�ʱ�����
	
    static CAfcBomOperationParam&  GetInstance()
    {
        return theInstance;
    }

private:

	SECTION_INFO m_section[4];	//�ֶ���Ϣ

    // ˽�г�Ա����
    TPU_PARAM			m_tpu_param;			// BOMƱ����д����
	MAINTAIN_PARAM		m_maintain_param;		// BOMά�޵�¼��ز���
	MAGAZINE_PARAM		m_magazine_param;		// BOMƱ�����
	TXN_LOOKUP_PARAM	m_txn_lookup_param;		// BOM����ɲ���С��������

    // ˽�г�Ա����
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAfcBomOperationParam(void);
    ~CAfcBomOperationParam();
	CAfcBomOperationParam(const CAfcBomOperationParam&);
	CAfcBomOperationParam& operator=(const CAfcBomOperationParam&);

    static  CAfcBomOperationParam theInstance;
};
