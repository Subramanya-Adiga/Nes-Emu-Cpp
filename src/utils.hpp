#pragma once
#include <memory>

namespace nes_emu {

using file_deleter = void (*)(FILE *handle);
using file_handle = std::unique_ptr<FILE, file_deleter>;

struct file_handle_helper {
  [[nodiscard]] static std::size_t file_size(file_handle &handle);
  [[nodiscard]] static std::size_t file_position(file_handle &handle);
  [[nodiscard]] static bool is_eof(file_handle &handle);

  //***************************Buffer Read**********************************//
  [[nodiscard]] static std::vector<uint8_t> read_vec_u8(file_handle &handle,
                                                        uint32_t size);
  [[nodiscard]] static std::vector<uint8_t> read_vec_u8(file_handle &handle);
  [[nodiscard]] static size_t write_vec_u8(file_handle &handle,
                                           std::vector<uint8_t> &vec);

  //***************************Intiger Read**********************************//
  [[nodiscard]] static uint8_t read_u8(file_handle &handle);
  [[nodiscard]] static uint16_t read_u16(file_handle &handle,
                                         bool big_endian = false);
  [[nodiscard]] static uint32_t read_u32(file_handle &handle,
                                         bool big_endian = false);

  [[nodiscard]] static uint64_t read_u64(file_handle &handle,
                                         bool big_endian = false);

  //***************************Intiger Write**********************************//
  [[nodiscard]] static size_t write_u8(file_handle &handle, uint8_t value);

  [[nodiscard]] static size_t write_u16(file_handle &handle, uint16_t value,
                                        bool big_endian = false);

  [[nodiscard]] static size_t write_u32(file_handle &handle, uint32_t value,
                                        bool big_endian = false);

  [[nodiscard]] static size_t write_u64(file_handle &handle, uint64_t value,
                                        bool big_endian = false);

  //***************************String Read**********************************//
  [[nodiscard]] static std::string read_z_string(file_handle &handle);
  [[nodiscard]] static std::string read_u8_string(file_handle &handle);

  [[nodiscard]] static std::string read_u16_string(file_handle &handle,
                                                   bool big_endian = false);

  [[nodiscard]] static std::string read_u32_string(file_handle &handle,
                                                   bool big_endian = false);

  //***************************String Write**********************************//
  [[nodiscard]] static bool write_z_string(file_handle &handle,
                                           const std::string &str);
  [[nodiscard]] static bool write_u8_string(file_handle &handle,
                                            const std::string &str);

  [[nodiscard]] static bool write_u16_string(file_handle &handle,
                                             const std::string &str,
                                             bool big_endian = false);

  [[nodiscard]] static bool write_u32_string(file_handle &handle,
                                             const std::string &str,
                                             bool big_endian = false);
};

} // namespace nes_emu
