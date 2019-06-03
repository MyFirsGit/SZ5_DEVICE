#pragma once 
#include "BHChangeCommand.h"
// 纸币找零模块硬件接口
class CBHChangeHelper
{
	enum { 
		REJECT	= 3,  // REJECT 拒绝张数限制0,
		RETRY	= 3,  // RETRY 重试张数限制RETRY 
		Lenmax	= 175, // Lenmax 最大长度  175mm 
		Lenmin	= 110, // 张数限制Lenmin 最小长度  110mm
		Thick	= 10  // Thick  厚度 10 代表 0.10毫米
	}; 

public:
	CBHChangeHelper();
	~CBHChangeHelper();
public:
	long initialize();													// 初始化
	long GetStatus(DEVICE_STATUS_INFO& device_info);					// 取纸币找零模块状态
	long SendOutBill(int nCountBoxA,int nCountBoxB);					// 纸币送出（搬送给乘客）！同步调用！
	void SendOutBill(CService* pService,int nCountBoxA,int nCountBoxB);	// 纸币送出（搬送给乘客）！异步调用！

	long Reset();														// 重置纸币找零模块
	long Repare();														// 修复纸币找零模块

	long UnInitialize();												// 反初始化
	
protected:
	// 预留给硬件自检（暂时不开放,未实现）
	//long CountBill(int nCountBoxA,int nCountBoxB);					// 计数搬出纸币数量
	//long TransportationBill();										// 将上述计数的纸币搬送出纸币找零模块
	//long RetrievalBill();												// 将上述计数的纸币搬送到纸币找零模块后方（回收箱）
	//long Abort();														// 终止操作(暂时不用)
private:
	void OpenBHChangeComPort();											// 连接纸币找零模块
	void CloseBHChangeComPort();										// 关闭纸币找零模块
	bool IsHardWareException(long lErrorCode);							// 是否为纸币找零硬件异常
};
