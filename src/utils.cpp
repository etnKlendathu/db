#include "fty/db/utils.h"

namespace fty::db {

Error::Code Error::code() const
{
    return m_code;
}

Error::Error(Code code, const std::string& message):
    std::runtime_error(message)
    , m_code(code)
{
}

Error::Error(const std::string& message)
    : std::runtime_error(message)
    , m_code(Code::Error)
{
}

} // namespace fty::db
