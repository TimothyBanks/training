//
//  main.cpp
//  variant
//
//  Created by Timothy Banks on 5/28/20.
//  Copyright © 2020 Timothy Banks. All rights reserved.
//

#include <iostream>
#include <variant>
#include <vector>

struct my_variant;
using variants = std::vector<my_variant>;
struct my_variant : public std::variant<int32_t, variants>
{
};

int main(int argc, const char * argv[])
{
    auto some_variants = std::vector<my_variant>{my_variant{1},my_variant{2}, my_variant{3}, my_variant{4}, my_variant{5}};
    auto the_variant = my_variant{some_variants};
    return 0;
}
