#include "fty/db/statement.h"
#include "private/statement.h"

namespace fty::db {

Statement::Statement(const std::shared_ptr<StatementImpl>& impl):
    m_impl(std::move(impl))
{
}

Statement::~Statement()
{
}

Statement& Statement::bind()
{
    return *this;
}

uint64_t Statement::execute() const
{
    return m_impl->execute();
}

Rows Statement::select() const
{
    return m_impl->select();
}

void Statement::set(const std::string& name, const std::string& val)
{
    m_impl->set(name, val);
}

void Statement::set(const std::string& name, bool val)
{
    m_impl->set(name, val);
}

void Statement::set(const std::string& name, int8_t val)
{
    m_impl->set(name, val);
}

void Statement::set(const std::string& name, uint8_t val)
{
    m_impl->set(name, val);
}

void Statement::set(const std::string& name, int16_t val)
{
    m_impl->set(name, val);
}

void Statement::set(const std::string& name, uint16_t val)
{
    m_impl->set(name, val);
}

void Statement::set(const std::string& name, int32_t val)
{
    m_impl->set(name, val);
}

void Statement::set(const std::string& name, uint32_t val)
{
    m_impl->set(name, val);
}

void Statement::set(const std::string& name, int64_t val)
{
    m_impl->set(name, val);
}

void Statement::set(const std::string& name, uint64_t val)
{
    m_impl->set(name, val);
}

void Statement::set(const std::string& name, float val)
{
    m_impl->set(name, val);
}

void Statement::set(const std::string& name, double val)
{
    m_impl->set(name, val);
}

void Statement::setNull(const std::string& name)
{
    m_impl->set(name, nullptr);
}


}
