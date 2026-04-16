#include <cstddef>

template< std::size_t CHUNK_SIZE = 16'384u >
class PoolStrategy
{

    struct FreeNode
    {
        FreeNode* next;
    };


    /// @brief Структура одного блока памяти.
    struct ChunkHeader
    {
        ChunkHeader* nextChnk;                ///< Указатель на следующий блок.
    };

    std::size_t nodeSize_;              ///< Размер ноды.
    std::size_t nodesPerChnk_;          ///< Кол-во нод, размещаемых в одном блоке памяти.
    FreeNode* freeList_ = nullptr;      ///< Указатель на свободные ноды в блоке.
    ChunkHeader* chnkHead_ = nullptr;   ///< Голова списка всех выделенных блоков памяти.

public:
    explicit PoolStrategy( std::size_t nodeSize, std::size_t nodesPerChnk = 64 )
        : nodeSize_{ nodeSize < sizeof( FreeNode ) ? sizeof( FreeNode ) : nodeSize }
        , nodesPerChnk_{ nodesPerChnk }
    {}

    ~PoolStrategy()
    {
        while ( chnkHead_ )
        {
            ChunkHeader* temp = chnkHead_;
            chnkHead_ = chnkHead_->nextChnk;

            ::operator delete( temp );
        }
    }

    void* allocate()
    {
        if ( !freeList_ )
        {
            CreateNewChunk();
        }

        void* ptr = freeList_;
        freeList_ = freeList_->next;
        return ptr;
    }

    void deallocate( void* ptr )
    {
        if ( !ptr )
        {
            return;
        }

        FreeNode* node = static_cast< FreeNode* >( ptr );
        node->next = freeList_;
        freeList_ = node;
    }

private:

    void CreateNewChunk()
    {
        std::size_t headerSize = sizeof( ChunkHeader );



    }
}