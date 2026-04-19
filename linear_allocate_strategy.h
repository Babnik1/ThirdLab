#pragma once

/// @file
/// 
/// Класс стратегии выделения памяти для линейных контейнеров.
///

#include <vector>
#include <new>
#include <algorithm>

#include "log.h"

class LinearStrategy
{
private:

    /// @brief Структура одного блока памяти.
    struct alignas( std::max_align_t ) ChunkHeader
    {
        ChunkHeader* nextChnk;              ///< Указатель на следующий блок.
    };

    ChunkHeader* chunks_;                   ///< Список выделенных чанков.
    char* currentChunk_;                    ///< Указатель на начало выдеденной памяти.
    std::size_t offset_;                    ///< Указатель на текущее положение ячейки, доступной для записи.
    std::size_t size_;                      ///< Размер выделенной памяти.

public:
    
    /// @brief Конструктор.
    /// @param totalSize Размер, который нужно выделить.
    explicit LinearStrategy( std::size_t totalSize )
    {
        init( totalSize );
    };

    /// @brief Деструктор.
    ~LinearStrategy()
    {
        int count = 0;
        while ( chunks_ )
        {
            ChunkHeader* temp = chunks_;
            chunks_ = chunks_->nextChnk;
            ::operator delete( temp );
            count++;
        }
        LOG( "LinearStrategy: Cleanup finished. Total chunks deleted: " << count );
    }

    /// @brief Инициализация стратегии выделения памяти.
    void init( std::size_t totalSize )
    {
        void* raw = ::operator new( sizeof( ChunkHeader ) + totalSize );
        chunks_ = static_cast< ChunkHeader* >( raw );
        chunks_->nextChnk = nullptr;

        currentChunk_ = static_cast< char* >( raw ) + sizeof( ChunkHeader );
        size_ = totalSize;
        offset_ = 0;
    }

    /// @brief Аллоцировать память.
    /// @param[in] size Размер памяти, которую мы отдаем.
    /// @return Указатель на начало аллоцированной памяти.
    void* allocate( std::size_t count, std::size_t objSize )
    {
        std::size_t sizeToAlloc = count * objSize;
        std::size_t alignment = alignof( std::max_align_t );
        sizeToAlloc = ( sizeToAlloc + alignment - 1 ) & ~( alignment - 1 );

        if ( offset_ + sizeToAlloc > size_ )
        {
            std::size_t newSize = std::max( size_ * 2, sizeToAlloc + 1024 );
            
            void* raw = ::operator new( sizeof( ChunkHeader ) + newSize );
            ChunkHeader* newChunk = static_cast< ChunkHeader* >( raw );
            
            newChunk->nextChnk = chunks_;
            chunks_ = newChunk;

            currentChunk_ = static_cast<char*>( raw ) + sizeof( ChunkHeader );
            size_ = newSize;
            offset_ = 0;
        }

        void* ptr = currentChunk_+ offset_;
        offset_ += sizeToAlloc;
        LOG( "Ptr allocated: " << ptr );
        return ptr;
    }

    /// @brief Деаллоцировать память.
    /// @param[in] ptr Указатель на память, которую нужно освободить.
    void deallocate( [[maybe_unused]] void* ptr )
    {
        /// В линейной стратегии не умеет освобождать частями память.
    }
};