/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Nov 30 15:01:34 2011
 */
/* Compiler settings for D:\BuildWork\Dunite\GrgCommManager\Src\new_commmanager\CommManager.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __CommManager_h__
#define __CommManager_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IComm_FWD_DEFINED__
#define __IComm_FWD_DEFINED__
typedef interface IComm IComm;
#endif 	/* __IComm_FWD_DEFINED__ */


#ifndef __ICommEvents_FWD_DEFINED__
#define __ICommEvents_FWD_DEFINED__
typedef interface ICommEvents ICommEvents;
#endif 	/* __ICommEvents_FWD_DEFINED__ */


#ifndef __Comm_FWD_DEFINED__
#define __Comm_FWD_DEFINED__

#ifdef __cplusplus
typedef class Comm Comm;
#else
typedef struct Comm Comm;
#endif /* __cplusplus */

#endif 	/* __Comm_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_CommManager_0000 */
/* [local] */ 


#pragma pack(push, 1)


extern RPC_IF_HANDLE __MIDL_itf_CommManager_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_CommManager_0000_v0_0_s_ifspec;

#ifndef __IComm_INTERFACE_DEFINED__
#define __IComm_INTERFACE_DEFINED__

/* interface IComm */
/* [unique][helpstring][uuid][object] */ 

typedef /* [public][public][public][public][public][public][public][public][public] */ 
enum __MIDL_IComm_0001
    {	COMM_COM	= 1,
	COMM_COMBOARD	= COMM_COM + 1,
	COMM_USB	= COMM_COMBOARD + 1,
	COMM_TCPIP	= COMM_USB + 1
    }	tCommType;

typedef /* [public] */ 
enum __MIDL_IComm_0002
    {	EXTERN_ESCAPECOMM	= 1
    }	tExtendComm;

struct  tComInfo
    {
    unsigned int iComID;
    unsigned int BaudRate;
    unsigned int fParity;
    unsigned int fOutxCtsFlow;
    unsigned int fOutxDsrFlow;
    unsigned int fDtrControl;
    unsigned int fDsrSensitivity;
    unsigned int fTXContinueOnXoff;
    unsigned int fOutX;
    unsigned int fInX;
    unsigned int fRtsControl;
    unsigned short XonLim;
    unsigned short XoffLim;
    byte ByteSize;
    byte Parity;
    byte StopBits;
    char XonChar;
    char XoffChar;
    unsigned int ReadIntervalTimeout;
    unsigned int ReadTotalTimeoutMultiplier;
    unsigned int ReadTotalTimeoutConstant;
    unsigned int WriteTotalTimeoutMultiplier;
    unsigned int WriteTotalTimeoutConstant;
    };
struct  tComBoardInfo
    {
    int iBand;
    int iDataLen;
    int iEventFlag;
    int iStopFlag;
    int iComPort;
    struct tComInfo sComInfo;
    };
struct  tUSBInfo
    {
    int iReserver1;
    int iReserver2;
    int iReserver3;
    int iReserver4;
    };
struct  tTCPIPInfo
    {
    unsigned char ipAddress[ 50 ];
    int iSendPort;
    int iReceivePort;
    int iLinkMode;
    };
typedef /* [switch_type] */ union _UCommInfo
    {
    /* [case()] */ struct tComInfo sComInfo;
    /* [case()] */ struct tComBoardInfo sComBoardInfo;
    /* [case()] */ struct tUSBInfo sUSBInfo;
    /* [case()] */ struct tTCPIPInfo sTCPIPInfo;
    /* [default] */  /* Empty union arm */ 
    }	tCommInfo;

struct  tCommCfg
    {
    tCommType eCommType;
    /* [switch_is] */ tCommInfo unCommInfo;
    /* [string] */ char __RPC_FAR *pcExtern;
    };
struct  tUSBInfo2
    {
    GUID sGuid;
    unsigned char acSendPipes[ 50 ];
    unsigned char acRecvPipes[ 50 ];
    unsigned char acExtendStr[ 256 ];
    int iReserve;
    };
typedef /* [switch_type] */ union _UCommInfo2
    {
    /* [case()] */ struct tComInfo sComInfo;
    /* [case()] */ struct tComBoardInfo sComBoardInfo;
    /* [case()] */ struct tUSBInfo2 sUSBInfo;
    /* [case()] */ struct tTCPIPInfo sTCPIPInfo;
    /* [default] */  /* Empty union arm */ 
    }	tCommInfo2;

struct  tCommCfg2
    {
    tCommType eCommType;
    /* [switch_is] */ tCommInfo2 unCommInfo;
    /* [string] */ char __RPC_FAR *pcExtern;
    };

EXTERN_C const IID IID_IComm;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F5857230-7094-11D7-9DD6-0010B54D6738")
    IComm : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetDefaultCommConfig( 
            /* [out][in] */ struct tCommCfg __RPC_FAR *p_psCommCfg) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentCfg( 
            /* [out] */ struct tCommCfg __RPC_FAR *p_psCommCfg,
            /* [out] */ boolean __RPC_FAR *p_pbEnableEvents) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InitComm( 
            struct tCommCfg p_sCommCfg,
            /* [in] */ boolean p_bEnableEvents) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CloseComm( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnableEvents( 
            /* [in] */ boolean p_bEnableEvents) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEventsEnable( 
            /* [out] */ boolean __RPC_FAR *p_pbEnableEvents) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SendData( 
            /* [in] */ int p_iLen,
            /* [size_is][in] */ byte __RPC_FAR *p_pbData,
            /* [out] */ long __RPC_FAR *p_plIden) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RecvData( 
            /* [in] */ int p_iLen,
            /* [out] */ int __RPC_FAR *p_piRecvLen,
            /* [length_is][size_is][out] */ byte __RPC_FAR *p_pbData,
            /* [defaultvalue][in] */ int p_iTimeOut = -1) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelSendData( 
            /* [in] */ long p_lIden,
            /* [out] */ boolean __RPC_FAR *p_pbRet) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CancelRecvData( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ExtendComm( 
            /* [in] */ int p_iExtendType,
            /* [in] */ VARIANT p_varIn,
            /* [out] */ VARIANT __RPC_FAR *p_varOut) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SendAndRecvData( 
            /* [in] */ int p_iSendLen,
            /* [size_is][in] */ byte __RPC_FAR *p_pbSendData,
            /* [in] */ int p_iOutMaxLen,
            /* [out] */ int __RPC_FAR *p_piRecvLen,
            /* [length_is][size_is][out] */ byte __RPC_FAR *p_pbData,
            /* [defaultvalue][in] */ int p_iTimeOut = -1,
            /* [defaultvalue][in] */ boolean p_bExclusive = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ClearRecvBuf( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnterExclusiveSend( 
            /* [in] */ int p_iTimeOut) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReleaseExclusive( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDefaultCommConfig2( 
            /* [out][in] */ struct tCommCfg2 __RPC_FAR *p_psCommCfg) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentCfg2( 
            /* [out] */ struct tCommCfg2 __RPC_FAR *p_psCommCfg,
            /* [out] */ boolean __RPC_FAR *p_pbEnableEvents) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InitComm2( 
            /* [in] */ struct tCommCfg2 p_sCommCfg,
            /* [in] */ boolean p_bEnableEvents) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SendData2( 
            /* [in] */ int p_iLen,
            /* [size_is][in] */ byte __RPC_FAR *p_pbData,
            /* [out] */ long __RPC_FAR *p_plIden,
            /* [defaultvalue][in] */ short p_shPipe = 1) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RecvData2( 
            /* [in] */ int p_iLen,
            /* [out] */ int __RPC_FAR *p_piRecvLen,
            /* [length_is][size_is][out] */ byte __RPC_FAR *p_pbData,
            /* [defaultvalue][in] */ int p_iTimeOut = -1,
            /* [defaultvalue][in] */ short p_shPipe = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SendAndRecvData2( 
            /* [in] */ int p_iSendLen,
            /* [size_is][in] */ byte __RPC_FAR *p_pbSendData,
            /* [in] */ int p_iOutMaxLen,
            /* [out] */ int __RPC_FAR *p_piRecvLen,
            /* [length_is][size_is][out] */ byte __RPC_FAR *p_pbData,
            /* [defaultvalue][in] */ int p_iTimeOut = -1,
            /* [defaultvalue][in] */ short p_shSendPipe = 1,
            /* [defaultvalue][in] */ short p_shRecvPipe = 0,
            /* [defaultvalue][in] */ boolean p_bExclusive = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ClearRecvBuf2( 
            /* [defaultvalue][in] */ short p_shPipe = -1) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnterExclusiveSend2( 
            /* [in] */ int p_iTimeOut,
            /* [in] */ short p_shPipe) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReleaseExclusive2( 
            /* [in] */ short p_shPipe) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICommVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IComm __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IComm __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IComm __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDefaultCommConfig )( 
            IComm __RPC_FAR * This,
            /* [out][in] */ struct tCommCfg __RPC_FAR *p_psCommCfg);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentCfg )( 
            IComm __RPC_FAR * This,
            /* [out] */ struct tCommCfg __RPC_FAR *p_psCommCfg,
            /* [out] */ boolean __RPC_FAR *p_pbEnableEvents);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitComm )( 
            IComm __RPC_FAR * This,
            struct tCommCfg p_sCommCfg,
            /* [in] */ boolean p_bEnableEvents);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloseComm )( 
            IComm __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnableEvents )( 
            IComm __RPC_FAR * This,
            /* [in] */ boolean p_bEnableEvents);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEventsEnable )( 
            IComm __RPC_FAR * This,
            /* [out] */ boolean __RPC_FAR *p_pbEnableEvents);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendData )( 
            IComm __RPC_FAR * This,
            /* [in] */ int p_iLen,
            /* [size_is][in] */ byte __RPC_FAR *p_pbData,
            /* [out] */ long __RPC_FAR *p_plIden);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RecvData )( 
            IComm __RPC_FAR * This,
            /* [in] */ int p_iLen,
            /* [out] */ int __RPC_FAR *p_piRecvLen,
            /* [length_is][size_is][out] */ byte __RPC_FAR *p_pbData,
            /* [defaultvalue][in] */ int p_iTimeOut);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CancelSendData )( 
            IComm __RPC_FAR * This,
            /* [in] */ long p_lIden,
            /* [out] */ boolean __RPC_FAR *p_pbRet);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CancelRecvData )( 
            IComm __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ExtendComm )( 
            IComm __RPC_FAR * This,
            /* [in] */ int p_iExtendType,
            /* [in] */ VARIANT p_varIn,
            /* [out] */ VARIANT __RPC_FAR *p_varOut);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendAndRecvData )( 
            IComm __RPC_FAR * This,
            /* [in] */ int p_iSendLen,
            /* [size_is][in] */ byte __RPC_FAR *p_pbSendData,
            /* [in] */ int p_iOutMaxLen,
            /* [out] */ int __RPC_FAR *p_piRecvLen,
            /* [length_is][size_is][out] */ byte __RPC_FAR *p_pbData,
            /* [defaultvalue][in] */ int p_iTimeOut,
            /* [defaultvalue][in] */ boolean p_bExclusive);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClearRecvBuf )( 
            IComm __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnterExclusiveSend )( 
            IComm __RPC_FAR * This,
            /* [in] */ int p_iTimeOut);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReleaseExclusive )( 
            IComm __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDefaultCommConfig2 )( 
            IComm __RPC_FAR * This,
            /* [out][in] */ struct tCommCfg2 __RPC_FAR *p_psCommCfg);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCurrentCfg2 )( 
            IComm __RPC_FAR * This,
            /* [out] */ struct tCommCfg2 __RPC_FAR *p_psCommCfg,
            /* [out] */ boolean __RPC_FAR *p_pbEnableEvents);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitComm2 )( 
            IComm __RPC_FAR * This,
            /* [in] */ struct tCommCfg2 p_sCommCfg,
            /* [in] */ boolean p_bEnableEvents);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendData2 )( 
            IComm __RPC_FAR * This,
            /* [in] */ int p_iLen,
            /* [size_is][in] */ byte __RPC_FAR *p_pbData,
            /* [out] */ long __RPC_FAR *p_plIden,
            /* [defaultvalue][in] */ short p_shPipe);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RecvData2 )( 
            IComm __RPC_FAR * This,
            /* [in] */ int p_iLen,
            /* [out] */ int __RPC_FAR *p_piRecvLen,
            /* [length_is][size_is][out] */ byte __RPC_FAR *p_pbData,
            /* [defaultvalue][in] */ int p_iTimeOut,
            /* [defaultvalue][in] */ short p_shPipe);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendAndRecvData2 )( 
            IComm __RPC_FAR * This,
            /* [in] */ int p_iSendLen,
            /* [size_is][in] */ byte __RPC_FAR *p_pbSendData,
            /* [in] */ int p_iOutMaxLen,
            /* [out] */ int __RPC_FAR *p_piRecvLen,
            /* [length_is][size_is][out] */ byte __RPC_FAR *p_pbData,
            /* [defaultvalue][in] */ int p_iTimeOut,
            /* [defaultvalue][in] */ short p_shSendPipe,
            /* [defaultvalue][in] */ short p_shRecvPipe,
            /* [defaultvalue][in] */ boolean p_bExclusive);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClearRecvBuf2 )( 
            IComm __RPC_FAR * This,
            /* [defaultvalue][in] */ short p_shPipe);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnterExclusiveSend2 )( 
            IComm __RPC_FAR * This,
            /* [in] */ int p_iTimeOut,
            /* [in] */ short p_shPipe);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReleaseExclusive2 )( 
            IComm __RPC_FAR * This,
            /* [in] */ short p_shPipe);
        
        END_INTERFACE
    } ICommVtbl;

    interface IComm
    {
        CONST_VTBL struct ICommVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IComm_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IComm_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IComm_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IComm_GetDefaultCommConfig(This,p_psCommCfg)	\
    (This)->lpVtbl -> GetDefaultCommConfig(This,p_psCommCfg)

#define IComm_GetCurrentCfg(This,p_psCommCfg,p_pbEnableEvents)	\
    (This)->lpVtbl -> GetCurrentCfg(This,p_psCommCfg,p_pbEnableEvents)

#define IComm_InitComm(This,p_sCommCfg,p_bEnableEvents)	\
    (This)->lpVtbl -> InitComm(This,p_sCommCfg,p_bEnableEvents)

#define IComm_CloseComm(This)	\
    (This)->lpVtbl -> CloseComm(This)

#define IComm_EnableEvents(This,p_bEnableEvents)	\
    (This)->lpVtbl -> EnableEvents(This,p_bEnableEvents)

#define IComm_GetEventsEnable(This,p_pbEnableEvents)	\
    (This)->lpVtbl -> GetEventsEnable(This,p_pbEnableEvents)

#define IComm_SendData(This,p_iLen,p_pbData,p_plIden)	\
    (This)->lpVtbl -> SendData(This,p_iLen,p_pbData,p_plIden)

#define IComm_RecvData(This,p_iLen,p_piRecvLen,p_pbData,p_iTimeOut)	\
    (This)->lpVtbl -> RecvData(This,p_iLen,p_piRecvLen,p_pbData,p_iTimeOut)

#define IComm_CancelSendData(This,p_lIden,p_pbRet)	\
    (This)->lpVtbl -> CancelSendData(This,p_lIden,p_pbRet)

#define IComm_CancelRecvData(This)	\
    (This)->lpVtbl -> CancelRecvData(This)

#define IComm_ExtendComm(This,p_iExtendType,p_varIn,p_varOut)	\
    (This)->lpVtbl -> ExtendComm(This,p_iExtendType,p_varIn,p_varOut)

#define IComm_SendAndRecvData(This,p_iSendLen,p_pbSendData,p_iOutMaxLen,p_piRecvLen,p_pbData,p_iTimeOut,p_bExclusive)	\
    (This)->lpVtbl -> SendAndRecvData(This,p_iSendLen,p_pbSendData,p_iOutMaxLen,p_piRecvLen,p_pbData,p_iTimeOut,p_bExclusive)

#define IComm_ClearRecvBuf(This)	\
    (This)->lpVtbl -> ClearRecvBuf(This)

#define IComm_EnterExclusiveSend(This,p_iTimeOut)	\
    (This)->lpVtbl -> EnterExclusiveSend(This,p_iTimeOut)

#define IComm_ReleaseExclusive(This)	\
    (This)->lpVtbl -> ReleaseExclusive(This)

#define IComm_GetDefaultCommConfig2(This,p_psCommCfg)	\
    (This)->lpVtbl -> GetDefaultCommConfig2(This,p_psCommCfg)

#define IComm_GetCurrentCfg2(This,p_psCommCfg,p_pbEnableEvents)	\
    (This)->lpVtbl -> GetCurrentCfg2(This,p_psCommCfg,p_pbEnableEvents)

#define IComm_InitComm2(This,p_sCommCfg,p_bEnableEvents)	\
    (This)->lpVtbl -> InitComm2(This,p_sCommCfg,p_bEnableEvents)

#define IComm_SendData2(This,p_iLen,p_pbData,p_plIden,p_shPipe)	\
    (This)->lpVtbl -> SendData2(This,p_iLen,p_pbData,p_plIden,p_shPipe)

#define IComm_RecvData2(This,p_iLen,p_piRecvLen,p_pbData,p_iTimeOut,p_shPipe)	\
    (This)->lpVtbl -> RecvData2(This,p_iLen,p_piRecvLen,p_pbData,p_iTimeOut,p_shPipe)

#define IComm_SendAndRecvData2(This,p_iSendLen,p_pbSendData,p_iOutMaxLen,p_piRecvLen,p_pbData,p_iTimeOut,p_shSendPipe,p_shRecvPipe,p_bExclusive)	\
    (This)->lpVtbl -> SendAndRecvData2(This,p_iSendLen,p_pbSendData,p_iOutMaxLen,p_piRecvLen,p_pbData,p_iTimeOut,p_shSendPipe,p_shRecvPipe,p_bExclusive)

#define IComm_ClearRecvBuf2(This,p_shPipe)	\
    (This)->lpVtbl -> ClearRecvBuf2(This,p_shPipe)

#define IComm_EnterExclusiveSend2(This,p_iTimeOut,p_shPipe)	\
    (This)->lpVtbl -> EnterExclusiveSend2(This,p_iTimeOut,p_shPipe)

#define IComm_ReleaseExclusive2(This,p_shPipe)	\
    (This)->lpVtbl -> ReleaseExclusive2(This,p_shPipe)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IComm_GetDefaultCommConfig_Proxy( 
    IComm __RPC_FAR * This,
    /* [out][in] */ struct tCommCfg __RPC_FAR *p_psCommCfg);


void __RPC_STUB IComm_GetDefaultCommConfig_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_GetCurrentCfg_Proxy( 
    IComm __RPC_FAR * This,
    /* [out] */ struct tCommCfg __RPC_FAR *p_psCommCfg,
    /* [out] */ boolean __RPC_FAR *p_pbEnableEvents);


void __RPC_STUB IComm_GetCurrentCfg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_InitComm_Proxy( 
    IComm __RPC_FAR * This,
    struct tCommCfg p_sCommCfg,
    /* [in] */ boolean p_bEnableEvents);


void __RPC_STUB IComm_InitComm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_CloseComm_Proxy( 
    IComm __RPC_FAR * This);


void __RPC_STUB IComm_CloseComm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_EnableEvents_Proxy( 
    IComm __RPC_FAR * This,
    /* [in] */ boolean p_bEnableEvents);


void __RPC_STUB IComm_EnableEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_GetEventsEnable_Proxy( 
    IComm __RPC_FAR * This,
    /* [out] */ boolean __RPC_FAR *p_pbEnableEvents);


void __RPC_STUB IComm_GetEventsEnable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_SendData_Proxy( 
    IComm __RPC_FAR * This,
    /* [in] */ int p_iLen,
    /* [size_is][in] */ byte __RPC_FAR *p_pbData,
    /* [out] */ long __RPC_FAR *p_plIden);


void __RPC_STUB IComm_SendData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_RecvData_Proxy( 
    IComm __RPC_FAR * This,
    /* [in] */ int p_iLen,
    /* [out] */ int __RPC_FAR *p_piRecvLen,
    /* [length_is][size_is][out] */ byte __RPC_FAR *p_pbData,
    /* [defaultvalue][in] */ int p_iTimeOut);


void __RPC_STUB IComm_RecvData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_CancelSendData_Proxy( 
    IComm __RPC_FAR * This,
    /* [in] */ long p_lIden,
    /* [out] */ boolean __RPC_FAR *p_pbRet);


void __RPC_STUB IComm_CancelSendData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_CancelRecvData_Proxy( 
    IComm __RPC_FAR * This);


void __RPC_STUB IComm_CancelRecvData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_ExtendComm_Proxy( 
    IComm __RPC_FAR * This,
    /* [in] */ int p_iExtendType,
    /* [in] */ VARIANT p_varIn,
    /* [out] */ VARIANT __RPC_FAR *p_varOut);


void __RPC_STUB IComm_ExtendComm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_SendAndRecvData_Proxy( 
    IComm __RPC_FAR * This,
    /* [in] */ int p_iSendLen,
    /* [size_is][in] */ byte __RPC_FAR *p_pbSendData,
    /* [in] */ int p_iOutMaxLen,
    /* [out] */ int __RPC_FAR *p_piRecvLen,
    /* [length_is][size_is][out] */ byte __RPC_FAR *p_pbData,
    /* [defaultvalue][in] */ int p_iTimeOut,
    /* [defaultvalue][in] */ boolean p_bExclusive);


void __RPC_STUB IComm_SendAndRecvData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_ClearRecvBuf_Proxy( 
    IComm __RPC_FAR * This);


void __RPC_STUB IComm_ClearRecvBuf_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_EnterExclusiveSend_Proxy( 
    IComm __RPC_FAR * This,
    /* [in] */ int p_iTimeOut);


void __RPC_STUB IComm_EnterExclusiveSend_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_ReleaseExclusive_Proxy( 
    IComm __RPC_FAR * This);


void __RPC_STUB IComm_ReleaseExclusive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_GetDefaultCommConfig2_Proxy( 
    IComm __RPC_FAR * This,
    /* [out][in] */ struct tCommCfg2 __RPC_FAR *p_psCommCfg);


void __RPC_STUB IComm_GetDefaultCommConfig2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_GetCurrentCfg2_Proxy( 
    IComm __RPC_FAR * This,
    /* [out] */ struct tCommCfg2 __RPC_FAR *p_psCommCfg,
    /* [out] */ boolean __RPC_FAR *p_pbEnableEvents);


void __RPC_STUB IComm_GetCurrentCfg2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_InitComm2_Proxy( 
    IComm __RPC_FAR * This,
    /* [in] */ struct tCommCfg2 p_sCommCfg,
    /* [in] */ boolean p_bEnableEvents);


void __RPC_STUB IComm_InitComm2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_SendData2_Proxy( 
    IComm __RPC_FAR * This,
    /* [in] */ int p_iLen,
    /* [size_is][in] */ byte __RPC_FAR *p_pbData,
    /* [out] */ long __RPC_FAR *p_plIden,
    /* [defaultvalue][in] */ short p_shPipe);


void __RPC_STUB IComm_SendData2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_RecvData2_Proxy( 
    IComm __RPC_FAR * This,
    /* [in] */ int p_iLen,
    /* [out] */ int __RPC_FAR *p_piRecvLen,
    /* [length_is][size_is][out] */ byte __RPC_FAR *p_pbData,
    /* [defaultvalue][in] */ int p_iTimeOut,
    /* [defaultvalue][in] */ short p_shPipe);


void __RPC_STUB IComm_RecvData2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_SendAndRecvData2_Proxy( 
    IComm __RPC_FAR * This,
    /* [in] */ int p_iSendLen,
    /* [size_is][in] */ byte __RPC_FAR *p_pbSendData,
    /* [in] */ int p_iOutMaxLen,
    /* [out] */ int __RPC_FAR *p_piRecvLen,
    /* [length_is][size_is][out] */ byte __RPC_FAR *p_pbData,
    /* [defaultvalue][in] */ int p_iTimeOut,
    /* [defaultvalue][in] */ short p_shSendPipe,
    /* [defaultvalue][in] */ short p_shRecvPipe,
    /* [defaultvalue][in] */ boolean p_bExclusive);


void __RPC_STUB IComm_SendAndRecvData2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_ClearRecvBuf2_Proxy( 
    IComm __RPC_FAR * This,
    /* [defaultvalue][in] */ short p_shPipe);


void __RPC_STUB IComm_ClearRecvBuf2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_EnterExclusiveSend2_Proxy( 
    IComm __RPC_FAR * This,
    /* [in] */ int p_iTimeOut,
    /* [in] */ short p_shPipe);


void __RPC_STUB IComm_EnterExclusiveSend2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IComm_ReleaseExclusive2_Proxy( 
    IComm __RPC_FAR * This,
    /* [in] */ short p_shPipe);


void __RPC_STUB IComm_ReleaseExclusive2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IComm_INTERFACE_DEFINED__ */


#ifndef __ICommEvents_INTERFACE_DEFINED__
#define __ICommEvents_INTERFACE_DEFINED__

/* interface ICommEvents */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ICommEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DA66DED1-A165-11d7-9E16-0010B54D6738")
    ICommEvents : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnError( 
            /* [in] */ int p_iErrCode,
            /* [string][in] */ char __RPC_FAR *p_pcError) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnDataReceived( 
            /* [in] */ int p_iLen,
            /* [size_is][in] */ byte __RPC_FAR *p_pbData,
            /* [out] */ boolean __RPC_FAR *p_pbRemove) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICommEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICommEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICommEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICommEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnError )( 
            ICommEvents __RPC_FAR * This,
            /* [in] */ int p_iErrCode,
            /* [string][in] */ char __RPC_FAR *p_pcError);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnDataReceived )( 
            ICommEvents __RPC_FAR * This,
            /* [in] */ int p_iLen,
            /* [size_is][in] */ byte __RPC_FAR *p_pbData,
            /* [out] */ boolean __RPC_FAR *p_pbRemove);
        
        END_INTERFACE
    } ICommEventsVtbl;

    interface ICommEvents
    {
        CONST_VTBL struct ICommEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICommEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICommEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICommEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICommEvents_OnError(This,p_iErrCode,p_pcError)	\
    (This)->lpVtbl -> OnError(This,p_iErrCode,p_pcError)

#define ICommEvents_OnDataReceived(This,p_iLen,p_pbData,p_pbRemove)	\
    (This)->lpVtbl -> OnDataReceived(This,p_iLen,p_pbData,p_pbRemove)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICommEvents_OnError_Proxy( 
    ICommEvents __RPC_FAR * This,
    /* [in] */ int p_iErrCode,
    /* [string][in] */ char __RPC_FAR *p_pcError);


void __RPC_STUB ICommEvents_OnError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICommEvents_OnDataReceived_Proxy( 
    ICommEvents __RPC_FAR * This,
    /* [in] */ int p_iLen,
    /* [size_is][in] */ byte __RPC_FAR *p_pbData,
    /* [out] */ boolean __RPC_FAR *p_pbRemove);


void __RPC_STUB ICommEvents_OnDataReceived_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICommEvents_INTERFACE_DEFINED__ */



#ifndef __COMMMANAGERLib_LIBRARY_DEFINED__
#define __COMMMANAGERLib_LIBRARY_DEFINED__

/* library COMMMANAGERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_COMMMANAGERLib;

EXTERN_C const CLSID CLSID_Comm;

#ifdef __cplusplus

class DECLSPEC_UUID("F5857231-7094-11D7-9DD6-0010B54D6738")
Comm;
#endif
#endif /* __COMMMANAGERLib_LIBRARY_DEFINED__ */

/* interface __MIDL_itf_CommManager_0210 */
/* [local] */ 


#pragma pack(pop)


extern RPC_IF_HANDLE __MIDL_itf_CommManager_0210_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_CommManager_0210_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
