#pragma once
#include <typeinfo>
#include <map>
#include "util.h"
#include "Any.h"
using namespace  std;

class CBag
{
private:
	map<CString,any*> m_Values;
public:
	CBag(){m_Values.clear();};

	CBag(const CBag& that)
	{
		for(map<CString,any*>::iterator iterObj = m_Values.begin();iterObj!=m_Values.end();iterObj++)
		{
			if( iterObj->second!=NULL)
			{
				delete iterObj->second;
			}
		}
		m_Values.clear();
		for(map<CString,any*>::const_iterator iterObj = that.m_Values.begin();iterObj!=that.m_Values.end();iterObj++)
		{
			m_Values[iterObj->first]  = new any(*(iterObj->second));
		}
	}
	
	CBag& operator =(const CBag& that)
	{
		for(map<CString,any*>::iterator iterObj = m_Values.begin();iterObj!=m_Values.end();iterObj++)
		{
			if( iterObj->second!=NULL)
			{
				delete iterObj->second;
			}
		}
		m_Values.clear();
		for(map<CString,any*>::const_iterator iterObj = that.m_Values.begin();iterObj!=that.m_Values.end();iterObj++)
		{
			m_Values[iterObj->first]  = new any(*(iterObj->second));
		}
		return *this;
	}

	virtual ~CBag()
	{
		for(map<CString,any*>::iterator iterObj = m_Values.begin();iterObj!=m_Values.end();iterObj++)
		{
			if( iterObj->second!=NULL)
			{
				delete iterObj->second;
			}
		}
		m_Values.clear();
	};

	template<typename T>
	void SetValue(CString key,T& t)
	{
		any* valueObj = m_Values[key];
		if(valueObj!=NULL)
		{
			delete valueObj;
			valueObj = NULL;
		}
		valueObj = new any(t);
		m_Values[key] = valueObj;
	}
	void RemoveValue(CString key)
	{
		map<CString,any*>::iterator iterObj =  m_Values.find(key);
		if(iterObj!=m_Values.end())
		{
			if(iterObj->second)
			{
				delete iterObj->second ;
			}
			m_Values.erase(iterObj);
		}
	};

	template<typename T>
	bool GetValue(CString key,T &t)
	{
		map<CString,any*>::const_iterator iterObj =  m_Values.find(key);
		if(iterObj==m_Values.end())
		{
			return false;
		}
		if(iterObj->second->type() ==typeid(T))
		{
				t =    any_cast<T>(*(iterObj->second));
				return true;
		}
		return false;
	}
};