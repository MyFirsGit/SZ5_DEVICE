#include "Xfile.h"
#pragma once


//////////////////////////////////////////////////////////////////////////
/*
@brief   文件与内存结构对象转换工具类   

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
	@brief     将结构对象序列化到文件中 

	@param CString fileName 保存的文件名
	@param T& t             需要序列化的结构化对象
	@param S				对t序列化为二进制对象的辅助对象 该类需要实现两个方法
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
	@brief  将结构对象序列化到文件中    

	@param CString fileName 保存的文件名
	@param T& t             需要序列化的结构化对象，该类需要实现两个方法
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
	@brief   将文件的内容反序列化为结构化对象   

	@param   CString fileName 文件名
	@param   T& t			  结构化内存对象
	@param	 S				  反序列化的辅助类，该类需要实现方法
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
	@brief   将文件的内容反序化为结构化对象   

	@param   CString fileName 文件名
	@param   T& t			  结构化内存对象，该类需要实现方法
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
	@brief   将文件的内容反序列化为结构化对象   

	@param   CString fileName 文件名
	@param   T& t			  结构化内存对象
	@param	 S				  反序列化的辅助类，该类需要实现方法
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
	@brief   将文件的内容反序化为结构化对象   

	@param   CString fileName 文件名
	@param   T& t			  结构化内存对象，该类需要实现方法
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