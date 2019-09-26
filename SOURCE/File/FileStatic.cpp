#include <File/File.h>
#include <stdio.h>             
#include <io.h>        
#include <stdarg.h>   
#include <windows.h>  

bool File::_IsExist(std::string _pathToFile)
{
	if(_access(_pathToFile.c_str(), 0) == 0)             	
	{ 
		return true;   
	} 
	
	return false;    
}



bool File::_CreateFile(std::string _pathToFile)
{ 
	if(_IsExist(_pathToFile))    
	{
		return true;   
	}
	else     
	{
		FILE* file = _Open(_pathToFile, File::REWRITE);        

		if(file)       
		{
			_Close(file);  
			return true;  
		}
	}

	return false;   
}


bool File::_Close(FILE* _file)
{
	if(_file)    
	{
		if(fclose(_file) == 0)                         
		{ 
			return true;   
		}
	}
	return false; 
}


bool File::_CreateFolder(std::string _path)
{ 
	if(_IsExist(_path)) { return false; } 
	return (bool)CreateDirectoryA(_path.c_str(), NULL);
}


bool File::_RemoveFolder(std::string _path)
{
	return (bool)RemoveDirectoryA(_path.c_str());
}

bool File::_CopyFile(std::string _pathToFile, std::string _targetPath, bool _rewrite)
{
	if(_IsExist(_pathToFile))    
	{
		return CopyFileA(_pathToFile.c_str(),    
		                 _targetPath.c_str(),      
					    (BOOL)_rewrite);                       
	}
	return false;
}


FILE* File::_Open(std::string _pathToFile, OpenMode _accessMode)
{
	if(_pathToFile.empty()) { return 0; }       

	switch(_accessMode)
	{
		case READ: 
		{
			FILE* file;
			if (fopen_s(&file, _pathToFile.c_str(), "rb") == 0)                  
			{
				return file;
			}
			break;
		}

		case WRITE:
		{
			FILE* file;
			if (fopen_s(&file, _pathToFile.c_str(), "r+b") == 0)                      
			{
				_SetPos(file, 0, END);      
				return file;
			}
			break;
		}

		case REWRITE:
		{
			FILE* file;
			if(fopen_s(&file, _pathToFile.c_str(), "wb") == 0)                   
			{
				return file;
			}
			break;
		}
	}

	return NULL;   
}




bool File::_Remove(std::string _pathToFile)
{
	if(_IsExist(_pathToFile))      
	{
		if(remove(_pathToFile.c_str()) == 0)          
		{
			return true;   
		}
	}
	return false;   
}





bool File::_Rename(std::string _pathToFile, std::string _newName)
{
	if(_IsExist(_pathToFile))    
	{
		_newName = _GetName(_newName);   

		_newName = _GetDir(_pathToFile) + _newName;            

		if(!_IsExist(_newName))   
		{
			if(rename(_pathToFile.c_str(), _newName.c_str()) == 0)         
			{
				return true;  
			}
		}
	}

	return false;   
}




bool File::_Relocate(std::string _pathToFile, std::string _newDir, bool _replace)
{
	if(_IsExist(_pathToFile))    
	{
		std::string pathToNewFile = _GetDir(_newDir) + _GetName(_pathToFile);       
			
		if(pathToNewFile == _pathToFile)   
		{
			return false;
		}
		if(_replace && _IsExist(pathToNewFile))          
		{
			_Remove(pathToNewFile);    
		}
				
		if(rename(_pathToFile.c_str(), pathToNewFile.c_str()) == 0)                    
		{
			return true;  
		}
	}

	return false;   
}



std::string File::_GetSuffix(std::string _pathToFile)
{
	int index = _pathToFile.find_last_of(".");                             
		
	if(index != -1)          
	{
		return _pathToFile.substr(index + 1, _pathToFile.length() - 1);                 
	}

	return "";     
}


std::string File::_GetBaseName(std::string _pathToFile)
{
	int index = _pathToFile.find_last_of("/\\");                                
		
	if(index != -1)            
	{
		_pathToFile = _pathToFile.substr(index + 1);               
	}

	index = _pathToFile.find_last_of(".");                          
	
	if(index != -1)     
	{
		_pathToFile.erase(index, _pathToFile.length() - index);         
	}
	return _pathToFile;   
}



std::string File::_GetName(std::string _pathToFile)
{
	int index = _pathToFile.find_last_of("/\\");                                
		
	if(index != -1)            
	{
		return _pathToFile.substr(index + 1);               
	}

	return _pathToFile;   
}



std::string File::_GetDir(std::string _pathToFile)
{
	int index = _pathToFile.find_last_of("/\\");                                
		
	if(index != -1)            
	{
		return _pathToFile.substr(0, index + 1);                    
	}

	return "";      
}



__int64 File::_GetPos(FILE* _file)
{
	if(_file)       
	{
		__int64 pos;

		if(fgetpos(_file, &pos) == 0)          
		{
			return pos;      
		}
	}
	return -1;    
}


void File::_MoveToNewLine(FILE* _file)
{
	if(_file)       
	{
		char string[WORD_SIZE];    
		
		while(fgets(string, sizeof(string) - 1, _file))
		{
			if(string[strlen(string) - 1] == '\n')     
			{
				break;  
			}	
		}
	}
}




__int64 File::_GetSize(FILE* _file)
{
	if(_file)       
	{
		__int64 pos = _GetPos(_file);      

		if(fseek(_file, 0, SEEK_END) == 0)                                         
		{
			__int64 size = _GetPos(_file);            
			fseek(_file, pos, SEEK_SET);     
			return size;       
		}
	}

	return -1;    
}




int File::_ReadString(FILE* _file, std::string& _data, int _size)
{
	if(_file)       
	{
		if(feof(_file))              
		{ 
			return -1;      
		}

		char* buffer = new char[_size + 1];                 

		int symbols = fread(buffer, sizeof(char), _size, _file);                                
		
		buffer[_size] = '\0';     
		
		_data = buffer;     

		delete []buffer;   

		return symbols;  
	}
	return 0;
}
	


bool File::_SetPos(FILE* _file, int _newPos, int _mode)
{
	if(_file)       
	{
		if(fseek(_file, _newPos, _mode) == 0)                
		{
			return true;  
		}
	}

	return false;  
}

	

int File::_IsEmpty(FILE* _file)
{
	if(_file)       
	{
		if(_GetSize(_file) == 0)     
		{
			return 1;    
		}
		return 0; 
	}

	return -1;    
}


int File::_ReadFile(FILE* _file, std::string&_data)
{
	if(_file)       
	{
		if(feof(_file))             
		{ 
			return -1;      
		}

		char symbol;      
		int symbols = 0;     

		while(fscanf_s(_file, "%c", &symbol, 1) != EOF)                         
		{ 
			_data += symbol;     
			++symbols;      
		}

		return symbols;     
	}

	return 0;   
}




int File::_WriteString(FILE* _file, std::string _data)
{
	if(_file)        
	{ 
		int result = fprintf(_file, "%s", _data.c_str());                  
			
		if(result > 0) 
		{
			return result;  
		}
	}

	return 0;  
}




int File::_ReadWord(FILE* _file, std::string& _data)
{
	if(_file)       
	{
		if(feof(_file))                 
		{ 
			return -1;   
		}

		char string[WORD_SIZE] = { 0 };    

		if(fscanf_s(_file, "%s", string, _countof(string)) > EOF)                        
		{ 
			_data = string;     
		}

		return _data.length();    
	}

	return 0;   
}




bool File::_GetIntKey(std::string _path, bool _relative, std::string _section, std::string _key, int& _result)
{
	char buffer[256];

	if(_relative) { _path = ".\\" + _path; }

	if(GetPrivateProfileStringA(_section.c_str(), _key.c_str(), "", buffer, sizeof(buffer), _path.c_str()))       
	{
		_result = atoi(buffer);         
		return true;
	}
	return false;
}


bool File::_GetFloatKey(std::string _path, bool _relative, std::string _section, std::string _key, float& _result)
{
	char buffer[256];
	
	if(_relative) { _path = ".\\" + _path; }

	if(GetPrivateProfileStringA(_section.c_str(), _key.c_str(), "", buffer, sizeof(buffer), _path.c_str()))       
	{ 
		_result = (float)atof(buffer);         
		return true;
	}
	return false;
}


bool File::_GetStringKey(std::string _path, bool _relative, std::string _section, std::string _key, std::string& _result)
{
	char buffer[256];

	if(_relative) { _path = ".\\" + _path; }

	if(GetPrivateProfileStringA(_section.c_str(), _key.c_str(), "", buffer, sizeof(buffer), _path.c_str()))       
	{
		_result = buffer;
		return true;
	}
	return false;
}



bool File::_GetBoolKey(std::string _path, bool _relative, std::string _section, std::string _key, bool& _result)
{
	char buffer[8];

	if(_relative) { _path = ".\\" + _path; }

	if(GetPrivateProfileStringA(_section.c_str(), _key.c_str(), "", buffer, sizeof(buffer), _path.c_str()))       
	{
		if(std::string("true") == buffer)
		{
			_result = true;
			return true;
		}
		else if(std::string("false") == buffer)
		{
			_result = false;
			return true;
		}
	}
	return false;
}


bool File::_SetIntKey(std::string _path, bool _relative, std::string _section, std::string _key, int _value)
{
	char buffer[256];

	sprintf_s(buffer, "%d", _value);      

	if(_relative) { _path = ".\\" + _path; }

	if(WritePrivateProfileStringA(_section.c_str(), _key.c_str(), buffer, _path.c_str()))
	{
		return true;
	}
	return false;
}


bool File::_SetFloatKey(std::string _path, bool _relative, std::string _section, std::string _key, float _value)
{
	char buffer[256];

	sprintf_s(buffer, "%f", _value);      

	if(_relative) { _path = ".\\" + _path; }

	if(WritePrivateProfileStringA(_section.c_str(), _key.c_str(), buffer, _path.c_str()))
	{
		return true;
	}
	return false;
}


bool File::_SetStringKey(std::string _path, bool _relative, std::string _section, std::string _key, std::string _value)
{
	char buffer[256];

	sprintf_s(buffer, "%s", _value.c_str());      

	if(_relative) { _path = ".\\" + _path; }

	if(WritePrivateProfileStringA(_section.c_str(), _key.c_str(), buffer, _path.c_str()))
	{
		return true;
	}
	return false;
}


bool File::_SetBoolKey(std::string _path, bool _relative, std::string _section, std::string _key, bool _value)
{
	char trueString[] = "true";
	char falseString[] = "false";

	if(_relative) { _path = ".\\" + _path; }

	if(_value)
	{
		if(WritePrivateProfileStringA(_section.c_str(), _key.c_str(), trueString, _path.c_str()))
		{
			return true;
		}
	}
	else
	{
		if(WritePrivateProfileStringA(_section.c_str(), _key.c_str(), falseString, _path.c_str()))
		{
			return true;
		}
	}
	return false;
}