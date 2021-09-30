#pragma once
#include "query.h"
#include <fty/flags.h>
#include <fty/string-utils.h>
#include <regex>
#include <string>
#include <iostream>
#include <fty/db/utils.h>

namespace fty::orm {

#define DBFIELD(name, options)                                                                                                             \
    {                                                                                                                                      \
        name, options                                                                                                                      \
    }

#define ORM_META(tableName, ...)                                                                                                           \
public:                                                                                                                                    \
    inline static std::vector<std::string> staticFieldNames()                                                                              \
    {                                                                                                                                      \
        return fty::split(#__VA_ARGS__, std::regex(",?\\s+"));                                                                             \
    }                                                                                                                                      \
    static constexpr const char* m_tableName = tableName

enum class Option
{
    Autoincrement,
    NotNull
};

template <typename T>
class Field
{
public:
    constexpr Field(const char* name, Option opt)
        : m_name(name)
        , m_option(opt)
    {
    }

    constexpr const char* name() const
    {
        return m_name;
    }

    Condition operator == (const T& val) const
    {
        return Condition(m_name, Condition::Operator::Equal, val);
    }

    Condition operator == (const fty::db::details::Arg& val) const
    {
        return Condition(m_name, Condition::Operator::Equal, val);
    }

    Condition operator == (std::nullptr_t) const
    {
        return Condition(m_name, Condition::Operator::Equal, nullptr);
    }

private:
    const char* m_name;
    Option      m_option;
};

using Int32 = Field<int32_t>;
using String = Field<std::string>;

template<typename Tab>
class Table
{
public:
    template <typename... T>
    static Select select(const T&... fields)
    {
        std::vector<std::string> names = {fields.name()...};
        std::cerr << implode(names, ", ") << std::endl;
        return Select(names, Tab::m_tableName);
    }
};

} // namespace fty::orm

ENABLE_FLAGS(fty::orm::Option)
