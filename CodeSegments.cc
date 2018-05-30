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
