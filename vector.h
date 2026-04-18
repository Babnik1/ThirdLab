#include "allocator.h"


template< typename T, class Allocator >
class MyOwnVector
{
private:
    Allocator alloc;                // Кастомный аллокатор.
    T* data_ = nullptr;             // Начало массива.
    std::size_t size_ = 0;          // Сколько элементов реально создано.
    std::size_t capacity_ = 0;      // На сколько элементов выделено памяти.

public:

    explicit MyOwnVector() = default;

    ~MyOwnVector()
    {
        clear();
    }

    std::size_t size() const
    {
        return size_;
    }

    // ???
    void push_back( const T& value )
    {
        if ( size_ == capacity_ )
        {
            reserve( capacity_ == 0 : 1 : capacity_ * 2 )
        }

        alloc->construct( T, value );
        size_++;
    }

    T& operator[]( std::size_t index ) 
    { 
        return data_[ index ]; 
    }

    void clear() 
    {
        for ( std::size_t i = 0; i < size_; ++i ) 
        {
            alloc->deallocate( data_[ i ] );
        }
        size_ = 0;
    }

}