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
    // clang-format off
    std::map<ocl::Device::Type, std::string> type_str = {
        { ocl::Device::CPU, "CPU" },
        { ocl::Device::GPU, "GPU" },
        { ocl::Device::ACCELERATOR, "Accelerator" },
        { ocl::Device::CUSTOM, "Custom" }
    };
    // clang-format on

    auto platforms = ocl::get_platforms();
    for (auto & plat : platforms) {
        auto devs = plat.devices();
        std::cerr << "devices:" << std::endl;
        for (auto & d : devs) {
            std::cerr << "  - name: " << d.name() << std::endl;
            std::cerr << "    type: " << type_str[d.type()] << std::endl;
            std::cerr << "    vendor: " << d.vendor() << std::endl;
            std::cerr << "    version: " << d.version() << std::endl;
            std::cerr << "    driver version: " << d.driver_version() << std::endl;
            std::cerr << "    address bits: " << d.address_bits() << std::endl;
        }
    }
}
