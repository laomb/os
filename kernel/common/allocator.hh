#ifndef COMMON_ALLOCATOR_HH
#define COMMON_ALLOCATOR_HH

#include <concepts.hh>

namespace common {

template <typename P>
concept AllocationPolicy = requires(usize n, void* p) {
	{ P::allocate_bytes(n) } -> SameAs<void*>;
	{ P::deallocate_bytes(p, n) };
};

template <typename T, AllocationPolicy Policy>
class allocator {
  public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using size_type = usize;
	using difference_type = isize;

	template <typename U> using rebind = allocator<U, Policy>;

	constexpr allocator() noexcept = default;
	constexpr allocator(const allocator&) noexcept = default;
	template <typename U>
	constexpr allocator(const allocator<U, Policy>&) noexcept {}

	pointer allocate(size_type n) {
		void* raw = Policy::allocate_bytes(n * sizeof(T));
		return static_cast<pointer>(raw);
	}

	void deallocate(pointer p, size_type n) noexcept {
		Policy::deallocate_bytes(static_cast<void*>(p), n * sizeof(T));
	}

	template <typename U, typename... Args>
	void construct(U* p, Args&&... args) {
		::new (static_cast<void*>(p)) U(static_cast<Args&&>(args)...);
	}

	template <typename U> void destroy(U* p) noexcept { p->~U(); }

	static constexpr size_type max_size() noexcept {
		return Policy::max_bytes() / sizeof(T);
	}
};

template <typename T1, typename P, typename T2>
constexpr bool operator==(allocator<T1, P>, allocator<T2, P>) noexcept {
	return true;
}
template <typename A, typename B>
constexpr bool operator!=(const A& a, const B& b) noexcept {
	return !(a == b);
}

} // namespace common

#endif // COMMON_ALLOCATOR_HH
