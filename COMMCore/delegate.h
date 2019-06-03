#pragma once

namespace Function
{
	template<typename>
	class ICallback;

	template<typename>
	class NativeCallback;

	template<typename,typename>
	class MemberCallback;
}
template<typename> class delegate;
#define DELEGATE_GENERATOR(TEMPLATE_LIST,TYPE_LIST,TYPE_PARAM_LIST,PARAM_LIST)\
	namespace Function{\
	template<typename R TEMPLATE_LIST>\
class ICallback<R(TYPE_LIST)>\
{\
	typedef ICallback<R(TYPE_LIST)> SelfType;\
public: \
	virtual ~ICallback() {}\
	virtual R Invoke(TYPE_LIST) const = 0; \
	virtual bool Equals(const SelfType* pCallback) const = 0; \
	virtual SelfType* Clone() const = 0; \
};\
	template<typename R TEMPLATE_LIST>\
class NativeCallback<R(TYPE_LIST)>\
:public ICallback<R(TYPE_LIST)>\
{\
	typedef ICallback<R(TYPE_LIST)> BaseType;\
	typedef NativeCallback<R(TYPE_LIST)> SelfType;\
	typedef R(*FunctonPtr)(TYPE_LIST);\
	\
private:\
	FunctonPtr m_handler;\
public:\
	explicit NativeCallback(FunctonPtr ptr)\
	:m_handler(ptr)\
{\
}\
	NativeCallback(const SelfType& another)\
	:ICallback<R(TYPE_LIST)>(another)\
	,m_handler(another.m_handler)\
{\
}\
	R Invoke(TYPE_PARAM_LIST) const\
{\
	return (*m_handler)(PARAM_LIST);\
}\
	bool Equals(const BaseType* pCallback) const\
{\
	const SelfType* pNatvieCallback = dynamic_cast<const SelfType*>(pCallback);\
	if(pNatvieCallback == NULL)\
{\
	return false;\
}\
	if(pNatvieCallback->m_handler == this->m_handler)\
{\
	return true;\
}\
	return false;\
}\
	SelfType* Clone() const\
{\
	return new SelfType(*this);\
}\
};\
	\
	template<typename C,typename R TEMPLATE_LIST>\
class MemberCallback<C,R(TYPE_LIST)>\
	:public ICallback<R(TYPE_LIST)>\
{\
	typedef ICallback<R(TYPE_LIST)> BaseType;\
	typedef MemberCallback<C,R(TYPE_LIST)> SelfType;\
	typedef R(C::*FunctionPtr)(TYPE_LIST);\
	typedef R(C::*ConstFunctionPtr)(TYPE_LIST) const;\
	enum{CONST_POINTER,NEED_DELETE,DONT_DELETE};\
	struct ObjectManager\
	{\
		union{C* pObject;const C* pConstObject;} object;\
		int property;\
		int refCount;\
		bool Equals(const ObjectManager& another)\
		{\
			return object.pObject == another.object.pObject;\
		}\
	};\
public:\
	MemberCallback(C* t,FunctionPtr ptr,bool needDelete=false)\
	:m_isConstMemFunc(false)\
{\
	m_pObjectManager = new ObjectManager();\
	m_pObjectManager->object.pObject = t;\
	m_pObjectManager->property = needDelete?NEED_DELETE:DONT_DELETE;\
	m_pObjectManager->refCount = 0;\
	m_handler.ptr = ptr;\
	IncrementRefCount();\
}\
	MemberCallback(C* t,ConstFunctionPtr ptr,bool needDelete = false)\
	:m_isConstMemFunc(true)\
{\
	m_pObjectManager = new ObjectManager();\
	m_pObjectManager->object.pObject = t;\
	m_pObjectManager->property = needDelete ? NEED_DELETE:DONT_DELETE;\
	m_pObjectManager->refCount = 0;\
	m_handler.constPtr = ptr;\
	IncrementRefCount();\
}\
	MemberCallback(const C* t,ConstFunctionPtr ptr)\
	:m_isConstMemFunc(true)\
{\
	m_pObjectManager = new ObjectManager();\
	m_pObjectManager->object.pConstObject = t;\
	m_pObjectManager->property = CONST_POINTER;\
	m_pObjectManager->refCount = 0;\
	m_handler.constPtr = ptr;\
	IncrementRefCount();\
}\
	MemberCallback(const SelfType& another)\
	:ICallback<R(TYPE_LIST)>(another)\
	,m_pObjectManager(another.m_pObjectManager)\
	,m_handler(another.m_handler)\
	,m_isConstMemFunc(another.m_isConstMemFunc)\
{\
	IncrementRefCount();\
}\
	virtual ~MemberCallback()\
{\
	DecrementRefCount();\
}\
	R Invoke(TYPE_PARAM_LIST) const\
{\
	if(m_isConstMemFunc)\
{\
	if(m_pObjectManager->property == CONST_POINTER)\
{\
	return (m_pObjectManager->object.pConstObject->*(m_handler.constPtr))(PARAM_LIST);\
}\
	return (m_pObjectManager->object.pObject->*(m_handler.constPtr))(PARAM_LIST);\
}\
	return (m_pObjectManager->object.pObject->*(m_handler.ptr))(PARAM_LIST);\
}\
	bool Equals(const BaseType* pCallback) const\
{\
	const SelfType*  pMemberCallback = dynamic_cast<const SelfType*>(pCallback);\
	if(pMemberCallback == NULL)\
{\
	return false;\
}\
	if(m_pObjectManager->Equals(*pMemberCallback->m_pObjectManager)\
	&& m_isConstMemFunc == pMemberCallback->m_isConstMemFunc\
	&& m_handler.ptr == pMemberCallback->m_handler.ptr\
	)\
{\
	return true;\
}\
	return false;\
}\
	SelfType* Clone() const\
{\
	return new SelfType(*this);\
}\
private:\
	void IncrementRefCount()\
{\
	++m_pObjectManager->refCount;\
};\
	void DecrementRefCount()\
{\
	if(--m_pObjectManager->refCount == 0)\
{\
	if(m_pObjectManager->property == NEED_DELETE)\
{\
	delete m_pObjectManager->object.pObject;\
}\
	delete m_pObjectManager;\
}\
}\
private:\
	ObjectManager* m_pObjectManager;\
union{FunctionPtr ptr;ConstFunctionPtr constPtr;} m_handler ;\
	bool m_isConstMemFunc;\
};\
}\
	\
	\
template<typename R TEMPLATE_LIST>\
class delegate<R(TYPE_LIST)>\
	:public Function::ICallback<R(TYPE_LIST)>\
{\
	typedef Function::ICallback<R(TYPE_LIST)> BaseType;\
	typedef delegate<R(TYPE_LIST)> SelfType;\
struct CallbackHolder\
{\
	BaseType* instance;\
	CallbackHolder* prev;\
	R Call(TYPE_PARAM_LIST)\
{\
	if(prev!=NULL)\
{\
	prev->Call(PARAM_LIST);\
}\
	if(instance==NULL)\
{\
	return R();\
}\
	return instance->Invoke(PARAM_LIST);\
}\
};\
public:\
	\
	delegate():m_pHolder(NULL)\
{\
	\
}\
	explicit delegate(R(*ptr)(TYPE_LIST))\
	:m_pHolder(NULL)\
{\
	AddHandler(ptr);\
}\
	\
	template<typename C>\
	delegate(const C* t,R(C::*ptr)(TYPE_LIST) const )\
	:m_pHolder(NULL)\
{\
	AddHandler(t,ptr);	\
}\
	template<typename C>\
	delegate(C* t,R(C::*ptr)(TYPE_LIST) ,bool needDelete = false)\
	:m_pHolder(NULL)\
{\
	AddHandler(t,ptr,needDelete);	\
}\
	\
	template<typename C>\
	delegate(C* t,R(C::*ptr)(TYPE_LIST) const,bool needDelete = false)\
	:m_pHolder(NULL)\
{\
	AddHandler(t,ptr,needDelete);	\
}\
	\
	template<typename FunctorType>\
	explicit delegate(const FunctorType* pFunctor)\
	:m_pHolder(NULL)\
{\
	AddHandler(pFunctor);\
}\
	\
	template<typename FunctorType>\
	explicit delegate(FunctorType* pFunctor,bool needDelete = false)\
	:m_pHolder(NULL)\
{\
	AddHandler(pFunctor,needDelete);\
}\
	\
	delegate(const SelfType& another)\
	:ICallback<R(TYPE_LIST)>(another)\
{\
	CopyFrom(another);\
}\
	\
	SelfType& operator=(const SelfType& another)\
{\
	this->Release();\
	CopyFrom(another);\
	return *this;\
}\
	\
	~delegate()\
{\
	Release();\
}\
	void Release()\
{\
	CallbackHolder* ptr = m_pHolder;\
	while(ptr!=NULL)\
{\
	CallbackHolder* prev = ptr->prev;\
	delete ptr->instance;\
	delete ptr;\
	ptr = prev;\
}\
	m_pHolder = NULL;\
}\
	R operator()(TYPE_PARAM_LIST)\
{\
	return this->Invoke(PARAM_LIST);\
}\
	\
	R Invoke(TYPE_PARAM_LIST) const\
{\
	if(m_pHolder == NULL)\
{\
	return R();\
}\
	return m_pHolder->Call(PARAM_LIST);\
}\
	\
	bool Equals(const BaseType* pCallback) const\
	{\
		const SelfType* pDelegate = dynamic_cast<const SelfType*>(pCallback);\
		if(pDelegate == NULL)\
		{\
			return false;\
		}\
		SelfType* temp = NULL;\
		const SelfType* pClone;\
		CloneForComparison(pDelegate,pClone,temp);\
		CallbackHolder* ptr1 = m_pHolder;\
		CallbackHolder* ptr2 = pClone->m_pHolder;\
		while(ptr1!=NULL&&ptr2!=NULL)\
		{\
			if(!ptr1->instance->Equals(ptr2->instance))\
			{\
				break;\
			}\
			ptr1 = ptr1->prev;\
			ptr2 = ptr2->prev;\
		}\
		delete temp;\
		return (ptr1 == NULL && ptr2 == NULL);\
	}\
	\
	SelfType* Clone() const\
	{\
		SelfType* pClone = new SelfType();\
		CallbackHolder* ptr = m_pHolder;\
		CallbackHolder* pReverse = NULL;\
		while(ptr!=NULL)\
		{\
			CallbackHolder* pHolder = new CallbackHolder();\
			pHolder->instance = ptr->instance->Clone();\
			pHolder->prev = pReverse;\
			pReverse = pHolder;\
			ptr = ptr->prev;\
		}\
		CallbackHolder* prev = NULL;\
		while(pReverse!=NULL)\
		{\
			CallbackHolder* next = pReverse->prev;\
			pReverse->prev = prev;\
			prev = pReverse;\
			pReverse = next;\
		}\
		pClone->m_pHolder = prev;\
		return pClone;\
	}\
	\
	void AddHandler(R(*ptr)(TYPE_LIST))\
	{\
		BaseType* pNew = new Function::NativeCallback<R(TYPE_LIST)>(ptr);\
		AddHandlerImpl(pNew);\
	}\
	template<typename C>\
	void AddHandler(const C* t,R(C::*ptr)(TYPE_LIST)  const)\
	{\
		BaseType* pNew = new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr);\
		AddHandlerImpl(pNew);\
	}\
	\
	template<typename C>\
	void AddHandler(C* t,R(C::*ptr)(TYPE_LIST)  ,bool needDelete = false)\
	{\
		BaseType* pNew = new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr,needDelete);\
		AddHandlerImpl(pNew);\
	}\
	template<typename C>\
	void AddHandler(C* t,R(C::*ptr)(TYPE_LIST) const ,bool needDelete = false)\
	{\
		BaseType* pNew = new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr,needDelete);\
		AddHandlerImpl(pNew);\
	}\
	template<typename FunctorType>\
	void AddHandler(FunctorType* pFunctor,bool needDelete = false)\
	{\
		this->AddHandler(pFunctor,&FunctorType::operator(),needDelete);\
	}\
	\
	template<typename FunctorType>\
	void AddHandler(const FunctorType* pFunctor)\
	{\
		this->AddHandler(pFunctor,&FunctorType::operator());\
	}\
	\
	void RemoveHandler(R(*ptr)(TYPE_LIST))\
	{\
		BaseType* pNew = new Function::NativeCallback<R(TYPE_LIST)>(ptr);\
		RemoveHandlerImp(pNew);\
	}\
	template<typename C>\
	void RemoveHandler(const C* t,R(C::*ptr)(TYPE_LIST) const)\
	{\
		BaseType* pNew = new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr);\
		RemoveHandlerImp(pNew);\
	}\
	template<typename C>\
	void RemoveHandler(C* t,R(C::*ptr)(TYPE_LIST) ,bool needDelete = false)\
	{\
		BaseType* pNew = new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr,needDelete);\
		RemoveHandlerImp(pNew);\
	}\
	template<typename C>\
	void RemoveHandler(C* t,R(C::*ptr)(TYPE_LIST) const,bool needDelete = false)\
	{\
		BaseType* pNew = new Function::MemberCallback<C,R(TYPE_LIST)>(t,ptr,needDelete);\
		RemoveHandlerImp(pNew);\
	}\
	template<typename FunctorType>\
	void RemoveHandler(FunctorType* pFunctor,bool needDelete = false)\
	{\
		RemoveHandler(pFunctor,&pFunctor::operator(),needDelete);\
	}\
	template<typename FunctorType>\
	void RemoveHandler(const FunctorType* pFunctor)\
	{\
		RemoveHandler(pFunctor,&pFunctor::operator());\
	}\
	void RemoveHandler(const SelfType& callback)\
	{\
		this->RemoveHandlerImp(&callback);\
	}\
private:\
	void CloneForComparison(const SelfType* pDelegate,const SelfType*& pClone,SelfType*& pDelegateForDelete) const\
	{\
		pClone = pDelegate;\
		pDelegateForDelete = NULL;\
	}\
	\
	void CopyFrom(const SelfType& pDelegate)\
	{\
		SelfType* pClone = pDelegate.Clone();\
		m_pHolder = pClone->m_pHolder;\
		pClone->m_pHolder = NULL;\
		delete pClone;\
	}\
	\
	void AddHandlerImpl(BaseType* pCallback)\
	{\
		CallbackHolder* pHolder = new CallbackHolder();\
		pHolder->instance = pCallback;\
		pHolder->prev = m_pHolder;\
		m_pHolder = pHolder;\
	}\
	\
	void RemoveHandlerImp(const BaseType* pConstCallback)\
	{\
		CallbackHolder* ptr = m_pHolder;\
		CallbackHolder* prev = NULL;\
		while(ptr!=NULL)\
		{\
			if(ptr->instance->Equals(pConstCallback))\
			{\
				if(prev == NULL)\
				{\
					m_pHolder = ptr->prev;\
				}\
				else\
				{\
					prev->prev = ptr->prev;\
				}\
				delete ptr->instance;\
				delete ptr;\
				break;\
			}\
			prev = ptr;\
			ptr = ptr->prev;\
		}\
	}\
	void RemoveHandlerImp(BaseType* pCallback)\
	{\
		const BaseType* pConstCallback = pCallback;\
		RemoveHandlerImp(pConstCallback);\
		delete pCallback;\
	}\
	CallbackHolder* m_pHolder;\
};

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

#define TYPE_PARAM_LIST_0
#define TYPE_PARAM_LIST_1 T0 t0
#define TYPE_PARAM_LIST_2 TYPE_PARAM_LIST_1, T1 t1
#define TYPE_PARAM_LIST_3 TYPE_PARAM_LIST_2, T2 t2
#define TYPE_PARAM_LIST_4 TYPE_PARAM_LIST_3, T3 t3
#define TYPE_PARAM_LIST_5 TYPE_PARAM_LIST_4, T4 t4
#define TYPE_PARAM_LIST_6 TYPE_PARAM_LIST_5, T5 t5
#define TYPE_PARAM_LIST_7 TYPE_PARAM_LIST_6, T6 t6
#define TYPE_PARAM_LIST_8 TYPE_PARAM_LIST_7, T7 t7
#define TYPE_PARAM_LIST_9 TYPE_PARAM_LIST_8, T8 t8
#define TYPE_PARAM_LIST_10 TYPE_PARAM_LIST_9, T9 t9
#define TYPE_PARAM_LIST_11 TYPE_PARAM_LIST_10, T10 t10

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

DELEGATE_GENERATOR(TEMPLATE_LIST_0, TYPE_LIST_0, TYPE_PARAM_LIST_0, PARAM_LIST_0)
DELEGATE_GENERATOR(TEMPLATE_LIST_1, TYPE_LIST_1, TYPE_PARAM_LIST_1, PARAM_LIST_1)
DELEGATE_GENERATOR(TEMPLATE_LIST_2, TYPE_LIST_2, TYPE_PARAM_LIST_2, PARAM_LIST_2)
DELEGATE_GENERATOR(TEMPLATE_LIST_3, TYPE_LIST_3, TYPE_PARAM_LIST_3, PARAM_LIST_3)
DELEGATE_GENERATOR(TEMPLATE_LIST_4, TYPE_LIST_4, TYPE_PARAM_LIST_4, PARAM_LIST_4)
DELEGATE_GENERATOR(TEMPLATE_LIST_5, TYPE_LIST_5, TYPE_PARAM_LIST_5, PARAM_LIST_5)
DELEGATE_GENERATOR(TEMPLATE_LIST_6, TYPE_LIST_6, TYPE_PARAM_LIST_6, PARAM_LIST_6)
DELEGATE_GENERATOR(TEMPLATE_LIST_7, TYPE_LIST_7, TYPE_PARAM_LIST_7, PARAM_LIST_7)
DELEGATE_GENERATOR(TEMPLATE_LIST_8, TYPE_LIST_8, TYPE_PARAM_LIST_8, PARAM_LIST_8)
DELEGATE_GENERATOR(TEMPLATE_LIST_9, TYPE_LIST_9, TYPE_PARAM_LIST_9, PARAM_LIST_9)
DELEGATE_GENERATOR(TEMPLATE_LIST_10, TYPE_LIST_10, TYPE_PARAM_LIST_10, PARAM_LIST_10)
DELEGATE_GENERATOR(TEMPLATE_LIST_11, TYPE_LIST_11, TYPE_PARAM_LIST_11, PARAM_LIST_11)

// remove the macros
#undef DELEGATE_GENERATOR
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