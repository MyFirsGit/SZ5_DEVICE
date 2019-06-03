#include "stdafx.h"
#include "SysException.h"
#include <map>
#include "delegate.h"
#include "CoreExp.h"



//////////////////////////////////////////////////////////////////////////
/*
@brief �쳣�������      
*/
//////////////////////////////////////////////////////////////////////////
class CORE_API CExceptionProcessor
{
public:
	CExceptionProcessor();                                                    // ���캯��
	virtual ~CExceptionProcessor(); 
	delegate<void(CSysException &,bool)> ExceptionItemChanged;			//�쳣���з����仯�¼�
	
	void RemoveException(long moduleID,long detailID = NO_DETAIL_ID);  // ����ģ�������ģ������ϸ����ɾ���쳣����
	void RemoveExceptionExcept(long moduleID,long detailID);			// ɾ��ָ���쳣�����ģ���������쳣��¼
	bool HasException();                                                // �Ƿ����쳣
	bool HasException(long moduleID);                                   // �Ƿ���ָ�������쳣
	bool HasException(vector<long>& moduleIDs);			// �Ƿ����ض����͵��쳣
	bool OnlyHasException(long moduleID);							//�Ƿ�ֻ���ض����쳣
	bool OnlyHasException(vector<long>& moduleIDs);		//�Ƿ�ֻ���ض����쳣
	bool HasException(long moduleID, long detailID);                    // �Ƿ���ָ�����ָ����ϸ������쳣
	CSysException& GetLastException();                                  // ��ȡ���һ���쳣����
	virtual void ProcessException(CSysException& e);							// �����쳣
	virtual bool RepareException();                                             // ���Ͻ��
	const vector<CSysException>& GetAllException();                     // �õ����е��쳣
protected:
	
	void AddException(CSysException& e);     // ��¼�쳣����
	vector<CSysException> m_exceptionArr;	                            // ��¼�������쳣����
private:	
	const static long NO_DETAIL_ID = 0;                                 // ����ϸ����
	
};