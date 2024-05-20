#pragma once

#include <string>
#include <iostream>
#include <fstream> 
#include "Tools_.h"



class ThyFileReader
{
//	|-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-|
//				    Members and Fields
//	|-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-|

	//	-------------------------------------------
	//					Variables
	//	-------------------------------------------
private:
	std::string m_FileName = std::string("script.rds");
	std::string m_ReadString = std::string();





//	|-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-|
//			        Functions and Methods
//	|-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-|

	//	-------------------------------------------
	//			Constructors and Destructor
	//	-------------------------------------------
public:
	ThyFileReader()
	{

	}
	~ThyFileReader()
	{
		this->Dispose();
	}

	//	-------------------------------------------
	//				Loading Functions
	//	-------------------------------------------
private:
	inline void Dispose()
	{
		
	}

	//	-------------------------------------------
	//			        Functions
	//	-------------------------------------------
public:
	inline void ReadFile()
	{
		std::fstream file(this->m_FileName);
		std::string line;
		while (std::getline(file, line))
		{
			this->m_ReadString += (line + "\n");
		}

		file.close();
	}

	//	-------------------------------------------
	//				Setters and Getters
	//	-------------------------------------------
	inline std::string& GetString()
	{
		return this->m_ReadString;
	}
};