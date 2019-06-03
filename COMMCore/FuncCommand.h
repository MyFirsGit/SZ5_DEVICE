#pragma once
#include "delegate.h"
#include "SysException.h"
#include "InnerException.h"
#include "Command.h"


template<typename>
class FuncCommand;
#define FUNC_GENERATOR(TEMPLATE_LIST,TYPE_LIST,TYPE_DECLARE_LIST,PARAM_LIST)\
template<typename R  TEMPLATE_LIST>\
class FuncCommand<R(TYPE_LIST)>\
	:public CCommand\
{\
	typedef Function::ICallback<R(TYPE_LIST)> BaseType;\
	typedef FuncCommand<R(TYPE_LIST)> SelfType;\
	BaseType* handler;\
private:\
	void InitHandler(R(*ptr)(TYPE_LIST))\
	{\
		handler = new Function::NativeCallback<R(TYPE_LIST)>(ptr);\
	}\
	template<typename C>\
	void InitHandler(const C* t,R(C::*ptr)(TYPE_LIST)  const)\
	{\
		handler = new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr);\
	}\
	template<typename C>\
	void InitHandler(C* t,R(C::*ptr)(TYPE_LIST) )\
	{\
		handler = new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr);\
	}\
	template<typename C>\
	void InitHandler(C* t,R(C::*ptr)(TYPE_LIST) const )\
	{\
		handler= new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr);\
	}\
	template<typename FunctorType>\
	void InitHandler(FunctorType* pFunctor)\
	{\
		InitHandler(pFunctor,&FunctorType::operator());\
	}\
	template<typename FunctorType>\
	void InitHandler(const FunctorType* pFunctor)\
	{\
		this->InitHandler(pFunctor,&FunctorType::operator());\
	}\
public:\
   TYPE_DECLARE_LIST\
	explicit FuncCommand(R(*ptr)(TYPE_LIST))\
	:CCommand()\
	{\
		InitHandler(ptr);\
	}\
	template<typename C>\
	FuncCommand(const C* t,R(C::*ptr)(TYPE_LIST) const)\
	:CCommand()\
	{\
		InitHandler(t,ptr);\
	}\
	template<typename C>\
	FuncCommand(C* t,R(C::*ptr)(TYPE_LIST) const)\
	:CCommand()\
	{\
		InitHandler(t,ptr);\
	}\
	template<typename C>\
	FuncCommand(C* t,R(C::*ptr)(TYPE_LIST))\
	:CCommand()\
	{\
		InitHandler(t,ptr);\
	}\
	template<typename FunctorType>\
	explicit FuncCommand(const FunctorType* pFunctor)\
	:CCommand()\
	{\
		InitHandler(pFunctor);\
	}\
	template<typename FunctorType>\
	explicit FuncCommand(FunctorType* pFunctor)\
	:CCommand()\
	{\
		InitHandler(pFunctor);\
	}\
	~FuncCommand()\
	{\
		if(handler!=NULL)\
		{\
			delete handler;\
			handler = NULL;\
		}\
	}\
	long ExecuteCommand()\
	{\
		if(handler!=NULL)\
		{\
			return handler->Invoke(PARAM_LIST);\
		}\
		return SP_SUCCESS;\
	}\
};\

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

#define TYPE_DECLARE_LIST_0 
#define TYPE_DECLARE_LIST_1 TYPE_DECLARE_LIST_0\
	T0 t0 ;
#define TYPE_DECLARE_LIST_2 TYPE_DECLARE_LIST_1\
	T1 t1;
#define TYPE_DECLARE_LIST_3 TYPE_DECLARE_LIST_2\
	T2 t2;
#define TYPE_DECLARE_LIST_4 TYPE_DECLARE_LIST_3\
	T3 t3;
#define TYPE_DECLARE_LIST_5 TYPE_DECLARE_LIST_4\
	T4 t4;
#define TYPE_DECLARE_LIST_6 TYPE_DECLARE_LIST_5\
	T5 t5;
#define TYPE_DECLARE_LIST_7 TYPE_DECLARE_LIST_6\
	T6 t6;
#define TYPE_DECLARE_LIST_8 TYPE_DECLARE_LIST_7\
	T7 t7;
#define TYPE_DECLARE_LIST_9 TYPE_DECLARE_LIST_8\
	T8 t8;
#define TYPE_DECLARE_LIST_10 TYPE_DECLARE_LIST_9\
	T9 t9;
#define TYPE_DECLARE_LIST_11 TYPE_DECLARE_LIST_10\
	T10 t10;


#define PARAM_LIST_0 
#define PARAM_LIST_1 t0
#define PARAM_LIST_2 PARAM_LIST_1, t1
#define PARAM_LIST_3 PARAM_LIST_2, t2
#define PARAM_LIST_4 PARAM_LIST_3, t3
#define PARAM_LIST_5 PARAM_LIST_4, t4
#define PARAM_LIST_6 PARAM_LIST_5, t5
#define PARAM_LIST_7 PARAM_LIST_6, t6
#define PARAM_LIST_8 PARAM_LIST_7, t7
#define PARAM_LIST_9 PARAM_LIST_8, t8
#define PARAM_LIST_10 PARAM_LIST_9, t9
#define PARAM_LIST_11 PARAM_LIST_10, t10

FUNC_GENERATOR(TEMPLATE_LIST_0,TYPE_LIST_0, TYPE_DECLARE_LIST_0,PARAM_LIST_0)
FUNC_GENERATOR(TEMPLATE_LIST_1, TYPE_LIST_1, TYPE_DECLARE_LIST_1,PARAM_LIST_1)
FUNC_GENERATOR(TEMPLATE_LIST_2, TYPE_LIST_2, TYPE_DECLARE_LIST_2,PARAM_LIST_2)
FUNC_GENERATOR(TEMPLATE_LIST_3, TYPE_LIST_3, TYPE_DECLARE_LIST_3,PARAM_LIST_3)
FUNC_GENERATOR(TEMPLATE_LIST_4, TYPE_LIST_4, TYPE_DECLARE_LIST_4,PARAM_LIST_4)
FUNC_GENERATOR(TEMPLATE_LIST_5, TYPE_LIST_5, TYPE_DECLARE_LIST_5,PARAM_LIST_5)
FUNC_GENERATOR(TEMPLATE_LIST_6, TYPE_LIST_6, TYPE_DECLARE_LIST_6,PARAM_LIST_6)
FUNC_GENERATOR(TEMPLATE_LIST_7, TYPE_LIST_7, TYPE_DECLARE_LIST_7,PARAM_LIST_7)
FUNC_GENERATOR(TEMPLATE_LIST_8, TYPE_LIST_8, TYPE_DECLARE_LIST_8,PARAM_LIST_8)
FUNC_GENERATOR(TEMPLATE_LIST_9, TYPE_LIST_9, TYPE_DECLARE_LIST_9,PARAM_LIST_9)
FUNC_GENERATOR(TEMPLATE_LIST_10, TYPE_LIST_10, TYPE_DECLARE_LIST_10,PARAM_LIST_10)
FUNC_GENERATOR(TEMPLATE_LIST_11, TYPE_LIST_11, TYPE_DECLARE_LIST_11,PARAM_LIST_11)

// remove the macros
#undef FUNC_GENERATOR
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

#undef TYPE_DECLARE_LIST_0
#undef TYPE_DECLARE_LIST_1
#undef TYPE_DECLARE_LIST_2
#undef TYPE_DECLARE_LIST_3
#undef TYPE_DECLARE_LIST_4
#undef TYPE_DECLARE_LIST_5
#undef TYPE_DECLARE_LIST_6
#undef TYPE_DECLARE_LIST_7
#undef TYPE_DECLARE_LIST_8
#undef TYPE_DECLARE_LIST_9
#undef TYPE_DECLARE_LIST_10
#undef TYPE_DECLARE_LIST_11

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



