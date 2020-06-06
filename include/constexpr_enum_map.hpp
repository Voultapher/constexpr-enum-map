#include <stdint.h>
#include <array>
#include <tuple>
#include <type_traits>

#if (__cplusplus >=201402L) and defined(CEM_NO_STD_SHIM)
#define cpp14shim std
#else
namespace cpp14shim {
    template <typename T, T... Ints>
    struct integer_sequence {
        using value_type = T;
        static constexpr size_t size() noexcept { return sizeof...(Ints); }
    };

    template <size_t... Ints>
    using index_sequence = integer_sequence<size_t, Ints...>;

    template <typename Seq, size_t SeqSize, size_t Rem>
    struct Extend;

    // Note that SeqSize == sizeof...(Ints). It's passed explicitly for efficiency.
    template <typename T, T... Ints, size_t SeqSize>
    struct Extend<integer_sequence<T, Ints...>, SeqSize, 0> {
        using type = integer_sequence<T, Ints..., (Ints + SeqSize)...>;
    };

    template <typename T, T... Ints, size_t SeqSize>
    struct Extend<integer_sequence<T, Ints...>, SeqSize, 1> {
        using type = integer_sequence<T, Ints..., (Ints + SeqSize)..., 2 * SeqSize>;
    };

    template <typename T, size_t N>
    struct Gen {
    using type =
        typename Extend<typename Gen<T, N / 2>::type, N / 2, N % 2>::type;
    };

    template <typename T>
    struct Gen<T, 0> {
        using type = integer_sequence<T>;
    };

    template <typename T, T N>
    using make_integer_sequence = typename  Gen<T, N>::type;

    template <size_t N>
    using make_index_sequence = make_integer_sequence<size_t, N>;

    // Technically wrong for certain cases of ref types.
    // Not constexpr ref prop types make a lot of sense.
    template <class... Ts>
    constexpr auto make_tuple(Ts&&... args) -> std::tuple<Ts...>
    {
        return std::tuple<Ts...>(std::forward<Ts>(args)...);
    }
} // cpp14shim
#endif

namespace detail {
    template<typename Enum, typename T, typename Impl, typename Last, size_t I>
    auto constexpr check_prop_init() -> T {
        static_assert(
          static_cast<size_t>(std::get<I>(Impl::value())._enum) != Last::value,
          "Do not use Last in prop map."
        );

        return std::get<I>(Impl::value());
    }

    template<typename T, typename Impl, size_t... Is>
    auto constexpr make_enum_property_map_impl(
        cpp14shim::index_sequence<Is...>
    ) -> std::array<T, Impl::size::value> {
        using Enum = decltype(std::declval<T>()._enum);
        using Last = std::integral_constant<
          size_t,
          static_cast<size_t>(Enum::DoNotUseMeMustBeLastEnumVariant)
        >;

        static_assert(
          std::is_unsigned<typename std::underlying_type<Enum>::type>::value,
          "Set the underlying type of the Enum to unsigned"
        );
        static_assert(
          Impl::size::value == Last::value,
          "Provide a prop for every enum variant."
        );
        // TODO check all enum values different
        // TODO sorted enums

        return std::array<T, Impl::size::value>{
            detail::check_prop_init<Enum, T, Impl, Last, Is>()...
        };
    }
} // namespace detail

/// Define constexpr enum map type, that maps an enum as key to value_type.
///
/// value_type MUST be a type that has a member called _enum of key type,
/// and it MUST be constexpr compatible.
///
/// The key enum MUST define as last variant DoNotUseMeMustBeLastEnumVariant,
/// it MUST have an unsigned underlying type and MUST NOT define enum values.
#define DEFINE_CONSTEXPR_ENUM_MAP_TYPE(value_type, ...) \
    struct value_type##_enum_map_t { \
        struct _impl { \
            using size = std::tuple_size<decltype(cpp14shim::make_tuple(__VA_ARGS__))>; \
            static constexpr auto value() -> decltype(cpp14shim::make_tuple(__VA_ARGS__)) { return cpp14shim::make_tuple(__VA_ARGS__); } \
        }; \
        \
        using map_t = std::array<value_type, _impl::size::value>; \
        \
        static constexpr auto value() -> map_t { \
            return detail::make_enum_property_map_impl<value_type, _impl>( \
                cpp14shim::make_index_sequence<_impl::size::value>() \
            ); \
        } \
    };
