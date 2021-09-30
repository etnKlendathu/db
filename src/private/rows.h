#pragma once
#include <mysql.h>
#include <memory>

namespace fty::db {

class RowsImpl
{
public:
    RowsImpl(MYSQL_RES*);
    ~RowsImpl();

    size_t columnCount() const;
    size_t rowsCount() const;
private:
    static void close(MYSQL_RES* res);
private:
    std::unique_ptr<MYSQL_RES, decltype(&close)> m_res;
};

}
