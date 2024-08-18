#include "utils.h"
#include <bit>

namespace nes_emu {

std::size_t file_handle_helper::file_size(file_handle &handle) {
  auto read_size = fseek(&*handle, 0, SEEK_END);
  if (read_size == 0) {
    auto size = ftell(&*handle);
    if (fseek(&*handle, 0, SEEK_SET) == 0) {
      // rewind(&*handle);
      return static_cast<std::size_t>(size);
    }
  }
  return read_size;
}

std::size_t file_handle_helper::file_position(file_handle &handle) {
  return static_cast<std::size_t>(ftell(&*handle));
}

bool file_handle_helper::is_eof(file_handle &handle) {
  return feof(&*handle) == 0U;
}

std::vector<uint8_t> file_handle_helper::read_vec_u8(file_handle &handle,uint32_t size) {
  if (size != 0) {
    std::vector<uint8_t> ret(size);
    if (fread(ret.data(), 1, size, &*handle) == size) {
      return ret;
    }
  }
  return {};
}

std::vector<uint8_t> file_handle_helper::read_vec_u8(file_handle &handle) {
  auto size = read_u64(handle);
  if (size != 0) {
    std::vector<uint8_t> ret(size);
    if (fread(ret.data(), 1, size, &*handle) == size) {
      return ret;
    }
  }
  return {};
}

size_t file_handle_helper::write_vec_u8(file_handle &handle,
                                        std::vector<uint8_t> &vec) {
  if (write_u64(handle, vec.size()) == vec.size()) {
    if (auto write_size = fwrite(vec.data(), 1, vec.size(), &*handle);
        write_size == vec.size()) {
      return write_size;
    }
  }
  return 0;
}

uint8_t file_handle_helper::read_u8(file_handle &handle) {
  std::array<uint8_t, sizeof(uint8_t)> buffer{};
  uint8_t value{};
  if (fread(buffer.data(), 1, sizeof(uint8_t), &*handle) == sizeof(uint8_t)) {
    memcpy(&value, buffer.data(), sizeof(uint8_t));
    return value;
  }
  return 0;
}

uint16_t file_handle_helper::read_u16(file_handle &handle, bool big_endian) {
  std::array<uint8_t, sizeof(uint16_t)> buffer{};
  uint16_t value{};
  if (fread(buffer.data(), 1, sizeof(uint16_t), &*handle) == sizeof(uint16_t)) {
    memcpy(&value, buffer.data(), sizeof(uint16_t));
    if (big_endian) {
      return std::byteswap(value);
    }
    return value;
  }
  return 0;
}

uint32_t file_handle_helper::read_u32(file_handle &handle, bool big_endian) {
  std::array<uint8_t, sizeof(uint32_t)> buffer{};
  uint32_t value{};
  if (fread(buffer.data(), 1, sizeof(uint32_t), &*handle) == sizeof(uint32_t)) {
    memcpy(&value, buffer.data(), sizeof(uint32_t));
    if (big_endian) {
      return std::byteswap(value);
    }
    return value;
  }
  return 0;
}

uint64_t file_handle_helper::read_u64(file_handle &handle, bool big_endian) {
  std::array<uint8_t, sizeof(uint64_t)> buffer{};
  uint64_t value{};
  if (fread(buffer.data(), 1, sizeof(uint64_t), &*handle) == sizeof(uint64_t)) {
    memcpy(&value, buffer.data(), sizeof(uint64_t));
    if (big_endian) {
      return std::byteswap(value);
    }
    return value;
  }
  return 0;
}

std::string file_handle_helper::read_z_string(file_handle &handle) {
  std::vector<char> buffer{};
  int file_char{};
  while ((file_char = fgetc(&*handle)) != EOF) {
    buffer.emplace_back(static_cast<char>(file_char));
    if (file_char == '\0') {
      break;
    }
  }
  return buffer.data();
}

std::string file_handle_helper::read_u8_string(file_handle &handle) {
  auto size = read_u8(handle);
  std::vector<uint8_t> buffer(size);

  if (fread(buffer.data(), 1, size, &*handle) == size) {
    return {buffer.begin(), buffer.end()};
  }
  return {};
}

std::string file_handle_helper::read_u16_string(file_handle &handle,
                                                bool big_endian) {
  auto size = read_u16(handle, big_endian);
  std::vector<uint8_t> buffer(size);
  if (fread(buffer.data(), 1, size, &*handle) == size) {
    return {buffer.begin(), buffer.end()};
  }
  return {};
}

std::string file_handle_helper::read_u32_string(file_handle &handle,
                                                bool big_endian) {
  auto size = read_u32(handle, big_endian);
  std::vector<uint8_t> buffer(size);
  if (fread(buffer.data(), 1, size, &*handle) == size) {
    return {buffer.begin(), buffer.end()};
  }
  return {};
}

size_t file_handle_helper::write_u8(file_handle &handle, uint8_t value) {
  if (auto size = fwrite(&value, 1, sizeof(uint8_t), &*handle);
      size == sizeof(uint8_t)) {
    return size;
  }
  return 0;
}

size_t file_handle_helper::write_u16(file_handle &handle, uint16_t value,
                                     bool big_endian) {
  if (big_endian) {
    value = std::byteswap(value);
  }
  if (auto size = fwrite(&value, 1, sizeof(uint16_t), &*handle);
      size == sizeof(uint16_t)) {
    return size;
  }
  return 0;
}

size_t file_handle_helper::write_u32(file_handle &handle, uint32_t value,
                                     bool big_endian) {
  if (big_endian) {
    value = std::byteswap(value);
  }
  if (auto size = fwrite(&value, 1, sizeof(uint32_t), &*handle);
      size == sizeof(uint32_t)) {
    return size;
  }
  return 0;
}

size_t file_handle_helper::write_u64(file_handle &handle, uint64_t value,
                                     bool big_endian) {
  if (big_endian) {
    value = std::byteswap(value);
  }
  if (auto size = fwrite(&value, 1, sizeof(uint64_t), &*handle);
      size == sizeof(uint64_t)) {
    return size;
  }
  return 0;
}

bool file_handle_helper::write_z_string(file_handle &handle,
                                        const std::string &str) {
  auto ret = fputs(str.c_str(), &*handle) != EOF;
  return ret && (fwrite("", 1, 1, &*handle) != 0U);
}

bool file_handle_helper::write_u8_string(file_handle &handle,
                                         const std::string &str) {
  auto sz_res = write_u8(handle, static_cast<uint8_t>(str.size()));
  return (sz_res != 0U) && fputs(str.c_str(), &*handle) != EOF;
}

bool file_handle_helper::write_u16_string(file_handle &handle,
                                          const std::string &str,
                                          bool big_endian) {
  auto sz_res =
      write_u16(handle, static_cast<uint16_t>(str.size()), big_endian);
  return (sz_res != 0U) && fputs(str.c_str(), &*handle) != EOF;
}

bool file_handle_helper::write_u32_string(file_handle &handle,
                                          const std::string &str,
                                          bool big_endian) {
  auto sz_res =
      write_u32(handle, static_cast<uint32_t>(str.size()), big_endian);
  return (sz_res != 0U) && fputs(str.c_str(), &*handle) != EOF;
}

} // namespace nes_emu
