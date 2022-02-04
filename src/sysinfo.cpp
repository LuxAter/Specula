#include "sysinfo.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <infoware/infoware.hpp>
#include <magic_enum.hpp>
#include <toml++/toml.h>

#include <specula/logging.hpp>

using namespace specula::sysinfo;

toml::table CPU::to_toml() const {
  toml::array is_array;
  for (const auto& it : supported_instruction_sets)
    is_array.push_back(magic_enum::enum_name(it));
  return toml::table{
      {"Architecture", magic_enum::enum_name(architecture)},
      {"Endianness", magic_enum::enum_name(endianness)},
      {"Frequency", static_cast<std::int64_t>(frequency)},
      {"Logical", logical},
      {"ModelName", model_name},
      {"Packages", packages},
      {"Physical", physical},
      {"SupportedInstructionSets", is_array},
      {"Vendor", vendor},
      {"VendorId", vendor_id},
  };
}

toml::table GPU::to_toml() const {
  return toml::table{
      {"Vendor", magic_enum::enum_name(vendor)},
      {"Name", name},
      {"Memory", static_cast<std::int64_t>(memory_size)},
      {"Cache", static_cast<std::int64_t>(cache_size)},
      {"MaxFrequency", static_cast<std::int64_t>(max_frequency)},
  };
}

toml::table Memory::to_toml() const {
  return toml::table{
      {"Physical", static_cast<std::int64_t>(physical_total)},
      {"Virtual", static_cast<std::int64_t>(virtual_total)},
  };
}

toml::table Kernel::to_toml() const {
  return toml::table{{"Variant", magic_enum::enum_name(variant)},
                     {"Version", fmt::format("{}.{}.{}.{}", major, minor, patch,
                                             build_number)}};
}

toml::table OS::to_toml() const {
  return toml::table{{"Name", name},
                     {"FullName", full_name},
                     {"Version", fmt::format("{}.{}.{}.{}", major, minor, patch,
                                             build_number)}};
}

toml::table Device::to_toml() const {
  toml::array ec_array;
  for (const auto& it : execution_capabilities) {
    ec_array.push_back(magic_enum::enum_name(it));
  }

  toml::array a_array;
  for (const auto& it : aspects) {
    a_array.push_back(magic_enum::enum_name(it));
  }

  return toml::table{
      {"DeviceType", magic_enum::enum_name(device_type)},
      {"VendorId", vendor_id},
      {"MaxComputeUnits", max_compute_units},
      {"MaxClockFrequency", max_clock_frequency},
      {"MaxMemAllocSize", static_cast<std::int64_t>(max_mem_alloc_size)},
      {"GlobalMemSize", static_cast<std::int64_t>(global_mem_size)},
      {"LocalMemSize", static_cast<std::int64_t>(local_mem_size)},
      {"IsAvailable", is_available},
      {"ExecutionCapabilities", ec_array},
      {"DriverVersion", driver_version},
      {"Version", version},
      {"Aspects", a_array},
      {"Name", name},
      {"Vendor", vendor}};
}

toml::table Platform::to_toml() const {
  toml::array device_array;
  std::transform(devices.begin(), devices.end(),
                 std::back_inserter(device_array),
                 [](const auto& it) { return it.to_toml(); });
  return toml::table{{"Name", name},
                     {"Vendor", vendor},
                     {"Version", version},
                     {"Device", device_array}};
}

toml::table System::to_toml() const {
  toml::array gpu_array;
  std::transform(gpus.begin(), gpus.end(), std::back_inserter(gpu_array),
                 [](const auto& it) { return it.to_toml(); });

  toml::array platform_array;
  std::transform(platforms.begin(), platforms.end(),
                 std::back_inserter(platform_array),
                 [](const auto& it) { return it.to_toml(); });
  return toml::table{
      {"Kernel", kernel.to_toml()}, {"Memory", memory.to_toml()},
      {"OS", os.to_toml()},         {"CPU", cpu.to_toml()},
      {"GPU", gpu_array},           {"Platform", platform_array},
  };
}

CPU specula::sysinfo::cpu() {
  const auto quantities = iware::cpu::quantities();
  return CPU{
      .architecture = iware::cpu::architecture(),
      .endianness = iware::cpu::endianness(),
      .frequency = iware::cpu::frequency(),
      .logical = quantities.logical,
      .physical = quantities.physical,
      .packages = quantities.packages,
      .model_name = iware::cpu::model_name(),
      .vendor = iware::cpu::vendor(),
      .vendor_id = iware::cpu::vendor_id(),
      .supported_instruction_sets = iware::cpu::supported_instruction_sets(),
  };
}

std::vector<GPU> specula::sysinfo::gpus() {
  std::vector<GPU> gpus;
  const auto device_properties = iware::gpu::device_properties();
  std::transform(device_properties.begin(), device_properties.end(),
                 std::back_inserter(gpus), [](const auto& it) {
                   return GPU{
                       .vendor = it.vendor,
                       .name = it.name,
                       .memory_size = it.memory_size,
                       .cache_size = it.cache_size,
                       .max_frequency = it.max_frequency,
                   };
                 });
  return gpus;
}

Kernel specula::sysinfo::kernel() {
  const auto kernel_info = iware::system::kernel_info();
  return Kernel{
      .variant = kernel_info.variant,
      .major = kernel_info.major,
      .minor = kernel_info.minor,
      .patch = kernel_info.patch,
      .build_number = kernel_info.build_number,
  };
}

Memory specula::sysinfo::memory() {
  const auto memory = iware::system::memory();
  return Memory{
      .physical_total = memory.physical_total,
      .virtual_total = memory.virtual_total,
  };
}

OS specula::sysinfo::os() {
  const auto os_info = iware::system::OS_info();
  return OS{
      .name = os_info.name,
      .full_name = os_info.full_name,
      .major = os_info.major,
      .minor = os_info.minor,
      .patch = os_info.patch,
      .build_number = os_info.build_number,
  };
}

std::vector<Device> specula::sysinfo::devices(const sycl::platform& platform) {
  std::vector<Device> devices;
  const auto sycl_devices = platform.get_devices();
  std::transform(
      sycl_devices.begin(), sycl_devices.end(), std::back_inserter(devices),
      [](const auto& it) {
        return Device{
            .device_type =
                it.template get_info<sycl::info::device::device_type>(),
            .vendor_id = it.template get_info<sycl::info::device::vendor_id>(),
            .max_compute_units =
                it.template get_info<sycl::info::device::max_compute_units>(),
            .max_clock_frequency =
                it.template get_info<sycl::info::device::max_clock_frequency>(),
            .max_mem_alloc_size =
                it.template get_info<sycl::info::device::max_mem_alloc_size>(),
            .global_mem_size =
                it.template get_info<sycl::info::device::global_mem_size>(),
            .local_mem_size =
                it.template get_info<sycl::info::device::local_mem_size>(),
            .is_available =
                it.template get_info<sycl::info::device::is_available>(),
            .execution_capabilities = it.template get_info<
                sycl::info::device::execution_capabilities>(),
            .driver_version =
                it.template get_info<sycl::info::device::driver_version>(),
            .version = it.template get_info<sycl::info::device::version>(),
            .aspects = it.template get_info<sycl::info::device::aspects>(),
            .name = it.template get_info<sycl::info::device::name>(),
            .vendor = it.template get_info<sycl::info::device::vendor>(),
        };
      });
  return devices;
}

std::vector<Platform> specula::sysinfo::platforms() {
  std::vector<Platform> platforms;
  const auto sycl_platforms = sycl::platform::get_platforms();
  std::transform(
      sycl_platforms.begin(), sycl_platforms.end(),
      std::back_inserter(platforms), [](const auto& it) {
        return Platform{
            .name = it.template get_info<sycl::info::platform::name>(),
            .vendor = it.template get_info<sycl::info::platform::vendor>(),
            .version = it.template get_info<sycl::info::platform::version>(),
            .devices = devices(it)};
      });
  return platforms;
}

System specula::sysinfo::system() {
  return System{.kernel = kernel(),
                .memory = memory(),
                .os = os(),
                .cpu = cpu(),
                .gpus = gpus(),
                .platforms = platforms()};
}
