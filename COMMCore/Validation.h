#pragma once

class CValidation
{
public:
	
	template<int N,typename T>
	static bool IsInRange(T value,T (&ranges)[N])
	{
		if(ranges == NULL)
		{
			return false;
		}
		for(int i=0;i<N;i++)
		{
			if(value == ranges[i])
			{
				return true;
			}
		}
		return false;
	};

	template<typename T>
	static bool IsInRange(T& value,T ranges[],UINT length)
	{
		if(ranges == NULL)
		{
			return false;
		}
		for(int i=0;i<length;i++)
		{
			if(value == ranges[i])
			{
				return true;
			}
		}
		return false;
	}
};