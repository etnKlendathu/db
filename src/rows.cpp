#include "fty/db/rows.h"
#include "private/rows.h"

namespace fty::db {

Rows::Rows()
{
}

Rows::~Rows()
{
}

ConstIterator Rows::begin() const
{
    return ConstIterator(*this);
}

ConstIterator Rows::end() const
{
    return ConstIterator(nullptr);
}

size_t Rows::size() const
{
    return m_impl->rowsCount();
}

bool Rows::empty() const
{
    return m_impl->rowsCount() == 0;
}

Row Rows::operator[](size_t off) const
{
    //return Row(std::make_unique<Row::Impl>(m_impl->m_rows.getRow(unsigned(off))));
    return {};
}

Rows::Rows(std::shared_ptr<RowsImpl> impl)
    : m_impl(impl)
{
}

// =========================================================================================================================================

ConstIterator::ConstIterator(const Rows& rows)
    : m_rows(rows)
{
}

bool ConstIterator::operator==(const ConstIterator& it) const
{
    return m_current == it.m_current;
}

bool ConstIterator::operator!=(const ConstIterator& it) const
{
    return !operator==(it);
}

ConstIterator& ConstIterator::operator++()
{
    m_current = Row(m_rows.m_impl->fetch());
    return *this;
}

ConstIterator::ConstReference ConstIterator::operator*() const
{
    return m_current;
}

ConstIterator::ConstPointer ConstIterator::operator->() const
{
    return &m_current;
}


} // namespace fty::db
