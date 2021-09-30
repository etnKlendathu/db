#pragma once
#include <fty/traits.h>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <fmt/format.h>

namespace fty::db {

// =========================================================================================================================================

class Error : public std::runtime_error
{
public:
    enum class Code
    {
        Error,
        NotFound,
        NullValue,
        TypeError,
        SqlError,
        FieldNotFound
    };

    Code               code() const;

    template <typename... Args>
    Error(Code code, const std::string& message, const Args&... args)
        : Error(code, fmt::format(message, args...))
    {
    }

    template <typename... Args>
    Error(const std::string& message, const Args&... args)
        : Error(Code::Error, fmt::format(message, args...))
    {
    }

    Error(Code code, const std::string& message);
    Error(const std::string& message);

private:
    Code        m_code;
};

// =========================================================================================================================================

template <typename T>
struct Arg
{
    std::string_view name;
    T                value;
    bool             isNull = false;
};

// =========================================================================================================================================

namespace details {

    template <class, template <class> class>
    struct is_instance : public std::false_type
    {
    };

    template <class T, template <class> class U>
    struct is_instance<U<T>, U> : public std::true_type
    {
    };

    template <typename T>
    struct Types
    {
        using Type = std::decay_t<T>;
    };

    template <size_t N>
    struct Types<char[N]>
    {
        using Type = std::string;
    };

    template <>
    struct Types<const char*>
    {
        using Type = std::string;
    };

    template <typename T>
    using UseType = typename Types<T>::Type;

    struct Arg
    {
        std::string_view str;

        template <typename T>
        auto operator=(T&& value) const
        {
            if constexpr (is_instance<T, std::optional>::value) {
                if (value) {
                    return db::Arg<typename T::value_type>{str, std::forward<typename T::value_type>(*value)};
                } else {
                    return db::Arg<typename T::value_type>{str, typename T::value_type{}, true};
                }
            } else {
                return db::Arg<T>{str, std::forward<T>(value)};
            }
        }
    };

} // namespace details

// =========================================================================================================================================

constexpr details::Arg operator"" _p(const char* s, size_t n)
{
    return {{s, n}};
}

// =========================================================================================================================================

} // namespace fty::db
