#ifndef CPP3DS_LINEARALLOCATOR_HPP
#define CPP3DS_LINEARALLOCATOR_HPP

#include <3ds.h>
#include <bits/c++allocator.h>
#if __cplusplus >= 201103L
#include <type_traits>
#endif

namespace cpp3ds {

	template<typename T>
	class LinearAllocator: public std::__allocator_base<T>
	{
	public:
		typedef size_t     size_type;
		typedef ptrdiff_t  difference_type;
		typedef T*         pointer;
		typedef const T*   const_pointer;
		typedef T&         reference;
		typedef const T&   const_reference;
		typedef T          value_type;

		template<typename T1>
		struct rebind
		{ typedef LinearAllocator<T1> other; };

	#if __cplusplus >= 201103L
		// _GLIBCXX_RESOLVE_LIB_DEFECTS
		// 2103. std::allocator propagate_on_container_move_assignment
		typedef std::true_type propagate_on_container_move_assignment;
	#endif

		LinearAllocator() throw() { }

		LinearAllocator(const LinearAllocator& __a) throw()
				: std::__allocator_base<T>(__a) { }

		template<typename T1>
		LinearAllocator(const LinearAllocator<T1>&) throw() { }

		~LinearAllocator() throw() { }

		// NB: __n is permitted to be 0.  The C++ standard says nothing
		// about what the return value is when __n == 0.
		pointer
		allocate(size_type __n, const void* = 0)
		{
			if (__n > this->max_size())
				std::__throw_bad_alloc();
			return static_cast<T*>(linearAlloc(__n * sizeof(T)));
		}

		// __p is not permitted to be a null pointer.
		void
		deallocate(pointer __p, size_type)
		{
			linearFree(__p);
		}

		size_type
		max_size() const
		{
			return size_t(-1) / sizeof(T);
		}

		// Inherit everything else.
	};

} // namespace cpp3ds

#endif // CPP3DS_LINEARALLOCATOR_HPP


////////////////////////////////////////////////////////////
/// \class cpp3ds::LinearAllocator
/// \ingroup system
///
/// This allocator class is useful for when you want to use a STL
/// container (e.g. std::vector) that allocates everything using ctrulib's linear memory.
///
/// \see cpp3ds::VertexArray
////////////////////////////////////////////////////////////