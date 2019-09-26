#include <File/File.h>
#include <stdio.h>             
#include <stdarg.h>   




File::File(std::string _pathToFile, File::OpenMode _accessMode)
{
	Open(_pathToFile, _accessMode);  
}


File::File(std::string _pathToFile): pathToFile(_pathToFile)
{}



File::File()
{}


 bool File::IsOpened()const
 {
	 if(file) { return true; }
	 else { return false; } 
 }



File::~File()
{
	_Close(file);   
}



FILE* File::GetFile()const
{
	return file;
}



int File::GetAccessMode()const
{
	if(file) { return accessMode; }
	return 0;
}


std::string File::GetPath()const
{
	return pathToFile; 
}
	

std::string File::GetName()const
{
	return _GetName(pathToFile);
}


std::string File::GetSuffix()const
{
	return _GetSuffix(pathToFile);
}



std::string File::GetBaseName()const
{
	return _GetBaseName(pathToFile);
}


std::string File::GetDir()const
{
	return _GetDir(pathToFile);
}


bool File::Close()
{
	return _Close(file);  
}



bool File::Open(std::string _pathToFile, OpenMode _accessMode)
{
	if(file)    
	{
		if(accessMode == _accessMode && pathToFile == _pathToFile) { return true; } 

		if(!_Close(file))     
		{
			return false;   
		}
	}

	if(file = _Open(_pathToFile, _accessMode))    
	{
		pathToFile = _pathToFile;    
		accessMode = _accessMode;    
		return true;  
	}

	return false;   
}



bool File::Open(OpenMode _accessMode)
{
	if(file)    
	{
		if(accessMode == _accessMode) { return true; } 
		
		if(!_Close(file))     
		{
			return false;   
		}
	}

	if(file = _Open(pathToFile, _accessMode))    
	{
		accessMode = _accessMode;    
		return true;  
	}

	return false;   
}


bool File::IsExist()const
{
	return _IsExist(pathToFile);
}



bool File::Remove()
{
	Close();  

	return _Remove(pathToFile); 
}



bool File::Rename(std::string _newName)
{
	return _Rename(pathToFile, _newName);
}


bool File::Relocate(std::string _newDir, bool _replace)
{
	return _Relocate(pathToFile, _newDir, _replace);
}



__int64 File::GetPos()const
{
	return _GetPos(file);
}


__int64 File::GetSize()const
{
	return _GetSize(file);
}



bool File::SetPos(int _newPos, int _mode)
{
	return _SetPos(file, _newPos, _mode);
}


void File::MoveToNewLine()
{
	_MoveToNewLine(file);
}



int File::IsEmpty()const
{
	return _IsEmpty(file);
}



int File::ReadFile(std::string& _data)const
{
	if(accessMode == READ)
	{
		return _ReadFile(file, _data);
	}
	return 0; 
	
}





int File::WriteFormatString(std::string _string, ...)
{
	if(accessMode == WRITE || accessMode == REWRITE)
	{
		if(file)      
		{
			va_list argList;    
			va_start(argList, _string);                 
			int symbols = vfprintf(file, _string.c_str(), argList);                 
			va_end(argList);   
			return symbols;    
		}
	}

	return -1;   
}





int File::WriteString(std::string _data)
{
	if(accessMode == WRITE || accessMode == REWRITE)
	{
		return _WriteString(file, _data);
	}
	return 0;
}
 


int File::ReadWord(std::string& _data)const
{
	if(accessMode == READ)
	{ 
		return _ReadWord(file, _data);
	}
	return 0;
}


int File::ReadString(std::string& _data, const int _size)const
{
	if(accessMode == READ)
	{
		_ReadString(file, _data, _size); 
	}
		
	return 0;   
} 

