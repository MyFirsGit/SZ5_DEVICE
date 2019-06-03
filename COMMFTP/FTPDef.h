#pragma once


#define E_NO_ERROR							0		/* no error occur	*/
#define E_INVALID_PARAMS					0x4001		/* parameter INVALID	*/
#define E_WIN32_ERROR						0x4002		/* win32 error check	*/
#define E_INTERNET_ERROR					0x4003		/* The internet could not be started*/
#define E_INVALID_URL						0x4004		/* Invalid FTP URL */
#define E_FILE_NOT_FOUND					0x4005		/* File not found on server */
#define E_GET_FILE							0x4006		/* Can not get File on server */
#define E_DOWNLOAD_FILE						0x4007		/* Downloading error */
#define E_OPEN_FILE							0x4008		/* Can not open file on server */
#define E_UNABLE_LOCAL_FILE					0x4009		/* Unable to create local file */
#define E_UNABLE_REMOTE_FILE				0x4010		/* Unable to open remote file */
#define E_OPEN_FOLDER						0x4011		/* An error occurred opening current folder on server */
#define E_OPEN_SERVER						0x4012		/* Can not connect to FTP server */
#define E_DIR_NOT_FOUND						0x4013		/* The local directory does not exist */
#define E_LOCAL_PATH_NOT_FOUND				0x4014		/* Local path not found (Upload process) */
#define E_RETMOTE_DIR_NOT_FOUND				0x4015		/* The remote directory can not found on server */
#define E_PUT_FILE							0x4016		/* Can not put File on server */

#define E_INTERNET_OUT_OF_HANDLES			0x4017		/* No more handles could be generated at this time. */	
#define E_INTERNET_TIMEOUT					0x4018		/* The request has timed out. */ 
#define E_INTERNET_INVALID_URL				0x4019		/* The URL is invalid */
#define E_INTERNET_UNRECOGNIZED_SCHEME		0x4020		/* The URL scheme could not be recognized or is not supported */
#define E_INTERNET_NAME_NOT_RESOLVED		0x4021		/* The server name could not be resolved */
#define E_INTERNET_PROTOCOL_NOT_FOUND		0x4022		/* The requested protocol could not be located */
#define E_INTERNET_INVALID_OPTION			0x4023		/* A request to InternetQueryOption or InternetSetOption specified an invalid option value */
#define E_INTERNET_BAD_OPTION_LENGTH		0x4024		/* InternetSetOption is incorrect for the type of option specified */
#define E_INTERNET_OPTION_NOT_SETTABLE		0x4025		/* The request option cannot be set, only queried */
#define E_INTERNET_SHUTDOWN					0x4026		/* The Win32 Internet function support is being shut down or unloaded */
#define E_INTERNET_INCORRECT_USER_NAME		0x4027		/* The request to connect and log on to an FTP server could not be completed because the supplied user name is incorrect */
#define E_INTERNET_INCORRECT_PASSWORD		0x4028		/* The request to connect and log on to an FTP server could not be completed because the supplied password is incorrect. */
#define E_INTERNET_LOGIN_FAILURE			0x4029		/* The request to connect to and log on to an FTP server failed */
#define E_INTERNET_INVALID_OPERATION		0x4030		/* The requested operation is invalid */
#define E_INTERNET_OPERATION_CANCELLED		0x4031		/* The requested operation was canceled */
#define E_INTERNET_INCORRECT_HANDLE_TYPE	0x4032		/* The type of handle supplied is incorrect for this operation */
#define E_INTERNET_INCORRECT_HANDLE_STATE	0x4033		/* The requested operation cannot be carried out because the handle supplied is not in the correct state. */
#define E_INTERNET_NOT_PROXY_REQUEST		0x4034		/* The request cannot be made via a proxy. */
#define E_INTERNET_REGISTRY_VALUE_NOT_FOUND	0x4035		/* A required registry value could not be located. */
#define E_INTERNET_BAD_REGISTRY_PARAMETER	0x4036		/* A required registry value was located but is an incorrect type or has an invalid value. */
#define E_INTERNET_NO_DIRECT_ACCESS			0x4037		/* Direct network access cannot be made at this time. */
#define E_INTERNET_NO_CONTEXT				0x4038		/* An asynchronous request could not be made because a zero context value was supplied. */
#define E_INTERNET_NO_CALLBACK				0x4039		/* An asynchronous request could not be made because a callback function has not been set. */
#define E_INTERNET_REQUEST_PENDING			0x4040		/* The required operation could not be completed because one or more requests are pending. */
#define E_INTERNET_INCORRECT_FORMAT			0x4041		/* The format of the request is invalid. */	
#define E_INTERNET_ITEM_NOT_FOUND			0x4042		/* The requested item could not be located. */
#define E_INTERNET_CANNOT_CONNECT			0x4043		/* The attempt to connect to the server failed. */
#define E_INTERNET_CONNECTION_ABORTED		0x4044		/* The connection with the server has been terminated. */	
#define E_INTERNET_CONNECTION_RESET			0x4045		/* The connection with the server has been reset. */
#define E_INTERNET_FORCE_RETRY				0x4046		/* Calls for the Win32 Internet function to redo the request. */
#define	E_INTERNET_INVALID_PROXY_REQUEST	0x4047		/* The request to the proxy was invalid. */
#define E_INTERNET_HANDLE_EXISTS			0x4048		/* The request failed because the handle already exists. */
#define E_FTP_TRANSFER_IN_PROGRESS			0x4049		/* The requested operation cannot be made on the FTP session handle because an operation is already in progress. */
#define E_FTP_DROPPED						0x4050		/* The FTP operation was not completed because the session was aborted. */
#define E_LOAD_INICONFIG					0x4051		/* Can not load ini config file */
#define E_CONNECTION_LOST					0x4052		/* The connection with the server has been lost */

#define C_FTP_CONNECT						0x4001
#define C_FTP_CLOSE							0x4002
#define C_FTP_DOWNLOAD						0x4003
#define	C_FTP_UPLOAD						0x4004

typedef enum _err_detail_ftp {
	FTP_SET_PARAM_ERR   = 1,	      // 设定值：参数异常
	FTP_FUNC_PARAM_ERR  = 2,	      // 函数参数异常
	FTP_SEQUENCE_ERR	= 3,	      // 函数调用顺序异常
	FTP_FUNC_ERR_RETURN	= 4,	      // 函数返回异常
	FTP_LOGIC_ERR	    = 5,	      // 逻辑异常
	FTP_OTHER_ERR	    = 6,	      // 异常发生
	FTP_CONNECT_ERR     = 101,        //FTP连接异常
	FTP_USERNO_ERR      = 102,        //非法的FTP用户名
	FTP_PASSWORD_ERR    = 103,        //非法的FTP密码
	FTP_URL_ERR         = 104,        //无法解释的FTP URL
	FTP_FILE_NOTEXIST_ERR = 105,      //上传文件不存在
	FTP_NOTCONNECT_ERR  = 106,         //FTP未连接
} ERR_DETAIL_FTP;


