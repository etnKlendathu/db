#include "rows.h"

namespace fty::db {

RowsImpl::RowsImpl(MysqlRes&& result):
    m_res(std::move(result))
{

}

RowsImpl::~RowsImpl()
{
}

void RowsImpl::close(MYSQL_RES* res)
{
    mysql_free_result(res);
}

size_t RowsImpl::columnCount() const
{
    return mysql_num_fields(m_res.get());
}

size_t RowsImpl::rowsCount() const
{
    return mysql_num_rows(m_res.get());
}

RowImpl RowsImpl::fetch() const
{
}

}
