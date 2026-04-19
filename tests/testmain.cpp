#include <gtest/gtest.h>

#include "allocator.h"
#include "pool_allocate_strategy.h"
#include "vector.h"
#include "linear_allocate_strategy.h"

TEST( CustomMemoryTest, MapTest )
{
    PoolStrategy plStrategy{ 10, 48 };

    using MyMapAllocator = MyOwnAllocator< std::pair< const int, int >, PoolStrategy >;
    std::map< int, int, std::less< int >, MyMapAllocator > myMap( std::less< int >(), MyMapAllocator{ plStrategy } );

    for ( int i = 1; i <= 20; ++i )
    {
        myMap[ i ] = i*i;
    }

    EXPECT_EQ( myMap.size(), 20 );
    for ( const auto& [ key, value ] : myMap )
    {
        EXPECT_EQ( value, key*key );
    }
}

TEST( CustomMemoryTest, VectorTest )
{
    LinearStrategy linStrategy{ 10 };

    using MyVecAllocator = MyOwnAllocator< int, LinearStrategy >;
    MyOwnVector< int, MyVecAllocator > vector ( MyVecAllocator{ linStrategy } );
    for ( int i = 0; i < 10; ++i )
    {
        vector.push_back( i );
    }

    EXPECT_EQ( vector.size(), 10 );
    for ( int i = 0; i < vector.size(); ++i )
    {
        EXPECT_EQ( vector [ i ], i );
    }
}

int main( int argc, char** argv )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}