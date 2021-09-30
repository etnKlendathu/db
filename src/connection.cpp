#include "fty/db/connection.h"
#include "private/connection.h"
#include "private/statement.h"

namespace fty::db {

std::string dburl()
{
    if (getenv("DBURL") != nullptr) {
        return getenv("DBURL");
    } else {
        return fmt::format(
            "mysql:db=box_utf8;user={};password={}", getenv("DB_USER") == nullptr ? "root" : getenv("DB_USER"),
            getenv("DB_PASSWD") == nullptr ? "" : getenv("DB_PASSWD"));
    }
}

// =========================================================================================================================================

Connection::Connection(const std::string& connection)
    : m_impl(createConnection(connection))
{
}

Connection::~Connection()
{
}

Statement Connection::prepare(const std::string& sql)
{
    return Statement(createStatement(*m_impl, sql));
}

int64_t Connection::lastInsertId()
{
    return 0; // m_impl->m_connection.lastInsertId();
}

Connection::operator bool() const
{
    return m_impl && m_impl->ping();
}

// bool Connection::reconnect()
//{
//    try {
//        m_impl.reset(new Impl());
//        if (*this) {
//            return {};
//        }
//        return unexpected("Cannot reconnect to database");
//    } catch (const std::exception& ex) {
//        return unexpected(ex.what());
//    }
//}

ConnectionImpl* Connection::createConnection(const std::string& connection)
{
    if (connection.find("mysql:") == 0) {
        return new ConnectionImpl(connection.substr(6));
    }
    throw Error("Wrong connection string");
}

} // namespace fty::db
