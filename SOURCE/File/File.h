#pragma once

#include <string>      


class File
{
    public: enum OpenMode
	{
		READ  = 1,   
		REWRITE = 2,  
		WRITE = 3   
	};


	public: enum 
	{
		ORIGIN = SEEK_SET,     
		CURRENT = SEEK_CUR,     
		END = SEEK_END     
	};

	private: enum
	{
		WORD_SIZE = 128   
	};

    private: FILE* file = NULL;  
    private: std::string pathToFile;     
    private: int accessMode = 0;      
    

    public: File(std::string _pathToFile, OpenMode _accessMode);


	public: File(std::string _pathToFile);


	public: File();


    public: virtual ~File();


	public: std::string GetName()const;


	public: static std::string _GetName(std::string _pathToFile);


	public: std::string GetBaseName()const;


	public: static std::string _GetBaseName(std::string _pathToFile);


	public: std::string GetSuffix()const;


	public: static std::string _GetSuffix(std::string _pathToFile);


	public: std::string GetDir()const;


	public: static std::string _GetDir(std::string _pathToFile);

	
	public: FILE* GetFile()const;


	public: std::string GetPath()const;


	public: int GetAccessMode()const;

	public: static bool _CreateFile(std::string _pathToFile);


    public: bool Close();


    public: static bool _Close(FILE* _file);


    public: bool Open(std::string _pathToFile, OpenMode _accessMode = WRITE);
    
 
    public: bool Open(OpenMode _accessMode = WRITE);
    

    public: static FILE* _Open(std::string _pathToFile, OpenMode _accessMode = WRITE); 
 

	public: bool IsOpened()const;


	public: bool IsExist()const;


	public: static bool _IsExist(std::string _pathToFile);


	public: static bool _Remove(std::string _pathToFile); 


	public: bool Remove();


	public: static bool _Rename(std::string _pathToFile, std::string _newName);
 
			
	public: bool Rename(std::string _newName);


    public: bool Relocate(std::string _newDir, bool _replace = false); 


    public: static bool _Relocate(std::string _pathToFile, std::string _newDir, bool _replace = false);


    public: static bool _CopyFile(std::string _pathToFile, std::string _targetPath, bool _rewrite = false);


    public: static bool _CreateFolder(std::string _path);


    public: static bool _RemoveFolder(std::string _path);


	public: static int _IsEmpty(FILE* _file);
 

	public: int IsEmpty()const;


	public: static __int64 _GetSize(FILE* _file);

		
	public: __int64 GetSize()const;


	public: static __int64 _GetPos(FILE* _file);
	

	public: __int64 GetPos()const;


    public: static bool _SetPos(FILE* _file, int _newPos, int _mode = ORIGIN);
  

	public: bool SetPos(int _newPos, int _mode = ORIGIN);


	public: static void _MoveToNewLine(FILE* _file);


	public: void MoveToNewLine();


	public: static int _WriteString(FILE* _file, std::string _data);


	public: int WriteString(std::string _data);


	public: static int _ReadString(FILE* _file, std::string& _data, int _size); 
	

	public: int ReadString(std::string& _data, int _size)const;
	

	public: static int _ReadWord(FILE* _file, std::string& _data);


	public: int ReadWord(std::string& _data)const;


	public: static int _ReadFile(FILE* _file, std::string& _data);
		
			
	public: int ReadFile(std::string& _data)const;


	public: int WriteFormatString(std::string _string, ...);        


	public: template<typename Arg> int ReadFormatString(std::string _string, Arg& _arg)const
	{
		if(file)       
		{
			if(feof(file))                 
			{ 
				return -1;   
			}

			return fscanf(file, _string.c_str(), &_arg);                        
		}
		return 0;   
	}
		

	public: template<typename Arg1, typename Arg2> int ReadFormatString(std::string _string, Arg1& _arg1, Arg2& _arg2)const
	{
		if(file)       
		{
			if(feof(file))                 
			{ 
				return -1;   
			}

			return fscanf(file, _string.c_str(), &_arg1, &_arg2);                        
		}
		return 0;   
	}

				
	public: template<typename Arg1, typename Arg2, typename Arg3> int ReadFormatString(std::string _string, Arg1& _arg1, Arg2& _arg2, Arg3& _arg3)const
	{
		if(file)       
		{
			if(feof(file))                 
			{ 
				return -1;   
			}

			return fscanf(file, _string.c_str(), &_arg1, &_arg2, &_arg3);                        
		}
		return 0;   
	}

				
	public: template<typename Arg1, typename Arg2, typename Arg3, typename Arg4> int ReadFormatString(std::string _string, Arg1& _arg1, Arg2& _arg2, Arg3& _arg3, Arg4& _arg4)const
	{
		if(file)       
		{
			if(feof(file))                 
			{ 
				return -1;   
			}

			return fscanf(file, _string.c_str(), &_arg1, &_arg2, &_arg3, &_arg4);                        
		}
		return 0;   
	}


	public: template<typename Type> int Write(Type _data)
	{
		if(file && accessMode == WRITE || accessMode == REWRITE)           
		{
			return fwrite(&_data, sizeof(Type), 1, file);                              
		}

		return 0;  
	}


	public: template<typename Type> int Read(Type& _data)const
	{
		if(file && accessMode == READ)            
		{
			if(feof(file))              
			{ 
				return -1;      
			}

			return fread(&_data, sizeof(Type), 1, file);                                 
		}

		return 0;   
	}

	
	public: template<typename Type> int Write(Type& _data, int _size)
	{
		if(file && accessMode == WRITE || accessMode == REWRITE)           
		{
			if(_size > 0)     
			{
				return fwrite(&_data, sizeof(Type), _size, file);                           
			}
		}
		return 0;  
	}


	public: template<typename Type> int Read(Type& _data, int _size)const
	{
		if(file && accessMode == READ)            
		{
			if(_size > 0)     
			{
				if(feof(file))              
				{ 
					return -1;      
				}

				return fread(&_data, sizeof(Type), _size, file);                             
			}
		}
		return 0;   
	}


	public: static bool _GetIntKey(std::string _path, bool _relative, std::string _section, std::string _key, int& _result);


	public: static bool _GetFloatKey(std::string _path, bool _relative, std::string _section, std::string _key, float& _result);


	public: static bool _GetStringKey(std::string _path, bool _relative, std::string _section, std::string _key, std::string& _result);


	public: static bool _GetBoolKey(std::string _path, bool _relative, std::string _section, std::string _key, bool& _result);


	public: static bool _SetIntKey(std::string _path, bool _relative, std::string _section, std::string _key, int _value);


	public: static bool _SetFloatKey(std::string _path, bool _relative, std::string _section, std::string _key, float _value);


	public: static bool _SetStringKey(std::string _path, bool _relative, std::string _section, std::string _key, std::string _value);


	public: static bool _SetBoolKey(std::string _path, bool _relative, std::string _section, std::string _key, bool _value);
};




