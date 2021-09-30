#include "bind.h"
#include <memory.h>

namespace fty::db {

Bind::Bind(MYSQL_BIND* data):
    m_data(data)
{
    memset(m_data, 0, sizeof(MYSQL_BIND));
}

// =========================================================================================================================================

void Binds::resize(size_t size)
{
    m_data.resize(size);
    for(auto& it: m_data) {
        m_binds.emplace_back(&it);
    }
}

Binds::~Binds()
{
}

Bind& Binds::operator[](size_t index)
{
    return m_binds[index];
}

MYSQL_BIND* Binds::data() const
{
    return const_cast<MYSQL_BIND*>(m_data.data());
}

// =========================================================================================================================================

}
