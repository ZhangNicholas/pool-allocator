#pragma once
#include <iostream>

#include "chunk.h"

/**
 * The allocator class.
 *
 * Features:
 *
 *   - Parametrized by number of chunks per block
 *   - Keeps track of the allocation pointer
 *   - Bump-allocates chunks
 *   - Requests a new larger block when needed
 *
 */

template<typename T, std::size_t N>
struct TPoolAllocator {
	using value_type = T;

	template <class U> struct rebind { typedef TPoolAllocator<U, N> other; };
	
	TPoolAllocator() {
		/*std::cout << "template allocator constructor" << std::endl;*/
	}

	T* allocate(std::size_t size);
	void deallocate(T* ptr, std::size_t size);

	template <class U>
		TPoolAllocator(const TPoolAllocator<U, N>& tpa) {};
	TPoolAllocator(const TPoolAllocator&) = default;
	TPoolAllocator& operator=(const TPoolAllocator&) = delete;

private:
	/**
	 * Number of chunks per larger block.
	 */
	// std::size_t mChunksPerBlock;

	/**
	 * Allocation pointer.
	 */
	Chunk* mAlloc = nullptr;

	/**
	 * Allocates a larger block (pool) for chunks.
	 */
	Chunk* allocateBlock();
};

/**
 * Allocates a new block from OS.
 *
 * Returns a Chunk pointer set to the beginning of the block.
 */
template<class T, std::size_t N>
Chunk* TPoolAllocator<T, N>::allocateBlock() {
	std::cout << "\nAllocating block (" << N << " chunks):\n\n";

	size_t blockSize = N * sizeof(T);

	// The first chunk of the new block.
	Chunk* blockBegin = reinterpret_cast<Chunk*>(malloc(blockSize));

	// Once the block is allocated, we need to chain all
	// the chunks in this block:

	Chunk* chunk = blockBegin;

	for (int i = 0; i < N - 1; ++i) {
		chunk->next =
			reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + sizeof(T));
		chunk = chunk->next;
	}

	chunk->next = nullptr;

	return blockBegin;
}

/**
 * Returns the first free chunk in the block.
 *
 * If there are no chunks left in the block,
 * allocates a new block.
 */
template<class T, std::size_t N>
T* TPoolAllocator<T, N>::allocate(size_t size) {
	// No chunks left in the current block, or no any block
	// exists yet. Allocate a new one, passing the chunk size:

	if (mAlloc == nullptr) {
		mAlloc = allocateBlock();
	}

	// The return value is the current position of
	// the allocation pointer:

	Chunk* freeChunk = mAlloc;

	// Advance (bump) the allocation pointer to the next chunk.
	//
	// When no chunks left, the `mAlloc` will be set to `nullptr`, and
	// this will cause allocation of a new block on the next request:

	mAlloc = mAlloc->next;

	return reinterpret_cast<T*>(freeChunk);
}

/**
 * Puts the chunk into the front of the chunks list.
 */
template<class T, std::size_t N>
void TPoolAllocator<T, N>::deallocate(T* chunk, size_t size) {

	// The freed chunk's next pointer points to the
	// current allocation pointer:

	reinterpret_cast<Chunk*>(chunk)->next = mAlloc;

	// And the allocation pointer is now set
	// to the returned (free) chunk:

	mAlloc = reinterpret_cast<Chunk*>(chunk);
}

template <class T, std::size_t N, class U, std::size_t M>
bool
operator==(const TPoolAllocator<T, N>& x, const TPoolAllocator<U, M>& y)
{
	return N == M && &x.a_ == &y.a_;
}

template <class T, std::size_t N, class U, std::size_t M>
bool
operator!=(const TPoolAllocator<T, N>& x, const TPoolAllocator<U, M>& y)
{
	return !(x == y);
}