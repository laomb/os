#ifndef COMMON_CONCEPTS_HH
#define COMMON_CONCEPTS_HH

namespace common {

namespace detail {

template <bool B> struct bool_constant {
	static constexpr bool value = B;
};
using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

template <typename T> struct remove_reference {
	using type = T;
};
template <typename T> struct remove_reference<T&> {
	using type = T;
};
template <typename T> struct remove_reference<T&&> {
	using type = T;
};
template <typename T> using remove_reference_t = typename remove_reference<T>::type;

template <typename T> struct remove_cv {
	using type = T;
};
template <typename T> struct remove_cv<const T> {
	using type = T;
};
template <typename T> struct remove_cv<volatile T> {
	using type = T;
};
template <typename T> struct remove_cv<const volatile T> {
	using type = T;
};
template <typename T> using remove_cv_t = typename remove_cv<T>::type;

template <typename T> using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;

template <typename A, typename B> struct is_same : false_type {};
template <typename A> struct is_same<A, A> : true_type {};
template <typename A, typename B> static constexpr bool is_same_v = is_same<A, B>::value;

template <typename T> struct is_integral : false_type {};
template <> struct is_integral<bool> : true_type {};
template <> struct is_integral<char> : true_type {};
template <> struct is_integral<signed char> : true_type {};
template <> struct is_integral<unsigned char> : true_type {};
template <> struct is_integral<short> : true_type {};
template <> struct is_integral<unsigned short> : true_type {};
template <> struct is_integral<int> : true_type {};
template <> struct is_integral<unsigned int> : true_type {};
template <> struct is_integral<long> : true_type {};
template <> struct is_integral<unsigned long> : true_type {};
template <> struct is_integral<long long> : true_type {};
template <> struct is_integral<unsigned long long> : true_type {};
template <typename T> static constexpr bool is_integral_v = is_integral<remove_cvref_t<T>>::value;

template <typename T> struct is_signed_integral : false_type {};
template <> struct is_signed_integral<signed char> : true_type {};
template <> struct is_signed_integral<short> : true_type {};
template <> struct is_signed_integral<int> : true_type {};
template <> struct is_signed_integral<long> : true_type {};
template <> struct is_signed_integral<long long> : true_type {};
template <typename T>
static constexpr bool is_signed_integral_v = is_signed_integral<remove_cvref_t<T>>::value;

template <typename T> struct is_floating_point : false_type {};
template <> struct is_floating_point<float> : true_type {};
template <> struct is_floating_point<double> : true_type {};
template <> struct is_floating_point<long double> : true_type {};
template <typename T>
static constexpr bool is_floating_point_v = is_floating_point<remove_cvref_t<T>>::value;

template <typename T> struct is_const : false_type {};
template <typename T> struct is_const<const T> : true_type {};
template <typename T> static constexpr bool is_const_v = is_const<remove_reference_t<T>>::value;

} // namespace detail

template <typename A, typename B>
concept SameAs = detail::is_same_v<A, B>;

template <typename From, typename To>
concept ConvertibleTo = requires(From&& f) { static_cast<To>(f); };

template <typename Derived, typename Base>
concept DerivedFrom = ConvertibleTo<Derived*, Base*> && ConvertibleTo<const Derived*, const Base*>;

template <typename T>
concept Integral = detail::is_integral_v<T>;

template <typename T>
concept UnsignedIntegral = Integral<T> && !detail::is_signed_integral_v<T>;

template <typename T>
concept SignedIntegral = Integral<T> && detail::is_signed_integral_v<T>;

template <typename T>
concept FloatingPoint = detail::is_floating_point_v<T>;

template <typename T>
concept Arithmetic = Integral<T> || FloatingPoint<T>;

template <typename T>
concept Const = detail::is_const_v<T>;

template <typename T>
concept EqualityComparable = requires(const T& a, const T& b) {
	{ a == b } -> ConvertibleTo<bool>;
	{ a != b } -> ConvertibleTo<bool>;
};

template <typename T>
concept TotallyOrdered = EqualityComparable<T> && requires(const T& a, const T& b) {
	{ a < b } -> ConvertibleTo<bool>;
	{ a > b } -> ConvertibleTo<bool>;
	{ a <= b } -> ConvertibleTo<bool>;
	{ a >= b } -> ConvertibleTo<bool>;
};

template <typename T>
concept DefaultInitializable = requires { T{}; };

template <typename T>
concept CopyConstructible = requires(const T& x) { T(x); };

template <typename T>
concept MoveConstructible = requires(T&& x) { T(static_cast<T &&>(x)); };

template <typename T>
concept CopyAssignable = requires(T& x, const T& y) { x = y; };

template <typename T>
concept MoveAssignable = requires(T& x, T&& y) { x = static_cast<T &&>(y); };

template <typename T>
concept Destructible = requires(T& x) { x.~T(); };

template <typename T>
concept SemiRegular =
	DefaultInitializable<T> && CopyConstructible<T> && CopyAssignable<T> && Destructible<T>;

template <typename T>
concept Movable = SemiRegular<T> && MoveConstructible<T> && MoveAssignable<T>;

} // namespace common

#endif // COMMON_CONCEPTS_HH
