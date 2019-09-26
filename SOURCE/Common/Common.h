#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <list>
#include <sstream>

namespace Common
{
    static int _GetNextPowerOf2(int _value)
	{
		int nextPowerOf2, i = 0;

		if((_value & (_value - 1)) != 0) 
		{
			while((_value >> (++i)) > 0 && (nextPowerOf2 = int(1) << (i + 1)))      
			{}
		}
		else       
		{
			nextPowerOf2 = _value;
		}
		return nextPowerOf2;
	}

	static std::list<std::string> SplitString(std::string _text, std::string _separators)
	{
		std::list<std::string> tokens;  

		if (_text.length() > 0 && _separators.length() > 0)       
		{
			int bufferSize = _text.length() + 1;          
			char* buffer = new char[bufferSize];     
			strcpy_s(buffer, bufferSize, _text.c_str());    

			char *nextToken = NULL;       
			char *token = strtok_s(buffer, _separators.c_str(), &nextToken);   

			while (token)        
			{
				tokens.push_back(token);     
				token = strtok_s(NULL, _separators.c_str(), &nextToken);    
			}
			delete[] buffer;
		}
		return tokens;
	}
};


#endif
