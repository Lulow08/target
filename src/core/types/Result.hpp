#pragma once

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
#include <expected>
template<typename T, typename E> using Result = std::expected<T, E>;
// Eliminamos la necesidad de una clase Unexpected propia en el modo estándar
template<typename E>
[[nodiscard]] auto makeUnexpected(E&& e) {
    return std::unexpected<std::decay_t<E>>(std::forward<E>(e));
}

#else

#include <variant>
#include <utility>
#include <cassert>
#include <type_traits>

template<typename E>
struct Unexpected {
    E error;
    explicit Unexpected(E e) : error(std::move(e)) {}
};

template<typename E>
[[nodiscard]] auto makeUnexpected(E&& e) {
    return Unexpected<std::decay_t<E>>{ std::forward<E>(e) };
}

template<typename T, typename E>
class Result {
public:
    Result(T val) noexcept(std::is_nothrow_move_constructible_v<T>)
        : m_data(std::in_place_index<0>, std::move(val)) {}

    Result(Unexpected<E> u) noexcept(std::is_nothrow_move_constructible_v<E>)
        : m_data(std::in_place_index<1>, std::move(u.error)) {}

    [[nodiscard]] bool has_value() const noexcept { return m_data.index() == 0; }
    [[nodiscard]] explicit operator bool() const noexcept { return has_value(); }

    [[nodiscard]] T&       value()       { assert(has_value()); return std::get<0>(m_data); }
    [[nodiscard]] const T& value() const { assert(has_value()); return std::get<0>(m_data); }
    [[nodiscard]] E&       error()       { assert(!has_value()); return std::get<1>(m_data); }
    [[nodiscard]] const E& error() const { assert(!has_value()); return std::get<1>(m_data); }

    [[nodiscard]] T* operator->()       { return &value(); }
    [[nodiscard]] const T* operator->() const { return &value(); }
    [[nodiscard]] T&       operator*()  &     { return value(); }
    [[nodiscard]] const T& operator*()  const& { return value(); }

    template<typename U>
    [[nodiscard]] T value_or(U&& default_value) const& {
        return has_value() ? value() : static_cast<T>(std::forward<U>(default_value));
    }

private:
    std::variant<T, E> m_data;
};

#endif