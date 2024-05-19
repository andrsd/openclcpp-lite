#include "gmock/gmock.h"
#include "openclcpp-lite/openclcpp-lite.h"

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
        std::cerr << "  extensions: " << exts << std::endl;
        std::cerr << "  profile: " << profile << std::endl;
    }
}
