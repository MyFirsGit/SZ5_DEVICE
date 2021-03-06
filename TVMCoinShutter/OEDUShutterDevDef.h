#ifndef	_OEDUSHUTTERDEVDEF_H_
#define	_OEDUSHUTTERDEVDEF_H_

#define S_OEDUSHUTTERSTATUS_BASE					6500			// 5800 -- 5819	状态码
#define W_OEDUSHUTTERWARN_BASE						6520			// 5820 -- 5839	警告码	
#define E_OEDUSHUTTERERROR_BASE						6540			// 5840 -- 5899	// 严重错误码


#define S_EDUSHUTTER_NORMAL					S_OEDUSHUTTERSTATUS_BASE+0	//6500  模块正常，正常码	
#define S_EDUSHUTTER_CLOSE					S_OEDUSHUTTERSTATUS_BASE+1	//6501  闸门处于关闭状态，正常码			
#define S_EDUSHUTTER_OPEN					S_OEDUSHUTTERSTATUS_BASE+2	//6502  闸门处于打开状态，正常码	

#define W_EDUSHUTTER_INVALIDCOMMAND			W_OEDUSHUTTERWARN_BASE+0	//6520  收到无效的命令，警告码	


#define E_EDUSHUTTER_SENSORERR				E_OEDUSHUTTERERROR_BASE+0	//6540  信封闸门Sensor坏。
#define E_EDUSHUTTER_OPENFAIL				E_OEDUSHUTTERERROR_BASE+1	//6541  开门失败，门处在关的状态
#define E_EDUSHUTTER_OPENJAM				E_OEDUSHUTTERERROR_BASE+2	//6542  开门失败，门处在半开启卡死
#define E_EDUSHUTTER_CLOSEFAIL				E_OEDUSHUTTERERROR_BASE+3	//6543  关门失败，马达不转，门处在开的状态
#define E_EDUSHUTTER_CLOSEJAM				E_OEDUSHUTTERERROR_BASE+4	//6544  关门失败，门处在半开卡死
#define E_EDUSHUTTER_DOORJAM				E_OEDUSHUTTERERROR_BASE+5	//6545  关门失败，门被异物顶住（全开）
#define E_EDUSHUTTER_COMMERR				E_OEDUSHUTTERERROR_BASE+6	//6546  COM组件错误
#define E_EDUSHUTTER_INVALIDDATA			E_OEDUSHUTTERERROR_BASE+7	//6547  返回无效的数据
#define E_EDUSHUTTER_ABNORMAL				E_OEDUSHUTTERERROR_BASE+8	//6548  闸门处于异常状态
#define E_EDUSHUTTER_TIMEOUT				E_OEDUSHUTTERERROR_BASE+59	//6599  超时错误


#endif


