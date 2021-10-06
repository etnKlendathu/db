#pragma once
#include <mysql.h>
#include <memory>
#include "row.h"

namespace fty::db {

class RowsImpl
{
private:
    static void close(MYSQL_RES* res);
    using MysqlRes = std::unique_ptr<MYSQL_RES, decltype(&close)>;

public:
    RowsImpl(MysqlRes&& res);
    ~RowsImpl();

    size_t columnCount() const;
    size_t rowsCount() const;
    RowImpl fetch() const;
private:
    MysqlRes m_res;
};

}
