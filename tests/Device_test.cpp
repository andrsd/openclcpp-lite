#include "gmock/gmock.h"
#include "openclcpp-lite/platform.h"

namespace ocl = openclcpp_lite;

TEST(DeviceTest, test)
{
    auto platform = ocl::Platform::get_default();
    auto dev = platform.devices()[0];
    dev.address_bits();
    dev.available();
    dev.built_in_kernels();
    dev.compiler_available();
    dev.endian_little();
    dev.error_correction_support();
    dev.extensions();
    dev.global_mem_cache_size();
    dev.global_mem_cache_line_size();
    dev.global_mem_size();
    dev.host_unified_memory();
    dev.image_support();
    dev.image2d_max_height();
    dev.image2d_max_width();
    dev.image3d_max_width();
    dev.image3d_max_depth();
    dev.image3d_max_height();
    dev.image3d_max_width();
    dev.image_max_buffer_size();
    dev.image_max_array_size();
    dev.linker_available();
    dev.local_mem_size();
    dev.max_clock_frequency();
    dev.max_compute_units();
    dev.max_constant_args();
    dev.max_constant_buffer_size();
    dev.max_mem_alloc_size();
    dev.max_parameter_size();
    dev.max_read_image_args();
    dev.max_samplers();
    dev.max_work_group_size();
    dev.max_work_item_dimensions();
    dev.max_work_item_sizes();
    dev.max_write_image_args();
    dev.mem_base_addr_align();
    dev.min_data_type_align_size();
    dev.name();
    dev.open_cl_version();
    dev.partition_max_sub_devices();
    dev.profile();
    dev.profiling_timer_resolution();
    dev.vendor();
    dev.vendor_id();
    dev.version();
    dev.driver_version();
    dev.type();
}
