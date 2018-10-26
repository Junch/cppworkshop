#include <gtest/gtest.h>
#include <vector>

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
    testing::InitGoogleTest(&argc, argv);
#if defined(_WIN32) && defined(_DEBUG) 
    testing::UnitTest::GetInstance()->listeners().Append(new testing::MemoryLeakDetector());
#endif

    return RUN_ALL_TESTS();
}
