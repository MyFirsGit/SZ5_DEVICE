#pragma once
#include "stdafx.h"
#include "Control.h"
#include "DataExp.h"

/**
@brief      ���ݿ�����
*/
namespace Data
{
	class TVMDB_API CDataControl: public CControl
	{
	public:
		CDataControl();
		virtual ~CDataControl();
		long BeginTransaction();	// �������ݿ�����
		long Commit();				// �ύ���ݿ�����
		long Rollback();			// �ع����ݿ�����
		virtual long Shutdown();	// ����
		virtual long Initialize();	// ���ڻ�
	};
};
