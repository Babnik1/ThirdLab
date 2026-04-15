#include <map>


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
    /// @param strategy Стратегия выделения памяти.
    explicit MyOwnAllocator( AllocationStrategy& strategy ) noexcept
            : m_allocation_strategy( &strategy ) 
    {}

    /// @brief Точный конструктор копирования. 
    /// @details <int> -> <int>.
    /// @tparam T - Тип этого класса.
    /// @param other Аллокатор.
    MyOwnAllocator( const MyOwnAllocator< T, AllocationStrategy >& other ) noexcept
            : m_allocation_strategy( other.m_allocation_strategy )
    {}

    /// @brief Конструктор копирования со сменой типов.
    /// @details <int> -> <Node<int>>.
    /// @tparam U - Любой тип.
    /// @param other Аллокатор.
    template< typename U >
    MyOwnAllocator( const MyOwnAllocator< U, AllocationStrategy >& other ) noexcept
            : m_allocation_strategy( other.m_allocation_strategy )
    {}

};


