#ifndef _CASDEF_H_
#define _CASDEF_H_

//========错误码定义========================================
#define S_CAS_BASE			    	17000			  // 错误码基值

#define W_CAS_BASE                  S_CAS_BASE + 5    // 17005 警告码基值
#define W_CAS_MHOPPER1_COUNT_ERROR  W_CAS_BASE + 1    // 17006 Hopper2清币数量不符						(清币失败)	//武汉11号线专用
#define W_CAS_MHOPPER2_COUNT_ERROR  W_CAS_BASE + 2    // 17007 Hopper1清币数量不符						(清币失败)  //武汉11号线专用
#define W_CAS_MHOPPER_COUNT_ERROR   W_CAS_BASE + 3    // 17008 两Hopper清币数量不符						(清币失败)

#define E_CAS_BASE				    S_CAS_BASE + 10   // 17010 严重错阈值

#define E_CAS_COMMAND_ERROR         E_CAS_BASE + 1    // 17011 无效命令
#define E_CAS_NO_CHANGE			    E_CAS_BASE + 2    // 17012 金额不足，找零失败						(建议屏蔽找零功能)
#define E_CAS_ESCROW1_MIDDLE		E_CAS_BASE + 3    // 17013 暂存器1找不到中间位置					(建议停机维护，影响找零和收币功能)
#define E_CAS_ESCROW1_LEFT			E_CAS_BASE + 4    // 17014 暂存器1找不到左边位置   					(建议停机维护，影响找零和收币功能)
#define E_CAS_ESCROW1_RIGHT	        E_CAS_BASE + 5    // 17015 暂存器1找不到右边位置   					(建议停机维护，影响找零和收币功能)
#define E_CAS_ESCROW2_MIDDLE		E_CAS_BASE + 6    // 17016 暂存器2找不到中间位置   					(建议停机维护，影响找零和收币功能)
#define E_CAS_ESCROW2_LEFT			E_CAS_BASE + 7    // 17017 暂存器2找不到左边位置   					(建议停机维护，影响找零和收币功能)
#define E_CAS_ESCROW2_RIGHT			E_CAS_BASE + 8    // 17018 暂存器2找不到右边位置   					(建议停机维护，影响找零和收币功能)
#define E_CAS_ROUTER1_LEFT			E_CAS_BASE + 9    // 17019 换向器1找不到左边位置   					(建议停机维护，影响找零和收币功能)
#define E_CAS_ROUTER1_RIGHT			E_CAS_BASE + 10   // 17020 换向器1找不到右边位置  					(建议停机维护，影响找零和收币功能)
#define E_CAS_ROUTER2_LEFT			E_CAS_BASE + 11   // 17021 换向器2找不到左边位置   					(建议停机维护，影响找零和收币功能)
#define E_CAS_ROUTER2_RIGHT			E_CAS_BASE + 12   // 17022 换向器2找不到右边位置   					(建议停机维护，影响找零和收币功能)

#define E_CAS_ROUTER1_MIDDLE		E_CAS_BASE + 13   // 17023 换向器1找不到中间位置   					(建议停机维护，影响找零和收币功能)
#define E_CAS_ROUTER2_MIDDLE		E_CAS_BASE + 14   // 17024 换向器2找不到中间位置   					(建议停机维护，影响找零和收币功能)

#define E_CAS_MHOPPER1_DISP			E_CAS_BASE + 15   // 17025 找零器2出币失败       					(建议停机维护，建议屏蔽找零功能)	//武汉11号线专用
#define E_CAS_MHOPPER1_JAM			E_CAS_BASE + 16   // 17026 找零器2卡币           					(建议停机维护，建议屏蔽找零功能)	//武汉11号线专用
#define E_CAS_MHOPPER2_DISP			E_CAS_BASE + 17   // 17027 找零器1出币失败       					(建议停机维护，建议屏蔽找零功能)	//武汉11号线专用
#define E_CAS_MHOPPER2_JAM			E_CAS_BASE + 18   // 17028 找零器1卡币           					(建议停机维护，建议屏蔽找零功能)	//武汉11号线专用

#define E_CAS_COINACCEPTOR_FAULT	E_CAS_BASE + 23   // 17033 硬币接收器故障          					(建议停机维护，建议屏蔽收币功能)
#define E_CAS_IBUTTON1				E_CAS_BASE + 24   // 17034 加币箱2iButton故障/RFID故障  			(建议停机维护)	//武汉11号线专用
#define E_CAS_IBUTTON2				E_CAS_BASE + 25   // 17035 加币箱1iButton故障/RFID故障  			(建议停机维护)	//武汉11号线专用
#define E_CAS_WRITE_FLASH			E_CAS_BASE + 26   // 17036 编程FLASH失败           					(建议停机维护)
#define E_CAS_FILE_NOTFINISH		E_CAS_BASE + 27   // 17037 文件未下载完            					(建议停机维护)
#define E_CAS_WRITE_LOG				E_CAS_BASE + 28   // 17038 写Log失败               					(预留)
#define E_CAS_READ_LOG				E_CAS_BASE + 29   // 17039 读Log失败               					(预留)
#define E_CAS_CLRCOIN_FAIL          E_CAS_BASE + 30   // 17040 清币失败                					(建议停机维护，不影响找零和接受硬币功能)
#define E_CAS_CAM_JAM				E_CAS_BASE + 31   // 17041 凸轮堵转                					(建议停机维护)
#define E_CAS_CAM_MOTOR				E_CAS_BASE + 32   // 17042 凸轮马达故障            					(建议停机维护)
#define E_CAS_COININ_JAM			E_CAS_BASE + 33   // 17043 投币口卡币              					(建议停机维护，建议屏蔽收币功能)
#define E_CAS_EJECT_SIGNAL			E_CAS_BASE + 34   // 17044 硬币接收器退币信号异常  					(建议停机维护，建议屏蔽收币功能)
#define E_CAS_ESCROW1_SENSOR		E_CAS_BASE + 35   // 17045 暂存器1传感器故障       					(建议停机维护)
#define E_CAS_ESCROW2_SENSOR		E_CAS_BASE + 36   // 17046 暂存器2传感器故障       					(建议停机维护)
#define E_CAS_ROUTER1_SENSOR		E_CAS_BASE + 37   // 17047 换向器1传感器故障       					(建议停机维护)
#define E_CAS_ROUTER2_SENSOR		E_CAS_BASE + 38   // 17048 换向器2传感器故障       					(建议停机维护)
#define E_CAS_EEPROM		 		E_CAS_BASE + 39   // 17049 EEPROM故障              					(建议停机维护)
#define E_CAS_COINACCEPTOR_TIMEOUT	E_CAS_BASE + 40   // 17050 硬币接受器超时          					(建议停机维护，建议屏蔽收币功能)
#define E_CAS_COINACCEPTOR_DATA		E_CAS_BASE + 41   // 17051 硬币接受器返回数据错    					(建议停机维护，建议屏蔽收币功能)
#define E_CAS_DEV_BUSY				E_CAS_BASE + 42   // 17052 设备忙                  
#define E_CAS_IBUTTON_ERROR         E_CAS_BASE + 43   // 17053 钱箱iButton故障/RFID故障					(建议停机维护)
#define E_CAS_COINOUT_JAM           E_CAS_BASE + 44   // 17054 投币出口卡币            					(建议停机维护，建议屏蔽收币功能)
#define E_CAS_MODULE_ERROR          E_CAS_BASE + 45   // 17055 模块位置异常          					(建议停机维护)
#define E_CAS_HOPPER1_POS           E_CAS_BASE + 46   // 17056 HOPPER1不到位         					(建议停机维护)
#define E_CAS_HOPPER2_POS           E_CAS_BASE + 47   // 17057 HOPPER2不到位           					(建议停机维护)
#define E_CAS_COINBOX_POS           E_CAS_BASE + 48   // 17058 钱箱不到位              					(建议停机维护)
#define E_CAS_MODULE_POS            E_CAS_BASE + 49   // 17059 模块不到位              					(建议停机维护)
#define E_CAS_IMAGE_ERROR			E_CAS_BASE + 50   // 17060 图像识别模块故障        					(建议停机维护)

#define E_CAS_RFID_REQUEST			E_CAS_BASE + 51   // 17061 感应区无有效卡或/卡模块未连接			(根据业务需求决定故障等级)
#define E_CAS_RFID_AUTHEN			E_CAS_BASE + 52   // 17062 感应区卡认证失败							(根据业务需求决定故障等级)
#define E_CAS_RFID_PARAMETER		E_CAS_BASE + 53   // 17063 读写RFID时参数错							(根据业务需求决定故障等级)

#define E_CAS_READ_FILE 			E_CAS_BASE + 56   // 17066 读文件失败								(建议停机维护)
#define E_CAS_FILE_LENGTH			E_CAS_BASE + 57   // 17067 文件太大									(建议停机维护)
#define E_CAS_OPEN_FILE				E_CAS_BASE + 58   // 17068 文件打开失败								(建议停机维护)
#define E_CAS_UPDATEDATA_EMPTY		E_CAS_BASE + 59	  // 17069 无法执行更新，无更新数据

#define E_CAS_COINBOXFULL			E_CAS_BASE + 63	  // 17073 硬币回收钱箱满
#define E_CAS_VOLTAGE_LACK			E_CAS_BASE + 64	  // 17074 电压不足 
#define E_CAS_MHOPPER1_INTERVENE	E_CAS_BASE + 65   // 17075 主找零1出币过程中传感器被干扰(当前交易出币计数不准)
#define E_CAS_MHOPPER1_LACK			E_CAS_BASE + 66   // 17076 主找零1出币过程中可能少出币 (当前交易出币计数不准)
#define E_CAS_MHOPPER2_INTERVENE	E_CAS_BASE + 67   // 17077 主找零2出币过程中传感器被干扰 (当前交易出币计数不准)
#define E_CAS_MHOPPER2_LACK			E_CAS_BASE + 68   // 17078 主找零2出币过程中可能少出币 (当前交易出币计数不准)

//added by ZacharyLiu 2015-12-03
#define E_CAS_UNSUPPORT				E_CAS_BASE + 69	  // 17079	设备不支持的功能						(某些接口仅针对特定型号，对于不支持的型号会返回此错误)
#define E_CAS_RTRBOX2_RFID			E_CAS_BASE + 71	  // 17081  回收箱2 iButton/RFID 故障
#define E_CAS_GATE_OPEN				E_CAS_BASE + 72	  // 17082  闸门打开失败
#define E_CAS_GATE_CLOSE			E_CAS_BASE + 73	  // 17083  闸门关闭失败
//end added

#define E_CAS_UNKNOWN				E_CAS_BASE + 80   // 17090 未知错
#define E_CAS_PARAM_ERROR			E_CAS_BASE + 81   // 17091 参数不正确 
#define E_CAS_DEV_CFG				E_CAS_BASE + 82   // 17092 读写设备配置错                
#define E_CAS_COMM_CFG	    		E_CAS_BASE + 83   // 17093 串口配置错 
#define E_CAS_RECEIVE_NAK			E_CAS_BASE + 84   // 17094 收到NAK,发送数据有错    
#define E_CAS_REVDATA_ERROR			E_CAS_BASE + 85   // 17095 接收到的数据有误        
#define E_CAS_SENDENQ_ERROR			E_CAS_BASE + 86   // 17096 发送ENQ出错             
#define E_CAS_SENDCMD_ERROR		    E_CAS_BASE + 87   // 17097 发送命令错              
#define E_CAS_WAITACK_TIMEOUT		E_CAS_BASE + 88   // 17098 等待ACK超时             
#define E_CAS_WAITRSP_TIMEOUT		E_CAS_BASE + 89   // 17099 等待响应超时            

#endif	// #ifndef _CASDEF_H_