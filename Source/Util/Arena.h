#pragma once

#include "Util\Types.h"
#include "Util\Std.h"

namespace util {

	class BumpAllocator {
	public:
		BumpAllocator(usize size = 65536);
		~BumpAllocator();

		BumpAllocator(const BumpAllocator& other) = delete;
		BumpAllocator& operator=(const BumpAllocator& other) = delete;

		BumpAllocator(BumpAllocator&& other) noexcept {
			*this = std::move(other);
		}

		BumpAllocator& operator=(BumpAllocator&& other) noexcept;

		void* Alloc(usize size, usize align = 0);

		template<typename T>
		T* AllocArray(u32 count) {
			void* ptr = Alloc(sizeof(T) * count, alignof(T));
			return reinterpret_cast<T*>(ptr);
		}

		template<typename T>
		T* AllocCopy(T* other) {
			T* newPtr = AllocArray<T>(1);
			*newPtr = *other;
			return newPtr;
		}

		template<typename T, typename... Args>
		T* Alloc(Args&&... args) {
			return new (AllocArray<T>(1)) T(std::forward<Args>(args)...);
		}

		void Clear(); //TODO: dynamically resize based on previous frame's usage

	private:
		u8* mem;
		u8* current;
		u8* max;
	};

	template<usize L>
	class LongAllocator {
	public:
		LongAllocator(usize size = 65536) {
			for (u32 i = 0; i < L; i++) {
				allocators[i] = std::make_unique<BumpAllocator>(size);
			}
		}

		LongAllocator(const LongAllocator& other) = delete;
		LongAllocator& operator=(const LongAllocator& other) = delete;

		LongAllocator(LongAllocator&& other) noexcept {
			*this = std::move(other);
		}

		LongAllocator& operator=(LongAllocator&& other) noexcept {
			current = other.current;

			for (u32 i = 0; i < L; i++) {
				allocators[i] = std::move(other.allocators[i]);
			}
		}

		void* Alloc(usize size, usize align = 0) {
			return allocators[current]->Alloc(size, align);
		}

		template<typename T>
		T* AllocArray(u32 count) {
			return allocators[current]->AllocArray<T>(count);
		}

		template<typename T>
		T* AllocCopy(T* other) {
			return allocators[current]->AllocCopy(other);
		}

		template<typename T, typename... Args>
		T* Alloc(Args&&... args) {
			return allocators[current]->Alloc<T>(std::forward<Args>(args)...);
		}

		void Clear() {
			current = (current + 1) % L;
			allocators[current]->Clear();
		}
		
	private:
		std::unique_ptr<BumpAllocator> allocators[L];
		usize current = 0;
	};
}