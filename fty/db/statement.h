#pragma once
#include "fty/db/row.h"
#include "fty/db/rows.h"
#include "fty/db/utils.h"
#include <fmt/format.h>
#include <memory>

namespace fty::db {

// =========================================================================================================================================

class StatementImpl;

class Statement
{
public:
    ~Statement();

    template <typename T>
    Statement& bind(const std::string& name, const T& value);

    template <typename TArg, typename... TArgs>
    Statement& bind(TArg&& val, TArgs&&... args);

    template <typename TArg>
    Statement& bind(Arg<TArg>&& arg);

    template <typename TArg, typename... TArgs>
    Statement& bindMulti(size_t count, TArg&& val, TArgs&&... args);

    template <typename TArg>
    Statement& bindMulti(size_t count, Arg<TArg>&& arg);

    Statement& bind();

protected:
    void set(const std::string& name, const std::string& val);
    void set(const std::string& name, bool val);
    void set(const std::string& name, int8_t val);
    void set(const std::string& name, uint8_t val);
    void set(const std::string& name, int16_t val);
    void set(const std::string& name, uint16_t val);
    void set(const std::string& name, int32_t val);
    void set(const std::string& name, uint32_t val);
    void set(const std::string& name, int64_t val);
    void set(const std::string& name, uint64_t val);
    void set(const std::string& name, float val);
    void set(const std::string& name, double val);
    void setNull(const std::string& name);

public:
    Row  selectRow() const;
    Rows select() const;
    uint64_t execute() const;

private:
    std::shared_ptr<StatementImpl> m_impl;
    Statement(const std::shared_ptr<StatementImpl>& impl);
    friend class Connection;
};

// =========================================================================================================================================

//template <typename T>
//std::optional<std::decay_t<T>> nullable(bool cond, T&& value)
//{
//    if (cond) {
//        return std::optional<std::decay_t<T>>(value);
//    }
//    return std::nullopt;
//}

// =========================================================================================================================================

template <typename T>
inline Statement& Statement::bind(const std::string& name, const T& value)
{
    set(name, value);
    return *this;
}

template <typename TArg, typename... TArgs>
inline Statement& Statement::bind(TArg&& val, TArgs&&... args)
{
    bind(std::forward<TArg>(val));
    bind(std::forward<TArgs>(args)...);
    return *this;
}

template <typename TArg>
inline Statement& Statement::bind(Arg<TArg>&& arg)
{
    if (arg.isNull) {
        setNull(arg.name.data());
    } else {
        set(arg.name.data(), arg.value);
    }
    return *this;
}

template <typename TArg, typename... TArgs>
inline Statement& Statement::bindMulti(size_t count, TArg&& val, TArgs&&... args)
{
    bindMulti(count, std::forward<TArg>(val));
    bindMulti(count, std::forward<TArgs>(args)...);
    return *this;
}

template <typename TArg>
inline Statement& Statement::bindMulti(size_t count, Arg<TArg>&& arg)
{
    if (arg.isNull) {
        setNull(fmt::format("{}_{}", arg.name, count));
    } else {
        set(fmt::format("{}_{}", arg.name, count), arg.value);
    }
    return *this;
}

} // namespace fty::db
