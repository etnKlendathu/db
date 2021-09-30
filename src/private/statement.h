#pragma once
#include "bind.h"
#include "fty/db/rows.h"
#include "fty/db/utils.h"
#include <algorithm>
#include <memory>
#include <mysql.h>
#include <string>
#include <vector>

namespace fty::db {

class ConnectionImpl;

class StatementImpl
{
public:
    StatementImpl(const ConnectionImpl& conn, const std::string& sql);
    virtual ~StatementImpl() = default;

    virtual uint64_t execute() const = 0;
    virtual Rows     select() const  = 0;

public:
    template <typename T>
    void set(const std::string& name, const T& val);

protected:
    std::string m_query;
    MYSQL*      m_conn;
};

// =========================================================================================================================================

class EmptyStatement : public StatementImpl
{
public:
    EmptyStatement(const ConnectionImpl& conn, const std::string& sql);

    uint64_t execute() const override;
    Rows     select() const override;
};

// =========================================================================================================================================

class PreparedStatement : public StatementImpl
{
public:
    PreparedStatement(const ConnectionImpl& conn, const std::string& sql, const std::vector<std::string>& placeHolders);

    template <typename T>
    void set(const std::string& name, const T& val)
    {
        for (size_t index = 0; index < m_placeHolders.size(); ++index) {
            if (m_placeHolders[index] != name) {
                continue;
            }

            m_binds[index].set(val);
        }
    }

    uint64_t execute() const override;
    Rows     select() const override;

private:
    static void close(MYSQL_STMT* stmt);

private:
    std::vector<std::string>                      m_placeHolders;
    std::unique_ptr<MYSQL_STMT, decltype(&close)> m_stmt;
    Binds                                         m_binds;
};

// =========================================================================================================================================

std::string                    parseQuery(const std::string& sql, std::vector<std::string>& placeHolders);
std::shared_ptr<StatementImpl> createStatement(const ConnectionImpl& conn, const std::string& sql);

// =========================================================================================================================================

template <typename T>
void StatementImpl::set(const std::string& name, const T& val)
{
    if (auto it = dynamic_cast<PreparedStatement*>(this)) {
        it->set(name, val);
    } else {
        throw Error(Error::Code::Error, "Query is not bindable");
    }
}

} // namespace fty::db
