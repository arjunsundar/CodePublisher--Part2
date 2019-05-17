/////////////////////////////////////////////////////////////////////
//  DependencyAnalyzer.cpp										   //
//  ver 1.0														   //
//  Language:      Visual C++ 2017                                 //
//  Platform:     HP intel core i7                                 //
//  Application:  Used to create extract and create dependecies    //
//  Author:		  Arjun Kalyanasundaram, Syracuse University	    //
//				  akalyana@syr.edu								   //
/////////////////////////////////////////////////////////////////////
#include<fstream>
#include<iostream>
#include<regex>
#include<string>
#include<algorithm>
#include"Dependencyanalyzer.h"
#include "../Logger/Logger.h"
using namespace LoggingC;
using namespace std;


//----< extracts file within starting and ending character " ">-----

std::string DependencyAnalyzer::extract(const std::string& str_n, char start, char finish) 
{
	std::size_t StartPos;
	if ((StartPos = str_n.find(start)) != std::string::npos)
	{
		std::size_t FinishPos;
		if ((FinishPos = str_n.find(finish, StartPos)) != std::string::npos && FinishPos != StartPos + 1)  ///conditions to extract header files////
			return str_n.substr(StartPos + 1, FinishPos - StartPos - 1);          
	}
	return std::string(); ////---<returns a string after extraction>/////
}

//----< Creates a dependency after header file is extracted>-----
std::vector<string> DependencyAnalyzer::CreateDependency(std::string &file)
{
	std::ifstream in(file);               //////////using ifstream to read file path//////////
	std::vector<std::string> filecontent; /////////vector to store filecontent///////
	std::regex reg1("#include");          //////// regex statement to match/////////
	std::string depend_value;
	std::string depn;
	const char c = ';';
	const char q = '"';
	for (std::string line; std::getline(in, line);)
	{
		if (std::regex_search(line, reg1))    //regex//
		{
			if (line.back() == c)            
			{
				break;
			}
			if (!line.empty()) {
				line.pop_back();
			}
			depend_value = extract(line, '"', '"');   /////function returns the header file as dependency/////
			if (depend_value.size() > 0)
			{
				filecontent.push_back(depend_value);  /////storing dependency values into a vector/////
			}
		}
	}
	return  filecontent;     ////returns a vector with dependencies///////
}




#include <iostream>
// ----< test stub for dependency table package >---------------------------


#ifdef Test_DependecyAnalyz
int main()
{
	LoggerDebug::attach(&std::cout);
	LoggerDebug::start();

	LoggerDebug::title("Testing DependencyAnalyzer Operations");

	DependencyAnalyzer da;
	std::vector<std::string> files;
	files.push_back("..\\Converter\\Converter.cpp");
	for (auto file : files)
	{
		da.CreateDependency(file);

	}
	LoggerDebug::stop();

	return 0;
}


#endif // Test_DependecyAnalyz
