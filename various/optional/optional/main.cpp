//
//  main.cpp
//  optional
//
//  Created by Timothy Banks on 6/17/20.
//  Copyright © 2020 Timothy Banks. All rights reserved.
//

#include <iostream>
#include <optional>
#include <unordered_set>
#include <vector>

/*
 * Complete the 'authEvents' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts 2D_STRING_ARRAY events as parameter.
 */

size_t hash_password(const std::string& s)
{
    if (s.empty())
    {
        return 0;
    }

    if (s.size() == 1)
    {
        return static_cast<size_t>(s[0]);
    }

    constexpr auto p = size_t{131};
    constexpr auto m = size_t{1000000000 + 7};

    auto result = static_cast<size_t>(s[s.size() - 1]);
    auto current_p = p;
    for (int32_t i = s.size() - 2; i >= 0; --i)
    {
        result += (s[i] * current_p) % m;
        current_p *= p;
    }
    return result;
}

std::vector<int> authEvents(std::vector<std::vector<std::string>> events)
{
    auto results = std::vector<int>{};
    results.reserve(events.size());

    auto password_hashes = std::unordered_set<size_t>{};
    for (auto& e : events)
    {
        if (e[0] == "setPassword")
        {
            password_hashes.clear();
            password_hashes.emplace(hash_password(e[1]));
            for (size_t i = 0; i <= 127; ++i)
            {
                e[1].push_back(static_cast<char>(i));
                password_hashes.emplace(hash_password(e[1]));
                e[1].pop_back();
            }
        }
        else if (e[0] == "authorize")
        {
            auto h = std::stoll(e[1]);
            if (password_hashes.find(h) != std::end(password_hashes))
            {
                results.push_back(1);
                continue;
            }
            results.push_back(0);
        }
    }

    return results;
}

int minimumNumber(int n, const std::string& password)
{
    // Return the minimum number of characters to make the password strong
    constexpr auto min_length = size_t{6};
    static const auto special_characters
        = std::unordered_set<char>{'!','@','#','$','%','^','&','*','(',')','-','+'};

    auto result = int8_t{0};
    for (size_t i = 0; i < password.size(); ++i)
    {
        if (std::isupper(password[i]))
        {
            result |= 0b00000001;
            continue;
        }
        if (std::islower(password[i]))
        {
            result |= 0b00000010;
            continue;
        }
        if (std::isdigit(password[i]))
        {
            result |= 0b00000100;
            continue;
        }
        if (special_characters.find(password[i]) != std::end(special_characters))
        {
            result |= 0b00001000;
            continue;
        }
    }

    auto missing = [](int8_t v)
    {
        switch (v)
        {
            case 0b00000000:
                return 4;
            case 0b00000001:
            case 0b00000010:
            case 0b00000100:
            case 0b00001000:
                return 3;
            case 0b00000011:
            case 0b00000110:
            case 0b00001100:
            case 0b00001001:
            case 0b00000101:
            case 0b00001010:
                return 2;
            case 0b00000111:
            case 0b00001011:
            case 0b00001101:
            case 0b00001110:
                return 1;
            default:
                return 0;
        };
    };

    auto minimum = missing(result);
    auto diff = static_cast<int64_t>(password.size() - min_length);

    // We've met the minimum length requirement.  The user
    // needs to meet the other rules.
    if (diff >= 0)
    {
        return minimum;
    }

    diff = std::abs(diff);
    if (minimum <= diff)
    {
        // This case the user hasn't met the length requirement AND
        // they could meet the full requirements if they included
        // the rest of the rules while fulfilling the length requirement.
        return diff;
    }

    // This case the user hasn't met the length requirement AND
    // they could meet most of the requirements by fulfilling the length
    // requirement plus some additional characters for the other requirements.
    return diff + (minimum - diff);
}

int main(int argc, const char * argv[])
{
    auto min = minimumNumber(11, "#HackerRank");
    
    auto h = hash_password("1");
    h = hash_password("2");
    h = hash_password("3");
    
    //auto o = std::make_optional<int32_t>();
    auto o = std::optional<int32_t>();
    
    if (!o)
    {
        return 1;
    }
    
    return 0;
}
