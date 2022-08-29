//
//  main.cpp
//  type_erasure
//
//  Created by Timothy Banks on 7/30/20.
//  Copyright © 2020 Timothy Banks. All rights reserved.
//

#include <memory>
#include <iostream>

#include <boost/pool/pool.hpp>


int main(int argc, const char * argv[])
{
    auto pool = boost::pool<>(sizeof(char));
    
    auto* key = pool.ordered_malloc(12);
    auto size = sizeof(key);
    memcpy(key, "hello world", 11);
    
    auto* string_view = reinterpret_cast<char*>(key);
    
    return 0;
}
