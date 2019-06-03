#pragma once
#include "stdafx.h"
#include "Control.h"
#include "DataExp.h"

/**
@brief      数据控制类
*/
namespace Data
{
	class TVMDB_API CDataControl: public CControl
	{
	public:
		CDataControl();
		virtual ~CDataControl();
		long BeginTransaction();	// 开启数据库事务
		long Commit();				// 提交数据库事务
		long Rollback();			// 回滚数据库事物
		virtual long Shutdown();	// 结束
		virtual long Initialize();	// 初期化
	};
};
