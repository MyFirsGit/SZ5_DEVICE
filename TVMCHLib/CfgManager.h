/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Nov 30 15:00:26 2011
 */
/* Compiler settings for D:\BuildWork\Dunite\GrgCfgManager\Src\CfgManager\CfgManager.idl:
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

#ifndef __CfgManager_h__
#define __CfgManager_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ICfgItem_FWD_DEFINED__
#define __ICfgItem_FWD_DEFINED__
typedef interface ICfgItem ICfgItem;
#endif 	/* __ICfgItem_FWD_DEFINED__ */


#ifndef __ICfgCollection_FWD_DEFINED__
#define __ICfgCollection_FWD_DEFINED__
typedef interface ICfgCollection ICfgCollection;
#endif 	/* __ICfgCollection_FWD_DEFINED__ */


#ifndef __ICfgAccess_FWD_DEFINED__
#define __ICfgAccess_FWD_DEFINED__
typedef interface ICfgAccess ICfgAccess;
#endif 	/* __ICfgAccess_FWD_DEFINED__ */


#ifndef __ICfgCollection_FWD_DEFINED__
#define __ICfgCollection_FWD_DEFINED__
typedef interface ICfgCollection ICfgCollection;
#endif 	/* __ICfgCollection_FWD_DEFINED__ */


#ifndef __ICfgItem_FWD_DEFINED__
#define __ICfgItem_FWD_DEFINED__
typedef interface ICfgItem ICfgItem;
#endif 	/* __ICfgItem_FWD_DEFINED__ */


#ifndef __ICfgAccess_FWD_DEFINED__
#define __ICfgAccess_FWD_DEFINED__
typedef interface ICfgAccess ICfgAccess;
#endif 	/* __ICfgAccess_FWD_DEFINED__ */


#ifndef __CfgAccess_FWD_DEFINED__
#define __CfgAccess_FWD_DEFINED__

#ifdef __cplusplus
typedef class CfgAccess CfgAccess;
#else
typedef struct CfgAccess CfgAccess;
#endif /* __cplusplus */

#endif 	/* __CfgAccess_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __ICfgItem_INTERFACE_DEFINED__
#define __ICfgItem_INTERFACE_DEFINED__

/* interface ICfgItem */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICfgItem;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E55C6C2B-A185-4480-AC36-DE43C3D20471")
    ICfgItem : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ItemName( 
            /* [retval][out] */ BSTR __RPC_FAR *p_pbstrName) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ItemValue( 
            /* [retval][out] */ VARIANT __RPC_FAR *p_pvarValue) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ItemType( 
            /* [retval][out] */ short __RPC_FAR *p_plItemType) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICfgItemVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICfgItem __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICfgItem __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICfgItem __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICfgItem __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICfgItem __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICfgItem __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICfgItem __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ItemName )( 
            ICfgItem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *p_pbstrName);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ItemValue )( 
            ICfgItem __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *p_pvarValue);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ItemType )( 
            ICfgItem __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *p_plItemType);
        
        END_INTERFACE
    } ICfgItemVtbl;

    interface ICfgItem
    {
        CONST_VTBL struct ICfgItemVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICfgItem_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICfgItem_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICfgItem_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICfgItem_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICfgItem_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICfgItem_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICfgItem_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICfgItem_get_ItemName(This,p_pbstrName)	\
    (This)->lpVtbl -> get_ItemName(This,p_pbstrName)

#define ICfgItem_get_ItemValue(This,p_pvarValue)	\
    (This)->lpVtbl -> get_ItemValue(This,p_pvarValue)

#define ICfgItem_get_ItemType(This,p_plItemType)	\
    (This)->lpVtbl -> get_ItemType(This,p_plItemType)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE ICfgItem_get_ItemName_Proxy( 
    ICfgItem __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *p_pbstrName);


void __RPC_STUB ICfgItem_get_ItemName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ICfgItem_get_ItemValue_Proxy( 
    ICfgItem __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *p_pvarValue);


void __RPC_STUB ICfgItem_get_ItemValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ICfgItem_get_ItemType_Proxy( 
    ICfgItem __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *p_plItemType);


void __RPC_STUB ICfgItem_get_ItemType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICfgItem_INTERFACE_DEFINED__ */


#ifndef __ICfgCollection_INTERFACE_DEFINED__
#define __ICfgCollection_INTERFACE_DEFINED__

/* interface ICfgCollection */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICfgCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("44212460-79CE-43A1-9822-BE657750C506")
    ICfgCollection : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *p_ppUnk) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long p_lIndex,
            /* [retval][out] */ ICfgItem __RPC_FAR *__RPC_FAR *p_ppvarCfgItem) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *p_plVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICfgCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICfgCollection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICfgCollection __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICfgCollection __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICfgCollection __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICfgCollection __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICfgCollection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICfgCollection __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get__NewEnum )( 
            ICfgCollection __RPC_FAR * This,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *p_ppUnk);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Item )( 
            ICfgCollection __RPC_FAR * This,
            /* [in] */ long p_lIndex,
            /* [retval][out] */ ICfgItem __RPC_FAR *__RPC_FAR *p_ppvarCfgItem);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            ICfgCollection __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *p_plVal);
        
        END_INTERFACE
    } ICfgCollectionVtbl;

    interface ICfgCollection
    {
        CONST_VTBL struct ICfgCollectionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICfgCollection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICfgCollection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICfgCollection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICfgCollection_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICfgCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICfgCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICfgCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICfgCollection_get__NewEnum(This,p_ppUnk)	\
    (This)->lpVtbl -> get__NewEnum(This,p_ppUnk)

#define ICfgCollection_get_Item(This,p_lIndex,p_ppvarCfgItem)	\
    (This)->lpVtbl -> get_Item(This,p_lIndex,p_ppvarCfgItem)

#define ICfgCollection_get_Count(This,p_plVal)	\
    (This)->lpVtbl -> get_Count(This,p_plVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget][id] */ HRESULT STDMETHODCALLTYPE ICfgCollection_get__NewEnum_Proxy( 
    ICfgCollection __RPC_FAR * This,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *p_ppUnk);


void __RPC_STUB ICfgCollection_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE ICfgCollection_get_Item_Proxy( 
    ICfgCollection __RPC_FAR * This,
    /* [in] */ long p_lIndex,
    /* [retval][out] */ ICfgItem __RPC_FAR *__RPC_FAR *p_ppvarCfgItem);


void __RPC_STUB ICfgCollection_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE ICfgCollection_get_Count_Proxy( 
    ICfgCollection __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *p_plVal);


void __RPC_STUB ICfgCollection_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICfgCollection_INTERFACE_DEFINED__ */


#ifndef __ICfgAccess_INTERFACE_DEFINED__
#define __ICfgAccess_INTERFACE_DEFINED__

/* interface ICfgAccess */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICfgAccess;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CA09C042-1505-4955-BA9F-F3D1289F4A40")
    ICfgAccess : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgOpenData( 
            short p_sSourceType,
            BSTR p_bstrSource,
            short p_psCreateNew,
            BSTR p_bstrExt,
            /* [retval][out] */ short __RPC_FAR *p_psRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgCloseData( 
            /* [retval][out] */ short __RPC_FAR *p_psRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgCreateSection( 
            BSTR p_bstrPSec,
            BSTR p_bstrNewSec,
            /* [retval][out] */ short __RPC_FAR *p_psRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgDeleteSection( 
            BSTR p_bstrPSec,
            BSTR p_bstrDelSec,
            /* [retval][out] */ short __RPC_FAR *p_psRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgGetStringValue( 
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            /* [retval][out] */ BSTR __RPC_FAR *p_pbstrValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgSetStringValue( 
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            BSTR p_bstrValue,
            /* [retval][out] */ short __RPC_FAR *p_psRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgGetIntValue( 
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            /* [retval][out] */ long __RPC_FAR *p_plValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgSetIntValue( 
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            long p_lValue,
            /* [retval][out] */ short __RPC_FAR *p_psRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgGetBinaryValue( 
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            /* [retval][out] */ VARIANT __RPC_FAR *p_pvarValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgSetBinaryValue( 
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            VARIANT p_varValue,
            /* [retval][out] */ short __RPC_FAR *p_psRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgCreateKey( 
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            VARIANT p_varInitValue,
            /* [retval][out] */ short __RPC_FAR *p_psRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgDeleteKey( 
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            /* [retval][out] */ short __RPC_FAR *p_psRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgGetAllofSameKey( 
            BSTR p_bstrPSection,
            BSTR p_bstrKeyName,
            /* [out] */ VARIANT __RPC_FAR *p_pvarValue,
            /* [retval][out] */ short __RPC_FAR *p_psRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgEnumChilds( 
            BSTR p_bstrPSection,
            /* [retval][out] */ ICfgCollection __RPC_FAR *__RPC_FAR *p_pvarCollection) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgCommit( 
            /* [retval][out] */ short __RPC_FAR *p_psRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgCreateSource( 
            BSTR p_bstrSource,
            short p_sSourceType,
            /* [retval][out] */ short __RPC_FAR *p_psRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgGetValues( 
            BSTR p_bstrSection,
            /* [in] */ VARIANT p_bstrKeyNames,
            /* [retval][out] */ VARIANT __RPC_FAR *p_pvarValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgSetValues( 
            BSTR p_bstrSection,
            /* [in] */ VARIANT p_bstrKeyNames,
            /* [in] */ VARIANT p_varValue,
            /* [retval][out] */ short __RPC_FAR *p_psRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgLock( 
            long p_lTimeout,
            /* [out] */ short __RPC_FAR *p_psRet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgUnlock( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CfgCleanup( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICfgAccessVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICfgAccess __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICfgAccess __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICfgAccess __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICfgAccess __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICfgAccess __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICfgAccess __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICfgAccess __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgOpenData )( 
            ICfgAccess __RPC_FAR * This,
            short p_sSourceType,
            BSTR p_bstrSource,
            short p_psCreateNew,
            BSTR p_bstrExt,
            /* [retval][out] */ short __RPC_FAR *p_psRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgCloseData )( 
            ICfgAccess __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *p_psRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgCreateSection )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrPSec,
            BSTR p_bstrNewSec,
            /* [retval][out] */ short __RPC_FAR *p_psRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgDeleteSection )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrPSec,
            BSTR p_bstrDelSec,
            /* [retval][out] */ short __RPC_FAR *p_psRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgGetStringValue )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            /* [retval][out] */ BSTR __RPC_FAR *p_pbstrValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgSetStringValue )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            BSTR p_bstrValue,
            /* [retval][out] */ short __RPC_FAR *p_psRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgGetIntValue )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            /* [retval][out] */ long __RPC_FAR *p_plValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgSetIntValue )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            long p_lValue,
            /* [retval][out] */ short __RPC_FAR *p_psRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgGetBinaryValue )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            /* [retval][out] */ VARIANT __RPC_FAR *p_pvarValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgSetBinaryValue )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            VARIANT p_varValue,
            /* [retval][out] */ short __RPC_FAR *p_psRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgCreateKey )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            VARIANT p_varInitValue,
            /* [retval][out] */ short __RPC_FAR *p_psRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgDeleteKey )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrSection,
            BSTR p_bstrKeyName,
            /* [retval][out] */ short __RPC_FAR *p_psRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgGetAllofSameKey )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrPSection,
            BSTR p_bstrKeyName,
            /* [out] */ VARIANT __RPC_FAR *p_pvarValue,
            /* [retval][out] */ short __RPC_FAR *p_psRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgEnumChilds )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrPSection,
            /* [retval][out] */ ICfgCollection __RPC_FAR *__RPC_FAR *p_pvarCollection);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgCommit )( 
            ICfgAccess __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *p_psRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgCreateSource )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrSource,
            short p_sSourceType,
            /* [retval][out] */ short __RPC_FAR *p_psRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgGetValues )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrSection,
            /* [in] */ VARIANT p_bstrKeyNames,
            /* [retval][out] */ VARIANT __RPC_FAR *p_pvarValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgSetValues )( 
            ICfgAccess __RPC_FAR * This,
            BSTR p_bstrSection,
            /* [in] */ VARIANT p_bstrKeyNames,
            /* [in] */ VARIANT p_varValue,
            /* [retval][out] */ short __RPC_FAR *p_psRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgLock )( 
            ICfgAccess __RPC_FAR * This,
            long p_lTimeout,
            /* [out] */ short __RPC_FAR *p_psRet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgUnlock )( 
            ICfgAccess __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CfgCleanup )( 
            ICfgAccess __RPC_FAR * This);
        
        END_INTERFACE
    } ICfgAccessVtbl;

    interface ICfgAccess
    {
        CONST_VTBL struct ICfgAccessVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICfgAccess_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICfgAccess_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICfgAccess_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICfgAccess_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICfgAccess_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICfgAccess_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICfgAccess_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICfgAccess_CfgOpenData(This,p_sSourceType,p_bstrSource,p_psCreateNew,p_bstrExt,p_psRet)	\
    (This)->lpVtbl -> CfgOpenData(This,p_sSourceType,p_bstrSource,p_psCreateNew,p_bstrExt,p_psRet)

#define ICfgAccess_CfgCloseData(This,p_psRet)	\
    (This)->lpVtbl -> CfgCloseData(This,p_psRet)

#define ICfgAccess_CfgCreateSection(This,p_bstrPSec,p_bstrNewSec,p_psRet)	\
    (This)->lpVtbl -> CfgCreateSection(This,p_bstrPSec,p_bstrNewSec,p_psRet)

#define ICfgAccess_CfgDeleteSection(This,p_bstrPSec,p_bstrDelSec,p_psRet)	\
    (This)->lpVtbl -> CfgDeleteSection(This,p_bstrPSec,p_bstrDelSec,p_psRet)

#define ICfgAccess_CfgGetStringValue(This,p_bstrSection,p_bstrKeyName,p_pbstrValue)	\
    (This)->lpVtbl -> CfgGetStringValue(This,p_bstrSection,p_bstrKeyName,p_pbstrValue)

#define ICfgAccess_CfgSetStringValue(This,p_bstrSection,p_bstrKeyName,p_bstrValue,p_psRet)	\
    (This)->lpVtbl -> CfgSetStringValue(This,p_bstrSection,p_bstrKeyName,p_bstrValue,p_psRet)

#define ICfgAccess_CfgGetIntValue(This,p_bstrSection,p_bstrKeyName,p_plValue)	\
    (This)->lpVtbl -> CfgGetIntValue(This,p_bstrSection,p_bstrKeyName,p_plValue)

#define ICfgAccess_CfgSetIntValue(This,p_bstrSection,p_bstrKeyName,p_lValue,p_psRet)	\
    (This)->lpVtbl -> CfgSetIntValue(This,p_bstrSection,p_bstrKeyName,p_lValue,p_psRet)

#define ICfgAccess_CfgGetBinaryValue(This,p_bstrSection,p_bstrKeyName,p_pvarValue)	\
    (This)->lpVtbl -> CfgGetBinaryValue(This,p_bstrSection,p_bstrKeyName,p_pvarValue)

#define ICfgAccess_CfgSetBinaryValue(This,p_bstrSection,p_bstrKeyName,p_varValue,p_psRet)	\
    (This)->lpVtbl -> CfgSetBinaryValue(This,p_bstrSection,p_bstrKeyName,p_varValue,p_psRet)

#define ICfgAccess_CfgCreateKey(This,p_bstrSection,p_bstrKeyName,p_varInitValue,p_psRet)	\
    (This)->lpVtbl -> CfgCreateKey(This,p_bstrSection,p_bstrKeyName,p_varInitValue,p_psRet)

#define ICfgAccess_CfgDeleteKey(This,p_bstrSection,p_bstrKeyName,p_psRet)	\
    (This)->lpVtbl -> CfgDeleteKey(This,p_bstrSection,p_bstrKeyName,p_psRet)

#define ICfgAccess_CfgGetAllofSameKey(This,p_bstrPSection,p_bstrKeyName,p_pvarValue,p_psRet)	\
    (This)->lpVtbl -> CfgGetAllofSameKey(This,p_bstrPSection,p_bstrKeyName,p_pvarValue,p_psRet)

#define ICfgAccess_CfgEnumChilds(This,p_bstrPSection,p_pvarCollection)	\
    (This)->lpVtbl -> CfgEnumChilds(This,p_bstrPSection,p_pvarCollection)

#define ICfgAccess_CfgCommit(This,p_psRet)	\
    (This)->lpVtbl -> CfgCommit(This,p_psRet)

#define ICfgAccess_CfgCreateSource(This,p_bstrSource,p_sSourceType,p_psRet)	\
    (This)->lpVtbl -> CfgCreateSource(This,p_bstrSource,p_sSourceType,p_psRet)

#define ICfgAccess_CfgGetValues(This,p_bstrSection,p_bstrKeyNames,p_pvarValue)	\
    (This)->lpVtbl -> CfgGetValues(This,p_bstrSection,p_bstrKeyNames,p_pvarValue)

#define ICfgAccess_CfgSetValues(This,p_bstrSection,p_bstrKeyNames,p_varValue,p_psRet)	\
    (This)->lpVtbl -> CfgSetValues(This,p_bstrSection,p_bstrKeyNames,p_varValue,p_psRet)

#define ICfgAccess_CfgLock(This,p_lTimeout,p_psRet)	\
    (This)->lpVtbl -> CfgLock(This,p_lTimeout,p_psRet)

#define ICfgAccess_CfgUnlock(This)	\
    (This)->lpVtbl -> CfgUnlock(This)

#define ICfgAccess_CfgCleanup(This)	\
    (This)->lpVtbl -> CfgCleanup(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgOpenData_Proxy( 
    ICfgAccess __RPC_FAR * This,
    short p_sSourceType,
    BSTR p_bstrSource,
    short p_psCreateNew,
    BSTR p_bstrExt,
    /* [retval][out] */ short __RPC_FAR *p_psRet);


void __RPC_STUB ICfgAccess_CfgOpenData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgCloseData_Proxy( 
    ICfgAccess __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *p_psRet);


void __RPC_STUB ICfgAccess_CfgCloseData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgCreateSection_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrPSec,
    BSTR p_bstrNewSec,
    /* [retval][out] */ short __RPC_FAR *p_psRet);


void __RPC_STUB ICfgAccess_CfgCreateSection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgDeleteSection_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrPSec,
    BSTR p_bstrDelSec,
    /* [retval][out] */ short __RPC_FAR *p_psRet);


void __RPC_STUB ICfgAccess_CfgDeleteSection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgGetStringValue_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrSection,
    BSTR p_bstrKeyName,
    /* [retval][out] */ BSTR __RPC_FAR *p_pbstrValue);


void __RPC_STUB ICfgAccess_CfgGetStringValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgSetStringValue_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrSection,
    BSTR p_bstrKeyName,
    BSTR p_bstrValue,
    /* [retval][out] */ short __RPC_FAR *p_psRet);


void __RPC_STUB ICfgAccess_CfgSetStringValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgGetIntValue_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrSection,
    BSTR p_bstrKeyName,
    /* [retval][out] */ long __RPC_FAR *p_plValue);


void __RPC_STUB ICfgAccess_CfgGetIntValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgSetIntValue_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrSection,
    BSTR p_bstrKeyName,
    long p_lValue,
    /* [retval][out] */ short __RPC_FAR *p_psRet);


void __RPC_STUB ICfgAccess_CfgSetIntValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgGetBinaryValue_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrSection,
    BSTR p_bstrKeyName,
    /* [retval][out] */ VARIANT __RPC_FAR *p_pvarValue);


void __RPC_STUB ICfgAccess_CfgGetBinaryValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgSetBinaryValue_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrSection,
    BSTR p_bstrKeyName,
    VARIANT p_varValue,
    /* [retval][out] */ short __RPC_FAR *p_psRet);


void __RPC_STUB ICfgAccess_CfgSetBinaryValue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgCreateKey_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrSection,
    BSTR p_bstrKeyName,
    VARIANT p_varInitValue,
    /* [retval][out] */ short __RPC_FAR *p_psRet);


void __RPC_STUB ICfgAccess_CfgCreateKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgDeleteKey_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrSection,
    BSTR p_bstrKeyName,
    /* [retval][out] */ short __RPC_FAR *p_psRet);


void __RPC_STUB ICfgAccess_CfgDeleteKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgGetAllofSameKey_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrPSection,
    BSTR p_bstrKeyName,
    /* [out] */ VARIANT __RPC_FAR *p_pvarValue,
    /* [retval][out] */ short __RPC_FAR *p_psRet);


void __RPC_STUB ICfgAccess_CfgGetAllofSameKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgEnumChilds_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrPSection,
    /* [retval][out] */ ICfgCollection __RPC_FAR *__RPC_FAR *p_pvarCollection);


void __RPC_STUB ICfgAccess_CfgEnumChilds_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgCommit_Proxy( 
    ICfgAccess __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *p_psRet);


void __RPC_STUB ICfgAccess_CfgCommit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgCreateSource_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrSource,
    short p_sSourceType,
    /* [retval][out] */ short __RPC_FAR *p_psRet);


void __RPC_STUB ICfgAccess_CfgCreateSource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgGetValues_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrSection,
    /* [in] */ VARIANT p_bstrKeyNames,
    /* [retval][out] */ VARIANT __RPC_FAR *p_pvarValue);


void __RPC_STUB ICfgAccess_CfgGetValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgSetValues_Proxy( 
    ICfgAccess __RPC_FAR * This,
    BSTR p_bstrSection,
    /* [in] */ VARIANT p_bstrKeyNames,
    /* [in] */ VARIANT p_varValue,
    /* [retval][out] */ short __RPC_FAR *p_psRet);


void __RPC_STUB ICfgAccess_CfgSetValues_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgLock_Proxy( 
    ICfgAccess __RPC_FAR * This,
    long p_lTimeout,
    /* [out] */ short __RPC_FAR *p_psRet);


void __RPC_STUB ICfgAccess_CfgLock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgUnlock_Proxy( 
    ICfgAccess __RPC_FAR * This);


void __RPC_STUB ICfgAccess_CfgUnlock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICfgAccess_CfgCleanup_Proxy( 
    ICfgAccess __RPC_FAR * This);


void __RPC_STUB ICfgAccess_CfgCleanup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICfgAccess_INTERFACE_DEFINED__ */



#ifndef __CFGMANAGERLib_LIBRARY_DEFINED__
#define __CFGMANAGERLib_LIBRARY_DEFINED__

/* library CFGMANAGERLib */
/* [helpstring][version][uuid] */ 


enum eRetVal
    {	CFG_SUCCESS	= 0,
	CFG_UPDATENEEDCOMMIT	= CFG_SUCCESS + 1,
	CFG_NODATASOURCE	= CFG_UPDATENEEDCOMMIT + 1,
	CFG_INVALIDSECTION	= CFG_NODATASOURCE + 1,
	CFG_INVALIDKEY	= CFG_INVALIDSECTION + 1,
	CFG_INVALIDDATATYPE	= CFG_INVALIDKEY + 1,
	CFG_LOCKED	= CFG_INVALIDDATATYPE + 1,
	CFG_FAIL	= CFG_LOCKED + 1
    };

enum eCfgFileType
    {	CFG_DEFAULTCFGDB	= 0,
	CFG_REGISTER	= CFG_DEFAULTCFGDB + 1,
	CFG_INIFILE	= CFG_REGISTER + 1,
	CFG_XMLFILE	= CFG_INIFILE + 1,
	CFG_ACCESSDB	= CFG_XMLFILE + 1
    };




EXTERN_C const IID LIBID_CFGMANAGERLib;

EXTERN_C const CLSID CLSID_CfgAccess;

#ifdef __cplusplus

class DECLSPEC_UUID("548AA8F2-6962-471B-9F7D-357D0D10D879")
CfgAccess;
#endif
#endif /* __CFGMANAGERLib_LIBRARY_DEFINED__ */

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
