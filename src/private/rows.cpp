#include "rows.h"

namespace fty::db {

RowsImpl::RowsImpl(MYSQL_RES* result):
    m_res(result, &RowsImpl::close)
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

}
