/*************************************************************************
*
*              Author: b51
*                Mail: b51live@gmail.com
*            FileName: CodeSegment.cc
*
*          Created On: 三  5/30 10:49:12 2018
*     Licensed under The MIT License [see LICENSE for details]
*
************************************************************************/

#include <iostream>
#include <fstream>
#include <map>
#include <glog/logging.h>

/**
 * FunctionLog for FUNCTION_SCOPE usage
 */
class FunctionLog
{
private:
  /**
   * FUNCTION_SCOPE will log out "FunctionName Enter" when function enter
   * and log out "FunctionName Eixt" when function exit.
   *
   * This method used shared ptr characteristic.
   * see http://www.cplusplus.com/reference/memory/shared_ptr/~shared_ptr/
   */
  #define FUNCTION_SCOPE()  auto __scope_logger__ = CreateScopeLogger(__FUNCTION__)

  std::shared_ptr<char> CreateScopeLogger(const char* functionName)
  {
    char* pText = const_cast<char*>(functionName);
    LOG(INFO) << pText << " Enter";
    return std::shared_ptr<char>(pText, [this](char* p){ LOG(INFO) << p << " Exit"; });
  }
public:
  /**
   * Eg: This will log out
   *
   *   "TestFunction Enter"
   *   "TestFunction Execute"
   *   "TestFunction Exit"
   */
  void TestFunction()
  {
    FUNCTION_SCOPE();
    LOG(INFO) << "TestFunction Execute.";
  }
}

/**
 * Usage of ...
 *
 * In function, use ##__VA_ARGS__ as ...
 */
#define DLOG(fmt, ...) \
        do { if (DEBUG) printf(fmt, ##__VA_ARGS__); } while(0)

/**********************************************************************
 **********************************************************************/
/**
 * Get class name and combine string in macro
 */
#define GET_NAME(name) \
  virtual const char* GetClassName() const { LOG(INFO) << #name; return #name; } \
  virtual void GetNameCompose() const ( LOG(INFO) << NameCompose_##name; )

const int NameCompose_ClassA   = 1;
const int NameCompose_ClassB   = 2;
const int NameCompose_ClassC   = 3;
const int NameCompose_ClassD   = 4;

/**
 * Usage:
 *    ClassA class_a;
 *    class_a.GetClassName() // logout "ClassA"
 *    class_a.GetNameCompose() // logout "1", for NameCompose_ClassA = 1
 */
class ClassA()
{
public:
  GET_NAME(ClassA);  // This method will add function GetClassName and GetNameCompose to ClassA
public:
  ClassA() {};
  ~ClassA() {};
}
/**********************************************************************
 **********************************************************************/

/**
 *  Reading configurations from file
 *  eg. Read from nicp_eth_laser.conf
 */
std::cout << "-- Read Configurations --" << std::endl;
std::ifstream is(argv[1]);
std::map<std::string, float> input_parameters;
if (!is)
{
  std::cerr << "Impossible to open configuration file: " << argv[1] << std::endl;
  return -1;
}
while (is.good())
{
  char buf[1024];
  is.getline(buf, 1024);
  std::istringstream iss(buf);

  std::string parameter;
  float value;
  if (!(iss >> parameter >> value)) continue;
  if (parameter[0] == '#') continue;
  input_parameters.insert(std::pair<std::string, float>(parameter, value));
}
for (auto it = input_parameters.begin(); it != input_parameters.end(); it++)
  std::cout << it->first << ": " << it->second << std::endl;
return 0;
/*********************************************************************/
