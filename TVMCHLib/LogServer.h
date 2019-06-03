/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Nov 30 15:00:40 2011
 */
/* Compiler settings for D:\BuildWork\Dunite\GrgLogServer\Src\LogServer\LogServer.idl:
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

#ifndef __LogServer_h__
#define __LogServer_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ILogServer_FWD_DEFINED__
#define __ILogServer_FWD_DEFINED__
typedef interface ILogServer ILogServer;
#endif 	/* __ILogServer_FWD_DEFINED__ */


#ifndef __LogServer_FWD_DEFINED__
#define __LogServer_FWD_DEFINED__

#ifdef __cplusplus
typedef class LogServer LogServer;
#else
typedef struct LogServer LogServer;
#endif /* __cplusplus */

#endif 	/* __LogServer_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __ILogServer_INTERFACE_DEFINED__
#define __ILogServer_INTERFACE_DEFINED__

/* interface ILogServer */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ILogServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("783D1E25-869B-11D7-BCD2-0000E88263A3")
    ILogServer : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE hrInitLog( 
            /* [in] */ BSTR p_bstrSectionName,
            /* [in] */ BSTR p_bstrDevice) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE hrLog( 
            /* [in] */ BSTR p_bstrData,
            /* [in] */ int p_iPriority,
            /* [in] */ int p_iInfoCatalog) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE hrLogVariant( 
            /* [in] */ VARIANT p_varData,
            /* [in] */ int p_iPriority,
            /* [in] */ int p_iInfoCatalog,
            /* [defaultvalue][in] */ int p_iLen = 0) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE hrCloseLog( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE hrSetConfig( 
            /* [in] */ BSTR p_bstrName,
            /* [in] */ BSTR p_bstrValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE hrGetConfig( 
            /* [in] */ BSTR p_bstrName,
            /* [out] */ BSTR __RPC_FAR *p_bstrValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE hrWriteAllLog( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE hrLogRaw( 
            /* [in] */ BSTR p_bstrData,
            /* [in] */ int p_iPriority,
            /* [in] */ int p_iInfoCatalog) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE hrLogVariantRaw( 
            /* [in] */ VARIANT p_varData,
            /* [in] */ int p_iPriority,
            /* [in] */ int p_iInfoCatalog,
            /* [defaultvalue][in] */ int p_iLen = 0) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILogServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ILogServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ILogServer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ILogServer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ILogServer __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ILogServer __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ILogServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ILogServer __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *hrInitLog )( 
            ILogServer __RPC_FAR * This,
            /* [in] */ BSTR p_bstrSectionName,
            /* [in] */ BSTR p_bstrDevice);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *hrLog )( 
            ILogServer __RPC_FAR * This,
            /* [in] */ BSTR p_bstrData,
            /* [in] */ int p_iPriority,
            /* [in] */ int p_iInfoCatalog);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *hrLogVariant )( 
            ILogServer __RPC_FAR * This,
            /* [in] */ VARIANT p_varData,
            /* [in] */ int p_iPriority,
            /* [in] */ int p_iInfoCatalog,
            /* [defaultvalue][in] */ int p_iLen);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *hrCloseLog )( 
            ILogServer __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *hrSetConfig )( 
            ILogServer __RPC_FAR * This,
            /* [in] */ BSTR p_bstrName,
            /* [in] */ BSTR p_bstrValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *hrGetConfig )( 
            ILogServer __RPC_FAR * This,
            /* [in] */ BSTR p_bstrName,
            /* [out] */ BSTR __RPC_FAR *p_bstrValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *hrWriteAllLog )( 
            ILogServer __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *hrLogRaw )( 
            ILogServer __RPC_FAR * This,
            /* [in] */ BSTR p_bstrData,
            /* [in] */ int p_iPriority,
            /* [in] */ int p_iInfoCatalog);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *hrLogVariantRaw )( 
            ILogServer __RPC_FAR * This,
            /* [in] */ VARIANT p_varData,
            /* [in] */ int p_iPriority,
            /* [in] */ int p_iInfoCatalog,
            /* [defaultvalue][in] */ int p_iLen);
        
        END_INTERFACE
    } ILogServerVtbl;

    interface ILogServer
    {
        CONST_VTBL struct ILogServerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILogServer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILogServer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILogServer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILogServer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ILogServer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ILogServer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ILogServer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ILogServer_hrInitLog(This,p_bstrSectionName,p_bstrDevice)	\
    (This)->lpVtbl -> hrInitLog(This,p_bstrSectionName,p_bstrDevice)

#define ILogServer_hrLog(This,p_bstrData,p_iPriority,p_iInfoCatalog)	\
    (This)->lpVtbl -> hrLog(This,p_bstrData,p_iPriority,p_iInfoCatalog)

#define ILogServer_hrLogVariant(This,p_varData,p_iPriority,p_iInfoCatalog,p_iLen)	\
    (This)->lpVtbl -> hrLogVariant(This,p_varData,p_iPriority,p_iInfoCatalog,p_iLen)

#define ILogServer_hrCloseLog(This)	\
    (This)->lpVtbl -> hrCloseLog(This)

#define ILogServer_hrSetConfig(This,p_bstrName,p_bstrValue)	\
    (This)->lpVtbl -> hrSetConfig(This,p_bstrName,p_bstrValue)

#define ILogServer_hrGetConfig(This,p_bstrName,p_bstrValue)	\
    (This)->lpVtbl -> hrGetConfig(This,p_bstrName,p_bstrValue)

#define ILogServer_hrWriteAllLog(This)	\
    (This)->lpVtbl -> hrWriteAllLog(This)

#define ILogServer_hrLogRaw(This,p_bstrData,p_iPriority,p_iInfoCatalog)	\
    (This)->lpVtbl -> hrLogRaw(This,p_bstrData,p_iPriority,p_iInfoCatalog)

#define ILogServer_hrLogVariantRaw(This,p_varData,p_iPriority,p_iInfoCatalog,p_iLen)	\
    (This)->lpVtbl -> hrLogVariantRaw(This,p_varData,p_iPriority,p_iInfoCatalog,p_iLen)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILogServer_hrInitLog_Proxy( 
    ILogServer __RPC_FAR * This,
    /* [in] */ BSTR p_bstrSectionName,
    /* [in] */ BSTR p_bstrDevice);


void __RPC_STUB ILogServer_hrInitLog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILogServer_hrLog_Proxy( 
    ILogServer __RPC_FAR * This,
    /* [in] */ BSTR p_bstrData,
    /* [in] */ int p_iPriority,
    /* [in] */ int p_iInfoCatalog);


void __RPC_STUB ILogServer_hrLog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILogServer_hrLogVariant_Proxy( 
    ILogServer __RPC_FAR * This,
    /* [in] */ VARIANT p_varData,
    /* [in] */ int p_iPriority,
    /* [in] */ int p_iInfoCatalog,
    /* [defaultvalue][in] */ int p_iLen);


void __RPC_STUB ILogServer_hrLogVariant_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILogServer_hrCloseLog_Proxy( 
    ILogServer __RPC_FAR * This);


void __RPC_STUB ILogServer_hrCloseLog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILogServer_hrSetConfig_Proxy( 
    ILogServer __RPC_FAR * This,
    /* [in] */ BSTR p_bstrName,
    /* [in] */ BSTR p_bstrValue);


void __RPC_STUB ILogServer_hrSetConfig_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILogServer_hrGetConfig_Proxy( 
    ILogServer __RPC_FAR * This,
    /* [in] */ BSTR p_bstrName,
    /* [out] */ BSTR __RPC_FAR *p_bstrValue);


void __RPC_STUB ILogServer_hrGetConfig_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILogServer_hrWriteAllLog_Proxy( 
    ILogServer __RPC_FAR * This);


void __RPC_STUB ILogServer_hrWriteAllLog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILogServer_hrLogRaw_Proxy( 
    ILogServer __RPC_FAR * This,
    /* [in] */ BSTR p_bstrData,
    /* [in] */ int p_iPriority,
    /* [in] */ int p_iInfoCatalog);


void __RPC_STUB ILogServer_hrLogRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ILogServer_hrLogVariantRaw_Proxy( 
    ILogServer __RPC_FAR * This,
    /* [in] */ VARIANT p_varData,
    /* [in] */ int p_iPriority,
    /* [in] */ int p_iInfoCatalog,
    /* [defaultvalue][in] */ int p_iLen);


void __RPC_STUB ILogServer_hrLogVariantRaw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILogServer_INTERFACE_DEFINED__ */



#ifndef __LOGSERVERLib_LIBRARY_DEFINED__
#define __LOGSERVERLib_LIBRARY_DEFINED__

/* library LOGSERVERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_LOGSERVERLib;

EXTERN_C const CLSID CLSID_LogServer;

#ifdef __cplusplus

class DECLSPEC_UUID("783D1E26-869B-11D7-BCD2-0000E88263A3")
LogServer;
#endif
#endif /* __LOGSERVERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
