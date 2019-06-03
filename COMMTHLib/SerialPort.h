/*//////////////////////////////////////////////////////////////////////////
/// COPYRIGHT NOTICE
/// 
/// @file    SerialPort.h  
/// @brief   串口通信类头文件
///
///  修订说明：
//////////////////////////////////////////////////////////////////////////
*/

#ifndef AFC_SERIALPORT_H_
#define AFC_SERIALPORT_H_

#include <Windows.h>  

     bool openPort(UINT portNo);
	/** 
	 * 打开串口
	 * 参数:
	 *  char portNo 串口编号
	 *  int baud   波特率
	 *  char parity 是否进行奇偶校验,'Y'表示需要奇偶校验,'N'表示不需要奇偶校验
	 *  int databits 数据位的个数,默认值为8个数据位
	 *  int stopsbits 停止位使用格式,默认值为1
	 * 返回: afc_rs232_t  打开成功后返回串口句柄,否则返回无效句柄AFC_INVALID_RS232
	 */
     bool sp_open(UINT portNo /*= 1*/, UINT baud /*= CBR_9600*/, char parity /*= 'N'*/,
	      UINT databits /*= 8*/, UINT stopsbits /*= 1*/, DWORD dwCommEvents /*= EV_RXCHAR*/);
	 /*
	 * 清除串口缓冲区数据
	 * 参数:
	 *  afc_rs232_t comFd 通过sp_open打开的串口句柄
	 */
	UINT sp_clear();
	/**
	 * 关闭串口
	 * 参数:
	 *  afc_rs232_t comFd 通过sp_open打开的串口句柄
	 */
	void sp_close( );

    /** 
	 * 从串口读取数据
	 * 参数:
	 *  afc_rs232_t comFd 通过sp_open打开的串口句柄
	 *  char * pData 从串口读取的数据缓冲区
	 *  unsigned int length 需要读取的串口数据长度
	 * 返回: >0实际读取数据的长度;其它异常,查看错误代码errno
	 */
    bool sp_read(char *p, int len);
    /** 
	 * 将缓冲区中的数据写入到串口
	 * 参数:
	 *  afc_rs232_t comFd 通过sp_open打开的串口句柄
	 *  char * pData 指向需要写入串口的数据缓冲区
	 *  unsigned int length 需要写入的数据长度
	 * 返回: >0实际写入数据的长度;其它异常,查看错误代码errno
	 */
     bool sp_write(char* pData, unsigned int length);
	/**
	 * 轮询串口,检查串口句柄是否准备接收/发送数据
	 * 参数:
	 *  afc_rs232_t comFd 通过sp_open打开的串口句柄
	 *  bool bRead 是否检查串口读准备(或检查是否是否数据到达)
	 *  bool bWrite 是否检查串口写准备
	 *  unsigned int nTimeOut 串口轮询超时时间(单位:毫秒)
	 * 返回:
	 *  < 0 轮询异常
	 *  = 0 串口无数据
	 *  > 0 串口已准备好读/写
	 */
	//int sp_select( afc_rs232_t comFd, bool bRead, bool bWrite, unsigned int nTimeOut );

#endif //AFC_SERIALPORT_H_
