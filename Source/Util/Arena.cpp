#include "Arena.h"

namespace util {

	BumpAllocator::BumpAllocator(usize size) {
		mem = reinterpret_cast<u8*>(std::malloc(size));
		current = mem;
		max = mem + size;
	}

	BumpAllocator::~BumpAllocator() {
		if (mem) {
			std::free(mem);
		}
	}

	BumpAllocator& BumpAllocator::operator=(BumpAllocator&& other) noexcept {
		if (mem) {
			std::free(mem);
		}

		mem = other.mem;
		current = other.current;
		max = other.max;

		other.mem = nullptr;

		return *this;
	}

	void* BumpAllocator::Alloc(usize size, usize align) {
		u8* aligned = (u8*)((usize)(current + align) & (~align));

		ASSERT(aligned + size < max, "Bump allocation too large!");

		current += size;
		return aligned;
	}

	void BumpAllocator::Clear() {
		current = mem;

#ifdef GAME_IS_DEBUG
		std::memset(mem, 0xFF, max - current);
#endif
	}
}