/////////////////////////////////////////////////////////////////////
//  Executive.cpp												   //
//  ver 1.0														   //
//  Language:      Visual C++ 2017                                 //
//  Platform:     HP intel core i7                                 //														   //
//  Author:		  Arjun Kalyanasundaram	Syracuse University	       //
//				  akalyana@syr.edu								   //
/////////////////////////////////////////////////////////////////////

#include "Executive.h"
#include "../Logger/Logger.h"

using namespace Utilities;
using namespace LoggingC;
using namespace FileSystem;

ProcessCmdLine::Usage customUsage();

CodePublisher::CodePublisher()
{
}

// -----< process cmd to get info >------------------------------------
bool CodePublisher::processCommandLineArgs(int argc, char ** argv)
{
	pcl_ = new ProcessCmdLine(argc, argv);
	pcl_->usage(customUsage());
	//preface("Command Line: ");
	//pcl_->showCmdLine();
	if (pcl_->parseError())
	{
		pcl_->usage();
		std::cout << "\n\n";
		return false;
	}
	dirIn_ = pcl_->path();
	return true;
}

// -----< return input directory - from PCL >----------------------------
const std::string & CodePublisher::workingDirectory() const
{
	return dirIn_;
}

// -----< set output directory >-----------------------------------------
void CodePublisher::outputDirectory(const std::string & dir)
{
	dirOut_ = dir;
	cconv_.outputDir(dir);
}

// -----< return output directory >--------------------------------------
const std::string & CodePublisher::outputDirectory() const
{
	return dirOut_;
}

// -----< extract files - after processing cmd >-------------------------
bool CodePublisher::extractFiles()
{
	DirExplorerN de(pcl_->path());

	for (auto patt : pcl_->patterns())
	{
		de.addPattern(patt);
	}

	if (pcl_->hasOption("s"))
	{
		de.recurse();
	}

	bool res = de.search() && de.match_regexes(pcl_->regexes());
	de.showStats();

	files_ = de.filesList();

	return res;
}


// -----< publish - files extracted from directory explorer >---------------
void CodePublisher::publish()
{
	display_.display(cconv_.convert(files_));
}

// -----< publish - single file >-------------------------------------------
void CodePublisher::publish(const std::string & file)
{
	display_.display(cconv_.convert(file));
}

// -----< publish - must provide list >-------------------------------------


void CodePublisher::publish(const std::vector<std::string>& files)
{
	display_.display(cconv_.convert(files));
}

// -----< gets display mode as set by PCL >---------------------------------


Utilities::DisplayMode CodePublisher::displayMode() const
{
	return pcl_->displayMode();
}


// -----< command line usage >----------------------------------------------


ProcessCmdLine::Usage customUsage()
{
	std::string usage;
	usage += "\n  Command Line: path [/option]* [/pattern]* [/regex]*";
	usage += "\n    path is relative or absolute path where processing begins";
	usage += "\n    [/option]* are one or more options of the form:";
	usage += "\n      /s     - walk directory recursively";
	usage += "\n      /demo  - run in demonstration mode (cannot coexist with /debug)";
	usage += "\n      /debug - run in debug mode (cannot coexist with /demo)";
	//usage += "\n      /h - hide empty directories";
	//usage += "\n      /a - on stopping, show all files in current directory";
	usage += "\n    [pattern]* are one or more pattern strings of the form:";
	usage += "\n      *.h *.cpp *.cs *.txt or *.*";
	usage += "\n    [regex] regular expression(s), i.e. [A-B](.*)";
	usage += "\n";
	return usage;
}

// -----< initialize loggers in proper display mode >------------------------


void initializeLoggers(DisplayMode dm) {
	if (dm == DisplayMode::Debug) {
		LoggerDebug::attach(&std::cout);
		LoggerDebug::start();
	}

	else if (dm == DisplayMode::Demo) {
		LoggerDemo::attach(&std::cout);
		LoggerDemo::start();
	}
}

// -----< stop loggers >-----------------------------------------------------

void stopLoggers() {
	LoggerDemo::stop();
	LoggerDebug::stop();
}

// -----< passing arguments to start publishing >-----------------------------------------------------
int demoReqn(CodePublisher& cp, int argc, char ** argv) {

if (argc < 2) return 1; // even before processing, if no path, just reject it
char * tmp = new char[sizeof(argv[1])];
strcpy_s(tmp, sizeof(tmp), argv[1]);
argv[1] = new char[9];
delete argv[1];
argv[1] = tmp;
if (!cp.processCommandLineArgs(argc, argv)) return 1;
if (!cp.extractFiles()) { 
return 2; 
}

return 0;
}


#ifdef DEMO_CP

#include <iostream>
// -----< demonstrating requirement 1 >----------------------------------------------
void demoreq1()
{

	LoggerDemo::title("----------<Demonstrating requirement 1>--------------\n");
	LoggerDemo::write("\n");
	LoggerDemo::write("Using  Visual Studio 2017 and its C++ Windows Console Projects \n");


}

// --------------------< demonstrating requirement 2 >----------------------------------------------
void demoreq2()
{
	LoggerDemo::write("\n");
	LoggerDemo::title("----------<Demonstrating requirement 2>--------------\n");
	LoggerDemo::write("Using standard streams libraries for all I/O and operators new and delete for memory management.\n");
	LoggerDemo::write("Code Fragments:\n");
	LoggerDemo::write("std::ifstream in(file);\n");
	LoggerDemo::write("std::vector<std::string> filecontent;\n");
	LoggerDemo::write("if (!line.empty()) { \n");
	LoggerDemo::write("line.pop_back();\n");
	LoggerDemo::write("}");
}

// -----< demonstrating  requirement 3 >----------------------------------------------
void demoReq3() {
	LoggerDemo::write("\n");
	LoggerDemo::title("-----<Demonstrating requirement 3>--------\n");
	LoggerDemo::write("Provided an Executive package that accepts, from its command line, a path containing files to be converted \n");
	LoggerDemo::write("\n");
	LoggerDemo::write("Command line Arguments: \n");
	LoggerDemo::write(".. /s /demo *.h *.cpp [A-Z](.*) \n");
}


// -----< demonstrating requirements 4>---------------------------------------


void demoreq4() {
	LoggerDemo::write("\n");
	LoggerDemo::title("-----<Demonstrating requirement 4>-------\n");
	LoggerDemo::write("Using the following packages: \n");
	LoggerDemo::write("1.Loader\n");
	LoggerDemo::write("2.Converter\n");
	LoggerDemo::write("3.Dependency\n");
	LoggerDemo::write("4.Display \n");
}

// -----< demonstrating requirement 5 >------------------------------


void demoReq5() {
	LoggerDemo::write("\n");
	LoggerDemo::title("----------<Demonstrating requirement 5>--------------\n");
	LoggerDemo::write("Provided a loader package that finds and loads files as specified by the Executive");

}


// -----< demonstrating requirement 6 >-------------------------------------

void demoReq6() {
	LoggerDemo::write("\n");
	LoggerDemo::title("----------<Demonstrating requirement 6>--------------\n");
	LoggerDemo::write("The converter package supports wrapping code in an html document, and translates  the < and > characters into their corresponding escape sequences \n");
	LoggerDemo::write("Code fragments:\n");
	LoggerDemo::write("size_t pos = line.npos;)\n");
	LoggerDemo::write("while ((pos = line.find('<')) != line.npos)\n");
	LoggerDemo::write("line.replace(pos, 1, \"&lt;\")\n");
	LoggerDemo::write("while ((pos = line.find('>')) != line.npos)\n");
	LoggerDemo::write("line.replace(pos, 1, \"&gt;\")\n");

}




// -----< demonstrating requirements 7 & 8>-------------------------------------
//------<displays dependencies through dependency package provides links to dependencies>--------
//------<buttons to hide comments,classes and functions by div tags>--------
void demoReq78(CodePublisher& cp) {
	LoggerDemo::write("\n");
	LoggerDemo::title("----------<Demonstrating requiremens 7 & 8>--------------\n");
	cp.publish();

}

// -----< demonstrating requirement 9>-------------------------------------
void demoReq9()
{
	LoggerDemo::write("\n");
	LoggerDemo::title("----------<Demonstrating requirement 9>--------------\n");
	LoggerDemo::write("Automated test demonstrates all Requirements \n");

}

// -----< DEMONSTRATION MAIN >---------------------------------------------//


int main(int argc, char ** argv) {
	// the following processing of cmd is not official, it's only
	// done to allow for demo/debug modes BEFORE CodePublisher does
	// its own processing of cmds..

	initializeLoggers(Utilities::DisplayMode::Debug); // by default go debug
	for (int i = 0; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "/demo") { // only go demo once told 
			stopLoggers();
			initializeLoggers(Utilities::DisplayMode::Demo);
			break;
		}
	}
	CodePublisher cp;
	demoreq1();                 ////demonstrating requirment 1 //////
	demoreq2();					////demonstrating requirment 2 //////
	demoReq3();					////demonstrating requirment 3 //////
	demoreq4();					////demonstrating requirment 4 //////
	demoReq5();					////demonstrating requirment 5 //////
	demoReqn(cp, argc, argv);
	demoReq6();					////demonstrating requirment 6 //////
	demoReq78(cp);             ////demonstrating requirment 7 & 8 //////
	demoReq9();               ////demonstrating requirment 9//////
	stopLoggers();
	return 0;
}

#endif

#ifdef USE_CP

#include <iostream>

// -----< NORMAL USE MAIN > ------------------------------------------
int main(int argc, char ** argv) {
	CodePublisher cp;
	if (!cp.processCommandLineArgs(argc, argv)) {
		std::cout << "\n  Failed to process command line arguments. Terminating\n\n";
		return 1;
	}

	initializeLoggers(cp.displayMode());

	if (!cp.extractFiles()) {
		stopLoggers();
		std::cout << "\n  Found no files to process. Terminating.\n\n";
		return 0;
	}

	cp.publish();
	stopLoggers();
	return 0;
}

#endif