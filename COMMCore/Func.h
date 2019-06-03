#pragma once
#include "delegate.h"
#include "SysException.h"
#include "InnerException.h"
//
//template<typename>
//class Func;
//#define FUNC_GENERATOR(TEMPLATE_LIST,TEMPLATE_2_LIST,TYPE_LIST,TYPE_DECLARE_LIST,PARAM_GET_LIST,PARAM_ASSIGN_LIST,TYPE_PARAM_LIST,PARAM_LIST)\
//	template<typename R TEMPLATE_LIST>\
//class Func<R(TYPE_LIST)>\
//	:Function::ICallback<R(TYPE_LIST)>\
//{\
//	typedef Function::ICallback<R(TYPE_LIST)> BaseType;\
//	typedef Func<R(TYPE_LIST)> SelfType;\
//	BaseType* handler;\
//private:\
//	TYPE_DECLARE_LIST\
//	R result;\
//	CSysException* pException;\
//	HANDLE workThread;\
//	static DWORD WINAPI Work(LPVOID lpVoid)\
//	{\
//		SelfType* self = (SelfType*)lpVoid;\
//		try\
//		{\
//			self->result = self->Invoke(PARAM_GET_LIST);\
//		}\
//		catch(CSysException& e)\
//		{\
//			self->pException = new CSysException(e);\
//		}\
//		return 0;\
//	};\
//	void InitHandler(R(*ptr)(TYPE_LIST))\
//	{\
//		handler = new Function::NativeCallback<R(TYPE_LIST)>(ptr);\
//	}\
//	template<typename C>\
//	void InitHandler(const C* t,R(C::*ptr)(TYPE_LIST)  const)\
//	{\
//		handler = new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr);\
//	}\
//	template<typename C>\
//	void InitHandler(C* t,R(C::*ptr)(TYPE_LIST) )\
//	{\
//		handler = new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr);\
//	}\
//	template<typename C>\
//	void InitHandler(C* t,R(C::*ptr)(TYPE_LIST) const )\
//	{\
//		handler= new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr);\
//	}\
//	template<typename FunctorType>\
//	void InitHandler(FunctorType* pFunctor)\
//	{\
//		InitHandler(pFunctor,&FunctorType::operator());\
//	}\
//	template<typename FunctorType>\
//	void InitHandler(const FunctorType* pFunctor)\
//	{\
//		this->InitHandler(pFunctor,&FunctorType::operator());\
//	}\
//	bool Equals(const BaseType* pCallback) const\
//	{\
//		return false;\
//	}\
//	SelfType* Clone() const \
//	{\
//		throw CSysException(-1,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);\
//	}\
//public:\
//	explicit Func(R(*ptr)(TYPE_LIST))\
//	:pException(NULL)\
//	,workThread(NULL)\
//	{\
//		InitHandler(ptr);\
//	}\
//	template<typename C>\
//	Func(const C* t,R(C::*ptr)(TYPE_LIST) const)\
//	:pException(NULL)\
//	,workThread(NULL)\
//	{\
//		InitHandler(t,ptr);\
//	}\
//	template<typename C>\
//	Func(C* t,R(C::*ptr)(TYPE_LIST) const)\
//	:pException(NULL)\
//	,workThread(NULL)\
//	{\
//		InitHandler(t,ptr);\
//	}\
//	template<typename C>\
//	Func(C* t,R(C::*ptr)(TYPE_LIST))\
//	:pException(NULL)\
//	,workThread(NULL)\
//	{\
//		InitHandler(t,ptr);\
//	}\
//	template<typename FunctorType>\
//	explicit Func(const FunctorType* pFunctor)\
//	{\
//		InitHandler(pFunctor);\
//	}\
//	template<typename FunctorType>\
//	explicit Func(FunctorType* pFunctor)\
//	{\
//		InitHandler(pFunctor);\
//	}\
//	R Invoke(TYPE_PARAM_LIST) const\
//	{\
//		if(handler!=NULL)\
//		{\
//			return handler->Invoke(PARAM_LIST);\
//		}\
//		return R();\
//	}\
//	virtual ~Func()\
//	{\
//		if(workThread!=NULL && workThread != INVALID_HANDLE_VALUE)\
//		{\
//			try\
//			{\
//				EndInvoke();\
//			}\
//			catch(CSysException& e)\
//			{\
//			}\
//			catch(...)\
//			{\
//			}\
//		}\
//		if(handler!=NULL)\
//		{\
//			delete handler;\
//		}\
//		if(pException!=NULL)\
//		{\
//			delete pException;\
//			pException = NULL;\
//		}\
//	}\
//	void BeginInvoke(TYPE_PARAM_LIST)\
//	{\
//		PARAM_ASSIGN_LIST\
//		workThread = CreateThread(NULL,0,Work,this,0,NULL);\
//	};\
//	R EndInvoke()\
//	{\
//		if(workThread == NULL || workThread == INVALID_HANDLE_VALUE)\
//		{\
//			throw	CInnerException(-1 , CInnerException::OTHER_ERR, __FILE__, __LINE__);\
//		}\
//		DWORD exitCode;\
//		GetExitCodeThread(workThread,&exitCode);\
//		if(exitCode==STILL_ACTIVE)\
//		{\
//			WaitForSingleObject(workThread,INFINITE);\
//		}\
//		CloseHandle(workThread);\
//		workThread = NULL;\
//		if(pException!=NULL)\
//		{\
//			CSysException e(*pException);\
//			delete pException;\
//			pException = NULL;\
//			throw e;\
//		}\
//		return result;\
//	}\
//};\
//template<TEMPLATE_2_LIST>\
//class Func<void(TYPE_LIST)>\
//	:Function::ICallback<void(TYPE_LIST)>\
//{\
//	typedef Function::ICallback<void(TYPE_LIST)> BaseType;\
//	typedef Func<void(TYPE_LIST)> SelfType;\
//	BaseType* handler;\
//private:\
//	TYPE_DECLARE_LIST\
//	CSysException* pException;\
//	HANDLE workThread;\
//	static DWORD WINAPI Work(LPVOID lpVoid)\
//	{\
//		SelfType* self = (SelfType*)lpVoid;\
//		try\
//		{\
//			self->Invoke(PARAM_GET_LIST);\
//		}\
//		catch(CSysException& e)\
//		{\
//			self->pException = new CSysException(e);\
//		}\
//		return 0;\
//	};\
//	void InitHandler(void(*ptr)(TYPE_LIST))\
//	{\
//		handler = new Function::NativeCallback<void(TYPE_LIST)>(ptr);\
//	}\
//	template<typename C>\
//	void InitHandler(const C* t,void(C::*ptr)(TYPE_LIST)  const)\
//	{\
//		handler = new Function::MemberCallback<C,void(TYPE_LIST)>(t,ptr);\
//	}\
//	template<typename C>\
//	void InitHandler(C* t,void(C::*ptr)(TYPE_LIST) )\
//	{\
//		handler = new Function::MemberCallback<C,void(TYPE_LIST)>(t,ptr);\
//	}\
//	template<typename C>\
//	void InitHandler(C* t,void(C::*ptr)(TYPE_LIST) const )\
//	{\
//		handler= new Function::MemberCallback<C,void(TYPE_LIST)>(t,ptr);\
//\
//}\
//	template<typename FunctorType>\
//	void InitHandler(FunctorType* pFunctor)\
//	{\
//		InitHandler(pFunctor,&FunctorType::operator());\
//	}\
//	template<typename FunctorType>\
//	void InitHandler(const FunctorType* pFunctor)\
//	{\
//		this->InitHandler(pFunctor,&FunctorType::operator());\
//	}\
//	bool Equals(const BaseType* pCallback) const\
//	{\
//		return false;\
//	}\
//	SelfType* Clone() const \
//	{\
//		throw CSysException(-1,CInnerException::OTHER_ERR,__FILE__,__LINE__);\
//	}\
//public:\
//	explicit Func(void(*ptr)(TYPE_LIST))\
//	:pException(NULL)\
//	,workThread(NULL)\
//	{\
//		InitHandler(ptr);\
//	}\
//	template<typename C>\
//	Func(const C* t,void(C::*ptr)(TYPE_LIST) const)\
//	:pException(NULL)\
//	,workThread(NULL)\
//	{\
//		InitHandler(t,ptr);\
//	}\
//	template<typename C>\
//	Func(C* t,void(C::*ptr)(TYPE_LIST) const)\
//	:pException(NULL)\
//	,workThread(NULL)\
//	{\
//		InitHandler(t,ptr);\
//	}\
//	template<typename C>\
//	Func(C* t,void(C::*ptr)(TYPE_LIST))\
//	:pException(NULL)\
//	,workThread(NULL)\
//	{\
//		InitHandler(t,ptr);\
//	}\
//	template<typename FunctorType>\
//	explicit Func(const FunctorType* pFunctor)\
//	{\
//		InitHandler(pFunctor);\
//	}\
//	template<typename FunctorType>\
//	explicit Func(FunctorType* pFunctor)\
//	{\
//		InitHandler(pFunctor);\
//	}\
//	void Invoke(TYPE_PARAM_LIST) const\
//	{\
//		if(handler!=NULL)\
//		{\
//\
//		handler->Invoke(PARAM_LIST);\
//		}\
//	}\
//	virtual ~Func()\
//	{\
//		if(workThread!=NULL && workThread != INVALID_HANDLE_VALUE)\
//		{\
//			try\
//			{\
//				EndInvoke();\
//			}\
//			catch(CSysException& e)\
//			{\
//			}\
//			catch(...)\
//			{\
//			}\
//		}\
//		if(handler!=NULL)\
//		{\
//			delete handler;\
//		}\
//		if(pException!=NULL)\
//		{\
//			delete pException;\
//			pException = NULL;\
//		}\
//	}\
//	void BeginInvoke(TYPE_PARAM_LIST)\
//	{\
//		PARAM_ASSIGN_LIST\
//		workThread = CreateThread(NULL,0,Work,this,0,NULL);\
//	};\
//	void EndInvoke()\
//	{\
//		if(workThread == NULL || workThread == INVALID_HANDLE_VALUE)\
//		{\
//			throw	CInnerException(-1 , CInnerException::OTHER_ERR, __FILE__, __LINE__);\
//		}\
//		DWORD exitCode;\
//		GetExitCodeThread(workThread,&exitCode);\
//		if(exitCode==STILL_ACTIVE)\
//		{\
//			WaitForSingleObject(workThread,INFINITE);\
//		}\
//		CloseHandle(workThread);\
//		workThread = NULL;\
//		if(pException!=NULL)\
//		{\
//			CSysException e(*pException);\
//			delete pException;\
//			pException = NULL;\
//			throw e;\
//		}\
//	}\
//};
//
//#define TEMPLATE_LIST_0 
//#define TEMPLATE_LIST_1 TEMPLATE_LIST_0, typename T0
//#define TEMPLATE_LIST_2 TEMPLATE_LIST_1, typename T1
//#define TEMPLATE_LIST_3 TEMPLATE_LIST_2, typename T2
//#define TEMPLATE_LIST_4 TEMPLATE_LIST_3, typename T3
//#define TEMPLATE_LIST_5 TEMPLATE_LIST_4, typename T4
//#define TEMPLATE_LIST_6 TEMPLATE_LIST_5, typename T5
//#define TEMPLATE_LIST_7 TEMPLATE_LIST_6, typename T6
//#define TEMPLATE_LIST_8 TEMPLATE_LIST_7, typename T7
//#define TEMPLATE_LIST_9 TEMPLATE_LIST_8, typename T8
//#define TEMPLATE_LIST_10 TEMPLATE_LIST_9, typename T9
//#define TEMPLATE_LIST_11 TEMPLATE_LIST_10, typename T10
//
//#define TEMPLATE_2_LIST_0 
//#define TEMPLATE_2_LIST_1  typename T0
//#define TEMPLATE_2_LIST_2 TEMPLATE_2_LIST_1, typename T1
//#define TEMPLATE_2_LIST_3 TEMPLATE_2_LIST_2, typename T2
//#define TEMPLATE_2_LIST_4 TEMPLATE_2_LIST_3, typename T3
//#define TEMPLATE_2_LIST_5 TEMPLATE_2_LIST_4, typename T4
//#define TEMPLATE_2_LIST_6 TEMPLATE_2_LIST_5, typename T5
//#define TEMPLATE_2_LIST_7 TEMPLATE_2_LIST_6, typename T6
//#define TEMPLATE_2_LIST_8 TEMPLATE_2_LIST_7, typename T7
//#define TEMPLATE_2_LIST_9 TEMPLATE_2_LIST_8, typename T8
//#define TEMPLATE_2_LIST_10 TEMPLATE_2_LIST_9, typename T9
//#define TEMPLATE_2_LIST_11 TEMPLATE_2_LIST_10, typename T10
//
//#define TYPE_LIST_0
//#define TYPE_LIST_1 T0
//#define TYPE_LIST_2 TYPE_LIST_1, T1
//#define TYPE_LIST_3 TYPE_LIST_2, T2
//#define TYPE_LIST_4 TYPE_LIST_3, T3
//#define TYPE_LIST_5 TYPE_LIST_4, T4
//#define TYPE_LIST_6 TYPE_LIST_5, T5
//#define TYPE_LIST_7 TYPE_LIST_6, T6
//#define TYPE_LIST_8 TYPE_LIST_7, T7
//#define TYPE_LIST_9 TYPE_LIST_8, T8
//#define TYPE_LIST_10 TYPE_LIST_9, T9
//#define TYPE_LIST_11 TYPE_LIST_10, T10
//
//#define TYPE_PARAM_LIST_0
//#define TYPE_PARAM_LIST_1 T0 t0
//#define TYPE_PARAM_LIST_2 TYPE_PARAM_LIST_1, T1 t1
//#define TYPE_PARAM_LIST_3 TYPE_PARAM_LIST_2, T2 t2
//#define TYPE_PARAM_LIST_4 TYPE_PARAM_LIST_3, T3 t3
//#define TYPE_PARAM_LIST_5 TYPE_PARAM_LIST_4, T4 t4
//#define TYPE_PARAM_LIST_6 TYPE_PARAM_LIST_5, T5 t5
//#define TYPE_PARAM_LIST_7 TYPE_PARAM_LIST_6, T6 t6
//#define TYPE_PARAM_LIST_8 TYPE_PARAM_LIST_7, T7 t7
//#define TYPE_PARAM_LIST_9 TYPE_PARAM_LIST_8, T8 t8
//#define TYPE_PARAM_LIST_10 TYPE_PARAM_LIST_9, T9 t9
//#define TYPE_PARAM_LIST_11 TYPE_PARAM_LIST_10, T10 t10
//
//#define TYPE_DECLARE_LIST_0
//#define TYPE_DECLARE_LIST_1 TYPE_DECLARE_LIST_0\
//	T0 t0;
//#define TYPE_DECLARE_LIST_2 TYPE_DECLARE_LIST_1\
//	T1 t1;
//#define TYPE_DECLARE_LIST_3 TYPE_DECLARE_LIST_2\
//	T2 t2;
//#define TYPE_DECLARE_LIST_4 TYPE_DECLARE_LIST_3\
//	T3 t3;
//#define TYPE_DECLARE_LIST_5 TYPE_DECLARE_LIST_4\
//	T4 t4;
//#define TYPE_DECLARE_LIST_6 TYPE_DECLARE_LIST_5\
//	T5 t5;
//#define TYPE_DECLARE_LIST_7 TYPE_DECLARE_LIST_6\
//	T6 t6;
//#define TYPE_DECLARE_LIST_8 TYPE_DECLARE_LIST_7\
//	T7 t7;
//#define TYPE_DECLARE_LIST_9 TYPE_DECLARE_LIST_8\
//	T8 t8;
//#define TYPE_DECLARE_LIST_10 TYPE_DECLARE_LIST_9\
//	T9 t9;
//#define TYPE_DECLARE_LIST_11 TYPE_DECLARE_LIST_10\
//	T10 t10;
//
//#define PARAM_GET_LIST_0
//#define PARAM_GET_LIST_1 self->t0
//#define PARAM_GET_LIST_2 PARAM_GET_LIST_1, self->t1
//#define PARAM_GET_LIST_3 PARAM_GET_LIST_2, self->t2
//#define PARAM_GET_LIST_4 PARAM_GET_LIST_3, self->t3
//#define PARAM_GET_LIST_5 PARAM_GET_LIST_4, self->t4
//#define PARAM_GET_LIST_6 PARAM_GET_LIST_5, self->t5
//#define PARAM_GET_LIST_7 PARAM_GET_LIST_6, self->t6
//#define PARAM_GET_LIST_8 PARAM_GET_LIST_7, self->t7
//#define PARAM_GET_LIST_9 PARAM_GET_LIST_8, self->t8
//#define PARAM_GET_LIST_10 PARAM_GET_LIST_9, self->t9
//#define PARAM_GET_LIST_11 PARAM_GET_LIST_10, self->t10
//
//#define PARAM_LIST_0
//#define PARAM_LIST_1 t0
//#define PARAM_LIST_2 PARAM_LIST_1, t1
//#define PARAM_LIST_3 PARAM_LIST_2, t2
//#define PARAM_LIST_4 PARAM_LIST_3, t3
//#define PARAM_LIST_5 PARAM_LIST_4, t4
//#define PARAM_LIST_6 PARAM_LIST_5, t5
//#define PARAM_LIST_7 PARAM_LIST_6, t6
//#define PARAM_LIST_8 PARAM_LIST_7, t7
//#define PARAM_LIST_9 PARAM_LIST_8, t8
//#define PARAM_LIST_10 PARAM_LIST_9, t9
//#define PARAM_LIST_11 PARAM_LIST_10, t10
//
//#define PARAM_ASSIGN_LIST_0
//#define PARAM_ASSIGN_LIST_1 PARAM_ASSIGN_LIST_0\
//	this->t0 = t0;
//#define PARAM_ASSIGN_LIST_2 PARAM_ASSIGN_LIST_1\
//	this->t1 = t1;
//#define PARAM_ASSIGN_LIST_3 PARAM_ASSIGN_LIST_2\
//	this->t2 = t2;
//#define PARAM_ASSIGN_LIST_4 PARAM_ASSIGN_LIST_3\
//	this->t3 = t3;
//#define PARAM_ASSIGN_LIST_5 PARAM_ASSIGN_LIST_4\
//	this->t4 = t4;
//#define PARAM_ASSIGN_LIST_6 PARAM_ASSIGN_LIST_5\
//	this->t5 = t5;
//#define PARAM_ASSIGN_LIST_7 PARAM_ASSIGN_LIST_6\
//	this->t6 = t6;
//#define PARAM_ASSIGN_LIST_8 PARAM_ASSIGN_LIST_7\
//	this->t7 = t7;
//#define PARAM_ASSIGN_LIST_9 PARAM_ASSIGN_LIST_8\
//	this->t8 = t8;
//#define PARAM_ASSIGN_LIST_10 PARAM_ASSIGN_LIST_9\
//	this->t9 = t9;
//#define PARAM_ASSIGN_LIST_11 PARAM_ASSIGN_LIST_10\
//	this->t10 = t10;
//
//FUNC_GENERATOR(TEMPLATE_LIST_0, TEMPLATE_2_LIST_0,TYPE_LIST_0, TYPE_DECLARE_LIST_0,PARAM_GET_LIST_0,PARAM_ASSIGN_LIST_0,TYPE_PARAM_LIST_0,PARAM_LIST_0)
//FUNC_GENERATOR(TEMPLATE_LIST_1, TEMPLATE_2_LIST_1, TYPE_LIST_1, TYPE_DECLARE_LIST_1,PARAM_GET_LIST_1,PARAM_ASSIGN_LIST_1,TYPE_PARAM_LIST_1,PARAM_LIST_1)
//FUNC_GENERATOR(TEMPLATE_LIST_2, TEMPLATE_2_LIST_2, TYPE_LIST_2, TYPE_DECLARE_LIST_2,PARAM_GET_LIST_2,PARAM_ASSIGN_LIST_2,TYPE_PARAM_LIST_2,PARAM_LIST_2)
//FUNC_GENERATOR(TEMPLATE_LIST_3, TEMPLATE_2_LIST_3, TYPE_LIST_3, TYPE_DECLARE_LIST_3,PARAM_GET_LIST_3,PARAM_ASSIGN_LIST_3,TYPE_PARAM_LIST_3,PARAM_LIST_3)
//FUNC_GENERATOR(TEMPLATE_LIST_4, TEMPLATE_2_LIST_4, TYPE_LIST_4, TYPE_DECLARE_LIST_4,PARAM_GET_LIST_4,PARAM_ASSIGN_LIST_4,TYPE_PARAM_LIST_4,PARAM_LIST_4)
//FUNC_GENERATOR(TEMPLATE_LIST_5, TEMPLATE_2_LIST_5, TYPE_LIST_5, TYPE_DECLARE_LIST_5,PARAM_GET_LIST_5,PARAM_ASSIGN_LIST_5,TYPE_PARAM_LIST_5,PARAM_LIST_5)
//FUNC_GENERATOR(TEMPLATE_LIST_6, TEMPLATE_2_LIST_6, TYPE_LIST_6, TYPE_DECLARE_LIST_6,PARAM_GET_LIST_6,PARAM_ASSIGN_LIST_6,TYPE_PARAM_LIST_6,PARAM_LIST_6)
//FUNC_GENERATOR(TEMPLATE_LIST_7, TEMPLATE_2_LIST_7, TYPE_LIST_7, TYPE_DECLARE_LIST_7,PARAM_GET_LIST_7,PARAM_ASSIGN_LIST_7,TYPE_PARAM_LIST_7,PARAM_LIST_7)
//FUNC_GENERATOR(TEMPLATE_LIST_8, TEMPLATE_2_LIST_8, TYPE_LIST_8, TYPE_DECLARE_LIST_8,PARAM_GET_LIST_8,PARAM_ASSIGN_LIST_8,TYPE_PARAM_LIST_8,PARAM_LIST_8)
//FUNC_GENERATOR(TEMPLATE_LIST_9, TEMPLATE_2_LIST_9, TYPE_LIST_9, TYPE_DECLARE_LIST_9,PARAM_GET_LIST_9,PARAM_ASSIGN_LIST_9,TYPE_PARAM_LIST_9,PARAM_LIST_9)
//FUNC_GENERATOR(TEMPLATE_LIST_10, TEMPLATE_2_LIST_10, TYPE_LIST_10, TYPE_DECLARE_LIST_10,PARAM_GET_LIST_10,PARAM_ASSIGN_LIST_10,TYPE_PARAM_LIST_10,PARAM_LIST_10)
//FUNC_GENERATOR(TEMPLATE_LIST_11, TEMPLATE_2_LIST_11, TYPE_LIST_11, TYPE_DECLARE_LIST_11,PARAM_GET_LIST_11,PARAM_ASSIGN_LIST_11,TYPE_PARAM_LIST_11,PARAM_LIST_11)
//
//// remove the macros
//#undef FUNC_GENERATOR
//#undef TEMPLATE_LIST_0
//#undef TEMPLATE_LIST_1
//#undef TEMPLATE_LIST_2
//#undef TEMPLATE_LIST_3
//#undef TEMPLATE_LIST_4
//#undef TEMPLATE_LIST_5
//#undef TEMPLATE_LIST_6
//#undef TEMPLATE_LIST_7
//#undef TEMPLATE_LIST_8
//#undef TEMPLATE_LIST_9
//#undef TEMPLATE_LIST_10
//#undef TEMPLATE_LIST_11
//
//#undef TEMPLATE_2_LIST_0
//#undef TEMPLATE_2_LIST_1
//#undef TEMPLATE_2_LIST_2
//#undef TEMPLATE_2_LIST_3
//#undef TEMPLATE_2_LIST_4
//#undef TEMPLATE_2_LIST_5
//#undef TEMPLATE_2_LIST_6
//#undef TEMPLATE_2_LIST_7
//#undef TEMPLATE_2_LIST_8
//#undef TEMPLATE_2_LIST_9
//#undef TEMPLATE_2_LIST_10
//#undef TEMPLATE_2_LIST_11
//
//#undef TYPE_LIST_0
//#undef TYPE_LIST_1
//#undef TYPE_LIST_2
//#undef TYPE_LIST_3
//#undef TYPE_LIST_4
//#undef TYPE_LIST_5
//#undef TYPE_LIST_6
//#undef TYPE_LIST_7
//#undef TYPE_LIST_8
//#undef TYPE_LIST_9
//#undef TYPE_LIST_10
//#undef TYPE_LIST_11
//
//#undef TYPE_PARAM_LIST_0
//#undef TYPE_PARAM_LIST_1
//#undef TYPE_PARAM_LIST_2
//#undef TYPE_PARAM_LIST_3
//#undef TYPE_PARAM_LIST_4
//#undef TYPE_PARAM_LIST_5
//#undef TYPE_PARAM_LIST_6
//#undef TYPE_PARAM_LIST_7
//#undef TYPE_PARAM_LIST_8
//#undef TYPE_PARAM_LIST_9
//#undef TYPE_PARAM_LIST_10
//#undef TYPE_PARAM_LIST_11
//
//#undef TYPE_DECLARE_LIST_0
//#undef TYPE_DECLARE_LIST_1
//#undef TYPE_DECLARE_LIST_2
//#undef TYPE_DECLARE_LIST_3
//#undef TYPE_DECLARE_LIST_4
//#undef TYPE_DECLARE_LIST_5
//#undef TYPE_DECLARE_LIST_6
//#undef TYPE_DECLARE_LIST_7
//#undef TYPE_DECLARE_LIST_8
//#undef TYPE_DECLARE_LIST_9
//#undef TYPE_DECLARE_LIST_10
//#undef TYPE_DECLARE_LIST_11
//
//#undef PARAM_GET_LIST_0
//#undef PARAM_GET_LIST_1
//#undef PARAM_GET_LIST_2
//#undef PARAM_GET_LIST_3
//#undef PARAM_GET_LIST_4
//#undef PARAM_GET_LIST_5
//#undef PARAM_GET_LIST_6
//#undef PARAM_GET_LIST_7
//#undef PARAM_GET_LIST_8
//#undef PARAM_GET_LIST_9
//#undef PARAM_GET_LIST_10
//#undef PARAM_GET_LIST_11
//
//#undef PARAM_LIST_0
//#undef PARAM_LIST_1
//#undef PARAM_LIST_2
//#undef PARAM_LIST_3
//#undef PARAM_LIST_4
//#undef PARAM_LIST_5
//#undef PARAM_LIST_6
//#undef PARAM_LIST_7
//#undef PARAM_LIST_8
//#undef PARAM_LIST_9
//#undef PARAM_LIST_10
//#undef PARAM_LIST_11
//
//
//#undef PARAM_ASSIGN_LIST_0
//#undef PARAM_ASSIGN_LIST_1
//#undef PARAM_ASSIGN_LIST_2
//#undef PARAM_ASSIGN_LIST_3
//#undef PARAM_ASSIGN_LIST_4
//#undef PARAM_ASSIGN_LIST_5
//#undef PARAM_ASSIGN_LIST_6
//#undef PARAM_ASSIGN_LIST_7
//#undef PARAM_ASSIGN_LIST_8
//#undef PARAM_ASSIGN_LIST_9
//#undef PARAM_ASSIGN_LIST_10
//#undef PARAM_ASSIGN_LIST_11
//



template<typename T>
struct GetReferenceType
{
	typedef T& type;
};

template<typename T>
struct GetReferenceType<T&>
{
	typedef T& type;
};

template<typename>
struct FuncParameter;

#define PARAMETER_GENERATOR(TEMPLATE_2_LIST,TYPE_LIST,PARAMETER_TYPE_DECLARE,PARAMETER_TYPE_CONCONTURE_DECLARE,PARAMETER_ASSIGN)\
	template<TEMPLATE_2_LIST>\
struct FuncParameter<void(TYPE_LIST)>\
{\
	PARAMETER_TYPE_DECLARE\
	FuncParameter(PARAMETER_TYPE_CONCONTURE_DECLARE)\
	PARAMETER_ASSIGN\
{\
}\
};\

template<typename>
class Func;
#define FUNC_GENERATOR(TEMPLATE_LIST,TYPE_LIST,PARAM_GET_LIST,TYPE_PARAM_LIST,PARAM_LIST)\
	template<typename R TEMPLATE_LIST>\
class Func<R(TYPE_LIST)>\
	:Function::ICallback<R(TYPE_LIST)>\
{\
	typedef Function::ICallback<R(TYPE_LIST)> BaseType;\
	typedef Func<R(TYPE_LIST)> SelfType;\
	BaseType* handler;\
private:\
	FuncParameter<void(TYPE_LIST)> *parameter;\
	R result;\
	CSysException* pException;\
	HANDLE workThread;\
	static DWORD WINAPI Work(LPVOID lpVoid)\
{\
	SelfType* self = (SelfType*)lpVoid;\
	try\
{\
	self->result = self->Invoke(PARAM_GET_LIST);\
}\
	catch(CSysException& e)\
{\
	self->pException = new CSysException(e);\
}\
	return 0;\
};\
	void InitHandler(R(*ptr)(TYPE_LIST))\
{\
	handler = new Function::NativeCallback<R(TYPE_LIST)>(ptr);\
	parameter = NULL;\
}\
	template<typename C>\
	void InitHandler(const C* t,R(C::*ptr)(TYPE_LIST)  const)\
{\
	handler = new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr);\
	parameter = NULL;\
}\
	template<typename C>\
	void InitHandler(C* t,R(C::*ptr)(TYPE_LIST) )\
{\
	handler = new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr);\
	parameter = NULL;\
}\
	template<typename C>\
	void InitHandler(C* t,R(C::*ptr)(TYPE_LIST) const )\
{\
	handler= new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr);\
	parameter = NULL;\
}\
	template<typename FunctorType>\
	void InitHandler(FunctorType* pFunctor)\
{\
	InitHandler(pFunctor,&FunctorType::operator());\
	parameter = NULL;\
}\
	template<typename FunctorType>\
	void InitHandler(const FunctorType* pFunctor)\
{\
	this->InitHandler(pFunctor,&FunctorType::operator());\
	parameter = NULL;\
}\
	bool Equals(const BaseType* pCallback) const\
{\
	return false;\
}\
	SelfType* Clone() const \
{\
	throw CSysException(-1,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);\
}\
public:\
	explicit Func(R(*ptr)(TYPE_LIST))\
	:pException(NULL)\
	,workThread(NULL)\
{\
	InitHandler(ptr);\
}\
	template<typename C>\
	Func(const C* t,R(C::*ptr)(TYPE_LIST) const)\
	:pException(NULL)\
	,workThread(NULL)\
{\
	InitHandler(t,ptr);\
}\
	template<typename C>\
	Func(C* t,R(C::*ptr)(TYPE_LIST) const)\
	:pException(NULL)\
	,workThread(NULL)\
{\
	InitHandler(t,ptr);\
}\
	template<typename C>\
	Func(C* t,R(C::*ptr)(TYPE_LIST))\
	:pException(NULL)\
	,workThread(NULL)\
{\
	InitHandler(t,ptr);\
}\
	template<typename FunctorType>\
	explicit Func(const FunctorType* pFunctor)\
{\
	InitHandler(pFunctor);\
}\
	template<typename FunctorType>\
	explicit Func(FunctorType* pFunctor)\
{\
	InitHandler(pFunctor);\
}\
	R Invoke(TYPE_PARAM_LIST) const\
{\
	if(handler!=NULL)\
{\
	return handler->Invoke(PARAM_LIST);\
}\
	return R();\
}\
	virtual ~Func()\
{\
	if(workThread!=NULL && workThread != INVALID_HANDLE_VALUE)\
{\
	try\
{\
	EndInvoke();\
}\
	catch(...)\
{\
	if(handler!=NULL)\
{\
	delete handler;\
}\
}\
}\
	if(handler!=NULL)\
{\
	delete handler;\
}\
	if(pException!=NULL)\
{\
	delete pException;\
	pException = NULL;\
}\
}\
	void BeginInvoke(TYPE_PARAM_LIST)\
{\
	if(parameter!=NULL)\
{\
	delete parameter;\
	parameter = NULL;\
}\
	parameter = new FuncParameter<void(TYPE_LIST)>(PARAM_LIST);\
	workThread = CreateThread(NULL,0,Work,this,0,NULL);\
};\
	R EndInvoke()\
{\
	if(workThread == NULL || workThread == INVALID_HANDLE_VALUE)\
{\
	throw	CInnerException(-1 , CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);\
}\
	DWORD exitCode;\
	GetExitCodeThread(workThread,&exitCode);\
	if(exitCode==STILL_ACTIVE)\
{\
	WaitForSingleObject(workThread,INFINITE);\
}\
	CloseHandle(workThread);\
	workThread = NULL;\
	if(parameter!=NULL)\
{\
	delete parameter;\
	parameter = NULL;\
}\
	if(pException!=NULL)\
{\
	CSysException e(*pException);\
	delete pException;\
	pException = NULL;\
	throw e;\
}\
	return result;\
}\
};\

#define ACTION_GENERATOR(TEMPLATE_2_LIST,TYPE_LIST,PARAM_GET_LIST,TYPE_PARAM_LIST,PARAM_LIST)\
	template<TEMPLATE_2_LIST>\
class Func<void(TYPE_LIST)>\
	:Function::ICallback<void(TYPE_LIST)>\
{\
	typedef Function::ICallback<void(TYPE_LIST)> BaseType;\
	typedef Func<void(TYPE_LIST)> SelfType;\
	BaseType* handler;\
private:\
	FuncParameter<void(TYPE_LIST)> *parameter;\
	CSysException* pException;\
	HANDLE workThread;\
	static DWORD WINAPI Work(LPVOID lpVoid)\
{\
	SelfType* self = (SelfType*)lpVoid;\
	try\
{\
	self->Invoke(PARAM_GET_LIST);\
}\
	catch(CSysException& e)\
{\
	self->pException = new CSysException(e);\
}\
	return 0;\
};\
	void InitHandler(void(*ptr)(TYPE_LIST))\
{\
	handler = new Function::NativeCallback<void(TYPE_LIST)>(ptr);\
	parameter = NULL;\
}\
	template<typename C>\
	void InitHandler(const C* t,void(C::*ptr)(TYPE_LIST)  const)\
{\
	handler = new Function::MemberCallback<C,void(TYPE_LIST)>(t,ptr);\
	parameter = NULL;\
}\
	template<typename C>\
	void InitHandler(C* t,void(C::*ptr)(TYPE_LIST) )\
{\
	handler = new Function::MemberCallback<C,void(TYPE_LIST)>(t,ptr);\
	parameter = NULL;\
}\
	template<typename C>\
	void InitHandler(C* t,void(C::*ptr)(TYPE_LIST) const )\
{\
	handler= new Function::MemberCallback<C,void(TYPE_LIST)>(t,ptr);\
	parameter = NULL;\
}\
	template<typename FunctorType>\
	void InitHandler(FunctorType* pFunctor)\
{\
	InitHandler(pFunctor,&FunctorType::operator());\
	parameter = NULL;\
}\
	template<typename FunctorType>\
	void InitHandler(const FunctorType* pFunctor)\
{\
	this->InitHandler(pFunctor,&FunctorType::operator());\
	parameter = NULL;\
}\
	bool Equals(const BaseType* pCallback) const\
{\
	return false;\
}\
	SelfType* Clone() const \
{\
	throw CSysException(-1,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);\
}\
public:\
	explicit Func(void(*ptr)(TYPE_LIST))\
	:pException(NULL)\
	,workThread(NULL)\
{\
	InitHandler(ptr);\
}\
	template<typename C>\
	Func(const C* t,void(C::*ptr)(TYPE_LIST) const)\
	:pException(NULL)\
	,workThread(NULL)\
{\
	InitHandler(t,ptr);\
}\
	template<typename C>\
	Func(C* t,void(C::*ptr)(TYPE_LIST) const)\
	:pException(NULL)\
	,workThread(NULL)\
{\
	InitHandler(t,ptr);\
}\
	template<typename C>\
	Func(C* t,void(C::*ptr)(TYPE_LIST))\
	:pException(NULL)\
	,workThread(NULL)\
{\
	InitHandler(t,ptr);\
}\
	template<typename FunctorType>\
	explicit Func(const FunctorType* pFunctor)\
{\
	InitHandler(pFunctor);\
}\
	template<typename FunctorType>\
	explicit Func(FunctorType* pFunctor)\
{\
	InitHandler(pFunctor);\
}\
	void Invoke(TYPE_PARAM_LIST) const\
{\
	if(handler!=NULL)\
{\
	handler->Invoke(PARAM_LIST);\
}\
}\
	virtual ~Func()\
{\
	if(workThread!=NULL && workThread != INVALID_HANDLE_VALUE)\
{\
	try\
{\
	EndInvoke();\
}\
	catch(...)\
{\
}\
}\
	if(handler!=NULL)\
{\
	delete handler;\
}\
	if(pException!=NULL)\
{\
	delete pException;\
	pException = NULL;\
}\
}\
	void BeginInvoke(TYPE_PARAM_LIST)\
{\
	if(parameter!=NULL)\
{\
	delete parameter;\
	parameter = NULL;\
}\
	parameter = new FuncParameter<void(TYPE_LIST)>(PARAM_LIST);\
	workThread = CreateThread(NULL,0,Work,this,0,NULL);\
};\
	void EndInvoke()\
{\
	if(workThread == NULL || workThread == INVALID_HANDLE_VALUE)\
{\
	throw	CInnerException(-1 , CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);\
}\
	DWORD exitCode;\
	GetExitCodeThread(workThread,&exitCode);\
	if(exitCode==STILL_ACTIVE)\
{\
	WaitForSingleObject(workThread,INFINITE);\
}\
	CloseHandle(workThread);\
	workThread = NULL;\
	if(parameter!=NULL)\
{\
	delete parameter;\
	parameter = NULL;\
}\
	if(pException!=NULL)\
{\
	CSysException e(*pException);\
	delete pException;\
	pException = NULL;\
	throw e;\
}\
}\
};
#define  GENERATOR(TEMPLATE_LIST,TEMPLATE_2_LIST,TYPE_LIST,PARAMETER_TYPE_DECLARE,PARAMETER_TYPE_CONCONTURE_DECLARE,PARAM_ASSIGN,PARAM_GET_LIST,TYPE_PARAM_LIST,PARAM_LIST)\
	PARAMETER_GENERATOR(TEMPLATE_2_LIST,TYPE_LIST,PARAMETER_TYPE_DECLARE,PARAMETER_TYPE_CONCONTURE_DECLARE,PARAM_ASSIGN)\
	FUNC_GENERATOR(TEMPLATE_LIST,TYPE_LIST,PARAM_GET_LIST,TYPE_PARAM_LIST,PARAM_LIST)\
	ACTION_GENERATOR(TEMPLATE_2_LIST,TYPE_LIST,PARAM_GET_LIST,TYPE_PARAM_LIST,PARAM_LIST)\

#define TEMPLATE_LIST_0 
#define TEMPLATE_LIST_1 TEMPLATE_LIST_0, typename T0
#define TEMPLATE_LIST_2 TEMPLATE_LIST_1, typename T1
#define TEMPLATE_LIST_3 TEMPLATE_LIST_2, typename T2
#define TEMPLATE_LIST_4 TEMPLATE_LIST_3, typename T3
#define TEMPLATE_LIST_5 TEMPLATE_LIST_4, typename T4
#define TEMPLATE_LIST_6 TEMPLATE_LIST_5, typename T5
#define TEMPLATE_LIST_7 TEMPLATE_LIST_6, typename T6
#define TEMPLATE_LIST_8 TEMPLATE_LIST_7, typename T7
#define TEMPLATE_LIST_9 TEMPLATE_LIST_8, typename T8
#define TEMPLATE_LIST_10 TEMPLATE_LIST_9, typename T9
#define TEMPLATE_LIST_11 TEMPLATE_LIST_10, typename T10
#define TEMPLATE_2_LIST_0 
#define TEMPLATE_2_LIST_1  typename T0
#define TEMPLATE_2_LIST_2 TEMPLATE_2_LIST_1, typename T1
#define TEMPLATE_2_LIST_3 TEMPLATE_2_LIST_2, typename T2
#define TEMPLATE_2_LIST_4 TEMPLATE_2_LIST_3, typename T3
#define TEMPLATE_2_LIST_5 TEMPLATE_2_LIST_4, typename T4
#define TEMPLATE_2_LIST_6 TEMPLATE_2_LIST_5, typename T5
#define TEMPLATE_2_LIST_7 TEMPLATE_2_LIST_6, typename T6
#define TEMPLATE_2_LIST_8 TEMPLATE_2_LIST_7, typename T7
#define TEMPLATE_2_LIST_9 TEMPLATE_2_LIST_8, typename T8
#define TEMPLATE_2_LIST_10 TEMPLATE_2_LIST_9, typename T9
#define TEMPLATE_2_LIST_11 TEMPLATE_2_LIST_10, typename T10
#define PARAMETER_TYPE_DECLARE_0
#define PARAMETER_TYPE_DECLARE_1 PARAMETER_TYPE_DECLARE_0\
	typename GetReferenceType<T0>::type t0;
#define PARAMETER_TYPE_DECLARE_2 PARAMETER_TYPE_DECLARE_1\
	typename GetReferenceType<T1>::type t1;
#define PARAMETER_TYPE_DECLARE_3 PARAMETER_TYPE_DECLARE_2\
	typename GetReferenceType<T2>::type t2;
#define PARAMETER_TYPE_DECLARE_4 PARAMETER_TYPE_DECLARE_3\
	typename GetReferenceType<T3>::type t3;
#define PARAMETER_TYPE_DECLARE_5 PARAMETER_TYPE_DECLARE_4\
	typename GetReferenceType<T4>::type t4;
#define PARAMETER_TYPE_DECLARE_6 PARAMETER_TYPE_DECLARE_5\
	typename GetReferenceType<T5>::type t5;
#define PARAMETER_TYPE_DECLARE_7 PARAMETER_TYPE_DECLARE_6\
	typename GetReferenceType<T6>::type t6;
#define PARAMETER_TYPE_DECLARE_8 PARAMETER_TYPE_DECLARE_7\
	typedef typename GetReferenceType<T7>::type t7;
#define PARAMETER_TYPE_DECLARE_9 PARAMETER_TYPE_DECLARE_8\
	typename GetReferenceType<T8>::type t8;
#define PARAMETER_TYPE_DECLARE_10 PARAMETER_TYPE_DECLARE_9\
	typename GetReferenceType<T9>::type t9;
#define PARAMETER_TYPE_DECLARE_11 PARAMETER_TYPE_DECLARE_10\
	typename GetReferenceType<T10>::type t10;
#define PARAMETER_TYPE_CONCONTURE_DECLARE_0
#define PARAMETER_TYPE_CONCONTURE_DECLARE_1 typename GetReferenceType<T0>::type _t0
#define PARAMETER_TYPE_CONCONTURE_DECLARE_2 PARAMETER_TYPE_CONCONTURE_DECLARE_1, typename GetReferenceType<T1>::type _t1
#define PARAMETER_TYPE_CONCONTURE_DECLARE_3 PARAMETER_TYPE_CONCONTURE_DECLARE_2, typename GetReferenceType<T2>::type _t2
#define PARAMETER_TYPE_CONCONTURE_DECLARE_4 PARAMETER_TYPE_CONCONTURE_DECLARE_3, typename GetReferenceType<T3>::type _t3
#define PARAMETER_TYPE_CONCONTURE_DECLARE_5 PARAMETER_TYPE_CONCONTURE_DECLARE_4, typename GetReferenceType<T4>::type _t4
#define PARAMETER_TYPE_CONCONTURE_DECLARE_6 PARAMETER_TYPE_CONCONTURE_DECLARE_5, typename GetReferenceType<T5>::type _t5
#define PARAMETER_TYPE_CONCONTURE_DECLARE_7 PARAMETER_TYPE_CONCONTURE_DECLARE_6, typename GetReferenceType<T6>::type _t6
#define PARAMETER_TYPE_CONCONTURE_DECLARE_8 PARAMETER_TYPE_CONCONTURE_DECLARE_7, typename GetReferenceType<T7>::type _t7
#define PARAMETER_TYPE_CONCONTURE_DECLARE_9 PARAMETER_TYPE_CONCONTURE_DECLARE_8, typename GetReferenceType<T8>::type _t8
#define PARAMETER_TYPE_CONCONTURE_DECLARE_10 PARAMETER_TYPE_CONCONTURE_DECLARE_9, typename GetReferenceType<T9>::type _t9
#define PARAMETER_TYPE_CONCONTURE_DECLARE_11 PARAMETER_TYPE_CONCONTURE_DECLARE_10, typename GetReferenceType<T10>::type _t10
#define PARAMETER_ASSIGN_0
#define PARAMETER_ASSIGN_1 PARAMETER_ASSIGN_0:t0(_t0)
#define PARAMETER_ASSIGN_2 PARAMETER_ASSIGN_1,t1(_t1)
#define PARAMETER_ASSIGN_3 PARAMETER_ASSIGN_2,t2(_t2)
#define PARAMETER_ASSIGN_4 PARAMETER_ASSIGN_3,t3(_t3)
#define PARAMETER_ASSIGN_5 PARAMETER_ASSIGN_4,t4(_t4)
#define PARAMETER_ASSIGN_6 PARAMETER_ASSIGN_5,t5(_t5)
#define PARAMETER_ASSIGN_7 PARAMETER_ASSIGN_6,t6(_t6)
#define PARAMETER_ASSIGN_8 PARAMETER_ASSIGN_7,t7(_t7)
#define PARAMETER_ASSIGN_9 PARAMETER_ASSIGN_8,t8(_t8)
#define PARAMETER_ASSIGN_10 PARAMETER_ASSIGN_9,t9(_t9)
#define PARAMETER_ASSIGN_11 PARAMETER_ASSIGN_10,t10(_t10)
#define TYPE_LIST_0
#define TYPE_LIST_1 T0
#define TYPE_LIST_2 TYPE_LIST_1, T1
#define TYPE_LIST_3 TYPE_LIST_2, T2
#define TYPE_LIST_4 TYPE_LIST_3, T3
#define TYPE_LIST_5 TYPE_LIST_4, T4
#define TYPE_LIST_6 TYPE_LIST_5, T5
#define TYPE_LIST_7 TYPE_LIST_6, T6
#define TYPE_LIST_8 TYPE_LIST_7, T7
#define TYPE_LIST_9 TYPE_LIST_8, T8
#define TYPE_LIST_10 TYPE_LIST_9, T9
#define TYPE_LIST_11 TYPE_LIST_10, T10
#define TYPE_PARAM_LIST_0
#define TYPE_PARAM_LIST_1 T0 _t0
#define TYPE_PARAM_LIST_2 TYPE_PARAM_LIST_1, T1 _t1
#define TYPE_PARAM_LIST_3 TYPE_PARAM_LIST_2, T2 _t2
#define TYPE_PARAM_LIST_4 TYPE_PARAM_LIST_3, T3 _t3
#define TYPE_PARAM_LIST_5 TYPE_PARAM_LIST_4, T4 _t4
#define TYPE_PARAM_LIST_6 TYPE_PARAM_LIST_5, T5 _t5
#define TYPE_PARAM_LIST_7 TYPE_PARAM_LIST_6, T6 _t6
#define TYPE_PARAM_LIST_8 TYPE_PARAM_LIST_7, T7 _t7
#define TYPE_PARAM_LIST_9 TYPE_PARAM_LIST_8, T8 _t8
#define TYPE_PARAM_LIST_10 TYPE_PARAM_LIST_9, T9 _t9
#define TYPE_PARAM_LIST_11 TYPE_PARAM_LIST_10, T10 _t10
#define PARAM_GET_LIST_0
#define PARAM_GET_LIST_1 self->parameter->t0
#define PARAM_GET_LIST_2 PARAM_GET_LIST_1, self->parameter->t1
#define PARAM_GET_LIST_3 PARAM_GET_LIST_2, self->parameter->t2
#define PARAM_GET_LIST_4 PARAM_GET_LIST_3, self->parameter->t3
#define PARAM_GET_LIST_5 PARAM_GET_LIST_4, self->parameter->t4
#define PARAM_GET_LIST_6 PARAM_GET_LIST_5, self->parameter->t5
#define PARAM_GET_LIST_7 PARAM_GET_LIST_6, self->parameter->t6
#define PARAM_GET_LIST_8 PARAM_GET_LIST_7, self->parameter->t7
#define PARAM_GET_LIST_9 PARAM_GET_LIST_8, self->parameter->t8
#define PARAM_GET_LIST_10 PARAM_GET_LIST_9, self->parameter->t9
#define PARAM_GET_LIST_11 PARAM_GET_LIST_10, self->parameter->t10
#define PARAM_LIST_0
#define PARAM_LIST_1 _t0
#define PARAM_LIST_2 PARAM_LIST_1, _t1
#define PARAM_LIST_3 PARAM_LIST_2, _t2
#define PARAM_LIST_4 PARAM_LIST_3, _t3
#define PARAM_LIST_5 PARAM_LIST_4, _t4
#define PARAM_LIST_6 PARAM_LIST_5, _t5
#define PARAM_LIST_7 PARAM_LIST_6, _t6
#define PARAM_LIST_8 PARAM_LIST_7, _t7
#define PARAM_LIST_9 PARAM_LIST_8, _t8
#define PARAM_LIST_10 PARAM_LIST_9, _t9
#define PARAM_LIST_11 PARAM_LIST_10, t10
GENERATOR(TEMPLATE_LIST_0, TEMPLATE_2_LIST_0,TYPE_LIST_0, PARAMETER_TYPE_DECLARE_0,PARAMETER_TYPE_CONCONTURE_DECLARE_0,PARAMETER_ASSIGN_0,PARAM_GET_LIST_0,TYPE_PARAM_LIST_0,PARAM_LIST_0)
GENERATOR(TEMPLATE_LIST_1, TEMPLATE_2_LIST_1,TYPE_LIST_1, PARAMETER_TYPE_DECLARE_1,PARAMETER_TYPE_CONCONTURE_DECLARE_1,PARAMETER_ASSIGN_1,PARAM_GET_LIST_1,TYPE_PARAM_LIST_1,PARAM_LIST_1)
GENERATOR(TEMPLATE_LIST_2, TEMPLATE_2_LIST_2,TYPE_LIST_2, PARAMETER_TYPE_DECLARE_2, PARAMETER_TYPE_CONCONTURE_DECLARE_2,PARAMETER_ASSIGN_2,PARAM_GET_LIST_2,TYPE_PARAM_LIST_2,PARAM_LIST_2)
GENERATOR(TEMPLATE_LIST_3, TEMPLATE_2_LIST_3,TYPE_LIST_3, PARAMETER_TYPE_DECLARE_3,PARAMETER_TYPE_CONCONTURE_DECLARE_3,PARAMETER_ASSIGN_3,PARAM_GET_LIST_3,TYPE_PARAM_LIST_3,PARAM_LIST_3)
GENERATOR(TEMPLATE_LIST_4, TEMPLATE_2_LIST_4,TYPE_LIST_4, PARAMETER_TYPE_DECLARE_4,PARAMETER_TYPE_CONCONTURE_DECLARE_4,PARAMETER_ASSIGN_4,PARAM_GET_LIST_4,TYPE_PARAM_LIST_4,PARAM_LIST_4)
GENERATOR(TEMPLATE_LIST_5, TEMPLATE_2_LIST_5,TYPE_LIST_5, PARAMETER_TYPE_DECLARE_5,PARAMETER_TYPE_CONCONTURE_DECLARE_5,PARAMETER_ASSIGN_5,PARAM_GET_LIST_5,TYPE_PARAM_LIST_5,PARAM_LIST_5)
GENERATOR(TEMPLATE_LIST_6, TEMPLATE_2_LIST_6,TYPE_LIST_6, PARAMETER_TYPE_DECLARE_6,PARAMETER_TYPE_CONCONTURE_DECLARE_6,PARAMETER_ASSIGN_6,PARAM_GET_LIST_6,TYPE_PARAM_LIST_6,PARAM_LIST_6)
GENERATOR(TEMPLATE_LIST_7, TEMPLATE_2_LIST_7,TYPE_LIST_7, PARAMETER_TYPE_DECLARE_7,PARAMETER_TYPE_CONCONTURE_DECLARE_7,PARAMETER_ASSIGN_7,PARAM_GET_LIST_7,TYPE_PARAM_LIST_7,PARAM_LIST_7)
GENERATOR(TEMPLATE_LIST_8, TEMPLATE_2_LIST_8,TYPE_LIST_8, PARAMETER_TYPE_DECLARE_8,PARAMETER_TYPE_CONCONTURE_DECLARE_8,PARAMETER_ASSIGN_8,PARAM_GET_LIST_8,TYPE_PARAM_LIST_8,PARAM_LIST_8)
GENERATOR(TEMPLATE_LIST_9, TEMPLATE_2_LIST_9,TYPE_LIST_9, PARAMETER_TYPE_DECLARE_9,PARAMETER_TYPE_CONCONTURE_DECLARE_9,PARAMETER_ASSIGN_9,PARAM_GET_LIST_9,TYPE_PARAM_LIST_9,PARAM_LIST_9)
GENERATOR(TEMPLATE_LIST_10, TEMPLATE_2_LIST_10,TYPE_LIST_10, PARAMETER_TYPE_DECLARE_10,PARAMETER_TYPE_CONCONTURE_DECLARE_10,PARAMETER_ASSIGN_10,PARAM_GET_LIST_10,TYPE_PARAM_LIST_10,PARAM_LIST_10)
GENERATOR(TEMPLATE_LIST_11, TEMPLATE_2_LIST_11,TYPE_LIST_11, PARAMETER_TYPE_DECLARE_11,PARAMETER_TYPE_CONCONTURE_DECLARE_11,PARAMETER_ASSIGN_11,PARAM_GET_LIST_11,TYPE_PARAM_LIST_11,PARAM_LIST_11)

// remove the macros

#undef FUNC_GENERATOR
#undef PARAMETER_GENERATOR
#undef ACTION_GENERATOR
#undef GENERATOR

#undef TEMPLATE_LIST_0
#undef TEMPLATE_LIST_1
#undef TEMPLATE_LIST_2
#undef TEMPLATE_LIST_3
#undef TEMPLATE_LIST_4
#undef TEMPLATE_LIST_5
#undef TEMPLATE_LIST_6
#undef TEMPLATE_LIST_7
#undef TEMPLATE_LIST_8
#undef TEMPLATE_LIST_9
#undef TEMPLATE_LIST_10
#undef TEMPLATE_LIST_11

#undef TEMPLATE_2_LIST_0
#undef TEMPLATE_2_LIST_1
#undef TEMPLATE_2_LIST_2
#undef TEMPLATE_2_LIST_3
#undef TEMPLATE_2_LIST_4
#undef TEMPLATE_2_LIST_5
#undef TEMPLATE_2_LIST_6
#undef TEMPLATE_2_LIST_7
#undef TEMPLATE_2_LIST_8
#undef TEMPLATE_2_LIST_9
#undef TEMPLATE_2_LIST_10
#undef TEMPLATE_2_LIST_11

#undef TYPE_LIST_0
#undef TYPE_LIST_1
#undef TYPE_LIST_2
#undef TYPE_LIST_3
#undef TYPE_LIST_4
#undef TYPE_LIST_5
#undef TYPE_LIST_6
#undef TYPE_LIST_7
#undef TYPE_LIST_8
#undef TYPE_LIST_9
#undef TYPE_LIST_10
#undef TYPE_LIST_11

#undef TYPE_PARAM_LIST_0
#undef TYPE_PARAM_LIST_1
#undef TYPE_PARAM_LIST_2
#undef TYPE_PARAM_LIST_3
#undef TYPE_PARAM_LIST_4
#undef TYPE_PARAM_LIST_5
#undef TYPE_PARAM_LIST_6
#undef TYPE_PARAM_LIST_7
#undef TYPE_PARAM_LIST_8
#undef TYPE_PARAM_LIST_9
#undef TYPE_PARAM_LIST_10
#undef TYPE_PARAM_LIST_11


#undef PARAM_GET_LIST_0
#undef PARAM_GET_LIST_1
#undef PARAM_GET_LIST_2
#undef PARAM_GET_LIST_3
#undef PARAM_GET_LIST_4
#undef PARAM_GET_LIST_5
#undef PARAM_GET_LIST_6
#undef PARAM_GET_LIST_7
#undef PARAM_GET_LIST_8
#undef PARAM_GET_LIST_9
#undef PARAM_GET_LIST_10
#undef PARAM_GET_LIST_11

#undef PARAM_LIST_0
#undef PARAM_LIST_1
#undef PARAM_LIST_2
#undef PARAM_LIST_3
#undef PARAM_LIST_4
#undef PARAM_LIST_5
#undef PARAM_LIST_6
#undef PARAM_LIST_7
#undef PARAM_LIST_8
#undef PARAM_LIST_9
#undef PARAM_LIST_10
#undef PARAM_LIST_11


#undef PARAMETER_ASSIGN_0
#undef PARAMETER_ASSIGN_1
#undef PARAMETER_ASSIGN_2
#undef PARAMETER_ASSIGN_3
#undef PARAMETER_ASSIGN_4
#undef PARAMETER_ASSIGN_5
#undef PARAMETER_ASSIGN_6
#undef PARAMETER_ASSIGN_7
#undef PARAMETER_ASSIGN_8
#undef PARAMETER_ASSIGN_9
#undef PARAMETER_ASSIGN_10
#undef PARAMETER_ASSIGN_11


#undef PARAMETER_TYPE_CONCONTURE_DECLARE_0
#undef PARAMETER_TYPE_CONCONTURE_DECLARE_1
#undef PARAMETER_TYPE_CONCONTURE_DECLARE_2
#undef PARAMETER_TYPE_CONCONTURE_DECLARE_3
#undef PARAMETER_TYPE_CONCONTURE_DECLARE_4
#undef PARAMETER_TYPE_CONCONTURE_DECLARE_5
#undef PARAMETER_TYPE_CONCONTURE_DECLARE_6
#undef PARAMETER_TYPE_CONCONTURE_DECLARE_7
#undef PARAMETER_TYPE_CONCONTURE_DECLARE_8
#undef PARAMETER_TYPE_CONCONTURE_DECLARE_9
#undef PARAMETER_TYPE_CONCONTURE_DECLARE_10
#undef PARAMETER_TYPE_CONCONTURE_DECLARE_11

#undef PARAMETER_TYPE_DECLARE_0
#undef PARAMETER_TYPE_DECLARE_1
#undef PARAMETER_TYPE_DECLARE_2
#undef PARAMETER_TYPE_DECLARE_3
#undef PARAMETER_TYPE_DECLARE_4
#undef PARAMETER_TYPE_DECLARE_5
#undef PARAMETER_TYPE_DECLARE_6
#undef PARAMETER_TYPE_DECLARE_7
#undef PARAMETER_TYPE_DECLARE_8
#undef PARAMETER_TYPE_DECLARE_9
#undef PARAMETER_TYPE_DECLARE_10
#undef PARAMETER_TYPE_DECLARE_11
