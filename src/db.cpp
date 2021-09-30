#include "fty/db.h"
//#include <tntdb.h>

#if 0

namespace fty::db {

static std::string dbpath()
{
    return fmt::format("mysql:db=box_utf8;user={};password={}",
        getenv("DB_USER") == nullptr ? "root" : getenv("DB_USER"),
        getenv("DB_PASSWD") == nullptr ? "" : getenv("DB_PASSWD")
    );
}

// =====================================================================================================================


// =====================================================================================================================

void shutdown()
{
    tntdb::dropCached();
}

// =====================================================================================================================

struct Row::Impl
{
    explicit Impl(const tntdb::Row& row)
        : m_row(row)
    {
    }

    tntdb::Row m_row;
};

// =====================================================================================================================

struct Rows::Impl
{
    explicit Impl(const tntdb::Result& rows)
        : m_rows(rows)
    {
    }

    tntdb::Result m_rows;
};

// =====================================================================================================================

struct Statement::Impl
{
    explicit Impl(const tntdb::Statement& st)
        : m_st(st)
    {
    }

    mutable tntdb::Statement m_st;
};

// =====================================================================================================================


// =====================================================================================================================

struct Transaction::Impl
{
    explicit Impl(tntdb::Connection& tr)
        : m_trans(tr)
    {
    }

    tntdb::Transaction m_trans;
};

// =====================================================================================================================


// =====================================================================================================================

Statement::Statement(std::unique_ptr<Statement::Impl> impl)
    : m_impl(std::move(impl))
{
}

Statement::~Statement()
{
}

Row Statement::selectRow() const
{
    try {
        return Row(std::make_shared<Row::Impl>(m_impl->m_st.selectRow()));
    } catch (const tntdb::NotFound& e) {
        return unexpected<Error>({Error::Code::NotFound, e.what()});
    } catch (const tntdb::NullValue& e) {
        return unexpected<Error>({Error::Code::NullValue, e.what()});
    } catch (const tntdb::TypeError& e) {
        return unexpected<Error>({Error::Code::TypeError, e.what()});
    } catch (const tntdb::SqlError& e) {
        return unexpected<Error>({Error::Code::SqlError, e.what()});
    } catch (const tntdb::FieldNotFound& e) {
        return unexpected<Error>({Error::Code::FieldNotFound, e.what()});
    } catch (const tntdb::Error& e) {
        return unexpected<Error>({Error::Code::Error, e.what()});
    } catch (const std::exception& e) {
        return unexpected<Error>({Error::Code::Error, e.what()});
    }
}

Expected<Rows, Error> Statement::select() const
{
    try {
        return Rows(std::make_shared<Rows::Impl>(m_impl->m_st.select()));
    } catch (const tntdb::NotFound& e) {
        return unexpected<Error>({Error::Code::NotFound, e.what()});
    } catch (const tntdb::NullValue& e) {
        return unexpected<Error>({Error::Code::NullValue, e.what()});
    } catch (const tntdb::TypeError& e) {
        return unexpected<Error>({Error::Code::TypeError, e.what()});
    } catch (const tntdb::SqlError& e) {
        return unexpected<Error>({Error::Code::SqlError, e.what()});
    } catch (const tntdb::FieldNotFound& e) {
        return unexpected<Error>({Error::Code::FieldNotFound, e.what()});
    } catch (const tntdb::Error& e) {
        return unexpected<Error>({Error::Code::Error, e.what()});
    } catch (const std::exception& e) {
        return unexpected<Error>({Error::Code::Error, e.what()});
    }
}

Expected<uint, Error> Statement::execute() const
{
    try {
        return m_impl->m_st.execute();
    } catch (const tntdb::NotFound& e) {
        return unexpected<Error>({Error::Code::NotFound, e.what()});
    } catch (const tntdb::NullValue& e) {
        return unexpected<Error>({Error::Code::NullValue, e.what()});
    } catch (const tntdb::TypeError& e) {
        return unexpected<Error>({Error::Code::TypeError, e.what()});
    } catch (const tntdb::SqlError& e) {
        return unexpected<Error>({Error::Code::SqlError, e.what()});
    } catch (const tntdb::FieldNotFound& e) {
        return unexpected<Error>({Error::Code::FieldNotFound, e.what()});
    } catch (const tntdb::Error& e) {
        return unexpected<Error>({Error::Code::Error, e.what()});
    } catch (const std::exception& e) {
        return unexpected<Error>({Error::Code::Error, e.what()});
    }
}

Statement& Statement::bind()
{
    return *this;
}

void Statement::set(const std::string& name, const std::string& val)
{
    m_impl->m_st.set(name, val);
}

void Statement::set(const std::string& name, bool val)
{
    m_impl->m_st.set(name, val);
}

void Statement::set(const std::string& name, int8_t val)
{
    m_impl->m_st.set(name, val);
}

void Statement::set(const std::string& name, uint8_t val)
{
    m_impl->m_st.set(name, val);
}


void Statement::set(const std::string& name, int16_t val)
{
    m_impl->m_st.set(name, val);
}

void Statement::set(const std::string& name, uint16_t val)
{
    m_impl->m_st.set(name, val);
}

void Statement::set(const std::string& name, int32_t val)
{
    m_impl->m_st.set(name, val);
}

void Statement::set(const std::string& name, uint32_t val)
{
    m_impl->m_st.set(name, val);
}

void Statement::set(const std::string& name, int64_t val)
{
    m_impl->m_st.set(name, val);
}

void Statement::set(const std::string& name, uint64_t val)
{
    m_impl->m_st.set(name, val);
}

void Statement::setNull(const std::string& name)
{
    m_impl->m_st.setNull(name);
}

void Statement::set(const std::string& name, float val)
{
    m_impl->m_st.set(name, val);
}

void Statement::set(const std::string& name, double val)
{
    m_impl->m_st.set(name, val);
}

// =====================================================================================================================

Row::Row(std::shared_ptr<Impl> impl)
    : m_impl(impl)
{
}

Row::~Row()
{
}

std::string Row::getString(const std::string& name, const std::string& defVal) const
{
    try {
        return m_impl->m_row.getString(name);
    } catch (const tntdb::NullValue& /* e*/) {
        return defVal;
    }
}

bool Row::getBool(const std::string& name, bool defVal) const
{
    try {
        return m_impl->m_row.getBool(name);
    } catch (const tntdb::NullValue& /* e*/) {
        return defVal;
    }
}

int8_t Row::getInt8(const std::string& name, int8_t defVal) const
{
    try {
        return int8_t(m_impl->m_row.getInt(name));
    } catch (const tntdb::NullValue& /* e*/) {
        return defVal;
    }
}

uint8_t Row::getUint8(const std::string& name, uint8_t defVal) const
{
    try {
        return uint8_t(m_impl->m_row.getUnsigned(name));
    } catch (const tntdb::NullValue& /* e*/) {
        return defVal;
    }
}

int16_t Row::getInt16(const std::string& name, int16_t defVal) const
{
    try {
        return int16_t(m_impl->m_row.getUnsigned(name));
    } catch (const tntdb::NullValue& /* e*/) {
        return defVal;
    }
}

uint16_t Row::getUint16(const std::string& name, uint16_t defVal) const
{
    try {
        return uint16_t(m_impl->m_row.getUnsigned(name));
    } catch (const tntdb::NullValue& /* e*/) {
        return defVal;
    }
}

int32_t Row::getInt32(const std::string& name, int32_t defVal) const
{
    try {
        return m_impl->m_row.getInt(name);
    } catch (const tntdb::NullValue& /* e*/) {
        return defVal;
    }
}

uint32_t Row::getUint32(const std::string& name, uint32_t defVal) const
{
    try {
        return m_impl->m_row.getUnsigned(name);
    } catch (const tntdb::NullValue& /* e*/) {
        return defVal;
    }
}

int64_t Row::getInt64(const std::string& name, int64_t defVal) const
{
    try {
        return m_impl->m_row.getInt64(name);
    } catch (const tntdb::NullValue& /* e*/) {
        return defVal;
    }
}

uint64_t Row::getUint64(const std::string& name, uint64_t defVal) const
{
    try {
        return m_impl->m_row.getUnsigned64(name);
    } catch (const tntdb::NullValue& /* e*/) {
        return defVal;
    }
}

float Row::getFloat(const std::string& name, float defVal) const
{
    try {
        return m_impl->m_row.getFloat(name);
    } catch (const tntdb::NullValue& /* e*/) {
        return defVal;
    }
}

double Row::getDouble(const std::string& name, double defVal) const
{
    try {
        return m_impl->m_row.getDouble(name);
    } catch (const tntdb::NullValue& /* e*/) {
        return defVal;
    }
}

bool Row::isNull(const std::string& name) const
{
    try {
        return m_impl->m_row.isNull(name);
    } catch (const tntdb::NullValue& /* e*/) {
        return {};
    }
}

std::string Row::get(const std::string& col, const std::string& defVal) const
{
    if (isNull(col)) {
        return defVal;
    } else {
        return getString(col, defVal);
    }
}

// =====================================================================================================================
// Rows iterator impl
// =====================================================================================================================

void ConstIterator::setOffset(size_t off)
{
    if (off != m_offset) {
        m_offset = off;
        if (m_offset < m_rows.m_impl->m_rows.size()) {
            m_current = std::make_shared<Row::Impl>(m_rows.m_impl->m_rows.getRow(unsigned(m_offset)));
        }
    }
}

ConstIterator::ConstIterator(const Rows& r, size_t off)
    : m_rows(r)
    , m_offset(off)
{
    if (m_offset < r.m_impl->m_rows.size()) {
        m_current = std::make_shared<Row::Impl>(r.m_impl->m_rows.getRow(unsigned(m_offset)));
    }
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

// =====================================================================================================================
// Rows impl
// =====================================================================================================================

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
    return m_impl->m_rows.size();
}

bool Rows::empty() const
{
    return m_impl->m_rows.empty();
}

Row Rows::operator[](size_t off) const
{
    return Row(std::make_unique<Row::Impl>(m_impl->m_rows.getRow(unsigned(off))));
}

Rows::Rows(std::shared_ptr<Impl> impl)
    : m_impl(impl)
{
}

Rows::Rows()
{
}

// =====================================================================================================================
// Transaction impl
// =====================================================================================================================

Transaction::Transaction(Connection& con)
    : m_impl(std::make_unique<Impl>(con.m_impl->m_connection))
{
}

Transaction::~Transaction()
{
}

void Transaction::commit()
{
    m_impl->m_trans.commit();
}

void Transaction::rollback()
{
    m_impl->m_trans.rollback();
}

// =====================================================================================================================

} // namespace fty::db
#endif
