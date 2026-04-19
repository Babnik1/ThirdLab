#pragma once

/// @file
/// 
/// Класс кастомного аллокатора.
///


#include <cassert>

template< typename T, class AllocationStrategy  >
class MyOwnAllocator
{
    static_assert( !std::is_same_v< T, void >, "Type of the allocator can not be void" );
public:
    using value_type = T;

    /// @brief Объявление дружественным такого же класса для доступа к приватным полям друг друга.  
    /// @tparam U - Любой тип.
    /// @tparam AllocStrategy - Единая стратегия. 
    template< typename U, class AllocStrategy >
    friend class MyOwnAllocator;

    /// @brief Стратегия создания копии класса с единой стратегией выделения памяти.
    /// @tparam U - Любой тип.
    template< typename U >
    struct rebind
    {
        using other = MyOwnAllocator< U, AllocationStrategy >;
    };
    

public:
    MyOwnAllocator() = default;

    /// @brief Конструктор.
    /// @param[in] strategy Стратегия выделения памяти.
    explicit MyOwnAllocator( AllocationStrategy& strategy ) noexcept
            : mAllocationStrategy_{ &strategy }
    {}

    /// @brief Точный конструктор копирования. 
    /// @details <int> -> <int>.
    /// @tparam T - Тип этого класса.
    /// @param[in] other Аллокатор.
    MyOwnAllocator( const MyOwnAllocator< T, AllocationStrategy >& other ) noexcept
            : mAllocationStrategy_{ other.mAllocationStrategy_ }
    {}

    /// @brief Конструктор копирования со сменой типов.
    /// @details <int> -> <Node<int>>.
    /// @tparam U - Любой тип.
    /// @param other Аллокатор.
    template< typename U >
    MyOwnAllocator( const MyOwnAllocator< U, AllocationStrategy >& other ) noexcept
            : mAllocationStrategy_{ other.mAllocationStrategy_ }
    {}

    /// @brief Аллокация памяти для объекта.
    /// @param[in] countObj Количество объектов.
    /// @return Указатель на готовый объект.
    T* allocate( std::size_t countObj )
    {
        assert( mAllocationStrategy_ && "Not initialized allocation strategy" );
        return static_cast< T* >( mAllocationStrategy_->allocate( countObj, sizeof( T ) ) );
    }

    /// @brief Деаллокация памяти.
    /// @param[in] memoryPtr Указатель на начало памяти, которую нужно деаллоцировать.
    /// @param[in] countObj Количество объектов, которые нужно деаллоцировать.
    void deallocate( void* memoryPtr, std::size_t countObj )
    {
        assert( mAllocationStrategy_ && "Not initialized allocation strategy" );
        mAllocationStrategy_->deallocate( memoryPtr );
    }

    /// @brief Функция размещения объекта в подготовленной памяти.
    /// @tparam U - Любой тип объекта.
    /// @param[in] ptr Указатель на объект.
    /// @param[in] ...args - Аргументы.
    template< typename U, typename... Args >
    void construct( U* ptr, Args... args )
    {
        new ( reinterpret_cast< void* >( ptr ) ) U { std::forward< Args >( args )... };
    }

    /// @brief Разрушение объекта.
    /// @tparam U - Любой тип.
    /// @param ptr Указатель на объект.
    template< typename U >
    void destroy( U* ptr )
    {
        ptr->~U();
    }

private:
    AllocationStrategy* mAllocationStrategy_ = nullptr;         ///< Стратегия аллокации.

};

/// @brief Перегрузка опратора ==.
/// @return true - равность, false - неравность.
template < typename T, typename U, class AllocationStrategy >
bool operator==( const MyOwnAllocator< T, AllocationStrategy >& lhs, const MyOwnAllocator< U, AllocationStrategy >& rhs )
{
    return lhs.mAllocationStrategy_ == rhs.mAllocationStrategy_;
}

/// @brief Перегрузка опратора !=.
/// @return true - неравность, false - равность.
template < typename T, typename U, class AllocationStrategy >
bool operator!=( const MyOwnAllocator< T, AllocationStrategy >& lhs, const MyOwnAllocator< U, AllocationStrategy >& rhs )
{
    return !( lhs == rhs );
}





