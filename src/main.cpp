#include <gtest/gtest.h>
#include <vector>
#include <gflags/gflags.h>
#include <glog/logging.h>

#if defined(_WIN32) && defined(_DEBUG) 
#include <crtdbg.h>

namespace testing
{
  class MemoryLeakDetector : public testing::EmptyTestEventListener
  {
  public:
    virtual void OnTestStart(const TestInfo&)
    {
      _CrtMemCheckpoint(&memState_);
    }

    virtual void OnTestEnd(const TestInfo& test_info){
      if(test_info.result()->Passed())
      {
        _CrtMemState stateNow, stateDiff;
        _CrtMemCheckpoint(&stateNow);
        int diffResult = _CrtMemDifference(&stateDiff, &memState_, &stateNow);
        if (diffResult)
        {
          _CrtMemDumpStatistics( &stateDiff );
          FAIL() << "Memory leak of " << stateDiff.lSizes[1] << " byte(s) detected.";
        }
      }
    }

  private:
    _CrtMemState memState_;
  };
}
#endif

int main(int argc, char **argv)
{
#if defined(_WIN32) && defined(_DEBUG)
    // https://stackoverflow.com/questions/2153776/how-to-use-crtdumpmemoryleaks
    // https://codeday.me/bug/20180807/210795.html
    // http://forums.codeguru.com/showthread.php?312742-Visual-C-Debugging-How-to-manage-memory-leaks

    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );
#endif

    testing::InitGoogleTest(&argc, argv);

    gflags::SetVersionString("1.0.0.0");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

#if defined(_WIN32) && defined(_DEBUG) 
    testing::UnitTest::GetInstance()->listeners().Append(new testing::MemoryLeakDetector());
#endif

    return RUN_ALL_TESTS();
}
