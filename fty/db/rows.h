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
    friend class EmptyStatement;
};

// =========================================================================================================================================

class ConstIterator : public std::iterator<std::forward_iterator_tag, Row>
{
public:
    using ConstReference = const value_type&;
    using ConstPointer   = const value_type*;

private:
    Rows   m_rows;
    Row    m_current;

public:
    ConstIterator(const Rows& rows);
    bool            operator==(const ConstIterator& it) const;
    bool            operator!=(const ConstIterator& it) const;
    ConstIterator&  operator++();
    ConstReference  operator*() const;
    ConstPointer    operator->() const;
};

// =========================================================================================================================================

} // namespace fty::db
