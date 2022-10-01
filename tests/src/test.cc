#include <gtest/gtest.h>
#include <include/tap.h>
#include <include/pk_buff.h>

TEST(TapDevTESTS, TapDevAllocTest)
{
  EXPECT_GT(tapdev::_TAPD()->get_file_desc(), 0);
}

TEST(TapDevTESTS, TapDevReadTest)
{
  tapdev::package_buff_t package{};
  EXPECT_GT(tapdev::_TAPD()->read_device(package.payload, MTU), 0);
}

// ****** TODO : More Tests ******