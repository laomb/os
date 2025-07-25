#ifndef COMMON_TUPLE_HH
#define COMMON_TUPLE_HH

#include <concepts.hh>

namespace common {

namespace detail {

template <typename T, T V> struct integral_constant {
	static constexpr T value = V;
	using m_valuetype = T;
	using type = integral_constant;
};

} // namespace detail

template <typename... Ts> class tuple;

template <> class tuple<> {
  public:
	constexpr tuple() noexcept = default;
};

template <typename T, typename... Ts>
class tuple<T, Ts...> : private tuple<Ts...> {
	using Base = tuple<Ts...>;
	T m_value;

  public:
	constexpr tuple()
		requires(DefaultInitializable<T> && (DefaultInitializable<Ts> && ...))
		: Base(), m_value() {}

	constexpr explicit tuple(const T& t, const Ts&... ts)
		: Base(ts...), m_value(t) {}

	constexpr explicit tuple(T&& t, Ts&&... ts)
		: Base(static_cast<Ts&&>(ts)...), m_value(static_cast<T&&>(t)) {}

	template <usize I> constexpr auto& get() & {
		if constexpr (I == 0)
			return m_value;
		else
			return Base::template get<I - 1>();
	}

	template <usize I> constexpr auto const& get() const& {
		if constexpr (I == 0)
			return m_value;
		else
			return Base::template get<I - 1>();
	}

	template <usize I> constexpr auto&& get() && {
		if constexpr (I == 0)
			return static_cast<T&&>(m_value);
		else
			return static_cast<Base&&>(*this).template get<I - 1>();
	}
};

template <usize I, typename... Ts>
constexpr decltype(auto) get(tuple<Ts...>& t) {
	return t.template get<I>();
}
template <usize I, typename... Ts>
constexpr decltype(auto) get(tuple<Ts...> const& t) {
	return t.template get<I>();
}
template <usize I, typename... Ts>
constexpr decltype(auto) get(tuple<Ts...>&& t) {
	return static_cast<tuple<Ts...>&&>(t).template get<I>();
}

template <typename Tuple> struct tuple_size;
template <typename... Ts>
struct tuple_size<tuple<Ts...>>
	: detail::integral_constant<usize, sizeof...(Ts)> {};
template <typename Tuple>
static constexpr usize tuple_size_v = tuple_size<Tuple>::value;

template <usize I, typename Tuple> struct tuple_element;
template <usize I, typename T, typename... Ts>
struct tuple_element<I, tuple<T, Ts...>> : tuple_element<I - 1, tuple<Ts...>> {
};
template <typename T, typename... Ts> struct tuple_element<0, tuple<T, Ts...>> {
	using type = T;
};
template <usize I, typename... Ts>
using tuple_element_t = typename tuple_element<I, tuple<Ts...>>::type;

template <typename... Ts> tuple(Ts...) -> tuple<Ts...>;

} // namespace common

#endif // COMMON_TUPLE_HH
