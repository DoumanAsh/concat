#pragma once

#include <tuple>
#include <iostream>

namespace concat {

namespace detail {
    struct none {};
    template <typename Arg>
    inline auto write_stream(std::ostream& out, const Arg& arg) -> decltype((out << arg), none()) {
        out << arg;
        return none();
    }
} //namespace detail

template <class... Args>
class Concat {
    std::tuple<Args...> args;
    public:
        inline constexpr Concat(Args&&... args) : args(std::forward<Args>(args)...) {}
        inline constexpr explicit Concat(std::tuple<Args...>&& args) : args(std::forward<decltype(args)>(args)) {}
        inline constexpr explicit Concat(std::tuple<Args...>& args) : args(args) {}

        constexpr std::ostream& write(std::ostream& out) const {
            (void)(detail::none[]) {
                detail::write_stream(out, std::get<Args>(args))... /* requires operator<<(std::ostream&, const T&) */
            };
            return out;
        }
};

template <class... Args>
inline constexpr Concat<Args...> concat(Args&&... args) {
    return Concat<Args...>(std::forward<Args>(args)...);
}

template <class... Args>
inline constexpr Concat<Args...> concat(std::tuple<Args...>& args) {
    return Concat<Args...>(args);
}

template <class... Args>
inline constexpr Concat<Args...> concat(std::tuple<Args...>&& args) {
    return Concat<Args...>(std::forward<std::tuple<Args...>>(args));
}

template <class... Args>
inline std::ostream& operator<<(std::ostream& out, const Concat<Args...>& concat) {
    concat.write(out);
    return out;
}

} // namespace concat
