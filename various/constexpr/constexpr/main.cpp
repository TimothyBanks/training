//
//  main.cpp
//  constexpr
//
//  Created by Timothy Banks on 8/17/20.
//  Copyright © 2020 Timothy Banks. All rights reserved.
//

#include <iostream>
#include <queue>
#include <random>
#include <vector>

struct key_value {
    int64_t value;
    int64_t key;
    char tag;
};

namespace std {
template <>
struct less<key_value*> final {
public:
    bool operator()(const key_value* lhs, const key_value* rhs) const {
        return lhs->key < rhs->key;
    };
};

template <>
struct greater<key_value*> final {
public:
    bool operator()(const key_value* lhs, const key_value* rhs) const {
        return lhs->key > rhs->key;
    };
};
}

#include <map>

std::vector<char> next_prefix(const uint8_t* prefix, uint32_t prefix_size) {
    auto buffer = std::vector<char>{prefix, prefix+prefix_size};

    for (int32_t i = buffer.size() - 1; i >= 0; --i) {
        if (++buffer[i] != std::numeric_limits<uint8_t>::min()) {
          break;
        }
    }

    return buffer;
}

int main(int argc, const char * argv[])
{
    constexpr int64_t powerup_frac = 1'000'000'000'000'000ll;
    constexpr int64_t powerup_frac_2 = 1 << 15;
    
    uint64_t v = 123456789;
    int64_t n_v = - v;
    
    auto str = std::string{"125531237439A41530BBD5BC43C110C85032114D4F38000000"};
    auto bytes = std::string{};
    for (size_t i = 0; i < str.size(); i += 2) {
      std::string hex = str.substr(i, 2);
      int8_t result = std::stoi(hex, 0, 16);
      char c{0};
      std::memcpy(&c, &result, 1);
      bytes += c;
    }
    
    
    
    
    auto align = ((1 + ((8) - 1)) & ~((8) - 1));
    align = ((2 + ((8) - 1)) & ~((8) - 1));
    align = ((3 + ((8) - 1)) & ~((8) - 1));
    align = ((4 + ((8) - 1)) & ~((8) - 1));
    align = ((5 + ((8) - 1)) & ~((8) - 1));
    align = ((6 + ((8) - 1)) & ~((8) - 1));
    align = ((7 + ((8) - 1)) & ~((8) - 1));
    align = ((8 + ((8) - 1)) & ~((8) - 1));
    align = ((9 + ((8) - 1)) & ~((8) - 1));
    align = ((10 + ((8) - 1)) & ~((8) - 1));
    align = ((11 + ((8) - 1)) & ~((8) - 1));
    align = ((0 + ((8) - 1)) & ~((8) - 1));
    
    auto mod = 0 % 8;
    mod = 1 % 8;
    mod = 2 % 8;
    mod = 3 % 8;
    mod = 4 % 8;
    mod = 5 % 8;
    mod = 6 % 8;
    mod = 7 % 8;
    mod = 8 % 8;
    mod = 9 % 8;
    mod = 10 % 8;
    mod = 11 % 8;
    mod = 12 % 8;
    mod = 13 % 8;
    mod = 14 % 8;
    mod = 15 % 8;
    mod = 16 % 8;
    
    uint64_t a = 10;
    uint64_t b = 15;
    auto diff = static_cast<int64_t>(a - b);
    
//    signed char a = 255;
//    unsigned char b = 255;
//    char c = 255;
//
//    uint8_t d = a;
//    d = b;
//    d = c;
    
    
    auto start = static_cast<char>(0xfe);
    ++start;
    ++start;
    auto end = static_cast<char>(0x0);
    
    if (end > start) {
        int i = 0;
    }
    
    
    auto buffer = std::vector<uint8_t>();
    buffer.emplace_back(0x00);
    buffer.emplace_back(0x01);
    buffer.emplace_back(0x02);
    buffer.emplace_back(0x03);
    buffer.emplace_back(0x04);
    buffer.emplace_back(0xFF);
    auto result = next_prefix(buffer.data(), buffer.size());
    
    auto lookup = std::map<std::string, std::string>{
        {"aahello",""},
        {"aaworld",""},
        {"bbhello",""},
        {"bbworld",""},
        {"bbtim",""},
        {"cchello",""},
        {"ccworld",""},
        {"ddhello",""},
        {"ddworld",""},
        {"eehello",""},
    };
    
    auto it = lookup.lower_bound("bb");
    auto it_2 = --lookup.lower_bound("cc");
    
//    auto result = std::less<std::string>{}("bb", "bbtim");
//    result = std::less<std::string>{}("bb", "cchello");
    
    auto hash_code = std::hash<std::string_view>()({nullptr, 0});
    
    int i = -1;
    if (i) {
        i = 1;
    }
    if (i) {
        i = 0;
    }
    if (i) {
        i = 2;
    }
    
    auto j = uint16_t{27217};
    auto k = uint16_t{53838};
    auto r = std::memcmp(&j, &k, sizeof(int16_t));
    
    auto values = std::vector<key_value*> {
        new key_value{.key = 0, .value = 0, .tag = 'a'},
        new key_value{.key = 0, .value = 0, .tag = 'b'},
        new key_value{.key = 0, .value = 0, .tag = 'c'},
        new key_value{.key = 0, .value = 1, .tag = 'd'},
        new key_value{.key = 0, .value = 0, .tag = 'e'},
    };
    
    auto next = std::priority_queue<key_value*, std::vector<key_value*>, std::greater<key_value*>>{};
    auto previous = std::priority_queue<key_value*, std::vector<key_value*>, std::less<key_value*>>{};
    
    auto random_number = [](uint16_t min, uint16_t max) {
        std::random_device random_device;
        std::mt19937 generator{random_device()};
        std::uniform_int_distribution<uint16_t> distribution{min, max};
        return distribution(generator);
    };
    
    for (const auto& kv : values) {
        next.push(kv);
        previous.push(kv);
    }
    
    for (size_t i = 0; i < 50; ++i) {
        auto top = next.top();
        next.pop();
        std::cout << top->key << ", ";
        top->key = random_number(previous.top()->key, std::numeric_limits<uint16_t>::max());
        next.push(top);
    }
    
//    auto top = previous.top();
//    previous.pop();
//    previous.push(top);
    for (size_t i = 0; i < 50; ++i) {
        auto top = previous.top();
        previous.pop();
        std::cout << top->key << ", ";
        top->key = random_number(0, next.top()->key);
        previous.push(top);
    }
    
    return 0;
}
