/////////////////////////////////////////////////////////////////////
//  Converter.cpp										           //
//  ver 1.0														    //
//  Language:     Visual C++ 2017								   //
//  Platform:     HP intel core i7                                 //
//  Application:  Converts source code into webpage with features  //
//  Author:		  Arjun Kalyanasundaram	Syracuse University	       //
//				  akalyana@syr.edu								   //
/////////////////////////////////////////////////////////////////////
#include "Converter.h"
#include"../tree/tree.h"
#include "../Logger/Logger.h"
#include"../CppParser/Parser/Parser.h"
#include "../FileSystem/FileSystem.h"
#include"../CppParser/Tokenizer/Toker.h"
#include"../DependencyTable/DependencyTable.h"
#include"../DependencyAnalyzer/Dependencyanalyzer.h"

using namespace FileSystem;
using namespace LoggingC;


// -----< default ctor >--------------------------------------------------
CodeConverter::CodeConverter()
{
}

// -----< ctor to set dep table >-----------------------------------------
CodeConverter::CodeConverter(const DependencyTable & dt) : dt_(dt)
{
	LoggerDebug::write("\n  CodeConverter initialized with DependencyTable\n");
}

// -----< set dependency table function >---------------------------------
void CodeConverter::setDepTable(const DependencyTable & dt)
{
	dt_ = dt;
}

// -----< convert pre-set dependency table >------------------------------
std::vector<std::string> CodeConverter::convert()
{
	if (!createOutpurDir()) {
		LoggerDebug::write("\n\n  Failed to start conversion due to inability to create output directory");
		LoggerDebug::write("\n  -- Output Directory: " + FileSystem::Path::getFullFileSpec(outputDir_));
		return std::vector<std::string>();
	}

	LoggerDemo::write("\n\n  Converting files to webpages");
	LoggerDebug::write("\n\n  Converting files to webpages");

	LoggerDebug::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
	LoggerDemo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));

	for (auto entry : dt_) {
		if (convertFile(entry.first)) {
			LoggerDemo::write("\n  -- Converted: ");
			LoggerDebug::write("\n  -- Converted: ");
		}
		else {
			LoggerDemo::write("\n  -- Failed:    ");
			LoggerDebug::write("\n  -- Failed:    ");
		}

		std::string filename = Path::getName(entry.first);
		LoggerDebug::write(filename);
		LoggerDemo::write(filename);
	}
	LoggerDebug::write("\n");
	LoggerDemo::write("\n");
	return convertedFiles_;
}

// -----< convert single file given path >----------------------------------
std::string CodeConverter::convert(const std::string & filepath)
{
	if (!createOutpurDir()) {
		LoggerDebug::write("\n\n  Failed to start conversion due to inability to create output directory");
		LoggerDebug::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
		return "";
	}

	LoggerDemo::write("\n\n  Converting files to webpages");
	LoggerDebug::write("\n\n  Converting files to webpages");

	LoggerDebug::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
	LoggerDemo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));

	if (convertFile(filepath)) {
		LoggerDemo::write("\n  -- Converted: ");
		LoggerDebug::write("\n  -- Converted: ");
	}
	else {
		LoggerDemo::write("\n  -- Failed:    ");
		LoggerDebug::write("\n  -- Failed:    ");
	}

	std::string filename = Path::getName(filepath);
	LoggerDebug::write(filename);
	LoggerDemo::write(filename);

	return filename + ".html";
}

std::vector<std::string> CodeConverter::convert(const std::vector<std::string>& files)
{
	clear();
	dt_ = DependencyTable(files);

	return convert();
}

// -----< get set output directory >----------------------------------------
const std::string & CodeConverter::outputDir() const
{
	return outputDir_;
}

// -----< set output directory >--------------------------------------------
void CodeConverter::outputDir(const std::string & dir)
{
	outputDir_ = dir;
}

// -----< create output directory >-----------------------------------------

bool CodeConverter::createOutpurDir()
{
	if (!Directory::exists(outputDir_)) {
		LoggerDebug::write("\n  Creating output directory. Path: " + Path::getFullFileSpec(outputDir_));
		return Directory::create(outputDir_);
	}
	LoggerDebug::write("\n  Output directory already exists. Proceeding...");
	return true;
}

// -----< get list of converted files >-------------------------------------
const std::vector<std::string> CodeConverter::convertedFiles() const
{
	return convertedFiles_;
}

void CodeConverter::clear()
{
	convertedFiles_.clear();
	dt_.clear();
}


// -----< private - read file and create webpage >--------------------------
bool CodeConverter::convertFile(std::string file)
{
	std::ifstream in(file);
	if (!in.is_open() || !in.good()) {
		LoggerDebug::write("Error -- unable to read file, path may be invalid.");
		in.close();
		return false;
	}
	std::vector<string> dep_vec;
	std::string filename = Path::getName(file);
	std::string dependency = Path::getName(file);
	std::string outputPath = outputDir_ + filename + ".html";
	out_.open(outputPath, std::ofstream::out);
	if (!out_.is_open() || !out_.good()) {
		LoggerDebug::write("Error -- unable to open output file for writing.");
		in.close();
		return false;
	}
	DependencyAnalyzer dk;
	dep_vec = dk.CreateDependency(file);
	for (auto dep_str : dep_vec)
	{
		dt_.addDependency(file, dep_str);
	}
	addPreCodeHTML(filename);
	addDependencyLinks(file);
	Tree t;
	std::unordered_map<size_t, std::pair<std::string, bool>> linedetail = t.function(file);    /////using unordered map/////
	auto iter = linedetail.begin();  
		size_t count1 = 0;
		addPreTag();
		while (in.good()) {
			std::string line;
			while (std::getline(in, line)) {
				count1++;
				skipSpecialChars(line);
				handleComments(line);       ///function to handle single line comments/////
				handleCommentsMulti(line);   /////function to handle multliline comments////
				handleFunctionsandclasses(line, linedetail, count1);   ///function to handle functions and classes///
				out_ << line << std::endl;
			}
		}
	addClosingTags();
	out_.close();
	convertedFiles_.push_back(outputPath);
	return true;
}

// -----< private - add generic HTML preliminary markup >-------------------
void CodeConverter::addPreCodeHTML(const std::string& title)
{
	out_ << "<DOCTYPE !HTML>" << std::endl;
	out_ << "<html>" << std::endl;
	out_ << "  <head>" << std::endl;
	out_ << "    <Title>" << title << "</Title>" << std::endl;
	out_ << "    <style>" << std::endl;
	out_ << "      body {" << std::endl;
	out_ << "        padding:15px 40px;" << std::endl;
	out_ << "        font-family: Consolas;" << std::endl;
	out_ << "        font-size: 1.25em;" << std::endl;
	out_ << "        font-weight: normal;" << std::endl;

	out_ << "      }" << std::endl;

	out_ << "      </style>" << std::endl;
	out_ << "<script src= \"script.js\" ></script>" << std::endl;
	out_ << "  </head>" << std::endl << std::endl;
	out_ << "  <body>" << std::endl;
	out_ << "<button onclick = togglevisibility('comments')>" << " Comments" << "</button>" << std::endl;
	out_ << "<button onclick = togglevisibility('function')>" << " Functions" << "</button>" << std::endl;
	out_ << "<button onclick = togglevisibility('class')>" << " Classes" << "</button>" << std::endl;
}





// -----< private - add pre tag >------------------------------------------
/* seperated into seperate function to allow for dependencies addition
*  before the actual code of the file */
void CodeConverter::addPreTag()
{
	out_ << "    <pre>" << std::endl;
}


// -----< private - add depedency links markup code >----------------------
void CodeConverter::addDependencyLinks(std::string file)
{

	std::string filename = Path::getName(file);
	if (!dt_.has(file)) { // in case of single file conversion
		LoggerDebug::write("\n    No entry found in DependencyTable for [" + filename + "]. Skipping dependency links..");
		return;
	}

	if (dt_[file].size() == 0) { // in case the file has no dependencies
		LoggerDebug::write("\n    No dependencies found for [" + filename + "]. Skipping dependency links..");
		return;
	}

	out_ << "    <h3>Dependencies: " << std::endl;
	for (auto dep : dt_[file]) {
		out_ << "      <a href=\"" << Path::getName(dep) << ".html\">" << Path::getName(dep) << "</a>" << std::endl;
	}
	out_ << "    </h3>";
}




// -----< private - add generic HTML markup closing tags >-----------------
void CodeConverter::addClosingTags()
{
	out_ << "    </pre>" << std::endl;
	out_ << "  </body>" << std::endl;
	out_ << "</html>" << std::endl;
}

// -----< private - replace HTML special chars >---------------------------
/* note: the function uses while loop to make sure ALL special characters
*  are replaced instead of just the first encounter. */
void CodeConverter::skipSpecialChars(std::string & line)
{
	size_t pos = line.npos;
	while ((pos = line.find('<')) != line.npos)
		line.replace(pos, 1, "&lt;");

	while ((pos = line.find('>')) != line.npos)
		line.replace(pos, 1, "&gt;");
}


// -----<private -To add <div> tags for functions and classes to hide >---------------------------
void CodeConverter::handleFunctionsandclasses(std::string &line, std::unordered_map<size_t, std::pair<std::string, bool>> linedetail,size_t ret_count)
{
	int pos_check = linedetail.count(ret_count);    ////checks from the returned unordered map/////
	std::pair<std::string, bool> condition1("function", true);    ////checks conditions///
	std::pair<std::string, bool> condition2("function", false);
	std::pair<std::string, bool> condition3("class", true);
	std::pair<std::string, bool> condition4("class", false);
	std::unordered_map<size_t, std::pair<std::string, bool>>::const_iterator Func_classtype; //////creating an iterator for the unordered map////

	switch(pos_check)
	{
	case 1:
	{
		Func_classtype = linedetail.find(ret_count);
		   if (Func_classtype->second == condition1)
			 {
			   line = "<div class =\"function\">" + line;   ////line to open div tags for functions/////

			}
		   if (Func_classtype->second == condition3)
			{
			
			   line = "<div class =\"class\">" + line;/////replacing line to open div tags for classes///
			}

		   if (Func_classtype->second == condition2 || Func_classtype->second == condition4)
		   {
			   line.replace(line.size(), 1, "</div>");      ///replacing line to add closing div tags////
		   }
		}
	default:
		break;
	}
}

// -----<private -Function to handle single line comments >---------------------------
void CodeConverter::handleComments(std::string &line)
{
	size_t pos = line.find('/');
	if (pos == line.npos) return;
	if (pos == line.size() - 1) return;
	if (line[pos + 1] == '/') {
		line.replace(pos, 1, "<div class =\"comments\">/");
		line += "</div>";
	}
}

// -----<private -Function to handle Multi line comments >---------------------------
void CodeConverter::handleCommentsMulti(std::string &line)
{
	CodeConverter c;
	if (c.isMultipleLineComment1(line) == true)
	{
		line = "<div class =\"comments\">" + line;
	}

	if (c.isMultipleLineComment2(line) == true)
	{
		line += "</div>";
	}
}
// -----<private -Function to check opening of comments >---------------------------
bool CodeConverter::isMultipleLineComment1(string tok)
{
	if (tok.size() < 2)
		return false;
	if (tok[0] == '/' && tok[1] == '*' ) //////Muliline comments opening////
		return true;
	return false;
}

// -----<private -Function to check closing of comments >---------------------------
bool CodeConverter::isMultipleLineComment2(string tok)
{
	if (tok.size() < 2)
		return false;
	if (tok[0] == '*' && tok[1] == '/')//////Muliline comments closing////
		return true;
	return false;
}


// -----< test stub for converter class >-----------------------------------
#ifdef Test_convert1
int main() {
	LoggerDebug::attach(&std::cout);
	LoggerDebug::start();

	LoggerDebug::title("Testing CodeConverter functions");

	LoggerDebug::stop(); // do not log DependencyTable logs

	DependencyTable dt;
	dt.addDependency("Converter.h", "Converter.cpp");
	dt.addDependency("Converter.cpp", "Converter.h");
	dt.addDependency("A.h", "A.h");

	LoggerDebug::start(); // log from now on

	CodeConverter cc(dt);
	dt.display();
	LoggerDebug::write("\n");
	//cc.outputDir("C:\\windows\\system32\\"); // will trigger errors which are handled
	cc.convert();

	LoggerDebug::stop();
	return 0;
}

#endif //


