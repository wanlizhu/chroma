#ifndef CHROMA_CHROMA_API_H
#define CHROMA_CHROMA_API_H

#include "compiler.h"
#include <stddef.h>

namespace chroma {

	// It ensures the class defining the API can't be created, destroyed, 
	// or copied by the caller.
    class PUBLIC_API ChromaAPI {
	public:
		// disallow copy and assignment
		ChromaAPI(ChromaAPI const&) = delete;
		ChromaAPI(ChromaAPI&&) = delete;
		ChromaAPI& operator=(ChromaAPI const&) = delete;
		ChromaAPI& operator=(ChromaAPI&&) = delete;

		// allow placement-new allocation, don't use "noexcept", to avoid compiler null check
		static void *operator new(size_t, void* p) { return p; }

		// prevent heap allocation
		static void *operator new(size_t) = delete;
		static void *operator new[](size_t) = delete;
		static void  operator delete(void*) = delete;
		static void  operator delete[](void*) = delete;

	protected:
		// disallow creation on the stack
		ChromaAPI() noexcept = default;
		~ChromaAPI() = default;
    };

	// It is used to hide the implementation details of builders 
	// and ensure a higher level of backward binary compatibility.
	template<typename T>
	class BuilderBase {
		// their implementation must be hidden from the public headers.
		template<typename ... ARGS>
		explicit BuilderBase(ARGS&& ...) noexcept;
		BuilderBase() noexcept;
		~BuilderBase() noexcept;
		BuilderBase(BuilderBase const& rhs) noexcept;
		BuilderBase& operator = (BuilderBase const& rhs) noexcept;

		// move ctor and copy operator can be implemented inline and don't need to be exported
		BuilderBase(BuilderBase&& rhs) noexcept : mImpl(rhs.mImpl) { rhs.mImpl = nullptr; }
		BuilderBase& operator = (BuilderBase&& rhs) noexcept {
			auto temp = mImpl;
			mImpl = rhs.mImpl;
			rhs.mImpl = temp;
			return *this;
		}

	protected:
		inline T* operator->() noexcept { return mImpl; }
		inline T const* operator->() const noexcept { return mImpl; }

	protected:
		T* mImpl;
	};
}

#endif