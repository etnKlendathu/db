#pragma once
#include <fty/string-utils.h>
#include <fty_log.h>
#include <mariadb/mysql.h>

namespace fty::db {

// =========================================================================================================================================

class ConnectionImpl
{
public:
    ConnectionImpl(const std::string& connection);
    virtual ~ConnectionImpl();

    bool ping() const;

    MYSQL* mysql() const;
protected:
    void closeDb();
protected:
    MYSQL* m_db = nullptr;
};

// =========================================================================================================================================

} // namespace fty::db
