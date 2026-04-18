#include <map>
#include <iostream>

#include "allocator.h"
#include "pool_allocate_strategy.h"
#include "vector.h"


/// @brief Создание факториала числа.
/// @param n Число из которого нужно получить факториал.
/// @return Факториал числа.
int factorial( int n )
{
    if ( n <= 1 )
    {
        return 1;
    }
    return n * factorial( n- 1 );
}

int main()
{
    PoolStrategy strategy{ 10, 48 };
    using MyAllocator = MyOwnAllocator< std::pair< const int, int >, PoolStrategy >;
    std::map< int, int, std::less< int >, MyAllocator > myMap( std::less< int >(), MyAllocator{ strategy } );

    for ( int i = 1; i <=20; ++i )
    {
        myMap[ i ] = factorial( i );
    }

    for ( const auto& [ key, value ] : myMap )
    {
        std::cout << key << " " << value << std::endl;
    }
    
}