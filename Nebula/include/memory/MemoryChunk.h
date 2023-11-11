//
// Created by michal-swiatek on 11.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef MEMORYCHUNK_H
#define MEMORYCHUNK_H

#include <array>
#include <cstddef>

namespace nebula::memory::impl {

    class MemoryChunk
    {
    public:
        explicit MemoryChunk(std::size_t size);
        ~MemoryChunk();

        MemoryChunk(const MemoryChunk&) = delete;
        MemoryChunk& operator = (const MemoryChunk&) = delete;

        MemoryChunk(MemoryChunk&& rhs) noexcept;
        MemoryChunk& operator = (MemoryChunk&& rhs) noexcept;

        [[nodiscard]] void* getAddress() const { return m_chunk; }
        [[nodiscard]] std::size_t getSize() const { return m_size; }

    private:
        void* m_chunk = nullptr;
        std::size_t m_size = 0;
    };

    template <std::size_t Size>
    class ScopedMemoryChunk
    {
    public:
        ScopedMemoryChunk(const ScopedMemoryChunk&) = delete;
        ScopedMemoryChunk& operator = (const ScopedMemoryChunk&) = delete;

        [[nodiscard]] void* getAddress() const { return reinterpret_cast<void*>(m_chunk.data()); }
        [[nodiscard]] std::size_t getSize() const { return Size; }

    private:
        std::array<std::byte, Size> m_chunk;
    };

}

#endif //MEMORYCHUNK_H
