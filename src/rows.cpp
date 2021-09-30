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
    return ConstIterator(*this, 0);
}

ConstIterator Rows::end() const
{
    return ConstIterator(*this, size());
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

void ConstIterator::setOffset(size_t off)
{
//    if (off != m_offset) {
//        m_offset = off;
//        if (m_offset < m_rows.m_impl->m_rows.size()) {
//            m_current = std::make_shared<Row::Impl>(m_rows.m_impl->m_rows.getRow(unsigned(m_offset)));
//        }
//    }
}

ConstIterator::ConstIterator(const Rows& r, size_t off)
    : m_rows(r)
    , m_offset(off)
{
//    if (m_offset < r.m_impl->m_rows.size()) {
//        m_current = std::make_shared<Row::Impl>(r.m_impl->m_rows.getRow(unsigned(m_offset)));
//    }
}

bool ConstIterator::operator==(const ConstIterator& it) const
{
    return m_offset == it.m_offset;
}

bool ConstIterator::operator!=(const ConstIterator& it) const
{
    return !operator==(it);
}

ConstIterator& ConstIterator::operator++()
{
    setOffset(m_offset + 1);
    return *this;
}

ConstIterator ConstIterator::operator++(int)
{
    ConstIterator ret = *this;
    setOffset(m_offset + 1);
    return ret;
}

ConstIterator ConstIterator::operator--()
{
    setOffset(m_offset - 1);
    return *this;
}

ConstIterator ConstIterator::operator--(int)
{
    ConstIterator ret = *this;
    setOffset(m_offset - 1);
    return ret;
}

ConstIterator::ConstReference ConstIterator::operator*() const
{
    return m_current;
}

ConstIterator::ConstPointer ConstIterator::operator->() const
{
    return &m_current;
}

ConstIterator& ConstIterator::operator+=(difference_type n)
{
    setOffset(m_offset + size_t(n));
    return *this;
}

ConstIterator ConstIterator::operator+(difference_type n) const
{
    ConstIterator it(*this);
    it += n;
    return it;
}

ConstIterator& ConstIterator::operator-=(difference_type n)
{
    setOffset(m_offset - size_t(n));
    return *this;
}

ConstIterator ConstIterator::operator-(difference_type n) const
{
    ConstIterator it(*this);
    it -= n;
    return it;
}

ConstIterator::difference_type ConstIterator::operator-(const ConstIterator& it) const
{
    return ConstIterator::difference_type(m_offset - it.m_offset);
}

} // namespace fty::db
