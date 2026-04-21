#include <map>
#include <iostream>

#include "allocator.h"
#include "pool_allocate_strategy.h"
#include "vector.h"
#include "linear_allocate_strategy.h"

constexpr int aroundNodeSize = 48;          ///< Примерный размер ноды.
constexpr int count = 10;                   ///< Число элементов для которых аллоцируем память.


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
/// -------------------- MAP --------------------

    /// Выделили пространство под 10 элементов, но ниже будем заполнять 20-ю.
    /// Этим демонтрируется расширяемость выделенной памяти.
    PoolStrategy plStrategy{ count, aroundNodeSize };

    using MyMapAllocator = MyOwnAllocator< std::pair< const int, int >, PoolStrategy >;
    std::map< int, int, std::less< int >, MyMapAllocator > myMap( std::less< int >(), MyMapAllocator{ plStrategy } );

    for ( int i = 1; i <= 20; ++i )
    {
        myMap[ i ] = factorial( i );
    }

    std::cout << "-------------- Map data: ---------------" << std::endl;
    for ( const auto& [ key, value ] : myMap )
    {
        std::cout << key << " " << value << std::endl;
    }
    std::cout << "----------- End of map data: -----------" << std::endl;


/// -------------------- CUSTOM VECTOR --------------------

    /// Выделили 10 байт, что не хватит на 10 чисел, которыми заполнится ниже.
    /// Так же демонстрируется расширяемость.
    LinearStrategy linStrategy{ count };

    using MyVecAllocator = MyOwnAllocator< int, LinearStrategy >;
    MyOwnVector< int, MyVecAllocator > vector ( MyVecAllocator{ linStrategy } );
    for ( int i = 0; i < count; ++i )
    {
        vector.push_back( i );
    }

    std::cout << "------------- Vector data: -------------" << std::endl;
    for ( int i = 0; i < vector.size(); ++i )
    {
        std::cout << vector[ i ] << std::endl;
    }
    std::cout << "--------- End of vector data: ----------" << std::endl;

}