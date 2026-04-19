#pragma once

/// @file
/// 
/// Класс кастомного контейнера наподоьие вектора.
///

template< typename T, class Allocator >
class MyOwnVector
{
private:
    Allocator alloc;                ///< Кастомный аллокатор.
    T* data_ = nullptr;             ///< Начало массива.
    std::size_t size_ = 0;          ///< Сколько элементов реально создано.
    std::size_t capacity_ = 0;      ///< На сколько элементов выделено памяти.

public:

    /// @brief Конструктор.
    /// @param[in] alloc Кастомный аллокатор.
    explicit MyOwnVector( const Allocator& alloc )
        : alloc{ alloc }
    {}

    /// @brief Деструктор.
    ~MyOwnVector()
    {
        clear();
    }

    /// @brief Получить размер контейнера.
    /// @return Размер.
    std::size_t size() const
    {
        return size_;
    }

    /// @brief Добавить элемент в контейнер назад.
    /// @param[in] value Элемент.
    void push_back( const T& value )
    {
        if ( size_ == capacity_ )
        {
            reserve( capacity_ == 0 ? 1 : capacity_ * 2 );
        }

        alloc.construct( &data_[ size_ ], value );
        size_++;
    }

    /// @brief Перегруженный оператор [].
    /// @param[in] index Индекс.
    /// @return Значение по индексу. 
    T& operator[]( std::size_t index ) 
    { 
        return data_[ index ]; 
    }

    /// @brief Очистить контейнер.
    void clear() 
    {
        for ( std::size_t i = 0; i < size_; ++i ) 
        {
            alloc.destroy( &data_[ i ] );
        }
        size_ = 0;
    }

    /// @brief Зарезервировать новое место.
    /// @param[in] newSize Новый размер.
    void reserve( std::size_t newSize )
    {
        T* newData = static_cast< T* >( alloc.allocate( newSize ) );
        for ( std::size_t i = 0; i < size_; ++i )
        {
            alloc.construct( &newData[ i ], std::move( data_[ i ] ) );
            alloc.destroy( &data_[ i ] );
        }

        if ( data_ )
        {
            alloc.deallocate( data_, capacity_ );
            data_ = nullptr;
        }

        data_ = newData;
        capacity_ = newSize;
    }

};