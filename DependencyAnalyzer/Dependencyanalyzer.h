/////////////////////////////////////////////////////////////////////
//  DependencyAnalyzer.h										   //
//  ver 1.0														   //
//  Language:     Visual C++ 2017                                 //
//  Platform:     HP intel core i7                                 //
//  Application:  Used to create extract and create dependecies    //
//  Author:		  Arjun Kalyanasundaram	Syracuse University	       //
//				  akalyana@syr.edu								   //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines DependencyAnalyzer class which is used to create 
*  dependecies.It extracts the dependencies from the files and then adds
*  them to the table.It consists of two fucntions extract and create dependencies.
*  Function extract is used to find header file within quotes("") and then returns
*  a string which is used sent to the create dependecy function.The function uses 
*  uses regex to match files with #include and then the corresponding dependencies 
*  for each file are added into a vector.
*  Required Files:
* =======================
*  -
* Build commands 
* - devenv  Codepublisher.sln
*  Maintainence History:
* =======================
*  ver 1.0 - 4 Mar 2019
*  - first release
*/


#include<vector>
using namespace std;
class DependencyAnalyzer
{

public:
	std::string extract(const std::string& str, char beg, char end);  ///// function to extract header files////
	std::vector<string> CreateDependency(std::string &file);          /// function to create Dependency///////
};

