#pragma once
/////////////////////////////////////////////////////////////////////
//  Executive.h									                   //
//  ver 1.0														   //
//  Language:      Visual C++ 2017                                 //
//  Platform:     HP intel core i7                                 //														  
//  Author:		  Arjun Kalyanasundaram	Syracuse University	       //
//				  akalyana@syr.edu								   //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines Executive which uses different components of
*  CodeConverter, Display and DirectoryN to put together the entire 
*  functionality of the CodePublisher. The mentioned packages are indep-
*  ndent of each other and their only interaction is through this facade.
*
*  Required Files:
* =======================
*   CodeUtilities.h
*  Converter.h Converter.cpp DirExplorerN.h DirExplorerN.cpp
*  Display.h Display.cpp Logger.h Logger.cpp 
*
*  Maintainence History:
* =======================
*  ver 1.0 - 14 Feb 2019
*  - first release
*/

#include <string>
#include <vector>

#include "../Converter/Converter.h"
#include "../Display/Display.h"
#include "../Loader/Loader.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"

class CodePublisher
{
public:
  CodePublisher();
  bool processCommandLineArgs(int argc, char ** argv);
  
  const std::string& workingDirectory() const;

  void outputDirectory(const std::string& dir);
  const std::string& outputDirectory() const; 

  bool extractFiles();

  void publish();
  void publish(const std::string& file);
  void publish(const std::vector<std::string>& files);

  Utilities::DisplayMode displayMode() const;

private:
  Utilities::ProcessCmdLine *pcl_;
  CodeConverter cconv_;
  Display display_;

  std::string dirIn_; 
  std::string dirOut_;

  std::vector<std::string> files_;
};

