#pragma once
#include "row.h"
#include <memory>

namespace fty::db {

class ConstIterator;
struct RowsImpl;

// =========================================================================================================================================

class Rows
{
public:
    Rows();
    ~Rows();

    ConstIterator begin() const;
    ConstIterator end() const;
    size_t        size() const;
    bool          empty() const;
    Row           operator[](size_t off) const;

private:
    std::shared_ptr<RowsImpl> m_impl;
    Rows(std::shared_ptr<RowsImpl> impl);
    friend class ConstIterator;
    friend class PreparedStatement;
};

// =========================================================================================================================================

class ConstIterator : public std::iterator<std::random_access_iterator_tag, Row>
{
public:
    using ConstReference = const value_type&;
    using ConstPointer   = const value_type*;

private:
    Rows   m_rows;
    Row    m_current;
    size_t m_offset;

    void setOffset(size_t off);

public:
    ConstIterator(const Rows& r, size_t off);
    bool            operator==(const ConstIterator& it) const;
    bool            operator!=(const ConstIterator& it) const;
    ConstIterator&  operator++();
    ConstIterator   operator++(int);
    ConstIterator   operator--();
    ConstIterator   operator--(int);
    ConstReference  operator*() const;
    ConstPointer    operator->() const;
    ConstIterator&  operator+=(difference_type n);
    ConstIterator   operator+(difference_type n) const;
    ConstIterator&  operator-=(difference_type n);
    ConstIterator   operator-(difference_type n) const;
    difference_type operator-(const ConstIterator& it) const;
};

// =========================================================================================================================================

} // namespace fty::db
