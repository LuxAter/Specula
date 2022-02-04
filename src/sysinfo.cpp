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

toml::table System::to_toml() const {
  toml::array gpu_array;
  for (const auto& it : gpus) gpu_array.push_back(it.to_toml());
  return toml::table{
      {"Kernel", kernel.to_toml()}, {"Memory", memory.to_toml()},
      {"OS", os.to_toml()},         {"CPU", cpu.to_toml()},
      {"GPU", gpu_array},
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
  for (const auto& it : iware::gpu::device_properties()) {
    gpus.push_back(GPU{
        .vendor = it.vendor,
        .name = it.name,
        .memory_size = it.memory_size,
        .cache_size = it.cache_size,
        .max_frequency = it.max_frequency,
    });
  }
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

System specula::sysinfo::system() {
  return System{
      .kernel = kernel(),
      .memory = memory(),
      .os = os(),
      .cpu = cpu(),
      .gpus = gpus(),
  };
}
