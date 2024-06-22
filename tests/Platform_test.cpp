#include "gmock/gmock.h"
#include "openclcpp-lite/platform.h"

namespace ocl = openclcpp_lite;

TEST(PlatformTest, get_platforms)
{
    auto platforms = ocl::get_platforms();
    for (auto & plat : platforms) {
        auto name = plat.name();
        auto vendor = plat.vendor();
        auto version = plat.version();
        auto exts = plat.extensions();
        auto profile = plat.profile();
        std::cerr << "- name: " << name << std::endl;
        std::cerr << "  vendor: " << vendor << std::endl;
        std::cerr << "  version: " << version << std::endl;
        std::cerr << "  extensions: " << std::endl;
        for (auto & e : exts)
            std::cerr << "    - " << e << std::endl;
        std::cerr << "  profile: " << profile << std::endl;
    }
}

TEST(PlatformTest, devices)
{
    auto platforms = ocl::get_platforms();
    for (auto & plat : platforms) {
        auto devs = plat.devices();
        EXPECT_GT(devs.size(), 0);
    }
}
