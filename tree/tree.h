///////////////////////////////////////////////////////////////////////
//  tree.h														      //
//  ver 1.0															  //
//  Language:      Visual C++ 2017									  //
//  Platform:     HP intel core i7									  //
//  Application:  Used to  parse and find functions and class details //
//  Author:		  Arjun Kalyanasundaram	Syracuse University	          //
//				  akalyana@syr.edu								      //
////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
* This package defines Tree  class which is used to parse 
* through the directory and find the functions and classes.It returns the 
* start line count and endline count in  an unordered map.
* The unordered map holds the information of the function and classes 
* after parsing.
*  Required Files:
* =======================
*  -Parser.h Toker.h -Semi.h -ActionsandRules.h -ConfigureParser.h -ScopeStack.h
* Build commands
* - devenv  Codepublisher.sln
*  Maintainence History:
* =======================
*  ver 1.0 - 4 Mar 2019
*  - first release
*/
#include<stdio.h>
#include <unordered_map>

using namespace std;
class Tree
{
public:
	std::unordered_map<size_t, std::pair<std::string, bool>> function(std::string &file);
};

//----< function that creates an unordered map finds only classes and functions>-----
template<typename element>
std::unordered_map <size_t, std::pair<std::string, bool>> check_type(std::unordered_map<size_t, std::pair<std::string, bool>>& linedetail, element* pItem, bool details = false)
{
	static std::string path;
	static size_t indentLevel = 0;
	auto item_check = pItem->children_.begin();
	++indentLevel;
	while (item_check != pItem->children_.end())
	{
		if ((*item_check)->type_ == "function")
		{
			linedetail[(*item_check)->startLineCount_] = std::pair<std::string, bool>("function", true);  ////returns the start line when condition is true for functions////
			linedetail[(*item_check)->endLineCount_] = std::pair<std::string, bool>("function", false);   ////returns the end line when condition is false for functions////
		}
		if ((*item_check)->type_ == "class")
		{
			linedetail[(*item_check)->startLineCount_] = std::pair<std::string, bool>("class", true); ////returns the start line when condition is true for classes////
			linedetail[(*item_check)->endLineCount_] = std::pair<std::string, bool>("class", false); ////returns the end line when condition is false  for classes////
		}
		check_type(linedetail, *item_check);   ////iters through treewalk 
		++item_check;
	}
	--indentLevel;
	return linedetail;    ///returns an unordered map containing the string and boolean type/////
}

