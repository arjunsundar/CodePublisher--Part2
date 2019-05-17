////////////////////////////////////////////////////////////////////////////
//  Converter.h										                      //
//  ver 1.0														          //
//  Language:      Visual C++ 2017                                        //
//  Platform:     HP intel core i7                                        //
//  Application:  Used to convert source code into webpages with features //
//  Author:		  Arjun Kalyanasundaram,	Syracuse University	           //
//				  akalyana@syr.edu								          //
////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
* Creates files for being Converted into HTML. Files created include:The HTML tags.
*Files are made into vector of strings and then converted.The special characters
*'<' and '>' are rplaced with their HTMl equivalent.In addition the converter 
* allows to add dependencies from the dependecy table after it has been created.
*Functions have been provided to handle comments both single line and multi line 
*and handle functions and classes.<div> tags are added in the place of comments,
*functions and classes to hide them using buttons.
*  Required Files:
* =======================
*  -DependencyTable.h, 
* Build commands
* - devenv  Codepublisher.sln
*  Maintainence History:
* =======================
*  ver 1.0 - 4 Mar 2019
*  - first release
*/

#include "../DependencyTable/DependencyTable.h"
#include <vector>
#include <string>
#include <fstream>

class CodeConverter
{
public:
	CodeConverter();
	CodeConverter(const DependencyTable& dt);

	const std::string& outputDir() const;
	void outputDir(const std::string& dir);
	bool createOutpurDir();

	void setDepTable(const DependencyTable& dt);

	std::vector<std::string> convert();
	std::string convert(const std::string& filepath);
	std::vector<std::string> convert(const std::vector<std::string>& files);

	const std::vector<std::string> convertedFiles() const;

	void clear();

private:
	bool convertFile(std::string file);
	void addPreCodeHTML(const std::string& title);
	void addPreTag();
	void addDependencyLinks(std::string file);
	void addClosingTags();
	void skipSpecialChars(std::string& line);
	void handleComments(std::string& line);
	static bool isMultipleLineComment1(std::string tok);
	static bool isMultipleLineComment2(std::string  tok);
	void handleFunctionsandclasses(std::string &line, std::unordered_map<size_t, std::pair<std::string, bool>> linedetail, size_t count);
	void handleCommentsMulti(std::string &line);
private:
	DependencyTable dt_;
	std::string outputDir_ = "..\\ConvertedWebpages\\";
	std::vector<std::string> convertedFiles_;
	std::ofstream out_;
};

