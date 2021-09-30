#include "fty/db/embedded.h"
#include <filesystem>
#include <iostream>
#include <mariadb/mysql.h>
#include <thread>
#include <unistd.h>

namespace fty::db {

// =====================================================================================================================

class CharArray
{
public:
    template <typename... Args>
    CharArray(const Args&... args)
    {
        add(args...);
        m_data.push_back(nullptr);
    }

    CharArray(const CharArray&) = delete;

    ~CharArray()
    {
        for (size_t i = 0; i < m_data.size(); i++) {
            delete[] m_data[i];
        }
    }

    template <typename... Args>
    void add(const std::string& arg, const Args&... args)
    {
        add(arg);
        add(args...);
    }

    void add(const std::string& str)
    {
        char* s = new char[str.size() + 1];
        memset(s, 0, str.size() + 1);
        strncpy(s, str.c_str(), str.size());
        m_data.push_back(s);
    }

    char** data()
    {
        return m_data.data();
    }

    size_t size() const
    {
        return m_data.size();
    }

private:
    std::vector<char*> m_data;
};

// =====================================================================================================================

Expected<std::string> Embedded::create(const std::string& db)
{
    std::stringstream ss;
    ss << getpid();
    std::string pid = ss.str();

    m_path = "/tmp/mysql-" + pid;
    std::string sock = "/tmp/mysql-" + pid + ".sock";

    if (!std::filesystem::create_directory(m_path)) {
        return unexpected("cannot create db dir {}", m_path);
    }

    CharArray options("mysql_test", "--datadir=" + m_path, "--socket=" + sock, "--bootstrap", "--default-storage-engine=innodb");
    CharArray groups("libmysqld_server", "libmysqld_client");

    mysql_library_init(int(options.size()) - 1, options.data(), groups.data());

    std::string sql   = fmt::format("CREATE DATABASE IF NOT EXISTS {} character set utf8 collate utf8_general_ci;", db);
    auto        mysql = mysql_init(nullptr);
    if (mysql) {
        if (mysql_real_connect(mysql, nullptr, nullptr, nullptr, nullptr, 0, sock.c_str(), 0)) {
            if (mysql_real_query(mysql, sql.c_str(), sql.size())) {
                mysql_close(mysql);
                return unexpected(mysql_error(mysql));
            }
        } else {
            mysql_close(mysql);
            return unexpected(mysql_error(mysql));
        }
        mysql_close(mysql);
    } else {
        return unexpected("cannot init db");
    }

    std::string url = fmt::format("mysql:unix_socket={};db={}", sock, db);
    setenv("DBURL", url.c_str(), 1);

    return url;
}

void Embedded::destroy()
{
    if (instance().m_inited) {
        mysql_thread_end();
        mysql_library_end();

        std::filesystem::remove_all(instance().m_path);
    }
}

Expected<void> Embedded::init(const std::string& db)
{
    if (instance().m_inited) {
        return {};
    }

    if (auto res = instance().create(db)) {
        std::string url = *res;
        setenv("DBURL", url.c_str(), 1);
    } else {
        std::cerr << res.error() << std::endl;
        return fty::unexpected(res.error());
    }

    instance().m_inited = true;
    return {};
}

Embedded& Embedded::instance()
{
    static Embedded db;
    return db;
}


} // namespace fty::db
