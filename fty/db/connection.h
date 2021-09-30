#pragma once
#include <memory>
#include <string>
#include "row.h"
#include "rows.h"
#include "statement.h"

namespace fty::db {

// =========================================================================================================================================

std::string dburl();

class ConnectionImpl;
class Connection
{
public:
    /// Creates connection.
    Connection(const std::string& connection = dburl());
    ~Connection();

    /// Creates a new Statement-object, with the given query and stores the statement in a cache. When called again with
    /// the same query, the cached result is returned.
    /// @param sql sql query string
    /// @return statement
    Statement prepare(const std::string& sql);

public:
    ///
    template <typename... Args>
    Row selectRow(const std::string& queryStr, Args&&... args);

    template <typename... Args>
    Rows select(const std::string& queryStr, Args&&... args);

    template <typename... Args>
    uint execute(const std::string& queryStr, Args&&... args);

    int64_t lastInsertId();

    operator bool() const;

    bool reconnect();
private:
    static ConnectionImpl* createConnection(const std::string& connection);
private:
    std::shared_ptr<ConnectionImpl> m_impl;
    friend class Transaction;
};

// =========================================================================================================================================

template <typename... Args>
Row Connection::selectRow(const std::string& queryStr, Args&&... args)
{
    return prepare(queryStr).bind(std::forward<Args>(args)...).selectRow();
}

template <typename... Args>
inline Rows Connection::select(const std::string& queryStr, Args&&... args)
{
    return prepare(queryStr).bind(std::forward<Args>(args)...).select();
}

template <typename... Args>
inline uint Connection::execute(const std::string& queryStr, Args&&... args)
{
    return prepare(queryStr).bind(std::forward<Args>(args)...).execute();
}

}
