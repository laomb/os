#ifndef COMMON_SPAN_HH
#define COMMON_SPAN_HH

#include <concepts.hh>

namespace common {

template <typename T> class span {
  public:
	using element_type = T;
	using value_type = detail::remove_cv_t<T>;
	using m_sizetype = ::usize;
	using difference_type = ::isize;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using iterator = pointer;
	using const_iterator = const_pointer;

  private:
	pointer m_data;
	m_sizetype m_size;

  public:
	constexpr span() noexcept : m_data(nullptr), m_size(0) {}
	constexpr span(decltype(nullptr)) noexcept : m_data(nullptr), m_size(0) {}

	constexpr span(pointer ptr, m_sizetype count) noexcept : m_data(ptr), m_size(count) {}

	constexpr span(pointer first, pointer last) noexcept
		: m_data(first), m_size(static_cast<m_sizetype>(last - first)) {}

	template <typename U>
		requires ConvertibleTo<U*, T*>
	constexpr span(const span<U>& other) noexcept : m_data(other.data()), m_size(other.size()) {}

	constexpr pointer data() noexcept { return m_data; }
	constexpr const_pointer data() const noexcept { return m_data; }

	constexpr m_sizetype size() const noexcept { return m_size; }
	constexpr bool empty() const noexcept { return m_size == 0; }

	constexpr reference operator[](m_sizetype idx) noexcept { return m_data[idx]; }
	constexpr const_reference operator[](m_sizetype idx) const noexcept { return m_data[idx]; }

	constexpr reference at(m_sizetype idx) noexcept {
		if (idx >= m_size)
			unreachable();
		return m_data[idx];
	}
	constexpr const_reference at(m_sizetype idx) const noexcept {
		if (idx >= m_size)
			unreachable();
		return m_data[idx];
	}

	constexpr reference front() noexcept { return m_data[0]; }
	constexpr const_reference front() const noexcept { return m_data[0]; }

	constexpr reference back() noexcept { return m_data[m_size - 1]; }
	constexpr const_reference back() const noexcept { return m_data[m_size - 1]; }

	constexpr iterator begin() noexcept { return m_data; }
	constexpr const_iterator begin() const noexcept { return m_data; }

	constexpr iterator end() noexcept { return m_data + m_size; }
	constexpr const_iterator end() const noexcept { return m_data + m_size; }

	constexpr const_iterator cbegin() const noexcept { return m_data; }
	constexpr const_iterator cend() const noexcept { return m_data + m_size; }

	constexpr span<T> subspan(usize offset, usize count) const noexcept {
		if (offset > m_size || offset + count > m_size)
			unreachable();
		return span<T>(m_data + offset, count);
	}

	constexpr span<T> subspan(usize offset) const noexcept {
		if (offset > m_size)
			unreachable();
		return span<T>(m_data + offset, m_size - offset);
	}

	constexpr span<T> first(usize count) const noexcept {
		if (count > m_size)
			unreachable();
		return span<T>(m_data, count);
	}

	constexpr span<T> last(usize count) const noexcept {
		if (count > m_size)
			unreachable();
		return span<T>(m_data + (m_size - count), count);
	}

	friend constexpr bool operator==(const span& a, const span& b) noexcept {
		if (a.size() != b.size())
			return false;
		for (usize i = 0; i < a.size(); ++i)
			if (a[i] != b[i])
				return false;
		return true;
	}

	friend constexpr bool operator!=(const span& a, const span& b) noexcept { return !(a == b); }

	constexpr const span<const u8> as_bytes() const noexcept {
		return span<const u8>(reinterpret_cast<const u8*>(m_data), m_size * sizeof(T));
	}

	constexpr span<u8> as_mut_bytes() noexcept {
		return span<u8>(reinterpret_cast<u8*>(m_data), m_size * sizeof(T));
	}

	constexpr void fill(const T& value) noexcept {
		for (usize i = 0; i < m_size; ++i)
			m_data[i] = value;
	}

	constexpr void copy_from(const span<T> src) noexcept {
		for (usize i = 0; i < common::min(src.size(), m_size); ++i)
			m_data[i] = src[i];
	}
};

template <typename T, ::usize N> span(T (&)[N]) -> span<T>;

} // namespace common

#endif // COMMON_SPAN_HH
