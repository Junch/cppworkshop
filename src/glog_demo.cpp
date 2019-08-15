#include <glog/logging.h>
#include <gtest/gtest.h>

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: How To Use Google Logging Library (glog)
// Web:   http://rpg.ifi.uzh.ch/docs/glog.html
//        https://blog.csdn.net/u012348774/article/details/80558533
///////////////////////////////////////////////////////////////////////////////

TEST(glog, simple)
{
    // --logtostderr 1 will outputs the INFO and WARNING as well
    // On Mac the command should be executed as administrator or specify the log file location by --log_dir
    // or else the log is not created under /tmp
    //   sudo ./MainTest --gtest_filter=glog.simple

    LOG(INFO) << "This is INFO";
    LOG(WARNING) << "This is WARNING";
    LOG(ERROR) << "This is Error";
}
