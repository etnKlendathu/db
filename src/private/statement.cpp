#include "statement.h"
#include "connection.h"
#include "fty/db/utils.h"
#include "fty/db/rows.h"
#include "mysql.h"
#include "rows.h"
#include <iostream>
#include <memory>
#include <regex>

namespace fty::db {

// =========================================================================================================================================

static void freeResult(MYSQL_RES* res)
{
    mysql_free_result(res);
}

// =========================================================================================================================================

std::string parseQuery(const std::string& sql, std::vector<std::string>& placeHolders)
{
    static std::regex re(":([a-zA-Z0-9_]+)|\\{([a-zA-Z0-9_]+)\\}");
    std::smatch       match;
    std::string       tmp = sql;
    while (std::regex_search(tmp, match, re)) {
        if (match[1].length()) {
            placeHolders.push_back(match[1].str());
        } else if (match[2].length()) {
            placeHolders.push_back(match[2].str());
        }
        tmp = match.suffix();
    }
    return std::regex_replace(sql, re, "?");
}

// =========================================================================================================================================

std::shared_ptr<StatementImpl> createStatement(const ConnectionImpl& conn, const std::string& sql)
{
    std::vector<std::string> placeHolders;
    std::string              realSql = parseQuery(sql, placeHolders);
    if (placeHolders.empty()) {
        return std::make_shared<EmptyStatement>(conn, realSql);
    }
    return std::make_shared<PreparedStatement>(conn, realSql, placeHolders);
}

// =========================================================================================================================================

StatementImpl::StatementImpl(const ConnectionImpl& conn, const std::string& sql)
    : m_query(sql)
    , m_conn(conn.mysql())
{
}

// =========================================================================================================================================

EmptyStatement::EmptyStatement(const ConnectionImpl& conn, const std::string& sql)
    : StatementImpl(conn, sql)
{
}

uint64_t EmptyStatement::execute() const
{
    logDebug("Execute query: {}", m_query);
    if (mysql_real_query(m_conn, m_query.c_str(), m_query.size())) {
        throw Error(Error::Code::SqlError, mysql_error(m_conn));
    }
    return mysql_affected_rows(m_conn);
}

Rows EmptyStatement::select() const
{
    if (mysql_real_query(m_conn, m_query.c_str(), m_query.size())) {
        throw Error(Error::Code::SqlError, mysql_error(m_conn));
    }
    std::unique_ptr<MYSQL_RES, decltype(&freeResult)> result(mysql_store_result(m_conn), &freeResult);
    if (!result) {
        throw Error(Error::Code::Error, mysql_error(m_conn));
    }
    return Rows(std::make_shared<RowsImpl>(std::move(result)));
}

// =========================================================================================================================================

PreparedStatement::PreparedStatement(const ConnectionImpl& conn, const std::string& sql, const std::vector<std::string>& placeHolders)
    : StatementImpl(conn, sql)
    , m_placeHolders(placeHolders)
    , m_stmt(mysql_stmt_init(conn.mysql()), &close)
{
    if (mysql_stmt_prepare(m_stmt.get(), m_query.c_str(), m_query.size())) {
        logError("Prepare fail: {}", mysql_stmt_error(m_stmt.get()));
        throw Error(Error::Code::SqlError, mysql_stmt_error(m_stmt.get()));
    }

    unsigned long count = mysql_stmt_param_count(m_stmt.get());
    if (count) {
        m_binds.resize(count);
        logDebug("Param count: {}", count);
    }
}

void PreparedStatement::close(MYSQL_STMT* stmt)
{
    if (stmt) {
        logDebug("close mysql");
        mysql_stmt_close(stmt);
    }
}

uint64_t PreparedStatement::execute() const
{
    logDebug("Execute statement: {}", m_query);
    if (mysql_stmt_bind_param(m_stmt.get(), m_binds.data())) {
        throw Error(Error::Code::Error, mysql_stmt_error(m_stmt.get()));
    }
    if (mysql_stmt_execute(m_stmt.get())) {
        throw Error(Error::Code::SqlError, mysql_stmt_error(m_stmt.get()));
    }
    return mysql_stmt_affected_rows(m_stmt.get());
}

Rows PreparedStatement::select() const
{
    logDebug("Select statement: {}", m_query);
    if (mysql_stmt_bind_param(m_stmt.get(), m_binds.data())) {
        throw Error(Error::Code::Error, mysql_stmt_error(m_stmt.get()));
    }
    if (mysql_stmt_execute(m_stmt.get())) {
        throw Error(Error::Code::SqlError, mysql_stmt_error(m_stmt.get()));
    }
    std::unique_ptr<MYSQL_RES, decltype(&freeResult)> result(mysql_stmt_result_metadata(m_stmt.get()), &freeResult);
    if (!result) {
        throw Error(Error::Code::Error, mysql_stmt_error(m_stmt.get()));
    }
    if (mysql_stmt_store_result(m_stmt.get())) {
      throw Error(Error::Code::Error, mysql_stmt_error(m_stmt.get()));
    }
    return Rows(std::make_shared<RowsImpl>(std::move(result)));
}

// =========================================================================================================================================

} // namespace fty::db
