#include <gtest/gtest.h>
#include <glog/logging.h>

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: How To Use Google Logging Library (glog)
// Web:   http://rpg.ifi.uzh.ch/docs/glog.html
//        https://blog.csdn.net/u012348774/article/details/80558533
/////////////////////////////////////////////////////////////////////////////// 

TEST(glog, simple)
{
    // --logtostderr 1 will outputs the INFO and WARNING as well

    LOG(INFO) << "This is INFO";
    LOG(WARNING) << "This is WARNING";
    LOG(ERROR) << "This is Error";
}
