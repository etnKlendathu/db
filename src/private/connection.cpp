#include "connection.h"
#include "fty/db/connection.h"
#include <filesystem>
#include <unistd.h>

namespace fty::db {

// =========================================================================================================================================

struct Params
{
    std::optional<std::string> host;
    std::optional<std::string> user;
    std::optional<std::string> passwd;
    std::optional<std::string> db;
    std::optional<uint32_t>    port = 0;
    std::optional<std::string> socket;

    static Params parse(const std::string& connection)
    {
        Params                   params;
        std::vector<std::string> chunks = split(connection, ";");
        for (const std::string& it : chunks) {
            auto [key, val] = split<std::string, std::string>(it, "=");
            if (key == "host") {
                params.host = val;
            } else if (key == "user") {
                params.user = val;
            } else if (key == "password") {
                params.passwd = val;
            } else if (key == "db") {
                params.db = val;
            } else if (key == "port") {
                params.port = convert<uint32_t>(val);
            } else if (key == "unix_socket") {
                params.socket = val;
            } else {
                logWarn("Inknown connection param: {}", key);
            }
        }
        return params;
    }
};

// =========================================================================================================================================

bool ConnectionImpl::ping() const
{
    assert(m_db);
    if (m_db) {
        return mysql_ping(m_db) == 0;
    }
    return false;
}

ConnectionImpl::~ConnectionImpl()
{
    closeDb();
}

void ConnectionImpl::closeDb()
{
    if (m_db) {
        logDebug("close mysql {}", long(m_db));
        mysql_close(m_db);
        mysql_thread_end();
        m_db = nullptr;
    }
}

ConnectionImpl::ConnectionImpl(const std::string& connection)
{
    Params params = Params::parse(connection);

    mysql_thread_init();
    if (MYSQL* my = mysql_init(nullptr); my) {
        // clang-format off
        MYSQL* ret = mysql_real_connect(my,
            params.host   ? params.host->c_str()   : nullptr,
            params.user   ? params.user->c_str()   : nullptr,
            params.passwd ? params.passwd->c_str() : nullptr,
            params.db     ? params.db->c_str()     : nullptr,
            params.port   ? *params.port           : 0,
            params.socket ? params.socket->c_str() : nullptr,
            0
        );
        // clang-format on
        if (ret) {
            m_db = ret;
            logDebug("open mysql {}", long(m_db));
        } else {
            throw Error(mysql_error(my));
        }
    } else {
        throw Error("Cannot init mysql database");
    }
}

MYSQL* ConnectionImpl::mysql() const
{
    return m_db;
}

// =========================================================================================================================================

} // namespace fty::db
