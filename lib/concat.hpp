#pragma once

#include <tuple>
#include <iostream>
#include <type_traits>

namespace concat {

namespace detail {
    struct none {};

    struct StreamWriter {
        std::ostream& out;
        inline constexpr StreamWriter(std::ostream& out) noexcept : out(out) {}

        template <typename Arg>
        inline auto operator()(const Arg& arg) -> decltype((out << arg), none()) {
            out << arg;
            return none();
        }
    };

    template <class Arg>
    inline constexpr std::tuple<Arg> tuple_join_by(const char* sep, Arg&& arg) {
        (void)sep;
        return {std::forward<Arg>(arg)};
    }

    template <class SEP, class Arg, class... Args>
    inline constexpr auto tuple_join_by(SEP sep, Arg&& arg, Args&&...args)
    {
        return std::tuple_cat(std::make_tuple(std::forward<Arg>(arg), sep), tuple_join_by(sep, std::forward<Args>(args)...));
    }

    template<class Tuple, class Writer, std::size_t... Index>
    inline void tuple_for_each(std::index_sequence<Index...>, const Tuple& tuple, Writer&& iostream_writer) {
        (void)(none[]){
            (void(
                   /* requires operator<<(std::ostream&, const T&) */ iostream_writer(
                      std::get<Index>(tuple)
                  )
            ),
            none())...
        };
    }

    template<class Tuple, class Function>
    inline void tuple_for_each(const Tuple& tuple, Function&& function) {
        return detail::tuple_for_each(std::make_index_sequence<std::tuple_size<typename std::remove_reference<Tuple>::type>::value>(),
                                      tuple, std::forward<Function>(function));
    }
} //namespace detail

template <class... Args>
class Concat;

template <class... Args>
inline constexpr Concat<Args...> concat(Args&&... args) {
    return Concat<Args...>(std::forward<Args>(args)...);
}

template <class... Args>
inline constexpr Concat<Args...> concat(const std::tuple<Args...>& args) noexcept(std::is_nothrow_copy_constructible<std::tuple<Args...>>::value) {
    return Concat<Args...>(args);
}

template <class... Args>
inline constexpr Concat<Args...> concat(std::tuple<Args...>& args) noexcept(std::is_nothrow_copy_constructible<std::tuple<Args...>>::value) {
    return Concat<Args...>(args);
}

template <class... Args>
inline constexpr Concat<Args...> concat(std::tuple<Args...>&& args) noexcept(std::is_nothrow_move_constructible<std::tuple<Args...>>::value) {
    return Concat<Args...>(std::forward<std::tuple<Args...>>(args));
}

template <class... Args>
class Concat {
    std::tuple<Args...> args;

    public:
        inline constexpr Concat(Args&&... args) : args(std::forward<Args>(args)...) {}
        inline constexpr explicit Concat(std::tuple<Args...>&& args)
            noexcept(std::is_nothrow_move_constructible<std::tuple<Args...>>::value)
            : args(std::forward<decltype(args)>(args)) {}
        inline constexpr explicit Concat(const std::tuple<Args...>& args)
            noexcept(std::is_nothrow_copy_constructible<std::tuple<Args...>>::value)
            : args(args) {}

        ///Wraps around this with `quote`.
        template <class QUOTE>
        inline constexpr auto quote(QUOTE quote) && {
            return concat(std::tuple_cat(std::tuple<QUOTE>(quote), std::move(this->args), std::tuple<QUOTE>(quote)));
        }

        ///Performs join onto concatenated list by inserting copy of `sep` between every argument
        template <class SEP>
        inline constexpr auto join(SEP sep) && {
            return concat(detail::tuple_join_by(sep, std::move(std::get<Args>(args))...));
        }

        ///Writes arguments onto `out` stream.`
        inline std::ostream& write(std::ostream& out) const {
            detail::tuple_for_each(this->args, detail::StreamWriter(out));
            return out;
        }
};

template <class... Args>
inline std::ostream& operator<<(std::ostream& out, const Concat<Args...>& concat) {
    concat.write(out);
    return out;
}

} // namespace concat
