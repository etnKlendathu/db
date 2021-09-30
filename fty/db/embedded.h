#pragma once
#include <fty/expected.h>

namespace fty::db {

// =====================================================================================================================

class Embedded
{
public:
    static Expected<void> init(const std::string& db);
    static Embedded&      instance();
    static void           destroy();

private:
    Expected<std::string> create(const std::string& db);

private:
    std::string m_path;
    bool        m_inited = false;
};


} // namespace fty::db
