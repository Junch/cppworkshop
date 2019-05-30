#include <gflags/gflags.h>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include <vector>

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    gflags::SetVersionString("1.0.0.0");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    return RUN_ALL_TESTS();
}
