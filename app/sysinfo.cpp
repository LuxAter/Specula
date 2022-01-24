#include "sysinfo.hpp"

#include <iostream>
#include <string>
#include <string_view>

#include <infoware/infoware.hpp>
#include <magic_enum.hpp>
#include <specula/logging.hpp>
#include <toml++/toml.h>

int specula::cli::sysinfo(std::string format) {
  transform(format.begin(), format.end(), format.begin(), ::tolower);

  const auto cpu_quantities = iware::cpu::quantities();
  const auto kernel_info = iware::system::kernel_info();
  const auto memory = iware::system::memory();
  const auto os_info = iware::system::OS_info();

  toml::array supported_instruction_sets;
  for (const auto &it : iware::cpu::supported_instruction_sets())
    supported_instruction_sets.push_back(magic_enum::enum_name(it));

  toml::array gpu;
  for (const auto &it : iware::gpu::device_properties())
    gpu.push_back(toml::table{
        {"Vendor", magic_enum::enum_name(it.vendor)},
        {"Name", it.name},
        {"Memory", static_cast<std::int64_t>(it.memory_size)},
        {"Cache", static_cast<std::int64_t>(it.cache_size)},
        {"MaxFrequency", static_cast<std::int64_t>(it.max_frequency)},
    });

  const auto tbl = toml::table{
      {"CPU",
       toml::table{
           {"Architecture", magic_enum::enum_name(iware::cpu::architecture())},
           {"Endianness", magic_enum::enum_name(iware::cpu::endianness())},
           {"Frequency", static_cast<std::int64_t>(iware::cpu::frequency())},
           {"Logical", cpu_quantities.logical},
           {"ModelName", iware::cpu::model_name()},
           {"Packages", cpu_quantities.packages},
           {"Physical", cpu_quantities.physical},
           {"SupportedInstructionSets", supported_instruction_sets},
           {"Vendor", iware::cpu::vendor()},
           {"VendorId", iware::cpu::vendor_id()},
       }},
      {"System",
       toml::table{
           {"Kernel",
            toml::table{
                {"Variant", magic_enum::enum_name(kernel_info.variant)},
                {"Version", fmt::format("{}.{}.{}.{}", kernel_info.major,
                                        kernel_info.minor, kernel_info.patch,
                                        kernel_info.build_number)}}},
           {"Memory",
            toml::table{
                {"Physical", static_cast<std::int64_t>(memory.physical_total)},
                {"Virtual", static_cast<std::int64_t>(memory.virtual_total)},
            }},
           {"OS",
            toml::table{{"Name", os_info.name},
                        {"FullName", os_info.full_name},
                        {"Version", fmt::format("{}.{}.{}.{}", os_info.major,
                                                os_info.minor, os_info.patch,
                                                os_info.build_number)}}}}},
      {"GPU", gpu}};

  if (format == "toml")
    std::cout << tbl << std::endl;
  else if (format == "json")
    std::cout << toml::json_formatter{tbl} << std::endl;
  else if (format == "yaml")
    std::cout << toml::yaml_formatter{tbl} << std::endl;
  return 0;
}
