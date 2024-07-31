#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace pinestreet {

struct block_header {
  uint32_t version{0};
  std::array<uint8_t, 32> previous_hash;
  std::array<uint8_t, 32> merkle_root;
  uint32_t time{0};
  uint32_t nBits{0};
  uint32_t nonce{0};
};

struct block {
  uint32_t magic{0};
  uint32_t size{0};
  block_header header;
  uint32_t tx_count{0};
  std::vector<uint8_t> transactions;
};

class query {
 public:
  std::tuple<uint32_t, block_header> query_block_header(size_t block_height) {
    auto it = m_blocks.find(block_height);
    if (it != std::end(m_blocks)) {
      return std::make_tuple(0, it->second.header);
    }

    auto file_stream =
        std::ifstream{"blk00000.dat", std::ios::binary | std::ios::in};
    if (!file_stream.is_open()) {
      std::cout << "Not opened" << std::endl;
    }

    auto current_block = size_t{0};
    auto magic_bytes = uint32_t{0};
    auto size = uint32_t{0};

    auto swap_endian = [](auto v) {
      // It appears we already have the right endianness when reading.
      return v;
      // auto result = decltype(v){};
      // for (size_t i = 0; i < sizeof(v); ++i) {
      //   reinterpret_cast<char*>(&result)[i] =
      //   reinterpret_cast<char*>(&v)[sizeof(v) - i - 1];
      // }
      // return result;
    };

    while (current_block <= block_height) {
      file_stream.read(reinterpret_cast<char*>(&magic_bytes),
                       sizeof(magic_bytes));
      magic_bytes = swap_endian(magic_bytes);
      file_stream.read(reinterpret_cast<char*>(&size), sizeof(size));
      size = swap_endian(size);

      if (current_block == block_height) {
        break;
      }

      file_stream.seekg(size, std::ios_base::cur);

      if (file_stream.eof()) {
        return std::make_tuple(1, block_header{});
      }

      ++current_block;
    }

    auto b = block{};
    b.magic = magic_bytes;
    b.size = size;

    file_stream.read(reinterpret_cast<char*>(&b.header), sizeof(b.header));
    b.header.version = swap_endian(b.header.version);
    b.header.time = swap_endian(b.header.time);
    b.header.nBits = swap_endian(b.header.nBits);
    b.header.nonce = swap_endian(b.header.nonce);
    std::reverse(std::begin(b.header.previous_hash),
                 std::end(b.header.previous_hash));
    std::reverse(std::begin(b.header.merkle_root),
                 std::end(b.header.merkle_root));
    m_blocks.emplace(block_height, b);

    return std::make_tuple(0, b.header);
  }

  static query& instance() {
    static auto _instance = query{};
    return _instance;
  }

 protected:
  query() = default;

 private:
  std::unordered_map<size_t, block> m_blocks;
};

namespace tests {
void run_tests() {
  auto print = [](const auto& height) {
    auto [result, b] = query::instance().query_block_header(height);
    std::cout << "Block Height " << std::to_string(height) << std::endl;
    std::cout << std::to_string(b.version) << std::endl;
    auto ss = std::stringstream{};
    for (const auto c : b.previous_hash) {
      ss << std::setfill('0') << std::setw(sizeof(c) * 2) << std::hex
         << static_cast<uint32_t>(c);
    }
    std::cout << ss.str() << std::endl;
    ss = std::stringstream{};
    for (const auto c : b.merkle_root) {
      ss << std::setfill('0') << std::setw(sizeof(c) * 2) << std::hex
         << static_cast<uint32_t>(c);
    }
    std::cout << ss.str() << std::endl;
    std::cout << std::to_string(b.time) << std::endl;
    std::cout << std::to_string(b.nBits) << std::endl;
    std::cout << std::to_string(b.nonce) << std::endl;
    std::cout << std::endl;
  };

  print(0);
  print(2);
  print(4);
  print(6);
  print(255);
  print(1024);
  print(9999);
  print(10000);
}
}  // namespace tests

}  // namespace pinestreet