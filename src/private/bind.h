#pragma once
#include <mariadb/mysql.h>
#include <memory.h>
#include <string>
#include <vector>


namespace fty::db {

// =========================================================================================================================================

class Bind
{
public:
    Bind(MYSQL_BIND*);

    template <typename T>
    void set(const T& value);

private:
    MYSQL_BIND* m_data = nullptr;
};

// =========================================================================================================================================

class Binds
{
public:
    ~Binds();

    void resize(size_t size);

    Bind& operator[](size_t index);

    MYSQL_BIND* data() const;

private:
    std::vector<MYSQL_BIND> m_data;
    std::vector<Bind>       m_binds;
};

// =========================================================================================================================================

template <typename T>
struct BindValue;

template <>
struct BindValue<uint8_t>
{
    static constexpr enum_field_types type = MYSQL_TYPE_TINY;
};

template <>
struct BindValue<int8_t>
{
    static constexpr enum_field_types type = MYSQL_TYPE_TINY;
};

template <>
struct BindValue<uint16_t>
{
    static constexpr enum_field_types type = MYSQL_TYPE_SHORT;
};

template <>
struct BindValue<int16_t>
{
    static constexpr enum_field_types type = MYSQL_TYPE_SHORT;
};

template <>
struct BindValue<uint32_t>
{
    static constexpr enum_field_types type = MYSQL_TYPE_LONG;
};

template <>
struct BindValue<int32_t>
{
    static constexpr enum_field_types type = MYSQL_TYPE_LONG;
};

template <>
struct BindValue<uint64_t>
{
    static constexpr enum_field_types type = MYSQL_TYPE_LONGLONG;
};

template <>
struct BindValue<int64_t>
{
    static constexpr enum_field_types type = MYSQL_TYPE_LONGLONG;
};

template <>
struct BindValue<float>
{
    static constexpr enum_field_types type = MYSQL_TYPE_FLOAT;
};

template <>
struct BindValue<double>
{
    static constexpr enum_field_types type = MYSQL_TYPE_DOUBLE;
};

template <>
struct BindValue<bool>
{
    static constexpr enum_field_types type = MYSQL_TYPE_TINY;
};

template <>
struct BindValue<std::string>
{
    static constexpr enum_field_types type = MYSQL_TYPE_STRING;
};

template <>
struct BindValue<std::nullptr_t>
{
    static constexpr enum_field_types type = MYSQL_TYPE_NULL;
};

// =========================================================================================================================================

template <typename T>
void Bind::set(const T& value)
{
    memset(m_data, 0x00, sizeof(MYSQL_BIND));
    m_data->buffer_type   = BindValue<T>::type;
    m_data->is_null_value = false;

    if constexpr (BindValue<T>::type == MYSQL_TYPE_NULL) {
        m_data->buffer        = nullptr;
        m_data->is_unsigned   = false;
        m_data->is_null_value = true;
    } else if constexpr (BindValue<T>::type != MYSQL_TYPE_STRING) {
        m_data->buffer      = const_cast<T*>(&value);
        m_data->is_unsigned = std::is_unsigned<T>::value;
    } else {
        m_data->buffer        = const_cast<char*>(value.data());
        m_data->buffer_length = value.size();
        m_data->length_value  = value.size();
    }
}

// =========================================================================================================================================

} // namespace fty::db
