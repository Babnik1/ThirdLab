#include <map>
#include <iostream>
#include "allocator.h"
#include "pool_allocate_strategy.h"



int main()
{
    PoolStrategy strategy{ 100, 32 };

    using MyAllocator = MyOwnAllocator< std::pair< const int, int >, PoolStrategy >;
    std::map< int, int, std::less< int >, MyAllocator > myMap( std::less< int >(), MyAllocator{ strategy } );

    myMap[ 1 ] = 10;
    myMap[ 2 ] = 20;
    // myMap[ 3 ] = 30;

    for ( const auto& [ key, value ] : myMap )
    {
        std::cout << key << " " << value << std::endl;
    }
}