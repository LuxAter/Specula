#ifndef SPECULA_SYSINFO_HPP_
#define SPECULA_SYSINFO_HPP_

#include <string>
#include <vector>

#include <infoware/infoware.hpp>
#include <toml++/toml.h>

namespace specula::sysinfo {
struct CPU {
  iware::cpu::architecture_t architecture;
  iware::cpu::endianness_t endianness;
  std::uint64_t frequency;
  std::uint32_t logical;
  std::uint32_t physical;
  std::uint32_t packages;
  std::string model_name;
  std::string vendor;
  std::string vendor_id;
  std::vector<iware::cpu::instruction_set_t> supported_instruction_sets;

  toml::table to_toml() const;
};

struct GPU {
  iware::gpu::vendor_t vendor;
  std::string name;
  std::uint64_t memory_size;
  std::uint64_t cache_size;
  std::uint64_t max_frequency;

  toml::table to_toml() const;
};

struct Kernel {
  iware::system::kernel_t variant;
  std::uint32_t major;
  std::uint32_t minor;
  std::uint32_t patch;
  std::uint32_t build_number;

  toml::table to_toml() const;
};
struct Memory {
  std::uint64_t physical_total;
  std::uint64_t virtual_total;

  toml::table to_toml() const;
};

struct OS {
  std::string name;
  std::string full_name;
  std::uint32_t major, minor, patch, build_number;

  toml::table to_toml() const;
};

struct System {
  Kernel kernel;
  Memory memory;
  OS os;

  CPU cpu;
  std::vector<GPU> gpus;

  toml::table to_toml() const;
};

CPU cpu();
std::vector<GPU> gpus();
Kernel kernel();
Memory memory();
OS os();

System system();

}  // namespace specula::sysinfo

#endif  // !SPECULA_SYSINFO_HPP_
