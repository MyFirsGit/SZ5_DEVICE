#include "Xfile.h"
#pragma once


//////////////////////////////////////////////////////////////////////////
/*
@brief   �ļ����ڴ�ṹ����ת��������   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
class CORE_API CXFileSerializer
{
public:

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief     ���ṹ�������л����ļ��� 

	@param CString fileName ������ļ���
	@param T& t             ��Ҫ���л��Ľṹ������
	@param S				��t���л�Ϊ�����ƶ���ĸ������� ������Ҫʵ����������
							1.  static ULONG GetLength(T& t);
							2.	static void Serialize(T& t,LPBYTE buffer);

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	template<typename T, typename S>
	static void Serialize(CString fileName, T& t)
	{
		CXFile file(fileName);
		VARIABLE_DATA buffer;
		buffer.nLen = S::GetLength(t);
		buffer.lpData = new BYTE[buffer.nLen];
		S::Serialize(t,buffer.lpData);
		file.WriteBinFile((char*)buffer.lpData,buffer.nLen);
	};
	
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief  ���ṹ�������л����ļ���    

	@param CString fileName ������ļ���
	@param T& t             ��Ҫ���л��Ľṹ�����󣬸�����Ҫʵ����������
							1.	ULONG GetLength();
							2.	void Serialize(LPBYTE buffer);

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static void Serialize(CString fileName,T& t)
	{
		CXFile file(fileName);
		VARIABLE_DATA buffer;
		buffer.nLen = t.GetLength();
		buffer.lpData = new BYTE[buffer.nLen];
		t.Serialize(buffer.lpData);
		file.WriteBinFile((char*)buffer.lpData,buffer.nLen);
	};

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief   ���ļ������ݷ����л�Ϊ�ṹ������   

	@param   CString fileName �ļ���
	@param   T& t			  �ṹ���ڴ����
	@param	 S				  �����л��ĸ����࣬������Ҫʵ�ַ���
							  static void Deserialize(T&,VARIABLE_DATA&)

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	template<typename T, typename S>
	static void Deserialize(CString fileName,T& t)
	{
		CXFile file(fileName);
		VARIABLE_DATA buffer;
		file.ReadBinFile(buffer);
		S::Deserialize(t,buffer);
	};
	
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief   ���ļ������ݷ���Ϊ�ṹ������   

	@param   CString fileName �ļ���
	@param   T& t			  �ṹ���ڴ���󣬸�����Ҫʵ�ַ���
							  void Deserialize(VARIABLE_DATA&)

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static void Deserialize(CString fileName,T& t)
	{
		CXFile file(fileName);
		VARIABLE_DATA buffer;
		file.ReadBinFile(buffer);
		t.Deserialize(buffer);
	};

		//////////////////////////////////////////////////////////////////////////
	/*
	@brief   ���ļ������ݷ����л�Ϊ�ṹ������   

	@param   CString fileName �ļ���
	@param   T& t			  �ṹ���ڴ����
	@param	 S				  �����л��ĸ����࣬������Ҫʵ�ַ���
							  static void Deserialize(T&,VARIABLE_DATA&)

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	template<typename T, typename S>
	static bool Deserialize(CString fileName,T& t,bool hasSum/* = false*/)
	{
		CXFile file(fileName);
		VARIABLE_DATA buffer;
		bool readSuccess = file.ReadBinFile(buffer,hasSum) == CXFile::FILE_OPERATION_OK;
		return readSuccess && S::Deserialize(t,buffer);
	};
	
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief   ���ļ������ݷ���Ϊ�ṹ������   

	@param   CString fileName �ļ���
	@param   T& t			  �ṹ���ڴ���󣬸�����Ҫʵ�ַ���
							  void Deserialize(VARIABLE_DATA&)

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	static bool Deserialize(CString fileName,T& t,bool hasSum/* = false*/)
	{
		CXFile file(fileName);
		VARIABLE_DATA buffer;
		bool readSuccess = file.ReadBinFile(buffer,hasSum) == CXFile::FILE_OPERATION_OK;
		return readSuccess && t.Deserialize(buffer);
	};
};