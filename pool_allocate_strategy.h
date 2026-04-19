#pragma once

/// @file
/// 
/// Класс стратегии выделения памяти для связных контейнеров.
///

#include <cstddef>
#include <cassert>
#include <new>

#include "log.h"


class PoolStrategy
{

    /// @brief Структура, соделжащая указатель на следующую свободную ноду.
    struct FreeNode
    {
        FreeNode* next;
    };


    /// @brief Структура одного блока памяти.
    struct alignas( std::max_align_t ) ChunkHeader
    {
        ChunkHeader* nextChnk;              ///< Указатель на следующий блок.
    };

    std::size_t nodeSize_;                  ///< Размер ноды.
    std::size_t nodesPerChnk_;              ///< Кол-во нод, размещаемых в одном блоке памяти.
    FreeNode* freeList_ = nullptr;          ///< Указатель на свободные ноды в блоке.
    ChunkHeader* chnkHead_ = nullptr;       ///< Голова списка всех выделенных блоков памяти.

public:

    /// @brief Конструктор.
    /// @param[in] capacity Кол-во элементов, для которых нужно выделить память.
    /// @param[in] objSize Размер одного элемента.
    explicit PoolStrategy( std::size_t capacity, std::size_t objSize )
        : nodesPerChnk_{ capacity }
    {
        std::size_t alignment = alignof( std::max_align_t );
        std::size_t size = ( objSize < sizeof( FreeNode ) ) ? sizeof( FreeNode ) : objSize;
        nodeSize_ = ( size + alignment - 1 ) & ~( alignment - 1 );
        init();
    };

    /// @brief Деструктор.
    ~PoolStrategy()
    {
        while ( chnkHead_ )
        {
            ChunkHeader* temp = chnkHead_;
            chnkHead_ = chnkHead_->nextChnk;

            ::operator delete( temp );
        }
    }

    /// @brief Инициализация стратегии выделения памяти.
    void init()
    {
        if ( chnkHead_ )
        {
            return;
        }
        void* raw = ::operator new( sizeof( ChunkHeader ) + ( nodesPerChnk_ * nodeSize_ ) );
        chnkHead_ = static_cast< ChunkHeader* >( raw );
        chnkHead_->nextChnk = nullptr;

        freeList_ = reinterpret_cast< FreeNode* >( static_cast< char* >( raw ) + sizeof( ChunkHeader ) );
        FreeNode* current = freeList_;
        LOG( "Node size: " << nodeSize_ << " Nodes per chunk: " << nodesPerChnk_ );

        for ( std::size_t i = 0; i < nodesPerChnk_ - 1; ++i )
        {
            char* nextPtr = reinterpret_cast< char* >( current ) + nodeSize_;
            current->next = reinterpret_cast< FreeNode* >( nextPtr );
            current = current->next;
        }
        current->next = nullptr;
    }

    /// @brief Аллоцировать память.
    /// @param[in] countObj Кол-во объектов на которые нужно аллоцировать память.
    /// @param[in] size Неиспользуемый параметр.
    /// @return Указатель на начало аллоцированной памяти.
    void* allocate( std::size_t countObj, [[maybe_unused]] std::size_t size )
    {
        assert( countObj == 1 && "The pool strategy is not intended for arrays" );
        if ( !freeList_ )
        {
            LOG( "FreeList is not free" );
            CreateNewChunk();
        }

        void* ptr = freeList_;
        freeList_ = freeList_->next;
        LOG( "Ptr allocated: " << ptr );
        return ptr;
    }

    /// @brief Деаллоцировать память.
    /// @param[in] ptr Указатель на память, которую нужно освободить.
    void deallocate( void* ptr )
    {
        if ( !ptr )
        {
            return;
        }

        FreeNode* node = static_cast< FreeNode* >( ptr );
        node->next = freeList_;
        freeList_ = node;
        LOG( "Ptr deallocated: " << ptr );
    }

private:

    /// @brief Создать новый чанк.
    void CreateNewChunk()
    {
        void* raw = ::operator new( sizeof( ChunkHeader ) + ( nodesPerChnk_ * nodeSize_ ) );
        ChunkHeader* newChnk = static_cast< ChunkHeader* >( raw );
        newChnk->nextChnk = chnkHead_;
        chnkHead_ = newChnk;
        LOG( "New chunk created.\n " << "\tAddress:" << newChnk );
        freeList_ = reinterpret_cast< FreeNode* >( static_cast< char* >( raw ) + sizeof( ChunkHeader ) );
        FreeNode* current = freeList_;

        for ( std::size_t i = 0; i < nodesPerChnk_ - 1; ++i )
        {
            char* nextPtr = reinterpret_cast< char* >( current ) + nodeSize_;
            current->next = reinterpret_cast< FreeNode* >( nextPtr );
            current = current->next;
        }
        current->next = nullptr;
    }
};