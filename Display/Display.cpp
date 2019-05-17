///////////////////////////////////////////////////////////////////////////
// Display.cpp   : defines webpage display using browser functions       //
// ver 1.0                                                               //
//                                                                       // 
// Application   : Displays converted files                              //
// Platform      : Visual Studio Community 2017, HP intel core i7        //
// Author        : Arjun Kalyanasundaram, Syracuse University            //
//                 akalyana@syr.edu                                      //
//Source         :Ammar Salman                                           //
///////////////////////////////////////////////////////////////////////////

#include "Display.h"
#include "../Process/Process.h"
#include "../Logger/Logger.h"
#include "../FileSystem/FileSystem.h"

using namespace LoggingC;

// -----< default ctor >--------------------------------------------------
Display::Display() { }

// -----< display single file >-------------------------------------------
void Display::display(const std::string & file)
{

  LoggerDebug::write("\n  Displaying file: " + FileSystem::Path::getName(file) + " in browser");
  LoggerDemo::write("\n  Displaying file: " + FileSystem::Path::getName(file) + " in browser");

  CBP callback = []() {
    LoggerDebug::write("\n  --- child browser exited with this message ---");
    LoggerDemo::write("\n  --- child browser exited with this message ---");
  };

  Process p;
  p.application("C:\\Windows\\System32\\cmd.exe");
  std::string cmd = "/C " + FileSystem::Path::getFullFileSpec(file);

  p.commandLine(cmd);
  p.create();
  p.setCallBackProcessing(callback);
  p.registerCallback();
  WaitForSingleObject(p.getProcessHandle(), INFINITE);
}

// -----< display multiple files  >---------------------------------------
void Display::display(const std::vector<std::string>& files)
{
	auto it1 = files.begin();
	auto it2 = files.begin() + 3;    ///displays upto 3 files//////
	while (it1 != it2) {
		display(*it1);
		it1++;
	}

}
	 


#include <iostream>
#ifdef Test_disp
int main() {
	LoggerDebug::attach(&std::cout);
	LoggerDebug::start();

	std::vector<std::string> files;
	files.push_back("..\\ConvertedWebpages\\Converter.h.html");
	files.push_back("..\\ConvertedWebpages\\Converter.cpp.html");

	Display d();
	d.display(files);

	LoggerDebug::stop();
}


#endif // Test_disp




