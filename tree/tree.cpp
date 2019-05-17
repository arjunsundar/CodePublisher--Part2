/////////////////////////////////////////////////////////////////////
//  tree.cpp													   //
//  ver 1.0														   //
//  Language:     Visual C++ 2017                                  //
//  Platform:     HP intel core i7                                 //
//  Application:  Used to  parse functions and classes		       //
//  Author:		  Arjun Kalyanasundaram	Syracuse University	       //
//				  akalyana@syr.edu								   //
/////////////////////////////////////////////////////////////////////
#include<iostream>
#include"tree.h"
#include"../FileSystem/FileSystem.h"
#include "../CppParser/Tokenizer/Toker.h"
#include "../CppParser/SemiExpression/Semi.h"
#include "../CppParser/Parser/ActionsAndRules.h"
#include "../CppParser/Parser/ConfigureParser.h"
#include"../CppParser/ScopeStack/ScopeStack.h" 
#include"../CppParser/Parser/Parser.h"
using namespace CodeAnalysis;

//----< returns information after parsing>-----
std::unordered_map<size_t, std::pair<std::string, bool>> Tree::function(std::string& file)
{
	std::unordered_map<size_t, std::pair<std::string, bool>> linedetail;      ////creating unordered map///
	ConfigParseForCodeAnal configure;
	Parser* pParser = configure.Build();
	std::string name;
	if (pParser)
	{
		name = FileSystem::Path::getName(file);    /////gets the name of the file////
		if (!configure.Attach(file))
		{
			std::cout << "\n  could not open file " << name << std::endl;
			
		}
	}
	Repository* pRepo = Repository::getInstance();
	pRepo->package() = name;
	while (pParser->next())
	{
		pParser->parse();
	}
	std::cout << "\n";
	ASTNode* pGlobalScope = pRepo->getGlobalScope();
	complexityEval(pGlobalScope);
	linedetail = check_type( linedetail, pGlobalScope);
	return linedetail;    ////returns map after treewalk///

}




