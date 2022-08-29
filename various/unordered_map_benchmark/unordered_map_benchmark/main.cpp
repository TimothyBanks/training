//
//  main.cpp
//  unordered_map_benchmark
//
//  Created by Timothy Banks on 7/22/20.
//  Copyright © 2020 Timothy Banks. All rights reserved.
//

#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <unordered_map>


template <typename T>
T random_t(std::random_device& device, T min, T max)
{
    std::mt19937 generator{device()};
    std::uniform_int_distribution<T> distribution{min, max};
    return distribution(generator);
}

std::string random_string(std::random_device& device, size_t length)
{
    auto result = std::string{};
    
    for (size_t i = 0; i < length; ++i)
    {
        result += static_cast<char>(random_t<int8_t>(device, 33, 126));
    }
    
    return result;
}

template <typename Key, typename Value>
void cache_benchmark_1(size_t iterations, const std::vector<Key>& range)
{
    auto cache = std::unordered_map<Key, Value>{};
    
    std::random_device random_device;
    auto key = random_t(random_device, range[0], range[1]);
    auto value = random_t(random_device, std::numeric_limits<Value>::min(), std::numeric_limits<Key>::max());
    
    cache.emplace(std::make_pair(key, value));
    auto& hot_spot = cache[key];
    
    auto total_time = std::chrono::nanoseconds{};
    
    auto read = [&]()
    {
        auto start = std::chrono::steady_clock::now();
        auto read_value = hot_spot;
        auto end = std::chrono::steady_clock::now();
        total_time += (end - start);
    };
    
    auto write = [&]()
    {
        auto new_value = random_t(random_device, std::numeric_limits<Value>::min(), std::numeric_limits<Key>::max());
        auto start = std::chrono::steady_clock::now();
        hot_spot = new_value;
        auto end = std::chrono::steady_clock::now();
        total_time += (end - start);
    };
    
    for (size_t i = 0; i < iterations; ++i)
    {
        if (random_t(random_device, 0, 1) == 0)
        {
            read();
            continue;
        }
        write();
    }
    auto start = std::chrono::steady_clock::now();
    hot_spot = value;
    auto end = std::chrono::steady_clock::now();
    total_time += (end - start);
    
    std::cout << "cache_benchmark_1 total time: " << total_time.count() << " nanoseconds (" << total_time.count() / 1000000000.0 << " seconds)" << std::endl;
}

template <>
void cache_benchmark_1<std::string, std::string>(size_t iterations, const std::vector<std::string>& range)
{
    auto cache = std::unordered_map<std::string, std::string>{};
    
    std::random_device random_device;
    auto key_position = random_t(random_device, static_cast<size_t>(0), range.size());
    auto& key = range[key_position];
    auto value = random_string(random_device, 20);
    
    cache.emplace(std::make_pair(key, value));
    auto& hot_spot = cache[key];
    
    auto total_time = std::chrono::nanoseconds{};
    
    auto read = [&]()
    {
        auto start = std::chrono::steady_clock::now();
        auto read_value = hot_spot;
        auto end = std::chrono::steady_clock::now();
        total_time += (end - start);
    };
    
    auto write = [&]()
    {
        auto new_value = random_string(random_device, 20);
        auto start = std::chrono::steady_clock::now();
        hot_spot = new_value;
        auto end = std::chrono::steady_clock::now();
        total_time += (end - start);
    };
    
    for (size_t i = 0; i < iterations; ++i)
    {
        if (random_t(random_device, 0, 1) == 0)
        {
            read();
            continue;
        }
        write();
    }
    auto start = std::chrono::steady_clock::now();
    hot_spot = value;
    auto end = std::chrono::steady_clock::now();
    total_time += (end - start);
    
    std::cout << "cache_benchmark_1 (string) total time: " << total_time.count() << " nanoseconds (" << total_time.count() / 1000000000.0 << " seconds)" << std::endl;
}

template <typename Key, typename Value>
void cache_benchmark_2(size_t iterations, const std::vector<Key>& range)
{
    auto cache = std::unordered_map<Key, Value>{};
    
    std::random_device random_device;
    auto key = random_t(random_device, range[0], range[1]);
    auto value = random_t(random_device, std::numeric_limits<Value>::min(), std::numeric_limits<Key>::max());
    
    cache.emplace(std::make_pair(key, value));
    auto& hot_spot = cache[key];
    
    auto total_time = std::chrono::nanoseconds{};
    
    auto read = [&]()
    {
        auto key_to_read = random_t(random_device, range[0], range[1]);
        auto start = std::chrono::steady_clock::now();
        auto it = cache.find(key_to_read);
        if (it == std::end(cache))
        {
            return;
        }
        auto& value_read = it->second;
        auto end = std::chrono::steady_clock::now();
        total_time += (end - start);
    };
    
    auto write = [&]()
    {
        auto key_to_write = random_t(random_device, range[0], range[1]);
        auto new_value = random_t(random_device, std::numeric_limits<Value>::min(), std::numeric_limits<Key>::max());
        auto start = std::chrono::steady_clock::now();
        auto it = cache.find(key_to_write);
        if (it != std::end(cache))
        {
            it->second = new_value;
            return;
        }
        cache.emplace(std::make_pair(key_to_write, new_value));
        auto end = std::chrono::steady_clock::now();
        total_time += (end - start);
    };
    
    for (size_t i = 0; i < iterations; ++i)
    {
        if (random_t(random_device, 0, 1) == 0)
        {
            read();
            continue;
        }
        write();
    }
    
    std::cout << "cache_benchmark_2 total time: " << total_time.count() << " nanoseconds (" << total_time.count() / 1000000000.0 << " seconds)" << std::endl;
}

template <>
void cache_benchmark_2<std::string, std::string>(size_t iterations, const std::vector<std::string>& range)
{
    auto cache = std::unordered_map<std::string, std::string>{};
    
    std::random_device random_device;
    auto key_position = random_t(random_device, static_cast<size_t>(0), range.size());
    auto& key = range[key_position];
    auto value = random_string(random_device, 100);
    
    cache.emplace(std::make_pair(key, value));
    auto& hot_spot = cache[key];
    
    auto total_time = std::chrono::nanoseconds{};
    
    auto read = [&]()
    {
        auto key_position = random_t(random_device, static_cast<size_t>(0), range.size());
        auto& key_to_read = range[key_position];
        auto start = std::chrono::steady_clock::now();
        auto it = cache.find(key_to_read);
        if (it == std::end(cache))
        {
            return;
        }
        auto& value_read = it->second;
        auto end = std::chrono::steady_clock::now();
        total_time += (end - start);
    };
    
    auto write = [&]()
    {
        auto key_position = random_t(random_device, static_cast<size_t>(0), range.size());
        auto& key_to_write = range[key_position];
        auto new_value = random_string(random_device, 20);
        auto start = std::chrono::steady_clock::now();
        auto it = cache.find(key_to_write);
        if (it != std::end(cache))
        {
            it->second = new_value;
            return;
        }
        cache.emplace(std::make_pair(key_to_write, new_value));
        auto end = std::chrono::steady_clock::now();
        total_time += (end - start);
    };
    
    for (size_t i = 0; i < iterations; ++i)
    {
        if (random_t(random_device, 0, 1) == 0)
        {
            read();
            continue;
        }
        write();
    }
    
    std::cout << "cache_benchmark_2 (string) total time: " << total_time.count() << " nanoseconds (" << total_time.count() / 1000000000.0 << " seconds)" << std::endl;
}

int main(int argc, const char * argv[])
{
    std::random_device random_device;
    constexpr auto key_count = size_t{50};
    auto keys = std::vector<std::string>{};
    keys.reserve(key_count);
    for (size_t i = 0; i < key_count; ++i)
    {
        keys.emplace_back(random_string(random_device, 100));
    }
    
    cache_benchmark_1<uint64_t, uint64_t>(1000000, {0, 1000});
    cache_benchmark_1<std::string, std::string>(1000000, keys);
    cache_benchmark_2<uint64_t, uint64_t>(1000000, {0, 1000});
    cache_benchmark_2<std::string, std::string>(1000000, keys);
    return 0;
}
